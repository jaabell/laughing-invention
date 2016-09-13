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

// $Revision: 1.3 $
// $Date: 2003/02/14 23:00:52 $
// $Source: /usr/local/cvs/OpenSees/SRC/convergenceTest/CTestRelativeNormUnbalance.cpp,v $

#include <CTestRelativeNormUnbalance.h>
#include <Vector.h>
#include <Channel.h>
#include <EquiSolnAlgo.h>
#include <LinearSOE.h>
#include <iostream>
using namespace std;


CTestRelativeNormUnbalance::CTestRelativeNormUnbalance()
    : ConvergenceTest(CONVERGENCE_TEST_CTestRelativeNormUnbalance),
      theSOE(0), tol(0.0), maxNumIter(0), currentIter(0), printFlag(0), norms(1), norm0(0.0)
{

}

CTestRelativeNormUnbalance::CTestRelativeNormUnbalance(double theTol, int maxIter, int printIt)
    : ConvergenceTest(CONVERGENCE_TEST_CTestRelativeNormUnbalance),
      theSOE(0), tol(theTol), maxNumIter(maxIter), currentIter(0), printFlag(printIt),
      norms(maxNumIter + 1), norm0(0.0)
{

}

CTestRelativeNormUnbalance::~CTestRelativeNormUnbalance()
{

}



ConvergenceTest*
CTestRelativeNormUnbalance::getCopy( int iterations )
{
    CTestRelativeNormUnbalance* theCopy ;
    theCopy = new CTestRelativeNormUnbalance( this->tol, iterations, this->printFlag ) ;

    theCopy->theSOE = this->theSOE ;

    return theCopy ;
}

void
CTestRelativeNormUnbalance::setTolerance(double newTol)
{
    tol = newTol;
}

int
CTestRelativeNormUnbalance::setEquiSolnAlgo(EquiSolnAlgo& theAlgo)
{
    theSOE = theAlgo.getLinearSOEptr();

    if (theSOE == 0)
    {
        cerr << "WARNING: CTestRelativeNormUnbalance::setEquiSolnAlgo - no SOE\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


int
CTestRelativeNormUnbalance::test(void)
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
    const Vector& x = theSOE->getB();
    double norm = x.Norm(); double x_norm = norm;

    if (currentIter <= maxNumIter)
    {
        norms(currentIter) = norm;
    }

    // determine the ration
    if (norm0 != 0.0)
    {
        norm /= norm0;
    }


    // print the data if required
    if (printFlag == 1)
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << std::setw(10) << "]  CTestRelativeNormUnbalance::(tol: " << tol << ")\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   |dR|/|dR1  : " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << x_norm                << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n"; 
    }

    // print the data if required
    if (printFlag == 4)
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << std::setw(10) << "]  CTestRelativeNormUnbalance::(tol: " << tol << ")\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   |dR|/|dR1  : " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << x_norm                << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n"; 
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
                cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << std::setw(10) << "]  CTestRelativeNormUnbalance::(tol: " << tol << ")\n";
                cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   |dR|/|dR1  : " << setprecision(5) << norm                  << "\n";
                cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << x_norm                << "\n";
                cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n"; 

            }
        }

        // return the number of times test has been called
        return currentIter;
    }

    // algo failed to converged after specified number of iterations - but RETURN OK
    else if (printFlag == 5 && currentIter >= maxNumIter)
    {

        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << std::setw(10) << "]  CTestRelativeNormUnbalance::(tol: " << tol << ")     !!!FAILED TO CONVERGE!!! [PROCEEDING..]"<<"\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   |dR|/|dR1  : " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << x_norm                << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n"; 


        return currentIter;
    }

    // algo failed to converged after specified number of iterations - return FAILURE -2
    else if (currentIter >= maxNumIter)   // the algorithm failed to converge
    {
        cerr << std::setw(5) << "\t [iteration " << std::setw(5) << currentIter << std::setw(10) << "]  CTestRelativeNormUnbalance::(tol: " << tol << ")     !!!FAILED TO CONVERGE!!! [EXITING..]"<<"\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   |dR|/|dR1  : " << setprecision(5) << norm                  << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaF: " << setprecision(5) << x_norm                << "\n";
        cerr << std::setw(5) << "\t            " << std::setw(5) << " "         << std::setw(10) << "   Norm deltaX: " << setprecision(5) << theSOE->getX().Norm() << "\n"; 

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
CTestRelativeNormUnbalance::start(void)
{
    if (theSOE == 0)
    {
        cerr << "WARNING: CTestRelativeNormUnbalance::test() - no SOE returning true\n";
        return -1;
    }

    // set iteration count = 1
    norms.Zero();
    currentIter = 1;
    norm0 = 0.0;

    //     determine the initial norm .. the the norm of the initial unbalance
    const Vector& x = theSOE->getB();
    double norm = x.Norm();

    if (currentIter <= maxNumIter)
    {
        norms(0) = norm;
    }

    norm0 = norm;

    return 0;
}


int
CTestRelativeNormUnbalance::getNumTests()
{
    return currentIter;
}

int
CTestRelativeNormUnbalance::getMaxNumTests(void)
{
    return maxNumIter;
}

double
CTestRelativeNormUnbalance::getRatioNumToMax(void)
{
    double div = maxNumIter;
    return currentIter / div;
}

const Vector&
CTestRelativeNormUnbalance::getNorms()
{
    return norms;
}


int
CTestRelativeNormUnbalance::sendSelf(int cTag, Channel& theChannel)
{
    int res = 0;
    Vector x(2);
    x(0) = tol;
    x(1) = maxNumIter;
    res = theChannel.sendVector(this->getDbTag(), cTag, x);

    if (res < 0)
    {
        cerr << "CTestRelativeNormUnbalance::sendSelf() - failed to send data\n";
    }

    return res;
}

int
CTestRelativeNormUnbalance::receiveSelf(int cTag, Channel& theChannel,
                                     FEM_ObjectBroker& theBroker)
{
    int res = 0;
    Vector x(2);
    res = theChannel.receiveVector(this->getDbTag(), cTag, x);

    if (res < 0)
    {
        tol = 1.0e-8;
        maxNumIter = 25;
        cerr << "CTestRelativeNormUnbalance::sendSelf() - failed to send data\n";
    }
    else
    {
        tol = x(0);
        maxNumIter = x(1);
        norms.resize(maxNumIter);
    }

    return res;
}

