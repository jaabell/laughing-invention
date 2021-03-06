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

//Some basic trait classes to aid with implementation of CEP
#ifndef CEPTraits_H
#define CEPTraits_H

template< typename T >
struct supports_pre_integration_callback
{
    static const bool value = false;
};

// struct defines_pre_integration_callback
// {};

// template<>
// struct supports_pre_integration_callback<defines_pre_integration_callback>
// {
//     static const bool value = true;
// };


template< typename T >
struct supports_post_integration_callback
{
    static const bool value = false;
};

template< typename T >
struct supports_implicit_integration
{
    static const bool value = false;
};

template< typename VarType >
struct requires_hardening_saturation_limit_check
{
    static const bool requires = false;
};

template< typename VarType >
struct evolving_variable_implements_custom_evolve_function
{
    static const bool value = false;
};


#endif