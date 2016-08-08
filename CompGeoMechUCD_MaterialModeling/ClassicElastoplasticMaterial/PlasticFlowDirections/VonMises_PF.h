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

#ifndef VonMises_PF_H
#define VonMises_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"

#include <cmath>



template<class AlphaHardeningType, class KHardeningType>
class VonMises_PF : public PlasticFlowBase<VonMises_PF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    // PlasticFlowBase<VonMises_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    VonMises_PF( AlphaType &alpha_in, KType &k_in):
        PlasticFlowBase<VonMises_PF<AlphaHardeningType , KHardeningType >>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                alpha_(alpha_in), k_(k_in)
    {

    }


    const DTensor2& operator()(const DTensor2 &depsilon, const DTensor2& sigma)
    {
        //Identical to derivative of VonMises_YF wrt sigma (a.k.a nij)
        const DTensor2 &alpha = alpha_.getVariableConstReference();

        //Zero these tensors
        s *= 0;
        result *= 0;

        double p = -sigma(i, i) / 3;

        s(i, j) = sigma(i, j) + p * kronecker_delta(i, j);
        result(i, j) = s(i, j) - alpha(i, j);
        double den = sqrt(result(i, j) * result(i, j));

        if (den == 0)
        {
            return result;
        }
        else
        {
            result(i, j) = result(i, j) / den;
        }

        // cout << "m = [";
        // for (int ii = 0; ii < 3; ii++)
        //     for (int jj = 0; jj < 3; jj++)
        //     {
        //         cout << result(ii, jj) << " ";
        //     }
        // cout << "]\n";
        // cout << "alpha = [";
        // for (int ii = 0; ii < 3; ii++)
        //     for (int jj = 0; jj < 3; jj++)
        //     {
        //         cout << alpha(ii, jj) << " ";
        //     }
        // cout << "]\n";

        return result;
    }

    DTensor4 const& dm_over_dsigma(DTensor2 const& sigma){
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        // const double &k = k_.getVariableConstReference();
        double p=0;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;
        static DTensor2 s_minus_alpha(3,3,0.0);
        s_minus_alpha(i,j) = s(i,j) - alpha(i,j);
        double intermediate = s_minus_alpha(i,j) * s_minus_alpha(i,j) ; 



        for (int ig = 0; ig < 3; ++ig)
            for (int mg = 0; mg < 3; ++mg)
                for (int jg = 0; jg < 3; ++jg)
                    for (int ng = 0; ng < 3; ++ng){
                        dm__dsigma(ig,jg,mg,ng) = 
                            (
                                kronecker_delta(ig,mg) * kronecker_delta(jg,ng) - 1.0/3.0 * kronecker_delta(ig,jg) * kronecker_delta(mg,ng) 
                            ) * pow(intermediate, -0.5) - 
                            (
                                (s(ig,jg)-alpha(ig,jg)) * (s(mg,ng)-alpha(mg,ng))
                            ) * pow(intermediate, -1.5); 
                    }

        // // ==============
        // //  Backup . LTensor do not accept this. Change to the naive for-loop.
        // // ==============
        // dm__dsigma(i,j,m,n) = 
        //     (
        //         kronecker_delta(i,m) * kronecker_delta(j,n) - 1.0/3.0 * kronecker_delta(i,j) * kronecker_delta(m,n) 
        //     ) * pow(intermediate, -0.5) - 
        //     (
        //         (s(i,j)-alpha(i,j)) * (s(m,n)-alpha(m,n))
        //     ) * pow(intermediate, -1.5); 
        // // ==============
        // =========================================
        // minimal failed example
        // =========================================
        // test(i,j,m,n)=kronecker_delta(i,m) * kronecker_delta(j,n) - 1.0/3.0 * kronecker_delta(i,j) * kronecker_delta(m,n) ;
        // =========================================

        return dm__dsigma;
    }
    // DTensor4 const& dm_over_dalpha(DTensor2 const& sigma){
        
    // }



private:

    AlphaType &alpha_;
    KType &k_;

    static DTensor2 s; //sigma deviator
    static DTensor2 result; //For returning Dtensor2s
    static DTensor4 dm__dsigma; //For returning dm_over_dsigma

};


template<class AlphaHardeningType, class KHardeningType>
DTensor2 VonMises_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 VonMises_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);
template<typename AlphaHardeningType, typename KHardeningType>
DTensor4 VonMises_PF<AlphaHardeningType , KHardeningType >::dm__dsigma(3, 3, 3, 3, 0.0);

#endif