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

#ifndef EvolvingScalar_H
#define EvolvingScalar_H

#include "../../ltensor/LTensor.h"

template<class VarType, class Derivative>
class EvolvingVariable
{
public:
	// EvolvingVariable(Derivative d_) : a(0.0), d(d_) {}

	EvolvingVariable(VarType a_, Derivative *d_): a(a_), d(d_) {}

	VarType getDerivative(const DTensor2 &strain, const DTensor2 &plastic_strain, const DTensor2& stress) const
	{
		// Derivative d(strain, plastic_strain, stress);
		return (*d)(strain, plastic_strain, stress);
	};

	void evolve(double dlambda, const DTensor2& strain, const DTensor2 &plastic_strain, const DTensor2& stress)
	{
		VarType h = getDerivative(strain, plastic_strain, stress);
		h *= dlambda;
		a +=  h;
	}

	VarType getVariable() const
	{
		return a;
	}


private:
	VarType a;
	Derivative *d;

};


#endif