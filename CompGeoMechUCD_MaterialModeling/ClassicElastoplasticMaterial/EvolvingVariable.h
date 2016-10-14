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
#include "CEPTraits.h"
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

    EvolvingVariable(VarType a_): a(a_), a_committed(a_), a_tmp(a_)
    {
        // a_tmp = a_;
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
        a_tmp = other.a_tmp;
        a_committed = other.a_committed;

        return *this;
    }

    template <typename U = T>
    typename std::enable_if < !evolving_variable_implements_custom_evolve_function<U>::value, void >::type
    evolve(double dlambda,
           const DTensor2& depsilon,
           const DTensor2& m,
           const DTensor2& sigma)
    {
        const VarType& h = getDerivative(depsilon, m, sigma);
        static VarType aux(a);
        aux = h;
        aux *= dlambda;
        a +=  aux;
    }

    template <typename U = T>
    typename std::enable_if < evolving_variable_implements_custom_evolve_function<U>::value, void >::type
    evolve(double dlambda,
           const DTensor2& depsilon,
           const DTensor2& m,
           const DTensor2& sigma)
    {
        static_cast<U*>(this)->evolve(dlambda, depsilon, m, sigma);
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
        // cout << "Final commit state from " << a_committed << " to " << a << endl;
        a_committed = a;
    }

    void revert()
    {
        a = a_committed;
    }

    void commit_tmp()
    {
        // cout << "Commiting from " << a_tmp << " to " << a << endl;
        a_tmp = a;
    }

    void revert_tmp()
    {
        // cout << "Reverting from " << a << " to " << a_tmp << endl;
        a = a_tmp;
    }

    void revert_to_start()
    {
        // cout << "Reverting from " << a << " to " << a_tmp << endl;
        a = 0 * a_tmp;
    }

    void print(std::ostream &s)
    {
        s << "     > a           = " << a << endl;
        s << "     > a_tmp       = " << a << endl;
        s << "     > a_committed = " << a << endl;
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

    // ///////////////////////////////////////////////////////////////////////////////////
    // Set the limit for the hardening saturation.
    // With small strain increment, the hardening saturation is implemented smoothly.
    // With great strain increment, the overshooting creates problems. So set limitation.
    // //////////////////////////////////////////////////////////////////////////////////
    template <typename U = T>
    typename std::enable_if < !requires_hardening_saturation_limit_check<U>::requires >::type
    check_hardening_saturation_limit_(VarType& a, DTensor2 const& plasticFlow_m) {}

    template <typename U = T>
    typename std::enable_if<requires_hardening_saturation_limit_check<U>::requires>::type
    check_hardening_saturation_limit_(VarType& a, DTensor2 const& plasticFlow_m)
    {
        static_cast<U*>(this)->check_hardening_saturation_limit(a, plasticFlow_m);
    }
    // ///////////////////////////////////////////////////////////////////////////////////
private:
    VarType a;
    VarType a_committed;
    VarType a_tmp;
    static VarType da_1;
    static VarType da_2;
};





// Forward stream operators to underlying class
template<class VarType, class T>
std::ostream& operator<<(std::ostream& os, const EvolvingVariable<VarType, T>& obj)
{
    os << obj.a;
    return os;
}

// template<class VarType, class T>
// VarType EvolvingVariable<VarType, T>::a_tmp;

#endif