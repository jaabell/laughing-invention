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

#include "CamClay_EL.h"
#include "Vector.h"

DTensor4 CamClay_EL::Ee(3, 3, 3, 3, 0.0);


CamClay_EL::CamClay_EL(double e0_, double kappa_, double nu_) : ElasticityBase<CamClay_EL>::ElasticityBase(),  // Note the full-qualification of ElasticityBase through the scope resolution operator (::)
    e0(e0_),
    kappa(kappa_),
    nu(nu_)
{

}


DTensor4& CamClay_EL::operator()(const DTensor2& stress) //See note on base class
{
    Ee *= 0; //Zero it. It may have values from another instance with different parameters;
    double p = -stress(i, i) / 3;
    double e = e0 - kappa * log(p);
    double K = (1 + e) * p / kappa;
    double lambda = 3 * K * nu / (1 + nu);
    double mu = 3 * K * (1 - 2 * nu) / (2 * (1 + nu));

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

int CamClay_EL::sendSelf(int commitTag, Channel &theChannel)
{
    static Vector data(3);
    data(0) = e0;
    data(1) = kappa;
    data(2) = nu;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "CamClay_EL::sendSelf() - Failed to send data. " << endl;
        return -1;
    }

    return 0;
}

int CamClay_EL::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(3);

    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "CamClay_EL::receiveSelf() - Failed to receive data. " << endl;
        return -1;
    }

    e0 = data(0);
    kappa = data(1);
    nu = data(2);

    return 0;
}
