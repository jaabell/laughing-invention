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

#ifndef MaterialInternalVariables_H
#define MaterialInternalVariables_H

#include <iostream>
#include "../../ltensor/LTensor.h"

template <class... Qs> struct MaterialInternalVariables
{
	void evolve(double dlambda,
	            const DTensor2& depsilon,
	            const DTensor2& m,
	            const DTensor2& sigma)
	{
		// Last guy does nothing, as it holds nothing.
		// Probably gets optimized out. (Empty struct optimization)
		// Needed to stop template recursion.
	}

	void setVars(const MaterialInternalVariables<>& vars)
	{
		//Does nothing.
	}
};

template <class Q, class... Qs>
struct MaterialInternalVariables<Q, Qs...> : MaterialInternalVariables<Qs...>
{
	MaterialInternalVariables(Q &q, Qs&... qs) : MaterialInternalVariables<Qs...>(qs...), q_i(q)
	{
		std::cout << "q_i  = " << &q_i << std::endl;
	}

	void setVars(const MaterialInternalVariables<Q, Qs...>& vars)
	{
		std::cout << "   --> Setting q_i =  "
		          << vars.q_i
		          << "   ("
		          << &q_i
		          << ") coming from ("
		          << &vars.q_i << ")" <<  std::endl;
		q_i = vars.q_i;
		std::cout << "  q_i is now " << q_i << "   ("
		          << &q_i
		          << ")" <<  endl;
		const MaterialInternalVariables<Qs...>* morevars = static_cast<const MaterialInternalVariables<Qs...>*>(&vars);

		static_cast<MaterialInternalVariables<Qs...>*>(this)->setVars(*morevars);
	}

	//Recurse, calling evolve on each internal variable.
	void evolve(double dlambda,
	            const DTensor2& depsilon,
	            const DTensor2& m,
	            const DTensor2& sigma)
	{
		q_i.evolve( dlambda,  depsilon,  m,  sigma);
		static_cast<MaterialInternalVariables<Qs...>*>(this)->evolve(dlambda,  depsilon,  m,  sigma);
	}

	Q& q_i;
};


#endif
