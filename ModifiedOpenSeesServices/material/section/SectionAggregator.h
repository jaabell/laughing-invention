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

// $Revision: 1.10 $
// $Date: 2003/02/25 23:33:34 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/SectionAggregator.h,v $


// File: ~/section/SectionAggregator.h
//
// Written: MHS
// Created: Jun 2000
// Revision: A
//
// Description: This file contains the class definition for
// SectionAggregator.  SectionAggregator decorates an MP
// section (couple bending and axial) with an uncoupled shear
// relation.
//
// What: "@(#) SectionAggregator.h, revA"

#ifndef SectionAggregator_h
#define SectionAggregator_h

#include <SectionForceDeformation.h>
#include <UniaxialMaterial.h>

#include <Vector.h>
#include <Matrix.h>

#include <iostream>

class SectionAggregator : public SectionForceDeformation
{
    public:
        SectionAggregator();

        SectionAggregator(int tag, SectionForceDeformation &theSection,
                          int numAdditions, UniaxialMaterial **theAdditions,
                          const ID &code);
        SectionAggregator(int tag, int numAdditions,
                          UniaxialMaterial **theAdditions, const ID &code);
        SectionAggregator(int tag, SectionForceDeformation &thesection,
                          UniaxialMaterial &theAddition, int c);

        ~SectionAggregator();

        int   setTrialSectionDeformation(const Vector &deforms);
        const Vector &getSectionDeformation(void);

        const Vector &getStressResultant(void);
        const Matrix &getSectionTangent(void);
        const Matrix &getInitialTangent(void);
        const Matrix &getSectionFlexibility(void);
        const Matrix &getInitialFlexibility(void);

        int   commitState(void);
        int   revertToLastCommit(void);
        int   revertToStart(void);

        SectionForceDeformation *getCopy(void);
        const ID &getType (void);
        int getOrder (void) const;

        int sendSelf(int cTag, Channel &theChannel);
        int receiveSelf(int cTag, Channel &theChannel,
                     FEM_ObjectBroker &theBroker);

        void Print(ostream &s, int flag = 0);

        // Response *setResponse(const char **argv, int argc, Information &info);
        // int getResponse(int responseID, Information &info);

        int setVariable(const char *argv);
        int getVariable(int variableID, double &info);

    protected:

    private:

        SectionForceDeformation *theSection;
        UniaxialMaterial **theAdditions;

        ID *matCodes;
        int numMats;

        Vector *e;    // Storage for section deformations
        Vector *s;    // Storage for stress resultants
        Matrix *ks;   // Storage for section stiffness
        Matrix *fs;   // Storage for section flexibility
        ID     *theCode;     // Storage for section type information

        int otherDbTag;

        static double workArea[];
        static int codeArea[];
};

#endif
