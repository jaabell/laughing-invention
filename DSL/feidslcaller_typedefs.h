#ifndef FEIDSLCALLER_INCLUDES_H
#define FEIDSLCALLER_INCLUDES_H

#include <iostream>
#include <vector>
#include <string>

typedef bool(*UnitCheckerFunctionPointerType)(const Quantity&) ;
typedef std::vector<Expression*> ArgumentType;
typedef std::vector<Expression*>::const_iterator ArgumentIterator;
typedef std::pair<string, UnitCheckerFunctionPointerType> SignatureElementType;
typedef std::vector<SignatureElementType> SignatureType;
typedef std::vector<SignatureElementType>::const_iterator SignatureIterator;

#endif // FEIDSLCALLER_INCLUDES_H
