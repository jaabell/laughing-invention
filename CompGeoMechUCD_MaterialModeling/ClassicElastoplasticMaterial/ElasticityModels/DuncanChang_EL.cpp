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

#include "DuncanChang_EL.h"
#include "Vector.h"
#include "../ClassicElastoplasticityGlobals.h"

DTensor4 DuncanChang_EL::Ee(3, 3, 3, 3, 0.0);


DuncanChang_EL::DuncanChang_EL(double K_in, double pa_in, double n_in, double nu_in, double sigma3_max_in) :
    ElasticityBase<DuncanChang_EL>::ElasticityBase(),  // Note the full-qualification of ElasticityBase through the scope resolution operator (::)
    K(K_in) , pa(pa_in) , n(n_in) , nu(nu_in), sigma3_max(sigma3_max_in)
{

}


DTensor4& DuncanChang_EL::operator()(const DTensor2& stress) //See note on base class
{
    using namespace ClassicElastoplasticityGlobals;

    double p, q, theta;
    std::tie(p, q, theta) = getpqtheta(stress);

    double sigma3 = -p + (2 * q) / 3 * cos(theta * M_PI / 180 + (2 * M_PI) / 3);

    if (sigma3 > sigma3_max)
    {
        sigma3 = sigma3_max;
    }

    double E = K * pa * pow(sigma3_max / pa, n);
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

int DuncanChang_EL::sendSelf(int commitTag, Channel &theChannel)
{
    static Vector data(4);
    data(0) = K;
    data(1) = pa;
    data(2) = n;
    data(3) = nu;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "DuncanChang_EL::sendSelf() - Failed to send data. " << endl;
        return -1;
    }

    return 0;
}

int DuncanChang_EL::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(4);

    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "DuncanChang_EL::receiveSelf() - Failed to receive data. " << endl;
        return -1;
    }

    K = data(0);
    pa = data(1);
    n = data(2);
    nu = data(3);

    return 0;
}
