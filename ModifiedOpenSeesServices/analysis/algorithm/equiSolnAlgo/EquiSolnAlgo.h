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
// $Date: 2003/02/14 23:00:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.h,v $


#ifndef EquiSolnAlgo_h
#define EquiSolnAlgo_h

// File: ~/OOP/analysis/algorithm/EquiSolnAlgo.h
//
// Written: fmk
// Created: 11/96
// Revision: A
//

// Description: This file contains the class definition for
// EquiSolnAlgo. EquiSolnAlgo is an abstract base class,
// i.e. no objects of it's type can be created.  Its subclasses deifine
// the sequence of operations to be performed in the analysis by static
// equilibrium of a finite element model.
//
// What: "@(#)EquiSolnAlgo.h, revA"

#include <SolutionAlgorithm.h>
#include <IncrementalIntegrator.h>

class AnalysisModel;
class LinearSOE;
class ConvergenceTest;

class EquiSolnAlgo: public SolutionAlgorithm
{
    public:
        EquiSolnAlgo(int classTag);
        virtual ~EquiSolnAlgo();

        // public functions defined for subclasses
        void setLinks(AnalysisModel& theModel,
                      IncrementalIntegrator& theIntegrator,
                      LinearSOE& theSOE);

        // virtual functions
        virtual int solveCurrentStep(void) = 0;
        virtual int setConvergenceTest(ConvergenceTest* theTest) = 0;
        virtual ConvergenceTest* getConvergenceTest(void) = 0;
        virtual void switchOutputIterationOption(bool status)=0; // Sumeet September, 2016

        virtual void Print(ostream& s, int flag = 0) = 0;

        // the following are not protected as convergence test
        // may need access to them
        AnalysisModel*           getAnalysisModelPtr(void) const;
        IncrementalIntegrator*   getIncrementalIntegratorPtr(void) const;
        LinearSOE*               getLinearSOEptr(void) const;

    protected:

    private:
        AnalysisModel*     theModel;
        IncrementalIntegrator* theIntegrator;
        LinearSOE*         theSysOfEqn;

};

#endif


