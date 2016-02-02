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

#ifndef DruckerPrager_YF_H
#define DruckerPrager_YF_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../YieldFunctionBase.h"
#include "cmath"
#include <iostream>

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"
using namespace ClassicElastoplasticityGlobals;




template<class AlphaHardeningType, class KHardeningType>
class DruckerPrager_YF : public YieldFunctionBase<DruckerPrager_YF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    DruckerPrager_YF( AlphaType &alpha_in, KType& k_in):
        YieldFunctionBase<DruckerPrager_YF<AlphaHardeningType, KHardeningType>>::YieldFunctionBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                alpha_(alpha_in), k_(k_in)
    {
        // std::cout << "k_in = " << &k_in << std::endl;
    }

    double operator()(const DTensor2& sigma) const
    {
        double p;
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        const double &k = k_.getVariableConstReference();
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p = -p;

        if (p >= 0)
        {
            cout << "p = " << p << endl;
            return 10;
        }

        return sqrt( (s(i, j) + p * alpha(i, j)) * (s(i, j) + p * alpha(i, j)) ) + SQRT_2_over_3 * k * p;  // This one assumes p positive in tension
    }

    const DTensor2& df_dsigma_ij(const DTensor2& sigma)
    {

        const DTensor2 &alpha = alpha_.getVariableConstReference();
        const double &k = k_.getVariableConstReference();

        // cout << "     --> YF alpha = " << alpha << endl;
        // cout << "     --> YF k     = " << k << endl;


        //Zero these tensors
        s *= 0;
        result *= 0;

        double p;

        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p = -p;

        // cout << "     --> YF p     = " << p << endl;
        // cout << "     --> YF s     = " << s << endl;

        double den = sqrt((s(i, j) - p * alpha(i, j)) * (s(i, j) - p * alpha(i, j)));
        // cout << "     --> YF den     = " << den << endl;

        if (den == 0)
        {
            return result; //Elastic
        }
        else
        {
            result(i, j) =
                (
                    (s(i, j) - p * alpha(i, j)) + alpha(m, n) * kronecker_delta(i, j) * (s(m, n) - p * alpha(m, n))
                )
                / den;
        }
        result(i, j) += SQRT_2_over_27 * k * kronecker_delta(i, j);

        return result;
    }

    double xi_star_h_star(const DTensor2& depsilon, const DTensor2& m, const DTensor2& sigma)
    {
        double dbl_result = 0.0;

        const DTensor2 &alpha = alpha_.getVariableConstReference();
        // const double &k = k_.getVariableConstReference();

        //Zero the stress deviator
        s *= 0;

        //Compute stress deviator (s) and mean pressure (p)
        double p;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension, so flip the sign
        p = -p;

        //
        double den = sqrt((s(i, j) - p * alpha(i, j)) * (s(i, j) - p * alpha(i, j)));

        // This is for the hardening of k
        double df_dk = -SQRT_2_over_3 * p;
        dbl_result +=  df_dk * k_.getDerivative(depsilon, m, sigma);

        //This is for the hardening of alpha
        dbl_result +=  (-p * (s(i, j) - p * alpha(i, j)) / den) * alpha_.getDerivative(depsilon, m, sigma)(i, j);

        return dbl_result;
    }

private:

    AlphaType &alpha_;
    KType &k_;
    static DTensor2 s; //Stress deviator
    static DTensor2 result; //For returning Dtensor2's

};

template <class AlphaHardeningType,  class KHardeningType>
DTensor2 DruckerPrager_YF<AlphaHardeningType, KHardeningType>::s(3, 3, 0.0);
template <class AlphaHardeningType,  class KHardeningType>
DTensor2 DruckerPrager_YF<AlphaHardeningType, KHardeningType>::result(3, 3, 0.0);


#endif