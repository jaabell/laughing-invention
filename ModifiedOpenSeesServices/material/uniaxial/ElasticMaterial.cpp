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

// $Revision: 1.10 $
// $Date: 2008-08-26 16:30:55 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/ElasticMaterial.cpp,v $


// Written: fmk
// Created: 07/98
// Revision: A
//
// Description: This file contains the class implementation for
// ElasticMaterial.
//
// What: "@(#) ElasticMaterial.C, revA"

#include <ElasticMaterial.h>
#include <Vector.h>
#include <Channel.h>
#include <Information.h>
// #include <Parameter.h>
#include <string.h>

#include <OPS_Globals.h>

// #include <elementAPI.h>

ElasticMaterial::ElasticMaterial(int tag, double e, double et)
    : UniaxialMaterial(tag, MAT_TAG_ElasticMaterial),
      trialStrain(0.0),  trialStrainRate(0.0),
      committedStrain(0.0),  committedStrainRate(0.0),
      Epos(e), Eneg(e), eta(et), parameterID(0)
{

}


ElasticMaterial::ElasticMaterial(int tag, double ep, double et, double en)
    : UniaxialMaterial(tag, MAT_TAG_ElasticMaterial),
      trialStrain(0.0),  trialStrainRate(0.0),
      committedStrain(0.0),  committedStrainRate(0.0),
      Epos(ep), Eneg(en), eta(et), parameterID(0)
{

}


ElasticMaterial::ElasticMaterial()
    : UniaxialMaterial(0, MAT_TAG_ElasticMaterial),
      trialStrain(0.0),  trialStrainRate(0.0),
      committedStrain(0.0),  committedStrainRate(0.0),
      Epos(0.0), Eneg(0.0), eta(0.0), parameterID(0)
{

}


ElasticMaterial::~ElasticMaterial()
{
    // does nothing
}


int
ElasticMaterial::setTrialStrain(double strain, double strainRate)
{
    trialStrain     = strain;
    trialStrainRate = strainRate;
    return 0;
}


int
ElasticMaterial::setTrial(double strain, double &stress, double &tangent, double strainRate)
{
    trialStrain     = strain;
    trialStrainRate = strainRate;

    if (trialStrain >= 0.0)
    {
        stress = Epos * trialStrain + eta * trialStrainRate;
        tangent = Epos;
    }
    else
    {
        stress = Eneg * trialStrain + eta * trialStrainRate;
        tangent = Eneg;
    }

    return 0;
}


double
ElasticMaterial::getStress(void)
{
    if (trialStrain >= 0.0)
    {
        return Epos * trialStrain + eta * trialStrainRate;
    }
    else
    {
        return Eneg * trialStrain + eta * trialStrainRate;
    }
}


double
ElasticMaterial::getTangent(void)
{
    if (trialStrain > 0.0)
    {
        return Epos;
    }
    else if (trialStrain < 0.0)
    {
        return Eneg;
    }
    else
    {
        return (Epos > Eneg) ? Epos : Eneg;
    }
}


double
ElasticMaterial::getInitialTangent(void)
{
    return (Epos > Eneg) ? Epos : Eneg;
}


int
ElasticMaterial::commitState(void)
{
    committedStrain = trialStrain;
    committedStrainRate = trialStrainRate;
    return 0;
}


int
ElasticMaterial::revertToLastCommit(void)
{
    trialStrain = committedStrain;
    trialStrainRate = committedStrainRate;
    return 0;
}


int
ElasticMaterial::revertToStart(void)
{
    trialStrain      = 0.0;
    trialStrainRate  = 0.0;
    return 0;
}


UniaxialMaterial *
ElasticMaterial::getCopy(void)
{
    ElasticMaterial *theCopy = new ElasticMaterial(this->getTag(), Epos, eta, Eneg);
    theCopy->trialStrain     = trialStrain;
    theCopy->trialStrainRate = trialStrainRate;
    theCopy->committedStrain     = committedStrain;
    theCopy->committedStrainRate = committedStrainRate;
    return theCopy;
}


