///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               January 2014
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////



// Original file, from OpenSees project, see ORIGINAL header below,
// was significantly changed, improved, massacred and massaged, so that
// this implementation/file now constitutes quite a novel development.
// In addition, ORIGINAL OpenSees license (see below) assigned
// (wrongly) copyright to the
// Regents of the University of California.
// ORIGINAL license also gave a number of permissions (see below)
// that we used.
//
//--------------------------------------------------------------------------
// Original OpenSees License:
// Copyright @ 1999,2000  The Regents of the University of California.
// All Rights Reserved.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for educational, research and non-profit purposes, without
// fee, and without a written agreement is hereby granted, provided that the
// above copyright notice, this paragraph and the following three paragraphs
// appear in all copies.
//
// Permission to incorporate this software into commercial products may be
// obtained by contacting the University of California.
// [Office of Technology Licensing,
// 2150 Shattuck Avenue #150
// Berkeley, CA 94720-1620,
// (510) 643-7201]
//
// This software program and documentation are copyrighted by The Regents of
// the University of California. The software program and documentation are
// supplied "as is", without any accompanying services from The Regents. The
// Regents does not warrant that the operation of the program will be
// uninterrupted or error-free. The end-user understands that the program was
// developed for research purposes and is advised not to rely exclusively on
// the program for any reason.
//
// IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
// DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
// LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
// DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE. THE UNIVERSITY OF CALIFORNIA SPECIFICALLY
// DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
// SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
// CALIFORNIA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
//--------------------------------------------------------------------------
//
//
// Original header for this file:






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

// $Revision: 1.19 $
// $Date: 2004/02/24 20:50:58 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/ElasticIsotropicMaterial.cpp,v $


// File: ~/material/ElasticIsotropicMaterial.C
//
// Written: MHS
// Created: Feb 2000
// Revision: A
// Boris Jeremic (@ucdavis.edu) 19June2002 added getE, getnu
//
// Description: This file contains the class implementation for ElasticIsotropicMaterial.
//
// What: "@(#) ElasticIsotropicMaterial.C, revA"

#include <string.h>

#include <ElasticIsotropicMaterial.h>
//#include <ElasticIsotropicPlaneStress2D.h>
//#include <ElasticIsotropicPlaneStrain2D.h>
#include <ElasticIsotropicAxiSymm.h>
#include <ElasticIsotropic3D.h>
#include <PressureDependentElastic3D.h>
#include <ElasticIsotropicPlateFiber.h>
#include <ElasticIsotropicBeamFiber.h>

#include <Tensor.h>
#include <Channel.h>

#include <OPS_Globals.h>

ElasticIsotropicMaterial::ElasticIsotropicMaterial
(int tag, int classTag, double e, double nu, double r)
    : NDMaterial(tag, classTag), E(e), v(nu), rho(r)
{

}

ElasticIsotropicMaterial::ElasticIsotropicMaterial
(int tag, double e, double nu, double r)
    : NDMaterial(tag, ND_TAG_ElasticIsotropic), E(e), v(nu), rho(r)
{

}

ElasticIsotropicMaterial::~ElasticIsotropicMaterial()
{

}

double
ElasticIsotropicMaterial::getRho()
{
    return rho ;
}


// Boris Jeremic (@ucdavis.edu) 19June2002
double ElasticIsotropicMaterial::getE()
{
    return E;
}

// Boris Jeremic (@ucdavis.edu) 19June2002
double ElasticIsotropicMaterial::getnu()
{
    return v;
}



