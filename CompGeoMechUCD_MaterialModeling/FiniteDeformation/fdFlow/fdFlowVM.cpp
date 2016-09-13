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

#ifndef fdFlowVM_CPP
#define fdFlowVM_CPP

#include "fdFlowVM.h"

stresstensor fdFlowVM::fdFVM2;
tensor fdFlowVM::fdFVM4(4, def_dim_4, 0.0);

//--------------------------------------------------------------------
fdFlowVM::fdFlowVM(double Y0_in)
{
    Y0 = Y0_in;

    tensor z2(2, def_dim_2, 0.0);
    a0.Initialize(z2);
}

//--------------------------------------------------------------------
fdFlowVM::fdFlowVM(double Y0_in, const stresstensor& a0_in)
{
    Y0 = Y0_in;

    a0.Initialize(a0_in);
}

//--------------------------------------------------------------------
fdFlow* fdFlowVM::newObj()
{
    fdFlow* newfdyd = new fdFlowVM(this->Y0, this->a0);

    return newfdyd;
}

//-------------------------------------------------------------------
// Q = 1.5*(S_ij-a'_ij)*(S_ij-a'_ij)  - Y'*Y' = 0, Note here NumRank = 2
//-------------------------------------------------------------------

//--------------------------------------------------------------------
const stresstensor& fdFlowVM::dFods(const stresstensor& sts, const FDEPState& fdepstate) const
{
    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    stresstensor dev = sts.deviator() - a;

    fdFlowVM::fdFVM2 = dev * 3.0;

    return fdFlowVM::fdFVM2;
}

//--------------------------------------------------------------------
double fdFlowVM::dFodq(const stresstensor& sts, const FDEPState& fdepstate) const
{
    double Y = fdepstate.getStressLikeInVar() + Y0;

    return -2.0 * Y;
}

//--------------------------------------------------------------------
const stresstensor& fdFlowVM::dFoda(const stresstensor& sts, const FDEPState& fdepstate) const
{
    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    stresstensor dev = sts.deviator() - a;
    fdFlowVM::fdFVM2 = dev * (-3.0);

    return fdFlowVM::fdFVM2;
}

//--------------------------------------------------------------------
const tensor& fdFlowVM::d2Fodsds(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2 , def_dim_2);
    tensor I4 = I2("ij") * I2("kl");
    I4.null_indices();
    tensor I4ns = I4.transpose0110();
    fdFlowVM::fdFVM4 = I4ns * 3.0 - I4;

    return fdFlowVM::fdFVM4;
}

//--------------------------------------------------------------------
const tensor& fdFlowVM::d2Fodsda(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2 , def_dim_2);
    tensor I4 = I2("ij") * I2("kl");
    I4.null_indices();
    tensor I4ns = I4.transpose0110();
    fdFlowVM::fdFVM4 = I4ns * (-1.0);

    return fdFlowVM::fdFVM4;
}

//--------------------------------------------------------------------
double fdFlowVM::d2Fodqdq(const stresstensor& sts, const FDEPState& fdepstate) const
{
    return -2.0;
}

//--------------------------------------------------------------------
const tensor& fdFlowVM::d2Fodada(const stresstensor& sts, const FDEPState& fdepstate) const
{
    tensor I2("I", 2 , def_dim_2);
    tensor I4 = I2("ij") * I2("kl");
    I4.null_indices();
    fdFlowVM::fdFVM4 = I4 * 3.0;

    return fdFlowVM::fdFVM4;
}

#endif

