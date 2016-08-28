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

#ifndef VonMises_YF_H
#define VonMises_YF_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../YieldFunctionBase.h"
#include "cmath"
#include <iostream>

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"
using namespace ClassicElastoplasticityGlobals;




template<class AlphaHardeningType, class KHardeningType>
class VonMises_YF : public YieldFunctionBase<VonMises_YF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    VonMises_YF( AlphaType &alpha_in, KType& k_in):
        YieldFunctionBase<VonMises_YF<AlphaHardeningType, KHardeningType>>::YieldFunctionBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                alpha_(alpha_in), k_(k_in)
    {
        // std::cout << "k_in = " << k_in << std::endl;
        // std::cout << "&k_in = " << &k_in << std::endl;
    }

    double operator()(const DTensor2& sigma) const
    {
        double p;
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        const double &k = k_.getVariableConstReference();
        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension

        // cout << "   YF : k = " << k << endl;

        return sqrt( (s(i, j) - alpha(i, j)) * (s(i, j) - alpha(i, j)) ) - SQRT_2_over_3 * k ;  // This one assumes p positive in tension
    }

    const DTensor2& df_dsigma_ij(const DTensor2& sigma)
    {

        const DTensor2 &alpha = alpha_.getVariableConstReference();


        //Zero these tensors
        s *= 0;
        result *= 0;

        double p;

        sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension

        double den = sqrt((s(i, j) - alpha(i, j)) * (s(i, j) - alpha(i, j)));

        if (den == 0)
        {
            return result;
        }

        result(i, j) = (s(i, j) - alpha(i, j)) / den;

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

        //Denominator of the expression
        double den = sqrt((s(i, j) - alpha(i, j)) * (s(i, j) - alpha(i, j)));

        if (den == 0)
        {
            return 0;
        }

        // This is for the hardening of k
        constexpr double df_dk = -SQRT_2_over_3;
        dbl_result +=  df_dk * k_.getDerivative(depsilon, m, sigma);

        //This is for the hardening of alpha
        dbl_result +=  -(s(i, j) - alpha(i, j)) / den * alpha_.getDerivative(depsilon, m, sigma)(i, j);

        return dbl_result;
    }

    // DTensor4 const& dalpha_over_dsigma(DTensor2 const& sigma){

    // }
    // DTensor4 const& dalpha_over_dalpha(DTensor2 const& sigma){

    // }
    int getHardeningType(){
        double H_alpha=alpha_.getHardeningType();
        double H_k=k_.getHardeningType();
        int HARDENING_TYPE=0;
        if( abs(H_alpha)< MACHINE_EPSILON &&  abs(H_k)<MACHINE_EPSILON){
          HARDENING_TYPE = Perfectly_Plastic;
        }else if(abs(H_alpha)<MACHINE_EPSILON &&  abs(H_k)>MACHINE_EPSILON){
          HARDENING_TYPE = One_Isotropic_Hardening_Only;
        }else if(abs(H_alpha)>MACHINE_EPSILON &&  abs(H_k)<MACHINE_EPSILON){
          HARDENING_TYPE = One_Kinematic_Hardening_Only;
        }else{
          HARDENING_TYPE = Both_One_Isotropic_One_Kinematic_Hardening;
        }
        return HARDENING_TYPE;
    }
private:

    AlphaType &alpha_;
    KType &k_;
    static DTensor2 s; //Stress deviator
    static DTensor2 result; //For returning Dtensor2's

};

template <class AlphaHardeningType,  class KHardeningType>
DTensor2 VonMises_YF<AlphaHardeningType, KHardeningType>::s(3, 3, 0.0);
template <class AlphaHardeningType,  class KHardeningType>
DTensor2 VonMises_YF<AlphaHardeningType, KHardeningType>::result(3, 3, 0.0);


#endif