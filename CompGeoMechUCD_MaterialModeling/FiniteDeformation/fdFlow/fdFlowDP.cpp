//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Finite Deformation Hyper-Elastic classes
//# CLASS:
//#
//# VERSION:           0.6_(1803398874989) (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhao Cheng, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//#
//#
//# DATE:              July 2004
//# UPDATE HISTORY:
//#
//===============================================================================

#ifndef fdFlowDP_CPP
#define fdFlowDP_CPP

#include "fdFlowDP.h"

stresstensor fdFlowDP::fdFDP2;
tensor fdFlowDP::fdFDP4(4, def_dim_4, 0.0);

//--------------------------------------------------------------------
fdFlowDP::fdFlowDP(double m0_in)
{
    m0 = m0_in;

    tensor z2(2, def_dim_2, 0.0);
    a0.Initialize(z2);
}

//--------------------------------------------------------------------
fdFlowDP::fdFlowDP(double m0_in, const stresstensor& a0_in)
{
    m0 = m0_in;

    a0.Initialize(a0_in);
}

//--------------------------------------------------------------------
fdFlow* fdFlowDP::newObj()
{
    fdFlow* newfdyd = new fdFlowDP(this->m0, this->a0);

    return newfdyd;
}

// Yd = [(sij-p*a'ij)(sij-p*a'ij)]^0.5 - sqrt(2/3)*(p*m') = 0
// Note here NumRank = 2: with Kinematic hardening

//--------------------------------------------------------------------
const stresstensor& fdFlowDP::dFods(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);

    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    double m = fdepstate.getStressLikeInVar() + m0;

    double p = sts.p_hydrostatic();
    stresstensor dev = sts.deviator();

    stresstensor s_bar = dev - a * p;
    double x = sqrt((s_bar("ij") * s_bar("ij")).trace());

    stresstensor nij;

    if (x != 0.0)
    {
        nij =  s_bar * (1.0 / x);
    }

    double a_n = (a("ij") * nij("ij")).trace();

    fdFlowDP::fdFDP2 = nij + I2 * (m * sqrt(2.0 / 27.0) + a_n / 3.0);

    return fdFlowDP::fdFDP2;
}

//--------------------------------------------------------------------
double fdFlowDP::dFodq(const stresstensor& sts, const FDEPState& fdepstate) const
{
    double p = sts.p_hydrostatic();

    double scalar1 = -p * sqrt(2.0 / 3.0);

    return scalar1;
}

//--------------------------------------------------------------------
const stresstensor& fdFlowDP::dFoda(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);

    stresstensor a = fdepstate.getStressLikeKiVar() + a0;

    double p = sts.p_hydrostatic();
    stresstensor dev = sts.deviator();

    stresstensor s_bar = dev - a * p;
    double x = sqrt((s_bar("ij") * s_bar("ij")).trace());

    stresstensor nij;

    if (x != 0.0)
    {
        nij =  s_bar * (1.0 / x);
    }

    fdFlowDP::fdFDP2 = nij * (-p);

    return fdFlowDP::fdFDP2;
}

//--------------------------------------------------------------------
const tensor& fdFlowDP::d2Fodsds(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);
    tensor I_ijkl = I2("ij") * I2("kl");
    I_ijkl.null_indices();
    tensor I_ikjl = I_ijkl.transpose0110();

    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    double m = fdepstate.getStressLikeInVar() + m0;

    double p = sts.p_hydrostatic();
    stresstensor sigma_b = sts - (a * p);

    tensor a_I = a("ij") * I2("mn");
    a_I.null_indices();
    tensor I_a = I2("ij") * a("mn");
    I_a.null_indices();
    double a_a = (a("pq") * a("pq")).trace();

    fdFlowDP::fdFDP4 =  I_ikjl * 3.0 + a_I + I_a + I_ijkl * (-1.0 + a_a / 3.0 - 2.0 * m * m / 9.0);

    return fdFlowDP::fdFDP4;
}

//--------------------------------------------------------------------
const stresstensor& fdFlowDP::d2Fodsdq(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);

    double m = fdepstate.getStressLikeInVar() + m0;
    double p = sts.p_hydrostatic();

    fdFlowDP::fdFDP2 = I2 * (m * p * 4.0 / 3.0);

    return fdFlowDP::fdFDP2;
}

//--------------------------------------------------------------------
const tensor& fdFlowDP::d2Fodsda(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);
    tensor I_ijkl = I2("ij") * I2("kl");
    I_ijkl.null_indices();
    tensor I_ikjl = I_ijkl.transpose0110();

    double p = sts.p_hydrostatic();
    stresstensor s = sts.deviator();
    tensor tensor1 = I2("ij") * s("mn");
    tensor1.null_indices();

    fdFlowDP::fdFDP4 = I_ikjl * (-3.0 * p) + tensor1;

    return fdFlowDP::fdFDP4;
}

//--------------------------------------------------------------------
double fdFlowDP::d2Fodqdq(const stresstensor& sts, const FDEPState& fdepstate) const
{
    return 0.0;
}

//--------------------------------------------------------------------
const tensor& fdFlowDP::d2Fodada(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2, def_dim_2);
    tensor I_ijkl = I2("ij") * I2("kl");
    I_ijkl.null_indices();
    tensor I_ikjl = I_ijkl.transpose0110();

    stresstensor a = fdepstate.getStressLikeKiVar() + a0;

    double p = sts.p_hydrostatic();
    stresstensor dev = sts.deviator();

    stresstensor s_bar = dev - a * p;
    double x = sqrt((s_bar("ij") * s_bar("ij")).trace());

    stresstensor nij;

    if (x != 0.0)
    {
        nij =  s_bar * (1.0 / x);
        tensor n_n = nij("ij") * nij("mn");
        n_n.null_indices();

        fdFlowDP::fdFDP4 = I_ikjl * (p / x / x) + n_n * (p / x);
    }
    else
    {
        fdFlowDP::fdFDP4 = fdFlowDP::fdFDP4 * 0.0;
    }

    return fdFlowDP::fdFDP4;
}


#endif
