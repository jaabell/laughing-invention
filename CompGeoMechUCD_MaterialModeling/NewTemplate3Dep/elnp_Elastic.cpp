///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// DESIGNER:          Zhao Cheng, Boris Jeremic
// PROGRAMMER:        Zhao Cheng,
// DATE:              Fall 2005
// UPDATE HISTORY:    Guanzhou Jie updated for parallel Dec 2006
//
///////////////////////////////////////////////////////////////////////////////
//

// This is based on "e = e_ref - log(p/p_ref)"
// So K = (1+e)*p/kappa (Wood, 1990, Soil Behavior and Critical State Soil Mechanics)
// Constants:
// 1: kappa: unliading slope in e-log(p) relation curve
// 2: v:     poissoin's ratio
// 3: Kc:    cut-off bulk modulus, when K<Kc, let K = Kc
// 4: e0:    initial void ratio

#ifndef elnp_Elastic_CPP
#define elnp_Elastic_CPP

#include "elnp_Elastic.h"

#include <stdlib.h>

using namespace std;

BJtensor elnp_Elastic::ElasticStiffness(4, def_dim_4, 0.0);

elnp_Elastic::elnp_Elastic(int kappa_in,
                           int v_in,
                           int K_c_in,
                           int e0_in,
                           const stresstensor& initialStress,
                           const straintensor& initialStrain)
    : ElasticState(ES_TAG_elnpElastic, initialStress, initialStrain),
      kappa_index(kappa_in),
      v_index(v_in),
      K_c_index(K_c_in),
      e0_index(e0_in)
{

}


// Nima Tafazzoli added for new Material Models
elnp_Elastic::elnp_Elastic(const stresstensor& initialStress,
                           const straintensor& initialStrain)
    : ElasticState(ES_TAG_elnpElastic, initialStress, initialStrain)
{

}

// Create a new
ElasticState* elnp_Elastic::newObj()
{
    ElasticState* Els = new  elnp_Elastic(this->kappa_index,
                                          this->v_index,
                                          this->K_c_index,
                                          this->e0_index,
                                          this->Stress,
                                          this->Strain);
    return Els;
}
////////////////////////////////////////////////////////////////
elnp_Elastic::~elnp_Elastic()
{

}
////////////////////////////////////////////////////////////////
// Get Stiffness Tensor
const BJtensor& elnp_Elastic::getElasticStiffness(const MaterialParameter& MaterialParameter_in) const
{
    // Kronecker delta tensor
    BJtensor I2("I", 2, def_dim_2);

    BJtensor I_ijkl = I2("ij") * I2("kl");
    I_ijkl.null_indices();
    BJtensor I_ikjl = I_ijkl.transpose0110();
    BJtensor I_iljk = I_ijkl.transpose0111();
    BJtensor I4s = (I_ikjl + I_iljk) * 0.5;

    double kappa = getkappa(MaterialParameter_in);
    double v = getv(MaterialParameter_in);

    if (v >= 0.5 || v < -1.0)
    {
        cerr << "Warning!! elnp_Elastic: Invalid Possoin's ratio. " << endln;
        exit (1);
    }

    double Kc = getK_c(MaterialParameter_in);
    double e0 = gete0(MaterialParameter_in);

    double p = this->getStress().p_hydrostatic();
    double epsilon_v = this->getStrain().Iinvariant1();
    double e = e0 + (1 + e0) * epsilon_v;
    double Kk = (1.0 + e) * p / kappa;

    double K = (Kk > Kc) ? Kk : Kc ;
    double G = K * 1.5 * (1.0 - 2.0 * v) / (1.0 + v);

    // To avoid numerical problems
    //if (G < (1.0e-3) *K)  G = 1.0e-3 *K;

    // Building elasticity tensor
    elnp_Elastic::ElasticStiffness = I_ijkl * (K - 2.0 * G / 3.0) + I4s * (2.0 * G);

    return elnp_Elastic::ElasticStiffness;
}

// Get kappa
double elnp_Elastic::getkappa(const MaterialParameter& MaterialParameter_in) const
{
    if ( kappa_index > MaterialParameter_in.getNum_Material_Constant() || kappa_index < 2)
    {
        cerr << "elnp_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(kappa_index - 1);
    }
}

// Get v
double elnp_Elastic::getv(const MaterialParameter& MaterialParameter_in) const
{
    if ( v_index > MaterialParameter_in.getNum_Material_Constant() || v_index < 2)
    {
        cerr << "elnp_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(v_index - 1);
    }
}

// Get K_c
double elnp_Elastic::getK_c(const MaterialParameter& MaterialParameter_in) const
{
    if ( K_c_index > MaterialParameter_in.getNum_Material_Constant() || K_c_index < 2)
    {
        cerr << "elnp_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(K_c_index - 1);
    }
}

// Get e0
double elnp_Elastic::gete0(const MaterialParameter& MaterialParameter_in) const
{
    if ( e0_index > MaterialParameter_in.getNum_Material_Constant() || e0_index < 2)
    {
        cerr << "elnp_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(e0_index - 1);
    }
}

//Guanzhou added for parallel
int elnp_Elastic::sendSelf(int commitTag, Channel& theChannel)
{
    int dataTag = this->getDbTag();

    static ID idData(6);
    idData.Zero();

    int stress_tag = theChannel.getDbTag();
    int strain_tag = theChannel.getDbTag();

    idData(0) = kappa_index;
    idData(1) = v_index;
    idData(2) = K_c_index;
    idData(3) = e0_index;
    idData(4) = stress_tag;
    idData(5) = strain_tag;

    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "elnp_Elastic::sendSelf -- failed to send ID\n";
        return -1;
    }

    if (theChannel.sendnDarray(stress_tag, commitTag, this->Stress) < 0)
    {
        cerr << "elnp_Elastic::sendSelf() -  failed to send nDarray Stress\n";
        return -1;
    }

    if (theChannel.sendnDarray(strain_tag, commitTag, this->Strain) < 0)
    {
        cerr << "elnp_Elastic::sendSelf() -  failed to send nDarray Strain\n";
        return -1;
    }


    return 0;
}

//Guanzhou added for parallel
int elnp_Elastic::receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();

    static ID idData(6);
    idData.Zero();

    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        cerr << "elnp_Elastic::receiveSelf -- failed to recv ID\n";
        return -1;
    }

    kappa_index   = idData(0);
    v_index      = idData(1);
    K_c_index  = idData(2);
    e0_index   = idData(3);
    int stress_tag   = idData(4);
    int strain_tag   = idData(5);

    if (theChannel.receivenDarray(stress_tag, commitTag, this->Stress) < 0)
    {
        cerr << "elnp_Elastic::receiveSelf() -  failed to recv nDarray Stress\n";
        return -1;
    }

    if (theChannel.receivenDarray(strain_tag, commitTag, this->Strain) < 0)
    {
        cerr << "elnp_Elastic::receiveSelf() -  failed to recv nDarray Strain\n";
        return -1;
    }


    return 0;
}

#endif
