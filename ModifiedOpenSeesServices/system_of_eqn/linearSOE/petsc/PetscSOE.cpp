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

// $Revision: 1.4 $
// $Date: 2005/05/18 19:24:49 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/petsc/PetscSOE.cpp,v $

// Written: fmk & om
// Created: 7/98
// Revision: A
//
// Description: This file contains the implementation for PetscSOE


#include "PetscSOE.h"
#include "PetscSolver.h"
#include <petscvec.h>
#include <Matrix.h>
#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <f2c.h>
#include <math.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <petscpc.h>


using namespace std;




//Guanzhou
PetscSOE::PetscSOE(PetscSolver &theSOESolver, MatType matType, int bs)
    : LinearSOE(theSOESolver, LinSOE_TAGS_PetscSOE),
      isFactored(0), size(0), processID(0), numProcesses(0), B(0), X(0),
      indices(0), vectX(0), vectB(0), A(0), M(0),  x(0), b(0), blockSize(bs),
      numChannels(0), theChannels(0),  localCol(0), mType(matType)
{
    theSOESolver.setLinearSOE(*this);
    init_done = false;//GZ added
}

int
PetscSOE::getNumEqn(void) const
{
    return size;
}

PetscSOE::~PetscSOE()
{
    cout << "PetscSOE::~PetscSOE()\n";
    if (theChannels != 0)
    {
        delete [] theChannels;
    }

    if (localCol != 0)
        for (int i = 0; i < numChannels; i++)
            if (localCol[i] != 0)
            {
                delete localCol[i];
            }

    delete [] localCol;

    if (vectX != 0)
    {
        delete vectX;
    }

    if (vectB != 0)
    {
        delete vectB;
    }

    if (B != 0)
    {
        delete [] B;
    }

    if (X != 0)
    {
        delete [] X;
    }

    // invoke the petsc destructors
    if (A != 0)
    {
        MatDestroy(&A);
    }

    if (M != 0)
    {
        MatDestroy(&M);
    } ;//Added by Babak to add mass Matrices ... 1/14/13

    if (b != 0)
    {
        VecDestroy(&b);
    }

    if (x != 0)
    {
        VecDestroy(&x);
    }

    //PetscLogPrintSummary(PETSC_COMM_SELF, (char *)0);

    PetscFinalize();
    //   PetscEnd(); //Added by Babak to clean up the stuff related to petsc ... on September 2012

}


