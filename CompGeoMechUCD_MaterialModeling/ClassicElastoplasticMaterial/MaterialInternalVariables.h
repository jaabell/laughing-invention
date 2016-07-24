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
#include <Channel.h>


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

    void setVars(const MaterialInternalVariables<>& vars) {}

    int sendSelf(int commitTag, Channel &theChannel)
    {
        return 0;
    }

    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
    {
        return 0;
    }

    void commit() { }
    void revert() { }
    void commit_tmp() { }
    void revert_tmp() { }
    void print() { }

};

template <class Q, class... Qs>
struct MaterialInternalVariables<Q, Qs...> : MaterialInternalVariables<Qs...>
{
    MaterialInternalVariables(Q &q, Qs&... qs) : MaterialInternalVariables<Qs...>(qs...), q_i(q) {}

    void setVars(const MaterialInternalVariables<Q, Qs...>& vars)
    {
        q_i = vars.q_i;
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

    int sendSelf(int commitTag, Channel &theChannel)
    {
        q_i.sendSelf(commitTag, theChannel);
        static_cast<MaterialInternalVariables<Qs...>*>(this)->sendSelf(commitTag, theChannel);
        return 0;
    }

    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
    {
        q_i.receiveSelf(commitTag, theChannel, theBroker);
        static_cast<MaterialInternalVariables<Qs...>*>(this)->receiveSelf(commitTag, theChannel, theBroker);
        return 0;
    }

    void commit()
    {
        q_i.commit();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->commit();
    }

    void revert()
    {
        q_i.revert();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->revert();
    }

    void revert_to_start()
    {
        q_i.revert_to_start();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->revert_to_start();
    }

    void commit_tmp()
    {
        q_i.commit_tmp();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->commit_tmp();
    }

    void revert_tmp()
    {
        q_i.revert_tmp();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->revert_tmp();
    }

    void print()
    {
        q_i.print();
        static_cast<MaterialInternalVariables<Qs...>*>(this)->print();
    }


    Q& q_i;
};


#endif
