/* ****************************************************************** **
** ****************************************************************** **
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
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/ModifiedNewton.h,v $


#ifndef ModifiedNewton_h
#define ModifiedNewton_h

// Written: fmk
// Created: 11/96
//
// Description: This file contains the class definition for
// ModifiedNewton. ModifiedNewton is a class which performs a modified
// Newton-Raphson  solution algorihm in solving the equations.
// No member functions are declared as virtual as
// it is not expected that this class will be subclassed.
//
// What: "@(#)ModifiedNewton.h, revA"

#include <EquiSolnAlgo.h>
#include <Vector.h>

class ConvergenceTest;

class ModifiedNewton: public EquiSolnAlgo
{
    public:
        ModifiedNewton(int tangent = CURRENT_TANGENT);
        ModifiedNewton(ConvergenceTest& theTest, int tangent = CURRENT_TANGENT);
        ~ModifiedNewton();

        int solveCurrentStep(void);
        int setConvergenceTest(ConvergenceTest* theTest);
        void switchOutputIterationOption(bool status);                     // Sumeet September 2016
        ConvergenceTest* getConvergenceTest(void);

        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int receiveSelf(int commitTag, Channel& theChannel,
                             FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    protected:

    private:
        ConvergenceTest* theTest;
        AnalysisModel*   theAnaModel;
        IncrementalIntegrator* theIncIntegratorr;
        LinearSOE*  theSOE ;
        int tangent;

        // Sumeet September, 2016 for writing iteration outputs
        int global_iteration_no;
        bool output_iterations;
};

#endif


