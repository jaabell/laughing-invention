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
    bool (*checkunit)(const string&, const Quantity&);

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

        if ( !checkunit(name,qty))
        {
            // cout << endl << endl << "     Error: " << name << " = " << qty << " does not have the required units." << endl;
            return false;
        }

        i++;
    }

    return true;
}


