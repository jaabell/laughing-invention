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
// $Date: 2000/12/12 06:19:32 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.cpp,v $


// File: ~/OOP/analysis/algorithm/EquiSolnAlgo.C
//
// Written: fmk
// Created: 11/96
// Revision: A
//

// Description: This file contains the class implementation for
// EquiSolnAlgo. EquiSolnAlgo is an abstract base class,
// i.e. no objects of it's type can be created.  Its subclasses deifine
// the sequence of operations to be performed in the analysis by static
// equilibrium of a finite element model.
//
// What: "@(#)EquiSolnAlgo.C, revA"


#include <EquiSolnAlgo.h>
#include <AnalysisModel.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>


EquiSolnAlgo::EquiSolnAlgo(int clasTag)
    : SolutionAlgorithm(clasTag),
      theModel(0), theIntegrator(0), theSysOfEqn(0)
{

}

EquiSolnAlgo::~EquiSolnAlgo()
{

}

void
EquiSolnAlgo::setLinks(AnalysisModel& theNewModel,
                       IncrementalIntegrator& theNewIntegrator,
                       LinearSOE& theSOE)
{
    theModel = &theNewModel;
    theIntegrator = &theNewIntegrator;
    theSysOfEqn = &theSOE;
}


ConvergenceTest*
EquiSolnAlgo::getConvergenceTest(void)
{
    return 0;
}


AnalysisModel*
EquiSolnAlgo::getAnalysisModelPtr(void) const
{
    return theModel;
}



IncrementalIntegrator*
EquiSolnAlgo::getIncrementalIntegratorPtr(void) const
{
    return theIntegrator;
}



LinearSOE*
EquiSolnAlgo::getLinearSOEptr(void) const
{
    return theSysOfEqn;
}

void 
EquiSolnAlgo::switchOutputIterationOption(bool status){

  return;
}


