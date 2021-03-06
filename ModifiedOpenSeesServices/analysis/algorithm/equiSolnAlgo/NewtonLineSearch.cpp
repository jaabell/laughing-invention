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

// $Revision: 1.6 $
// $Date: 2007-05-04 06:59:54 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/NewtonLineSearch.cpp,v $

// Written: fmk
// Created: 11/96
// Modified: Ed "C++" Love 10/00 to perform the line search
//
// Description: This file contains the implementation for NewtonLineSearch.
//
// What: "@(#)NewtonLineSearch.h, revA"

#include <NewtonLineSearch.h>
#include <AnalysisModel.h>
#include <StaticAnalysis.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ConvergenceTest.h>
#include <ID.h>

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

//Null Constructor
NewtonLineSearch::NewtonLineSearch( )
    : EquiSolnAlgo(EquiALGORITHM_TAGS_NewtonLineSearch),
      theTest(0), theOtherTest(0), theLineSearch(0),global_iteration_no(0), output_iterations(false)
{
}


//Constructor
NewtonLineSearch::NewtonLineSearch( ConvergenceTest &theT,
                                    LineSearch *theSearch)
    : EquiSolnAlgo(EquiALGORITHM_TAGS_NewtonLineSearch),
      theTest(&theT), theLineSearch(theSearch),global_iteration_no(0), output_iterations(false)
{
    theOtherTest = theTest->getCopy(10);
    theOtherTest->setEquiSolnAlgo(*this);
}


// Destructor
NewtonLineSearch::~NewtonLineSearch()
{
    if (theOtherTest != 0)
    {
        delete theOtherTest;
    }
}

int
NewtonLineSearch::setConvergenceTest(ConvergenceTest *newTest)
{
    theTest = newTest;
    if (theOtherTest != 0)
    {
        delete theOtherTest;
    }
    theOtherTest = theTest->getCopy(10);
    theOtherTest->setEquiSolnAlgo(*this);
    return 0;
}


int
NewtonLineSearch::solveCurrentStep(void)
{
    // set up some pointers and check they are valid
    // NOTE this could be taken away if we set Ptrs as protecetd in superclass
    AnalysisModel   *theAnaModel = this->getAnalysisModelPtr();
    IncrementalIntegrator *theIntegrator = this->getIncrementalIntegratorPtr();
    LinearSOE  *theSOE = this->getLinearSOEptr();

    if ((theAnaModel == 0) || (theIntegrator == 0) || (theSOE == 0)
            || (theTest == 0))
    {
        cout << "WARNING NewtonLineSearch::solveCurrentStep() - setLinks() has";
        cout << " not been called - or no ConvergenceTest has been set\n";
        return -5;
    }

    theLineSearch->newStep(*theSOE);

    // set itself as the ConvergenceTest objects EquiSolnAlgo
    theTest->setEquiSolnAlgo(*this);
    if (theTest->start() < 0)
    {
        cout << "NewtonLineSearch::solveCurrentStep() -";
        cout << "the ConvergenceTest object failed in start()\n";
        return -3;
    }

    if (theIntegrator->formUnbalance() < 0)
    {
        cout << "WARNING NewtonLineSearch::solveCurrentStep() -";
        cout << "the Integrator failed in formUnbalance()\n";
        return -2;
    }

    int result = -1;
    do
    {

        //residual at this iteration before next solve
        const Vector &Resid0 = theSOE->getB() ;

        //form the tangent
        if (theIntegrator->formTangent() < 0)
        {
            cout << "WARNING NewtonLineSearch::solveCurrentStep() -";
            cout << "the Integrator failed in formTangent()\n";
            return -1;
        }

        //solve
        if (theSOE->solve() < 0)
        {
            cout << "WARNING NewtonLineSearch::solveCurrentStep() -";
            cout << "the LinearSysOfEqn failed in solve()\n";
            return -3;
        }


        //line search direction
        const Vector &dx0 = theSOE->getX() ;

        //intial value of s
        double s0 = - (dx0 ^ Resid0) ;

        if (theIntegrator->update(theSOE->getX()) < 0)
        {
            cout << "WARNING NewtonLineSearch::solveCurrentStep() -";
            cout << "the Integrator failed in update()\n";
            return -4;
        }

        if (theIntegrator->formUnbalance() < 0)
        {
            cout << "WARNING NewtonLineSearch::solveCurrentStep() -";
            cout << "the Integrator failed in formUnbalance()\n";
            return -2;
        }

        // do a line search only if convergence criteria not met
        theOtherTest->start();
        result = theOtherTest->test();

        if (result < 1)
        {
            //new residual
            const Vector &Resid = theSOE->getB() ;

            //new value of s
            double s = - ( dx0 ^ Resid ) ;

#ifdef _PARALLEL_PROCESSING
            // In the case of parallel processing, residuals are computed locally at each processor and
            // are different in general.
            // On the other hand dx0 was computed once and for all at the invokation of solve() and
            // should be the same across all processes. Therefore, to compute s in parallel a reduction
            // operation is needed, summing up all values of 's' across all preocesses.
            double s_;
            MPI_Allreduce(
                &s,
                &s_,
                1,
                MPI_DOUBLE,
                MPI_SUM,
                MPI_COMM_WORLD);
            s = s_;
#endif
            if (theLineSearch != 0)
            {
                theLineSearch->search(s0, s, *theSOE, *theIntegrator);
            }
        }

        // this->record(0);

        result = theTest->test();

        if(output_iterations) // Added by Sumeet September, 2016
            theIntegrator->output_iteration(++global_iteration_no);

    }
    while (result == -1);

    if (result == -2)
    {
        cout << "NewtonLineSearch::solveCurrentStep() -";
        cout << "the ConvergenceTest object failed in test()\n";
        return -3;
    }

    // note - if postive result we are returning what the convergence test returned
    // which should be the number of iterations
    return result;
}

