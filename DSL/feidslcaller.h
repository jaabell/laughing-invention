// Created with create_feidslcaller.cpp .....

#ifndef FEIDSLCALLER
#define FEIDSLCALLER
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include "quantity.h"
#include "feiast.h"
#include "unitdefinitions.h"
#include "argcheck.h"

#include "feidslcaller_typedefs.h"
#include "feiexceptions.h"



bool argcheck(ArgumentType arguments, SignatureType signature);

std::pair<string, bool (*)(const string &, const Quantity &)> this_signature(string s, bool (*f)(const string & , const Quantity &))
{
    return make_pair(s, f);
}
// enable_if and disable_if are Boost C++ library magic to perform selective
// instantiation of functions on compile time.
//
// It is needed here because Expression* variables can be Numbers or FeiStrings
// which are needed for those DSLs which take strings as arguments.
// In order to detect FeiStrings a dynamic cast is used and then the corresponding
// member function is called to retrieve the string. Numbers are assigned in
// another different way.
//
// Since the code depends on template type (due to lowsy design of the Expression and
// and children) there is ambiguity that cannot be resolved by the compiler. Which
// function is called?
//
// This is why these functions are needed. They provide SFINAE (substitution failure is
// and error).


// If T is not a string
template<typename T>
typename boost::disable_if_c<boost::is_same<std::string, T>::value, T>::type getTheArgument(ArgumentIterator &it)
{
    return (*it)->value() .Getvalue();
}

// If T is a string
template<typename T>
typename boost::enable_if_c<boost::is_same<std::string, T>::value, T>::type getTheArgument(ArgumentIterator &it)
{
    FeiString *feistringpointer = dynamic_cast<FeiString *>(*it);
    if (feistringpointer != 0)
    {
        return feistringpointer -> Getstring();
    }
    else
    {
        // Code that leads to this part of the loop should not compile...
        cout << "FEIDSLCALLER.h : getTheArgument(it) : very odd condition." << endl;
        cout << "                                      Did you specify a string template parameter and passed something else?" << endl;
        cout << "                                      Are you missing a string?" << endl;
        cout << "                                      Who am I?" << endl;

        return std::string("PARAMETER FAILURE: Check how FeiDslCaller is being called.");
    }
}


// Base Class
class FeiDslCallerBase : public DslAction
{
protected:
    ArgumentType args;
    SignatureType signature;

public:
    static bool dry_run;
    static fstream *cppfile;
    FeiDslCallerBase(ArgumentType a, SignatureType s, string name) :
        DslAction(), args(a), signature(s)
    {
        setDslActionName(name);
    }

    virtual Quantity value ()
    {
        if (cppfile != NULL)
        {
            *cppfile << "   " << dslActionName << "(" ;
            ArgumentType::iterator it = args.begin();
            size_t i = 1;
            for (; it != args.end(); it++)
            {
                Expression *e = (*it);
                FeiString *ptr = dynamic_cast<FeiString *>(e);
                if ( ptr != 0) // Is a FEISTRING
                {
                    *cppfile << "\"" << ptr -> Getstring() << "\"";
                }
                else // Is not a FEISTRING
                {
                    *cppfile << e->value(). Getvalue();
                }
                if (i < args.size())
                {
                    *cppfile << ", ";
                }
                i++;
            }
            *cppfile << ");" << endl;
        }


        if (!dry_run)
        {
            execute();
        }
        return returnValue;
    }

    virtual void execute() = 0;
    virtual Expression *clone () = 0;
};

fstream *FeiDslCallerBase::cppfile = 0;
bool FeiDslCallerBase::dry_run = false;


// Derived Classes
template<typename ReturnType = int>
class FeiDslCaller0 : public FeiDslCallerBase
{
public:
    FeiDslCaller0(ReturnType (*f)(), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ReturnType retval = (*dslFunctionPointer)();
        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller0(*this);
    }
private:
    ReturnType (*dslFunctionPointer)();
};

template<typename T1, typename ReturnType = int>
class FeiDslCaller1 : public FeiDslCallerBase
{
public:
    FeiDslCaller1(ReturnType (*f)(T1), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1);
        }
        else
        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller1(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1);
};



