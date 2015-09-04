///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell, Nima Tafazzoli  Boris Jeremic
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


#ifndef BardetCopnstraintLT_H
#define BardetCopnstraintLT_H

//#include <iostream>

#include "../ltensor/LTensor.h"

#include <NDMaterialLT.h>
#include <G3Globals.h>

#include <Channel.h>



enum BardetConstraintType
{
    CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL         ,
    DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL         ,
    DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL            ,
    UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL          ,
    UNDRAINED_CYCLIC_TRIAXIAL_LOADING_STRESS_CONTROL   ,
    UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL
};


// DTensor2 BardetConstraintLT(int , double , NDMaterialLT *);

class BardetConstraintLT
{
public:
    BardetConstraintLT(BardetConstraintType type,  NDMaterialLT *material);
    ~BardetConstraintLT();
    void  applyIncrement(double increment);
    const DTensor2 &getStrainIncrement() const ;
    const DTensor2 &getStress() const ;
    void fillSE(double * S_values, double * E_values);

private:
    int type_of_test;
    NDMaterialLT *material;
    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'p' > p;
    DTensor2 S;//(6, 6);
    DTensor2 E;//(6, 6);
    DTensor1 Y;//(6, 0.0);
    DTensor2 CurrentStiffness;
    DTensor2 d_epsilon_tensor;//(3, 3, 0.0);
};


#endif
