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


#ifndef sanisand2004_e_EV_H
#define sanisand2004_e_EV_H


#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.

class sanisand2004_e_EV;

// This class overloads the evolve, as it is not a proper internal variable

template< >
struct evolving_variable_implements_custom_evolve_function<sanisand2004_e_EV>
{
    static const bool value = true;
};

class sanisand2004_e_EV : public EvolvingVariable<double, sanisand2004_e_EV> //CRTP on sanisand2004_e_EV
{
public:

    sanisand2004_e_EV( double e_in);

    const double& getDerivative(const DTensor2 &depsilon,
                                const DTensor2 &m,
                                const DTensor2& stress) const;
    void evolve(double dlambda,
                const DTensor2& depsilon,
                const DTensor2& m,
                const DTensor2& sigma);
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);


private:
    static double derivative; //Must return a reference.
};


#endif