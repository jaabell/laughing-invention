///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         .
// DATE:               .
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
**                                                                    **
** ****************************************************************** */

// $Revision: 1.16 $
// $Date: 2004/07/20 22:39:02 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/NDMaterial.h,v $


#ifndef NDMaterial_h
#define NDMaterial_h

// File: ~/material/NDMaterial.h
//
// Written: MHS
// Created: Feb 2000
// Revision: A
//
// Description: This file contains the class definition for NDMaterial.
// NDMaterial is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes.
//
// What: "@(#) NDMaterial.h, revA"

#include <Material.h>
#include <Tensor.h>

// Boris Jeremic 17Nov2008
#include <iostream>
using namespace std;


class Matrix;
class ID;
class Vector;
class Information;
// class Response;

class NDMaterial : public Material
{
public:
    NDMaterial(int tag, int classTag);
    NDMaterial();
    virtual ~NDMaterial();

    // methods to set state and retrieve state using Matrix and Vector classes
    virtual double getRho(void);
    // BJ added 19June2002
    virtual double getE(void);
    virtual double getnu(void);
    virtual double getpsi(void);


    virtual int setTrialStrain(const Vector &v);
    virtual int setTrialStrain(const Vector &v, const Vector &r);
    virtual int setTrialStrainIncr(const Vector &v);
    virtual int setTrialStrainIncr(const Vector &v, const Vector &r);
    virtual const Matrix &getTangent(void);
    virtual const Matrix &getInitialTangent(void)
    {
        return this->getTangent();
    };

    virtual const Vector &getStress(void);
    virtual const Vector &getStrain(void);

    // new methods for recorder requested by Zhouhui Yang .. MAY NOT STAY
    virtual const Vector &getCommittedStress(void);
    virtual const Vector &getCommittedStrain(void);

    // methods to set and retrieve state using the Tensor class
    virtual int setTrialStrain(const Tensor &v);
    virtual int setTrialStrain(const Tensor &v, const Tensor &r);
    virtual int setTrialStrainIncr(const Tensor &v);
    virtual int setTrialStrainIncr(const Tensor &v, const Tensor &r);
    virtual const Tensor &getTangentTensor(void);
    virtual const stresstensor &getStressTensor(void);
    virtual const straintensor &getStrainTensor(void);
    //Added Joey Aug. 13, 2001
    virtual const straintensor &getPlasticStrainTensor(void);

    //Zhao (zcheng@ucdavis.edu)
    // added Sept 22 2003 for Large Deformation, F is the Deformation Gradient
    virtual int setTrialF(const straintensor &f);
    virtual int setTrialFIncr(const straintensor &df);
    virtual int setTrialC(const straintensor &c);
    virtual int setTrialCIncr(const straintensor &dc);
    virtual const stresstensor &getPK1StressTensor(void);
    virtual const stresstensor &getCauchyStressTensor(void);
    virtual const straintensor &getF(void);
    virtual const straintensor &getC(void);
    virtual const straintensor &getFp(void);
    // Only For Large Deformation, END////////////////////////////////////////

    virtual int commitState(void) = 0;
    virtual int revertToLastCommit(void) = 0;
    virtual int revertToStart(void) = 0;

    virtual NDMaterial *getCopy(void) = 0;
    virtual NDMaterial *getCopy(const char *code) = 0;

    virtual const char *getType(void) const = 0;
    virtual int getOrder(void) const
    {
        return 1;
    };

    // virtual Response* setResponse (const char** argv, int argc, Information& matInformation);
    // virtual int getResponse (int responseID, Information& matInformation);



    virtual int getObjectSize()
    {
        return 1;
    }; //Guanzhou added, subclasses to override!


    // Nima Tafazzoli added for checking the mesh (September 2012)
    virtual int CheckMesh(ofstream &);



protected:

private:
    static Matrix errMatrix;
    static Vector errVector;
    static Tensor errTensor;
    static stresstensor errstresstensor;
    static straintensor errstraintensor;
};


#endif
