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

// $Revision: 1.5 $
// $Date: 2003/02/14 23:00:52 $
// $Source: /usr/local/cvs/OpenSees/SRC/convergenceTest/CTestNormUnbalance.cpp,v $



#include <CTestNormUnbalance.h>
#include <Vector.h>
#include <Channel.h>
#include <EquiSolnAlgo.h>
#include <LinearSOE.h>
#include <iostream>
#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif
using namespace std;


CTestNormUnbalance::CTestNormUnbalance()
    : ConvergenceTest(CONVERGENCE_TEST_CTestNormUnbalance),
      theSOE(0), tol(0.0), maxNumIter(0), currentIter(0), printFlag(0), norms(1)
{

}

CTestNormUnbalance::CTestNormUnbalance(double theTol, int maxIter, int printIt)
    : ConvergenceTest(CONVERGENCE_TEST_CTestNormUnbalance),
      theSOE(0), tol(theTol), maxNumIter(maxIter), currentIter(0), printFlag(printIt),
      norms(maxNumIter)
{

}

CTestNormUnbalance::~CTestNormUnbalance()
{

}



ConvergenceTest*
CTestNormUnbalance::getCopy( int iterations )
{
    CTestNormUnbalance* theCopy ;
    theCopy = new CTestNormUnbalance( this->tol, iterations, this->printFlag ) ;

    theCopy->theSOE = this->theSOE ;

    return theCopy ;
}

void
CTestNormUnbalance::setTolerance(double newTol)
{
    tol = newTol;
}

int
CTestNormUnbalance::setEquiSolnAlgo(EquiSolnAlgo& theAlgo)
{
    theSOE = theAlgo.getLinearSOEptr();

    if (theSOE == 0)
    {
        cerr << "WARNING: CTestNormUnbalance::setEquiSolnAlgo - no SOE\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


int
CTestNormUnbalance::test(void)
{
    // check to ensure the SOE has been set - this should not happen if the
    // return from start() is checked
    if (theSOE == 0)
    {
        return -2;
    }

    // check to ensure the algo does invoke start() - this is needed otherwise
    // may never get convergence later on in analysis!
    if (currentIter == 0)
    {
        cerr << "WARNING: CTestNormDisp::test() - start() was never invoked.\n";
        return -2;
    }


    // get the B vector & determine it's norm & save the value in norms vector
    Vector x;
    #ifdef _PARALLEL_PROCESSING
        int rank=0;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        // // ================================
        // // Debug printing
        // // ================================
        // cout<<"rank "<<rank << ". vecB.size = "<<(theSOE->getB()).Size()<<endl;
        // cout<<"rank "<<rank << ". vecB.norm = "<<(theSOE->getB()).Norm()<<endl;
        // // ================================
        Vector myVectB =theSOE->getB();
        int bsize = myVectB.Size();
        std::vector<double> in_B(bsize,0.0);
        std::vector<double> out_B(bsize,0.0);
        for (int i = 0; i < bsize; ++i)
            in_B[i]=myVectB(i) ; 
        MPI_Allreduce(&in_B[0] , &out_B[0] , bsize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        x.resize(bsize);
        for (int i = 0; i < bsize; ++i)
           x(i)=out_B[i];
    #else
        x = theSOE->getB();
    #endif
    double norm = x.Norm();

    if (currentIter <= maxNumIter)
    {
        norms(currentIter - 1) = norm;
    }

    // print the data if required
    if (printFlag == 1)
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << "/" << left << std::setw(5) << maxNumIter << "]  CTestNormUnbalance::(tol: " << tol <<")\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n";
    }

    // print the data if required
    if (printFlag == 4)
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << "/" << left << std::setw(5) << maxNumIter << "]  CTestNormUnbalance::(tol: " << tol <<")\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n";
    }

    //
    // check if the algorithm converged
    //

    // if converged - print & return ok
    if (norm <= tol)   // the algorithm converged
    {

        // do some printing first
        if (printFlag != 0)
        {
            if (printFlag == 1)
            {
                cerr << endln;
            }
            else if (printFlag == 2)
            {
                cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << "/" << left << std::setw(5) << maxNumIter << "]  CTestNormUnbalance::(tol: " << tol <<")\n";
                cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << norm                  << "\n";
                cerr << std::setw(5) << "\t \t          " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n";
            }
        }

        // return the number of times test has been called
        return currentIter;
    }

    // algo failed to converged after specified number of iterations - but RETURN OK
    else if (printFlag == 5 && currentIter >= maxNumIter)
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << "/" << left << std::setw(5) << maxNumIter << "]  CTestNormUnbalance::(tol: " << setprecision(5) << tol << ")     !!!FAILED TO CONVERGE!!! [PROCEEDING..]"<<"\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n";
        return currentIter;
    }

    // algo failed to converged after specified number of iterations - return FAILURE -2
    else if (currentIter >= maxNumIter)   // the algorithm failed to converge
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << "/" << left << std::setw(5) << maxNumIter << "]  CTestNormUnbalance::(tol: " << setprecision(5) << tol << ")     !!!FAILED TO CONVERGE!!! [EXITING..]"<<"\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t \t           " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n";
        currentIter++;  // we increment in case analysis does not check for convergence
        return -2;
    }

    // algo not yet converged - increment counter and return -1
    else   // the algorithm has not converged yet
    {
        currentIter++;
        return -1;
    }
}


int
CTestNormUnbalance::start(void)
{
    if (theSOE == 0)
    {
        cerr << "WARNING: CTestNormUnbalance::test() - no SOE returning true\n";
        return -1;
    }

    // set iteration count = 1
    norms.Zero();
    currentIter = 1;
    return 0;
}


int
CTestNormUnbalance::getNumTests()
{
    return currentIter;
}

int
CTestNormUnbalance::getMaxNumTests(void)
{
    return maxNumIter;
}

double
CTestNormUnbalance::getRatioNumToMax(void)
{
    double div = maxNumIter;
    return currentIter / div;
}

const Vector&
CTestNormUnbalance::getNorms()
{
    return norms;
}



int
CTestNormUnbalance::sendSelf(int cTag, Channel &theChannel)
{
    int res = 0;
    Vector x(4);
    x(0) = tol;
    x(1) = maxNumIter;
    // x(2) = printFlag;
    x(2) = 0; // Only processor 0 prints info, whatever it is
    x(3) = norms.Size();
    res = theChannel.sendVector(this->getDbTag(), cTag, x);

    if (res < 0)
    {
        cerr << "CTestNormUnbalance::sendSelf() - failed to send data\n";
    }

    return res;
}

int
CTestNormUnbalance::receiveSelf(int cTag, Channel &theChannel,
                               FEM_ObjectBroker &theBroker)
{
    int res = 0;
    Vector x(4);
    res = theChannel.receiveVector(this->getDbTag(), cTag, x);

    tol = x(0);
    maxNumIter = x(1);
    printFlag = x(2);
    norms.resize(x(3));
    //  norms.resize(maxNumIter);

    return res;
}

