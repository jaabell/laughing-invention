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

#include <cmath>



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
                (s(i, j) - p * alpha(i, j)) + alpha(m, n) * kronecker_delta(i, j) * (s(m, n) - p * alpha(m, n))/3.0
            )
            / den;
        result(i, j) += SQRT_2_over_27 * k * kronecker_delta(i, j);

        // DTensor2 bshit(3, 3, 0.0);
        // bshit(i, j) = SQRT_2_over_27 * k * kronecker_delta(i, j);

        // cout << "     --> PF result     = " << result << endl;
        // cout << "     --> PF bshit     = " << bshit << endl;
        return result;
    }

    DTensor4 const& dm_over_dsigma(DTensor2 const& sigma){
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        // const double &k = k_.getVariableConstReference();
        double p=0.0;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;
        static DTensor2 s_minus_palpha(3,3,0.0);
        s_minus_palpha(i,j) = s(i,j) - p*alpha(i,j);
        double intermediate = s_minus_palpha(i,j) * s_minus_palpha(i,j) ; 


        // // ======================================================================
        // //  Backup . LTensor does not accept this. So change to the naive for-loop.
        // // ======================================================================
        // dm__dsigma(i,j,m,n) = 
        //     ( 
        //         (kronecker_delta(m,i)*kronecker_delta(n,j) - 1./3.0 * kronecker_delta(m,n) * kronecker_delta(i,j) 
        //             + 1./3.0 * kronecker_delta(m,n)*alpha(i,j) ) + 1./3.0 * alpha(p,q)*kronecker_delta(i,j) * 
        //         (kronecker_delta(m,p)*kronecker_delta(n,q) - 1.0/3.0*kronecker_delta(m,n)*kronecker_delta(p,q) 
        //             + 1./3.0 * kronecker_delta(m,n)*alpha(p,q) )
        //     ) * pow(intermediate, -0.5)  -
        //     (
        //          (s(i,j)-p*alpha(i,j) + 1./3.0 *alpha(p,q) * kronecker_delta(i,j) * (s(p,q) - p*alpha(p,q))) *
        //          (kronecker_delta(m,r)*kronecker_delta(n,s) - 1./3.0*kronecker_delta(m,n)*kronecker_delta(r,s) 
        //             +1./3.0 * kronecker_delta(m,n) * alpha(r,s)) *
        //          (s(r,s)-p*alpha(r,s)) 
        //     ) * pow(intermediate, -1.5); 
        // // ======================================================================
        // =========================================
        // The minimal failed example
        // Possible reasons: i,j,m,n are free indices but ?...
        // =========================================
        // test(i,j,m,n)=kronecker_delta(i,m) * kronecker_delta(j,n) - 1.0/3.0 * kronecker_delta(i,j) * kronecker_delta(m,n) ;
        // =========================================

        for (int ig = 0; ig < 3; ++ig)
            for (int mg = 0; mg < 3; ++mg)
                for (int jg = 0; jg < 3; ++jg)
                    for (int ng = 0; ng < 3; ++ng)
                        for (int pg = 0; pg < 3; ++pg)
                            for (int qg = 0; qg < 3; ++qg)
                                for (int rg = 0; rg < 3; ++rg)
                                    for (int sg = 0; sg < 3; ++sg){
                                        dm__dsigma(ig,jg,mg,ng) += 
                                            ( 
                                                (kronecker_delta(mg,ig)*kronecker_delta(ng,jg) - 1./3.0 * kronecker_delta(mg,ng) * kronecker_delta(ig,jg) 
                                                    + 1./3.0 * kronecker_delta(mg,ng)*alpha(ig,jg) ) + 1./3.0 * alpha(pg,qg)*kronecker_delta(ig,jg) * 
                                                (kronecker_delta(mg,pg)*kronecker_delta(ng,qg) - 1.0/3.0*kronecker_delta(mg,ng)*kronecker_delta(pg,qg) 
                                                    + 1./3.0 * kronecker_delta(mg,ng)*alpha(pg,qg) )
                                            ) * pow(intermediate, -0.5)  -
                                            (
                                                 (s(ig,jg)-p*alpha(ig,jg) + 1./3.0 *alpha(pg,qg) * kronecker_delta(ig,jg) * (s(pg,qg) - p*alpha(pg,qg))) *
                                                 (kronecker_delta(mg,rg)*kronecker_delta(ng,sg) - 1./3.0*kronecker_delta(mg,ng)*kronecker_delta(rg,sg) 
                                                    +1./3.0 * kronecker_delta(mg,ng) * alpha(rg,sg)) *
                                                 (s(rg,sg)-p*alpha(rg,sg)) 
                                            ) * pow(intermediate, -1.5); 
                                    }


        return dm__dsigma;
    }
    DTensor4 const& dm_over_dalpha(DTensor2 const& sigma){
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        // const double &k = k_.getVariableConstReference();
        double p=0.0;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;
        static DTensor2 s_minus_palpha(3,3,0.0);
        s_minus_palpha(i,j) = s(i,j) - p*alpha(i,j);
        double intermediate = s_minus_palpha(i,j) * s_minus_palpha(i,j) ; 
        for (int ig = 0; ig < 3; ++ig)
            for (int mg = 0; mg < 3; ++mg)
                for (int jg = 0; jg < 3; ++jg)
                    for (int ng = 0; ng < 3; ++ng)
                        for (int pg = 0; pg < 3; ++pg)
                            for (int qg = 0; qg < 3; ++qg)
                                for (int rg = 0; rg < 3; ++rg)
                                    for (int sg = 0; sg < 3; ++sg){
                                        dm__dalpha(ig,jg,mg,ng) += 
                                            ( 
                                                -p*kronecker_delta(mg,ig)*kronecker_delta(ng,jg) + 1./3.0 * kronecker_delta(mg,pg) * kronecker_delta(ig,jg) 

                                            ) * pow(intermediate, -0.5)  ;
                                            // Not finished yet!
                                            // -
                                            // (
                                            //      (s(ig,jg)-p*alpha(ig,jg) + 1./3.0 *alpha(pg,qg) * kronecker_delta(ig,jg) * (s(pg,qg) - p*alpha(pg,qg))) *
                                            //      (kronecker_delta(mg,rg)*kronecker_delta(ng,sg) - 1./3.0*kronecker_delta(mg,ng)*kronecker_delta(rg,sg) 
                                            //         +1./3.0 * kronecker_delta(mg,ng) * alpha(rg,sg)) *
                                            //      (s(rg,sg)-p*alpha(rg,sg)) 
                                            // ) * pow(intermediate, -1.5); 
                                    }


        return dm__dalpha;
    }


private:

    AlphaType &alpha_;
    KType &k_;

    static DTensor2 s; //sigma deviator
    static DTensor2 result; //For returning Dtensor2s
    static DTensor4 dm__dsigma; //For returning dm_over_dsigma
    static DTensor4 dm__dalpha; //For returning dm_over_dsigma

};

template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);
template<typename AlphaHardeningType, typename KHardeningType>
DTensor4 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::dm__dsigma(3, 3, 3, 3, 0.0);
template<typename AlphaHardeningType, typename KHardeningType>
DTensor4 DruckerPrager_PF<AlphaHardeningType , KHardeningType >::dm__dalpha(3, 3, 3, 3, 0.0);

#endif