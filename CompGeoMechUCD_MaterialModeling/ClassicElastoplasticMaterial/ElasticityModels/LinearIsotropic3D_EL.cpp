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

#include "LinearIsotropic3D_EL.h"
#include "Vector.h"

DTensor4 LinearIsotropic3D_EL::Ee(3, 3, 3, 3, 0.0);


LinearIsotropic3D_EL::LinearIsotropic3D_EL(double E, double nu) : ElasticityBase<LinearIsotropic3D_EL>::ElasticityBase()  // Note the full-qualification of ElasticityBase through the scope resolution operator (::)
{
    lambda = ( nu * E ) / ( ( 1.0 + nu ) * ( 1.0 - 2.0 * nu ) );
    mu = E / ( 2.0 * ( 1.0 + nu ) );
    // std::cout << "E  = " << E << std::endl;
    // std::cout << "nu = " << nu << std::endl;
}


DTensor4& LinearIsotropic3D_EL::operator()(const DTensor2& stress) //See note on base class
{
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

int LinearIsotropic3D_EL::sendSelf(int commitTag, Channel &theChannel)
{
    static Vector data(2);
    data(0) = lambda;
    data(1) = mu;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "LinearIsotropic3D_EL::sendSelf() - Failed to send data. " << endl;
        return -1;
    }

    return 0;
}

int LinearIsotropic3D_EL::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(2);

    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "LinearIsotropic3D_EL::receiveSelf() - Failed to receive data. " << endl;
        return -1;
    }

    lambda = data(0);
    mu = data(1);

    return 0;
}