//Reimplemented by Babak 6/4/13:
//-------
int
PetscSOE::setSize(int MaxDOFtag)
{
    if (!init_done)
    {
        PetscInitialize(0, PETSC_NULL, (char *)0, PETSC_NULL);
        MPI_Comm_size(PETSC_COMM_WORLD, &numProcesses);
        MPI_Comm_rank(PETSC_COMM_WORLD, &processID);
        init_done = true;
    }

    //PetscLogInfoAllow(PETSC_TRUE, (char *)0);

    int result = 0;
    int ierr = 0;

    //
    // first determine system size
    //

    if (numProcesses == 1)
    {

        // if single process, the system size is size of graph
        size = MaxDOFtag;
        isFactored = 0;

    }
    else
    {

        // first determine local max
        size = MaxDOFtag;
        isFactored = 0;
        static ID data(1);

        // all local max's sent to P0 which determines the max
        // and informs all others

        if (processID != 0)
        {
            Channel *theChannel = theChannels[0];

            data(0) = size;
            theChannel->sendID(0, 0, data);
            theChannel->receiveID(0, 0, data);

            size = data(0);
        }
        else
        {

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->receiveID(0, 0, data);

                if (data(0) > size)
                {
                    size = data(0);
                }
            }

            data(0) = size;

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->sendID(0, 0, data);
            }
        }

        size = size + 1; // vertices numbered 0 through n-1
    }



    // invoke the petsc destructors
    if (A != 0)
    {
        MatDestroy(&A);
    }

    if (b != 0)
    {
        VecDestroy(&b);
    }

    if (x != 0)
    {
        VecDestroy(&x);
    }

    //
    // now we create the opensees vector objects
    //

    // delete the old vectors
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

    if (B == 0 || X == 0)
    {
        cerr << "WARNING PetscSOE::PetscSOE :";
        cerr << " ran out of memory for vectors (size) (";
        cerr << size << ") \n";
        size = 0;
        result = -1;
    }

    // zero the vectors
    for (int j = 0; j < size; j++)
    {
        B[j] = 0;
        X[j] = 0;
    }

    if (vectX != 0)
    {
        delete vectX;
    }

    if (vectB != 0)
    {
        delete vectB;
    }

    vectX = new Vector(X, size);

    //Added by Babak on 5/31/13
    //-------
    if (vectX == NULL)
    {
        std::cerr << "PetscSOE::PetscSOE : can not allocate memory for vectX \n";
    }

    //-------
    vectB = new Vector(B, size);

    //Added by Babak on 5/31/13
    //-------
    if (vectB == NULL)
    {
        std::cerr << "PetscSOE::PetscSOE : can not allocate memory for vectB \n";
    }

    //-------


    //
    // now create petsc matrix and vector objects
    //

    if (numProcesses == 1)
    {

        //       // we determine the number of non-zeros & number of nonzero's
        //       // in each row of A
        //       int *rowA = new int[size];  // will contain no of non-zero entries
        //       // in each row
        //
        //       int NNZ = 0;
        //       for (int a=0; a<size; a++)
        //       {
        //  Vertex *theVertex = theGraph.getVertexPtr(a);
        //  if (theVertex == 0)
        //  {
        //    cerr << "WARNING:PetscSOE::setSize :";
        //    cerr << " vertex " << a << " not in graph! - size set to 0\n";
        //    size = 0;
        //    return -1;
        //  }
        //
        //  const ID &theAdjacency = theVertex->getAdjacency();
        //  int idSize = theAdjacency.Size();
        //
        //  NNZ += idSize +1;
        //  rowA[a] = idSize +1;  // +1 for the diagonal entry
        //       }
        //
        //       //
        //       // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
        //       //
        //
        //       //      ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);
        //
        //       if (blockSize == 1)
        //       {
        //  ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, size, size, 0, rowA, &A); CHKERRQ(ierr);
        //       }
        //       else
        //       {
        //  ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF, blockSize, size,size, 0, rowA, &A); CHKERRQ(ierr);
        //       }
        //
        //       ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, X, &x); CHKERRQ(ierr);
        //       ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, B, &b); CHKERRQ(ierr);
        //
        //       // invoke setSize() on the Solver
        //       LinearSOESolver *tSolver = this->getSolver();
        //       int solverOK = tSolver->setSize();
        //       if (solverOK < 0)
        //       {
        //  cerr << "WARNING:PetscSOE::setSize :";
        //  cerr << " solver failed setSize()\n";
        //  return solverOK;
        //       }
        //
        //       // clear up the memory we used
        //       delete [] rowA;


    }
    else
    {

        //
        // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
        //
        //

        //Guanzhou changed interface to petsc_2.3.0
        PetscOptionsGetInt(PETSC_NULL, "-n", &size, PETSC_NULL);

        //ICC MatCreate(PETSC_COMM_WORLD,&A); //ICC
        //ICC MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,size,size); //ICC
        //ICC MatSetType(A, MATMPIROWBS); //ICC

        //MatCreateMPIAIJ(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE, //NONE
        //    size, size, 99, PETSC_NULL, 99, PETSC_NULL, &A);    //NONE

        ierr = MatCreate(PETSC_COMM_WORLD, &A);
        CHKERRQ(ierr);
        ierr = MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, size, size);
        CHKERRQ(ierr);
        //       ierr = MatSetType(A,mType);CHKERRQ(ierr);
        //      ierr = MatSetType(A,MATAIJ);CHKERRQ(ierr);
        ierr = MatSetFromOptions(A);
        CHKERRQ(ierr);

        //MatSetOption(A, MAT_SYMMETRIC);
        //MatSetOption(A, MAT_SYMMETRY_ETERNAL);

        //ICC MatSetFromOptions(A); //ICC
        //Added by Babak 7/11/13
        //------
        //       MatSetUp(A);
        ierr = MatMPIAIJSetPreallocation(A, 650, PETSC_NULL, 650, PETSC_NULL);
        CHKERRQ(ierr);
        ierr = MatSeqAIJSetPreallocation(A, 650 , PETSC_NULL);
        CHKERRQ(ierr);

        //------

        MatGetOwnershipRange(A, &startRow, &endRow);

        //ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);
        //ierr = MatCreate(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE,size, size, &A); CHKERRQ(ierr);
        //ierr = MatSetFromOptions(A);CHKERRQ(ierr);
        //ierr = MatGetOwnershipRange(A, &startRow, &endRow);CHKERRQ(ierr);

        //out by babak to be compatible with petsc 3.3 replaced 7/11/13
        //-----
        //       ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, endRow-startRow, size, &X[startRow],  &x); CHKERRQ(ierr);
        //       ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, endRow-startRow, size, &B[startRow],  &b); CHKERRQ(ierr);
        //-----
        ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, blockSize, endRow - startRow , size, &X[startRow], &x);
        CHKERRQ(ierr);
        ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, blockSize, endRow - startRow , size, &B[startRow], &b);
        CHKERRQ(ierr);


        // invoke setSize() on the Solver
        LinearSOESolver *tSolver = this->getSolver();
        int solverOK = tSolver->setSize();

        if (solverOK < 0)
        {
            cerr << "WARNING:PetscSOE::setSize :";
            cerr << " solver failed setSize()\n";
            return solverOK;
        }
    }



    return result;
}
//--------


