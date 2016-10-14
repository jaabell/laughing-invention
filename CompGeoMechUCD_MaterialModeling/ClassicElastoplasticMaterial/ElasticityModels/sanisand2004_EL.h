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

#ifndef sanisand2004_EL_H
#define sanisand2004_EL_H

#include "../../../ltensor/LTensor.h"
#include "../EvolvingVariable.h"
#include "../ElasticityBase.h"

#include <iostream>


class sanisand2004_EL : public ElasticityBase<sanisand2004_EL> // CRTP on ElasticityBase
{
public:
    sanisand2004_EL(double G0_in, double patm_in, double nu_in, const double& e_in);

    DTensor4& operator()(const DTensor2& stress); //See note on base class

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:

    double G0;
    double patm;
    double nu;
    const double &e;
    static DTensor4 Ee;  //Provides class-wide storage, which avoids mallocs and allows const returning a const & to this object.
};



#endif
