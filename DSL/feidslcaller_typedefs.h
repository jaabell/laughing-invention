///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Jan 2012
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////




#ifndef FEIDSLCALLER_INCLUDES_H
#define FEIDSLCALLER_INCLUDES_H

#include <iostream>
#include <vector>
#include <string>

typedef bool(*UnitCheckerFunctionPointerType)(const string&, const Quantity&) ;
typedef std::vector<Expression*> ArgumentType;
typedef std::vector<Expression*>::const_iterator ArgumentIterator;
typedef std::pair<string, UnitCheckerFunctionPointerType> SignatureElementType;
typedef std::vector<SignatureElementType> SignatureType;
typedef std::vector<SignatureElementType>::const_iterator SignatureIterator;

#endif // FEIDSLCALLER_INCLUDES_H