//Out and reimplemented by Babak 6/4/13
/*
int
PetscSOE::setSize(Graph &theGraph)
{
  if (!init_done)
  {
    PetscInitialize(0, PETSC_NULL, (char *)0, PETSC_NULL);
    MPI_Comm_size(PETSC_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(PETSC_COMM_WORLD, &processID);
    init_done = true;
  }

  //PetscLogInfoAllow(PETSC_TRUE, (char *)0);

  int result = 0;
  int ierr = 0;

    //
    // first determine system size
    //

    if (numProcesses == 1)
    {

      // if single process, the system size is size of graph
      size = theGraph.getNumVertex();
      isFactored = 0;

    }
    else
    {

      // first determine local max
      size = 0;
      isFactored = 0;
      VertexIter &theVertices = theGraph.getVertices();
      Vertex *theVertex;
      while ((theVertex = theVertices()) != 0)
      {
    int vertexTag = theVertex->getTag();
    if (vertexTag > size)
      size = vertexTag;

    #ifdef _BABAK_DEBUG
      cerr << "BABAK @ size PetscSOE::setSize ---- PID:" << processID << "Size: "<< size << "\n";
      cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID<< "---raphNumEdge:"<<theGraph.getNumEdge()<<endl;
      cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID<< "---graphgetNumVertex:"<<theGraph.getNumVertex()<<endl;
    #endif
      }

      static ID data(1);

      // all local max's sent to P0 which determines the max
      // and informs all others
    #ifdef _BABAK_DEBUG

      cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID<< "set size is done" <<endl;
    #endif
      if (processID != 0)
      {
    Channel *theChannel = theChannels[0];

    data(0) = size;
    theChannel->sendID(0, 0, data);
    theChannel->receiveID(0, 0, data);

    size = data(0);
      }
      else
      {

    for (int j=0; j<numChannels; j++)
    {
      Channel *theChannel = theChannels[j];
      theChannel->receiveID(0, 0, data);
      if (data(0) > size)
        size = data(0);
    }
    data(0) = size;
    for (int j=0; j<numChannels; j++)
    {
      Channel *theChannel = theChannels[j];
      theChannel->sendID(0, 0, data);
    }
      }
      size = size+1; // vertices numbered 0 through n-1
    }



    // invoke the petsc destructors
    if (A != 0) MatDestroy(&A);
    if (b != 0) VecDestroy(&b);
    if (x != 0) VecDestroy(&x);

    //
    // now we create the opensees vector objects
    //

    // delete the old vectors
    if (B != 0) delete [] B;
    if (X != 0) delete [] X;

    // create the new
    B = new double[size];
    X = new double[size];

    if (B == 0 || X == 0)
    {
      cerr << "WARNING PetscSOE::PetscSOE :";
      cerr << " ran out of memory for vectors (size) (";
      cerr << size << ") \n";
      size = 0;
      result = -1;
    }

    // zero the vectors
    for (int j=0; j<size; j++)
    {
      B[j] = 0;
      X[j] = 0;
    }

    if (vectX != 0)
    {
      delete vectX;
      vectX == NULL;    //Added by Babak on 5/31/13
    }

    if (vectB != 0)
    {
      delete vectB;
      vectB == NULL;     //Added by Babak on 5/31/13
    }

    vectX = new Vector(X, size);
    //Added by Babak on 5/31/13
    //-------
    if(vectX == NULL)
    {
      std::cerr << "PetscSOE::PetscSOE : can not allocate memory for vectX \n";
    }
    //-------
    vectB = new Vector(B, size);
    //Added by Babak on 5/31/13
    //-------
    if(vectB == NULL)
    {
      std::cerr << "PetscSOE::PetscSOE : can not allocate memory for vectB \n";
    }
    //-------


    //
    // now create petsc matrix and vector objects
    //

    if (numProcesses == 1)
    {

      // we determine the number of non-zeros & number of nonzero's
      // in each row of A
      int *rowA = new int[size];  // will contain no of non-zero entries
      // in each row

      int NNZ = 0;
      for (int a=0; a<size; a++)
      {
    Vertex *theVertex = theGraph.getVertexPtr(a);
    if (theVertex == 0)
    {
      cerr << "WARNING:PetscSOE::setSize :";
      cerr << " vertex " << a << " not in graph! - size set to 0\n";
      size = 0;
      return -1;
    }

    const ID &theAdjacency = theVertex->getAdjacency();
    int idSize = theAdjacency.Size();

    NNZ += idSize +1;
    rowA[a] = idSize +1;  // +1 for the diagonal entry
      }

      //
      // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
      //

      //      ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);

      if (blockSize == 1)
      {
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, size, size, 0, rowA, &A); CHKERRQ(ierr);
      }
      else
      {
    ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF, blockSize, size,size, 0, rowA, &A); CHKERRQ(ierr);
      }

      ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, X, &x); CHKERRQ(ierr);
      ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, B, &b); CHKERRQ(ierr);

      // invoke setSize() on the Solver
      LinearSOESolver *tSolver = this->getSolver();
      int solverOK = tSolver->setSize();
      if (solverOK < 0)
      {
    cerr << "WARNING:PetscSOE::setSize :";
    cerr << " solver failed setSize()\n";
    return solverOK;
      }

      // clear up the memory we used
      delete [] rowA;


    }
    else
    {

      //
      // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
      //
      //

      //Guanzhou changed interface to petsc_2.3.0
      PetscOptionsGetInt(PETSC_NULL,"-n",&size,PETSC_NULL);

      //ICC MatCreate(PETSC_COMM_WORLD,&A); //ICC
      //ICC MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,size,size); //ICC
      //ICC MatSetType(A, MATMPIROWBS); //ICC

      //MatCreateMPIAIJ(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE, //NONE
      //    size, size, 99, PETSC_NULL, 99, PETSC_NULL, &A);    //NONE

      ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);
      ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,size,size);CHKERRQ(ierr);
      ierr = MatSetType(A,mType);CHKERRQ(ierr);
      ierr = MatSetFromOptions(A);CHKERRQ(ierr);

      //MatSetOption(A, MAT_SYMMETRIC);
      //MatSetOption(A, MAT_SYMMETRY_ETERNAL);

      //ICC MatSetFromOptions(A); //ICC
      MatGetOwnershipRange(A,&startRow,&endRow);

      //ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);
      //ierr = MatCreate(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE,size, size, &A); CHKERRQ(ierr);
      //ierr = MatSetFromOptions(A);CHKERRQ(ierr);
      //ierr = MatGetOwnershipRange(A, &startRow, &endRow);CHKERRQ(ierr);

      ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, endRow-startRow, size, &X[startRow],  &x); CHKERRQ(ierr);
      ierr = VecCreateMPIWithArray(PETSC_COMM_WORLD, endRow-startRow, size, &B[startRow],  &b); CHKERRQ(ierr);

      // invoke setSize() on the Solver
      LinearSOESolver *tSolver = this->getSolver();
      int solverOK = tSolver->setSize();
      if (solverOK < 0)
      {
    cerr << "WARNING:PetscSOE::setSize :";
    cerr << " solver failed setSize()\n";
    return solverOK;
      }
    }



    return result;
}
*/


