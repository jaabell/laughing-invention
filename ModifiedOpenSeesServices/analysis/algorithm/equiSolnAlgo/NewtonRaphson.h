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
// $Date: 2003/02/14 23:00:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/NewtonRaphson.h,v $


#ifndef NewtonRaphson_h
#define NewtonRaphson_h

// Written: fmk
// Created: 11/96

// Description: This file contains the class definition for
// NewtonRaphson. NewtonRaphson is a class which performs a Newton-Raphson
// solution algorihm in solving the equations.
// No member functions are declared as virtual as
// it is not expected that this class will be subclassed.
//
// What: "@(#)NewtonRaphson.h, revA"

#include <EquiSolnAlgo.h>
#include <Vector.h>


class NewtonRaphson: public EquiSolnAlgo
{
public:
    NewtonRaphson(int tangent = CURRENT_TANGENT);
    NewtonRaphson(ConvergenceTest& theTest, int tangent = CURRENT_TANGENT);
    ~NewtonRaphson();

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
    IncrementalIntegrator* theIntegrator;
    LinearSOE*  theSOE ;
    int tangent;
    int global_iteration_no;
    bool output_iterations;
};

#endif


