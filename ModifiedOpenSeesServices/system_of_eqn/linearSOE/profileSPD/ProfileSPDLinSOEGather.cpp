/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision$
// $Date$
// $Source$

// Written: fmk
// Revision: A
//
// Description: This file contains the implementation for ProfileSPDLinSOEGather


#include <ProfileSPDLinSOEGather.h>
#include <ProfileSPDLinSolverGather.h>
#include <Matrix.h>
#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <f2c.h>
#include <math.h>

#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <Channel.h>
#include <iostream>
using namespace std;


ProfileSPDLinSOEGather::ProfileSPDLinSOEGather(ProfileSPDLinSolverGather& the_Solver,
        int nChannel, Channel** theCs)
    : LinearSOE(the_Solver, LinSOE_TAGS_ProfileSPDLinSOEGather),
      size(0), profileSize(0), A(0), B(0), X(0), vectX(0), vectB(0),
      iDiagLoc(0), Asize(0), Bsize(0), isAfactored(false), isAcondensed(false),
      numInt(0) , work(0)
{
    the_Solver.setLinearSOE(*this);

    numChannel = nChannel;

    if (numChannel > 0)
    {
        theChannels = new Channel *[numChannel];

        if (theChannels == 0)
        {
            cerr << "ProfileSPDLinSOEGather::ProfileSPDLinSOEGather() - out of memory\n";
            numChannel = 0;
        }

        for (int i = 0; i < numChannel; i++)
        {
            theChannels[i] = theCs[i];
        }
    }
}


ProfileSPDLinSOEGather::~ProfileSPDLinSOEGather()
{
    if (A != 0)
    {
        delete [] A;
    }

    if (B != 0)
    {
        delete [] B;
    }

    if (X != 0)
    {
        delete [] X;
    }

    if (iDiagLoc != 0)
    {
        delete [] iDiagLoc;
    }

    if (vectX != 0)
    {
        delete vectX;
    }

    if (vectB != 0)
    {
        delete vectB;
    }

    if (theChannels != 0)
    {
        delete [] theChannels;
    }

    if (work != 0)
    {
        delete [] theChannels;
    }
}


int
ProfileSPDLinSOEGather::getNumEqn(void) const
{
    return size;
}

int
ProfileSPDLinSOEGather::setSize(Graph& theGraph)
{
    if (numChannel > 1)
    {

        // from each part receive it's graph
        for (int i = 0; i < numChannel; i++)
        {
            Graph theSubGraph;
            theSubGraph.recvSelf(0, *theChannels[i], theBroker);
            theGraph.merge(theSubGraph);
        }

        // from each part receive it's graph
        for (int i = 0; i < numChannel; i++)
        {
            theGraph.sendSelf(0, *theChannels[i]);
        }
    }
    else if (numChannel == 1)
    {
        // send the small subdomain graph
        theGraph.sendSelf(0, *theChannels[0]);

        // receive the larger graph
        theGraph.recvSelf(0, *theChannels[0], theBroker);
    }


    // now we setSie as per original
    int oldSize = size;
    int result = 0;
    size = theGraph.getNumVertex();

    // check we have enough space in iDiagLoc and iLastCol
    // if not delete old and create new
    if (size > Bsize)
    {
        if (iDiagLoc != 0)
        {
            delete [] iDiagLoc;
        }

        iDiagLoc = new int[size];

        if (iDiagLoc == 0)
        {
            cerr << "WARNING ProfileSPDLinSOEGather::setSize() : ";
            cerr << " - ran out of memory for iDiagLoc\n";
            size = 0;
            Asize = 0;
            result = -1;
        }
    }

    // zero out iDiagLoc
    for (int i = 0; i < size; i++)
    {
        iDiagLoc[i] = 0;
    }

    // now we go through the vertices to find the height of each col and
    // width of each row from the connectivity information.

    Vertex* vertexPtr;
    VertexIter& theVertices = theGraph.getVertices();

    while ((vertexPtr = theVertices()) != 0)
    {
        int vertexNum = vertexPtr->getTag();
        const ID& theAdjacency = vertexPtr->getAdjacency();
        int iiDiagLoc = iDiagLoc[vertexNum];
        int* iiDiagLocPtr = &(iDiagLoc[vertexNum]);

        for (int i = 0; i < theAdjacency.Size(); i++)
        {
            int otherNum = theAdjacency(i);
            int diff = vertexNum - otherNum;

            if (diff > 0)
            {
                if (iiDiagLoc < diff)
                {
                    iiDiagLoc = diff;
                    *iiDiagLocPtr = diff;
                }
            }
        }
    }


    // now go through iDiagLoc, adding 1 for the diagonal element
    // and then adding previous entry to give current location.
    if (iDiagLoc != 0)
    {
        iDiagLoc[0] = 1;    // NOTE FORTRAN ARRAY LOCATION
    }

    for (int j = 1; j < size; j++)
    {
        iDiagLoc[j] = iDiagLoc[j] + 1 + iDiagLoc[j - 1];
    }

    if (iDiagLoc != 0)
    {
        profileSize = iDiagLoc[size - 1];
    }

    // check if we need more space to hold A
    // if so then go get it
    if (profileSize > Asize)
    {

        // delete old space
        if (A != 0)
        {
            delete [] A;
        }

        if (work != 0)
        {
            delete [] work;
        }

        // get new space
        A = new double[profileSize];
        work = new double[profileSize];

        if (A == 0)
        {
            cerr << "ProfileSPDLinSOEGather::ProfileSPDLinSOEGather :";
            cerr << " ran out of memory for A (size,Profile) (";
            cerr << size << ", " << profileSize << ") \n";
            size = 0;
            Asize = 0;
            profileSize = 0;
            result = -1;
        }
        else
        {
            Asize = profileSize;
        }
    }

    // zero the matrix
    for (int k = 0; k < profileSize; k++)
    {
        A[k] = 0;
    }

    isAfactored = false;
    isAcondensed = false;

    if (size > Bsize)   // we have to get another space for A
    {

        // delete the old
        if (B != 0)
        {
            delete [] B;
        }

        if (X != 0)
        {
            delete [] X;
        }

        // create the new
        B = new double[size];
        X = new double[size];

        if (B == 0 || X == 0 )
        {
            cerr << "ProfileSPDLinSOEGather::ProfileSPDLinSOEGather :";
            cerr << " ran out of memory for vectors (size) (";
            cerr << size << ") \n";
            size = 0;
            Bsize = 0;
            result = -1;
        }
    }

    // zero the vectors
    for (int l = 0; l < size; l++)
    {
        B[l] = 0;
        X[l] = 0;
    }

    if (size != oldSize)
    {

        if (vectX != 0)
        {
            delete vectX;
        }

        if (vectB != 0)
        {
            delete vectB;
        }

        vectX = new Vector(X, size);
        vectB = new Vector(B, size);

        if (size > Bsize)
        {
            Bsize = size;
        }
    }

    // invoke setSize() on the Solver
    LinearSOESolver* the_Solver = this->getSolver();
    int solverOK = the_Solver->setSize();

    if (solverOK < 0)
    {
        cerr << "WARNING ProfileSPDLinSOEGather::setSize :";
        cerr << " solver failed setSize()\n";
        return solverOK;
    }

    return result;
}