int
PetscSOE::addA(const Matrix &m, const ID &id, double fact)
{
    isFactored = 0;

    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }


    // check that m and id are of similar size
    int idSize = id.Size();

    if (idSize != m.noRows() && idSize != m.noCols())
    {
        cerr << "PetscSOE::addA() - Matrix and ID not of similar sizes\n";
        return -1;
    }

    int n = id.Size();
    // int row;
    // int col;
    // double value;

    // for (int i = 0; i < n; i++)
    // {
    //     row = id(i);

    //     if (row >= 0)
    //     {
    //         for (int j = 0; j < n; j++)
    //         {
    //             col = id(j);

    //             if (col >= 0)
    //             {
    //                 value = m(i, j) * fact;
    //                 int ierr = MatSetValues(A, 1, &row, 1, &col, &value, ADD_VALUES);

    //                 if (ierr)
    //                 {
    //                     cerr << processID << " " << row << " " << col << endln;
    //                 }

    //                 CHKERRQ(ierr);
    //             }
    //         }
    //     }
    // }

    int row[n * 2];
    int col[n * 2];
    double values[n * 2];

    const int * iddata = id.getData();
    const double * matdata = m.getData();
    int pos = 0;
    for (int i = 0; i < n; i++)
    {
        if (iddata[i] >= 0)
        {
            for (int j = 0; j < n; j++) // Row-major, like C
            {
                if (iddata[j] >= 0)
                {
                    row[pos] = iddata[i];
                    col[pos] = iddata[j];
                    values[pos] = matdata[j + i * n] * fact;
                    pos++;
                }
            }
        }
    }

    int ierr = MatSetValues(A, pos, row, pos, col, values, ADD_VALUES);

    if (ierr)
    {
        cerr << processID << " " << row << " " << col << endln;
    }

    CHKERRQ(ierr);


    return 0;
}


