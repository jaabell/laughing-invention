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
//# UPDATE HISTORY:    input parameter are m0 and a0
//#
//===============================================================================

#ifndef fdYieldDP_CPP
#define fdYieldDP_CPP

#include "fdYieldDP.h"

stresstensor fdYieldDP::fdYDP;

//--------------------------------------------------------------------
fdYieldDP::fdYieldDP(double m0_in)
{
    m0 = m0_in;

    tensor z2(2, def_dim_2, 0.0);
    a0.Initialize(z2);
}

//--------------------------------------------------------------------
fdYieldDP::fdYieldDP(double m0_in, const stresstensor& a0_in)
{
    m0 = m0_in;

    a0.Initialize(a0_in);
}

//--------------------------------------------------------------------
fdYield* fdYieldDP::newObj()
{
    fdYield* newfdyd = new fdYieldDP(this->m0, this->a0);

    return newfdyd;
}


//--------------------------------------------------------------------
int fdYieldDP::getNumRank()
{
    return 2;
}

//--------------------------------------------------------------------
double fdYieldDP::getTolerance()
{
    double eps = 1.0e-7;

    return eps;
}

//--------------------------------------------------------------------------------------------
// Yd = [(sij-p*a'ij)(sij-p*a'ij)]^0.5 - sqrt(2/3)*(p*m') = 0
// Note here NumRank = 2: with Kinematic hardening
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------
double fdYieldDP::Yd(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank = 2, with Ki Hardeing

    stresstensor dev = sts.deviator();
    double p = sts.p_hydrostatic();

    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    double m = fdepstate.getStressLikeInVar() + m0;

    stresstensor s_bar = dev - a;

    double x = sqrt((s_bar("ij") * s_bar("ij")).trace());

    double yd = x - sqrt(2.0 / 3.0) * p * m;

    return yd;
}

//--------------------------------------------------------------------
const stresstensor& fdYieldDP::dYods(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank = 2, with Ki Hardening

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

    fdYieldDP::fdYDP = nij + I2 * (m * sqrt(2.0 / 27.0) + a_n / 3.0);

    return fdYieldDP::fdYDP;
}

//--------------------------------------------------------------------
double fdYieldDP::dYodq(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank = 2, with Ki Hardening
    double p = sts.p_hydrostatic();

    double scalar1 = -p * sqrt(2.0 / 3.0);

    return scalar1;
}

//--------------------------------------------------------------------
const stresstensor& fdYieldDP::dYoda(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank = 2, with Ki Hardening

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

    fdYieldDP::fdYDP = nij * (-p);

    return fdYieldDP::fdYDP;
}


#endif

