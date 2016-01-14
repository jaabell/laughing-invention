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
// PROGRAMMER:
// DATE:
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef YieldFunctionBase_H
#define YieldFunctionBase_H

#include "../../ltensor/LTensor.h"
#include "EvolvingVariable.h"
#include <Channel.h>


template <class T>
class YieldFunctionBase
{
public:
    YieldFunctionBase()
    {
    }

    double operator()( const DTensor2& sigma) const
    {
        return static_cast<T*>(this)->operator()(sigma);
    }

    const DTensor2& df_dsigma_ij(const DTensor2& sigma)
    {
        return static_cast<T*>(this)->df_dsigma_ij(sigma);
    }

    double xi_star_h_star(const DTensor2& depsilon, const DTensor2& depsilon_pl, const DTensor2& sigma)
    {
        return static_cast<T*>(this)->df_dxi_star_h_star(depsilon, depsilon_pl , sigma);
    }

private:

};




#endif