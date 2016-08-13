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

#ifndef DruckerPragerNonAssociate_PF_H
#define DruckerPragerNonAssociate_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"





template<typename AlphaHardeningType, typename KHardeningType>
class DruckerPragerNonAssociate_PF : public PlasticFlowBase<DruckerPragerNonAssociate_PF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<double, KHardeningType> KType;


    // PlasticFlowBase<DruckerPragerNonAssociate_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    DruckerPragerNonAssociate_PF( AlphaType &alpha_in, KType &k_in, double const& xi_in, double const& Kd_in):
        PlasticFlowBase<DruckerPragerNonAssociate_PF<AlphaHardeningType , KHardeningType >>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
                alpha_(alpha_in), k_(k_in), xi_(xi_in), Kd_(Kd_in)
    {

    }


    const DTensor2& operator()(const DTensor2 &depsilon, const DTensor2& sigma)
    {
        //Identical to derivative of VonMises_YF wrt sigma (a.k.a nij)
        const DTensor2 &alpha = alpha_.getVariableConstReference();

        //Zero these tensors
        s *= 0;
        result *= 0;

        // (1) Governing the deviatoric component.
        double p = -sigma(i, i) / 3;
        s(i, j) = sigma(i, j) + p * kronecker_delta(i, j);
        result(i, j) = s(i, j) - p * alpha(i, j);
        double den = sqrt(result(i, j) * result(i, j));

        if (den == 0){
            return result;
        }else{
            result(i, j) = result(i, j) / den;
        }

        // (2) Governing the volumetric component.
        // cout<<"DruckerPragerNonAssociate_PF: --> Kd_         : "<<Kd_<<endl;
        // cout<<"DruckerPragerNonAssociate_PF: --> xi_         : "<<xi_<<endl;

        double dilatancy_D= xi_ * (sqrt(2.0/3.0)*Kd_ - sqrt( (s(i,j)/p) * (s(i,j)/p) ));
        result(i,j)=result(i,j)- 1.0 / 3.0 * dilatancy_D * kronecker_delta(i, j);

        // cout<<"DruckerPragerNonAssociate_PF: --> dilatancy_D : "<<dilatancy_D<<endl;

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
        static DTensor4 placeholder(3,3,3,3,0.0);
        return placeholder;
    }


    // placeholder:
    DTensor2 const& dm_over_dq_start_h_star(const DTensor2& stress){
        static DTensor2 s(3, 3, 0.0);
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        // const double &k = k_.getVariableConstReference();
        double p=0;
        stress.compute_deviatoric_tensor(s, p); // here p is positive if in tension
        p=-p;

        static DTensor4 IdentityTensor4(3,3,3,3, 0); //optimize this to global later.
        IdentityTensor4(i,j,k,l)=kronecker_delta(i, j)*kronecker_delta(k,l);
        // (1) von Mises material always has this part zero. 
        // double dm_dk=0.0; 
        // (2) dm_dalpha part
        static DTensor4 dm_dalpha(3,3,3,3,0.0);
        static DTensor2 s_minus_alpha(3,3,0.0);
        s_minus_alpha(i,j) = s(i,j) - alpha(i,j);
        double s_minus_alpha_square = s_minus_alpha(i,j) * s_minus_alpha(i,j) ; 

        dm_dalpha(i,j,m,n) = - IdentityTensor4(i,m,j,n) * pow(s_minus_alpha_square,-0.5) + s_minus_alpha(i,j)*s_minus_alpha(m,n) * pow(s_minus_alpha_square,-1.5);
        static DTensor2 ret(3,3,0.0);
        ret(i,j) = dm_dalpha(i,j,m,n)*alpha(m,n);

        return ret;
    }

private:

    AlphaType &alpha_;
    KType &k_;
    double xi_;
    double Kd_;
    static DTensor2 s; //sigma deviator
    static DTensor2 result; //For returning Dtensor2s

};


template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPragerNonAssociate_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 DruckerPragerNonAssociate_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);

#endif




    // DTensor4 const& dm_over_dalpha(DTensor2 const& sigma, DTensor2 const& m){
    //     static DTensor4 placeholder(3,3,3,3,0.0);
    //     return placeholder;
    // }