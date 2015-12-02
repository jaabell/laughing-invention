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

// Written: fmk

#ifndef UniaxialMaterial_h
#define UniaxialMaterial_h

#define POS_INF_STRAIN        1.0e16
#define NEG_INF_STRAIN       -1.0e16

#include <Material.h>
class ID;
class Vector;
class Matrix;
class Information;
class Response;

class SectionForceDeformation;

class UniaxialMaterial : public Material
{
public:
    UniaxialMaterial (int tag, int classTag);
    virtual ~UniaxialMaterial();

    virtual int setTrialStrain (double strain, double strainRate = 0) = 0;
    virtual int setTrial (double strain, double &stress, double &tangent, double strainRate = 0.0);

    virtual double getStrain (void) = 0;
    virtual double getStrainRate (void);
    virtual double getStress (void) = 0;
    virtual double getTangent (void) = 0;
    virtual double getInitialTangent (void) = 0;
    virtual double getDampTangent (void);
    virtual double getRho(void);

    virtual int commitState (void) = 0;
    virtual int revertToLastCommit (void) = 0;
    virtual int revertToStart (void) = 0;

    virtual UniaxialMaterial *getCopy (void) = 0;
    virtual UniaxialMaterial *getCopy(SectionForceDeformation *s);

    // virtual Response *setResponse (const char **argv, int argc,
    //                                ostream &theOutputStream);
    // virtual int getResponse (int responseID, Information &matInformation);
    virtual bool hasFailed(void)
    {
        return false;
    }


    int CheckMesh( ofstream &checkmesh_file );


    // // AddingSensitivity:BEGIN //////////////////////////////////////////
    // virtual double getStressSensitivity     (int gradIndex, bool conditional);
    // virtual double getStrainSensitivity     (int gradIndex);
    // virtual double getTangentSensitivity(int gradIndex);
    // virtual double getInitialTangentSensitivity(int gradIndex);
    // virtual double getDampTangentSensitivity(int gradIndex);
    // virtual double getRhoSensitivity        (int gradIndex);
    // virtual int    commitSensitivity        (double strainGradient, int gradIndex, int numGrads);
    // AddingSensitivity:END ///////////////////////////////////////////

protected:

private:
};

extern bool OPS_addUniaxialMaterial(UniaxialMaterial *newComponent);
extern UniaxialMaterial *OPS_getUniaxialMaterial(int tag);
extern void OPS_clearAllUniaxialMaterial(void);

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