NDMaterial *
ElasticIsotropicMaterial::getCopy (const char *type)
{
    // Boris Jeremic out 27Mar2009
    //    if (strcmp(type,"PlaneStress2D") == 0 || strcmp(type,"PlaneStress") == 0)
    //    {
    //  ElasticIsotropicPlaneStress2D *theModel;
    //  theModel = new ElasticIsotropicPlaneStress2D (this->getTag(), E, v, rho);
    //      // DOES NOT COPY sigma, D, and epsilon ...
    //      // This function should only be called during element instantiation, so
    //      // no state determination is performed on the material model object
    //      // prior to copying the material model (calling this function)
    //  return theModel;
    //    }
    //
    //    else if (strcmp(type,"PlaneStrain2D") == 0 || strcmp(type,"PlaneStrain") == 0)
    //    {
    //  ElasticIsotropicPlaneStrain2D *theModel;
    //  theModel = new ElasticIsotropicPlaneStrain2D (this->getTag(), E, v, rho);
    //      // DOES NOT COPY sigma, D, and epsilon ...
    //      // This function should only be called during element instantiation, so
    //      // no state determination is performed on the material model object
    //      // prior to copying the material model (calling this function)
    //  return theModel;
    //    }
    //    else if (strcmp(type,"AxiSymmetric2D") == 0 || strcmp(type,"AxiSymmetric") == 0)
    //    {
    //  ElasticIsotropicAxiSymm *theModel;
    //  theModel = new ElasticIsotropicAxiSymm(this->getTag(), E, v, rho);
    //      // DOES NOT COPY sigma, D, and epsilon ...
    //      // This function should only be called during element instantiation, so
    //      // no state determination is performed on the material model object
    //      // prior to copying the material model (calling this function)
    //  return theModel;
    //    }
    /////////////////////////////////
    //    else if (strcmp(type,"ThreeDimensional") == 0 ||
    //       strcmp(type,"3D") == 0)
    //    {
    ElasticIsotropic3D *theModel;
    theModel = new ElasticIsotropic3D (this->getTag(), E, v, rho);
    // DOES NOT COPY sigma, D, and epsilon ...
    // This function should only be called during element instantiation, so
    // no state determination is performed on the material model object
    // prior to copying the material model (calling this function)
    return theModel;
    //    }
    ///////////////////////////////////
    ////    else if (strcmp(type,"PlateFiber") == 0)
    ////    {
    ////    ElasticIsotropicPlateFiber *theModel;
    ////    theModel = new ElasticIsotropicPlateFiber(this->getTag(), E, v, rho);
    ////        // DOES NOT COPY sigma, D, and epsilon ...
    ////        // This function should only be called during element instantiation, so
    ////        // no state determination is performed on the material model object
    ////        // prior to copying the material model (calling this function)
    ////    return theModel;
    ////    }
    //    else if (strcmp(type,"BeamFiber") == 0)
    //    {
    //  ElasticIsotropicBeamFiber *theModel;
    //  theModel = new ElasticIsotropicBeamFiber(this->getTag(), E, v, rho);
    //      // DOES NOT COPY sigma, D, and epsilon ...
    //      // This function should only be called during element instantiation, so
    //      // no state determination is performed on the material model object
    //      // prior to copying the material model (calling this function)
    //  return theModel;
    //    }
    //
    //
    //    // Handle other cases
    //    else
    //    {
    //      cerr << "ElasticIsotropicMaterial::getModel failed to get model: " << type << endln;
    //      exit(-1);
    //    }
    //
    //    return 0;
}

int
ElasticIsotropicMaterial::setTrialStrain (const Vector &v)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrain -- subclass responsibility\n";
    exit(-1);
    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrain (const Vector &v, const Vector &rate)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrain -- subclass responsibility\n";
    exit(-1);
    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrainIncr (const Vector &v)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrainIncr -- subclass responsibility\n";
    exit(-1);
    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrainIncr (const Vector &v, const Vector &rate)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrainIncr -- subclass responsibility\n";
    exit(-1);
    return -1;
}

const Matrix &
ElasticIsotropicMaterial::getTangent (void)
{
    cerr << "ElasticIsotropicMaterial::getTangent -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    Matrix *ret = new Matrix();
    return *ret;
}

const Matrix &
ElasticIsotropicMaterial::getInitialTangent (void)
{
    cerr << "ElasticIsotropicMaterial::getInitialTangent -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    Matrix *ret = new Matrix();
    return *ret;
}

const Vector &
ElasticIsotropicMaterial::getStress (void)
{
    cerr << "ElasticIsotropicMaterial::getStress -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    Vector *ret = new Vector();
    return *ret;
}

const Vector &
ElasticIsotropicMaterial::getStrain (void)
{
    cerr << "ElasticIsotropicMaterial::getStrain -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    Vector *ret = new Vector();
    return *ret;
}