int
PetscSOE::addB(const Vector &v, const ID &id, double fact)
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
        cerr << "addB::addB()	- Vector and ID not of similar sizes\n";
        return -1;
    }

    if (fact == 1.0)   // do not need to multiply if fact == 1.0
    {
        for (int i = 0; i < idSize; i++)
        {
            int pos = id(i);

            if (pos < size && pos >= 0)
            {
                B[pos] += v(i);
            }

#ifdef _BABAK_DEBUG
            cerr << "BABAK @ PetscSOE::addB--PID #" << processID << "B [" << pos << "] = " << B[pos] << "and v(" << i << ") = " << v(i) << endl;
#endif



        }
    }
    else if (fact == -1.0)
    {
        for (int i = 0; i < idSize; i++)
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
        for (int i = 0; i < idSize; i++)
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
PetscSOE::setB(const Vector &v, double fact)
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
PetscSOE::zeroA(void)
{
    isFactored = 0;
    MatZeroEntries(A);
}

void
PetscSOE::zeroB(void)
{
    double *Bptr = B;

    for (int i = 0; i < size; i++)
    {
        *Bptr++ = 0;
    }

    VecZeroEntries(b);
}


const Vector &
PetscSOE::getX(void)
{
    if (vectX == 0)
    {
        cerr << "FATAL PetscSOE::getX - vectX == 0!";
        exit(-1);
    }

    return *vectX;
}


const Vector &
PetscSOE::getB(void)
{
    if (vectB == 0)
    {
        cerr << "FATAL PetscSOE::getB - vectB == 0!";
        exit(-1);
    }

    return *vectB;
}


double
PetscSOE::normRHS(void)
{
    this->getB();
    double norm = 0.0;
    double *Bptr = B;

    for (int i = 0; i < size; i++)
    {
        double Yi = *Bptr++;
        norm += Yi * Yi;
    }

    return sqrt(norm);
}


void
PetscSOE::setX(int loc, double value)
{
    if (loc < size && loc >= 0)
    {
        X[loc] = value;
    }
}

void
PetscSOE::setX(const Vector &xData)
{
    if (xData.Size() == size && vectX != 0)
    {
        *vectX = xData;
    }
}

int
PetscSOE::setSolver(PetscSolver &newSolver)
{
    newSolver.setLinearSOE(*this);

    if (size != 0)
    {
        int solverOK = newSolver.setSize();

        if (solverOK < 0)
        {
            cerr << "WARNING:PetscSOE::setSolver :";
            cerr << "the new solver could not setSeize() - staying with old\n";
            return solverOK;
        }
    }

    return this->LinearSOE::setSolver(newSolver);
}


int
PetscSOE::sendSelf(int cTag, Channel &theChannel)
{
    //The following statement makes the if condition below always true
    // processID = 0;
    // int sendID;// = 0;

    // if P0 check if already sent. If already sent use old processID; if not allocate a new process
    // id for remote part of object, enlarge channel * to hold a channel * for this remote object.

    // if not P0, send current processID

    // if (processID == 0)
    // {

    // check if already using this object
    bool found = false;

    for (int i = 0; i < numChannels; i++)
        if (theChannels[i] == &theChannel)
        {
            // sendID = i + 1;
            found = true;
        }

    // if new object, enlarge Channel pointers to hold new channel * & allocate new ID
    if (found == false)
    {
        int nextNumChannels = numChannels + 1;
        Channel **nextChannels = new Channel *[nextNumChannels];

        if (nextNumChannels == 0)
        {
            cerr << "DistributedBandGenLinSOE::sendSelf() - failed to allocate channel array of size: " <<
                 nextNumChannels << endln;
            return -1;
        }

        for (int i = 0; i < numChannels; i++)
        {
            nextChannels[i] = theChannels[i];
        }

        nextChannels[numChannels] = &theChannel;
        numChannels = nextNumChannels;

        if (theChannels != 0)
        {
            delete [] theChannels;
        }

        theChannels = nextChannels;

        if (localCol != 0)
        {
            delete [] localCol;
        }

        localCol = new ID *[numChannels];

        if (localCol == 0)
        {
            cerr << "DistributedBandGenLinSOE::sendSelf() - failed to allocate id array of size: " <<
                 nextNumChannels << endln;
            return -1;
        }

        for (int i = 0; i < numChannels; i++)
        {
            localCol[i] = 0;
        }

        // allocate new processID for remote object
        // sendID = numChannels;
    }

    // }
    // else
    // {
    //     sendID = processID;
    // }

    return 0;
}


int
PetscSOE::receiveSelf(int cTag, Channel &theChannel,
                      FEM_ObjectBroker &theBroker)
{
    numChannels = 1;
    theChannels = new Channel *[1];
    theChannels[0] = &theChannel;

    localCol = new ID *[numChannels];

    for (int i = 0; i < numChannels; i++)
    {
        localCol[i] = 0;
    }

    return 0;
}

int
PetscSOE::setChannels(int nChannels, Channel **theC)
{
    numChannels = nChannels;

    if (theChannels != 0)
    {
        delete [] theChannels;
    }

    theChannels = new Channel *[numChannels];

    for (int i = 0; i < numChannels; i++)
    {
        theChannels[i] = theC[i];
    }


    localCol = new ID *[nChannels];

    for (int i = 0; i < numChannels; i++)
    {
        localCol[i] = 0;
    }

    return 0;
}

//This function is written by Babak Kamrani to dump A Matrix in a file on 03/30/2012:
void
PetscSOE::DumpB()
{
    ofstream outputfile;
    //     char fname[5];
    //     std::string s =std::to_string(processID);
    char buffer [33];
    sprintf(buffer, "B_PID# %d .txt", processID);

    //     itoa (processID,buffer,10);

    //     std::stringstream ss;//create a stringstream
    //     ss << processID;//add number to the stream
    //     string fname = ss.str();//return a string with the contents of the stream

    //      std::string PID = std::to_string(processID);
    //      std::cout << "123 as a string is " << str << '\n';

    //     itoa(processID, PID, 5);
    //     fname = "B_" + PID;
    outputfile.open(buffer , ios::out);
    outputfile << "processID is " << processID << endl;

    for (int i = 0; i < size; i++)
    {
        outputfile << i << "-" << B[i] << " " << endl;
    }

    outputfile << "end" << endl;

    outputfile.close();

}

//This function is written by Babak Kamrani to dump A Matrix in a file on 03/30/2012:
void
PetscSOE::DumpVectB(void)
{

    //     ofstream outputfile;
    //     outputfile.open("VectB.txt" , ios::out);
    //
    //     for (int i=0; i<size; i++)
    //     {
    //           outputfile << i << "-" << vectB(i) << " " << endl;
    //     }
    //     outputfile << "end" << endl;
    //
    //     outputfile.close();

}





//-------------------------------------------------------------//
//Implemented by Babak to add mass Matrices ... 1/14/13:

int
PetscSOE::addM(const Matrix &m, const ID &id, double fact)
{

    isFactored = 0;

    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }


    // check that m and id are of similar size
    int idSize = id.Size();

    if (idSize != m.noRows() && idSize != m.noCols())
    {
        std::cerr << "PetscSOE::addM() - Matrix and ID not of similar sizes\n";
        return -1;
    }

    int n = id.Size();
    int row;
    int col;
    double value;

    for (int i = 0; i < n; i++)
    {
        row = id(i);

        if (row >= 0)
        {
            for (int j = 0; j < n; j++)
            {
                col = id(j);

                if (col >= 0)
                {
                    value = m(i, j) * fact;
                    int ierr = MatSetValues(M, 1, &row, 1, &col, &value, ADD_VALUES);

                    if (ierr)
                    {
                        cerr << processID << " " << row << " " << col << endln;
                    }

                    CHKERRQ(ierr);
                }
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------//
//Implemented by Babak to add mass Matrices ... 1/15/13:

int
PetscSOE::addK(const Matrix &m, const ID &id, double fact)
{

    isFactored = 0;

    // check for a quick return
    if (fact == 0.0)
    {
        return 0;
    }


    // check that m and id are of similar size
    int idSize = id.Size();

    if (idSize != m.noRows() && idSize != m.noCols())
    {
        std::cerr << "PetscSOE::addM() - Matrix and ID not of similar sizes\n";
        return -1;
    }

    int n = id.Size();
    int row;
    int col;
    double value;

    for (int i = 0; i < n; i++)
    {
        row = id(i);

        if (row >= 0)
        {
            for (int j = 0; j < n; j++)
            {
                col = id(j);

                if (col >= 0)
                {
                    value = m(i, j) * fact;
                    int ierr = MatSetValues(K, 1, &row, 1, &col, &value, ADD_VALUES);

                    if (ierr)
                    {
                        cerr << processID << " " << row << " " << col << endln;
                    }

                    CHKERRQ(ierr);
                }
            }
        }
    }

    return 0;
}


//-------------------------------------------------------------//

//Added by Babak to add mass Matrices ... 1/14/13
void
PetscSOE::zeroM(void)
{
    isFactored = 0;
    MatZeroEntries(M);
}

//-------------------------------------------------------------//

//Added by Babak to add mass Matrices ... 1/15/13
void
PetscSOE::zeroK(void)
{
    isFactored = 0;
    MatZeroEntries(K);
}

//-------------------------------------------------------------//

//Added by Babak to add mass Matrices ... 1/15/13
int
PetscSOE::setEigenSize(Graph &theGraph)

{
    if (!init_done)
    {
        PetscInitialize(0, PETSC_NULL, (char *)0, PETSC_NULL);
        MPI_Comm_size(PETSC_COMM_WORLD, &numProcesses);
        MPI_Comm_rank(PETSC_COMM_WORLD, &processID);
        init_done = true;
    }

    //PetscLogInfoAllow(PETSC_TRUE, (char *)0);

    int result = 0;
    int ierr = 0;

    //
    // first determine system size
    //

    if (numProcesses == 1)
    {

        // if single process, the system size is size of graph
        size = theGraph.getNumVertex();
        isFactored = 0;

    }
    else
    {

        // first determine local max
        size = 0;
        isFactored = 0;
        VertexIter &theVertices = theGraph.getVertices();
        Vertex *theVertex;

        while ((theVertex = theVertices()) != 0)
        {
            int vertexTag = theVertex->getTag();

            if (vertexTag > size)
            {
                size = vertexTag;
            }

#ifdef _BABAK_DEBUG
            cerr << "BABAK @ size PetscSOE::setSize ---- PID:" << processID << "Size: " << size << "\n";
            cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID << "---raphNumEdge:" << theGraph.getNumEdge() << endl;
            cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID << "---graphgetNumVertex:" << theGraph.getNumVertex() << endl;
#endif
        }

        static ID data(1);

        // all local max's sent to P0 which determines the max
        // and informs all others
#ifdef _BABAK_DEBUG

        cerr << "BABAK @ PetscSOE::setSize --  PID:" << processID << "set size is done" << endl;
#endif

        if (processID != 0)
        {
            Channel *theChannel = theChannels[0];

            data(0) = size;
            theChannel->sendID(0, 0, data);
            theChannel->receiveID(0, 0, data);

            size = data(0);
        }
        else
        {

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->receiveID(0, 0, data);

                if (data(0) > size)
                {
                    size = data(0);
                }
            }

            data(0) = size;

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->sendID(0, 0, data);
            }
        }

        size = size + 1; // vertices numbered 0 through n-1
    }



    // invoke the petsc destructors
    if (A != 0)
    {
        MatDestroy(&A);
    }

    if (M != 0)
    {
        MatDestroy(&M);
    }

    if (b != 0)
    {
        VecDestroy(&b);
    }

    if (x != 0)
    {
        VecDestroy(&x);
    }

    //
    // now we create the opensees vector objects
    //

    // delete the old vectors
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

    if (B == 0 || X == 0)
    {
        cerr << "WARNING PetscSOE::PetscSOE :";
        cerr << " ran out of memory for vectors (size) (";
        cerr << size << ") \n";
        size = 0;
        result = -1;
    }

    // zero the vectors
    for (int j = 0; j < size; j++)
    {
        B[j] = 0;
        X[j] = 0;
    }

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

    //
    // now create petsc matrix and vector objects
    //

    if (numProcesses == 1)
    {
        /*
              // we determine the number of non-zeros & number of nonzero's
              // in each row of A
              int *rowA = new int[size];  // will contain no of non-zero entries
              // in each row

              int NNZ = 0;
              for (int a=0; a<size; a++)
              {
            Vertex *theVertex = theGraph.getVertexPtr(a);
            if (theVertex == 0)
            {
              cerr << "WARNING:PetscSOE::setSize :";
              cerr << " vertex " << a << " not in graph! - size set to 0\n";
              size = 0;
              return -1;
            }

            const ID &theAdjacency = theVertex->getAdjacency();
            int idSize = theAdjacency.Size();

            NNZ += idSize +1;
            rowA[a] = idSize +1;  // +1 for the diagonal entry
              }

              //
              // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
              //

              //      ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);

              if (blockSize == 1)
              {
            ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, size, size, 0, rowA, &A); CHKERRQ(ierr);
            ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, size, size, 0, rowA, &M); CHKERRQ(ierr);

              }
              else
              {
            ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF, blockSize, size,size, 0, rowA, &A); CHKERRQ(ierr);
            ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF, blockSize, size,size, 0, rowA, &M); CHKERRQ(ierr);

              }

              ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, &X, &x); CHKERRQ(ierr);
              ierr = VecCreateSeqWithArray(PETSC_COMM_WORLD, size, &B, &b); CHKERRQ(ierr);


              // invoke setSize() on the Solver
              LinearSOESolver *tSolver = this->getSolver();
              int solverOK = tSolver->setSize();
              if (solverOK < 0)
              {
            cerr << "WARNING:PetscSOE::setSize :";
            cerr << " solver failed setSize()\n";
            return solverOK;
              }

              // clear up the memory we used
              delete [] rowA;*/


    }
    else
    {

        //
        // Call Petsc VecCreate & MatCreate; NOTE: using previously allocated storage for vectors
        //
        //

        //Guanzhou changed interface to petsc_2.3.0
        PetscOptionsGetInt(PETSC_NULL, "-n", &size, PETSC_NULL);

        //ICC MatCreate(PETSC_COMM_WORLD,&A); //ICC
        //ICC MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,size,size); //ICC
        //ICC MatSetType(A, MATMPIROWBS); //ICC

        //MatCreateMPIAIJ(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE, //NONE
        //    size, size, 99, PETSC_NULL, 99, PETSC_NULL, &A);    //NONE

        ierr = MatCreate(PETSC_COMM_WORLD, &A);
        CHKERRQ(ierr);
        ierr = MatCreate(PETSC_COMM_WORLD, &M);
        CHKERRQ(ierr);

        ierr = MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, size, size);
        CHKERRQ(ierr);
        ierr = MatSetSizes(M, PETSC_DECIDE, PETSC_DECIDE, size, size);
        CHKERRQ(ierr);

        ierr = MatSetType(A, mType);
        CHKERRQ(ierr);
        ierr = MatSetType(M, mType);
        CHKERRQ(ierr);

        ierr = MatSetFromOptions(A);
        CHKERRQ(ierr);
        ierr = MatSetFromOptions(M);
        CHKERRQ(ierr);

        //MatSetOption(A, MAT_SYMMETRIC);
        //MatSetOption(A, MAT_SYMMETRY_ETERNAL);

        //ICC MatSetFromOptions(A); //ICC
        MatGetOwnershipRange(A, &startRow, &endRow);
        MatGetOwnershipRange(M, &startRow, &endRow);


        //ierr = PetscOptionsGetInt(PETSC_NULL, "-n", &size, &flg); CHKERRQ(ierr);
        //ierr = MatCreate(PETSC_COMM_WORLD, PETSC_DECIDE, PETSC_DECIDE,size, size, &A); CHKERRQ(ierr);
        //ierr = MatSetFromOptions(A);CHKERRQ(ierr);
        //ierr = MatGetOwnershipRange(A, &startRow, &endRow);CHKERRQ(ierr);

        VecCreateMPIWithArray(PETSC_COMM_WORLD, blockSize, endRow - startRow , size, &X[startRow], &x);
        CHKERRQ(ierr);
        VecCreateMPIWithArray(PETSC_COMM_WORLD, blockSize, endRow - startRow , size, &B[startRow], &b);
        CHKERRQ(ierr);

        // invoke setSize() on the Solver
        LinearSOESolver *tSolver = this->getSolver();
        int solverOK = tSolver->setSize();

        if (solverOK < 0)
        {
            cerr << "WARNING:PetscSOE::setSize :";
            cerr << " solver failed setSize()\n";
            return solverOK;
        }
    }



    return result;
}