int
ProfileSPDLinSOEGather::solve(void)
{
    static ID result(1);

    Vector dataA(work, profileSize);

    if (numChannel > 1)
    {

        // if main working process:
        //   1. receive contribution to A matrix from all sub process
        //   2. receive contribution to B vector from all sub process
        //   3. solve total system
        //   4. return B vector to all sub processes

        Vector dataB(work, size);
        Vector dataX(work, size);

        // from each part receive it's graph
        for (int i = 0; i < numChannel; i++)
        {

            // if not factored, recv the remote contribution to A
            if (isAfactored == false)
            {
                theChannels[i]->recvVector(0, 0, dataA);

                for (int j = 0; j < profileSize; j++)
                {
                    A[j] += work[j];
                }
            }

            // recv the remote contribution to B
            theChannels[i]->recvVector(0, 0, dataB);

            for (int k = 0; k < profileSize; k++)
            {
                B[k] += work[k];
            }
        }

        // solve the system
        result[0] = this->LinearSOE::solve();

        // send the results (X) back to remotes
        for (int i = 0; i < numChannel; i++)
        {
            theChannels[i]->sendID(0, 0, result);

            if (result[0] == 0)
            {
                theChannels[i]->sendVector(0, 0, dataX);
            }
        }
    }

    else if (numChannel == 1)
    {

        // if sub process:
        //   1. send contribution to A matrix to main
        //   2. send contribution to B vector to main
        //   4. receive B vector from main

        if (isAfactored == false)
        {
            Vector dataA(A, profileSize);
            theChannels[0]->sendVector(0, 0, dataA);
        }

        theChannels[0]->sendVector(0, 0, *vectB);
        theChannels[0]->recvID(0, 0, result);

        if (result[0] == 0)
        {
            theChannels[0]->recvVector(0, 0, *vectX);
        }
    }


    return result[0];
}

