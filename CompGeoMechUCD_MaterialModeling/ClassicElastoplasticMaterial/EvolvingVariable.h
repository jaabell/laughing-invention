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
#include <Channel.h>
#include <iostream>
using namespace std;

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

    EvolvingVariable(VarType a_): a(a_), a_committed(a_)
    {
        // cout << "EvolvingVariable::EvolvingVariable(a) a_ = " << a_ << endl;
        // cout << "EvolvingVariable::EvolvingVariable(a) a_committed = " << a_committed << endl;
    }

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
        a_committed = other.a_committed;

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

        // cout << "a = " << a << endl;
        // cout << "h = " << a << endl;
        // cout << "dLambda = " << dlambda << endl;
        a +=  tmp;
        // cout << "     EVOOOOOLVE to a = " << a << endl;
    }

    const VarType& getVariableConstReference() const
    {
        return a;
    }

    const VarType& getCommittedVariableConstReference() const
    {
        return a_committed;
    }

    void setVar(VarType& v)
    {
        a = v;
    }

    void setCommittedVar(VarType& v)
    {
        a_committed = v;
    }

    void commit()
    {
        // cout << "a = " << a << endl;
        // cout << "a_committed = " << a_committed << endl;
        a_committed = a;
        // cout << "a_committed = " << a_committed << endl;
    }

    void revert()
    {
        a = a_committed;
    }

    //Overloaded operators.
    operator VarType ()
    {
        return a;    // Convert into variable type
    }
    friend std::ostream& operator<< <>(std::ostream& os, const EvolvingVariable<VarType, T>& obj);

    int sendSelf(int commitTag, Channel &theChannel)
    {
        return static_cast<T*>(this)->sendSelf(commitTag, theChannel);
    }

    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
    {
        return static_cast<T*>(this)->receiveSelf(commitTag, theChannel, theBroker);
    }


private:
    VarType a;
    VarType a_committed;
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