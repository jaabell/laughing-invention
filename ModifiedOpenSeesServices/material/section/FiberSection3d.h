/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** See Copyright end of file.                                         **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.14 $
// $Date: 2008-08-26 16:47:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/FiberSection3d.h,v $

// Written: fmk
// Created: 04/01
//
// Description: This file contains the class definition for
// FiberSection3d.h. FiberSection3d provides the abstraction of a
// 3d beam section discretized by fibers. The section stiffness and
// stress resultants are obtained by summing fiber contributions.

#ifndef FiberSection3d_h
#define FiberSection3d_h

#include <SectionForceDeformation.h>
#include <Vector.h>
#include <Matrix.h>

class UniaxialMaterial;
class Fiber;
class Response;
class SectionIntegration;

class FiberSection3d : public SectionForceDeformation
{
public:
    FiberSection3d();
    FiberSection3d(int tag, int numFibers, Fiber **fibers);
    FiberSection3d(int tag, int numFibers);
    FiberSection3d(int tag, int numFibers, UniaxialMaterial **mats,
                   SectionIntegration &si);
    ~FiberSection3d();

    const char *getClassType(void) const
    {
        return "FiberSection3d";
    };

    int   setTrialSectionDeformation(const Vector &deforms);
    const Vector &getSectionDeformation(void);

    const Vector &getStressResultant(void);
    const Matrix &getSectionTangent(void);
    const Matrix &getInitialTangent(void);

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

    // Response *setResponse(const char **argv, int argc,
    //                       ostream &s);
    // int getResponse(int responseID, Information &info);

    int addFiber(Fiber &theFiber);

    // AddingSensitivity:BEGIN //////////////////////////////////////////
    // int setParameter(const char **argv, int argc, Parameter &param);

    // const Vector & getStressResultantSensitivity(int gradIndex, bool conditional);
    // const Matrix & getSectionTangentSensitivity(int gradIndex);
    // int   commitSensitivity(const Vector& sectionDeformationGradient, int gradIndex, int numGrads);

    // const Vector & getSectionDeformationSensitivity(int gradIndex);
    // AddingSensitivity:END ///////////////////////////////////////////



protected:

private:
    int numFibers, sizeFibers;       // number of fibers in the section
    UniaxialMaterial **theMaterials; // array of pointers to materials
    double   *matData;               // data for the materials [yloc and area]
    double   kData[9];               // data for ks matrix
    double   sData[3];               // data for s vector

    double QzBar, QyBar, Abar;
    double yBar;       // Section centroid
    double zBar;

    SectionIntegration *sectionIntegr;

    static ID code;

    Vector e;          // trial section deformations
    Vector *s;         // section resisting forces  (axial force, bending moment)
    Matrix *ks;        // section stiffness
};


/*
Copyright @ 1999,2000 The Regents of the University of California (The Regents).
All Rights Reserved.

The Regents grants permission, without fee and without a written license agreement,
for (a) use, reproduction, modification, and distribution of this software and its
documentation by educational, research, and non-profit entities for noncommercial
purposes only; and (b) use, reproduction and modification of this software by other
entities for internal purposes only. The above copyright notice, this paragraph and
the following three paragraphs must appear in all copies and modifications of the
software and/or documentation.

Permission to incorporate this software into products for commercial distribution
may be obtained
by contacting the University of California
Office of Technology Licensing
2150 Shattuck Avenue #510,
Berkeley, CA 94720-1620,
(510) 643-7201.

This software program and documentation are copyrighted by The Regents of the University
of California. The Regents does not warrant that the operation of the program will be
uninterrupted or error-free. The end-user understands that the program was developed
for research purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE
AND ITS DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
REGENTS GRANTS NO EXPRESS OR IMPLIED LICENSE IN ANY PATENT RIGHTS OF REGENTS BUT HAS
IMPLEMENTED AN INDIVIDUAL CONTRIBUTOR LICENSE AGREEMENT FOR THE OPENSEES PROJECT AT THE
UNIVERISTY OF CALIFORNIA, BERKELEY TO BENEFIT THE END USER.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND
ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#endif
