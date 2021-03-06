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
// PROGRAMMER:        Zhao Cheng
// Note:              Helpful discuss with Mahdi Taiebat and Professor Y.F. Dafalias
// DATE:              Fall 2005
// UPDATE HISTORY:    Guanzhou Jie updated for parallel, Dec. 2006
//
///////////////////////////////////////////////////////////////////////////////
//

// This is based on G = G0*Pat*[(2.97-e)^2/(1+e)]*(p/Pat)^0.5
// Richard et al 1970, Li & Dafalias 2000, Dafalias & Mazari 2004
// Constants:
// 1: G0:    reference shear mudulus factor (no unit);
// 2: v:     Poisson's ratio;
// 3: Pat:   Atmospheric pressure;
// 4: k_c;   cut-off factor, for p < k_c*Pat, let p = k_c*Pat to calculate G;
// 5: e0;    initial void ratio

#ifndef DM04_Elastic_CPP
#define DM04_Elastic_CPP

#include "DM04_Elastic.h"
#include <iostream>
using namespace std;


BJtensor DM04_Elastic::ElasticStiffness(4, def_dim_4, 0.0);

DM04_Elastic::DM04_Elastic(int G0_in,
                           int v_in,
                           int Pat_in,
                           int k_c_in,
                           int e0_in,
                           const stresstensor& initialStress,
                           const straintensor& initialStrain)
    : ElasticState(ES_TAG_DM04, initialStress, initialStrain),
      G0_index(G0_in),
      v_index(v_in),
      Pat_index(Pat_in),
      k_c_index(k_c_in),
      e0_index(e0_in)
{

}

// Create a new
ElasticState* DM04_Elastic::newObj()
{
    ElasticState* Els = new  DM04_Elastic(this->G0_index,
                                          this->v_index,
                                          this->Pat_index,
                                          this->k_c_index,
                                          this->e0_index,
                                          this->Stress,
                                          this->Strain);
    return Els;
}
////////////////////////////////////////////////////////////////
DM04_Elastic::~DM04_Elastic()
{

}
////////////////////////////////////////////////////////////////

// Get Stiffness Tensor
const BJtensor& DM04_Elastic::getElasticStiffness(const MaterialParameter& MaterialParameter_in) const
{
    // Kronecker delta tensor
    BJtensor I2("I", 2, def_dim_2);

    BJtensor I_ijkl = I2("ij") * I2("kl");
    I_ijkl.null_indices();
    BJtensor I_ikjl = I_ijkl.transpose0110();
    BJtensor I_iljk = I_ijkl.transpose0111();
    BJtensor I4s = (I_ikjl + I_iljk) * 0.5;

    double G0 = getG0(MaterialParameter_in);
    double v = getv(MaterialParameter_in);
    double Pat = getPat(MaterialParameter_in);
    double k_c = getk_c(MaterialParameter_in);
    double e0 = gete0(MaterialParameter_in);

    double epsilon_v = this->getStrain().Iinvariant1();
    double e = e0 + (1.0 + e0) * epsilon_v;
    double ef = (2.97 - e) * (2.97 - e) / (1.0 + e);
    double p_cal = this->getStress().p_hydrostatic();

    if (p_cal < 0.0)
    {
        p_cal = 0.0;
    }

    double p_cut = Pat * k_c;

    double p = (p_cal > p_cut) ? p_cal : p_cut;

    double G = G0 * Pat * ef * sqrt(p / Pat);
    double K = G * (2.0 * (1.0 + v) / (3.0 * (1.0 - 2.0 * v)));

    // Building elasticity tensor
    DM04_Elastic::ElasticStiffness = I_ijkl * (K - 2.0 * G / 3.0) + I4s * (2.0 * G);

    return DM04_Elastic::ElasticStiffness;
}

////////////////////////////////////////////////////////////////
const stresstensor& DM04_Elastic::getStress() const
{
    return ElasticState::Stress;
}

// Get G0
double DM04_Elastic::getG0(const MaterialParameter& MaterialParameter_in) const
{
    if ( G0_index > MaterialParameter_in.getNum_Material_Constant() || G0_index < 2)
    {
        cerr << "DM04_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(G0_index - 1);
    }
}

// Get v
double DM04_Elastic::getv(const MaterialParameter& MaterialParameter_in) const
{
    double v = 0.0;

    if ( v_index > MaterialParameter_in.getNum_Material_Constant() || v_index < 2)
    {
        cerr << "DM04_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        v = MaterialParameter_in.getMaterial_Constant(v_index - 1);

        if (v >= 0.5 || v <= -1.0)
        {
            cerr << "Warning!! DM04_Elastic: Invalid possoin's ratio. " << endln;
            exit (1);
        }

        return v;
    }
}

// Get Pat
double DM04_Elastic::getPat(const MaterialParameter& MaterialParameter_in) const
{
    if ( Pat_index > MaterialParameter_in.getNum_Material_Constant() || Pat_index < 1)
    {
        cerr << "DM04_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(Pat_index - 1);
    }
}

// Get k_cut
double DM04_Elastic::getk_c(const MaterialParameter& MaterialParameter_in) const
{
    if ( k_c_index > MaterialParameter_in.getNum_Material_Constant() || k_c_index < 1)
    {
        cerr << "DM04_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(k_c_index - 1);
    }
}

// Get e0
double DM04_Elastic::gete0(const MaterialParameter& MaterialParameter_in) const
{
    if ( e0_index > MaterialParameter_in.getNum_Material_Constant() || e0_index < 2)
    {
        cerr << "DM04_Elastic: Invalid Input. " << endln;
        exit (1);
    }
    else
    {
        return MaterialParameter_in.getMaterial_Constant(e0_index - 1);
    }
}

//Guanzhou added for parallel
int DM04_Elastic::sendSelf(int commitTag, Channel& theChannel)
{
    int dataTag = this->getDbTag();

    static ID idData(5);
    idData.Zero();

    idData(0) = G0_index;
    idData(1) = v_index;
    idData(2) = Pat_index;
    idData(3) = k_c_index;
    idData(4) = e0_index;

    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "DM04_Elastic::sendSelf -- failed to send ID\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, this->Stress) < 0)
    {
        cerr << "DM04_Elastic::sendSelf() -  failed to send nDarray Stress\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, this->Strain) < 0)
    {
        cerr << "DM04_Elastic::sendSelf() -  failed to send nDarray Strain\n";
        return -1;
    }

    return 0;
}

//Guanzhou added for parallel
int DM04_Elastic::receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();

    static ID idData(5);
    idData.Zero();

    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        cerr << "DM04_Elastic::receiveSelf -- failed to recv ID\n";
        return -1;
    }

    G0_index    = idData(0);
    v_index     = idData(1);
    Pat_index = idData(2);
    k_c_index = idData(3);
    e0_index = idData(4);

    if (theChannel.receivenDarray(dataTag, commitTag, this->Stress) < 0)
    {
        cerr << "DM04_Elastic::receiveSelf() -  failed to recv nDarray Stress\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, this->Strain) < 0)
    {
        cerr << "DM04_Elastic::receiveSelf() -  failed to recv nDarray Strain\n";
        return -1;
    }

    return 0;
}

#endif

