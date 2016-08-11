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


#ifndef Linear_HardeningScalarV_H
#define Linear_HardeningScalarV_H


#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.


class LinearHardeningScalar_EV : public EvolvingVariable<double, LinearHardeningScalar_EV> //CRTP on LinearHardeningScalar_EV
{
public:

    LinearHardeningScalar_EV( double H_);

    LinearHardeningScalar_EV( double H_, double k0);

    const double& getDerivative(const DTensor2 &depsilon,
                                const DTensor2 &m,
                                const DTensor2& stress) const;
    double const& getHardeningType() const;
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);


private:
    double H;
    static double derivative; //Must return a reference.
};


#endif