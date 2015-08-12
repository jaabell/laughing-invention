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


#ifndef ClassicElastoplasticityGlobals_H
#define ClassicElastoplasticityGlobals_H

#include "../../ltensor/LTensor.h"

namespace ClassicElastoplasticityGlobals
{

//Reserved LTensor indexes
extern Index<'i'> i;
extern Index<'j'> j;
extern Index<'k'> k;
extern Index<'l'> l;
extern Index<'m'> m;
extern Index<'n'> n;
extern Index<'p'> p;
extern Index<'q'> q;
extern Index<'r'> r;
extern Index<'s'> s;
extern DTensor2 kronecker_delta;

constexpr double SQRT_2_over_3  = sqrt(2.0 / 3.0);
constexpr double SQRT_2_over_27 = sqrt(2.0 / 27.0);


}

#endif