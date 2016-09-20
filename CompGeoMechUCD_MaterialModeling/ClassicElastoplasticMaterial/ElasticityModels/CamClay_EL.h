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

#ifndef CamClay_EL_H
#define CamClay_EL_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../ElasticityBase.h"

#include <iostream>


class CamClay_EL : public ElasticityBase<CamClay_EL> // CRTP on ElasticityBase
{
public:
    CamClay_EL(double e0_, double kappa_, double nu_);

    DTensor4& operator()(const DTensor2& stress); //See note on base class

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:

    double e0;
    double kappa;
    double nu;
    static DTensor4 Ee;  //Provides class-wide storage, which avoids mallocs and allows const returning a const & to this object.

};



#endif
