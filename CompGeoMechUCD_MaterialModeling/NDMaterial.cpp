///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:
// DATE:
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
**                                                                    **
** Additions and changes by:                                          **
**   Boris Jeremic (@ucdavis.edu)                                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.17 $
// $Date: 2004/07/20 22:39:02 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/NDMaterial.cpp,v $

// File: ~/material/NDMaterial.C
//
// Written: MHS
// Created: Feb 2000
// Revision: A
//
// Description: This file contains the class implementation for NDMaterial.
//
// What: "@(#) NDMaterial.C, revA"

#include <NDMaterial.h>
#include <Information.h>
#include <OPS_Globals.h>
#include <Matrix.h>
#include <Vector.h>
#include <stresst.h>
#include <straint.h>
// #include <Material3.h>

Matrix NDMaterial::errMatrix(1, 1);
Vector NDMaterial::errVector(1);
Tensor NDMaterial::errTensor(2, def_dim_2, 0.0 );
stresstensor NDMaterial::errstresstensor;
straintensor NDMaterial::errstraintensor;

NDMaterial::NDMaterial(int tag, int classTag)
    : Material(tag, classTag)
{

}

NDMaterial::NDMaterial()
    : Material(0, 0)
{

}

NDMaterial::~NDMaterial()
{

}


double
NDMaterial::getRho(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterial::getE(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterial::getnu(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterial::getpsi(void)
{
    return 0.0;
}

const Vector &
NDMaterial::getCommittedStress(void)
{
    return this->getStress();
}

const Vector &
NDMaterial::getCommittedStrain(void)
{
    return this->getStrain();
}

// methods to set and retrieve state.
int
NDMaterial::setTrialStrain(const Vector &v)
{
    cerr << "NDMaterial::setTrialStrain -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrain(const Vector &v, const Vector &r)
{
    cerr << "NDMaterial::setTrialStrain -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrainIncr(const Vector &v)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrainIncr(const Vector &v, const Vector &r)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

const Matrix &
NDMaterial::getTangent(void)
{
    cerr << "NDMaterial::getTangent -- subclass responsibility\n";
    return errMatrix;
}

const Vector &
NDMaterial::getStress(void)
{
    cerr << "NDMaterial::getStress -- subclass responsibility\n";
    return errVector;
}

const Vector &
NDMaterial::getStrain(void)
{
    cerr << "NDMaterial::getStrain -- subclass responsibility\n";
    return errVector;
}

int
NDMaterial::setTrialStrain(const Tensor &v)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrain(const Tensor &v, const Tensor &r)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrainIncr(const Tensor &v)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialStrainIncr(const Tensor &v, const Tensor &r)
{
    cerr << "NDMaterial::setTrialStrainIncr -- subclass responsibility\n";
    return -1;
}

//Zhao (zcheng@ucdavis.edu)
// added Sept 22 2003 for Large Deformation, F is the Deformation Grandient
int
NDMaterial::setTrialF(const straintensor &f)
{
    cerr << "NDMaterial::setTrialF -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialFIncr(const straintensor &df)
{
    cerr << "NDMaterial::setTrialF -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialC(const straintensor &c)
{
    cerr << "NDMaterial::setTrialC -- subclass responsibility\n";
    return -1;
}

int
NDMaterial::setTrialCIncr(const straintensor &c)
{
    cerr << "NDMaterial::setTrialC -- subclass responsibility\n";
    return -1;
}

const stresstensor &NDMaterial::getPK1StressTensor(void)
{
    cerr << "NDMaterial::getPK1StressTensor -- subclass responsibility\n";
    return errstresstensor;
}

const stresstensor &NDMaterial::getCauchyStressTensor(void)
{
    cerr << "NDMaterial::getCauchyStressTensor -- subclass responsibility\n";
    return errstresstensor;
}

const straintensor &NDMaterial::getF(void)
{
    cerr << "NDMaterial::getF -- subclass responsibility\n";
    return errstraintensor;
}

const straintensor &NDMaterial::getC(void)
{
    cerr << "NDMaterial::getF -- subclass responsibility\n";
    return errstraintensor;
}

const straintensor &NDMaterial::getFp(void)
{
    cerr << "NDMaterial::getFp -- subclass responsibility\n";
    return errstraintensor;
}
// Only For Large Deformation, END////////////////////////////

const Tensor &
NDMaterial::getTangentTensor(void)
{
    cerr << "NDMaterial::getTangentTensor -- subclass responsibility\n";
    return errTensor;
}

const stresstensor &NDMaterial::getStressTensor(void)
{
    cerr << "NDMaterial::getStressTensor -- subclass responsibility\n";
    return errstresstensor;
}

const straintensor &NDMaterial::getStrainTensor(void)
{
    cerr << "NDMaterial::getStrainTensor -- subclass responsibility\n";
    return errstraintensor;
}

const straintensor &NDMaterial::getPlasticStrainTensor(void)
{
    cerr << "NDMaterial::getPlasticStrainTensor -- subclass responsibility\n";
    return errstraintensor;
}






// Nima Tafazzoli added for checking the mesh (September 2012)
int
NDMaterial::CheckMesh(ofstream &checkmesh_file)
{
    return 0;
}


