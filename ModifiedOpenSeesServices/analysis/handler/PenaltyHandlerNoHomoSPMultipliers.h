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

// $Revision$
// $Date$
// $Source$

// Written: fmk
// Created: 2002.
//
// Description: This file contains the class definition for
// PenaltyHandlerNoHomoSPMultipliers. PenaltyHandlerNoHomoSPMultipliers is a
// constraint handler for handling constraints using the penalty method.
// for each element and degree-of-freedom at a node it constructs regular
// FE_Element and DOF_Groups; for each non homogeneous SP_Constraint and
// all lMP_Constraint PenaltyNoHomeSPSP_FE and PenaltyNoHomeSPMP_FE elements
// are created.
//
// What: "@(#) PenaltyHandlerNoHomoSPMultipliers.h, revA"

#ifndef PenaltyHandlerNoHomoSPMultipliers_h
#define PenaltyHandlerNoHomoSPMultipliers_h

#include <ConstraintHandler.h>

class FE_Element;
class DOF_Group;

class PenaltyHandlerNoHomoSPMultipliers : public ConstraintHandler
{
    public:
        PenaltyHandlerNoHomoSPMultipliers(double alphaSP, double alphaMP);
        ~PenaltyHandlerNoHomoSPMultipliers();

        int handle(const ID* nodesNumberedLast = 0);
        void clearAll(void);

        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int recvSelf(int commitTag, Channel& theChannel,
                             FEM_ObjectBroker& theBroker);

    protected:

    private:
        double alphaSP;
        double alphaMP;
        FE_Element**  theFEs;
        DOF_Group**   theDOFs;
        int     numFE;
        int     numDOF;
};

#endif




