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

// $Revision: 1.14 $
// $Date: 2005/01/25 21:55:36 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/Steel01.cpp,v $

// Written: MHS
// Created: 06/99
// Revision: A
//
// Description: This file contains the class implementation for
// Steel01.
//
// What: "@(#) Steel01.C, revA"


#include <Steel01.h>
#include <Vector.h>
#include <Matrix.h>
#include <Channel.h>
#include <Information.h>
#include <math.h>
#include <float.h>
#include <iostream>
using namespace std;


Steel01::Steel01
(int tag, double FY, double E, double B,
 double A1, double A2, double A3, double A4):
    UniaxialMaterial(tag, MAT_TAG_Steel01),
    fy(FY), E0(E), b(B), a1(A1), a2(A2), a3(A3), a4(A4)
{
    // Sets all history and state variables to initial values
    // History variables
    CminStrain = 0.0;
    CmaxStrain = 0.0;
    CshiftP = 1.0;
    CshiftN = 1.0;
    Cloading = 0;

    TminStrain = 0.0;
    TmaxStrain = 0.0;
    TshiftP = 1.0;
    TshiftN = 1.0;
    Tloading = 0;

    // State variables
    Cstrain = 0.0;
    Cstress = 0.0;
    Ctangent = E0;

    Tstrain = 0.0;
    Tstress = 0.0;
    Ttangent = E0;


}

Steel01::Steel01(): UniaxialMaterial(0, MAT_TAG_Steel01),
    fy(0.0), E0(0.0), b(0.0), a1(0.0), a2(0.0), a3(0.0), a4(0.0)
{



}

Steel01::~Steel01 ()
{

}

int Steel01::setTrialStrain (double strain, double strainRate)
{
    // Reset history variables to last converged state
    TminStrain = CminStrain;
    TmaxStrain = CmaxStrain;
    TshiftP = CshiftP;
    TshiftN = CshiftN;
    Tloading = Cloading;

    // Set trial strain
    Tstrain = strain;

    // Determine change in strain from last converged state
    double dStrain = Tstrain - Cstrain;

    // Calculate the trial state given the trial strain
    if (fabs(dStrain) > DBL_EPSILON)
    {
        determineTrialState (dStrain);
    }

    return 0;
}

int Steel01::setTrial (double strain, double& stress, double& tangent, double strainRate)
{
    // Reset history variables to last converged state
    TminStrain = CminStrain;
    TmaxStrain = CmaxStrain;
    TshiftP = CshiftP;
    TshiftN = CshiftN;
    Tloading = Cloading;

    // Set trial strain
    Tstrain = strain;

    // Determine change in strain from last converged state
    double dStrain;
    dStrain = Tstrain - Cstrain;

    // Calculate the trial state given the trial strain
    if (fabs(dStrain) > DBL_EPSILON)
    {
        determineTrialState (dStrain);
    }

    stress = Tstress;
    tangent = Ttangent;

    return 0;
}

