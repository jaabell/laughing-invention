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

#include "sanisand2004_alpha_in_EV.h"
#include <Vector.h>

#define UNLOAD_PROD_MIN 1e-8

sanisand2004_alpha_in_EV::sanisand2004_alpha_in_EV(const DTensor2* alpha_ptr_)
    :
    EvolvingVariable(DTensor2(3, 3, 0.0)),
    alpha_ptr(alpha_ptr_)
{}

sanisand2004_alpha_in_EV::sanisand2004_alpha_in_EV(DTensor2& alpha_in0, const DTensor2* alpha_ptr_  )
    :
    EvolvingVariable(alpha_in0),  alpha_ptr(alpha_ptr_)
{}

const DTensor2& sanisand2004_alpha_in_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &dQdsigma_ij,
        const DTensor2& stress) const
{
    static DTensor2 derivative(3, 3, 0.0);
    derivative *= 0;
    //Defines its own evolution

    return derivative;
}


void sanisand2004_alpha_in_EV::evolve(double dlambda,
                                      const DTensor2& depsilon,
                                      const DTensor2& m,
                                      const DTensor2& sigma)
{
    using namespace ClassicElastoplasticityGlobals;
    const DTensor2& alpha = *alpha_ptr;
    const DTensor2& alpha_in = this->getVariableConstReference();

    static DTensor2 s(3, 3, 0);
    static DTensor2 n_dev(3, 3, 0);
    s *= 0;
    n_dev *= 0;

    //Compute stress deviator and mean stress
    double p;
    sigma.compute_deviatoric_tensor(s, p);
    p = -p;

    //Unit deviatoric tensor normal
    n_dev(i, j) = s(i, j) - p * alpha(i, j);            // Given in Andrade after Eqn 9. on Page 86
    double norm_ndev = sqrt(n_dev(i, j) * n_dev(i, j));
    n_dev(i, j) = n_dev(i, j) / norm_ndev;

    double unload_prod = (alpha(i, j) - alpha_in(i, j)) * n_dev(i, j);

    if (unload_prod <= 0)
    {
        cout << "(updating alpha_in) unload_prod = " << unload_prod << endl;
        this->setVar(alpha);
    }
}


int sanisand2004_alpha_in_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

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
        cerr << "sanisand2004_alpha_in_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int sanisand2004_alpha_in_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(9 + 9);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_alpha_in_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    static DTensor2 tmp_a(3, 3, 0);
    static DTensor2 tmp_a_committed(3, 3, 0);

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