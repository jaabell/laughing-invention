#ifndef ARGCHECK
#define ARGCHECK

#include <iostream>
#include <string>
#include <map>

#include "siunit.h"
#include "quantity.h"
#include "feiast.h"

#include "feidslcaller_typedefs.h"
using namespace std;

bool argcheck(ArgumentType arguments, SignatureType signature);

//void showarglist(ArgumentType arguments, SignatureType signature);

#endif // ARGCHECK
