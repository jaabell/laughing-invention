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

#ifndef CamClay_YF_H
#define CamClay_YF_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../YieldFunctionBase.h"
#include "cmath"
#include <iostream>

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"
using namespace ClassicElastoplasticityGlobals;




template<class p0HardeningType>
class CamClay_YF : public YieldFunctionBase<CamClay_YF<p0HardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<double, p0HardeningType> p0Type;


    CamClay_YF( double M_in, p0Type& p0_in):
        YieldFunctionBase<CamClay_YF<p0HardeningType>>::YieldFunctionBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                M(M_in), p0_(p0_in)
    {    }

    double operator()(const DTensor2& sigma) const
    {
        double p, q, theta;
        std::tie(p, q, theta) = getpqtheta(sigma);
        const double &p0 = p0_.getVariableConstReference();

        double yf = q * q - M * M * p * (p0 - p);

        return yf;
    }

    const DTensor2& df_dsigma_ij(const DTensor2& sigma)
    {
        //Zero these tensors
        s *= 0;
        result *= 0;

        double p;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension, so flip the sign
        p = -p;
        const double &p0 = p0_.getVariableConstReference();


        double scalar1 = M * M * (p0 - 2 * p) / 3;

        result(i, j) = s(i, j) * 3 + kronecker_delta(i, j) * scalar1;
        return result;
    }

    double xi_star_h_star(const DTensor2& depsilon, const DTensor2& m, const DTensor2& sigma)
    {
        double dbl_result = 0.0;

        // const double &p0 = p0_.getVariableConstReference();

        //Compute stress deviator (s) and mean pressure (p)
        double p = -sigma(i, i) / 3;

        // This is for the hardening of k
        double df_dp0 = M * M * p;
        dbl_result +=  df_dp0 * p0_.getDerivative(depsilon, m, sigma);

        return dbl_result;
    }

    bool hasCorner() const
    {
        return false;
    }

    double get_k() const
    {
        return 0;
    }

    DTensor2 & get_alpha() const
    {
        return s;
    }




private:

    double M;
    p0Type &p0_;
    static DTensor2 s; //Stress deviator
    static DTensor2 result; //For returning Dtensor2's
};

template <class p0HardeningType>
DTensor2 CamClay_YF<p0HardeningType>::s(3, 3, 0.0);
template <class p0HardeningType>
DTensor2 CamClay_YF<p0HardeningType>::result(3, 3, 0.0);


#endif