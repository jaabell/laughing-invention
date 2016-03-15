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




#ifndef FEIAST_H
#define FEIAST_H

// FEIAST.h
//
// This is the definition of the Abstract Syntax Tree classes used
// by the FEI parser.
// Expressions are syntax trees with different nodes. Nodes are classes
// which implement the node functionality. They inherit directly or
// indirectly from the abstract base class 'Expression'. This enables
// using polymorphism to build a tree of different expressions without
// knowing the exact type of each node.
//



#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <map>
#include <set>

#include "siunit.h"
#include "quantity.h"
#include "unitdefinitions.h"
#include "feiexceptions.h"


class Expression
{
public:
    virtual ~Expression () {}
    virtual Expression* clone () = 0;
    virtual Quantity value () = 0;
    virtual Quantity value (int i )
    {
        return this->value();
    }

    //For vector expressions
    virtual void select_component(int i) {}
    virtual int size()
    {
        return 1;
    }
    virtual Quantity sum()
    {
        return this->value();
    }
};

// Numbers are the most basic of expressions. They just hold a Quantity.
class Number : public Expression
{
    Quantity qty;

public:

    Number (double val): qty(val, ESSIunits::unitless) {}
    Number (double val, SIUnit u): qty(val, u) {}
    Number (Quantity q): qty(q) {}

    // Copy constructor
    Number (const Number& other)
    {
        qty = other.qty;
    }

    Number& operator = (const Number& other)
    {
        if (&other != this)
        {
            qty = other.qty;
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new Number (*this);
    }

    virtual Quantity value ()
    {
        return qty;
    }
};


// Numbers are the most basic of expressions. They just hold a Quantity.
class FeiString : public Expression
{
    std::string str;

public:

    FeiString (std::string s): str(s) {}

    // Copy constructor
    FeiString (const FeiString& other)
    {
        str = other.str;
    }

    FeiString& operator = (const FeiString& other)
    {
        if (&other != this)
        {
            str = other.str;
        }

        return *this;
    }

    std::string Getstring() const
    {
        return str;
    }

    virtual Expression* clone ()
    {
        return new FeiString (*this);
    }

    virtual Quantity value ()
    {
        return Quantity(0, ESSIunits::unitless);
    }
};

//#ifdef ISFEISTRING
//bool isFeiString(const Expression* exp);
//#endif
//
//#ifndef ISFEISTRING
//#define ISFEISTRING
//bool isFeiString(const Expression* exp)
//{
//    bool theTruth = true;
//    const FeiString* ptr= dynamic_cast<const FeiString*> (exp);
//    if( ptr == 0)
//        theTruth = false;
//    return theTruth;
//}
//#endif // ISFEISTRING


// Idea: define a complexnumber which inherits from number to implement
// complex number handling. Since arithmetic operators are defined
// in the standard complex class, the nodes below would work with complex
// numbers or any other class which implements them (strings, but special
// arrays).


// Empty statement
class Empty : public Expression
{
public:
    Empty () {}
    Empty (const Empty& other) { }
    Empty& operator = (const Empty& other)
    {
        return *this;
    }
    virtual Expression* clone ()
    {
        return new Empty (*this);
    }
    virtual Quantity value ()
    {
        Quantity qempty(0., ESSIunits::unitless);
        return qempty;
    }
};


// ========================================================================
// Arithmetic operations
// ========================================================================


// For addictive expressions
class Plus : public Expression
{
    Expression* m_left, *m_right;

public:

    Plus (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Plus (const Plus& other)
    {
        m_left  = other.m_left  -> clone ();
        m_right = other.m_right -> clone ();
    }

    virtual ~Plus ()
    {
        delete m_left;
        delete m_right;
    }

    Plus& operator = (const Plus& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left    = other.m_left  -> clone ();
            m_right   = other.m_right -> clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new Plus (*this);
    }

    virtual Quantity value ()
    {
        return m_left->value () + m_right->value ();
    }

};

// For subtractive expressions
class Minus : public Expression
{
    Expression* m_left, *m_right;

public:

    Minus (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Minus (const Minus& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Minus ()
    {
        delete m_left;
        delete m_right;
    }

    Minus& operator = (const Minus& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new Minus (*this);
    }

    virtual Quantity value ()
    {
        return m_left->value () - m_right->value ();
    }

};

// For multiplicative expressions
class Times : public Expression
{
    Expression* m_left, *m_right;

public:

    Times (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Times (const Times& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Times ()
    {
        delete m_left;
        delete m_right;
    }

    Times& operator = (const Times& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new Times (*this);
    }

    virtual Quantity value ()
    {
        return m_left->value () * m_right->value ();
    }

};


// For division expressions
class Divide : public Expression
{
    Expression* m_left, *m_right;

public:

    Divide (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Divide (const Divide& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Divide ()
    {
        delete m_left;
        delete m_right;
    }

    Divide& operator = (const Divide& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new Divide (*this);
    }

    virtual Quantity value ()
    {
        return m_left->value () / m_right->value ();    // Division by zero is handled by '/' operator between Quantities.
    }

};


// For exponentiation expressions
class Pow : public Expression
{
    Expression* m_left, *m_right;

public:

    Pow (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Pow (const Pow& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Pow ()
    {
        delete m_left;
        delete m_right;
    }

    Pow& operator = (const Pow& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new Pow (*this);
    }

    virtual Quantity value ()
    {
        return pow(m_left->value (), m_right->value ());
    }

};


// For exponentiation expressions
class Modulo : public Expression
{
    Expression* m_left, *m_right;

public:

    Modulo (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    Modulo (const Modulo& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Modulo ()
    {
        delete m_left;
        delete m_right;
    }

    Modulo& operator = (const Modulo& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new Modulo (*this);
    }

    virtual Quantity value ()
    {
        Quantity quotient = m_left->value()  / m_right->value();
        int iquotient = static_cast<int>( quotient.Getvalue() );
        double mod = (m_left->value()).Getvalue()  - static_cast<double>(iquotient) * (m_right->value()).Getvalue() ;
        Quantity result(mod, quotient.Getunit());
        return result;
    }

};



// ========================================================================
// Expressions, identifiers and names....
// ========================================================================



// For names
class VariableReference : public Expression
{
    std::string name;
    const map<std::string, Quantity>& vars;

public:

    VariableReference (std::string n, const map<std::string, Quantity>&  v):
        name (n),
        vars(v)
    {
        // empty
    }

    // Copy constructor
    VariableReference (const VariableReference& other):
        name(other.name),
        vars(other.vars)
    {
        // empty
    }

    VariableReference& operator = (const VariableReference& other)
    {
        if (&other != this)
        {
            name = other.name;
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new VariableReference (*this);
    }

    virtual Quantity value()
    {
        map<std::string, Quantity>::const_iterator iter;
        iter = vars.find(name);

        if (iter != vars.end())
        {
            return iter->second;
        }
        else
        {
            throw  RunTimeException("Variable or function name \'" + name +  "\' not found!");

        }
    }
    std::string getName()
    {
        return name;
    };
};


// For list of expressions
class ExpressionList : public Expression
{
    Expression* m_left, *m_right;

public:

    ExpressionList (): m_left (0), m_right (0) {}
    ExpressionList (Expression* left, Expression* right): m_left (left), m_right (right) {}

    // Copy constructor
    ExpressionList (const ExpressionList& other)
    {
        m_left = other.m_left -> clone();
        m_right = other.m_right->clone ();
    }

    virtual ~ExpressionList ()
    {
        delete m_left;
        delete m_right;
    }

    ExpressionList& operator = (const ExpressionList& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new ExpressionList (*this);
    }

    virtual Quantity value ()
    {
        m_left -> value();
        return m_right->value ();
    }

};


// ========================================================================
// For comparison nodes
// ========================================================================

class Comparison : public Expression
{
protected:
    Expression* m_left, *m_right;

public:

    Comparison();
    Comparison (Expression* left, Expression* right): m_left (left), m_right (right)
    {
        // empty
    }

    // Copy constructor
    Comparison (const Comparison& other)
    {
        m_left = other.m_left->clone ();
        m_right = other.m_right->clone ();
    }

    virtual ~Comparison ()
    {
        delete m_left;
        delete m_right;
    }

    Comparison& operator = (const Comparison& other)
    {
        if (&other != this)
        {
            delete m_left;
            delete m_right;

            m_left = other.m_left->clone ();
            m_right = other.m_right->clone ();
        }

        return *this;
    }

    virtual bool truth() = 0;
    virtual Expression* clone () = 0;

    virtual Quantity value ()
    {
        return Quantity(static_cast<double>(truth()), ESSIunits::unitless);
    }

};

//                        a  >  b
class GreaterThan : public Comparison
{
public:
    GreaterThan(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() > m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new GreaterThan (*this);
    }
};

//                        a  <  b
class LessThan : public Comparison
{
public:
    LessThan(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() < m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new LessThan (*this);
    }
};

//                        a  !=  b
class NotEqualTo : public Comparison
{
public:
    NotEqualTo(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() != m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new NotEqualTo (*this);
    }
};

//                        a  ==  b
class EqualTo : public Comparison
{
public:
    EqualTo(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() == m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new EqualTo (*this);
    }
};

//                        a  >=  b
class GreaterThanOrEqualTo : public Comparison
{
public:
    GreaterThanOrEqualTo(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() >= m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new GreaterThanOrEqualTo (*this);
    }
};

//                        a  <=  b
class LessThanOrEqualTo : public Comparison
{
public:
    LessThanOrEqualTo(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() <= m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new LessThanOrEqualTo (*this);
    }
};

//                        a  &  b
class LogicalAnd : public Comparison
{
public:
    LogicalAnd(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() & m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new LogicalAnd (*this);
    }
};

//                        a  |  b
class LogicalOr : public Comparison
{
public:
    LogicalOr(Expression* left, Expression* right): Comparison(left, right)
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth = m_left->value() | m_right->value();
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new LogicalOr (*this);
    }
};

//                        ! a
class LogicalNot : public Comparison
{
public:
    LogicalNot(Expression* left): Comparison(left, left)  // I know... I'm cheap... :P
    {
        //empty
    }
    virtual bool truth()
    {
        bool theTruth =  ~m_left->value() ;
        //cout << "The truth is " << theTruth <<  endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new LogicalNot (*this);
    }
};

//
class UnitTypeTest : public Comparison
{
    /*
    "isAdimensional"
    "isMass"
    "isLength"
    "isTime"
    "isArea"
    "isVolume"
    "isForce"
    "isEnergy"
    "isTorque"
    "isPressure"
    "isBodyForce"
    "isDensity"
    */
public:
    UnitTypeTest(std::string t, Expression* e): Comparison(new Empty(), e), test(t)
    {
    }
    virtual bool truth()
    {
        bool theTruth = false;

        if (test.compare("isAdimensional") == 0)
        {
            theTruth = isAdimensional(m_right->value());
        }

        if (test.compare("isMass") == 0)
        {
            theTruth = isMass(m_right->value());
        }

        if (test.compare("isLength") == 0)
        {
            theTruth = isLength(m_right->value());
        }

        if (test.compare("isTime") == 0)
        {
            theTruth = isTime(m_right->value());
        }

        if (test.compare("isArea") == 0)
        {
            theTruth = isArea(m_right->value());
        }

        if (test.compare("isVolume") == 0)
        {
            theTruth = isVolume(m_right->value());
        }

        if (test.compare("isForce") == 0)
        {
            theTruth = isForce(m_right->value());
        }

        if (test.compare("isEnergy") == 0)
        {
            theTruth = isEnergy(m_right->value());
        }

        if (test.compare("isTorque") == 0)
        {
            theTruth = isTorque(m_right->value());
        }

        if (test.compare("isPressure") == 0)
        {
            theTruth = isPressure(m_right->value());
        }

        if (test.compare("isBodyForce") == 0)
        {
            theTruth = isBodyForce(m_right->value());
        }

        if (test.compare("isDensity") == 0)
        {
            theTruth = isDensity(m_right->value());
        }

        cout << test << " " <<  theTruth << endl;
        return theTruth;
    }
    virtual Expression* clone ()
    {
        return new UnitTypeTest (*this);
    }

private:
    std::string test;
};



// ========================================================================
// Assign node ('=' node)
// ========================================================================

class Assign : public Expression
{
    std::string varname;
    Expression* rhs;
    map<std::string, Quantity>& vars;
    set<std::string> lockedvars;

public:

    Assign (std::string vname, Expression* right, map<std::string, Quantity>& v, set<std::string> l):
        varname (vname),
        rhs (right),
        vars(v),
        lockedvars(l)
    {
        // empty
    }

    // Copy constructor
    Assign (const Assign& other) :
        vars(other.vars),
        lockedvars(other.lockedvars)
    {
        varname = other.varname;
        rhs = other.rhs -> clone();
    }

    virtual ~Assign ()
    {
        delete rhs;
    }

    Assign& operator = (const Assign& other)
    {
        if (&other != this)
        {
            delete rhs;
            rhs = other.rhs -> clone();
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new Assign (*this);
    }

    virtual Quantity value ()
    {
        if (lockedvars.count(varname) != 0)
        {
            cout << " Cannot assign to " << varname << " variable is locked." << endl;
            return vars[varname];
        }
        else
        {
            return (vars[varname] = rhs->value());
        }
    }
};



// ========================================================================
// Require node ('=' node)
// ========================================================================

class Require : public Expression
{
    std::string varname;
    Expression* rhs;
    map<std::string, Quantity>& vars;
    set<std::string> lockedvars;

public:

    Require (std::string vname, Expression* right, map<std::string, Quantity>& v, set<std::string> l):
        varname (vname),
        rhs (right),
        vars(v),
        lockedvars(l)
    {
        // empty
    }

    // Copy constructor
    Require (const Require& other) :
        vars(other.vars),
        lockedvars(other.lockedvars)
    {
        varname = other.varname;
        rhs = other.rhs -> clone();
    }

    virtual ~Require ()
    {
        delete rhs;
    }

    Require& operator = (const Require& other)
    {
        if (&other != this)
        {
            delete rhs;
            rhs = other.rhs -> clone();
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new Require (*this);
    }

    virtual Quantity value ()
    {
        if (lockedvars.count(varname) != 0)
        {
            cout << " Cannot assign to " << varname << " variable is locked." << endl;
            return vars[varname];
        }
        else
        {
            return (vars[varname] = rhs->value());
        }
    }
};




// ========================================================================
// Unit conversion node ('in' node)
// ========================================================================

class ConvertUnits : public Expression
{
    std::string var1;
    std::string var2;
    map<std::string, Quantity>& vars;

public:

    ConvertUnits (std::string v1, std::string v2, map<std::string, Quantity>& v):
        var1 (v1),
        var2 (v2),
        vars(v)
    {
        // empty
    }

    virtual Expression* clone ()
    {
        return new ConvertUnits (*this);
    }

    virtual Quantity value ()
    {
        Quantity q1 = vars[var1];
        Quantity q2 = vars[var2];

        Quantity newqty;
        newqty = q1.convert(q2.Getunit());
        return newqty;
    }
};


// ========================================================================
// Flow control
// ========================================================================


//   IF


class IfStatement : public Expression
{
    Expression* mainExp, *elseExp;
    Comparison* condExp;

public:

    IfStatement (Comparison* cond, Expression* main, Expression* else_):
        mainExp (main),
        elseExp (else_),
        condExp (cond)
    {
        /*empty */
    }

    // Copy constructor
    IfStatement (const IfStatement& other)
    {
        condExp = dynamic_cast<Comparison*> (other.condExp -> clone());
        mainExp = other.mainExp -> clone();
        elseExp = other.elseExp -> clone();
    }

    virtual ~IfStatement ()
    {
        delete condExp;
        delete mainExp;
        delete elseExp;
    }

    IfStatement& operator = (const IfStatement& other)
    {
        if (&other != this)
        {
            delete condExp;
            delete mainExp;
            delete elseExp;

            condExp  = dynamic_cast<Comparison*> (other.condExp ->  clone());
            mainExp  = other.mainExp ->  clone();
            elseExp  = other.elseExp ->  clone();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new IfStatement (*this);
    }

    virtual Quantity value ()
    {
        Quantity val;

        if (condExp->truth())
        {
            if (mainExp) /*protect from dereferencing a NULL pointer*/
            {
                val = mainExp -> value();
            }
        }
        else
        {
            if (elseExp) /*protect from dereferencing a NULL pointer*/
            {
                val = elseExp -> value();
            }
        }

        return val;
    }
};


// WHILE


class WhileStatement : public Expression
{
    Expression* mainExp;
    Comparison* condExp;

public:

    WhileStatement (Comparison* cond, Expression* main):
        mainExp (main),
        condExp (cond)
    {
        /*empty */
    }

    // Copy constructor
    WhileStatement (const WhileStatement& other)
    {
        condExp = dynamic_cast<Comparison*> (other.condExp -> clone());
        mainExp = other.mainExp -> clone();
    }

    virtual ~WhileStatement ()
    {
        delete condExp;
        delete mainExp;
    }

    WhileStatement& operator = (const WhileStatement& other)
    {
        if (&other != this)
        {
            delete condExp;
            delete mainExp;

            condExp  = dynamic_cast<Comparison*> (other.condExp ->  clone());
            mainExp  = other.mainExp ->  clone();
        }

        return *this;
    }


    virtual Expression* clone ()
    {
        return new WhileStatement (*this);
    }

    virtual Quantity value ()
    {
        Quantity val;

        while (condExp->truth())
        {
            if (mainExp) /*protect from dereferencing a NULL pointer*/
            {
                val = mainExp -> value();
            }
        }

        return val;
    }
};

// ========================================================================
// DSL Actions other than variable manipulation.
// ========================================================================

class DslAction : public Expression
{
public:
    DslAction ()
    {
        returnValue = Quantity(0.0, ESSIunits::unitless);
        setDslActionName("DslAction");
    }
    ~DslAction() {}
    virtual void execute() = 0;
    virtual Quantity value ()
    {
        execute();
        return returnValue;
    }
    void setReturnValue(Quantity rv)
    {
        returnValue = rv;
    }
    std::string getDslActionName()
    {
        return dslActionName;
    }
    void setDslActionName(std::string name)
    {
        dslActionName = name;
    }
    virtual Expression* clone () = 0;
protected:
    Quantity returnValue;
    std::string dslActionName;
};


#endif // FEIAST_H
