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

// Implemented by Jose Abell, Oct 2016
// In what follows
//   "Andrade" is  ---> T. Mohammadnejad and J. E. Andrade, “Flow liquefaction instability prediction using finite elements,” Acta Geotech., pp. 83–100, 2014.
//   "Dafalias" is ---> M. Taiebat and Y. F. Dafalias, “{SANISAND}: Simple anisotropic sand plasticity model,” Int. J. Numer. Anal. Methods Geomech., 2008.
//
//  I find Andrade's exposition of the SAniSand 2004 material better.


#ifndef sanisand2004_PF_H
#define sanisand2004_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"
#include "../EvolvingVariable.h"
// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"





template<class AlphaHardeningType, class ZHardeningType, class MHardeningType>
class sanisand2004_PF : public PlasticFlowBase<sanisand2004_PF<AlphaHardeningType, ZHardeningType, MHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<DTensor2, ZHardeningType> ZType;
    typedef EvolvingVariable<double, MHardeningType> MType;


    // PlasticFlowBase<sanisand2004_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    sanisand2004_PF( AlphaType &alpha_in, ZType &z_in, MType &m_in):
        PlasticFlowBase<sanisand2004_PF<AlphaHardeningType , ZHardeningType, MHardeningType >>::PlasticFlowBase(),
                alpha_(alpha_in), z_(z_in), m_(m_in)
    {

    }


    const DTensor2& operator()(const DTensor2 &depsilon, const DTensor2& sigma)
    {
        static DTensor2 R(3, 3, 0);
        static DTensor2 s(3, 3, 0);
        static DTensor2 n_dev(3, 3, 0);
        static DTensor2 n_dev2(3, 3, 0);
        static DTensor2 d(3, 3, 0);
        R *= 0;
        s *= 0;
        n_dev *= 0;
        n_dev2 *= 0;
        d *= 0;

        //Compute stress deviator and mean stress
        double p;
        const DTensor2 &alpha = alpha_.getVariableConstReference();
        sigma.compute_deviatoric_tensor(s, p);
        p = -p;

        //Unit deviatoric tensor normal
        n_dev(i, j) = s(i, j) - p * alpha(i, j);            // Given in Andrade after Eqn 9. on Page 86
        double norm_ndev = sqrt(n_dev(i, j) * n_dev(i, j));
        n_dev(i, j) = n_dev(i, j) / norm_ndev;
        n_dev2(i, j) = n_dev(i, k) * n_dev(k, j);

        //Identical to derivative of VonMises_YF wrt sigma (a.k.a nij)

        double tr_n_dev = n_dev(i, i);
        double cos3theta = -sqrt(6.) * tr_n_dev * tr_n_dev * tr_n_dev;      //Andrade parragraph after Eq 12
        double c = Me / Mc;                                                 //Andrade parragraph after Eq 12
        double g = 2 * c / ((1. + c) - (1 - c) * cos3theta);                //Andrade Eq 12
        double B = 1. + 3 * (1 - c) / (2 * c * g * cos3theta);              //Andrade parragraph after Eq 13
        double C = (2 / SQRT_2_over_3) * (1 - c) / (c * g);                 //Andrade parragraph after Eq 13

        const DTensor2 &z = z_.getVariableConstReference();
        double brak_zn = macaulay_bracket(z(i, j) * n_dev(i, j));           // <zn> operator
        double Ad = A0 * (1. + brak_zn);                                    //Andrade eqn 15
        double D = Ad * d(i, j) * n_dev(i, j) ;

        R(i, j) = B * n_dev(i, j) + C * (n_dev2(i, j) - kronecker_delta(i, j) / 3) - D * kronecker_delta(i, j) / 3 ;  // Andrade Eqn 13

        return R;
    }

    DTensor4 const& dm_over_dsigma(DTensor2 const& sigma)
    {
        static DTensor4 dm__dsigma(3, 3, 3, 3, 0.0);
        dm__dsigma *= 0;

        //Unimplemented yet

        return dm__dsigma;
    }


    DTensor2 const& dm_over_dq_start_h_star(DTensor2 const& depsilon, DTensor2 const& pf_m, const DTensor2& stress)
    {
        static DTensor2 result(3, 3, 0.0);
        result *= 0;

        //Unimplemented yet

        return result;
    }


private:

    //Non evolving variables
    double Me, Mc, A0;

    AlphaType &alpha_;
    ZType &z_;
    MType &m_;

};

#endif

