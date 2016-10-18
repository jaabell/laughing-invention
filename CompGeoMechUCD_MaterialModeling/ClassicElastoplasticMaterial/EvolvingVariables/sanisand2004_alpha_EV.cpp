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

#include "sanisand2004_alpha_EV.h"
#include <Vector.h>

#define UNLOAD_PROD_MIN 1e-12

sanisand2004_alpha_EV::sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_,
        double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nb_, double m_,
        const double* e_ptr_, const double* m_ptr_,  const DTensor2* alpha_in_ptr_)
    :
    EvolvingVariable(DTensor2(3, 3, 0.0)),
    G0(G0_), h0(h0_), ch(ch_), patm(patm_),
    Me(Me_), Mc(Mc_), ec0(ec0_), lambda_c(lambda_c_), xi(xi_), nb(nb_),
    e_ptr(e_ptr_), m_ptr(m_ptr_), alpha_in_ptr(alpha_in_ptr_)
{}

sanisand2004_alpha_EV::sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_,
        double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nb_, double m_,
        const double* e_ptr_, const double* m_ptr_, const DTensor2* alpha_in_ptr_,  DTensor2& alpha0)
    :
    EvolvingVariable(alpha0), G0(G0_), h0(h0_), ch(ch_), patm(patm_),
    Me(Me_), Mc(Mc_), ec0(ec0_), lambda_c(lambda_c_), xi(xi_), nb(nb_),
    e_ptr(e_ptr_), m_ptr(m_ptr_), alpha_in_ptr(alpha_in_ptr_)
{}

const DTensor2& sanisand2004_alpha_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &dQdsigma_ij,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;

    //Zero de static variable
    static DTensor2 b(3, 3, 0.0);
    static DTensor2 s(3, 3, 0.0);
    static DTensor2 r(3, 3, 0.0);
    static DTensor2 n_dev(3, 3, 0.0);
    static DTensor2 derivative(3, 3, 0.0);
    b *= 0;
    s *= 0;
    r *= 0;
    n_dev *= 0;
    derivative *= 0;

    //Compute stress deviator and mean stress
    double p;
    stress.compute_deviatoric_tensor(s, p);
    p = -p;

    //Unit deviatoric tensor normal
    const double& m = *m_ptr;
    const DTensor2 &alpha = this->getVariableConstReference();
    // r(i, j) = s(i, j) / p;
    // n_dev(i, j) = r(i, j) - alpha(i, j);            // Given in Andrade after Eqn 9. on Page 86
    // double norm_ndev = SQRT_2_over_3 * m; //sqrt(n_dev(i, j) * n_dev(i, j));
    n_dev(i, j) = s(i, j) - p * alpha(i, j);
    double norm_ndev = sqrt(n_dev(i, j) * n_dev(i, j));
    n_dev(i, j) = n_dev(i, j) / norm_ndev;


    //Hardening coefficient
    const DTensor2& alpha_in = *alpha_in_ptr;
    const double& e = *e_ptr;
    double unload_prod = (alpha(i, j) - alpha_in(i, j) ) * n_dev(i, j);
    double b0 = G0 * h0 * (1.0 - ch * e) * pow(p / patm, -0.5);     // Andrade Eqn 9, Dafalias Eqn 6
    double h;
    if (unload_prod > 0)
    {
        h = b0 / unload_prod;                               // Andrade Eqn 9, Dafalias Eqn 6
    }
    else
    {
        h = b0 / UNLOAD_PROD_MIN;
    }

    double tr_n_dev_cubed = n_dev(i, j) * n_dev(j, k) * n_dev(k, i);
    double cos3theta = -sqrt(6.) * tr_n_dev_cubed;      //Andrade parragraph after Eq 12
    double c = Me / Mc;                                                 //Andrade parragraph after Eq 12
    double g = 2 * c / ((1. + c) - (1 - c) * cos3theta);                //Andrade Eq 12
    double M = g * Mc;                                                  //Andrade Eq 12
    double ec = ec0 - lambda_c * pow(p / patm, xi);                     //Andrade Eq 11
    double psi = e - ec;                                                //State paramter (Andrade parragraph after Eq 11)
    double alpha0_b = M * exp(-nb * psi) - m;                           //Andrade Eqn 10
    b(i, j) = SQRT_2_over_3 * alpha0_b * n_dev(i, j) - alpha(i, j);     //Andrade Eqn 10

    // cout << "  alpha -> n_dev = " << n_dev << endl;
    // cout << "  alpha -> alpha ("  << &alpha << ") = " << alpha << endl;
    // cout << "alpha_in ("  << &alpha_in << ")= " << alpha_in << endl;
    // cout << "  alpha -> G0          = "  << G0 << endl;
    // cout << "  alpha -> h0          = "  << h0 << endl;
    // cout << "  alpha -> ch          = "  << ch << endl;
    // cout << "  alpha -> nb          = "  << nb << endl;
    // cout << "  alpha -> p           = "  << p << endl;
    // cout << "  alpha -> norm_ndev   = "  << norm_ndev << endl;
    // cout << "  alpha -> e           = "  << e << endl;
    // cout << "  alpha -> unload_prod = "  << unload_prod << endl;
    // cout << "  alpha -> b0          = "  << b0 << endl;
    // cout << "  alpha -> h           = "  << h << endl;
    // cout << "  alpha -> tr_n_dev_cubed = "  << tr_n_dev_cubed << endl;
    // cout << "  alpha -> cos3theta   = "  << cos3theta << endl;
    // cout << "  alpha -> c           = "  << c << endl;
    // cout << "  alpha -> g           = "  << g << endl;
    // cout << "  alpha -> M           = "  << M << endl;
    // cout << "  alpha -> ec             = " << ec  << endl;
    // cout << "  alpha -> ec0            = " << ec0  << endl;
    // cout << "  alpha -> lambda_c       = " << lambda_c  << endl;
    // cout << "  alpha -> m                = " << m  << endl;
    // cout << "  alpha -> p              = " << p  << endl;
    // cout << "  alpha -> patm           = " << patm  << endl;
    // cout << "  alpha -> xi             = " << xi  << endl;
    // cout << "  alpha -> e              = " << e  << endl;
    // cout << "  alpha -> psi         = "  << psi << endl;
    // cout << "  alpha -> alpha0_b    = "  << alpha0_b << endl;

    //Compute the derivative (hardening function)
    derivative(i, j) =  (2 * h) * b(i, j) / 3;                 // Andrade Eqn 9, Dafalias Table 2 Eqn for Backstress evolution

    return derivative;
}



int sanisand2004_alpha_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9 + 4 + 7);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

    data(pos++) = G0;
    data(pos++) = h0;
    data(pos++) = ch;
    data(pos++) = patm;

    data(pos++) = Me;
    data(pos++) = Mc;
    data(pos++) = ec0;
    data(pos++) = lambda_c;
    data(pos++) = xi;
    data(pos++) = nb;
    data(pos++) = 0;


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
        cerr << "sanisand2004_alpha_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int sanisand2004_alpha_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(9 + 9 + 4 + 7);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_alpha_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    static DTensor2 tmp_a(3, 3, 0);
    static DTensor2 tmp_a_committed(3, 3, 0);
    G0 = data(pos++);
    h0 = data(pos++);
    ch = data(pos++);
    patm = data(pos++);

    Me = data(pos++) ;
    Mc = data(pos++) ;
    ec0 = data(pos++) ;
    lambda_c = data(pos++) ;
    xi = data(pos++) ;
    nb = data(pos++) ;
    // m = data(pos++) ;

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