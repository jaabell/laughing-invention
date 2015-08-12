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

#ifndef VonMises_PF_H
#define VonMises_PF_H

#include "../../../ltensor/LTensor.h"
#include "../PlasticFlowBase.h"

// Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.
#include "../ClassicElastoplasticityGlobals.h"





template<class AlphaHardeningType, class KHardeningType>
class VonMises_PF : public PlasticFlowBase<VonMises_PF<AlphaHardeningType, KHardeningType>> // CRTP
{
public:

	typedef EvolvingVariable<DTensor2, AlphaHardeningType> AlphaType;
	typedef EvolvingVariable<double, KHardeningType> KType;


	// PlasticFlowBase<VonMises_PF<HardeningType>>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
	VonMises_PF( AlphaType &alpha_in, KType &k_in):
		PlasticFlowBase<VonMises_PF<AlphaHardeningType , KHardeningType >>::PlasticFlowBase(), // Note here that we need to fully-qualify the type of YieldFunctionBase, e.g. use scope resolution :: to tell compiler which instance of YieldFunctionBase will be used :/
		        alpha_(alpha_in), k_(k_in)
	{

	}


	const DTensor2& operator()(const DTensor2 &depsilon, const DTensor2& sigma)
	{
		//Identical to derivative of VonMises_YF wrt sigma (a.k.a nij)
		const DTensor2 &alpha = alpha_.getVariableConstReference();
		// const double &k = k_.getVariableConstReference();

		//Zero these tensors
		s *= 0;
		result *= 0;

		double p;

		sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension

		double den = sqrt((s(i, j) -  alpha(i, j)) * (s(i, j) -  alpha(i, j)));

		result(i, j) = (s(i, j) - alpha(i, j)) / den;

		return result;
	}

private:

	AlphaType &alpha_;
	KType &k_;

	static DTensor2 s; //sigma deviator
	static DTensor2 result; //For returning Dtensor2s

};


template<class AlphaHardeningType, class KHardeningType>
DTensor2 VonMises_PF<AlphaHardeningType , KHardeningType >::s(3, 3, 0.0);
template<class AlphaHardeningType, class KHardeningType>
DTensor2 VonMises_PF<AlphaHardeningType , KHardeningType >::result(3, 3, 0.0);

#endif