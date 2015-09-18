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


#include "LinearHardeningTensor_EV.h"

DTensor2 LinearHardeningTensor_EV::derivative(3, 3, 0.0);

LinearHardeningTensor_EV::LinearHardeningTensor_EV( double H_) : EvolvingVariable(DTensor2(3, 3, 0.0)), H(H_)
{}

LinearHardeningTensor_EV::LinearHardeningTensor_EV( double H_, DTensor2& alpha0) : EvolvingVariable(alpha0), H(H_)
{}

const DTensor2& LinearHardeningTensor_EV::getDerivative(const DTensor2 &depsilon,
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


