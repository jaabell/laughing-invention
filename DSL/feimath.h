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




#ifndef FEIMATH
#define FEIMATH

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <map>
#include <set>

#include "feiast.h"
#include "feiexceptions.h"
#include "quantity.h"
#include "unitdefinitions.h"

double defaultfunction(double x)
{
    return 0.0;
}

class FeiMath : public Expression
{
    Expression* evalme;
    string function_name;
    double (*function) (double );

public:
    FeiMath (string f, Expression* e)
    {
        evalme = e;
        function_name = f;

        function = &defaultfunction;

        // Function from cmath.h
        if (function_name.compare("cos") == 0 )
        {
            function = &cos;
        }

        if (function_name.compare("sin") == 0 )
        {
            function = &sin;
        }

        if (function_name.compare("tan") == 0 )
        {
            function = &tan;
        }

        if (function_name.compare("cosh") == 0 )
        {
            function = &cosh;
        }

        if (function_name.compare("sinh") == 0 )
        {
            function = &sinh;
        }

        if (function_name.compare("tanh") == 0 )
        {
            function = &tanh;
        }

        if (function_name.compare("acos") == 0 )
        {
            function = &acos;
        }

        if (function_name.compare("asin") == 0 )
        {
            function = &asin;
        }

        if (function_name.compare("atan") == 0 )
        {
            function = &atan;
        }

        //if (function_name.compare("atan2") == 0 ) function = &atan2;
        if (function_name.compare("sqrt") == 0 )
        {
            function = &sqrt;
        }

        if (function_name.compare("exp") == 0 )
        {
            function = &exp;
        }

        if (function_name.compare("log") == 0 )
        {
            function = &log;
        }

        if (function_name.compare("log10") == 0 )
        {
            function = &log10;
        }

        if (function_name.compare("ceil") == 0 )
        {
            function = &ceil;
        }

        if (function_name.compare("fabs") == 0 )
        {
            function = &fabs;
        }

        if (function_name.compare("floor") == 0 )
        {
            function = &floor;
        }

        // This should never occur if the function name is not in the lexer file.
        if (function == &defaultfunction)
        {
            string err = " FEIMATH: Unknown function name " + function_name + "(). \n" ;
            throw RunTimeException(err);
        }
    }
    virtual Quantity value ()
    {
        Quantity argument = evalme->value();
        Quantity result;

        if (isAdimensional(argument))
        {
            result = Quantity( (*function)( argument.Getvalue()) , ESSIunits::unitless);
        }
        else
        {
            string err = "Math error: " + function_name + "() not supported for non-adimensional units.\n";
            throw RunTimeException(err);
        }

        return result;
    }

    //Meet the Expression interface

    // Cloner
    virtual Expression* clone ()
    {
        return new FeiMath (*this);
    }

    // Copy constructor
    FeiMath& operator = (const FeiMath& other)
    {
        if (&other != this)
        {
            evalme = other.evalme -> clone();
            function_name = other.function_name;
            function = other.function;
        }

        return *this;
    }

    //Destructor
    virtual ~FeiMath ()
    {
    }

    FeiMath (const FeiMath& other) :
        function_name(other.function_name),
        function(other.function)
    {
        evalme = other.evalme -> clone();
    }
};


#endif // FEIMATH