template<typename T1,  typename T2, typename ReturnType = int>
class FeiDslCaller2 : public FeiDslCallerBase
{
public:
    FeiDslCaller2(ReturnType (*f)(T1, T2), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller2(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2);
};


template<typename T1,  typename T2, typename T3, typename ReturnType = int>
class FeiDslCaller3 : public FeiDslCallerBase
{
public:
    FeiDslCaller3(ReturnType (*f)(T1, T2, T3), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller3(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3);
};


template<typename T1,  typename T2, typename T3, typename T4, typename ReturnType = int>
class FeiDslCaller4 : public FeiDslCallerBase
{
public:
    FeiDslCaller4(ReturnType (*f)(T1, T2, T3, T4), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller4(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename ReturnType = int>
class FeiDslCaller5 : public FeiDslCallerBase
{
public:
    FeiDslCaller5(ReturnType (*f)(T1, T2, T3, T4, T5), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller5(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename ReturnType = int>
class FeiDslCaller6 : public FeiDslCallerBase
{
public:
    FeiDslCaller6(ReturnType (*f)(T1, T2, T3, T4, T5, T6), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller6(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename ReturnType = int>
class FeiDslCaller7 : public FeiDslCallerBase
{
public:
    FeiDslCaller7(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller7(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename ReturnType = int>
class FeiDslCaller8 : public FeiDslCallerBase
{
public:
    FeiDslCaller8(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller8(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename ReturnType = int>
class FeiDslCaller9 : public FeiDslCallerBase
{
public:
    FeiDslCaller9(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller9(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename ReturnType = int>
class FeiDslCaller10 : public FeiDslCallerBase
{
public:
    FeiDslCaller10(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller10(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename ReturnType = int>
class FeiDslCaller11 : public FeiDslCallerBase
{
public:
    FeiDslCaller11(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller11(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename ReturnType = int>
class FeiDslCaller12 : public FeiDslCallerBase
{
public:
    FeiDslCaller12(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller12(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename ReturnType = int>
class FeiDslCaller13 : public FeiDslCallerBase
{
public:
    FeiDslCaller13(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller13(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename ReturnType = int>
class FeiDslCaller14 : public FeiDslCallerBase
{
public:
    FeiDslCaller14(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller14(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename ReturnType = int>
class FeiDslCaller15 : public FeiDslCallerBase
{
public:
    FeiDslCaller15(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller15(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename ReturnType = int>
class FeiDslCaller16 : public FeiDslCallerBase
{
public:
    FeiDslCaller16(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller16(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename ReturnType = int>
class FeiDslCaller17 : public FeiDslCallerBase
{
public:
    FeiDslCaller17(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller17(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename ReturnType = int>
class FeiDslCaller18 : public FeiDslCallerBase
{
public:
    FeiDslCaller18(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller18(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename ReturnType = int>
class FeiDslCaller19 : public FeiDslCallerBase
{
public:
    FeiDslCaller19(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller19(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename ReturnType = int>
class FeiDslCaller20 : public FeiDslCallerBase
{
public:
    FeiDslCaller20(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller20(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename ReturnType = int>
class FeiDslCaller21 : public FeiDslCallerBase
{
public:
    FeiDslCaller21(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller21(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename ReturnType = int>
class FeiDslCaller22 : public FeiDslCallerBase
{
public:
    FeiDslCaller22(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller22(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename ReturnType = int>
class FeiDslCaller23 : public FeiDslCallerBase
{
public:
    FeiDslCaller23(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller23(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename ReturnType = int>
class FeiDslCaller24 : public FeiDslCallerBase
{
public:
    FeiDslCaller24(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller24(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename ReturnType = int>
class FeiDslCaller25 : public FeiDslCallerBase
{
public:
    FeiDslCaller25(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller25(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename ReturnType = int>
class FeiDslCaller26 : public FeiDslCallerBase
{
public:
    FeiDslCaller26(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller26(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename ReturnType = int>
class FeiDslCaller27 : public FeiDslCallerBase
{
public:
    FeiDslCaller27(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller27(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename ReturnType = int>
class FeiDslCaller28 : public FeiDslCallerBase
{
public:
    FeiDslCaller28(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller28(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename ReturnType = int>
class FeiDslCaller29 : public FeiDslCallerBase
{
public:
    FeiDslCaller29(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller29(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename ReturnType = int>
class FeiDslCaller30 : public FeiDslCallerBase
{
public:
    FeiDslCaller30(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller30(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename ReturnType = int>
class FeiDslCaller31 : public FeiDslCallerBase
{
public:
    FeiDslCaller31(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller31(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename ReturnType = int>
class FeiDslCaller32 : public FeiDslCallerBase
{
public:
    FeiDslCaller32(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller32(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename ReturnType = int>
class FeiDslCaller33 : public FeiDslCallerBase
{
public:
    FeiDslCaller33(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller33(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33);
};

template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename ReturnType = int>
class FeiDslCaller34 : public FeiDslCallerBase
{
public:
    FeiDslCaller34(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T33 arg34 = getTheArgument<T34>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller34(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34);
};

template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35,  typename ReturnType = int>
class FeiDslCaller35 : public FeiDslCallerBase
{
public:
    FeiDslCaller35(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T34 arg34 = getTheArgument<T34>(it);
        it++;
        T35 arg35 = getTheArgument<T35>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller35(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35);
};


template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,  typename ReturnType = int>
class FeiDslCaller36 : public FeiDslCallerBase
{
public:
    FeiDslCaller36(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T34 arg34 = getTheArgument<T34>(it);
        it++;
        T35 arg35 = getTheArgument<T35>(it);
        it++;
        T36 arg36 = getTheArgument<T36>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller36(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36);
};

template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename ReturnType = int>
class FeiDslCaller37 : public FeiDslCallerBase
{
public:
    FeiDslCaller37(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T34 arg34 = getTheArgument<T34>(it);
        it++;
        T35 arg35 = getTheArgument<T35>(it);
        it++;
        T36 arg36 = getTheArgument<T36>(it);
        it++;
        T37 arg37 = getTheArgument<T37>(it);
        it++;

        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36, arg37);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller37(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37);
};

template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38,  typename ReturnType = int>
class FeiDslCaller38 : public FeiDslCallerBase
{
public:
    FeiDslCaller38(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T34 arg34 = getTheArgument<T34>(it);
        it++;
        T35 arg35 = getTheArgument<T35>(it);
        it++;
        T36 arg36 = getTheArgument<T36>(it);
        it++;
        T37 arg37 = getTheArgument<T37>(it);
        it++;
        T38 arg38 = getTheArgument<T38>(it);
        it++;
        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller38(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38);
};

template<typename T1,  typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39,  typename ReturnType = int>
class FeiDslCaller39 : public FeiDslCallerBase
{
public:
    FeiDslCaller39(ReturnType (*f)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)
    {
        dslFunctionPointer = f;
    }
    virtual void execute()
    {
        ArgumentIterator it = args.begin();
        T1 arg1 = getTheArgument<T1>(it);
        it++;
        T2 arg2 = getTheArgument<T2>(it);
        it++;
        T3 arg3 = getTheArgument<T3>(it);
        it++;
        T4 arg4 = getTheArgument<T4>(it);
        it++;
        T5 arg5 = getTheArgument<T5>(it);
        it++;
        T6 arg6 = getTheArgument<T6>(it);
        it++;
        T7 arg7 = getTheArgument<T7>(it);
        it++;
        T8 arg8 = getTheArgument<T8>(it);
        it++;
        T9 arg9 = getTheArgument<T9>(it);
        it++;
        T10 arg10 = getTheArgument<T10>(it);
        it++;
        T11 arg11 = getTheArgument<T11>(it);
        it++;
        T12 arg12 = getTheArgument<T12>(it);
        it++;
        T13 arg13 = getTheArgument<T13>(it);
        it++;
        T14 arg14 = getTheArgument<T14>(it);
        it++;
        T15 arg15 = getTheArgument<T15>(it);
        it++;
        T16 arg16 = getTheArgument<T16>(it);
        it++;
        T17 arg17 = getTheArgument<T17>(it);
        it++;
        T18 arg18 = getTheArgument<T18>(it);
        it++;
        T19 arg19 = getTheArgument<T19>(it);
        it++;
        T20 arg20 = getTheArgument<T20>(it);
        it++;
        T21 arg21 = getTheArgument<T21>(it);
        it++;
        T22 arg22 = getTheArgument<T22>(it);
        it++;
        T23 arg23 = getTheArgument<T23>(it);
        it++;
        T24 arg24 = getTheArgument<T24>(it);
        it++;
        T25 arg25 = getTheArgument<T25>(it);
        it++;
        T26 arg26 = getTheArgument<T26>(it);
        it++;
        T27 arg27 = getTheArgument<T27>(it);
        it++;
        T28 arg28 = getTheArgument<T28>(it);
        it++;
        T29 arg29 = getTheArgument<T29>(it);
        it++;
        T30 arg30 = getTheArgument<T30>(it);
        it++;
        T31 arg31 = getTheArgument<T31>(it);
        it++;
        T32 arg32 = getTheArgument<T32>(it);
        it++;
        T33 arg33 = getTheArgument<T33>(it);
        it++;
        T34 arg34 = getTheArgument<T34>(it);
        it++;
        T35 arg35 = getTheArgument<T35>(it);
        it++;
        T36 arg36 = getTheArgument<T36>(it);
        it++;
        T37 arg37 = getTheArgument<T37>(it);
        it++;
        T38 arg38 = getTheArgument<T38>(it);
        it++;
        T39 arg39 = getTheArgument<T39>(it);
        it++;
        ReturnType retval;
        if (argcheck(args, signature) == 1)
        {
            retval = (*dslFunctionPointer)(arg1,  arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39);
        }
        else

        {
            retval = -1;
            throw RunTimeException(" Argument error.");
        }

        setReturnValue(Quantity(retval));
    }
    virtual Expression *clone ()
    {
        return new FeiDslCaller39(*this);
    }

private:
    ReturnType (*dslFunctionPointer)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39);
};























#endif //FEIDSLCALLER
