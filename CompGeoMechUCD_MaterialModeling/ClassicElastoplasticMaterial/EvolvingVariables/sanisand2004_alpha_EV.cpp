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

#define UNLOAD_PROD_MIN 1e-8

sanisand2004_alpha_EV::sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_, const double& e_, const DTensor2& alpha_in_)
    :
    EvolvingVariable(DTensor2(3, 3, 0.0)),
    G0(G0_), h0(h0_), ch(ch_), patm(patm_), e(e_), alpha_in(alpha_in_)
{}

sanisand2004_alpha_EV::sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_, const double& e_, const DTensor2& alpha_in_,  DTensor2& alpha0)
    :
    EvolvingVariable(alpha0), G0(G0_), h0(h0_), ch(ch_), patm(patm_), e(e_), alpha_in(alpha_in_)
{}

const DTensor2& sanisand2004_alpha_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &m,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;
    //Zero de static variable
    static DTensor2 b(3, 3, 0.0);
    static DTensor2 s(3, 3, 0.0);
    static DTensor2 n_dev(3, 3, 0.0);
    static DTensor2 derivative(3, 3, 0.0);
    double p;
    b *= 0;
    s *= 0;
    n_dev *= 0;
    derivative *= 0;

    //Compute stress deviator and mean stress
    stress.compute_deviatoric_tensor(s, p);
    p = -p;

    //Unit deviatoric tensor normal
    const DTensor2 &alpha = this->getVariableConstReference();
    n_dev(i, j) = s(i, j) - p * alpha(i, j);            // Given in Andrade after Eqn 9. on Page 86
    double norm_ndev = sqrt(n_dev(i, j) * n_dev(i, j));
    n_dev(i, j) = n_dev(i, j) / norm_ndev;

    //Hardening coefficient
    double unload_prod = (alpha(i, j) - alpha_in(i, j) ) * n_dev(i, j);
    double b0 = G0 * h0 * (1.0 - ch * e) / sqrt(p / patm);     // Andrade Eqn 9, Dafalias Eqn 6
    double h;
    if (unload_prod > 0)
    {
        h = b0 / unload_prod;                               // Andrade Eqn 9, Dafalias Eqn 6
    }
    else
    {
        h = b0 / UNLOAD_PROD_MIN;
    }
    //Compute the derivative (hardening function)
    derivative(i, j) =  (2 * h) * b(i, j) / 3;                 // Andrade Eqn 9, Dafalias Table 2 Eqn for Backstress evolution

    return derivative;
}



int sanisand2004_alpha_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9 + 4);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

    data(pos++) = G0;
    data(pos++) = h0;
    data(pos++) = ch;
    data(pos++) = patm;
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
    static Vector data(9 + 9 + 4);
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