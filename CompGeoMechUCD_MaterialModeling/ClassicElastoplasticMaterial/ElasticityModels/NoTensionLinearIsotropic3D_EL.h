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

#ifndef NoTensionLinearIsotropic3D_EL_H
#define NoTensionLinearIsotropic3D_EL_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../ElasticityBase.h"

#include <iostream>


class NoTensionLinearIsotropic3D_EL : public ElasticityBase<NoTensionLinearIsotropic3D_EL> // CRTP on ElasticityBase
{
public:
    NoTensionLinearIsotropic3D_EL(double E, double nu);

    DTensor4& operator()(const DTensor2& stress); //See note on base class

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:

    double lambda;
    double mu;
    static DTensor4 Ee;  //Provides class-wide storage, which avoids mallocs and allows const returning a const & to this object.

};



#endif
