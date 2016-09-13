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
//# UPDATE HISTORY:    Nov 2006, add initial back stress
//#
//===============================================================================

#ifndef fdYieldVM_CPP
#define fdYieldVM_CPP

#include "fdYieldVM.h"
//#include <iostream.h>

stresstensor fdYieldVM::fdYVM;

//--------------------------------------------------------------------
fdYieldVM::fdYieldVM(double Y0_in)
{
    Y0 = Y0_in;

    tensor z2(2, def_dim_2, 0.0);
    a0.Initialize(z2);
}

//--------------------------------------------------------------------
fdYieldVM::fdYieldVM(double Y0_in, const stresstensor& a0_in)
{
    Y0 = Y0_in;

    a0.Initialize(a0_in);
}

//--------------------------------------------------------------------
fdYield* fdYieldVM::newObj()
{
    fdYield* newfdyd = new fdYieldVM(this->Y0, this->a0);

    return newfdyd;
}

//--------------------------------------------------------------------
int fdYieldVM::getNumRank()
{
    return 2;
}

//--------------------------------------------------------------------
double fdYieldVM::getTolerance()
{
    double eps = 1.0e-7;
    double tol = (Y0 * eps) * (Y0 * eps);

    return tol > eps ?  tol : eps;
}

//--------------------------------------------------------------------------------------
// Yd =  1.5*(S_ij - a0_ij - a_ij)*(S_ij - a0_ij - a_ij) - (Y0+Y)*(Y0+Y) = 0,
// Note here NumRank = 2
//--------------------------------------------------------------------------------------

double fdYieldVM::Yd(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank=2, With Ki Hardening
    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    double Y = fdepstate.getStressLikeInVar() + Y0;

    stresstensor dev = sts.deviator() - a;

    double x = (dev("ij") * dev("ij")).trace();
    double yfun = 1.5 * x - Y * Y;

    return yfun;
}

//--------------------------------------------------------------------
const stresstensor& fdYieldVM::dYods(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank=2, With Ki Hardening
    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    stresstensor dev = sts.deviator() - a;

    fdYieldVM::fdYVM = dev * 3.0;

    return fdYieldVM::fdYVM;
}

//--------------------------------------------------------------------
double fdYieldVM::dYodq(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank=2, With Ki Hardening
    double Y = fdepstate.getStressLikeInVar() + Y0;

    return -2.0 * Y;
}

//--------------------------------------------------------------------
const stresstensor& fdYieldVM::dYoda(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    // NumRank=2, With Ki Hardening
    stresstensor a = fdepstate.getStressLikeKiVar() + a0;
    stresstensor dev = sts.deviator() - a;

    fdYieldVM::fdYVM = dev * (-3.0);

    return fdYieldVM::fdYVM;
}


#endif