ConvergenceTest *
NewtonLineSearch::getConvergenceTest(void)
{
    return theTest;
}

int
NewtonLineSearch::sendSelf(int cTag, Channel &theChannel)
{
    static ID data(1);
    data(0) = theLineSearch->getClassTag();
    if (theChannel.sendID(0, cTag, data) < 0)
    {
        cout << "NewtonLineSearch::sendSelf(int cTag, Channel &theChannel)   - failed to send date\n";
        return -1;
    }

    if (theLineSearch->sendSelf(cTag, theChannel) < 0)
    {
        cout << "NewtonLineSearch::sendSelf(int cTag, Channel &theChannel)   - failed to send line search\n";
        return -1;
    }

    return 0;
}

int
NewtonLineSearch::receiveSelf(int cTag,
                              Channel &theChannel,
                              FEM_ObjectBroker &theBroker)
{
    static ID data(1);
    if (theChannel.receiveID(0, cTag, data) < 0)
    {
        cout << "NewtonLineSearch::receiveSelf(int cTag, Channel &theChannel) - failed to receive data\n";
        return -1;
    }

    int lineSearchClassTag = data(0);

    if (theLineSearch == 0 || theLineSearch->getClassTag() != lineSearchClassTag)
    {
        if (theLineSearch != 0)
        {
            delete theLineSearch;
        }

        theLineSearch = theBroker.getNewLineSearch(lineSearchClassTag);
        if (theLineSearch == 0)
        {
            cout << "NewtonLineSearch::receiveSelf(int cTag, Channel &theChannel) - failed to obtain a LineSerach object\n";
            return -1;
        }
    }

    if (theLineSearch->receiveSelf(cTag, theChannel, theBroker) < 0)
    {
        cout << "NewtonLineSearch::receiveSelf(int cTag, Channel &theChannel) - failed to receive the LineSerach object\n";
        return -1;
    }

    return 0;

}


void
NewtonLineSearch::Print(std::ostream &s, int flag)
{
    if (flag == 0)
    {
        s << "NewtonLineSearch\n";
    }

    if (theLineSearch != 0)
    {
        theLineSearch->Print(s, flag);
    }
}


/***********************************************************************
* Sumeet September, 2016
* Switch on/off the saving of output of iterations 
************************************************************************/
void 
NewtonLineSearch::switchOutputIterationOption(bool status){

    output_iterations=status;
}






