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

#ifndef DruckerPrager_PF_H
#define DruckerPrager_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"





template<class AlphaHardeningType, class KHardeningType>
class DruckerPrager_PF : public PlasticFlowBase<DruckerPrager_PF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    // PlasticFlowBase<DruckerPrager_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    DruckerPrager_PF( AlphaType &alpha_in, KType &k_in):
        PlasticFlowBase<DruckerPrager_PF<AlphaHardeningType , KHardeningType >>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                alpha_(alpha_in), k_(k_in)
    {
        // std::cout << "k_in = " << &k_in << std::endl;
    }


    const DTensor2& operator()(const DTensor2 &depsilon, const DTensor2& sigma)
    {
        using namespace ClassicElastoplasticityGlobals;
        //Identical to derivative of DruckerPrager_YF wrt sigma (a.k.a nij)
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        const double &k = k_.getVariableConstReference();

        // cout << "     --> PF alpha = " << alpha << endl;
        // cout << "     --> PF k     = " << k << endl;


        //Zero these tensors
        s *= 0;
        result *= 0;

        double p;

        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p = -p;

        // cout << "     --> PF p     = " << p << endl;
        // cout << "     --> PF s     = " << s << endl;


        double den = sqrt((s(i, j) - p * alpha(i, j)) * (s(i, j) - p * alpha(i, j)));
        // cout << "     --> PF den     = " << den << endl;

        result(i, j) =
            (
                (s(i, j) - p * alpha(i, j)) + alpha(m, n) * kronecker_delta(i, j) * (s(m, n) - p * alpha(m, n))
            )
            / den;
        result(i, j) += SQRT_2_over_27 * k * kronecker_delta(i, j);

        // DTensor2 bshit(3, 3, 0.0);
        // bshit(i, j) = SQRT_2_over_27 * k * kronecker_delta(i, j);

        // cout << "     --> PF result     = " << result << endl;
        // cout << "     --> PF bshit     = " << bshit << endl;
        return result;
    }

private:

    AlphaType &alpha_;
    KType &k_;

    static DTensor2 s; //sigma deviator
    static DTensor2 result; //For returning Dtensor2s

};


template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);

#endif