void Steel01::determineTrialState (double dStrain)
{
    double fyOneMinusB = fy * (1.0 - b);

    double Esh = b * E0;
    double epsy = fy / E0;

    double c1 = Esh * Tstrain;

    double c2 = TshiftN * fyOneMinusB;

    double c3 = TshiftP * fyOneMinusB;

    double c = Cstress + E0 * dStrain;

    /**********************************************************
       removal of the following lines due to problems with
    optimization may be required (e.g. on gnucc compiler
           with optimization turned on & -ffloat-store option not
           used) .. replace them with line that follows but which
           now requires 2 function calls to achieve same result !!
        ************************************************************/

    double c1c3 = c1 + c3;

    if (c1c3 < c)
    {
        Tstress = c1c3;
    }
    else
    {
        Tstress = c;
    }

    double c1c2 = c1 - c2;

    if (c1c2 > Tstress)
    {
        Tstress = c1c2;
    }

    /* ***********************************************************
    and replace them with:

    Tstress = fmax((c1-c2), fmin((c1+c3),c));
    **************************************************************/

    if (fabs(Tstress - c) < DBL_EPSILON)
    {
        Ttangent = E0;
    }
    else
    {
        Ttangent = Esh;
    }

    //
    // Determine if a load reversal has occurred due to the trial strain
    //

    // Determine initial loading condition
    if (Tloading == 0 && dStrain != 0.0)
    {
        if (dStrain > 0.0)
        {
            Tloading = 1;
        }
        else
        {
            Tloading = -1;
        }
    }

    // Transition from loading to unloading, i.e. positive strain increment
    // to negative strain increment
    if (Tloading == 1 && dStrain < 0.0)
    {
        Tloading = -1;

        if (Cstrain > TmaxStrain)
        {
            TmaxStrain = Cstrain;
        }

        TshiftN = 1 + a1 * pow((TmaxStrain - TminStrain) / (2.0 * a2 * epsy), 0.8);
    }

    // Transition from unloading to loading, i.e. negative strain increment
    // to positive strain increment
    if (Tloading == -1 && dStrain > 0.0)
    {
        Tloading = 1;

        if (Cstrain < TminStrain)
        {
            TminStrain = Cstrain;
        }

        TshiftP = 1 + a3 * pow((TmaxStrain - TminStrain) / (2.0 * a4 * epsy), 0.8);
    }
}

void Steel01::detectLoadReversal (double dStrain)
{
    // Determine initial loading condition
    if (Tloading == 0 && dStrain != 0.0)
    {
        if (dStrain > 0.0)
        {
            Tloading = 1;
        }
        else
        {
            Tloading = -1;
        }
    }

    double epsy = fy / E0;

    // Transition from loading to unloading, i.e. positive strain increment
    // to negative strain increment
    if (Tloading == 1 && dStrain < 0.0)
    {
        Tloading = -1;

        if (Cstrain > TmaxStrain)
        {
            TmaxStrain = Cstrain;
        }

        TshiftN = 1 + a1 * pow((TmaxStrain - TminStrain) / (2.0 * a2 * epsy), 0.8);
    }

    // Transition from unloading to loading, i.e. negative strain increment
    // to positive strain increment
    if (Tloading == -1 && dStrain > 0.0)
    {
        Tloading = 1;

        if (Cstrain < TminStrain)
        {
            TminStrain = Cstrain;
        }

        TshiftP = 1 + a3 * pow((TmaxStrain - TminStrain) / (2.0 * a4 * epsy), 0.8);
    }
}

double Steel01::getStrain ()
{
    return Tstrain;
}

double Steel01::getStress ()
{
    return Tstress;
}

double Steel01::getTangent ()
{
    return Ttangent;
}

int Steel01::commitState ()
{
    // History variables
    CminStrain = TminStrain;
    CmaxStrain = TmaxStrain;
    CshiftP = TshiftP;
    CshiftN = TshiftN;
    Cloading = Tloading;

    // State variables
    Cstrain = Tstrain;
    Cstress = Tstress;
    Ctangent = Ttangent;

    return 0;
}

int Steel01::revertToLastCommit ()
{
    // Reset trial history variables to last committed state
    TminStrain = CminStrain;
    TmaxStrain = CmaxStrain;
    TshiftP = CshiftP;
    TshiftN = CshiftN;
    Tloading = Cloading;

    // Reset trial state variables to last committed state
    Tstrain = Cstrain;
    Tstress = Cstress;
    Ttangent = Ctangent;

    return 0;
}

int Steel01::revertToStart ()
{
    // History variables
    CminStrain = 0.0;
    CmaxStrain = 0.0;
    CshiftP = 1.0;
    CshiftN = 1.0;
    Cloading = 0;

    TminStrain = 0.0;
    TmaxStrain = 0.0;
    TshiftP = 1.0;
    TshiftN = 1.0;
    Tloading = 0;

    // State variables
    Cstrain = 0.0;
    Cstress = 0.0;
    Ctangent = E0;

    Tstrain = 0.0;
    Tstress = 0.0;
    Ttangent = E0;


    return 0;
}

