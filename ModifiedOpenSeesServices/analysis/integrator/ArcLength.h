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
// $Date: 2003/02/14 23:00:46 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/integrator/ArcLength.h,v $


// File: ~/analysis/integrator/ArcLength.h
//
// Written: fmk
// Created: 07/98
// Revision: A
//
// Description: This file contains the class definition for ArcLength.
// ArcLength is an algorithmic class for perfroming a static analysis
// using the arc length scheme, that is within a load step the follwing
// constraint is enforced:
//  i=1        delta U^T delta U + alpha^2 delta lambda^2 = delta s^2
//  i>1        dU^T delta U + alpha^2 dLambda delta lambda = 0
// where dU is change in nodal displacements for step, dLambda is
// change in applied load and arcLength is a control parameter.
//
// What: "@(#) ArcLength.h, revA"

#ifndef ArcLength_h
#define ArcLength_h

#include <StaticIntegrator.h>

class LinearSOE;
class AnalysisModel;
class FE_Element;
class Vector;

class ArcLength : public StaticIntegrator
{
    public:
        ArcLength(double arcLength, double alpha = 1.0);

        ~ArcLength();

        int newStep(void);
        int update(const Vector& deltaU);
        int domainChanged(void);

        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    protected:

    private:
        double arcLength2;
        double alpha2;
        Vector* deltaUhat, *deltaUbar, *deltaU, *deltaUstep;
        Vector* phat; // the reference load vector
        double deltaLambdaStep, currentLambda;
        int signLastDeltaLambdaStep;
};

#endif

