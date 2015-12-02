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

#ifndef Concrete01_h
#define Concrete01_h

// Written: MHS
// Description: This file contains the class definition for
// Concrete01.h adapted from Concr1.f90 (Filippou)
//   - Modified Kent-Park envelope
//   - No tension
//   - Linear unloading/reloading
//
// What: "@(#) Concrete01.h, revA"


#include <UniaxialMaterial.h>

class Concrete01 : public UniaxialMaterial
{
public:
    Concrete01 (int tag, double fpc, double eco, double fpcu, double ecu);
    Concrete01 ();
    ~Concrete01();

    const char *getClassType(void) const
    {
        return "Concrete01";
    };

    int setTrialStrain(double strain, double strainRate = 0.0);
    int setTrial (double strain, double &stress, double &tangent, double strainRate = 0.0);
    double getStrain(void);
    double getStress(void);
    double getTangent(void);
    double getInitialTangent(void)
    {
        return 2.0 * fpc / epsc0;
    }

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    UniaxialMaterial *getCopy(void);

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel,
                    FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    // AddingSensitivity:BEGIN //////////////////////////////////////////
    // int    setParameter             (const char **argv, int argc, Parameter &param);
    // int    updateParameter          (int parameterID, Information &info);
    // int    activateParameter        (int parameterID);
    // double getStressSensitivity     (int gradIndex, bool conditional);
    // int    commitSensitivity        (double strainGradient, int gradIndex, int numGrads);
    // AddingSensitivity:END ///////////////////////////////////////////

    int getVariable(const char *variable, Information &);

protected:

private:
    /*** Material Properties ***/
    double fpc;    // Compressive strength
    double epsc0;  // Strain at compressive strength
    double fpcu;   // Crushing strength
    double epscu;  // Strain at crushing strength

    /*** CONVERGED History Variables ***/
    double CminStrain;   // Smallest previous concrete strain (compression)
    double CunloadSlope; // Unloading (reloading) slope from CminStrain
    double CendStrain;   // Strain at the end of unloading from CminStrain

    /*** CONVERGED State Variables ***/
    double Cstrain;
    double Cstress;
    double Ctangent;  // Don't need Ctangent other than for revert and sendSelf/receiveSelf
    // Storing it is better than recomputing it!!!

    /*** TRIAL History Variables ***/
    double TminStrain;
    double TunloadSlope;
    double TendStrain;

    /*** TRIAL State Variables ***/
    double Tstrain;
    double Tstress;
    double Ttangent; // Not really a state variable, but declared here
    // for convenience

    void determineTrialState (double dStrain);

    void reload();
    void unload();
    void envelope();

    // AddingSensitivity:BEGIN //////////////////////////////////////////
    int parameterID;
    Matrix *SHVs;
    // AddingSensitivity:END ///////////////////////////////////////////
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


