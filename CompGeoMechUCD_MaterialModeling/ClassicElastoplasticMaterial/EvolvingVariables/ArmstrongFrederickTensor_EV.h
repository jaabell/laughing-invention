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


#ifndef ArmstrongFrederickTensor_EV_H
#define ArmstrongFrederickTensor_EV_H

#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.



class ArmstrongFrederickTensor_EV : public EvolvingVariable<DTensor2, ArmstrongFrederickTensor_EV> //CRTP on ArmstrongFrederickTensor_EV
{
public:

    ArmstrongFrederickTensor_EV( double ha_, double cr_);
    ArmstrongFrederickTensor_EV( double ha_, double cr_, DTensor2& alpha0);

    const DTensor2& getDerivative(const DTensor2 &depsilon,
                                  const DTensor2 &m,
                                  const DTensor2& stress) const;
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:
    double ha, cr;
    static DTensor2 derivative;     // Needs to be static so multiple instances only do one malloc call and we can return a const-reference
};



#endif