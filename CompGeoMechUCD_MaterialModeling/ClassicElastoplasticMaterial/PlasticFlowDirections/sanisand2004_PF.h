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





template<class AlphaHardeningType, class ZHardeningType, class MHardeningType, class EHardeningType>
class sanisand2004_PF : public PlasticFlowBase<sanisand2004_PF<AlphaHardeningType, ZHardeningType, MHardeningType, EHardeningType>> // CRTP
{
public:

    typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
    typedef EvolvingVariable<DTensor2, ZHardeningType> ZType;
    typedef EvolvingVariable<double, MHardeningType> MType;
    typedef EvolvingVariable<double, EHardeningType> EType;


    // PlasticFlowBase<sanisand2004_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
    sanisand2004_PF( double Me_, double Mc_, double A0_,
                     double ec0_, double lambda_c_, double patm_, double  xi_, double nd_,
                     EType &e_in,
                     AlphaType &alpha_in, ZType &z_in, MType &m_in):
        PlasticFlowBase<sanisand2004_PF<AlphaHardeningType , ZHardeningType, MHardeningType, EHardeningType>>::PlasticFlowBase(),
                Me(Me_),
                Mc(Mc_),
                A0(A0_),
                ec0(ec0_), lambda_c(lambda_c_), patm(patm_), xi(xi_), nd(nd_),
                e_(e_in), alpha_(alpha_in), z_(z_in), m_(m_in)
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

        double tr_n_dev_cubed = n_dev(i, j) * n_dev(j, k) * n_dev(k, i);
        double cos3theta = -sqrt(6.) * tr_n_dev_cubed;      //Andrade parragraph after Eq 12
        double c = Me / Mc;                                                 //Andrade parragraph after Eq 12
        double g = 2 * c / ((1. + c) - (1 - c) * cos3theta);                //Andrade Eq 12
        double B = 1. + 3 * (1 - c) / (2 * c) * g * cos3theta;              //Andrade parragraph after Eq 13
        double C = (3 / SQRT_2_over_3) * (1 - c) / c * g;                 //Andrade parragraph after Eq 13

        double M = g * Mc;                                                  //Andrade Eq 12
        double ec = ec0 - lambda_c * pow(p / patm, xi);                     //Andrade Eq 11
        const double &e = e_.getVariableConstReference();
        double psi = e - ec;
        const DTensor2 &z = z_.getVariableConstReference();
        double brak_zn = macaulay_bracket(z(i, j) * n_dev(i, j));           // <zn> operator
        double Ad = A0 * (1. + brak_zn);                                    //Andrade eqn 15

        const double & m = m_.getVariableConstReference();
        double alpha0_d = M * exp(nd * psi) - m;                           //Andrade Eqn 10
        d(i, j) = SQRT_2_over_3 * alpha0_d * n_dev(i, j) - alpha(i, j);     //Andrade Eqn 10


        double D = Ad * d(i, j) * n_dev(i, j) ;
        cout << "  pf -> A0             = " << A0  << endl;
        cout << "  pf -> nd             = " << nd  << endl;
        cout << "  pf -> Me             = " << Me << endl;
        cout << "  pf -> Mc             = " << Mc << endl;
        cout << "  pf -> M              = " << M << endl;
        cout << "  pf -> A0             = " << A0 << endl;
        cout << "  pf -> norm_ndev      = " << norm_ndev << endl;
        cout << "  pf -> tr_n_dev_cubed = " << tr_n_dev_cubed << endl;
        cout << "  pf -> cos3theta      = " << cos3theta << endl;
        cout << "  pf -> c              = " << c << endl;
        cout << "  pf -> g              = " << g << endl;
        cout << "  pf -> B              = " << B << endl;
        cout << "  pf -> C              = " << C << endl;
        cout << "  pf -> ec             = " << ec  << endl;
        cout << "  pf -> ec0            = " << ec0  << endl;
        cout << "  pf -> lambda_c       = " << lambda_c  << endl;
        cout << "  pf -> p              = " << p  << endl;
        cout << "  pf -> patm           = " << patm  << endl;
        cout << "  pf -> xi             = " << xi  << endl;
        cout << "  pf -> e              = " << e  << endl;
        cout << "  pf -> psi            = " << psi  << endl;
        cout << "  pf -> alpha0_d       = " << alpha0_d  << endl;
        cout << "  pf -> brak_zn        = " << brak_zn  << endl;
        cout << "  pf -> Ad             = " << Ad  << endl;
        cout << "  pf -> D              = " << D << endl;

        R(i, j) = B * n_dev(i, j) + C * (n_dev2(i, j) - kronecker_delta(i, j) / 3) - D * kronecker_delta(i, j) / 3 ;  // Andrade Eqn 13
        // R(i, j) = B * n_dev(i, j) - C * (n_dev2(i, j) - kronecker_delta(i, j) / 3) + D * kronecker_delta(i, j) / 3 ;  // Andrade Eqn 13

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
    double ec0, lambda_c, patm, xi, nd;

    EType & e_;
    AlphaType &alpha_;
    ZType &z_;
    MType &m_;


};

#endif

