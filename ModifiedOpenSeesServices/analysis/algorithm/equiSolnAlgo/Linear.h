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
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/Linear.h,v $


// File: ~/OOP/analysis/algorithm/Linear.h
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
// What: "@(#)Linear.h, revA"

#ifndef Linear_h
#define Linear_h

#include <EquiSolnAlgo.h>

class Linear: public EquiSolnAlgo
{
    public:
        Linear();
        ~Linear();

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
        int global_iteration_no;
        bool output_iterations;

};

#endif