int
ElasticIsotropicMaterial::setTrialStrain (const Tensor &v)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrain -- subclass responsibility\n";
    exit(-1);

    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrain (const Tensor &v, const Tensor &r)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrain -- subclass responsibility\n";
    exit(-1);

    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrainIncr (const Tensor &v)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrainIncr -- subclass responsibility\n";
    exit(-1);

    return -1;
}

int
ElasticIsotropicMaterial::setTrialStrainIncr (const Tensor &v, const Tensor &r)
{
    cerr << "ElasticIsotropicMaterial::setTrialStrainIncr -- subclass responsibility\n";

    return -1;
}

const Tensor &
ElasticIsotropicMaterial::getTangentTensor (void)
{
    cerr << "ElasticIsotropicMaterial::getTangentTensor -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    Tensor *t = new Tensor;
    return *t;
}

const stresstensor &ElasticIsotropicMaterial::getStressTensor (void)
{
    cerr << "ElasticIsotropicMaterial::getStressTensor -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    stresstensor *t = new stresstensor();
    return *t;
}

const straintensor &ElasticIsotropicMaterial::getStrainTensor (void)
{
    cerr << "ElasticIsotropicMaterial::getStrainTensor -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    stresstensor *t = new stresstensor();
    return *t;
}

const straintensor &ElasticIsotropicMaterial::getPlasticStrainTensor (void)
{
    cerr << "ElasticIsotropicMaterial::getPlasticStrainTensor -- subclass responsibility\n";
    exit(-1);

    // Just to make it compile
    stresstensor *t = new stresstensor();
    return *t;
}

int
ElasticIsotropicMaterial::commitState (void)
{
    cerr << "ElasticIsotropicMaterial::commitState -- subclass responsibility\n";
    exit(-1);
    return -1;
}

int
ElasticIsotropicMaterial::revertToLastCommit (void)
{
    cerr << "ElasticIsotropicMaterial::revertToLastCommit -- subclass responsibility\n";
    exit(-1);

    return -1;
}

int
ElasticIsotropicMaterial::revertToStart (void)
{
    cerr << "ElasticIsotropicMaterial::revertToStart -- subclass responsibility\n";
    exit(-1);
    return -1;
}

NDMaterial *
ElasticIsotropicMaterial::getCopy (void)
{
    cerr << "ElasticIsotropicMaterial::getCopy -- subclass responsibility\n";
    exit(-1);
    return 0;
}

const char *
ElasticIsotropicMaterial::getType (void) const
{
    cerr << "ElasticIsotropicMaterial::getType -- subclass responsibility\n";
    exit(-1);

    return 0;
}

int
ElasticIsotropicMaterial::getOrder (void) const
{
    cerr << "ElasticIsotropicMaterial::getOrder -- subclass responsibility\n";
    exit(-1);
    return -1;
}

int
ElasticIsotropicMaterial::sendSelf (int commitTag, Channel &theChannel)
{
    int res = 0;

    static Vector data(4);

    data(0) = this->getTag();
    data(1) = E;
    data(2) = v;
    data(3) = rho;

    res += theChannel.sendVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Vector\n";
        return res;
    }

    return res;
}

int
ElasticIsotropicMaterial::receiveSelf (int commitTag, Channel &theChannel,
                                       FEM_ObjectBroker &theBroker)
{
    int res = 0;

    static Vector data(4);

    res += theChannel.receiveVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Vector\n";
        return res;
    }

    this->setTag((int)data(0));
    E = data(1);
    v = data(2);
    rho = data(3);

    return res;
}

void
ElasticIsotropicMaterial::Print (ostream &s, int flag)
{
    s << "ElasticIsotropicMaterial::" << endln;
    s << "\tTag: " << this->getTag() << endln;
    s << "\tElastic_Modulus: " << E << endln;
    s << "\tPoissons_Ratio: " << v << endln;
    s << "\tDensity: " << rho << endln;

    return;
}

int
ElasticIsotropicMaterial::setParameter(const char **argv, int argc, Information &eleInformation)
{
    return -1;
}

int
ElasticIsotropicMaterial::updateParameter(int parameterID, Information &info)
{
    return -1;
}
