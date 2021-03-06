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


// Purpose: Meta-metaprogramming on template metaprogram. Create FEIDSLCALLER classes
// for different ammounts of inputs.
//
// This was before C++11 was mainstream. This can be re-implemented using variadic templates
// for greater simplicity in coding.
// This is hereby left as a task for those young future generations.


#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
    int nparams;
    fstream fid("feidslcaller_h.tmp", fstream::out);

    if (argc < 2)
    {
        cout << "Enter the maximum number of template parameters that"  << endl;
        cout << "should be generated:"  << endl;
        cout << endl << "Number of Template parameters = ";

        cin >> nparams;
    }
    else
    {
        nparams = atoi(argv[1]);  // I like the C version better because it makes things look harder. :-)
    }

    cout << "Generating \n""feidslcaller_h.tmp\n"" for " << nparams << " template parameters." << endl;
    cout << endl;

    fid << "// Created with create_feidslcaller.cpp .....\n"
        "\n"
        "#ifndef FEIDSLCALLER\n"
        "#define FEIDSLCALLER\n"
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <vector>\n"
        "#include <fstream>\n"
        "\n"
        "#include <boost/type_traits/is_same.hpp>\n"
        "#include <boost/utility/enable_if.hpp>\n"
        "\n"
        "#include \"quantity.h\"\n"
        "#include \"feiast.h\"\n"
        "#include \"unitdefinitions.h\"\n"
        "#include \"argcheck.h\"\n"
        "\n"
        "#include \"feidslcaller_typedefs.h\"\n"
        "#include \"feiexceptions.h\"\n"
        "\n"
        "\n"
        "\n"
        "bool argcheck(ArgumentType arguments, SignatureType signature);\n"
        "\n"
        "std::pair<string, bool (*)(const Quantity &)> this_signature(string s, bool (*f)(const Quantity &))\n"
        "{\n"
        "    return make_pair(s, f);\n"
        "}\n"
        "// enable_if and disable_if are Boost C++ library magic to perform selective\n"
        "// instantiation of functions on compile time.\n"
        "//\n"
        "// It is needed here because Expression* variables can be Numbers or FeiStrings\n"
        "// which are needed for those DSLs which take strings as arguments.\n"
        "// In order to detect FeiStrings a dynamic cast is used and then the corresponding\n"
        "// member function is called to retrieve the string. Numbers are assigned in\n"
        "// another different way.\n"
        "//\n"
        "// Since the code depends on template type (due to lowsy design of the Expression and\n"
        "// and children) there is ambiguity that cannot be resolved by the compiler. Which\n"
        "// function is called?\n"
        "//\n"
        "// This is why these functions are needed. They provide SFINAE (substitution failure is\n"
        "// and error).\n"
        "\n"
        "\n"
        "// If T is not a string\n"
        "template<typename T>\n"
        "typename boost::disable_if_c<boost::is_same<std::string, T>::value, T>::type getTheArgument(ArgumentIterator &it)\n"
        "{\n"
        "    return (*it)->value() .Getvalue();\n"
        "}\n"
        "\n"
        "// If T is a string\n"
        "template<typename T>\n"
        "typename boost::enable_if_c<boost::is_same<std::string, T>::value, T>::type getTheArgument(ArgumentIterator &it)\n"
        "{\n"
        "    FeiString *feistringpointer = dynamic_cast<FeiString *>(*it);\n"
        "    if (feistringpointer != 0)\n"
        "    {\n"
        "        return feistringpointer -> Getstring();\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        // Code that leads to this part of the loop should not compile...\n"
        "        cout << \"FEIDSLCALLER.h : getTheArgument(it) : very odd condition.\" << endl;\n"
        "        cout << \"                                      Did you specify a string template parameter and passed something else?\" << endl;\n"
        "        cout << \"                                      Are you missing a string?\" << endl;\n"
        "        cout << \"                                      Who am I?\" << endl;\n"
        "\n"
        "        return std::string(\"PARAMETER FAILURE: Check how FeiDslCaller is being called.\");\n"
        "    }\n"
        "}\n"
        "\n"
        "\n"
        "// Base Class\n"
        "class FeiDslCallerBase : public DslAction\n"
        "{\n"
        "    protected:\n"
        "    ArgumentType args;\n"
        "    SignatureType signature;\n"
        "\n"
        "    public:\n"
        "    static bool dry_run;\n"
        "    static fstream *cppfile;\n"
        "    FeiDslCallerBase(ArgumentType a, SignatureType s, string name) :\n"
        "    DslAction(), args(a), signature(s)\n"
        "    {\n"
        "        setDslActionName(name);\n"
        "    }\n"
        "\n"
        "    virtual Quantity value ()\n"
        "    {\n"
        "        if (cppfile != NULL)\n"
        "        {\n"
        "          *cppfile << \"   \" << dslActionName << \"(\" ;\n"
        "          ArgumentType::iterator it = args.begin();\n"
        "          size_t i = 1;\n"
        "          for (; it != args.end(); it++)\n"
        "          {\n"
        "              Expression *e = (*it);\n"
        "              FeiString *ptr = dynamic_cast<FeiString *>(e);\n"
        "              if ( ptr != 0) // Is a FEISTRING\n"
        "              {\n"
        "                  *cppfile << \"\\\"\" << ptr -> Getstring() << \"\\\"\";\n"
        "              }\n"
        "              else // Is not a FEISTRING\n"
        "              {\n"
        "                  *cppfile << e->value(). Getvalue();\n"
        "              }\n"
        "              if (i < args.size())\n"
        "              {\n"
        "                  *cppfile << \", \";\n"
        "              }\n"
        "              i++;\n"
        "          }\n"
        "          *cppfile << \");\" << endl;\n"
        "        }\n"
        "\n"
        "\n"
        "          if (!dry_run)\n"
        "          {\n"
        "               execute();\n"
        "          }\n"
        "          return returnValue;\n"
        "    }\n"
        "\n"
        "        virtual void execute() = 0;\n"
        "        virtual Expression *clone () = 0;\n"
        "};\n"
        "\n"
        "fstream *FeiDslCallerBase::cppfile = 0;\n"
        "bool FeiDslCallerBase::dry_run = false;\n"
        "\n"
        "\n"
        "// Derived Classes\n"
        "template<typename ReturnType = int>\n"
        "class FeiDslCaller0 : public FeiDslCallerBase\n"
        "{\n"
        "    public:\n"
        "        FeiDslCaller0(ReturnType (*f)(), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)\n"
        "        {\n"
        "            dslFunctionPointer = f;\n"
        "        }\n"
        "        virtual void execute()\n"
        "        {\n"
        "            ReturnType retval = (*dslFunctionPointer)();\n"
        "            setReturnValue(Quantity(retval));\n"
        "        }\n"
        "        virtual Expression *clone ()\n"
        "        {\n"
        "            return new FeiDslCaller0(*this);\n"
        "        }\n"
        "    private:\n"
        "        ReturnType (*dslFunctionPointer)();\n"
        "};\n"
        "\n"
        "template<typename T1, typename ReturnType = int>\n"
        "class FeiDslCaller1 : public FeiDslCallerBase\n"
        "{\n"
        "    public:\n"
        "        FeiDslCaller1(ReturnType (*f)(T1), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)\n"
        "        {\n"
        "            dslFunctionPointer = f;\n"
        "        }\n"
        "        virtual void execute()\n"
        "        {\n"
        "            ArgumentIterator it = args.begin();\n"
        "            T1 arg1 = getTheArgument<T1>(it);\n"
        "\n"
        "            ReturnType retval;\n"
        "            if (argcheck(args, signature) == 1)\n"
        "            {\n"
        "                retval = (*dslFunctionPointer)(arg1);\n"
        "            }\n"
        "            else\n"
        "            {\n"
        "                retval = -1;\n"
        "                throw RunTimeException(\" Argument error.\");\n"
        "            }\n"
        "\n"
        "            setReturnValue(Quantity(retval));\n"
        "        }\n"
        "        virtual Expression *clone ()\n"
        "            {\n"
        "               return new FeiDslCaller1(*this);\n"
        "            }\n"
        "\n"
        "    private:\n"
        "        ReturnType (*dslFunctionPointer)(T1);\n"
        "};\n\n";

    int i;
    int j;

    for (i = 2; i <= nparams; i++)
    {
        fid << endl << endl;
        fid << "template<typename T1, ";

        for (j = 2; j <= i; j++)
        {
            fid << " typename T" << j;

            if (j < i)
            {
                fid << ",";
            }
        }

        fid << ", typename ReturnType = int>" << endl;

        fid << "class FeiDslCaller" << i << " : public FeiDslCallerBase" << endl;
        fid << "{" << endl;
        fid << "    public:" << endl;
        fid << "        FeiDslCaller" << i << "(ReturnType (*f)(T1,";

        for (j = 2; j <= i; j++)
        {
            fid << " T" << j;

            if (j < i)
            {
                fid << ",";
            }
        }

        fid << "), ArgumentType a, SignatureType s, string name) : FeiDslCallerBase(a, s, name)" << endl;
        fid << "        {" << endl;
        fid << "            dslFunctionPointer = f;" << endl;
        fid << "        }" << endl;
        fid << "        virtual void execute()" << endl;
        fid << "        {" << endl;
        fid << "            ArgumentIterator it = args.begin();" << endl;
        fid << "            T1 arg1 = getTheArgument<T1>(it);\n it++;" << endl;

        for (j = 2; j <= i; j++)
        {
            fid << "            T" << j << " arg" << j << " = getTheArgument<T" << j << ">(it);\n it++;" << endl;
        }


        fid << "" << endl;
        //fid << "            cout << \n""  --> \n"" << getDslActionName() << \n"" received parameters: \n"";" << endl;
        fid << "            ReturnType retval;" << endl;
        fid << "            if (argcheck(args, signature) == 1)" << endl;
        fid << "            {" << endl;
        fid << "                retval = (*dslFunctionPointer)(arg1, ";

        for (j = 2; j <= i; j++)
        {
            fid << " arg" << j;

            if (j < i)
            {
                fid << ",";
            }
        }

        fid << ");" << endl;
        fid << "            }" << endl;
        fid << "            else" << endl;
        fid << "            " << endl;
        fid << "            {" << endl;
        fid << "                retval = -1;" << endl;
        fid << "                throw RunTimeException(\" Argument error.\");\n";
        fid << "            }" << endl;
        fid << "" << endl;
        fid << "            setReturnValue(Quantity(retval));" << endl;
        fid << "        }" << endl;
        fid << "        virtual Expression *clone ()\n";
        fid << "        {\n";
        fid << "            return new FeiDslCaller" << i << "(*this);\n";
        fid << "        }\n";
        fid << "" << endl;
        fid << "    private:" << endl;
        fid << "        ReturnType (*dslFunctionPointer)(T1,";

        for (j = 2; j <= i; j++)
        {
            fid << " T" << j;

            if (j < i)
            {
                fid << ",";
            }
        }

        fid << ");" << endl;
        fid << "};" << endl;
    }

    fid << endl << "#endif //FEIDSLCALLER" << endl;

    cout << "Done! :)" << endl;
    fid.close();
    return 0;
}