int
ElasticMaterial::sendSelf(int cTag, Channel &theChannel)
{
    int res = 0;
    static Vector data(6);
    data(0) = this->getTag();
    data(1) = Epos;
    data(2) = Eneg;
    data(3) = eta;
    data(4) = committedStrain;
    data(5) = committedStrainRate;
    res = theChannel.sendVector(this->getDbTag(), cTag, data);
    if (res < 0)
    {
        std::cerr << "ElasticMaterial::sendSelf() - failed to send data\n";
    }

    return res;
}


int
ElasticMaterial::receiveSelf(int cTag, Channel &theChannel,
                             FEM_ObjectBroker &theBroker)
{
    int res = 0;
    static Vector data(6);
    res = theChannel.receiveVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        std::cerr << "ElasticMaterial::receiveSelf() - failed to receive data\n";
        Epos = Eneg = 0;
        this->setTag(0);
    }
    else
    {
        this->setTag(int(data(0)));
        Epos = data(1);
        Eneg = data(2);
        eta  = data(3);
        committedStrain = data(4);
        committedStrainRate = data(5);
        this->revertToLastCommit();
    }

    return res;
}


void
ElasticMaterial::Print(ostream &s, int flag)
{
    s << "Elastic tag: " << this->getTag() << endln;
    s << "  Epos: " << Epos << " Eneg: " << Eneg << " eta: " << eta << endln;
}


// int
// ElasticMaterial::setParameter(const char **argv, int argc, Parameter &param)
// {

//     if (strcmp(argv[0], "E") == 0)
//     {
//         param.setValue(Epos);
//         return param.addObject(1, this);
//     }
//     if (strcmp(argv[0], "Epos") == 0)
//     {
//         param.setValue(Epos);
//         return param.addObject(2, this);
//     }
//     if (strcmp(argv[0], "Eneg") == 0)
//     {
//         param.setValue(Eneg);
//         return param.addObject(3, this);
//     }
//     else if (strcmp(argv[0], "eta") == 0)
//     {
//         param.setValue(eta);
//         return param.addObject(4, this);
//     }
//     return -1;
// }


// int
// ElasticMaterial::updateParameter(int parameterID, Information &info)
// {
//     switch (parameterID)
//     {
//     case 1:
//         Epos = info.theDouble;
//         Eneg = info.theDouble;
//         return 0;
//     case 2:
//         Epos = info.theDouble;
//         return 0;
//     case 3:
//         Eneg = info.theDouble;
//         return 0;
//     case 4:
//         eta = info.theDouble;
//         return 0;
//     default:
//         return -1;
//     }
// }


// int
// ElasticMaterial::activateParameter(int paramID)
// {
//     parameterID = paramID;

//     return 0;
// }


// double
// ElasticMaterial::getStressSensitivity(int gradIndex, bool conditional)
// {
//     if (parameterID == 1)
//     {
//         return trialStrain;
//     }
//     if (parameterID == 2 && trialStrain > 0.0)
//     {
//         return trialStrain;
//     }
//     if (parameterID == 3 && trialStrain < 0.0)
//     {
//         return trialStrain;
//     }
//     if (parameterID == 4)
//     {
//         return trialStrainRate;
//     }

//     return 0.0;
// }


// double
// ElasticMaterial::getTangentSensitivity(int gradIndex)
// {
//     if (parameterID == 1)
//     {
//         return 1.0;
//     }
//     if (parameterID == 2 && trialStrain >= 0.0)
//     {
//         return 1.0;
//     }
//     if (parameterID == 3 && trialStrain <= 0.0)
//     {
//         return 1.0;
//     }

//     return 0.0;
// }


// double
// ElasticMaterial::getInitialTangentSensitivity(int gradIndex)
// {
//     if (parameterID == 1)
//     {
//         return 1.0;
//     }
//     if (parameterID == 2)
//     {
//         return 1.0;
//     }
//     if (parameterID == 3)
//     {
//         return 1.0;
//     }

//     return 0.0;
// }


// int
// ElasticMaterial::commitSensitivity(double strainGradient,
//                                    int gradIndex, int numGrads)
// {
//     // Nothing to commit ... path independent
//     return 0;
// }

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
