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


#ifndef LinearHardeningTensor_EV_H
#define LinearHardeningTensor_EV_H

#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.


class LinearHardeningTensor_EV : public EvolvingVariable<DTensor2, LinearHardeningTensor_EV> //CRTP on LinearHardeningTensor_EV
{
public:

	LinearHardeningTensor_EV( double H_) : EvolvingVariable(DTensor2(3, 3, 0.0)), H(H_)
	{}

	LinearHardeningTensor_EV( double H_, DTensor2& alpha0) : EvolvingVariable(alpha0), H(H_)
	{}

	const DTensor2& getDerivative(const DTensor2 &depsilon,
	                              const DTensor2 &m,
	                              const DTensor2& stress) const
	{
		using namespace ClassicElastoplasticityGlobals;
		//Zero de static variable
		derivative *= 0;

		//Compute the derivative (hardening function)
		derivative(i, j) =  H * (m(i, j) - (m(k, k) / 3) * kronecker_delta(i, j));

		return derivative;
	}



private:
	double H;
	static DTensor2 derivative;		// Needs to be static so multiple instances only do one malloc call and we can return a const-reference
};

DTensor2 LinearHardeningTensor_EV::derivative(3, 3, 0.0);



#endif