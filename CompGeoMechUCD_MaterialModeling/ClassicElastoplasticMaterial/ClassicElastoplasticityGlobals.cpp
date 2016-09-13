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


#include "ClassicElastoplasticityGlobals.h"
Index<'i'> ClassicElastoplasticityGlobals::i;
Index<'j'> ClassicElastoplasticityGlobals::j;
Index<'k'> ClassicElastoplasticityGlobals::k;
Index<'l'> ClassicElastoplasticityGlobals::l;
Index<'m'> ClassicElastoplasticityGlobals::m;
Index<'n'> ClassicElastoplasticityGlobals::n;
Index<'p'> ClassicElastoplasticityGlobals::p;
Index<'q'> ClassicElastoplasticityGlobals::q;
Index<'r'> ClassicElastoplasticityGlobals::r;
Index<'s'> ClassicElastoplasticityGlobals::s;

DTensor2 ClassicElastoplasticityGlobals::kronecker_delta(3, 3, "identity");

