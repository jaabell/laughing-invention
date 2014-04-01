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
**                                                                    **
** Additions and changes by:                                          **
**   Boris Jeremic (@ucdavis.edu)                                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.17 $
// $Date: 2004/07/20 22:39:02 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/NDMaterialLT.cpp,v $

// File: ~/material/NDMaterialLT.C
//
// Written: MHS
// Created: Feb 2000
// Revision: A
//
// Description: This file contains the class implementation for NDMaterialLT.
//
// What: "@(#) NDMaterialLT.C, revA"

#include <nDarray.h>
#include "NDMaterialLT.h"
#include <Information.h>
#include <OPS_Globals.h>
//#include <DTensor2.h>
//#include <Vector.h>
//#include <stresst.h>
//#include <straint.h>
#include <MaterialResponse.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
using namespace std;


DTensor2 NDMaterialLT::errMatrix      (3, 3, 0.0);
DTensor1 NDMaterialLT::errVector      (3,    0.0);
DTensor2 NDMaterialLT::errTensor      (3, 3, 0.0);
DTensor4 NDMaterialLT::errTensor4     (3, 3, 3, 3, 0.0);
DTensor2 NDMaterialLT::errTensor2     (3, 3, 0.0);
DTensor2 NDMaterialLT::errstresstensor(3, 3, 0.0);
DTensor2 NDMaterialLT::errstraintensor(3, 3, 0.0);

NDMaterialLT::NDMaterialLT(int tag, int classTag)
    : Material(tag, classTag)
{

}

NDMaterialLT::NDMaterialLT()
    : Material(0, 0)
{

}

NDMaterialLT::~NDMaterialLT()
{

}


double
NDMaterialLT::getRho(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getE(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getnu(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getpsi(void)
{
    return 0.0;
}

const DTensor1 &
NDMaterialLT::getCommittedStress(void)
{
    return this->getStress();
}

const DTensor1 &
NDMaterialLT::getCommittedStrain(void)
{
    return this->getStrain();
}






// methods to set and retrieve state.
int
NDMaterialLT::setTrialStrain(const DTensor1 &v)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor1 &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor1 &v, const DTensor1 &r)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor1 &v, const DTensor1 &r) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor2  &v)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor2  &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor2  &v, const DTensor2  &r)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor2  &v, const DTensor2  &r) -- subclass responsibility\n";
    return -1;
}





int
NDMaterialLT::setTrialStrainIncr(const DTensor1 &v)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor1 &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor1 &v, const DTensor1 &r)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor1 &v, const DTensor1 &r) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor2  &v)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor2  &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor2  &v, const DTensor2  &r)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor2  &v, const DTensor2  &r) -- subclass responsibility\n";
    return -1;
}













const DTensor2 &
NDMaterialLT::getTangent(void)
{
    cerr << "NDMaterialLT::getTangent -- subclass responsibility\n";
    return errTensor2;
}

const DTensor1 &
NDMaterialLT::getStress(void)
{
    cerr << "NDMaterialLT::getStress -- subclass responsibility\n";
    return errVector;
}

const DTensor1 &
NDMaterialLT::getStrain(void)
{
    cerr << "NDMaterialLT::getStrain -- subclass responsibility\n";
    return errVector;
}


const DTensor4 &
NDMaterialLT::getTangentTensor(void)
{
    cerr << "NDMaterialLT::getTangentTensor  -- subclass responsibility\n";
    return errTensor4;
}

const DTensor2    &NDMaterialLT::getStressTensor(void)
{
    cerr << "NDMaterialLT::getStressTensor    -- subclass responsibility\n";
    return errTensor;
}

const DTensor2    &NDMaterialLT::getStrainTensor(void)
{
    cerr << "NDMaterialLT::getStrainTensor    -- subclass responsibility\n";
    return errstraintensor;
}

const DTensor2    &NDMaterialLT::getPlasticStrainTensor(void)
{
    cerr << "NDMaterialLT::getPlasticStrainTensor    -- subclass responsibility\n";
    return errstraintensor;
}


Response *
NDMaterialLT::setResponse (const char **argv, int argc, Information &matInfo)
{
    if (strcmp(argv[0], "stress") == 0 || strcmp(argv[0], "stresses") == 0)
    {
        return 0;
    }
    //  return new MaterialResponse(this, 1, DTensor1_to_nDarray(this->getStress()));

    else if (strcmp(argv[0], "strain") == 0 || strcmp(argv[0], "strains") == 0)
    {
        return 0;
    }
    //return new MaterialResponse(this, 2, this->getStrain());

    else if (strcmp(argv[0], "tangent") == 0)
    {
        return 0;
    }
    //return new MaterialResponse(this, 3, this->getTangent());

    else
    {
        return 0;
    }
}

// TODO: Need to interface LTensor with old tensors so that information can be passed.
//   Optionally we can rewrite or overload functions everywhere to also work with LTensor types.
int
NDMaterialLT::getResponse (int responseID, Information &matInfo)
{
    switch (responseID)
    {
        case 1:
            break;//return matInfo.setVector(this->getStress());

        case 2:
            break;//return matInfo.setVector(this->getStrain());

        case 3:
            break;//return matInfo.setDTensor2(this->getTangent());

        default:
            return -1;
    }

    return -1;
}


// Nima Tafazzoli added for checking the mesh (September 2012)
int
NDMaterialLT::CheckMesh(ofstream &checkmesh_file)
{
    return 0;
}

// J.Abell added June 20, 2013 -> needed for Concrete3D model -> to partially fix mesh sensitivity during softening of concrete
void NDMaterialLT::setVolume(double vol)
{
    volume = vol;
}

int NDMaterialLT::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
{
    cerr << "NDMaterialLT::describeSelf -> Subclass responsability" << endl;
    return -1;
}

int NDMaterialLT::sendSelf(int commitTag, Channel &theChannel)
{
    cerr << "NDMaterialLT::sendSelf -> Subclass responsability" << endl;
    return -1;
}
int NDMaterialLT::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    cerr << "NDMaterialLT::recvSelf -> Subclass responsability" << endl;
    return -1;
}

void NDMaterialLT::Print(ostream &s, int flag)
{
    cerr << "NDMaterialLT::Print -> Subclass responsability" << endl;
}


int NDMaterialLT::commitState(void)
{
    cerr << "NDMaterialLT::commitState -> Subclass responsability" << endl;
    return -1;
}

int NDMaterialLT::revertToLastCommit(void)
{
    cerr << "NDMaterialLT::revertToLastCommit -> Subclass responsability" << endl;
    return -1;
}

int NDMaterialLT::revertToStart(void)
{
    cerr << "NDMaterialLT::revertToStart -> Subclass responsability" << endl;
    return -1;
}

NDMaterialLT *NDMaterialLT::getCopy(void)
{
    cerr << "NDMaterialLT::getCopy -> Subclass responsability" << endl;
    return 0;
}


const char *NDMaterialLT::getType(void) const
{
    cerr << "NDMaterialLT::getType -> Subclass responsability" << endl;
    return 0;
}
