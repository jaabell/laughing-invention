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

// $Revision: 1.5 $
// $Date: 2003/02/25 23:33:39 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/ParallelMaterial.h,v $


#ifndef ParallelMaterial_h
#define ParallelMaterial_h

// File: ~/material/ParallelMaterial.h
//
// Written: fmk
// Created: 07/98
// Revision: A
//
// Description: This file contains the class definition for
// ParallelMaterial. ParallelMaterial is an aggregation
// of UniaxialMaterial objects all considered acting in parallel.
//
// What: "@(#) ParallelMaterial.h, revA"

#include <UniaxialMaterial.h>

class ParallelMaterial : public UniaxialMaterial
{
    public:
        ParallelMaterial(int tag,
                         int numMaterial,
                         UniaxialMaterial **theMaterials);
        ParallelMaterial();
        ~ParallelMaterial();

        int setTrialStrain(double strain, double strainRate = 0.0);
        double getStrain(void);
        double getStrainRate(void);
        double getStress(void);
        double getTangent(void);
        double getDampTangent(void);
        double getInitialTangent(void);

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        UniaxialMaterial *getCopy(void);

        int sendSelf(int commitTag, Channel &theChannel);
        int receiveSelf(int commitTag, Channel &theChannel,
                     FEM_ObjectBroker &theBroker);

        void Print(ostream &s, int flag = 0);

        // Response* setResponse(const char** argv, int argc,
        //                       Information& matInformation);
        // int getResponse(int responseID, Information& matInformation);

    protected:

    private:
        double trialStrain;
        double trialStrainRate;
        int numMaterials;   // the number of UniaxialMaterials in the aggregation
        UniaxialMaterial **theModels; // an array of pointers to the UniaxialMaterials
};


#endif

