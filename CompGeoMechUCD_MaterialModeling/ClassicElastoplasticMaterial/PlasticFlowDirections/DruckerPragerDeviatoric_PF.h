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

#ifndef DruckerPragerDeviatoric_PF_H
#define DruckerPragerDeviatoric_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"





template<class AlphaHardeningType, class KHardeningType>
class DruckerPragerDeviatoric_PF : public PlasticFlowBase<DruckerPragerDeviatoric_PF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    // PlasticFlowBase<DruckerPragerDeviatoric_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    DruckerPragerDeviatoric_PF( AlphaType &alpha_in, KType &k_in):
        PlasticFlowBase<DruckerPragerDeviatoric_PF<AlphaHardeningType , KHardeningType >>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
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
        result(i, j) = s(i, j) - p * alpha(i, j);
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
        double p=0.0;
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;
        // if(p<MACHINE_EPSILON){
        //     cout<<"DruckerPragerNonAssociate_PF:: pressuse < 0 ! The Drucker-Prager has tensile force.\n";
        // }
        static DTensor2 s_minus_palpha(3,3,0.0);
        s_minus_palpha(i,j) = s(i,j) - p*alpha(i,j);
        double s_minus_p_alpha_square = s_minus_palpha(i,j) * s_minus_palpha(i,j) ; 
        double s_square = s(i,j) * s(i,j) ; 
        // =========================================
        // the naive for-loop:
        // =========================================
        // pre-computation for the dummy indices. 
        double alpha_square = alpha(i,j) * alpha(i,j);
        double alpha_volume = alpha(i,i);
        double alpha_times_s= alpha(i,j) * s(i,j);
        // =========================================
        static DTensor4 dm__dsigma(3,3,3,3,0.0);
        dm__dsigma*=0;
        // Four free indices
        for (int ig = 0; ig < 3; ++ig)
            for (int jg = 0; jg < 3; ++jg)
                for (int mg = 0; mg < 3; ++mg)
                    for (int ng = 0; ng < 3; ++ng){
                        dm__dsigma(ig,jg,mg,ng) = 
                            ( 
                                kronecker_delta(mg,ig)*kronecker_delta(ng,jg) 
                                - 1./3.0 * kronecker_delta(mg,ng) * kronecker_delta(ig,jg) 
                                + 1./3.0 * kronecker_delta(mg,ng)*alpha(ig,jg)   
                            ) * pow(s_minus_p_alpha_square, -0.5)  
                            -
                            (
                                s_minus_palpha(ig,jg) 
                                *
                                (
                                    s_minus_palpha(mg,ng)
                                    - 1./3.0 * kronecker_delta(mg,ng) * (-p*alpha_volume)
                                    + 1./3.0 * kronecker_delta(mg,ng) * (alpha_times_s-p*alpha_square)
                                ) 
                                * 
                                pow(s_minus_p_alpha_square, -1.5)
                            )   
                            + 
                            (
                                1./3. * kronecker_delta(ig,jg) * s(mg,ng) * pow(p,-1) 
                            ) * pow(s_square,-0.5) 
                            +
                            (
                                1./9. * kronecker_delta(ig,jg) * kronecker_delta(mg,ng) * pow(p,-2)
                            ) * pow(s_square,0.5); 
                    }


        return dm__dsigma;
    }
    

    DTensor2 const& dm_over_dq_start_h_star(const DTensor2& stress){
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        const double &k = k_.getVariableConstReference();
        double p=0;
        stress.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;

        static DTensor4 IdentityTensor4(3,3,3,3, 0); //optimize this to global later.
        IdentityTensor4(i,j,k,l)=kronecker_delta(i, j)*kronecker_delta(k,l);
        // (1) von Mises material always has this part zero. 
        static DTensor2 dm_dk(3,3,0.0);
        dm_dk(i,j) = SQRT_2_over_27 * kronecker_delta(i, j) ; 

        // (2) dm_dalpha part
        static DTensor4 dm_dalpha(3,3,3,3,0.0);
        static DTensor2 s_minus_p_alpha(3,3,0.0);
        s_minus_p_alpha(i,j) = s(i,j) - p * alpha(i,j);
        double s_minus_p_alpha_square = s_minus_p_alpha(i,j) * s_minus_p_alpha(i,j) ; 

        for (int ig = 0; ig < 3; ++ig)
            for (int jg = 0; jg < 3; ++jg)
                for (int kg = 0; kg < 3; ++kg)
                    for (int lg = 0; lg < 3; ++lg){
                        dm_dalpha(ig,jg,kg,lg) = 
                            (
                                - p * IdentityTensor4(kg,ig,lg,jg) 
                            ) * pow(s_minus_p_alpha_square,-0.5) 
                            -
                            (
                                s_minus_p_alpha(ig,jg) 
                                *(- p *  s_minus_p_alpha(kg,lg) )
                                * pow(s_minus_p_alpha_square,-1.5)
                            );
                        }

        static DTensor2 ret(3,3,0.0);
        ret(i,j) = dm_dalpha(i,j,m,n)*alpha(m,n);
                
        return ret;
    }
private:

    AlphaType &alpha_;
    KType &k_;

    static DTensor2 s; //sigma deviator
    static DTensor2 result; //For returning Dtensor2s

};


template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPragerDeviatoric_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPragerDeviatoric_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);

#endif



    // DTensor4 const& dm_over_dalpha(DTensor2 const& sigma, DTensor2 const& m){
    //     static DTensor4 placeholder(3,3,3,3,0.0);
    //     return placeholder;
    // }
    // 