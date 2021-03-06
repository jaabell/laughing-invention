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

#ifndef fdFlow_CPP
#define fdFlow_CPP

#include "fdFlow.h"

double fdFlow::dFodq(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    return 0.0;
}

const stresstensor& fdFlow::dFoda(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static stresstensor Z2;

    return Z2;
}

const tensor& fdFlow::d2Fodsds(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static tensor Z4(4, def_dim_4, 0.0);

    return Z4;
}

const stresstensor& fdFlow::d2Fodsdq(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static stresstensor Z2;

    return Z2;
}

const tensor& fdFlow::d2Fodsda(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static tensor Z4(4, def_dim_4, 0.0);

    return Z4;
}

double fdFlow::d2Fodqdq(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    return 0.0;
}

const stresstensor& fdFlow::d2Fodqda(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static stresstensor Z2;

    return Z2;
}

const tensor& fdFlow::d2Fodada(const stresstensor& sts, const FDEPState& fdepstate ) const
{
    static tensor Z4(4, def_dim_4, 0.0);

    return Z4;
}


#endif