int
ProfileSPDLinSOEGather::addA(const Matrix& m, const ID& id, double fact)
{
    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }

    // check that m and id are of similar size
    int idSize = id.Size();

    if (idSize != m.noRows() && idSize != m.noCols())
    {
        cerr << "FullGenLinSOE::addA()	- Matrix and ID not of similar sizes\n";
        return -1;
    }

    if (fact == 1.0)   // do not need to multiply
    {
        for (int i = 0; i < idSize; i++)
        {
            int col = id(i);

            if (col < size && col >= 0)
            {
                double* coliiPtr = &A[iDiagLoc[col] - 1]; // -1 as fortran indexing
                int minColRow;

                if (col == 0)
                {
                    minColRow = 0;
                }
                else
                {
                    minColRow = col - (iDiagLoc[col] - iDiagLoc[col - 1]) + 1;
                }

                for (int j = 0; j < idSize; j++)
                {
                    int row = id(j);

                    if (row < size && row >= 0 &&
                            row <= col && row >= minColRow)
                    {

                        // we only add upper and inside profile
                        double* APtr = coliiPtr + (row - col);
                        *APtr += m(j, i);
                    }
                }  // for j
            }
        }  // for i
    }
    else
    {
        for (int i = 0; i < idSize; i++)
        {
            int col = id(i);

            if (col < size && col >= 0)
            {
                double* coliiPtr = &A[iDiagLoc[col] - 1]; // -1 as fortran indexing
                int minColRow;

                if (col == 0)
                {
                    minColRow = 0;
                }
                else
                {
                    minColRow = col - (iDiagLoc[col] - iDiagLoc[col - 1]) + 1;
                }

                for (int j = 0; j < idSize; j++)
                {
                    int row = id(j);

                    if (row < size && row >= 0 &&
                            row <= col && row >= minColRow)
                    {

                        // we only add upper and inside profile
                        double* APtr = coliiPtr + (row - col);
                        *APtr += m(j, i) * fact;
                    }
                }  // for j
            }
        }  // for i

    }

    return 0;
}


int
ProfileSPDLinSOEGather::addB(const Vector& v, const ID& id, double fact)
{

    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }

    // check that m and id are of similar size
    int idSize = id.Size();

    if (idSize != v.Size() )
    {
        cerr << "ProfileSPDLinSOEGather::addB() -";
        cerr << " Vector and ID not of similar sizes\n";
        return -1;
    }

    if (fact == 1.0)   // do not need to multiply if fact == 1.0
    {
        for (int i = 0; i < id.Size(); i++)
        {
            int pos = id(i);

            if (pos < size && pos >= 0)
            {
                B[pos] += v(i);
            }
        }
    }
    else if (fact == -1.0)     // do not need to multiply if fact == -1.0
    {
        for (int i = 0; i < id.Size(); i++)
        {
            int pos = id(i);

            if (pos < size && pos >= 0)
            {
                B[pos] -= v(i);
            }
        }
    }
    else
    {
        for (int i = 0; i < id.Size(); i++)
        {
            int pos = id(i);

            if (pos < size && pos >= 0)
            {
                B[pos] += v(i) * fact;
            }
        }
    }

    return 0;
}


int
ProfileSPDLinSOEGather::setB(const Vector& v, double fact)
{
    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }


    if (v.Size() != size)
    {
        cerr << "WARNING BandGenLinSOE::setB() -";
        cerr << " incomptable sizes " << size << " and " << v.Size() << endln;
        return -1;
    }

    if (fact == 1.0)   // do not need to multiply if fact == 1.0
    {
        for (int i = 0; i < size; i++)
        {
            B[i] = v(i);
        }
    }
    else if (fact == -1.0)
    {
        for (int i = 0; i < size; i++)
        {
            B[i] = -v(i);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            B[i] = v(i) * fact;
        }
    }

    return 0;
}

void
ProfileSPDLinSOEGather::zeroA(void)
{
    double* Aptr = A;

    for (int i = 0; i < profileSize; i++)
    {
        *Aptr++ = 0;
    }

    isAfactored = false;
}

void
ProfileSPDLinSOEGather::zeroB(void)
{
    double* Bptr = B;

    for (int i = 0; i < size; i++)
    {
        *Bptr++ = 0;
    }
}


void
ProfileSPDLinSOEGather::setX(int loc, double value)
{
    if (loc < size && loc >= 0)
    {
        X[loc] = value;
    }
}

void
ProfileSPDLinSOEGather::setX(const Vector& x)
{
    if (x.Size() == size && vectX != 0)
    {
        *vectX = x;
    }
}

const Vector&
ProfileSPDLinSOEGather::getX(void)
{
    if (vectX == 0)
    {
        cerr << "FATAL ProfileSPDLinSOEGather::getX - vectX == 0";
        exit(-1);
    }

    return *vectX;
}

const Vector&
ProfileSPDLinSOEGather::getB(void)
{
    if (vectB == 0)
    {
        cerr << "FATAL ProfileSPDLinSOEGather::getB - vectB == 0";
        exit(-1);
    }

    return *vectB;
}



double
ProfileSPDLinSOEGather::normRHS(void)
{
    double norm = 0.0;

    for (int i = 0; i < size; i++)
    {
        double Yi = B[i];
        norm += Yi * Yi;
    }

    return sqrt(norm);

}


int
ProfileSPDLinSOEGather::setProfileSPDSolver(ProfileSPDLinSolverGather& newSolver)
{
    newSolver.setLinearSOE(*this);

    if (size != 0)
    {
        int solverOK = newSolver.setSize();

        if (solverOK < 0)
        {
            cerr << "WARNING:ProfileSPDLinSOEGather::setSolver :";
            cerr << "the new solver could not setSeize() - staying with old\n";
            return -1;
        }
    }

    return this->setSolver(newSolver);
}


int
ProfileSPDLinSOEGather::sendSelf(int cTag, Channel& theChannel)
{
    return 0;
}


int
ProfileSPDLinSOEGather::recvSelf(int cTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}
