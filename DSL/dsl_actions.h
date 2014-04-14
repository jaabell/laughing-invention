/*
    This file holds the definition of the DSL actions for FEI.
    Actions are commands which operate over the defined variables
    on the workspace. They are used to display variables, create elements,
    etc.

    DSL actions only return an execution code.
*/

#ifndef DSL_ACTIONS_H
#define DSL_ACTIONS_H

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <map>
#include <set>

#include "feiast.h"

/*

Guidelines to defining new DSL actions.

All DSL actions inherit from DslAction which, in turn, is a child of
Expression. This is requiered for the consistency of the DS language.

New actions must at *least*:
 + publicly inherit from DslAction.
 + Define a constructor which sets the data.
 + Define an execute() virtual member function which does the job
   assigned to the action.

Actions must operate only on passed variables or references.
DSL actions return a double.

To set the return value use the setRetunValue() member function within your
execute() code. setRetunValue() is inherited from DslAction. By default
the return value  is set to 0.0 so, if nothing is done, this value is
returned.

DSL actions also have a name which is set via the setDslActionName function
(which receives a string). By default this is set to the string "DslAction".

Print DSL action is commented to illustrate this.

The DslAction code is located in feiast.h near the end.

DSL ACTIONS SHOULD *NOT* CREATE NEW VARIABLES. New variables and
their handling is done within feiast.h. They could, however, change
the value of existing variables by using references or pointers.

The following is a basic template for a new action

class DslNewAction : public DslAction
{
    public:
        DslNewAction()
        {
        }

        void execute()
        {
        }

    private:
};

*/

using namespace std;

// These are provided to simplify setting and getting the internal variables.
typedef map<string, Quantity>::const_iterator varMapConstIterator;
typedef map<string, Quantity>::iterator varMapIterator;
typedef map<string, Quantity> varMapType;

// New actions start with the Dsl prefix name (good practise) and inherit
// publicly from DslAction.
class DslPrint : public DslAction
/*
 The print command recieves a string name representation of the variable
 name to be printed and reference to the variables map (vars).
 It checks whether the variable has been set. If it is set it prints
 out its value, if not it prints out this fact.
*/
{
    public:
        // Constructor sets internal data
        DslPrint (Expression *e)  // <- Actually a VariableReference type object. (see the cast below)
        {
            exp = e;
            setDslActionName("print");
        }

        //execute member function does the dirty work
        void execute()
        {
            if (exp != 0) //check for null pointer
            {
                VariableReference *vref_ptr =
                    dynamic_cast<VariableReference *>(exp);

                if (vref_ptr != 0)
                {
                    cout << vref_ptr->getName() << " = ";
                }

                FeiString *feistring_ptr = dynamic_cast<FeiString *>(exp);

                if (feistring_ptr != 0)
                {
                    cout << feistring_ptr->Getstring();
                }
                else
                {
                    cout << exp->value();
                }

                cout << endl;
            }
        }
        virtual Expression *clone ()
        {
            return new DslPrint (*this);
        }


    private:
        // internal data should always be private.
        // Remember the rule of 3 when holding pointers and
        // references to variables or objects.
        // http://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
        Expression *exp;
};

#include <iomanip>
// Next is the whos commands, which illustrates the use of references to variables
class DslWhos : public DslAction
{
    public:
        DslWhos(const varMapType &v , const set<string> &l) : vars(v), lockedvars(l)
        {
        }

        void execute()
        {
            cout << endl << "Declared variables:" << endl;

            for (varMapConstIterator iter = vars.begin();
                    iter != vars.end();
                    iter ++)
            {
                if (lockedvars.count(iter->first) != 0)
                {
                    cout << "  * " << setw(8) << iter->first << " = " << setw(15) << iter->second;
                    cout << endl;
                }
                else
                {
                    cout << "    " << setw(8) << iter->first << " = " << setw(15) << iter->second;
                    cout << endl;
                }
            }

            cout << endl << "    * = locked variable";
            cout << endl;
        }

        virtual Expression *clone ()
        {
            return new DslWhos (*this);
        }

    private:
        const varMapType &vars;
        const set<string> &lockedvars;
};



// Next is the whos commands, which illustrates the use of references.
class DslParseError : public DslAction
{
    public:
        DslParseError() : str("Unspecified error."), line(-1)
        {
        }
        DslParseError(const string s) : str(s), line(-1)
        {
        }
        DslParseError(const string s, const int l) : str(s), line(l)
        {
        }

        void execute()
        {
            cout << "Line " << line << ": Parse error --> " << str << endl;
        }

        virtual Expression *clone ()
        {
            return new DslParseError (*this);
        }

    private:
        const string str;
        const int line;
};


#endif
