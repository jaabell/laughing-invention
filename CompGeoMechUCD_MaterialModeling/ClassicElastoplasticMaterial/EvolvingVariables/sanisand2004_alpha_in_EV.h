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


#ifndef sanisand2004_alpha_in_EV_H
#define sanisand2004_alpha_in_EV_H

#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.


class sanisand2004_alpha_in_EV;

template< >
struct evolving_variable_implements_custom_evolve_function<sanisand2004_alpha_in_EV>
{
    static const bool value = true;
};

class sanisand2004_alpha_in_EV : public EvolvingVariable<DTensor2, sanisand2004_alpha_in_EV> //CRTP on sanisand2004_alpha_in_EV
{
public:

    sanisand2004_alpha_in_EV(const DTensor2* alpha_ptr_);
    sanisand2004_alpha_in_EV(DTensor2& alpha_in0,
                             const DTensor2* alpha_ptr_);

    const DTensor2& getDerivative(const DTensor2 &depsilon,
                                  const DTensor2 &m,
                                  const DTensor2& stress) const;
    void evolve(double dlambda,
                const DTensor2& depsilon,
                const DTensor2& m,
                const DTensor2& sigma);

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:

    const DTensor2* alpha_ptr;
};



#endif