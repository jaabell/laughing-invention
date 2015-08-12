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

#ifndef EvolvingVariable_H
#define EvolvingVariable_H

#include "../../ltensor/LTensor.h"
#include <iostream>

//Forward declaration needed for declaring the operator overload << (friend)
template<class VarType, class T>
class EvolvingVariable;

//Forward declaration
template<class VarType, class T>
std::ostream& operator<<(std::ostream& os, const EvolvingVariable<VarType, T>& obj);




template<class VarType, class T>
class EvolvingVariable
{
public:

	EvolvingVariable(VarType a_): a(a_) {}

	const VarType &getDerivative(const DTensor2 &depsilon,
	                             const DTensor2 &m,
	                             const DTensor2& sigma) const
	{
		return static_cast<const T*>(this)->getDerivative(depsilon,  m,  sigma);
	};

	EvolvingVariable<VarType, T> & operator= ( const EvolvingVariable<VarType, T> & other)
	{
		//Check self-assignment
		if (&other == this)
		{
			return *this;
		}

		a = other.a;

		return *this;
	}

	void evolve(double dlambda,
	            const DTensor2& depsilon,
	            const DTensor2& m,
	            const DTensor2& sigma)
	{
		const VarType& h = getDerivative(depsilon, m, sigma);
		tmp = h;
		tmp *= dlambda;

		cout << "a = " << a << endl;

		a +=  tmp;
		cout << "     EVOOOOOLVE to a = " << a << endl;
	}

	const VarType& getVariableConstReference() const
	{
		return a;
	}

	void setVar(VarType v)
	{
		a = v;
	}

	//Overloaded operators.
	operator VarType () { return a; }  // Convert into variable type
	friend std::ostream& operator<< <>(std::ostream& os, const EvolvingVariable<VarType, T>& obj);


private:
	VarType a;
	static VarType tmp;
};





// Forward stream operators to underlying class
template<class VarType, class T>
std::ostream& operator<<(std::ostream& os, const EvolvingVariable<VarType, T>& obj)
{
	os << obj.a;
	return os;
}

template<class VarType, class T>
VarType EvolvingVariable<VarType, T>::tmp;

#endif