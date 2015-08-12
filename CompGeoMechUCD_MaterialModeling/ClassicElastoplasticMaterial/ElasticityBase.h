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

#ifndef ElasticityBase_H
#define ElasticityBase_H

#include "../../ltensor/LTensor.h"
#include "EvolvingVariable.h"


template <class T>
class ElasticityBase
{
public:

	ElasticityBase()
	{
		// Derived classes will have custom constructors.
	}

	// Note the use of the Curiously-recurring-template-pattern
	// Operator () retunrs a const-reference. Thereforce, implementation must provide
	// a reference to a persistent data member. Usually this one is declared static
	// so that storage and be reused across instances, and we avoid calls to malloc.
	DTensor4& operator()(const DTensor2 &strain, const DTensor2 &plastic_strain, const DTensor2& stress) // Best practise here would be to return a const reference. But we can't due to LTensor design limitation. Hopefully this will be solved in the future.... by us. -J.Abell
	{
		return static_cast<T*>(this)->operator()( strain, plastic_strain,  stress);
	}


private:
	// Derived classes will store their parameters and such.
};




#endif