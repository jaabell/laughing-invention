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

// $Revision: 1.2 $
// $Date: 2003/02/14 23:00:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/Linear.cpp,v $


// File: ~/OOP/analysis/algorithm/Linear.C
//
// Written: fmk
// Created: Sun Sept 15 15:06:47: 1996
// Revision: A
//

// Description: This file contains the class definition for
// Linear. Linear is a class which performs a linear solution algorihm
// to solve the equations. No member functions are declared as virtual as
// it is not expected that this class will be subclassed.
//
// What: "@(#)Linear.C, revA"

#include <Linear.h>
#include <AnalysisModel.h>
#include <StaticAnalysis.h>
#include <StaticIntegrator.h>
#include <LinearSOE.h>
#include <Vector.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ConvergenceTest.h>
#include <iostream>
using namespace std;


#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif


#include <ESSITimer.h>
// Constructor
Linear::Linear()
    : EquiSolnAlgo(EquiALGORITHM_TAGS_Linear),global_iteration_no(0), output_iterations(false)
{

}

// Destructor
Linear::~Linear()
{

}

// int run(void)
//    Performs the linear solution algorithm.

int
Linear::solveCurrentStep(void)
{
    // set up some pointers and check they are valid
    // NOTE this could be taken away if we set Ptrs as protecetd in superclass

    AnalysisModel *theAnalysisModel = this->getAnalysisModelPtr();
    LinearSOE  *theSOE = this->getLinearSOEptr();
    IncrementalIntegrator  *theIncIntegrator = this->getIncrementalIntegratorPtr();

    if ((theAnalysisModel == 0) || (theIncIntegrator == 0 ) || (theSOE == 0))
    {
        cerr << "WARNING Linear::solveCurrentStep() -";
        cerr << "setLinks() has not been called.\n";
        return -5;
    }

    globalESSITimer.start("SOE_Form_K");
    if (theIncIntegrator->formTangent() < 0)
    {
        cerr << "WARNING Linear::solveCurrentStep() -";
        cerr << "the Integrator failed in formTangent()\n";
        return -1;
    }

#ifdef _PARALLEL_PROCESSING
    //Need to syncronize to get proper global timing
    MPI_Barrier( MPI_COMM_WORLD);
#endif

    globalESSITimer.stop("SOE_Form_K");

    globalESSITimer.start("SOE_Form_b");
    if (theIncIntegrator->formUnbalance() < 0)
    {
        cerr << "WARNING Linear::solveCurrentStep() -";
        cerr << "the Integrator failed in formUnbalance()\n";
        return -2;
    }
#ifdef _PARALLEL_PROCESSING
    //Need to syncronize to get proper global timing
    MPI_Barrier( MPI_COMM_WORLD);
#endif

    globalESSITimer.stop("SOE_Form_b");

    globalESSITimer.start("SOE_Solve");
    if (theSOE->solve() < 0)
    {
        cerr << "WARNING Linear::solveCurrentStep() -";
        cerr << "the LinearSOE failed in solve()\n";
        return -3;
    }
    //Doesn't need sync, there is a barrier for the SOE solution inside solve() when called in parallel.
#ifdef _PARALLEL_PROCESSING
    //Need to syncronize to get proper global timing
    MPI_Barrier( MPI_COMM_WORLD);
#endif

    globalESSITimer.stop("SOE_Solve");

    globalESSITimer.start("SOE_Update");
    const Vector &deltaU = theSOE->getX();

    if (theIncIntegrator->update(deltaU) < 0)
    {
        cerr << "WARNING Linear::solveCurrentStep() -";
        cerr << "the Integrator failed in update()\n";
        return -4;
    }
#ifdef _PARALLEL_PROCESSING
    //Need to syncronize to get proper global timing
    MPI_Barrier( MPI_COMM_WORLD);
#endif
    globalESSITimer.stop("SOE_Update");
    return 0;
}

int
Linear::setConvergenceTest(ConvergenceTest *theNewTest)
{
    // Linear Algorithm dooes not need a convergence test
    return 0;
}

ConvergenceTest *
Linear::getConvergenceTest(void)
{
    // Linear Algorithm dooes not have a convergence test
    return 0;
}

int
Linear::sendSelf(int cTag, Channel &theChannel)
{
    return 0;
}

int
Linear::receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}


void
Linear::Print(ostream &s, int flag)
{
    s << "\t Linear algorithm";
}

/***********************************************************************
* Sumeet September, 2016
* Switch on/off the saving of output of iterations 
************************************************************************/
void 
Linear::switchOutputIterationOption(bool status){

    output_iterations=status;
}
