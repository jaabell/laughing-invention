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

#include "sanisand2004_z_EV.h"
#include <Vector.h>

#define UNLOAD_PROD_MIN 1e-8

sanisand2004_z_EV::sanisand2004_z_EV(double A0_, double cz_, double zmax_, double patm_,
                                     double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nd_,
                                     const double* m_ptr_,
                                     const double* e_ptr_, const DTensor2* alpha_ptr_)
    :
    EvolvingVariable(DTensor2(3, 3, 0.0)),
    A0(A0_), cz(cz_), zmax(zmax_), patm(patm_),
    Me(Me_), Mc(Mc_), ec0(ec0_), lambda_c(lambda_c_), xi(xi_), nd(nd_),
    m_ptr(m_ptr_),
    e_ptr(e_ptr_),
    alpha_ptr(alpha_ptr_)
{}

sanisand2004_z_EV::sanisand2004_z_EV(double A0_, double cz_, double zmax_, double patm_,
                                     double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nd_,
                                     const double* m_ptr_,
                                     const double* e_ptr_, const DTensor2* alpha_ptr_,  DTensor2& z0)
    :
    EvolvingVariable(z0),
    A0(A0_), cz(cz_), zmax(zmax_), patm(patm_),
    Me(Me_), Mc(Mc_), ec0(ec0_), lambda_c(lambda_c_), xi(xi_), nd(nd_),
    m_ptr(m_ptr_),
    e_ptr(e_ptr_),
    alpha_ptr(alpha_ptr_)
{}

const DTensor2& sanisand2004_z_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &dQdsigma_ij,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;

    //Zero de static variable
    static DTensor2 d(3, 3, 0.0);
    static DTensor2 s(3, 3, 0.0);
    static DTensor2 n_dev(3, 3, 0.0);
    static DTensor2 derivative(3, 3, 0.0);
    d *= 0;
    s *= 0;
    n_dev *= 0;
    derivative *= 0;

    //Compute stress deviator and mean stress
    double p;
    stress.compute_deviatoric_tensor(s, p);
    p = -p;


    // const double& m = *m_ptr;
    // const double& e = *e_ptr;
    const DTensor2& alpha = *alpha_ptr;


    //Unit deviatoric tensor normal
    n_dev(i, j) = s(i, j) - p * alpha(i, j);            // Given in Andrade after Eqn 9. on Page 86
    double norm_ndev = sqrt(n_dev(i, j) * n_dev(i, j));
    n_dev(i, j) = n_dev(i, j) / norm_ndev;

    // double tr_n_dev_cubed = n_dev(i, j) * n_dev(j, k) * n_dev(k, i);
    // double cos3theta = -sqrt(6.) * tr_n_dev_cubed;      //Andrade parragraph after Eq 12
    // double c = Me / Mc;                                                 //Andrade parragraph after Eq 12
    // double g = 2 * c / ((1. + c) - (1 - c) * cos3theta);                //Andrade Eq 12
    // double M = g * Mc;                                                  //Andrade Eq 12
    // double ec = ec0 - lambda_c * pow(p / patm, xi);                     //Andrade Eq 11
    // double psi = e - ec;                                                //State paramter (Andrade parragraph after Eq 11)
    // double alpha0_d = M * exp(nd * psi) - m;                           //Andrade Eqn 10
    // d(i, j) = SQRT_2_over_3 * alpha0_d * n_dev(i, j) - alpha(i, j);     //Andrade Eqn 10

    // //Compute the derivative (hardening function)
    const DTensor2 &z = this->getVariableConstReference();
    // double brak_zn = macaulay_bracket(z(i, j) * n_dev(i, j));           // <zn> operator
    // double Ad = A0 * (1. + brak_zn);                                    //Andrade eqn 15

    // double D0 = SQRT_2_over_3 * M * exp(nd * psi) - (s(i, j) * n_dev(i, j)) / p;
    // double D = -Ad * D0;

    double D = dQdsigma_ij(i, i);

    double brak_minus_D = macaulay_bracket(D);                                 // <D> operator
    derivative(i, j) =  -cz * brak_minus_D * (zmax * n_dev(i, j) + z(i, j));           // Andrade Eqn 16

    // cout << "  z -> n_dev = " << n_dev << endl;
    // cout << "  z -> alpha ("  << &alpha << ") = " << alpha << endl;

    // cout << "  z -> A0             = " << A0  << endl;
    // cout << "  z -> nd             = " << nd  << endl;
    // cout << "  z -> zmax           = " << zmax  << endl;
    // cout << "  z -> cz             = " << cz  << endl;
    // cout << "  z -> norm_ndev      = " << norm_ndev  << endl;
    // cout << "  z -> tr_n_dev_cubed = " << tr_n_dev_cubed  << endl;
    // cout << "  z -> cos3theta      = " << cos3theta  << endl;
    // cout << "  z -> c              = " << c  << endl;
    // cout << "  z -> g              = " << g  << endl;
    // cout << "  z -> M              = " << M  << endl;
    // cout << "  z -> ec             = " << ec  << endl;
    // cout << "  z -> ec0            = " << ec0  << endl;
    // cout << "  z -> lambda_c       = " << lambda_c  << endl;
    // cout << "  z -> p              = " << p  << endl;
    // cout << "  z -> patm           = " << patm  << endl;
    // cout << "  z -> xi             = " << xi  << endl;
    // cout << "  z -> e              = " << e  << endl;
    // cout << "  z -> psi            = " << psi  << endl;
    // cout << "  z -> alpha0_d       = " << alpha0_d  << endl;
    // cout << "  z -> brak_zn        = " << brak_zn  << endl;
    // cout << "  z -> Ad             = " << Ad  << endl;
    // cout << "  z -> D              = " << D  << endl;
    // cout << "  z -> brak_minus_D   = " << brak_minus_D  << endl;

    return derivative;
}



int sanisand2004_z_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9 + 4 + 6);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

    data(pos++) = A0;
    data(pos++) = cz;
    data(pos++) = zmax;
    data(pos++) = patm;

    data(pos++) = Me;
    data(pos++) = Mc;
    data(pos++) = ec0;
    data(pos++) = lambda_c;
    data(pos++) = xi;
    data(pos++) = nd;


    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            data(pos++) = a(i, j);
        }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            data(pos++) = a_committed(i, j);
        }
    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_z_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int sanisand2004_z_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(9 + 9 + 4 + 6);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_z_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    static DTensor2 tmp_a(3, 3, 0);
    static DTensor2 tmp_a_committed(3, 3, 0);
    A0 = data(pos++);
    cz = data(pos++);
    zmax = data(pos++);
    patm = data(pos++);

    Me = data(pos++) ;
    Mc = data(pos++) ;
    ec0 = data(pos++) ;
    lambda_c = data(pos++) ;
    xi = data(pos++) ;
    nd = data(pos++) ;

    // TODO: This loop should get taken care of by the variables themselves.
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            tmp_a(i, j) = data(pos++);
        }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            tmp_a_committed(i, j) = data(pos++) ;
        }

    this->setVar(tmp_a);
    this->setCommittedVar(tmp_a_committed);

    return 0;
}