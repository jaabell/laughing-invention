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

// $Revision: 1.13 $
// $Date: 2006/01/10 19:15:48 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/UniaxialMaterial.cpp,v $


// File: ~/material/UniaxialMaterial.C
//
// Written: fmk
// Created: 05/98
// Revision: A
//
// Description: This file contains the class implementation for
// UniaxialMaterial.
//
// What: "@(#) UniaxialMaterial.C, revA"

#include <UniaxialMaterial.h>
#include <string.h>
#include <Information.h>
#include <MaterialResponse.h>
#include <float.h>
#include <Vector.h>
#include <iostream>
using namespace std;

UniaxialMaterial::UniaxialMaterial(int tag, int clasTag)
    : Material(tag, clasTag)
{

}

UniaxialMaterial::~UniaxialMaterial()
{
    // does nothing
}

int
UniaxialMaterial::setTrial(double strain, double& stress, double& tangent, double strainRate)
{
    int res = this->setTrialStrain(strain, strainRate);

    if (res == 0)
    {
        stress = this->getStress();
        tangent = this->getTangent();
    }
    else
    {
        cerr << "UniaxialMaterial::setTrial() - material failed in setTrialStrain()\n";
    }

    return res;
}

// default operation for strain rate is zero
double
UniaxialMaterial::getStrainRate(void)
{
    return 0.0;
}



// default operation for damping tangent is zero
double
UniaxialMaterial::getDampTangent(void)
{
    return 0.0;
}

// default operation for secant stiffness
double
UniaxialMaterial::getSecant (void)
{
    double strain = this->getStrain();
    double stress = this->getStress();

    if (strain != 0.0)
    {
        return stress / strain;
    }
    else
    {
        return this->getTangent();
    }
}

double
UniaxialMaterial::getRho(void)
{
    return 0.0;
}

UniaxialMaterial*
UniaxialMaterial::getCopy(SectionForceDeformation* s)
{
    return this->getCopy();
}

Response*
UniaxialMaterial::setResponse(const char** argv, int argc, Information& matInfo)
{
    if (argc == 0)
    {
        return 0;
    }

    // stress
    if (strcmp(argv[0], "stress") == 0)
    {
        return new MaterialResponse(this, 1, this->getStress());
    }

    // tangent
    else if (strcmp(argv[0], "tangent") == 0)
    {
        return new MaterialResponse(this, 2, this->getTangent());
    }

    // strain
    else if (strcmp(argv[0], "strain") == 0)
    {
        return new MaterialResponse(this, 3, this->getStrain());
    }

    // strain
    else if ((strcmp(argv[0], "stressStrain") == 0) ||
             (strcmp(argv[0], "stressANDstrain") == 0))
    {
        return new MaterialResponse(this, 4, Vector(2));

    }
    // otherwise unknown
    else
    {
        return 0;
    }
}

int
UniaxialMaterial::getResponse(int responseID, Information& matInfo)
{
    static Vector stressStrain(2);

    // each subclass must implement its own stuff
    switch (responseID)
    {
        case 1:
            matInfo.setDouble(this->getStress());
            return 0;

        case 2:
            matInfo.setDouble(this->getTangent());
            return 0;

        case 3:
            matInfo.setDouble(this->getStrain());
            return 0;

        case 4:
            stressStrain(0) = this->getStress();
            stressStrain(1) = this->getStrain();
            matInfo.setVector(stressStrain);
            return 0;

        default:
            return -1;
    }
}



// Nima Tafazzoli added for checking the mesh (September 2012)
int
UniaxialMaterial::CheckMesh(ofstream& checkmesh_file)
{
    return 0;
}


