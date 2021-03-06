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
// $Date: 2005/06/16 21:41:03 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/EPPGapMaterial.h,v $

// File: ~/material/EPPGapMaterial.h
//
// Written: krm
// Created: 07/2000
// Revision: A
//
// Description: This file contains the class definition for
// ElasticMaterial. EPPGapMaterial provides the abstraction
// of an elastic perfectly plastic (tension only) path dependent uniaxial
// material, with an initial gap offset (force-displacement units)
// For compression only behavior, enter negative gap and ep
// Damage can accumulate through specification of damage = 1 switch,
// otherwise damage = 0
//
//                  gap ep
//                  |<>|>|
//          stress  |    +-----+ fy
//                  |   /E    /
//         ---------+--+-+---+----strain
//                  |
//                  |
//                  |
//
// What: "@(#) EPPGapMaterial.h, revA"

#ifndef EPPGapMaterial_h
#define EPPGapMaterial_h

#include <UniaxialMaterial.h>

class EPPGapMaterial : public UniaxialMaterial
{
    public:
        EPPGapMaterial(int tag, double E, double fy, double gap, double eta, int damage = 0);
        EPPGapMaterial();
        ~EPPGapMaterial();

        int setTrialStrain(double strain, double strainRate = 0.0);
        double getStrain(void);
        double getStress(void);
        double getTangent(void);
        double getInitialTangent(void);

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        UniaxialMaterial* getCopy(void);

        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    protected:

    private:
        double commitStrain;
        double trialStrain;
        double E;
        double fy;
        double gap;
        double eta;
        double maxElasticYieldStrain;
        double minElasticYieldStrain;
        int damage;

        double trialStress;      // current trial stress
        double trialTangent;     // current trial tangent
};


#endif
