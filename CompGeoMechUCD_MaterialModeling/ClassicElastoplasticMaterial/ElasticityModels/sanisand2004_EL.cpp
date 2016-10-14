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

#include "sanisand2004_EL.h"
#include "Vector.h"
#include "../ClassicElastoplasticityGlobals.h"

DTensor4 sanisand2004_EL::Ee(3, 3, 3, 3, 0.0);


sanisand2004_EL::sanisand2004_EL(double G0_in, double patm_in, double nu_in, const double& e_in) :
    ElasticityBase<sanisand2004_EL>::ElasticityBase(),  // Note the full-qualification of ElasticityBase through the scope resolution operator (::)
    G0(G0_in) , patm(patm_in) , nu(nu_in) , e(e_in)
{

}


DTensor4 & sanisand2004_EL::operator()(const DTensor2& stress) //See note on base class
{
    using namespace ClassicElastoplasticityGlobals;

    double p = -stress(i, i) / 3;

    if (p < patm)
    {
        p = patm;
    }
    double G = G0 * patm * (2.97 - e) * (2.97 - e) / (1 + e) * sqrt(p / patm);
    double E = 2 * (1 + nu) * G;
    double lambda = ( nu * E ) / ( ( 1.0 + nu ) * ( 1.0 - 2.0 * nu ) );
    double mu = E / ( 2.0 * ( 1.0 + nu ) );

    Ee *= 0; //Zero it. It may have values from another instance with different parameters;
    Ee( 0, 0, 0, 0 ) = lambda + 2 * mu;
    Ee( 0, 0, 1, 1 ) = lambda;
    Ee( 0, 0, 2, 2 ) = lambda;
    Ee( 0, 1, 0, 1 ) = mu;
    Ee( 0, 1, 1, 0 ) = mu;
    Ee( 0, 2, 0, 2 ) = mu;
    Ee( 0, 2, 2, 0 ) = mu;
    Ee( 1, 0, 0, 1 ) = mu;
    Ee( 1, 0, 1, 0 ) = mu;
    Ee( 1, 1, 0, 0 ) = lambda;
    Ee( 1, 1, 1, 1 ) = lambda + 2 * mu;
    Ee( 1, 1, 2, 2 ) = lambda;
    Ee( 1, 2, 1, 2 ) = mu;
    Ee( 1, 2, 2, 1 ) = mu;
    Ee( 2, 0, 0, 2 ) = mu;
    Ee( 2, 0, 2, 0 ) = mu;
    Ee( 2, 1, 1, 2 ) = mu;
    Ee( 2, 1, 2, 1 ) = mu;
    Ee( 2, 2, 0, 0 ) = lambda;
    Ee( 2, 2, 1, 1 ) = lambda;
    Ee( 2, 2, 2, 2 ) = lambda + 2 * mu;

    return Ee;
}

int sanisand2004_EL::sendSelf(int commitTag, Channel &theChannel)
{
    static Vector data(3);
    data(0) = G0;
    data(1) = patm;
    data(2) = nu;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_EL::sendSelf() - Failed to send data. " << endl;
        return -1;
    }

    return 0;
}

int sanisand2004_EL::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(3);

    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_EL::receiveSelf() - Failed to receive data. " << endl;
        return -1;
    }

    G0 = data(0);
    patm = data(1);
    nu = data(2);

    return 0;
}
