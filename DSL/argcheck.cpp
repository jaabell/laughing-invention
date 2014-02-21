#include <iostream>
#include <string>
#include <map>

#include "siunit.h"
#include "quantity.h"

#include "argcheck.h"
#include "feiast.h"
#include "feidslcaller_typedefs.h"

bool argcheck(ArgumentType arguments, SignatureType signature)
{
    SignatureIterator sig_it;
    ArgumentIterator  arg_it;
    bool (*checkunit)(const Quantity&);

    if (arguments.size() != signature.size())
    {
        cout << "Error: argcheck: arguments.size() != signature.size()" << endl;
        return false;
    }

    int i = 1;

    for ( sig_it = signature.begin(), arg_it = arguments.begin() ;
            sig_it != signature.end();
            sig_it++, arg_it++ )
    {
        string name = sig_it -> first;
        checkunit = sig_it -> second;   // This is a function pointer to the respective unit checker
        Quantity qty = (*arg_it) -> value();

        if ( checkunit(qty) != 1)
        {
            cout << endl << endl << "     Error: " << name << " = " << qty << " does not have the required units." << endl;
            return false;
        }

        i++;
    }

    return true;
}