UniaxialMaterial* Steel01::getCopy ()
{
    Steel01* theCopy = new Steel01(this->getTag(), fy, E0, b,
                                   a1, a2, a3, a4);

    // Converged history variables
    theCopy->CminStrain = CminStrain;
    theCopy->CmaxStrain = CmaxStrain;
    theCopy->CshiftP = CshiftP;
    theCopy->CshiftN = CshiftN;
    theCopy->Cloading = Cloading;

    // Trial history variables
    theCopy->TminStrain = TminStrain;
    theCopy->TmaxStrain = TmaxStrain;
    theCopy->TshiftP = TshiftP;
    theCopy->TshiftN = TshiftN;
    theCopy->Tloading = Tloading;

    // Converged state variables
    theCopy->Cstrain = Cstrain;
    theCopy->Cstress = Cstress;
    theCopy->Ctangent = Ctangent;

    // Trial state variables
    theCopy->Tstrain = Tstrain;
    theCopy->Tstress = Tstress;
    theCopy->Ttangent = Ttangent;

    return theCopy;
}

int Steel01::sendSelf (int commitTag, Channel& theChannel)
{
    int res = 0;
    static Vector data(16);
    data(0) = this->getTag();

    // Material properties
    data(1) = fy;
    data(2) = E0;
    data(3) = b;
    data(4) = a1;
    data(5) = a2;
    data(6) = a3;
    data(7) = a4;

    // History variables from last converged state
    data(8) = CminStrain;
    data(9) = CmaxStrain;
    data(10) = CshiftP;
    data(11) = CshiftN;
    data(12) = Cloading;

    // State variables from last converged state
    data(13) = Cstrain;
    data(14) = Cstress;
    data(15) = Ctangent;

    // Data is only sent after convergence, so no trial variables
    // need to be sent through data vector

    res = theChannel.sendVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "Steel01::sendSelf() - failed to send data\n";
    }

    return res;
}

int Steel01::receiveSelf (int commitTag, Channel& theChannel,
                       FEM_ObjectBroker& theBroker)
{
    int res = 0;
    static Vector data(16);
    res = theChannel.receiveVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "Steel01::receiveSelf() - failed to receive data\n";
        this->setTag(0);
    }
    else
    {
        this->setTag(int(data(0)));

        // Material properties
        fy = data(1);
        E0 = data(2);
        b = data(3);
        a1 = data(4);
        a2 = data(5);
        a3 = data(6);
        a4 = data(7);

        // History variables from last converged state
        CminStrain = data(8);
        CmaxStrain = data(9);
        CshiftP = data(10);
        CshiftN = data(11);
        Cloading = int(data(12));

        // Copy converged history values into trial values since data is only
        // sent (received) after convergence
        TminStrain = CminStrain;
        TmaxStrain = CmaxStrain;
        TshiftP = CshiftP;
        TshiftN = CshiftN;
        Tloading = Cloading;

        // State variables from last converged state
        Cstrain = data(13);
        Cstress = data(14);
        Ctangent = data(15);

        // Copy converged state values into trial values
        Tstrain = Cstrain;
        Tstress = Cstress;
        Ttangent = Ctangent;
    }

    return res;
}

void Steel01::Print (ostream& s, int flag)
{
    s << "Steel01 tag: " << this->getTag() << endln;
    s << "  fy: " << fy << " ";
    s << "  E0: " << E0 << " ";
    s << "  b:  " << b << " ";
    s << "  a1: " << a1 << " ";
    s << "  a2: " << a2 << " ";
    s << "  a3: " << a3 << " ";
    s << "  a4: " << a4 << " ";
}


