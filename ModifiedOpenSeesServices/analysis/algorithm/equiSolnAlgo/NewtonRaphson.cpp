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

// $Revision: 1.8 $
// $Date: 2003/02/14 23:00:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/NewtonRaphson.cpp,v $


// File: ~/OOP/analysis/algorithm/NewtonRaphson.C
//
// Written: fmk
// Created: Sun Sept 15 15:06:47: 1996
// Revision: A
//

// Description: This file contains the class definition for
// NewtonRaphson. NewtonRaphson is a class which uses the
// Newton-Raphson solution algorihm
// to solve the equations. No member functions are declared as virtual as
// it is not expected that this class will be subclassed.
//
// What: "@(#)NewtonRaphson.C, revA"

#include <NewtonRaphson.h>
#include <AnalysisModel.h>
#include <StaticAnalysis.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ConvergenceTest.h>
#include <ID.h>
#include <iostream>
using namespace std;


// Constructor
NewtonRaphson::NewtonRaphson(int theTangentToUse)
    : EquiSolnAlgo(EquiALGORITHM_TAGS_NewtonRaphson),
      theTest(0), tangent(theTangentToUse),global_iteration_no(0), output_iterations(false)
{

}


NewtonRaphson::NewtonRaphson(ConvergenceTest& theT, int theTangentToUse)
    : EquiSolnAlgo(EquiALGORITHM_TAGS_NewtonRaphson),
      theTest(&theT), tangent(theTangentToUse),global_iteration_no(0), output_iterations(false)
{

}

// Destructor
NewtonRaphson::~NewtonRaphson()
{


}

int
NewtonRaphson::setConvergenceTest(ConvergenceTest* newTest)
{
    theTest = newTest;
    return 0;
}



int
NewtonRaphson::solveCurrentStep(void)
{
    // set up some pointers and check they are valid
    // NOTE this could be taken away if we set Ptrs as protecetd in superclass
    AnalysisModel*   theAnaModel = this->getAnalysisModelPtr();
    IncrementalIntegrator* theIntegrator = this->getIncrementalIntegratorPtr();
    LinearSOE*  theSOE = this->getLinearSOEptr();

    if ((theAnaModel == 0) || (theIntegrator == 0) || (theSOE == 0)
            || (theTest == 0))
    {
        cerr << "WARNING NewtonRaphson::solveCurrentStep() - setLinks() has";
        cerr << " not been called - or no ConvergenceTest has been set\n";
        return -5;
    }

    if (theIntegrator->formUnbalance() < 0)
    {
        cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
        cerr << "the Integrator failed in formUnbalance()\n";
        return -2;
    }

    // set itself as the ConvergenceTest objects EquiSolnAlgo
    theTest->setEquiSolnAlgo(*this);

    if (theTest->start() < 0)
    {
        cerr << "NewtnRaphson::solveCurrentStep() -";
        cerr << "the ConvergenceTest object failed in start()\n";
        return -3;
    }

    int result = -1;
    int count = 0;

    do
    {
        if (tangent == INITIAL_THEN_CURRENT_TANGENT)
        {
            if (count == 0)
            {
                if (theIntegrator->formTangent(INITIAL_TANGENT) < 0)
                {
                    cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
                    cerr << "the Integrator failed in formTangent()\n";
                    return -1;
                }
            }
            else
            {
                if (theIntegrator->formTangent(CURRENT_TANGENT) < 0)
                {
                    cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
                    cerr << "the Integrator failed in formTangent()\n";
                    return -1;
                }
            }
        }
        else
        {
            if (theIntegrator->formTangent(tangent) < 0)
            {
                cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
                cerr << "the Integrator failed in formTangent()\n";
                return -1;
            }
        }

        if (theSOE->solve() < 0)
        {
            cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
            cerr << "the LinearSysOfEqn failed in solve()\n";
            return -3;
        }

        if (theIntegrator->update(theSOE->getX()) < 0)
        {
            cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
            cerr << "the Integrator failed in update()\n";
            return -4;
        }

        if (theIntegrator->formUnbalance() < 0)
        {
            cerr << "WARNING NewtonRaphson::solveCurrentStep() -";
            cerr << "the Integrator failed in formUnbalance()\n";
            return -2;
        }

        result = theTest->test();

        if(output_iterations) // Added by Sumeet September, 2016
            theIntegrator->output_iteration(++global_iteration_no);

    }
    while (result == -1);

    if (result == -2)
    {
        cerr << "NewtonRaphson::solveCurrentStep() -";
        cerr << "the ConvergenceTest object failed in test()\n";
        return -3;
    }

    // note - if postive result we are returning what the convergence test returned
    // which should be the number of iterations
    return result;
}


ConvergenceTest*
NewtonRaphson::getConvergenceTest(void)
{
    return theTest;
}

int
NewtonRaphson::sendSelf(int cTag, Channel& theChannel)
{
    static ID data(1);
    data(0) = tangent;
    return theChannel.sendID(this->getDbTag(), cTag, data);
}

int
NewtonRaphson::receiveSelf(int cTag,
                           Channel& theChannel,
                           FEM_ObjectBroker& theBroker)
{
    static ID data(1);
    theChannel.receiveID(this->getDbTag(), cTag, data);
    tangent = data(0);
    return 0;
}


void
NewtonRaphson::Print(ostream& s, int flag)
{
    if (flag == 0)
    {
        s << "NewtonRaphson" << endln;
    }
}

/***********************************************************************
* Sumeet September, 2016
* Switch on/off the saving of output of iterations 
************************************************************************/
void 
NewtonRaphson::switchOutputIterationOption(bool status){

    output_iterations=status;
}






