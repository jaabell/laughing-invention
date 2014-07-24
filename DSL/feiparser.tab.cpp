// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.
#line 11 "feiparser.yy" // lalr1.cc:399

    // Basic
    #include <iostream>
    #include <iomanip>

    // STL
    #include <vector>
    #include <set>
    #include <string>
    #include <stack>
    #include <map>
    #include <utility>

    // C library
    #include <cctype>
    #include <cstring>
    #include <cstdio>

    // Boost
    #include <boost/lexical_cast.hpp>

    // Own
    #include "feiast.h"
    #include "dsl_actions.h"
    #include "siunit.h"
    #include "quantity.h"
    #include "unitdefinitions.h"
    #include "feidslcaller.h"
    #include "feimath.h"

/*     #include "command_suggestion.h" */

    // CompGeoMech API
    typedef std::vector<Number*> VectorNumber;          // This hack gets rid of a collision between std::vector and Vector classes used within FEI
    #include "../API/CPPIncludes.h"

    using namespace std;

    // Prototypes to keep the compiler happy (these are defined on lexer source feiparser.l feiparser.lex.cpp)
    void yyerror (const char *error);
    void clear_stack ();

    //These are auxilliary functions to help in parsing
    Number* dof2number(string dof);
    UnitCheckerFunctionPointerType dof2signature(string dof);
    Number* force2number(string dof);
    UnitCheckerFunctionPointerType force2signature(string dof);
    UnitCheckerFunctionPointerType dof2stiffnesschecker(string dof);

    // Variables
    std::map<string,Quantity> global_variables;
    std::set<string> locked_global_variables;

    //To vectorize commands which apply to several dofs.
    VectorNumber dofchain_holder;
    VectorNumber::const_iterator dofchain_holder_it;

    // Arguments to pass to functions
    ArgumentType args;              //ArgumentType defined in feidslcaller_typedefs.h (std::vector<Expression*>)
    SignatureType signature;        //signatureType defined in feidslcaller_typedefs.h (std::vector<SignatureElementType>)

    // stack class that takes care of all the AST nodes that were allocated (nothing to do with FEM nodes)
    stack <Expression *> nodes;

    // Some useful quantities,
    Quantity q0(0.0, unitless);

    extern int yylineno;
    extern char* curfilename;
    extern const string thePrompt = "ESSI> ";
    extern int newfile(char* fn);
    extern int newstdin(void);
    extern int popfile(void);
    extern int error_behavior(void);

    void set_outcppfile_name(string newfilename);

    //Command line options flags
    extern int FLAG_interactive_mode;
    extern int FLAG_generate_cpp_output;
    extern int FLAG_dry_run;
    extern int FLAG_execute_apis;
    extern int FLAG_inclusion_mode;
    extern int FLAG_binary_output;
    extern int FLAG_exodusii_output;

    string out_cpp_filename("");
    string prompt = thePrompt;

    // Some parse mode flags
    int recovery_mode = 0;

    // Command advisor helps with useful suggestions (hopefully) when parse errors are encountered.
    //    CommandSuggestion cmd_advisor;


#line 133 "feiparser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "feiparser.tab.hh"

// User implementation prologue.
#line 240 "feiparser.yy" // lalr1.cc:407

extern int yylex(yy::feiparser::semantic_type *yylval,
                 yy::feiparser::location_type *yylloc);

#line 151 "feiparser.tab.cc" // lalr1.cc:407


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 237 "feiparser.tab.cc" // lalr1.cc:474

  /// Build a parser object.
  feiparser::feiparser ()
#if YYDEBUG
     :yydebug_ (false),
      yycdebug_ (&std::cerr)
#endif
  {}

  feiparser::~feiparser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  feiparser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  feiparser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  feiparser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  feiparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  feiparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  feiparser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  feiparser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  feiparser::by_type::by_type ()
     : type (empty)
  {}

  inline
  feiparser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  feiparser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  feiparser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  feiparser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  feiparser::by_state::by_state ()
    : state (empty)
  {}

  inline
  feiparser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  feiparser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  feiparser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  feiparser::symbol_number_type
  feiparser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  feiparser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  feiparser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  feiparser::stack_symbol_type&
  feiparser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  feiparser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  feiparser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  feiparser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  feiparser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  feiparser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  feiparser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  feiparser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  feiparser::debug_level_type
  feiparser::debug_level () const
  {
    return yydebug_;
  }

  void
  feiparser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline feiparser::state_type
  feiparser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  feiparser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  feiparser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  feiparser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 245 "feiparser.yy" // lalr1.cc:725
{
    yyla.location.begin.filename = yyla.location.end.filename = new std::string(curfilename);
}

#line 540 "feiparser.tab.cc" // lalr1.cc:725

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 266 "feiparser.yy" // lalr1.cc:847
    {
        if ( (yystack_[0].value.exp) != 0 ) // ... and deliver us from the null pointer
        {
            (yylhs.value.exp) = new DslPrint((yystack_[0].value.exp));
        } else {
            (yylhs.value.exp) = new Empty();
        }
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 663 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 280 "feiparser.yy" // lalr1.cc:847
    {
        //theDomain.Print(cerr);
        cerr << "Not currently working. Should be implemented as API function and used here." << endl;
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 674 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 290 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "output_of_element_to_screen");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 688 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 313 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_node_to_screen, args, signature, "output_of_node_to_screen");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 702 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 326 "feiparser.yy" // lalr1.cc:847
    {
        cerr << "Not currently working. Should be implemented a API function and used here." << endl;
        //theDomain.getNDMaterial( $4->value().Getvalue() ) -> Print(cerr,0);
        (yylhs.value.exp) = new Empty();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 714 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 337 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("filename", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<string>(&check_mesh, args, signature,"check_mesh");

        nodes.pop();
        nodes.push((yylhs.value.exp));
        }
#line 728 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 425 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
        nodes.push((yylhs.value.exp));
    }
#line 737 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 433 "feiparser.yy" // lalr1.cc:847
    {
        cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 747 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 438 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 753 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 439 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 759 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 440 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 765 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 441 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 771 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 442 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 777 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 446 "feiparser.yy" // lalr1.cc:847
    {
                                    cout << "    *** Commands will now be execued. " << endl << endl;
                                    recovery_mode = 0;
                                    prompt = thePrompt;
                                    (yylhs.value.exp) = new Empty();
                                    nodes.push((yylhs.value.exp));
                                }
#line 789 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 456 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 795 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 461 "feiparser.yy" // lalr1.cc:847
    {

        //Always clear the args and signature vectors ... errors ensue if not... like segfaults
        args.clear();
        signature.clear();

        // Add parsed 'exp's to the argument list. Make sure these are of type Expression*.
        // The only reason to not use exp type tokens is to parse strings in which case
        // one matches a STRING token and creates an Expression* by using FeiString (which
        // inherits from Expression) to hold the string.
        args.push_back((yystack_[11].value.exp));     //Number
        args.push_back((yystack_[1].value.exp));    //ndofs
        args.push_back((yystack_[8].value.exp));     //x
        args.push_back((yystack_[6].value.exp));     //y
        args.push_back((yystack_[4].value.exp));    //z

        // Create the command signature for input verification
        signature.push_back(this_signature("number", &isAdimensional));
        signature.push_back(this_signature("dofs", &isAdimensional));
        signature.push_back(this_signature("x", &isLength));
        signature.push_back(this_signature("y", &isLength));
        signature.push_back(this_signature("z", &isLength));

        // Create the DSL caller node.
        (yylhs.value.exp) = new FeiDslCaller5<int, int, double, double, double>(&add_node, args, signature, "add_node");

        // Remove the parsed exps from the nodes (free up the memory)
        // There must be as many 'pop's as there are 'exp's.
        nodes.pop();
        nodes.pop();
        nodes.pop();
        nodes.pop();
        nodes.pop();

        // Then add the new caller to the nodes!
        nodes.push((yylhs.value.exp));
    }
#line 837 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 502 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();
        signature.clear();

        args.push_back((yystack_[3].value.exp));   //File name
        args.push_back((yystack_[1].value.exp));   //Numbdofs

        // Create the command signature for input verification
        signature.push_back(this_signature("filename", &isAdimensional));

        // Create the DSL caller node.
        //add_nodes_from_file(std::string inputnodesfile, int ndof)
        (yylhs.value.exp) = new FeiDslCaller2<string, int>(&add_nodes_from_file, args, signature, "add_nodes_from_file");
        nodes.pop();
        nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 860 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 520 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 866 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 525 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("ax", &isAcceleration));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("ay", &isAcceleration));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("az", &isAcceleration));

        (yylhs.value.exp) = new FeiDslCaller4<int, double, double, double>(&add_acceleration_field, args, signature, "add_acceleration_field");

        for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
        nodes.push((yylhs.value.exp));
    }
#line 883 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 537 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 889 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 542 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();
        signature.clear();

        args.push_back((yystack_[0].value.exp));   //File name

        // Create the command signature for input verification
        signature.push_back(this_signature("filename", &isAdimensional));

        // Create the DSL caller node.
        //add_nodes_from_file(std::string inputnodesfile, int ndof)
        (yylhs.value.exp) = new FeiDslCaller1<std::string>(&add_elements_from_file, args, signature, "add_elements_from_file");
        nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 910 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 563 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

        for(int ii = 1;ii <=3; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 928 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 582 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[20].value.exp));        signature.push_back(this_signature("number",     &isAdimensional));
        args.push_back((yystack_[16].value.exp));        signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[10].value.exp));       signature.push_back(this_signature("node1",      &isAdimensional));
        args.push_back((yystack_[8].value.exp));       signature.push_back(this_signature("node2",      &isAdimensional));
        args.push_back((yystack_[6].value.exp));       signature.push_back(this_signature("node3",      &isAdimensional));
        args.push_back((yystack_[4].value.exp));       signature.push_back(this_signature("node4",      &isAdimensional));
        args.push_back((yystack_[0].value.exp));       signature.push_back(this_signature("magnitude",  &isPressure));


        (yylhs.value.exp) = new FeiDslCaller7<int, int,
                               int, int, int, int,
                               double>(&add_load_constant_normal_pressure_to_8node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_8node_brick_surface");

        for(int ii = 1;ii <=7; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 953 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 608 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[28].value.exp));        signature.push_back(this_signature("number",     &isAdimensional));
        args.push_back((yystack_[24].value.exp));        signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[18].value.exp));       signature.push_back(this_signature("node1",      &isAdimensional));
        args.push_back((yystack_[16].value.exp));       signature.push_back(this_signature("node2",      &isAdimensional));
        args.push_back((yystack_[14].value.exp));       signature.push_back(this_signature("node3",      &isAdimensional));
        args.push_back((yystack_[12].value.exp));       signature.push_back(this_signature("node4",      &isAdimensional));
        args.push_back((yystack_[7].value.exp));       signature.push_back(this_signature("magnitude1", &isPressure));
        args.push_back((yystack_[5].value.exp));       signature.push_back(this_signature("magnitude2", &isPressure));
        args.push_back((yystack_[3].value.exp));       signature.push_back(this_signature("magnitude3", &isPressure));
        args.push_back((yystack_[1].value.exp));       signature.push_back(this_signature("magnitude4", &isPressure));


        (yylhs.value.exp) = new FeiDslCaller10<int, int,
                                int, int, int, int,
                                double, double, double, double>(&add_load_different_normal_pressure_to_8node_brick_surface, args, signature, "add_load_different_normal_pressure_to_8node_brick_surface");

        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 981 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 637 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[28].value.exp));        signature.push_back(this_signature("number",     &isAdimensional));
        args.push_back((yystack_[24].value.exp));        signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[18].value.exp));       signature.push_back(this_signature("node1",      &isAdimensional));
        args.push_back((yystack_[16].value.exp));       signature.push_back(this_signature("node2",      &isAdimensional));
        args.push_back((yystack_[14].value.exp));       signature.push_back(this_signature("node3",      &isAdimensional));
        args.push_back((yystack_[12].value.exp));       signature.push_back(this_signature("node4",      &isAdimensional));
        args.push_back((yystack_[10].value.exp));       signature.push_back(this_signature("node5",      &isAdimensional));
        args.push_back((yystack_[8].value.exp));       signature.push_back(this_signature("node6",      &isAdimensional));
        args.push_back((yystack_[6].value.exp));       signature.push_back(this_signature("node7",      &isAdimensional));
        args.push_back((yystack_[4].value.exp));       signature.push_back(this_signature("node8",      &isAdimensional));
        args.push_back((yystack_[0].value.exp));       signature.push_back(this_signature("magnitude",  &isPressure));


        (yylhs.value.exp) = new FeiDslCaller11<int, int,
                                int, int, int, int, int, int, int, int,
                                double>(&add_load_constant_normal_pressure_to_20node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_20node_brick_surface");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1010 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 667 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[44].value.exp));        signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back((yystack_[40].value.exp));        signature.push_back(this_signature("to_element",  &isAdimensional));
        args.push_back((yystack_[34].value.exp));       signature.push_back(this_signature("node1",       &isAdimensional));
        args.push_back((yystack_[32].value.exp));       signature.push_back(this_signature("node2",       &isAdimensional));
        args.push_back((yystack_[30].value.exp));       signature.push_back(this_signature("node3",       &isAdimensional));
        args.push_back((yystack_[28].value.exp));       signature.push_back(this_signature("node4",       &isAdimensional));
        args.push_back((yystack_[26].value.exp));       signature.push_back(this_signature("node5",       &isAdimensional));
        args.push_back((yystack_[24].value.exp));       signature.push_back(this_signature("node6",       &isAdimensional));
        args.push_back((yystack_[22].value.exp));       signature.push_back(this_signature("node7",       &isAdimensional));
        args.push_back((yystack_[20].value.exp));       signature.push_back(this_signature("node8",       &isAdimensional));
        args.push_back((yystack_[15].value.exp));       signature.push_back(this_signature("magnitude1",  &isPressure));
        args.push_back((yystack_[13].value.exp));       signature.push_back(this_signature("magnitude2",  &isPressure));
        args.push_back((yystack_[11].value.exp));       signature.push_back(this_signature("magnitude3",  &isPressure));
        args.push_back((yystack_[9].value.exp));       signature.push_back(this_signature("magnitude4",  &isPressure));
        args.push_back((yystack_[7].value.exp));       signature.push_back(this_signature("magnitude5",  &isPressure));
        args.push_back((yystack_[5].value.exp));       signature.push_back(this_signature("magnitude6",  &isPressure));
        args.push_back((yystack_[3].value.exp));       signature.push_back(this_signature("magnitude7",  &isPressure));
        args.push_back((yystack_[1].value.exp));       signature.push_back(this_signature("magnitude8",  &isPressure));


        (yylhs.value.exp) = new FeiDslCaller18<int, int,
                                int, int, int, int, int, int, int, int,
                                double, double, double, double, double, double, double, double>(&add_load_different_normal_pressure_to_20node_brick_surface, args, signature, "add_load_different_normal_pressure_to_20node_brick_surface");

        for(int ii = 1;ii <=18; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1046 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 704 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[30].value.exp));        signature.push_back(this_signature("number",     &isAdimensional));
        args.push_back((yystack_[26].value.exp));        signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[20].value.exp));       signature.push_back(this_signature("node1",      &isAdimensional));
        args.push_back((yystack_[18].value.exp));       signature.push_back(this_signature("node2",      &isAdimensional));
        args.push_back((yystack_[16].value.exp));       signature.push_back(this_signature("node3",      &isAdimensional));
        args.push_back((yystack_[14].value.exp));       signature.push_back(this_signature("node4",      &isAdimensional));
        args.push_back((yystack_[12].value.exp));       signature.push_back(this_signature("node5",      &isAdimensional));
        args.push_back((yystack_[10].value.exp));       signature.push_back(this_signature("node6",      &isAdimensional));
        args.push_back((yystack_[8].value.exp));       signature.push_back(this_signature("node7",      &isAdimensional));
        args.push_back((yystack_[6].value.exp));       signature.push_back(this_signature("node8",      &isAdimensional));
        args.push_back((yystack_[4].value.exp));       signature.push_back(this_signature("node9",      &isAdimensional));
        args.push_back((yystack_[0].value.exp));       signature.push_back(this_signature("magnitude",  &isPressure));


        (yylhs.value.exp) = new FeiDslCaller12<int, int,
                                int, int, int, int, int, int, int, int, int,
                                double>(&add_load_constant_normal_pressure_to_27node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_27node_brick_surface");

        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1076 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 735 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[48].value.exp));        signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back((yystack_[44].value.exp));        signature.push_back(this_signature("to_element",  &isAdimensional));
        args.push_back((yystack_[38].value.exp));       signature.push_back(this_signature("node1",       &isAdimensional));
        args.push_back((yystack_[36].value.exp));       signature.push_back(this_signature("node2",       &isAdimensional));
        args.push_back((yystack_[34].value.exp));       signature.push_back(this_signature("node3",       &isAdimensional));
        args.push_back((yystack_[32].value.exp));       signature.push_back(this_signature("node4",       &isAdimensional));
        args.push_back((yystack_[30].value.exp));       signature.push_back(this_signature("node5",       &isAdimensional));
        args.push_back((yystack_[28].value.exp));       signature.push_back(this_signature("node6",       &isAdimensional));
        args.push_back((yystack_[26].value.exp));       signature.push_back(this_signature("node7",       &isAdimensional));
        args.push_back((yystack_[24].value.exp));       signature.push_back(this_signature("node8",       &isAdimensional));
        args.push_back((yystack_[22].value.exp));       signature.push_back(this_signature("node9",       &isAdimensional));
        args.push_back((yystack_[17].value.exp));       signature.push_back(this_signature("magnitude1",  &isPressure));
        args.push_back((yystack_[15].value.exp));       signature.push_back(this_signature("magnitude2",  &isPressure));
        args.push_back((yystack_[13].value.exp));       signature.push_back(this_signature("magnitude3",  &isPressure));
        args.push_back((yystack_[11].value.exp));       signature.push_back(this_signature("magnitude4",  &isPressure));
        args.push_back((yystack_[9].value.exp));       signature.push_back(this_signature("magnitude5",  &isPressure));
        args.push_back((yystack_[7].value.exp));       signature.push_back(this_signature("magnitude6",  &isPressure));
        args.push_back((yystack_[5].value.exp));       signature.push_back(this_signature("magnitude7",  &isPressure));
        args.push_back((yystack_[3].value.exp));       signature.push_back(this_signature("magnitude8",  &isPressure));
        args.push_back((yystack_[1].value.exp));       signature.push_back(this_signature("magnitude9",  &isPressure));


        (yylhs.value.exp) = new FeiDslCaller20<int, int,
                                int, int, int, int, int, int, int, int, int,
                                double, double, double, double, double,
                                double, double, double, double>(&add_load_different_normal_pressure_to_27node_brick_surface, args, signature, "add_load_different_normal_pressure_to_27node_brick_surface");

        for(int ii = 1;ii <=20; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1115 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 773 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        Expression* dof_number                         = force2number(*(yystack_[2].value.ident));
        UnitCheckerFunctionPointerType function_ptr    = force2signature(*(yystack_[2].value.ident));

        args.push_back((yystack_[9].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[5].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
        args.push_back((yystack_[0].value.exp));        signature.push_back(this_signature(*(yystack_[2].value.ident), function_ptr));

        (yylhs.value.exp) = new FeiDslCaller4<int, int, int, double>(&add_force_time_history_linear, args, signature, "add_force_time_history_linear");

        for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 5 exps
        nodes.push((yylhs.value.exp));
    }
#line 1136 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 795 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        Expression* dof_number                         = force2number(*(yystack_[5].value.ident));
        UnitCheckerFunctionPointerType function_ptr    = force2signature(*(yystack_[5].value.ident));

        args.push_back((yystack_[12].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[8].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
        args.push_back((yystack_[3].value.exp));        signature.push_back(this_signature(*(yystack_[5].value.ident), function_ptr));

        //Get the string from the parsed string
        string modelname = *(yystack_[0].value.ident);
        //Remove quotes
        modelname.erase(0, 1);
        modelname.erase(modelname.length()-1, modelname.length());

        //Put the string in the argument list
        args.push_back( new FeiString(modelname));
        signature.push_back(this_signature("series_file", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller5<int, int, int, double, string>(&add_force_time_history_path_time_series, args, signature, "add_force_time_history_path_time_series");

        for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 5 exps
            nodes.push((yylhs.value.exp));
    }
#line 1168 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 829 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        Expression* dof_number                         = force2number(*(yystack_[8].value.ident));
        UnitCheckerFunctionPointerType function_ptr    = force2signature(*(yystack_[8].value.ident));

        args.push_back((yystack_[15].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[11].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
        args.push_back((yystack_[3].value.exp));        signature.push_back(this_signature("time_step", &isTime));
        args.push_back((yystack_[6].value.exp));        signature.push_back(this_signature(*(yystack_[8].value.ident), function_ptr));

        //Get the string from the parsed string
        string modelname = *(yystack_[0].value.ident);
        //Remove quotes
        modelname.erase(0, 1);
        modelname.erase(modelname.length()-1, modelname.length());

        //Put the string in the argument list
        args.push_back( new FeiString(modelname));
        signature.push_back(this_signature("series_file", &isAdimensional));



        (yylhs.value.exp) = new FeiDslCaller6<int, int, int, double, double, string>(&add_force_time_history_path_series, args, signature, "add_force_time_history_path_series");

        for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 5 exps
            nodes.push((yylhs.value.exp));
    }
#line 1202 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 862 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[2].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

        for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 1218 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 877 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back(new Number(-1, unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

        nodes.pop(); //pop 1 exps
        nodes.push((yylhs.value.exp));
    }
#line 1234 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 899 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        // Required units are deduced from the passed dof.
        Expression* dof_number = dof2number(*(yystack_[21].value.ident));

        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node",    &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("time_step",    &isTime));

        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("displacement_scale_unit",    &isLength));


        //Add displacements file.
        args.push_back( (yystack_[12].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("displacement_file", &isAdimensional));                            // Specify signature


        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("velocity_scale_unit",    &isVelocity));

        //Add velocities file.
        args.push_back( (yystack_[6].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("velocity_file", &isAdimensional));                            // Specify signature


        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("acceleration_scale_unit",    &isAcceleration));

        //Add accelerations file.
        args.push_back( (yystack_[0].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature

        // Generate one command
       (yylhs.value.exp) = new FeiDslCaller10<int, int, int, double,
                              double, string, double, string, double, string>(&add_imposed_motion, args,signature, "add_imposed_motion");


        for(int i = 1; i <= 9; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1280 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 950 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        // Required units are deduced from the passed dof.
        Expression* dof_number = dof2number(*(yystack_[18].value.ident));

        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));
        args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("node",    &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));

        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("displacement_scale_unit",    &isLength));


        //Add displacements file.
        args.push_back( (yystack_[12].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("displacement_file", &isAdimensional));                            // Specify signature

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("velocity_scale_unit",    &isVelocity));

        //Add velocities file.
        args.push_back( (yystack_[6].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("velocity_file", &isAdimensional));                            // Specify signature


        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("acceleration_scale_unit",    &isAcceleration));

        //Add accelerations file.
        args.push_back( (yystack_[0].value.exp));                                          // Add to arguments of call signature
        signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature

        // Generate one command
       (yylhs.value.exp) = new FeiDslCaller9<int, int, int,
                              double, string, double, string, double, string>(&add_imposed_motion, args,signature, "add_imposed_motion");


        for(int i = 1; i <= 8; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1324 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 993 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*(yystack_[4].value.ident));

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back((yystack_[2].value.exp)); signature.push_back(this_signature(*(yystack_[4].value.ident),   &isLength));


       (yylhs.value.exp) = new FeiDslCaller4<int, int, int, double>(&add_penalty_displacement_time_history_linear, args,signature, "add_penalty_displacement_time_history_linear");


        for(int i = 1; i <= 3; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1347 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 1018 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*(yystack_[11].value.ident));

        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("time_step",   &isTime));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("scale_factor",   &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("series_file",   &isAdimensional));


       (yylhs.value.exp) = new FeiDslCaller6<int, int, int, double, double, string>(&add_penalty_displacement_time_history_path_series, args,signature, "add_penalty_displacement_time_history_path_series");


        for(int i = 1; i <= 5; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1372 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 1044 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*(yystack_[8].value.ident));

        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("scale_factor",   &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("series_file",   &isAdimensional));


       (yylhs.value.exp) = new FeiDslCaller5<int, int, int, double, string>(&add_penalty_displacement_time_history_path_time_series, args,signature, "add_penalty_displacement_time_history_path_time_series");


        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1396 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 1067 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a0",    &isFrequency));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a1",    &isTime));
        args.push_back(new FeiString(*(yystack_[0].value.ident))); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller4<int,double,double,string>(&add_damping_rayleigh, args, signature, "add_damping_rayleigh");

        for(int i = 1; i <= 3; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1414 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 1085 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("number",   &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a0",      &isTime));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a1",      &isFrequency));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a2",      &isAdimensional));
        args.push_back(new FeiString(*(yystack_[0].value.ident))); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller5<int,double,double,double,string>(&add_damping_caughey3rd, args, signature, "add_damping_caughey3rd");

        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1433 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 1104 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number",   &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("a0",      &isTime));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a1",      &isFrequency));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a2",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a3",      &isAdimensional));
        args.push_back(new FeiString(*(yystack_[0].value.ident))); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller6<int,double,double,double,double,string>(&add_damping_caughey4th, args, signature, "add_damping_caughey4th");

        for(int i = 1; i <= 5; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1453 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 1123 "feiparser.yy" // lalr1.cc:847
    {
        //add_damping_to_element(int elementNumber, int dampingNumber)
        args.clear(); signature.clear();

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("element_number",    &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1470 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 1139 "feiparser.yy" // lalr1.cc:847
    {
        //add_damping_to_node(int nodeNumber, int dampingNumber)
        args.clear(); signature.clear();

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("node_number",    &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1487 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 1155 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("mx",    &isMass));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("my",    &isMass));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("mz",    &isMass));

        //add_mass_to_node(int, double, double, double, double, double, double, double);
        (yylhs.value.exp) = new FeiDslCaller4<int,double,double,double>(&add_mass_to_node, args, signature, "add_mass_to_node");

        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1506 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 1173 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("mx",    &isMass));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("my",    &isMass));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("mz",    &isMass));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("Imx",    &isMassMomentOfInertia));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("Imy",    &isMassMomentOfInertia));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("Imz",    &isMassMomentOfInertia));

        //add_mass_to_node(int, double, double, double, double, double, double, double);
        (yylhs.value.exp) = new FeiDslCaller7<int,double,double,double,double,double,double>(&add_mass_to_node, args, signature, "add_mass_to_node");

        for(int i = 1; i <= 7; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1528 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 1194 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("master",           &isAdimensional));
        args.push_back((yystack_[2].value.exp)); signature.push_back(this_signature("slave",           &isAdimensional));

        int dofnum = 1;
        for (dofchain_holder_it = dofchain_holder.begin();
            dofchain_holder_it != dofchain_holder.end();
            dofchain_holder_it++)
        {
            args.push_back(*dofchain_holder_it);    signature.push_back(this_signature("dof"+boost::lexical_cast<string>(dofnum++), &isAdimensional)); // lexical_cast converts number to string in this case (www.boost.org) :)
        }

        //add_equaldof_to_two_nodes(int, int, int);
        if (dofchain_holder.size() == 1)
            (yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 2)
            (yylhs.value.exp) = new FeiDslCaller4<int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 3)
            (yylhs.value.exp) = new FeiDslCaller5<int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 4)
            (yylhs.value.exp) = new FeiDslCaller6<int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 5)
            (yylhs.value.exp) = new FeiDslCaller7<int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 6)
            (yylhs.value.exp) = new FeiDslCaller8<int, int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
        else if (dofchain_holder.size() == 7)
            (yylhs.value.exp) = new FeiDslCaller9<int, int, int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1566 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 1246 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("time_step",            &isTime));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("scale_factor",         &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("number_of_steps",       &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number_of_drm_nodes",     &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("number_of_drm_elements",   &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("element_file",       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("nodes_file",         &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("displacement_file",  &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("acceleration_file",  &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller16<int, 
                                double, double, 
                                int, int, int, 
                                double, double, double, 
                                double, double, double, 
                                string, string, 
                                string, string>(&add_load_pattern_domain_reduction_method, args, signature, "add_load_pattern_domain_reduction_method");

        for(int i = 1; i <= 16; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1604 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 1299 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("pattern_number",           &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("time_step",           &isTime));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("scale_factor",           &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("number_of_steps",           &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("number_of_drm_nodes",           &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number_of_drm_elements",           &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("element_file",       &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("nodes_file",         &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("displacement_file",  &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("acceleration_file",  &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("force_file",         &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller17<int, double, double, int, int, int, double, double, double, double, double, double, string, string, string, string, string>(&add_load_pattern_domain_reduction_method_save_forces, args, signature, "add_load_pattern_domain_reduction_method_save_forces");

        for(int i = 1; i <= 17; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1637 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 1345 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("pattern_number",           &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("time_step",           &isTime));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("scale_factor",           &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number_of_steps",           &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("number_of_drm_nodes",           &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("number_of_drm_elements",           &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("element_file",           &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("nodes_file",           &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("force_file",           &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller15<int, double, double, int, int, int, double, double, double, double, double, double, string, string, string>(&add_load_pattern_domain_reduction_method_restore_forces, args, signature, "add_load_pattern_domain_reduction_method_restore_forces");

        for(int i = 1; i <= 15; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1668 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 1375 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("section_number",           &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material_number",           &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("thickness",           &isLength));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

        for(int i = 1; i <= 3; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1685 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 1395 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("section_number",      &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("elastic_modulus",    &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("poisson_ratio",       &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("thickness",           &isLength));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("mass_density",        &isDensity));

        (yylhs.value.exp) = new FeiDslCaller5<int, double, double, double, double>(&add_section_elastic_membrane_plate_to_shell, args, signature, "add_section_elastic_membrane_plate_to_shell");

        for(int i = 1; i <= 5; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1704 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 1421 "feiparser.yy" // lalr1.cc:847
    {
        // This implementation is a bit uncommon. The need arises due to the unknown
        // number of fixed dofs the user will specify. Also since the add_support_to_node
        // and related DSLs take only two parameters, one command will generate
        // multiple calls to the DSL. The way to do this is by using ExpressionList
        // which is a linked list of expressions.
        Expression* cmd;
        Expression* command_list;
        int count = 0;

        //dofchain is just a std::vector<Number*> to hold the parsed DOFS to fix.
        //dofchain_it is its const iterator to access each component.
        for (dofchain_holder_it = dofchain_holder.begin();
            dofchain_holder_it != dofchain_holder.end();
            dofchain_holder_it++)
        {
            //Since we're creating one command for each element in dofchain, clear the args and signature.
            args.clear(); signature.clear();

            args.push_back((yystack_[2].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
            args.push_back(*dofchain_holder_it); signature.push_back(this_signature("dof", &isAdimensional));

            // Generate one command
            cmd = new FeiDslCaller2<int, int>(&add_support_to_node, args,signature,"add_support_to_node");

            // Add to the linked list
            if (count == 0)
                command_list = cmd;
            else
                command_list = new ExpressionList(cmd, command_list);
            count++;
        }
        (yylhs.value.exp) = command_list;
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1745 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 1461 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();           signature.clear();
        args.push_back((yystack_[2].value.exp));     signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1760 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 1475 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();           signature.clear();

        Expression* dof_number = dof2number(*(yystack_[4].value.ident));
        UnitCheckerFunctionPointerType function_ptr = dof2signature(*(yystack_[4].value.ident));


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("node", &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("constraint_value", function_ptr));


        (yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_single_point_constraint_to_node,args,signature, "add_single_point_constraint_to_node");


        for(int ii = 1;ii <=2; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1783 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1497 "feiparser.yy" // lalr1.cc:847
    {
                Expression* cmd;
        Expression* command_list = 0;
        int count = 0;
        for (dofchain_holder_it = dofchain_holder.begin();
            dofchain_holder_it != dofchain_holder.end();
            dofchain_holder_it++)
        {
            args.clear(); signature.clear();
            args.push_back((yystack_[2].value.exp));                     signature.push_back(this_signature("number", &isAdimensional));
            args.push_back(*dofchain_holder_it);    signature.push_back(this_signature("dof", &isAdimensional));

            cmd = new FeiDslCaller2<int, int>(&remove_support_from_node, args,signature ,"remove_support_from_node");

            if (count == 0)
                command_list = cmd;
            else
                command_list = new ExpressionList(cmd, command_list);
            count++;
        }
        (yylhs.value.exp) = command_list;
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1812 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1525 "feiparser.yy" // lalr1.cc:847
    {
                            dofchain_holder.clear();
                            dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
                        }
#line 1821 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1529 "feiparser.yy" // lalr1.cc:847
    {
                            dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
                        }
#line 1829 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1539 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
        nodes.push((yylhs.value.exp));
    }
#line 1839 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1548 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
        nodes.push((yylhs.value.exp));
    }
#line 1849 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1557 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("factor", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1863 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1570 "feiparser.yy" // lalr1.cc:847
    {
        // This case is a bit different. ALGNAME contains a string to
        // indicate which algorithm to choose. Instead of having different
        // tokens for each algorithm, just have one and decide in the
        // semantic action which DSL to call.
        // Usually different DSLs have different arguments, but in this
        // case all three take no arguments, which makes this approach easier.

        int (*f)() = NULL;         // function poiner to the algorithm DSL
        string fname;       // name of the DSL called to report


        args.clear(); signature.clear();
        if((*(yystack_[0].value.ident)).compare("With_no_convergence_check") == 0 || (*(yystack_[0].value.ident)).compare("with_no_convergence_check") == 0)
        {
            f = &define_algorithm_with_no_convergence_check_for_analysis;
            fname = "define_algorithm_with_no_convergence_check_for_analysis";
        }
        else if((*(yystack_[0].value.ident)).compare("Modified_Newton") == 0 || (*(yystack_[0].value.ident)).compare("modified_newton") == 0)
        {
            f = &define_algorithm_modifiednewton_for_analysis;
            fname = "define_algorithm_modifiednewton_for_analysis";
        }
        else if((*(yystack_[0].value.ident)).compare("Newton") == 0 || (*(yystack_[0].value.ident)).compare("newton") == 0)
        {
            f = &define_algorithm_newton_for_analysis;
            fname = "define_algorithm_newton_for_analysis";
        }
        else
        {
            cerr << "Algorithm " << *(yystack_[0].value.ident) << " not recognized.\n\n";
        }

        if(f == NULL)
        {
            (yylhs.value.exp) = new Empty();
        }
        else
        {
            (yylhs.value.exp) = new FeiDslCaller0<>(f,args, signature, fname);
        }
        
        nodes.push((yylhs.value.exp));
    }
#line 1912 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1618 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        int (*f)(double, int, int);         // function poiner to the test DSL
        string fname;       // name of the DSL called to report

        args.clear(); signature.clear();
        if((yystack_[9].value.ident)->compare("Norm_Displacement_Increment") == 0)
        {
            f = &define_convergence_test_normdisplacementincrement_for_analysis;
            fname = "define_convergence_test_normdisplacementincrement_for_analysis";
            args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isLength));
        }
        if((yystack_[9].value.ident)->compare("Energy_Increment") == 0)
        {
            f = &define_convergence_test_energyincrement_for_analysis;
            fname = "define_convergence_test_energyincrement_for_analysis";
            args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isEnergy));
        }
        if((yystack_[9].value.ident)->compare("Norm_Unbalance") == 0)
        {
            f = &define_convergence_test_normunbalance_for_analysis;
            fname = "define_convergence_test_normunbalance_for_analysis";
            args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isForce));
        }
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("verbose_level", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller3<double, int, int> (f, args, signature, fname);

        for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 3 exps
        nodes.push((yylhs.value.exp));
    }
#line 1950 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1655 "feiparser.yy" // lalr1.cc:847
    {
        int (*f)() = &define_solver_umfpack_for_analysis;         // function poiner to the algorithm DSL
        string fname;       // name of the DSL called to report

        args.clear(); signature.clear();
        if((yystack_[0].value.ident)->compare("ProfileSPD") == 0 || (yystack_[0].value.ident)->compare("profilespd") == 0)
        {
            f = &define_solver_profilespd_for_analysis;
            fname = "define_solver_profilespd_for_analysis";
        }
        if((yystack_[0].value.ident)->compare("UMFPack") == 0 || (yystack_[0].value.ident)->compare("umfpack") == 0)
        {
            f = &define_solver_umfpack_for_analysis;
            fname = "define_solver_umfpack_for_analysis";
        }
        if((yystack_[0].value.ident)->compare("parallel") == 0)
        {
             f = &define_solver_parallel_for_analysis; 
             fname = "define_solver_parallel_for_analysis"; 
        }

        (yylhs.value.exp) = new FeiDslCaller0<>(f, args, signature, fname);

        nodes.push((yylhs.value.exp));
    }
#line 1980 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 1684 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("gamma", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

        for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 1995 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 1698 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

        nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 2009 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 1711 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*(yystack_[2].value.ident));


        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("node",         &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",  &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("increment",   &isLength));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&define_static_solution_advancement_integrator_displacement_control, args, signature, "define_static_solution_advancement_integrator_displacement_control");



        for(int ii = 1;ii <=2; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2031 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 1738 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        //cout << "  --> Starting new model named " << *$3 << endl;
        //$$ = new FeiDslCaller0<>(&wipe_analysis, args, signature, "wipe_analysis");
        //$$ = new ExpressionList(new Empty(), $$);

        //Get the string from the parsed string
        string modelname = *(yystack_[0].value.ident);
        //Remove quotes
        modelname.erase(0, 1);
        modelname.erase(modelname.length()-1, modelname.length());

        //Put the string in the argument list
        args.push_back( new FeiString(modelname));
        signature.push_back(this_signature("name", &isAdimensional));

        //Push into the expression list
        (yylhs.value.exp) = new FeiDslCaller1<string>(&define_model_name, args, signature, "define_model_name");
        nodes.push((yylhs.value.exp));

        // Define output file name with the string
        // Remove quotes
        if (out_cpp_filename.length() == 0)
        {
            string newfilename = *(yystack_[0].value.ident);
            newfilename.erase(0, 1);
            newfilename.erase(newfilename.length()-1, newfilename.length());

            set_outcppfile_name(newfilename);
        }



    }
#line 2070 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 1776 "feiparser.yy" // lalr1.cc:847
    {
        Quantity returnQuantity;
        string temp = *(yystack_[0].value.ident);
        temp.erase(0, 1);
        temp.erase(temp.length()-1, temp.length());

        //Repace white spaces with underscore "_".
        size_t pos = temp.find(" ");
        while( pos != string::npos)
        {
            temp.replace(pos,1,"_");
            pos = temp.find(" ");
        }

        // Now generate the command
        args.clear();
        signature.clear();
        args.push_back( new FeiString(temp));
        signature.push_back(this_signature("name", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<string>(&start_new_stage, args, signature,"start_new_stage");

        nodes.push((yylhs.value.exp));
    }
#line 2099 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 1804 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2113 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 1817 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("time_step", &isTime));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

        nodes.pop();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2130 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 1837 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("time_step", &isTime));
        args.push_back((yystack_[16].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("minimum_time_step", &isTime));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_time_step", &isTime));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_iterations", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller5<double, int, double, double, int>(&simulate_using_transient_variable_multistep, args, signature, "simulate_using_transient_variable_multistep");

        for(int ii = 1;ii <=5; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2150 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 1856 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back
        (this_signature("number_of_modes", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2165 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 1873 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_strain", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("material", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_constant_p_triaxial, args, signature, "simulate_constitutive_testing_for_constant_p_triaxial");


        for(int ii = 1;ii <=4; ii++) nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 2187 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 1897 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_strain", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("material", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_drained_triaxial, args, signature, "simulate_constitutive_testing_for_drained_triaxial");


        for(int ii = 1;ii <=4; ii++) nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 2209 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 1921 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_strain", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("material", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_triaxial_stress_control, args, signature, "simulate_constitutive_testing_for_undrained_triaxial_stress_control");


        for(int ii = 1;ii <=4; ii++) nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 2231 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1945 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_strain", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("material", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_simple_shear, args, signature, "simulate_constitutive_testing_for_undrained_simple_shear");


        for(int ii = 1;ii <=4; ii++) nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 2253 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1969 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_strain", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("material", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_triaxial, args, signature, "simulate_constitutive_testing_for_undrained_triaxial");


        for(int ii = 1;ii <=4; ii++) nodes.pop();

        nodes.push((yylhs.value.exp));
    }
#line 2275 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 1995 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2289 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 2008 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2303 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 2021 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

        //pop 1
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2318 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 2035 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
             (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
             //pop 1
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2331 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 2047 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
             (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2344 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 2071 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));

        (yylhs.value.exp) = new FeiDslCaller4<int, double, double, double>(&add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d, args, signature, "add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d");
        for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
        nodes.push((yylhs.value.exp));
    }
#line 2360 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 2096 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",            &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller11<int, int, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2385 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 2128 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[26].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller9<int, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2407 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 2157 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[26].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller9<int, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterialLT_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterialLT_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2429 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2189 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",       &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller12<int, int, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_isotropic_hardening");
        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2453 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2221 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller10<int, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening");
        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2476 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2255 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller13<int, int, double, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_kinematic_hardening");
        for(int ii = 1;ii <=13; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2502 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2291 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",       &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller12<int, int, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening");
        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2527 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2325 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",   &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",   &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller11<int, double, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening");
        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2551 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2357 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller10<int, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_linear_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_linear_kinematic_hardening");
        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2574 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2409 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[92].value.exp));  signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                 &isAdimensional));
        args.push_back((yystack_[87].value.exp));  signature.push_back(this_signature("mass_density",              &isDensity));
        args.push_back((yystack_[84].value.exp)); signature.push_back(this_signature("initial_void_ratio",       &isAdimensional));
        args.push_back((yystack_[81].value.exp)); signature.push_back(this_signature("initial_shear_modulus",    &isAdimensional));
        args.push_back((yystack_[78].value.exp)); signature.push_back(this_signature("initial_bulk_modulus",     &isAdimensional));
        args.push_back((yystack_[75].value.exp)); signature.push_back(this_signature("sanisand2008_Pat",             &isPressure));
        args.push_back((yystack_[72].value.exp)); signature.push_back(this_signature("sanisand2008_k_c",             &isAdimensional));
        args.push_back((yystack_[69].value.exp)); signature.push_back(this_signature("sanisand2008_alpha_cc",        &isAdimensional));
        args.push_back((yystack_[66].value.exp)); signature.push_back(this_signature("sanisand2008_c",               &isAdimensional));
        args.push_back((yystack_[63].value.exp)); signature.push_back(this_signature("sanisand2008_xi",              &isAdimensional));
        args.push_back((yystack_[60].value.exp)); signature.push_back(this_signature("sanisand2008_lambda",          &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("sanisand2008_ec_ref",          &isAdimensional));
        args.push_back((yystack_[54].value.exp)); signature.push_back(this_signature("sanisand2008_m",               &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("sanisand2008_h0",              &isAdimensional));
        args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("sanisand2008_ch",              &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("sanisand2008_nb",              &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("sanisand2008_A0",              &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("sanisand2008_nd",              &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("sanisand2008_p_r",             &isPressure));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("sanisand2008_rho_c",           &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("sanisand2008_theta_c",         &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("sanisand2008_X",               &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("sanisand2008_z_max",           &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("sanisand2008_cz",              &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("sanisand2008_p0",              &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("sanisand2008_p_in",             &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


/*         cout << "\n\n\n ******************************************** \n\n" ; */
/*         cout << "WARNING ON SANISAND2008!!! Names of parameters are not definitive! Also the unit checking is not correct! :)"; */
/*         cout << "\n\n\n ******************************************** \n\n" ; */

        (yylhs.value.exp) = new FeiDslCaller31<int, int,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_sanisand_2008, args, signature, "add_constitutive_model_NDMaterial_sanisand_2008");
        for(int ii = 1;ii <= 31; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2628 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2476 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[44].value.exp));  signature.push_back(this_signature("number",                       &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                     &isAdimensional));
        args.push_back((yystack_[39].value.exp));  signature.push_back(this_signature("mass_density",                 &isDensity));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("reference_void_ratio",         &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("critical_stress_ratio_M",      &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("lambda",                       &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("kappa",                        &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("minimum_bulk_modulus",         &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("pressure_reference_p0",        &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller15<int, int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_camclay, args, signature, "add_constitutive_model_NDMaterial_camclay");

        for(int ii = 1;ii <=15; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2662 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2521 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[38].value.exp));  signature.push_back(this_signature("number",                       &isAdimensional));
        args.push_back((yystack_[33].value.exp));  signature.push_back(this_signature("mass_density",                 &isDensity));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("reference_void_ratio",         &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("critical_stress_ratio_M",      &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("lambda",                       &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("kappa",                        &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("minimum_bulk_modulus",         &isPressure));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("pressure_reference_p0",        &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller13<int,
                               double, double, double,
                               double, double, double, double,
                               double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_camclay, args, signature, "add_constitutive_model_NDMaterial_accelerated_camclay");
        for(int ii = 1;ii <=13; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2692 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2575 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        args.push_back((yystack_[77].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                 &isAdimensional));
        args.push_back((yystack_[72].value.exp)); signature.push_back(this_signature("mass_density",              &isDensity));
        args.push_back((yystack_[69].value.exp)); signature.push_back(this_signature("e0",       &isAdimensional));
        args.push_back((yystack_[66].value.exp)); signature.push_back(this_signature("sanisand2004_G0",    &isAdimensional));
        args.push_back((yystack_[63].value.exp)); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back((yystack_[60].value.exp)); signature.push_back(this_signature("sanisand2004_Pat",                   &isPressure));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("sanisand2004_p_cut",                 &isAdimensional));
        args.push_back((yystack_[54].value.exp)); signature.push_back(this_signature("sanisand2004_Mc",                    &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("sanisand2004_c",                     &isAdimensional));
        args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("sanisand2004_lambda_c",              &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("sanisand2004_xi",                    &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("sanisand2004_ec_ref",                &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("sanisand2004_m",                     &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("sanisand2004_h0",                    &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("sanisand2004_ch",                    &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("sanisand2004_nb",                    &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("sanisand2004_A0",                    &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("sanisand2004_nd",                    &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("sanisand2004_z_max",                 &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("sanisand2004_cz",                    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

/*         cout << "\n\n\n ******************************************** \n\n" ; */
/*         cout << "WARNING ON Dafalias-Manzari!!! Names of parameters are not definitive! Also the unit checking is not correct! :)"; */
/*         cout << "\n\n\n ******************************************** \n\n" ; */

        (yylhs.value.exp) = new FeiDslCaller26<int, int,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                double, double, double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_sanisand_2004, args, signature, "add_constitutive_model_NDMaterial_sanisand_2004");

        for(int ii = 1;ii <= 26; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2742 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2635 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number",                         &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                      &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",                   &isDensity));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",               &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",                 &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("druckerprager_angle",           &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",      &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",  &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",  &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller12<int, int,
                                double, double, double,
                                double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening");

        for(int ii = 1;ii <= 12; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2772 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2673 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",     &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller10<int, double,
                                double, double, double,
                                double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_isotropic_hardening");

        for(int ii = 1;ii <= 10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2799 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2711 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                     &isAdimensional));

        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));  //units?
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));  //units?
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller13<int, int,
                                double, double, double,
                                double, double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening");

        for(int ii = 1;ii <= 13; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2831 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2752 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("druckerprager_angle",          &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));  //units?
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));  //units?
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller11<int, double,
                                double, double, double,
                                double, double, double,
                                int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening");

        for(int ii = 1;ii <= 11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2860 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2791 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                     &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller11<int, int,
                                double, double, double,
                                double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic");

        for(int ii = 1;ii <= 11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2889 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2827 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[26].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller9<int,
                                double, double,
                                double, double, double,
                                int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_perfectly_plastic");
        for(int ii = 1;ii <= 9; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2915 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2858 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[20].value.exp));  signature.push_back(this_signature("number",                       &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("elastic_modulus_horizontal",   &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("elastic_modulus_vertical",     &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("poisson_ratio_h_v",            &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("poisson_ratio_h_h",            &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("shear_modulus_h_v",            &isPressure));
        args.push_back((yystack_[15].value.exp));  signature.push_back(this_signature("mass_density",                 &isDensity));

        (yylhs.value.exp) = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic, args, signature, "add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic");
        for(int ii = 1;ii <=7; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2935 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2877 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
        (yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

        for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
            nodes.push((yylhs.value.exp));
    }
#line 2951 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2899 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("yield_strength",            &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("elastic_modulus",          &isPressure));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("strain_hardening_ratio",   &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a1",                       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a2",                       &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a3",                       &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("a4",                       &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_steel01, args, signature, "add_constitutive_model_uniaxial_steel01");
        for(int ii = 1;ii <=8; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
      }
#line 2974 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2931 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("yield_strength",            &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",          &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("strain_hardening_ratio",   &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("R0",                       &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("cR1",                      &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("cR2",                      &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a1",                       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a2",                       &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a3",                       &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("a4",                       &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller11<int, double, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_steel02, args, signature, "add_constitutive_model_uniaxial_steel02");
        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3000 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2963 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("compressive_strength",            &isPressure));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("strain_at_compressive_strength",          &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("crushing_strength",   &isPressure));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("strain_at_crushing_strength",                       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("lambda",                      &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tensile_strength",                      &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tension_softening_stiffness",                       &isPressure));


        (yylhs.value.exp) = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_concrete02, args, signature, "add_constitutive_model_uniaxial_concrete02");
        for(int ii = 1;ii <=8; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3023 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2995 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta_min",                 &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller11<int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                double>(&add_constitutive_model_NDMaterial_pisano, args, signature, "add_constitutive_model_NDMaterial_pisano");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3052 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 3033 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta_min",                 &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller11<int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                double>(&add_constitutive_model_NDMaterialLT_pisano, args, signature, "add_constitutive_model_NDMaterialLT_pisano");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3081 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 3064 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));

        (yylhs.value.exp) = new FeiDslCaller4<int, double, double, double>(&add_constitutive_model_NDMaterialLT_linear_elastic_isotropic_3d, args,
            signature,"add_constitutive_model_NDMaterialLT_linear_elastic_isotropic_3d");
        for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
        nodes.push((yylhs.value.exp));
    }
#line 3098 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 3089 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));

        (yylhs.value.exp) = new FeiDslCaller10<int,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_constitutive_model_NDMaterialLT_New_Pisano, args, signature, "add_constitutive_model_NDMaterialLT_New_Pisano");

        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3125 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 3127 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node, args, signature, "add_element_brick_8node");


        for(int ii = 1;ii <=10; ii++) nodes.pop(); //pop 11 exps
        nodes.push((yylhs.value.exp));
    }
#line 3149 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 3153 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node_ltensor, args, signature, "add_element_brick_8node_ltensor");

        for(int ii = 1;ii <=10; ii++) nodes.pop(); //pop 10 exps
        nodes.push((yylhs.value.exp));
    }
#line 3172 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 3178 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node_elastic, args, signature, "add_element_brick_8node_elastic");

        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3195 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 3202 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("noGPs", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller11<int,int,int,int,
                                int,int,int,int,
                                int,int,int>(&add_element_brick_8node_variable_number_of_gauss_points, args, signature, "add_element_brick_8node_variable_number_of_gauss_points");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3221 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 3231 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[63].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[55].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[47].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[43].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node9", &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller29<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int>(&add_element_brick_27node, args, signature, "add_element_brick_27node");

        for(int ii = 1;ii <=29; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3265 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3278 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[63].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[55].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[47].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[43].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node9", &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller29<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_ltensor, args, signature, "add_element_brick_27node_ltensor");

        for(int ii = 1;ii <=29; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3309 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3325 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[69].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[65].value.exp)); signature.push_back(this_signature("noGP", &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[55].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[47].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[43].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node9", &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller30<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_variable_number_of_gauss_points, args, signature, "add_element_brick_27node_variable_number_of_gauss_points");

        for(int ii = 1;ii <=30; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3354 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3374 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[63].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[55].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[47].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[43].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node9", &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller29<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_elastic, args, signature, "add_element_brick_27node_elastic");

        for(int ii = 1;ii <=29; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3398 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3421 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[69].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[65].value.exp)); signature.push_back(this_signature("noGP",   &isAdimensional));
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[55].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[47].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[43].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node9", &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller30<int,int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,
                                int>(&add_element_brick_variable_node_8_to_27, args, signature, "add_element_brick_variable_node_8_to_27");


        for(int ii = 1;ii <=30; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3445 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3472 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[49].value.exp));  signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[43].value.exp));  signature.push_back(this_signature("node1",  &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node2",  &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node3",  &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node4",  &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node5",  &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node6",  &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node7",  &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node8",  &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node9",  &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller22<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int>(&add_element_brick_20node, args, signature, "add_element_brick_20node");

        for(int ii = 1;ii <=22; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3482 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3512 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[55].value.exp));  signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[51].value.exp));  signature.push_back(this_signature("noGP", &isAdimensional));
        args.push_back((yystack_[43].value.exp));  signature.push_back(this_signature("node1",  &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node2",  &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node3",  &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node4",  &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node5",  &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node6",  &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node7",  &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node8",  &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node9",  &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller23<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int,int>(&add_element_brick_20node_variable_number_of_gauss_points, args, signature, "add_element_brick_20node_variable_number_of_gauss_points");

        for(int ii = 1;ii <=23; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3520 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3554 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[49].value.exp));  signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[43].value.exp));  signature.push_back(this_signature("node1",  &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node2",  &isAdimensional));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node3",  &isAdimensional));
        args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node4",  &isAdimensional));
        args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("node5",  &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("node6",  &isAdimensional));
        args.push_back((yystack_[31].value.exp)); signature.push_back(this_signature("node7",  &isAdimensional));
        args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("node8",  &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node9",  &isAdimensional));
        args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller22<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int,int>(&add_element_brick_20node_elastic, args, signature, "add_element_brick_20node_elastic");

        for(int ii = 1;ii <=22; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3557 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3602 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[52].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[46].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[44].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[40].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[34].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("porosity", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("rho_s", &isDensity));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("rho_f", &isDensity));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));  //L^3*T/M
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("K_s", &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("K_f", &isPressure));

        (yylhs.value.exp) = new FeiDslCaller19<int,int,int,int,int,int,
                                int,int,int,int,
                                double,double,double,double,
                                double,double,double,double,double>(&add_element_brick_8node_upU, args, signature, "add_element_brick_8node_upU");

        for(int ii = 1;ii <=19; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3593 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3642 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[52].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[46].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[44].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[40].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[34].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("porosity", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("rho_s", &isDensity));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("rho_f", &isDensity));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("K_s", &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("K_f", &isPressure));

        (yylhs.value.exp) = new FeiDslCaller19<int,int,int,int,int,int,int,int,int,int,
                                double,double,double,double,double,double,double,double,double>(&add_element_brick_8node_up, args, signature, "add_element_brick_8node_up");

        for(int ii = 1;ii <=19; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3627 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3681 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[76].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[70].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[68].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[66].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[64].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
        args.push_back((yystack_[62].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
        args.push_back((yystack_[60].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
        args.push_back((yystack_[58].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
        args.push_back((yystack_[56].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
        args.push_back((yystack_[54].value.exp)); signature.push_back(this_signature("node9",  &isAdimensional));
        args.push_back((yystack_[52].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
        args.push_back((yystack_[50].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
        args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
        args.push_back((yystack_[46].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
        args.push_back((yystack_[44].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
        args.push_back((yystack_[40].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
        args.push_back((yystack_[34].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
        args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));

        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("porosity", &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("rho_s", &isDensity));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("rho_f", &isDensity));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("K_s", &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("K_f", &isPressure));

        (yylhs.value.exp) = new FeiDslCaller31<int,int,int,int,int,int,int,int,int,int,
                                int,int,int,int,int,int,int,int,int,int,
                                int, int,
                                double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_upU, args, signature, "add_element_brick_20node_upU");

        for(int ii = 1;ii <=31; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3676 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3740 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("elastic_modulus", &isPressure));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("shear_modulus", &isPressure));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));

        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density", &isDensity));

        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));
        //
        //int add_element_beam_elastic_3d(int ElementNumber,
        //                 double A,
        //                 double E,
        //                 double G,
        //                 double Jx,
        //                 double Iy,
        //                 double Iz,
        //                 int iNode,
        //                 int jNode,
        //                 double rho,
        //                 double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        //                 double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        //                 double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)
        (yylhs.value.exp) = new FeiDslCaller19<int,
                                double, double, double, double, double, double,
                                int, int, double,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_element_beam_elastic, args, signature, "add_element_beam_elastic");

        for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop        19 exps
        nodes.push((yylhs.value.exp));
    }
#line 3730 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3804 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("elastic_modulus", &isPressure));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("shear_modulus", &isPressure));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));

        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density", &isDensity));

        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));
        //
        //int add_element_beam_elastic_3d(int ElementNumber,
        //                 double A,
        //                 double E,
        //                 double G,
        //                 double Jx,
        //                 double Iy,
        //                 double Iz,
        //                 int iNode,
        //                 int jNode,
        //                 double rho,
        //                 double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        //                 double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        //                 double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)
        (yylhs.value.exp) = new FeiDslCaller19<int,
                                double, double, double, double, double, double,
                                int, int, double,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_element_beam_elastic_lumped_mass, args, signature, "beam_elastic_lumped_mass");

        for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop 19 exps
        nodes.push((yylhs.value.exp));
    }
#line 3784 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3865 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[49].value.exp));  signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back((yystack_[43].value.exp));  signature.push_back(this_signature("node1",                         &isAdimensional));
        args.push_back((yystack_[41].value.exp)); signature.push_back(this_signature("node2",                         &isAdimensional));
        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("number_of_integration_points",  &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("SectionNumber",                 &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));




/*        string modelname = *$24;
        modelname.erase(0, 1);
        modelname.erase(modelname.length()-1, modelname.length());
        args.push_back( new FeiString(modelname));
        signature.push_back(this_signature("IntegrationRule", &isAdimensional));*/


        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("IntegrationRule",               &isAdimensional));


        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("vecxzPlane_X",    &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));

        (yylhs.value.exp) = new FeiDslCaller16<int, int, int, int, int, double, string,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_element_beam_displacement_based, args, signature, "add_element_beam_displacement_based");

        for(int ii = 1;ii <=16; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3830 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3912 "feiparser.yy" // lalr1.cc:847
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*(yystack_[0].value.ident));
        UnitCheckerFunctionPointerType function_ptr = dof2stiffnesschecker(*(yystack_[0].value.ident));

        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("node",        &isAdimensional));
        args.push_back((yystack_[2].value.exp)); signature.push_back(this_signature("stiffness",  function_ptr));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller4<int, int, double, int>(&add_element_penalty_for_applying_generalized_displacement, args, signature, "add_element_penalty_for_applying_generalized_displacement");

        for(int ii = 1;ii <=3; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3853 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3936 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        Expression* dof_number = dof2number(*(yystack_[0].value.ident));
        UnitCheckerFunctionPointerType function_ptr = dof2stiffnesschecker(*(yystack_[0].value.ident));


        args.push_back((yystack_[14].value.exp));  signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back((yystack_[8].value.exp));  signature.push_back(this_signature("node1",       &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("node2",       &isAdimensional));
        args.push_back((yystack_[2].value.exp)); signature.push_back(this_signature("stiffness",    function_ptr));
        args.push_back(dof_number); signature.push_back(this_signature("dof",  isAdimensional));

         (yylhs.value.exp) = new FeiDslCaller5<int, int, int, double, int>(&add_element_penalty, args, signature, "add_element_penalty");


        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3878 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3971 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[57].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
        args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("elastic_modulus", &isPressure));
        args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("shear_modulus", &isPressure));
        args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

        args.push_back((yystack_[51].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[49].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));

        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density", &isDensity));

        args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));

        (yylhs.value.exp) = new FeiDslCaller19<int,
                                double, double, double, double, double, double,
                                int, int, double,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_element_rank_one_deficient_elastic_pinned_fixed_beam, args, signature, "add_element_rank_one_deficient_elastic_pinned_fixed_beam");


        for(int ii = 1;ii <=19; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3920 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 4018 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("number",                  &isAdimensional));

        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("node1",                   &isAdimensional));
        args.push_back((yystack_[22].value.exp)); signature.push_back(this_signature("node2",                  &isAdimensional));

        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("normal_stiffness",       &isThisUnit<1, 0, -2>));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("tangential_stiffness",   &isThisUnit<1, 0, -2>));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("friction_ratio",         &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("maximum_gap",            &isLength));

        args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("x_local_1",              &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("x_local_2",              &isAdimensional));
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("x_local_3",              &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller10<int, int, int,
                               double, double, double,
                               double, double, double, double>(&add_element_contact_nonlinear_3dof_to_3dof, args, signature, "add_element_contact_nonlinear_3dof_to_3dof");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3950 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 4050 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("thickness", &isLength));

        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller7<int, int, int, int, int,
                               double, int>(&add_element_shell_MITC4, args, signature, "add_element_shell_MITC4");

        for(int ii = 1;ii <=7; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3974 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 4076 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("thickness", &isLength));

        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller7<int, int, int, int, int,
                               double, int>(&add_element_shell_NewMITC4, args, signature, "add_element_shell_NewMITC4");

        for(int ii = 1;ii <=7; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3998 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 4102 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("thickness", &isLength));

        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller6<int, int, int, int, double, int>(&add_element_shell_andes_3node, args, signature, "add_element_shell_andes_3node");

        for(int ii = 1;ii <=6; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4020 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 4126 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("thickness", &isLength));

        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller7<int, int, int, int, int, double, int>(&add_element_shell_andes_4node, args, signature, "add_element_shell_andes_4node");

        for(int ii = 1;ii <=7; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4043 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 4152 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[19].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[13].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("material", &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("mass_density", &isDensity));

        (yylhs.value.exp) = new FeiDslCaller6<int, int, int, int, double, double>(&add_element_truss, args, signature, "add_element_truss");

        for(int ii = 1;ii <=6; ii++) nodes.pop(); //pop 6 exps
        nodes.push((yylhs.value.exp));
    }
#line 4063 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 4340 "feiparser.yy" // lalr1.cc:847
    {
        if ((yystack_[0].value.exp)) // Null pointer safety
        {
            /* Check if the statement is a DslAction in which case we may
            want to report the error code.
            */
            DslAction *actionpointer = dynamic_cast<DslAction*>((yystack_[0].value.exp));
            if (actionpointer != 0)
            {
                try
                {
                    actionpointer->value();   /* this evaluates the AST */
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                    cout << "Returning control to user!" << endl << endl;
                    newstdin();
                }            }
            else // If Statement is just an expression evaluate it.
            {
                try
                {
                    (yystack_[0].value.exp)->value();   /* this evaluates the AST */
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                    cout << "Returning control to user!" << endl << endl;
                    newstdin();
                }
            }
            clear_stack ();

        }
        if (FLAG_interactive_mode == 1)
        {
            cout << prompt;
        }
        (yylhs.value.errcode) = 0;
    }
#line 4109 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4382 "feiparser.yy" // lalr1.cc:847
    {
        if ((yystack_[0].value.exp)) // Null pointer safety
        {
            /* Check if the statement is a DslAction in which case we may
            want to report the error code.
            */
            DslAction *actionpointer = dynamic_cast<DslAction*>((yystack_[0].value.exp));
            if (actionpointer != 0)
            {
                try
                {
                    actionpointer->value();   /* this evaluates the AST */
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                    cout << "Returning control to user!" << endl << endl;
                    newstdin();
                }
            }
            else
            {  // If Statement is just an expression evaluate it.
                try
                {
                    (yystack_[0].value.exp)->value();   /* this evaluates the AST */
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                    cout << "Returning control to user!" << endl << endl;
                    newstdin();
                }
            }
            clear_stack ();
        }
        if (FLAG_interactive_mode == 1)
        {
            cout << prompt;
        }
        (yylhs.value.errcode) = 0;
    }
#line 4155 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4424 "feiparser.yy" // lalr1.cc:847
    {
        clear_stack ();

        if ( recovery_mode == 0)
        {
            //Tell lexer to discard every file in stack and go into interactive safe mode
            error_behavior();
        }

        (yylhs.value.errcode) = 0;
    }
#line 4171 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4439 "feiparser.yy" // lalr1.cc:847
    {   /*  Just a reminder...
        ">"  = 1
        "<"  = 2
        "<>" = 3
        "==" = 4
        ">=" = 5
        "<=" = 6
        "&" = 7
        "|" = 8
        */
        switch ((yystack_[1].value.fn))
        {
            case 1: // >
                (yylhs.value.exp) = new GreaterThan((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 2: // <
                (yylhs.value.exp) = new LessThan((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 3: // <>
                (yylhs.value.exp) = new NotEqualTo((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 4: // ==
                (yylhs.value.exp) = new EqualTo((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 5: // >=
                (yylhs.value.exp) = new GreaterThanOrEqualTo((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 6: // <=
                (yylhs.value.exp) = new LessThanOrEqualTo((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 7: // &
                (yylhs.value.exp) = new LogicalAnd((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            case 8: // |
                (yylhs.value.exp) = new LogicalOr((yystack_[2].value.exp),(yystack_[0].value.exp));
                break;
            default:
                (yylhs.value.exp) = new Empty() ;
                cout << "Comparison operator not defined!" << endl;
        }
        nodes.pop();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4220 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4484 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  //  The children are handled by Plus so we
        nodes.pop ();  // take them of the allocated nodes stack.
        nodes.push ((yylhs.value.exp));
    }
#line 4231 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4491 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  //  The children are handled by Plus so we
        nodes.pop ();  // take them of the allocated nodes stack.
        nodes.push ((yylhs.value.exp));
    }
#line 4242 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4498 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4253 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4505 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4264 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4512 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4275 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4519 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4286 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4526 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.push ((yylhs.value.exp));
    }
#line 4296 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4532 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4307 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4539 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4319 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4547 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4331 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4555 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4343 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4563 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4355 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4571 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4367 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4579 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4375 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4583 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4385 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4589 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Number (*(yystack_[0].value.value));
        nodes.push ((yylhs.value.exp));
    }
#line 4394 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4594 "feiparser.yy" // lalr1.cc:847
    {

        (yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
        nodes.push((yylhs.value.exp));
    }
#line 4404 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4600 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4414 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4606 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

        nodes.push((yylhs.value.exp));
    }
#line 4424 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4612 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4434 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4618 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4444 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4624 "feiparser.yy" // lalr1.cc:847
    {
        string temp = *(yystack_[0].value.ident);
        temp.erase(0, 1);                               //remove quotes
        temp.erase(temp.length()-1, temp.length());     //remove quotes
        (yylhs.value.exp) = new FeiString(temp);
        nodes.push((yylhs.value.exp));
    }
#line 4456 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4632 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
        nodes.push((yylhs.value.exp));
    }
#line 4466 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4638 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
        nodes.push((yylhs.value.exp));
    }
#line 4475 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4643 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
        nodes.push((yylhs.value.exp));
    }
#line 4484 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4648 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
        nodes.push((yylhs.value.exp));
    }
#line 4493 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4653 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
        nodes.push((yylhs.value.exp));
    }
#line 4502 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4658 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
        nodes.push((yylhs.value.exp));
    }
#line 4511 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4663 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
        nodes.push((yylhs.value.exp));
    }
#line 4520 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4671 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();
        signature.clear();

        Expression* which_coordinate;
        which_coordinate = dof2number(*(yystack_[0].value.ident));
        if (which_coordinate->value() == -100)
        {
            string msg = "Unknown node data field request -> ";
            msg += *(yystack_[0].value.ident);
            throw RunTimeException( msg);
            (yylhs.value.exp) = new Empty();
        }
        else
        {
            nodes.push(which_coordinate);

            args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("node_number", &isAdimensional));
            args.push_back(which_coordinate); signature.push_back(this_signature("coordinate", &isAdimensional));

            (yylhs.value.exp) = new FeiDslCaller2<int,int,Quantity>(&get_nodal_coordinate,args,signature,"get_nodal_coordinate");
        }

        nodes.pop();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4552 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4702 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 4561 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4707 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4569 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4711 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
        args.clear();
        signature.clear();
    }
#line 4579 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4720 "feiparser.yy" // lalr1.cc:847
    {
        Comparison* ptr_comp = dynamic_cast<Comparison*>((yystack_[1].value.exp));
        if (!ptr_comp) // Check whether ptr_comp is a valid Comparison* pointer
        {
            cout << "If statement requieres a comparison." << endl;
            (yylhs.value.exp) = new Empty();
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));
        }
        else
        {
            (yylhs.value.exp) = new IfStatement(ptr_comp, (yystack_[0].value.exp), NULL);
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));
        }
    }
#line 4602 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4742 "feiparser.yy" // lalr1.cc:847
    {
        Comparison* ptr_comp = dynamic_cast<Comparison*>((yystack_[3].value.exp));
        if (!ptr_comp) // Check whether ptr_comp is a valid Comparison* pointer
        {
            cout << "If statement requires a comparison." << endl;
            (yylhs.value.exp) = new Empty();
            nodes.pop();
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));

        }
        else
        {
            (yylhs.value.exp) = new IfStatement(ptr_comp, (yystack_[2].value.exp), (yystack_[0].value.exp));
            nodes.pop();
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));
        }
    }
#line 4628 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4767 "feiparser.yy" // lalr1.cc:847
    {
        Comparison* ptr_comp = dynamic_cast<Comparison*>((yystack_[1].value.exp));
        if (!ptr_comp)  // Check whether ptr_comp is a valid Comparison* pointer
        {
            cout << "While statement requires a comparison." << endl;
            (yylhs.value.exp) = new Empty();
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));

        }
        else
        {
            (yylhs.value.exp) = new WhileStatement(ptr_comp, (yystack_[0].value.exp));
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));
        }
    }
#line 4652 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4787 "feiparser.yy" // lalr1.cc:847
    {
        if (FLAG_generate_cpp_output == 1)
        {
            *FeiDslCallerBase::cppfile << "}" << endl;
        }
        if((yystack_[0].value.ident)->compare("quit") == 0) cout << "quit: \n/kwit/ \nverb \n   1. leave (a place), usually permanently. \n " << endl ;
        if((yystack_[0].value.ident)->compare("exit") == 0) cout << "All exits are marked by the \"exit\" word..."<< endl ;
        if((yystack_[0].value.ident)->compare("bye") == 0) cout << "How polite! Bye, have a nice day!" << endl ;

        wipe_model();

        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));

        //exit(0);
        return(0);
    }
#line 4674 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4805 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        if (FLAG_generate_cpp_output == 1)
        {
            *FeiDslCallerBase::cppfile << "}" << endl;
        }

        (yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
        nodes.push((yylhs.value.exp));
    }
#line 4689 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4816 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Empty();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4699 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4826 "feiparser.yy" // lalr1.cc:847
    {
        // empty statement generates an  empty expression
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 4709 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4832 "feiparser.yy" // lalr1.cc:847
    {
        // Used to initialize a list of statements (compound statement)
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4718 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4840 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[0].value.exp);
    }
#line 4726 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4844 "feiparser.yy" // lalr1.cc:847
    {
        if ((yystack_[0].value.exp) == NULL)
            (yylhs.value.exp) = (yystack_[1].value.exp);
        else
        {
            (yylhs.value.exp) = new ExpressionList((yystack_[1].value.exp), (yystack_[0].value.exp));
            nodes.pop();
            nodes.pop();
            nodes.push((yylhs.value.exp));
        }
    }
#line 4742 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4746 "feiparser.tab.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  feiparser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  feiparser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const short int feiparser::yypact_ninf_ = -378;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
     241,  -378,   598,  -378,  -378,  -377,  -335,   621,   621,  -378,
    -378,    64,  -378,  -378,  -378,  -378,  -378,  -378,  -378,   621,
     621,    28,  -378,  -378,    25,    31,  -378,  5884,  -318,    41,
      48,   220,   273,   362,    -2,  -262,  -230,   621,  -378,  -231,
    -378,  -378,  -378,  -378,  -378,   158,   -19,  -378,   173,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   -31,   -31,
    -378,   140,   140,   -98,    85,    87,  -378, 10548,   621,   193,
      93,    98,    90,   103,   115,   129,   131,  -150,   121,   144,
     145,   147,   139,    78,   152,  -378,   621,   155,   156,   159,
     170,   172,   174,  -124,   164,   210,   212,   208,   -33,   215,
     202,   150, 10187,   199,  -378,  -378,   179,  -378,  -378,  -378,
     621,   621,   621,   621,   621,   621,   621,  -378,  -378, 10548,
   10548, 10548, 10548, 10548, 10548, 10548,   263,   272,  1093,   302,
    -378,   621,   621,   621, 10548,  -378,   621,   621,  -378,   211,
     621,   621,   621,   621,   223,   -27,   621,   621,   621,   216,
     266,   278,    36,   621,   621,   621,   621,   621,   621,   280,
     214,  -378,   -11,  -378,   248,   252,   267,   283,   -47,   270,
     358,  -378,   284,   284,    89,    89,   -15,   140,   322,  -378,
    -378,  -378,  2858,   -39,   -35, 10548, 10548, 10548, 10354, 10116,
     621, 10126, 10244, 10257, 10106,   -18,   275,   276,   621, 10135,
   10267,  6019,   621,   264,   288,   -17, 10290, 10300, 10548, 10548,
   10548, 10548,   289,   621,   341,   190,    45,   346,   305,   238,
     244,  -103,     3,  -378,  -378,  -378,  -378,    11, 10463, 10548,
   10380,    83,   357,    88,  -249,   359,   621,   621, 10392,   -91,
     364,   381, 10333,   363,   621,   417,   -34,   312,   621, 10548,
     621,   390,   391,   318,   393,   218,   254,    -6,   269,   368,
     369,   371,   621,   -67,   -64,   347,   396,   -63,   398,   401,
     407,   404,   405,   410,   411,   412,   413,   416,   420,   422,
     426,   430,   447,   448,   449,   450,   454,   183,   194,   243,
     292,   293,   294,   295,   300,   303,   310,   311,   315,   316,
     327,   330,   331,   333,   334,   336,   337,   177,   338,   298,
     301,   339,   360,   366,   367,   365,   498,   529,   530,   534,
     535,   539,   540,   541,   547, 10405, 10414,   622,   423,   389,
     551,   621,   621,   552,  1607,  -378,  -378,   312,  -378,  -378,
   10548,  5977,   621,   621,   621,   621,   496,   489,   500,   501,
     504,  -378,   577,   585,   307,   621,   569,   621,   571,   572,
     573,   621,   574,   576,   578,   621,   582,   583,   584,   587,
     591,   602,   633,   641,   642,   650,   654,   655,   657,   658,
     659,   660,   742,   743,   744,   745,   755,   756,   758,   759,
     760,   761,   762,   770,   771,   773,   781,   782,   784,   785,
     786,   787,   788,   790,   791,   792,   793,   794,   795,   803,
     804,   561,   621,   621,   621,   621,   456,   469,   470,   621,
     817,   818,   621,   819,   821,   621,  6010, 10309,   621,   822,
    -378,   823, 10548,  5968, 10207, 10548,   683,   751,   753,   757,
     687,   831,   833,   621,  8528,   468,  8537,   472,   476,   480,
    8546,   483,   484,   485,  8555,   486,   488,   493,   494,   503,
     506,   513,   514,   522,   523,   525,   526,   527,   531,   542,
     543,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     747, 10145, 10154, 10216, 10548, 10548,   870,   896,   907, 10343,
     621,   621, 10065,   621,   621, 10197,   910,  -378,   189,   621,
     621,   912,   836,   780,   900,   902,   904,   854,   621,   621,
     632,   799,   621,   800,   621,   621,   621,   801,   621,   621,
     621,   837,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,  7661,  7670,
    7679,  7688,  7697,  7713,  7722,  7734,  7743,  6526,  6350,  6359,
    6539,  7753,  7764,  7775,  7784,  7793,  7802,  6861,  6265,  7650,
    7812,  7821,  7411,  7830,  7423,  7432,  7839,   832,   -30,    39,
     855,   621,   621,   621,   623, 10074, 10083,   931, 10279,  7441,
     940,   864,   621,   914,  1306,  5986,   621,   892,   918,   920,
     921,   922,   989, 10548, 10373,   621,   874,  1687,   876,  1957,
    2021,  2074,   886,  2088,  2110,  2121,   887,  2175,  2188,  2229,
    2245,  2290,  2314,   395,  2355,  2366,  2392,  2401,  2441,  2491,
    2570,  2582,  2612,  1017,  1036,  1037,  1038,  1039,  1040,  1041,
    1042,  1050,  1051,  1053,  1054,  1056,  1060,  1066,  1070,  1073,
    1074,  1076,  1078,  1081,  1082,  1083,  1084,  1085,  1094,  1096,
    1097,  1098,  -378,   963,  1013,  1019,  -378,  1027,  1044,   -85,
    5843,  5855,  5868,  1086,  1108,  1110,   621,  1091,  1123,   621,
    1035,  6001,  1099,  1126,  1127, 10548,   621,  1104,   621,   621,
     621,  1058,  1130,   406,   938,   621,   939,   621,   621,   621,
     941,   621,   621,   621,   943,   621,   621,   621,   621,   621,
     621,   952,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
    1133,  1145,  1150,  1125,  1088,  1168,  1169,  1170,  1171,  1172,
    1100,   621,   621,  9975,  1129,   621, 10168,    50,  1173,  1055,
     621,   621, 10548,  1132,  9422,  9438,  9447,   621,   621,  1136,
    1137,  2638,  1140,  2690,  2879,  2888,  1146,  2902,  2914,  2923,
    1147,  2937,  2949,   421,   459,   537,   575,  1216,   748,  2958,
    2967,  2979,  2988,   763,   774,  2997,  3006,  7450,  7459,  7475,
    7484,  7496,  7505,  7516,  7527,  7537,  6560,  6325,  6334,  6517,
    7546,  7555,  7564,  7573,  7583,  7592,  6849,  6256, 10548,  6274,
    6287,  6186,  7601,  6196,  6210,  7613,   621,   621,   621,  1148,
     853,   621,   621,   621,   621,   621,   621,  9984,  9999,  1229,
     621,  7970,  1138,  1176,  1131,   621,   875,  1791, 10548,   621,
    1234,  1241,  1243,  9459, 10382,   621,  1165,   621,  1167,   621,
     621,   621,  1177,   621,   621,   621,  1178,   621,   621,  1195,
    1062,  1063,  1077,   621,  1095,   621,   621,   621,   621,  1193,
    1089,   621,   621,  1281,  1283,  1285,  1286,  1288,  1289,  1290,
    1291,  1313,  1325,  1326,  1334,  1335,  1337,  1338,  1339,  1340,
    1341,  1343,  1344,  1353,  1355,  1365,  1366,  1374,  1375,  1379,
    1382, 10548, 10423,  9479,   621,   621, 10050,  9613,  5816,  5777,
    5786,   721,  1383,  1384,   621, 10548,  1412,  -378,  1413,  1414,
   10548,   621,  1417,  9468,   621,   621,   621,  1420,  1421, 10320,
    1057,  3015,  1072,  3024,  3033,  3042,  1079,  3057,  3066,  3075,
    1080,  3084,  3093,   621,  1422,  1442,  1443, 10226,  1450,  3102,
    3111,   796,  3120,  1294,  1451,  3129,  3138,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,  1452,  1453, 10548,  3147,  1454,
    1455,  1456,  1460,  1461,   312,   621,   621,  9943,   621,   621,
     621, 10548,   621,  1462,  9373,  9382,  9391,   621,   621,  -378,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621, 10235,   621,   621,   621,  1386,   621,   621,
     621,  1389,   621,  1392,   621,   621,   621, 10548,  7324,  7333,
    7343,  7353,  7362,  7374,  7383,  7393,  6797,   342,  5924,  7623,
    7265,  7277,  7287,  7296,  7306,  7315,  6894,  6247,  5877,  5723,
    6159,  7402,  6168,  6177, 10548,   621,  1496,   621,   621,   621,
    1139,   621,   621,  -378,  9957,  9966,  1480,  7879, 10092,  9491,
      76,   621,  1481,  1482,  1483,  9400, 10364,  3156,  3165,  3174,
    3183,  3192,  3201,  3210,  3230,  3239,  3248,  3257,  3269,  3282,
    1410,  7850,  7861,   933,  -378,  7870,   862,   872,  1469,   881,
     621,  8085,  3291,  3304,  1494,  1497,  1498,  1499,  1500,  1508,
    1511,  1512,  1520,  1521,  1529,  1530,  1539,  1540,  1542,  1543,
    1544,  1545,  1547,  1548,  1562,  1563,  1564,  1565,  1569,  1600,
    9500,  -378,  3317,  9622, 10030,  -378,  5825,  5750,  1602,  1603,
     621,  1606,  1614,  1615,  1618,  9412,   621,   621,   621,  1619,
    1620,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,  1479,  1621,  1625,  1419,  1627,  1457,
    1554,  1571,  1561,  8237,  1646,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,  1654,   621,  1655,  1659,  1662,  1663,   621,
     621,  9911,   621,   621,   621,   621,  1664,  9318,  9331,  9340,
     621,   621,  3326,  3335,  3347,  3356,  3365,  3374,  3383,  3392,
    3401,  3410,  3425,  3434,  3443,  1591,   621,   621,  -378,   621,
    1643,  1651,   621,  1653,  1670,   621,  3452,  3461,  6964,  6924,
    6903,  6915,  7218,  6976,  6937,  7234,  6818,  5942,  5951,  6551,
    6946,  6955,  7243,  7256,  6991,  7000,  6885,  5933,  5796,  2197,
    6131,  7013,  6140,  6150,  1708,  3470,   621,   621,  1346,   621,
    9921,  9930,  1681, 10548,  9509, 10548,  1532,   621,  1683,  1684,
    1694,  9352, 10548,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,  8195,  8204,  8226,
    1613,  1623,  7980,  1624,   621,  8075,   621,   621,  1705,  1706,
    1709,  1710,  1712,  1714,  1715,  1716,  1725,  1734,  1737,  1763,
    1765,  1766,  1769,  1771,  1774,  1779,  1781,  1783,  1787,  1788,
    1790,  1798,  1799,  1801,  -378,   621, 10039,  9539,  -378,  5834,
    1802,  1803,   621,  1811,  1812,  9363,   621,   621,   621,  1814,
    3479,  3488,  3497,  3506,  3515,  3524,  3533,  3542,  3551,  3560,
    3569,  3578,  3587,  7888,  1816,  1819,  1820,   621,   621,  1821,
     621,  7897,  1822,  3598,  3607,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,    52,  1824,  1825,  1827,   621,   621,  9873,   621,   621,
    1828, 10548, 10548, 10548,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,  1829,   621,
     621,   621,  7989,  7999,   621,  8011,  1830,   621,   621,   621,
   10432,  7022,  7034,  7043,  7181,  9009,  7053,  7190,  6806,  7632,
    7641,  6503,  7062,  7072,  7199,  7208, 10447,  9022,  6872,  6234,
    5759,  1222,  6104,  9031,  6113,  6122,   621,  1637,   621,   621,
    1502,  9889,  9898,  1832, 10548,  1026,   621, 10548,  3616,  3625,
    3636,  3650,  3659,  3671,  3685,  3694,  3703,  3715,  3724,  3733,
    3742,   621,  8160,  8176,  8185,  1834,  1835, 10548,  1837,   621,
    8066,  3751,  3760,  1841,  1849,  1850,  1852,  1853,  1862,  1863,
    1865,  1867,  1868,  1870,  1873,  1876,  1883,  1887,  1888,  1889,
    1916,  1917,  1918,  1927,  1928,  1930,  1931,  1932,  1933,  3769,
      24,  9548, 10008,  -378,  1944,  1945,   621,  1953, 10548,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,  8097,  1954,  1956,  1967,   621,   621,   621, 10548,
    1968,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
    1352,   621,  1969,  1981,   621,   621,  9846,   621,  3778,   897,
    3793,  3802,   916,  3811,  3820,  3829,  3838,   974,  3847,  1043,
    3856,  1982,   621,   621,   621, 10548, 10548, 10548,   621,  1116,
    3865,  9215, 10456, 10469,  9040,  7081,  8805,  9053,  7090,  6785,
    6302,  6316,  6493, 10478,  9062,  7099,  7109,  9225,  8814, 10548,
    6219,  5732,  5893,  6055,  8823,  6086,  6095,  3874,   621, 10548,
     621,   621,  9855,  9864,  1983, 10548,   621,  1729,   621,   621,
    1767,   621,   621,   621,   621,  1780,   621,  1909,   621,   621,
    8106,  8115,  8124,  8020,  1921,   621,  1992,  1993,  1995,  2005,
    2009,  2010,  2011,  2012,  2014,  2016,  2017,  2018,  2019,  2020,
    2028,  2051,  2053,  2057,  2058,  2061,  2073,  2081,  2082,  2084,
    2085,   621,  3883, 10019,  9564,  2087,  2098,   621,  3892,  1974,
    3901,  3910,  1997,  3919,  3928,  3937,  3946,  2064,  3955,  2075,
    3966,  8029,  2100,  2102,  2103,  2105,  2083,  3975,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,  3984,   621,  2106,  2108,   621,   621,  9808,
     621,  1717,   621,   621,  2036,   621,   621,   621,   621,  2043,
     621,  2044,   621,  2120,   621,   621,   621,  1755,  2054,   621,
    9076,  9234,  9244,  8833, 10493,  8564,  8844,  9085,  6776,  6829,
    6838,  6478,  9253,  8859, 10502,  9095,  9104,  8579, 10548, 10548,
    5805,  7906,  8590,  7915,  7924,   621,  3993,   621,   621,  9818,
    9832,  2129,  4003,   621,  4018,  4027,   621,  4038,  4053,  4062,
    4071,   621,  4083,   621,  4092,  1772,  8133,  8142,  8151,   621,
     621,  4101,  2131,  2132,  2133,  2135,  2137,  2140,  2148,  2151,
    2152,  2153,  2154,  2173,  2182,  2183,  2185,  2186,  2187,  2195,
    2196,  2204,  2205,  2207,  2211,   118,   621,  9573, 10548,  2212,
    2228,   621,   621, 10548,   621,   621,  8497,   621,   621,   621,
     621,  8506,   621, 10548,   621,   621,  2236,  2237,  2239,  4110,
   10548,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,  2117,  4119,  2240,   621,
     621,  9776,  1249,  4128,  4137,  2242,  4146,  4161,  4170,  4179,
    2243,  4188,  4197,  4206,   621,   621,   621,   621,  4215,  8870,
    9114,  9123,  8600,  9262, 10548,  8609,  8882,  6762,  7119,  7131,
    6469,  9137,  8618,  9275,  8891,  8900, 10548,  5768,  7140, 10548,
    7149,  7159,  1258,    40,   621,   621,  9785,  9799,  2244,  2170,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,  7933,  7949,  7958,  4224,   621,  2253,  2255,  2256,  2270,
    2273,  2278,  2281,  2286,  2287,  2289,  2297,  2298,  2300,  2301,
    2303,  2304,  2306,  2308,  2309,  2311,  2158,  1924,   621,  1269,
   10548,  2312,  2313,   621,  2323,  4233,  4242,  8463,  4251,  4260,
    4269,  4278,  8479,  4287,  4296,  4305,  2328,  2333,  2336,   621,
    4314,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,    63,   621, 10548,  -378,   621,   621,  9749,  2274,   621,
     621,  2351,   621,   621,   621,   621,  2352,   621,   621,   621,
     621,   621,   621,  1318,   621,  8631,  8913,  8922, 10548,  9147,
   10548,  8640,  6753, 10515,  9161,  6460,  8936, 10548,  9170,  8656,
    8693,  5741,  6037,  6046, 10548,  1988,   621,  4323,  9758,  9767,
    2363,   621,  4334,  4343,   621,  4352,  4362,  4371,  4386,   621,
    4395,  4405,  1327,  8039,  8048,  8057,  -378,  4421,  2365,  2376,
    2377,  2389,  2390,  2391,  2399,  2408,  2409,  2411,  2412,  2413,
    2414,  2415,  2423,  2426,   621, 10548,   621,  2431,  2434,   621,
   10548,   621,   621,  8431,   621,   621,   621,   621,  8440,   621,
     621,  2157,  2435,  2436,  2437,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,  4430,  4439,   621,   621,  9718,  4451,  4460,  2439,
    4469,  4478,  4487,  4496,  2440,  4505,  4514,  2448,  2086,  2090,
    2091,  4529, 10548,  8704,  8719,  8945, 10548,  6738,  9285,  8954,
    6451,  8730,  8964, 10548, 10548, 10548, 10548, 10548,   621,   621,
    9731,  9740,  2451,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,  2095,   621,   621,   621,   621,  2453,  2462,
    2463,  2464,  2467,  2468,  2472,  2481,  2482,  4538,  4547,  2483,
    2489,   621,  4556,  4565,  8401,  4574,  4583,  4592,  4601,  8412,
    4610,  4619,   621,  4628,  4637,  4646,  4655,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
    9683,   621,   621,  2498,   621,   621,   621,   621,  2499,   621,
     621,  4664,   621,   621,   621,   621, 10548, 10548,  8741,  6729,
    9179,  8750,  6441, 10548,  8759,  4673,  4682,  9692,  9703,  2501,
    4691,  4702,   621,  4711,  4721,  4730,  4739,   621,  4754,  4764,
     621,  4773,  4789,  4798,  4807,  2502,  2503,  2507,  2508,  2509,
    2510,   621,   621,  2512,  2513,   621,   621,   621,  8374,   621,
     621,   621,   621,  8383,   621,   621,  4819,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,  4828,  4837,   621,
     621,  9650,  4846,  4855,  2514,  4864,  4873,  4882,  4897,  2515,
    4906,  4915,   621,  1345,  1358,  1367,  4924, 10548,  6713,  8973,
   10548,  6425, 10548,   621,   621,  9659,  9668,  2519,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,  1468,  2324,
    2329,  2330,   621,  2521,  2529,  2531,  4933,  4942,  2532,  2533,
     621,  4951,  4960,  8347,  4969,  4978,  4987,  4996,  8356,  5005,
    5014,  2335,  2536,  2537,  2538,  5023,   621,   621,   621,   621,
     621,   621,   621,  9631,   621,   621,  2539,   621,   621,   621,
     621,  2540,   621,   621,  2541,  2177,  2179,  2180,   621,  6698,
    8772,  6416,  5032,  5041,  9640,  9518,  2550,  5050,  5059,   621,
    5070,  5080,  5089,  5098,   621,  5107,  5121,  2192,   621,   621,
     621,  5132,  2559,  2561,  2562,   621,   621,  2563,  2567,   621,
     621,   621,  8314,   621,   621,   621,   621,  8323,   621,   621,
     621,  5141,  5157,  5166,   621,   621,   621,   621,  5175,  1486,
     621,   621,  9585,  5187,  5196,  2568,  1501,  5205,  1513,  1522,
    2569,  5214,  5223,  5232,   621,   621,   621,  5241,  6689, 10548,
    6407,   621,  -378,  9600, 10548,  2589,   621,   621,   621,  2517,
     621,  2518,  2522,   621,   621,   621,   621,  5250,  5265,  5274,
     621,  2590,  2599,  5283,  2600,   621,  5292,  5301,  8280,  2577,
    5310,  2580,  2581,  8296,  5319,  5328,  5337,   621,   621,   621,
    5346,   621,   621,   621,   621, 10548,   621,   621,  2606,  2545,
     621,  2546,  2548,  2607,   621,   621,   621,  1629,  1638,  1647,
     621,  6679,  6398,  1698,  9527,  5355,  5364,   621,   621,  5373,
     621,   621,   621,  5382,  5391,  1718,  2424,  2425,  2427,  5400,
    2625,  2631,  -378,  2645,   621,   621,  8246,  8515,   621, 10548,
   10548,  8255,   621,   621,  -378,  2646,  2648,  2649,   621,   621,
     621,   621,  5409,  5418,  2650,  2651,  1727,  2652,  5427,  5437,
    2292,  2299,  2302,  5448,  6670,  6386, 10548,   621,   621,   621,
     621,  2591,   621,   621,   621,   621,   621,   621,   621,  2667,
    2671,  5457,  5466, 10548,  8488,  2653, 10548,  5475,  5488,  5500,
    5509,  5525,  5534,   621,   621,   621,   621,  2672,  2601,   621,
     621,   621,   621,   621,   621,  6655,  6377,  5543,  5555,   621,
     621,  5564,  5573,  5582,  5591,  5600,  5609,  2673,  2674,   621,
     621,  8454, 10548,   621,   621,   621,   621,   621,   621,   621,
     621,  5618,  5633,  2676,  5642,  5651,  1804,  1842,  1855,  5660,
    6646,  6368,   621,   621,   621,   621,   621,  -378,  -378,  -378,
     621,  2678,  2682,  5669,  1920,  8422,  5678,  1937,  1946,   621,
     621,   621,  2622,  2698,   621,  2626,  2632,  6631,  7168,  5687,
    2684,   621,  5696,  2685,  2687,  2700,  2712,   621,  2639,  8392,
     621,  2640,  2641,   621,   621,  5705,   621,  2716,  5714,   621,
     621,  6622, 10524,   621, 10548,   621,   621, 10548, 10548,  2717,
    2718,  1985,  8365,  1998,   621,   621,  2654,  2723,  2655,  6613,
    9300,  2701,   621,  2702,  2727,  2728,  2658,  8332,  2660,   621,
     621,   621,  2735,   621,  6599,  9192, 10548,   621, 10548,  2736,
    2738,  8305,   621,   621,  2739,  6587,  8983,   621,  2741,  2742,
    8271,   621,   621,  2745,  6574,  8781,   621,  2747,  2748, 10548,
     621,   621, 10539, 10548,  2751,   621,  9309,  2752,   621,  9201,
    2754,   621,  8998,  2757,   621,  8796,  2758,   621, 10548
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   142,   160,   159,   165,     0,     0,     0,     0,    15,
     180,     0,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     9,     8,     0,     0,   181,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   174,     0,
      10,    12,    13,    11,    14,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     182,   158,   150,     0,     0,     0,     3,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,     0,   176,     1,   141,
       0,     0,     0,     0,     0,     0,     0,   175,   162,   151,
     152,   153,   154,   155,   156,   161,     0,     0,     0,   177,
     179,     0,     0,     0,     7,    68,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   157,   144,   145,   146,   147,   148,   149,   143,   164,
     163,   183,   185,     0,     0,     5,     4,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    80,    79,
      82,    81,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,   186,   184,   178,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,    54,    57,    53,    56,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    66,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,    37,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    31,     0,     0,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,    39,     0,     0,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    78,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   135,   136,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,     0,     0,     0,     0,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    26,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
      89,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,     0,     0,     0,     0,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,   101,   102,   107,   109,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    91,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
      96,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    27,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,     0,     0,     0,     0,     0,   122,
     124,     0,     0,     0,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,   126,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   128,   129,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   117,     0,     0,   120,   118,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,     0,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -378,  -378,  -378,  -378,  -245,  -378,  -378,  -378,  -378,  -378,
    -378,    -7,    21,   -25,  2611
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    39,    40,    41,   338,    42,    43,    44,    85,   138,
      45,    46,   182,   129,   183
  };

  const short int
  feiparser::yytable_[] =
  {
      58,    59,   339,   110,   111,   112,   113,   114,   115,   131,
     765,   116,    61,    62,    67,   110,   111,   112,   113,   114,
     115,    47,    56,   116,   115,   355,   102,   116,   357,   361,
     106,     2,     3,     4,   130,     5,     6,   356,   196,   197,
     358,   362,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   766,   164,   165,    12,    13,    14,    15,    16,    17,
      18,   134,   257,    19,    57,   258,   109,    60,   336,    20,
     110,   111,   112,   113,   114,   115,   337,    68,   116,   152,
     198,    69,    63,    64,    86,    65,   110,   111,   112,   113,
     114,   115,   430,   259,   116,    87,   260,   673,   674,   675,
     261,   676,    88,   172,   173,   174,   175,   176,   177,   178,
     110,   111,   112,   113,   114,   115,   219,   103,   116,   220,
     221,   321,   322,   323,   185,   186,   187,   114,   115,   188,
     189,   116,   104,   191,   192,   193,   194,   316,   317,   199,
     200,   201,   319,   320,   328,   329,   206,   207,   208,   209,
     210,   211,   110,   111,   112,   113,   114,   115,   108,   226,
     116,     2,     3,     4,   105,     5,     6,     7,   348,   349,
       8,   677,   678,     9,   107,    10,   118,    11,   863,   864,
    1640,  1641,   116,   229,    12,    13,    14,    15,    16,    17,
      18,   238,   132,    19,   133,   242,  2047,  2048,   135,    20,
     136,    21,    22,    23,    24,   137,   249,    25,   139,    26,
     140,    27,    28,   110,   111,   112,   113,   114,   115,  2135,
    2136,   116,   141,   110,   111,   112,   113,   114,   115,   325,
     326,   116,    29,    30,    31,    32,   142,   334,   143,   144,
     145,   340,     1,   341,     2,     3,     4,    33,     5,     6,
       7,   146,   147,     8,   148,   354,     9,   149,    10,   150,
      11,   151,   153,   154,    34,   159,   155,    12,    13,    14,
      15,    16,    17,    18,    89,    90,    19,   156,    91,   157,
      92,   158,    20,    93,    21,    22,    23,    24,   160,   161,
      25,   163,    26,   162,    27,    28,   167,   110,   111,   112,
     113,   114,   115,   166,    86,   116,   110,   111,   112,   113,
     114,   115,   168,   184,   116,    29,    30,    31,    32,   170,
     112,   113,   114,   115,   426,   427,   116,   195,   190,   203,
      33,    94,   204,   202,   212,   432,   433,   434,   435,   213,
     214,   110,   111,   112,   113,   114,   115,    34,   444,   116,
     446,    95,   215,    96,   450,    97,   216,    98,   454,   217,
      99,   218,   222,   223,    -1,     2,     3,     4,   225,     5,
       6,   128,   235,   243,   250,   128,   110,   111,   112,   113,
     114,   115,   236,   237,   116,    66,   117,   245,    12,    13,
      14,    15,    16,    17,    18,   244,   248,    19,   252,   251,
     253,   254,   255,    20,   501,   502,   503,   504,   505,   256,
     262,   318,   509,   324,   331,   512,    28,   333,   515,   330,
     335,   518,   337,   342,   343,   344,   345,    37,   347,   110,
     111,   112,   113,   114,   115,   346,   530,   116,   350,   205,
     110,   111,   112,   113,   114,   115,   351,   352,   116,   353,
     382,   359,  1516,  1517,   100,   110,   111,   112,   113,   114,
     115,   383,  1184,   116,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   110,   111,   112,   113,   114,   115,   365,
     360,   116,   363,   595,   596,   364,   598,   599,   366,   367,
     384,   402,   604,   605,   368,   369,   370,   371,  1955,  1956,
     372,   613,   614,   101,   373,   617,   374,   619,   620,   621,
     375,   623,   624,   625,   376,   627,   628,   629,   630,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   377,   378,   379,   380,    35,    36,    37,   381,   385,
     386,   387,   388,    38,     2,     3,     4,   389,     5,     6,
     390,   110,   111,   112,   113,   114,   115,   391,   392,   116,
     171,   603,   393,   394,   680,   681,   682,    12,    13,    14,
      15,    16,    17,    18,   395,   691,    19,   396,   397,   695,
     398,   399,    20,   400,   401,   411,   403,   406,   703,   110,
     111,   112,   113,   114,   115,    28,    48,   116,    49,    50,
      51,    52,    53,    54,     2,     3,     4,   407,     5,     6,
     404,    55,   410,   405,   408,   409,   412,   413,    35,    36,
      37,   414,   415,   416,   417,   418,    38,    12,    13,    14,
      15,    16,    17,    18,   419,   422,    19,   424,   425,   428,
     423,   436,    20,   500,   179,   437,   110,   111,   112,   113,
     114,   115,   441,   180,   116,    28,   438,   439,   440,   773,
     442,   445,   776,   447,   448,   449,   451,   461,   452,   782,
     453,   784,   785,   786,   455,   456,   457,  1153,   791,   458,
     793,   794,   795,   459,   797,   798,   799,   443,   801,   802,
     803,   804,   805,   806,   460,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   830,   831,   832,
     833,   834,   835,   836,   837,   838,   839,   840,   841,   842,
     843,   844,   845,   462,   463,   110,   111,   112,   113,   114,
     115,    37,   464,   116,   857,   858,   465,   466,   861,   467,
     468,   469,   470,   867,   868,   471,   472,   473,   474,  1103,
     873,   874,   110,   111,   112,   113,   114,   115,   475,   476,
     116,   477,   478,   479,   480,   481,   721,   110,   111,   112,
     113,   114,   115,   482,   483,   116,   484,   789,   110,   111,
     112,   113,   114,   115,   485,   486,   116,   487,   488,   489,
     490,   491,   889,   492,   493,   494,   495,   496,   497,   506,
     110,   111,   112,   113,   114,   115,   498,   499,   116,   931,
     932,   933,   507,   508,   936,   937,   938,   939,   940,   941,
     510,   511,   513,   945,   514,   519,   520,   523,   950,   524,
     890,   525,   953,   527,   528,   526,   529,   532,   959,   587,
     961,   534,   963,   964,   965,   535,   967,   968,   969,   536,
     971,   972,   538,   539,   540,   542,   977,   543,   979,   980,
     981,   982,   544,   545,   985,   986,   110,   111,   112,   113,
     114,   115,   546,   591,   116,   547,   110,   111,   112,   113,
     114,   115,   548,   549,   116,   110,   111,   112,   113,   114,
     115,   550,   551,   116,   552,   553,   554,  1017,  1018,   592,
     555,   110,   111,   112,   113,   114,   115,  1027,   891,   116,
     593,   556,   557,   602,  1031,   606,   607,  1034,  1035,  1036,
     110,   111,   112,   113,   114,   115,   608,   609,   116,   610,
      37,   611,   612,   672,   686,   679,  1053,   110,   111,   112,
     113,   114,   115,   689,   690,   116,   892,   616,   618,   622,
    1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,
    1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,
    1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,   110,   111,
     112,   113,   114,   115,   683,   626,   116,   696,  1104,  1105,
      37,  1107,  1108,  1109,   692,  1110,   697,   698,   699,   700,
    1115,  1116,   615,  1117,  1118,  1119,  1120,  1121,  1122,  1123,
    1124,  1125,  1126,  1127,  1128,  1129,   701,  1131,  1132,  1133,
     731,  1135,  1136,  1137,   704,  1139,   706,  1141,  1142,  1143,
     110,   111,   112,   113,   114,   115,   710,   714,   116,   732,
     733,   734,   735,   736,   737,   738,   760,   110,   111,   112,
     113,   114,   115,   739,   740,   116,   741,   742,  1170,   743,
    1172,  1173,  1174,   744,  1176,  1177,     2,     3,     4,   745,
       5,     6,     7,   746,  1185,     8,   747,   748,     9,   749,
      10,   750,    11,  1024,   751,   752,   753,   754,   755,    12,
      13,    14,    15,    16,    17,    18,   761,   756,    19,   757,
     758,   759,   762,  1213,    20,   763,    21,    22,    23,    24,
     770,   771,    25,   772,    26,   764,    27,    28,   774,   894,
     110,   111,   112,   113,   114,   115,   775,   777,   116,   780,
     781,   783,   779,   788,   899,   787,   846,    29,    30,    31,
      32,   790,   792,  1251,   796,   900,   800,   807,   847,  1257,
    1258,  1259,    33,   848,  1262,  1263,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,  1271,  1272,  1273,  1274,  1061,   849,    34,
     850,   851,   852,   853,   854,   855,   865,   856,  1286,  1287,
    1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,
    1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,
    1308,  1309,  1310,  1311,  1312,  1313,   860,  1315,   866,   869,
     875,   876,  1320,  1321,   878,  1323,  1324,  1325,  1326,   893,
     882,   886,   935,  1331,  1332,   934,   110,   111,   112,   113,
     114,   115,   944,  1209,   116,   947,   949,   954,   951,  1347,
    1348,   948,  1349,  1210,   955,  1352,   956,   960,  1355,   962,
     974,   975,  1212,   110,   111,   112,   113,   114,   115,   966,
     970,   116,   110,   111,   112,   113,   114,   115,  1707,   973,
     116,   983,   976,   110,   111,   112,   113,   114,   115,  1386,
    1387,   116,  1389,   978,   987,   984,   988,  1710,   989,   990,
    1395,   991,   992,   993,   994,  1207,  1400,  1401,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
     110,   111,   112,   113,   114,   115,   995,  1421,   116,  1423,
    1424,  1063,   110,   111,   112,   113,   114,   115,   996,   997,
     116,   110,   111,   112,   113,   114,   115,   998,   999,   116,
    1000,  1001,  1002,  1003,  1004,  1715,  1005,  1006,  1451,   110,
     111,   112,   113,   114,   115,  1457,  1007,   116,  1008,  1461,
    1462,  1463,   110,   111,   112,   113,   114,   115,  1009,  1010,
     116,   110,   111,   112,   113,   114,   115,  1011,  1012,   116,
    1482,  1483,  1013,  1485,  1587,  1014,  1025,  1026,  1490,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1717,  1028,  1029,  1030,  1521,  1522,
    1032,  1524,  1525,  1037,  1038,  1054,  1040,  1527,  1528,  1529,
    1530,  1531,  1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,
    1540,  1042,  1542,  1543,  1544,  1055,  1056,  1547,  1046,  1050,
    1550,  1551,  1552,  1058,  1064,  1095,  1096,  1098,  1099,  1100,
      35,    36,    37,  1101,  1102,  1111,  1134,  1138,    38,  1140,
     181,  1171,   110,   111,   112,   113,   114,   115,  1175,  1579,
     116,  1581,  1582,  1180,  1186,  1187,  1188,  1724,  1204,  1588,
     110,   111,   112,   113,   114,   115,  1211,  1217,   116,  1278,
    1218,  1219,  1220,  1221,  1602,   110,   111,   112,   113,   114,
     115,  1222,  1609,   116,  1223,  1224,  1275,   110,   111,   112,
     113,   114,   115,  1225,  1226,   116,   110,   111,   112,   113,
     114,   115,  1227,  1228,   116,  1280,   110,   111,   112,   113,
     114,   115,  1229,  1230,   116,  1231,  1232,  1233,  1234,  1646,
    1235,  1236,  1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,
    1656,  1657,  1658,  1659,  1660,  1237,  1238,  1239,  1240,  1665,
    1666,  1667,  1241,  1574,  1669,  1670,  1671,  1672,  1673,  1674,
    1675,  1676,  1677,  1678,  1679,  1680,  1681,  1682,  1683,  1684,
    1685,  1686,  1687,  1688,  1689,  1690,  1691,  1692,  1693,  1694,
    1695,  1696,  1697,  1242,  1699,  1249,  1250,  1702,  1703,  1252,
    1705,   110,   111,   112,   113,   114,   115,  1253,  1254,   116,
    2009,  1255,  1260,  1261,  1276,  1720,  1721,  1722,  1277,  2046,
    1279,  1723,  1281,   110,   111,   112,   113,   114,   115,  1283,
    2094,   116,   110,   111,   112,   113,   114,   115,  1282,  1285,
     116,   110,   111,   112,   113,   114,   115,  1314,  1316,   116,
     693,  1752,  1317,  1753,  1754,  1318,  1319,  1327,  1346,  1758,
    1350,  1760,  1761,  1354,  1763,  1764,  1765,  1766,  1351,  1768,
    1353,  1770,  1771,  1384,  1392,  1388,  1396,  1397,  1777,  2156,
    1417,   110,   111,   112,   113,   114,   115,  1398,  2191,   116,
    1418,  1420,   110,   111,   112,   113,   114,   115,  1425,  1426,
     116,  1580,  1427,  1428,  1803,  1429,  2429,  1430,  1431,  1432,
    1809,  1698,   110,   111,   112,   113,   114,   115,  1433,  2430,
     116,   110,   111,   112,   113,   114,   115,  1434,  2431,   116,
    1435,  1830,  1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,
    1849,  1850,  1851,  1852,  1853,  1854,  1436,  1856,  1437,  1438,
    1859,  1860,  1439,  1862,  1440,  1864,  1865,  1441,  1867,  1868,
    1869,  1870,  1442,  1872,  1443,  1874,  1444,  1876,  1877,  1878,
    1445,  1446,  1881,  1447,  1863,   110,   111,   112,   113,   114,
     115,  1448,  1449,   116,  1450,  1455,  1456,  1759,   110,   111,
     112,   113,   114,   115,  1458,  1459,   116,  1464,  1905,  1479,
    1907,  1908,  1480,  1481,  1484,  1487,  1913,  1518,  1519,  1916,
    1520,  1526,  1541,  1549,  1921,  1586,  1923,  1606,  1607,  2451,
    1608,  1583,  1929,  1930,  1613,  1762,   110,   111,   112,   113,
     114,   115,  1614,  1615,   116,  1616,  1617,  2552,  1767,   110,
     111,   112,   113,   114,   115,  1618,  1619,   116,  1620,  1957,
    1621,  1622,  2559,  1623,  1961,  1962,  1624,  1963,  1964,  1625,
    1966,  1967,  1968,  1969,  2561,  1971,  1626,  1972,  1973,  1394,
    1627,  1628,  1629,  2562,  1978,  1979,  1980,  1981,  1982,  1983,
    1984,  1985,  1986,  1987,  1988,  1989,  1990,  1991,  1992,  1993,
    1994,  1995,  1996,  1997,  1998,  1999,  2000,  2001,  2002,  1630,
    1631,  1632,  2006,  2007,   110,   111,   112,   113,   114,   115,
    1633,  1634,   116,  1635,  1636,  1637,  1638,  2021,  2022,  2023,
    2024,   110,   111,   112,   113,   114,   115,  1644,  1645,   116,
     110,   111,   112,   113,   114,   115,  1647,  1662,   116,  1663,
     429,   110,   111,   112,   113,   114,   115,  2049,  2050,   116,
    1664,  1668,  1700,  2055,  2056,  2057,  2058,  2059,  2060,  2061,
    2062,  2063,  2064,  2065,  1701,  1719,  1757,  1769,  2070,   110,
     111,   112,   113,   114,   115,  1778,  1779,   116,  1780,  1776,
    2626,  1811,   110,   111,   112,   113,   114,   115,  1781,  2627,
     116,  2093,  1782,  1783,  1784,  1785,  2097,  1786,  2628,  1787,
    1788,  1789,  1790,  1791,  1814,   110,   111,   112,   113,   114,
     115,  1792,  2113,   116,  2115,  2116,  2117,  2118,  2119,  2120,
    2121,  2122,  2123,  2124,  2125,  2126,  2127,  2128,  2129,  2130,
    2131,  2132,  2133,  2134,  1793,  2137,  1794,   705,  2138,  2139,
    1795,  1796,  2142,  2143,  1797,  2145,  2146,  2147,  2148,  2632,
    2150,  2151,  2152,  2153,  2154,  2155,  1798,  2157,   110,   111,
     112,   113,   114,   115,  1799,  1800,   116,  1801,  1802,  2644,
    1807,  1819,   110,   111,   112,   113,   114,   115,  2671,  2175,
     116,  1808,  1821,  1824,  2180,  1825,  1826,  2183,  1827,  1857,
    1828,  1858,  2188,  1866,   110,   111,   112,   113,   114,   115,
    1871,  1873,   116,  1875,  1879,   110,   111,   112,   113,   114,
     115,  1880,  1911,   116,  1932,  1933,  1934,  2212,  1935,  2213,
    1936,  1925,  2216,  1937,  2217,  2218,   952,  2220,  2221,  2222,
    2223,  1938,  2225,  2226,  1939,  1940,  1941,  1942,  2231,  2232,
    2233,  2234,  2235,  2236,  2237,  2238,  2239,  2240,  2241,  2242,
    2243,  2244,  2245,  2246,  2247,  2747,  1943,  2250,  2251,   110,
     111,   112,   113,   114,   115,  1944,  1945,   116,  1946,  1947,
    1948,  2003,   110,   111,   112,   113,   114,   115,  1949,  1950,
     116,   110,   111,   112,   113,   114,   115,  1951,  1952,   116,
    1953,  2277,  2278,  2748,  1954,  1959,  2282,  2283,  2284,  2285,
    2286,  2287,  2288,  2289,  2290,  2291,  2749,  2293,  2294,  2295,
    2296,  1960,  2091,   110,   111,   112,   113,   114,   115,  1974,
    1975,   116,  1976,  2005,  2310,  2012,  2017,  2053,  2054,   110,
     111,   112,   113,   114,   115,  2321,  2071,   116,  2072,  2073,
    2326,  2327,  2328,  2329,  2330,  2331,  2332,  2333,  2334,  2335,
    2336,  2337,  2338,  2074,  2340,  2341,  2075,  2343,  2344,  2345,
    2346,  2076,  2348,  2349,  2077,  2351,  2352,  2353,  2354,  2078,
    2079,  2762,  2080,  2092,   110,   111,   112,   113,   114,   115,
    2081,  2082,   116,  2083,  2084,  2368,  2085,  2086,  2765,  2087,
    2373,  2088,  2089,  2376,  2090,  2095,  2096,  2766,   110,   111,
     112,   113,   114,   115,  2387,  2388,   116,   707,  2391,  2392,
    2393,  2110,  2395,  2396,  2397,  2398,  2111,  2400,  2401,  2112,
    2403,  2404,  2405,  2406,  2407,  2408,  2409,  2410,  2411,  2412,
    2098,  2141,  2415,  2416,  2144,  2149,  2806,  2174,  2227,   110,
     111,   112,   113,   114,   115,  2428,  2179,   116,  2196,  2808,
     110,   111,   112,   113,   114,   115,  2436,  2437,   116,  2197,
    2198,  2441,  2442,  2443,  2444,  2445,  2446,  2447,  2448,  2449,
    2450,   708,  2199,  2200,  2201,  2455,   110,   111,   112,   113,
     114,   115,  2202,  2463,   116,   110,   111,   112,   113,   114,
     115,  2203,  2204,   116,  2205,  2206,  2207,  2208,  2209,  2479,
    2480,  2481,  2482,  2483,  2484,  2485,  2210,  2487,  2488,  2211,
    2490,  2491,  2492,  2493,  2214,  2495,  2496,  2215,  2228,  2229,
    2230,  2501,  2255,  2260,   709,   110,   111,   112,   113,   114,
     115,  2263,  2512,   116,  2281,  2264,  2297,  2517,   711,  2265,
    2266,  2521,  2522,  2523,  2292,  2298,  2299,  2300,  2528,  2529,
    2301,  2302,  2532,  2533,  2534,  2303,  2536,  2537,  2538,  2539,
     712,  2541,  2542,  2543,  2304,  2305,  2308,  2547,  2548,  2549,
    2550,   713,  2309,  2553,  2554,   110,   111,   112,   113,   114,
     115,  2342,  2347,   116,  2365,  2381,  2382,  2567,  2568,  2569,
    2383,  2384,  2385,  2386,  2573,  2389,  2390,  2420,  2425,  2576,
    2577,  2578,  2440,  2580,  2456,  2452,  2583,  2584,  2585,  2586,
    2453,  2454,  2457,  2590,  2458,  2461,  2462,  2474,  2595,  2475,
    2476,  2477,  2489,  2494,  2497,   715,  2498,  1379,  2499,  2500,
    2607,  2608,  2609,  2509,  2611,  2612,  2613,  2614,   716,  2615,
    2616,  2520,  2525,  2619,  2526,  2527,  2530,  2623,  2624,  2625,
    2531,  2558,  2563,  2629,   110,   111,   112,   113,   114,   115,
    2636,  2637,   116,  2639,  2640,  2641,   110,   111,   112,   113,
     114,   115,  2575,  2591,   116,  2579,  2581,  2652,  2653,   717,
    2582,  2656,  2592,  2594,  2599,  2658,  2659,  2601,  2602,  2617,
    2622,  2663,  2664,  2665,  2666,   718,   110,   111,   112,   113,
     114,   115,  2618,  2620,   116,  2621,  2645,  2646,  2649,  2647,
    2681,  2682,  2683,  2684,  2650,  2686,  2687,  2688,  2689,  2690,
    2691,  2692,   110,   111,   112,   113,   114,   115,  2651,  2660,
     116,  2661,  2662,  2669,  2670,  2672,  2705,  2706,  2707,  2708,
     719,  2675,  2711,  2712,  2713,  2714,  2715,  2716,  2676,  2685,
    2693,  2677,  2721,  2722,  2694,  2709,  2729,  2730,  2710,  2744,
    2698,  2759,  2731,  2732,   720,  2760,  2734,  2735,  2736,  2737,
    2738,  2739,  2740,  2741,   110,   111,   112,   113,   114,   115,
    2770,  2771,   116,  2783,  2773,  2753,  2754,  2755,  2756,  2757,
    2774,  2778,  2781,  2758,  2782,  2784,  2786,  2789,  2790,  2795,
    2804,  2805,  2767,  2768,  2769,   722,  2812,  2772,  2816,  2818,
    2819,  2820,  2811,  2813,  2779,  2821,   723,  2823,  2827,  2832,
    2785,  2833,  2837,  2788,  2841,  2842,  2791,  2792,  2846,  2794,
    2850,  2851,  2797,  2798,  2855,  2858,  2801,  2861,  2802,  2803,
    2864,  2867,   724,   224,     0,     0,     0,  2809,  2810,     0,
       0,   725,     0,     0,     0,  2817,     0,     0,     0,     0,
       0,     0,  2824,  2825,  2826,     0,  2828,     0,     0,     0,
    2831,     0,     0,     0,     0,  2835,  2836,     0,     0,     0,
    2840,     0,     0,     0,  2844,  2845,     0,     0,     0,  2849,
       0,   726,     0,  2852,  2853,     0,     0,     0,  2856,     0,
       0,  2859,     0,     0,  2862,     0,     0,  2865,     0,     0,
    2868,     2,     3,     4,     0,     5,     6,     7,     0,     0,
       8,     0,     0,     9,     0,    10,     0,    11,     0,     0,
       0,     0,     0,     0,    12,    13,    14,    15,    16,    17,
      18,   727,     0,    19,     0,     0,     0,     0,     0,    20,
       0,    21,    22,    23,    24,     0,     0,    25,     0,    26,
       0,    27,    28,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,    29,    30,    31,    32,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,    33,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   110,   111,   112,
     113,   114,   115,     0,    34,   116,     0,     0,     0,     0,
     728,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,   729,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,   730,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   110,   111,   112,   113,   114,   115,     0,   877,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,     0,     0,
     879,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,    35,    36,    37,     0,     0,
       0,     0,     0,    38,   110,   111,   112,   113,   114,   115,
       0,     0,   116,   110,   111,   112,   113,   114,   115,   880,
       0,   116,   110,   111,   112,   113,   114,   115,   881,     0,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,   883,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,   884,     0,   110,   111,   112,   113,
     114,   115,     0,   885,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,   887,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,   888,
       0,   110,   111,   112,   113,   114,   115,     0,   895,   116,
     110,   111,   112,   113,   114,   115,     0,   896,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,   897,
       0,   110,   111,   112,   113,   114,   115,     0,   898,   116,
     110,   111,   112,   113,   114,   115,     0,   901,   116,   110,
     111,   112,   113,   114,   115,     0,   902,   116,   110,   111,
     112,   113,   114,   115,     0,  1041,   116,   110,   111,   112,
     113,   114,   115,     0,  1043,   116,   110,   111,   112,   113,
     114,   115,     0,  1044,   116,   110,   111,   112,   113,   114,
     115,     0,  1045,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,     0,  1047,     0,   110,
     111,   112,   113,   114,   115,     0,  1048,   116,   110,   111,
     112,   113,   114,   115,     0,  1049,   116,   110,   111,   112,
     113,   114,   115,     0,  1051,   116,   110,   111,   112,   113,
     114,   115,     0,  1052,   116,   110,   111,   112,   113,   114,
     115,     0,  1059,   116,   110,   111,   112,   113,   114,   115,
       0,  1060,   116,   110,   111,   112,   113,   114,   115,     0,
    1062,   116,   110,   111,   112,   113,   114,   115,     0,  1065,
     116,   110,   111,   112,   113,   114,   115,     0,  1066,   116,
     110,   111,   112,   113,   114,   115,     0,  1097,   116,   110,
     111,   112,   113,   114,   115,     0,  1191,   116,   110,   111,
     112,   113,   114,   115,     0,  1192,   116,   110,   111,   112,
     113,   114,   115,     0,  1193,   116,   110,   111,   112,   113,
     114,   115,     0,  1194,   116,   110,   111,   112,   113,   114,
     115,     0,  1195,   116,   110,   111,   112,   113,   114,   115,
       0,  1196,   116,   110,   111,   112,   113,   114,   115,     0,
    1197,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
    1198,     0,   110,   111,   112,   113,   114,   115,     0,  1199,
     116,   110,   111,   112,   113,   114,   115,     0,  1200,   116,
     110,   111,   112,   113,   114,   115,     0,  1201,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,  1202,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,  1203,     0,   110,   111,   112,   113,   114,   115,
       0,  1215,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  1216,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,  1244,     0,   110,
     111,   112,   113,   114,   115,     0,  1333,   116,   110,   111,
     112,   113,   114,   115,     0,  1334,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,  1335,     0,   110,
     111,   112,   113,   114,   115,     0,  1336,   116,   110,   111,
     112,   113,   114,   115,     0,  1337,   116,   110,   111,   112,
     113,   114,   115,     0,  1338,   116,   110,   111,   112,   113,
     114,   115,     0,  1339,   116,   110,   111,   112,   113,   114,
     115,     0,  1340,   116,   110,   111,   112,   113,   114,   115,
       0,  1341,   116,   110,   111,   112,   113,   114,   115,     0,
    1342,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,  1343,     0,   110,   111,   112,
     113,   114,   115,     0,  1344,   116,   110,   111,   112,   113,
     114,   115,     0,  1345,   116,   110,   111,   112,   113,   114,
     115,     0,  1356,   116,   110,   111,   112,   113,   114,   115,
       0,  1357,   116,   110,   111,   112,   113,   114,   115,     0,
    1385,   116,   110,   111,   112,   113,   114,   115,     0,  1465,
     116,   110,   111,   112,   113,   114,   115,     0,  1466,   116,
     110,   111,   112,   113,   114,   115,     0,  1467,   116,   110,
     111,   112,   113,   114,   115,     0,  1468,   116,   110,   111,
     112,   113,   114,   115,     0,  1469,   116,   110,   111,   112,
     113,   114,   115,     0,  1470,   116,   110,   111,   112,   113,
     114,   115,     0,  1471,   116,   110,   111,   112,   113,   114,
     115,     0,  1472,   116,   110,   111,   112,   113,   114,   115,
       0,  1473,   116,   110,   111,   112,   113,   114,   115,     0,
    1474,   116,   110,   111,   112,   113,   114,   115,     0,  1475,
     116,   110,   111,   112,   113,   114,   115,     0,  1476,   116,
     110,   111,   112,   113,   114,   115,     0,  1477,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,  1488,     0,
     110,   111,   112,   113,   114,   115,     0,  1489,   116,   110,
     111,   112,   113,   114,   115,     0,  1589,   116,   110,   111,
     112,   113,   114,   115,     0,  1590,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,  1591,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
    1592,     0,   110,   111,   112,   113,   114,   115,     0,  1593,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,  1594,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,  1595,     0,   110,   111,   112,
     113,   114,   115,     0,  1596,   116,   110,   111,   112,   113,
     114,   115,     0,  1597,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,  1598,     0,   110,   111,   112,
     113,   114,   115,     0,  1599,   116,   110,   111,   112,   113,
     114,   115,     0,  1600,   116,   110,   111,   112,   113,   114,
     115,     0,  1601,   116,   110,   111,   112,   113,   114,   115,
       0,  1611,   116,   110,   111,   112,   113,   114,   115,     0,
    1612,   116,   110,   111,   112,   113,   114,   115,     0,  1639,
     116,   110,   111,   112,   113,   114,   115,     0,  1706,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,     0,  1708,     0,   110,   111,   112,   113,   114,
     115,     0,  1709,   116,   110,   111,   112,   113,   114,   115,
       0,  1711,   116,   110,   111,   112,   113,   114,   115,     0,
    1712,   116,   110,   111,   112,   113,   114,   115,     0,  1713,
     116,   110,   111,   112,   113,   114,   115,     0,  1714,   116,
     110,   111,   112,   113,   114,   115,     0,  1716,   116,   110,
     111,   112,   113,   114,   115,     0,  1718,   116,   110,   111,
     112,   113,   114,   115,     0,  1725,   116,   110,   111,   112,
     113,   114,   115,     0,  1751,   116,   110,   111,   112,   113,
     114,   115,     0,  1804,   116,   110,   111,   112,   113,   114,
     115,     0,  1810,   116,   110,   111,   112,   113,   114,   115,
       0,  1812,   116,   110,   111,   112,   113,   114,   115,     0,
    1813,   116,   110,   111,   112,   113,   114,   115,     0,  1815,
     116,   110,   111,   112,   113,   114,   115,     0,  1816,   116,
     110,   111,   112,   113,   114,   115,     0,  1817,   116,   110,
     111,   112,   113,   114,   115,     0,  1818,   116,   110,   111,
     112,   113,   114,   115,     0,  1820,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,  1822,     0,   110,   111,
     112,   113,   114,   115,     0,  1829,   116,   110,   111,   112,
     113,   114,   115,     0,  1855,   116,   110,   111,   112,   113,
     114,   115,     0,  1906,   116,     0,   110,   111,   112,   113,
     114,   115,     0,  1912,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,     0,  1914,     0,
     110,   111,   112,   113,   114,   115,     0,  1915,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,  1917,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
       0,     0,     0,  1918,     0,   110,   111,   112,   113,   114,
     115,     0,  1919,   116,   110,   111,   112,   113,   114,   115,
       0,  1920,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  1922,     0,   110,   111,   112,   113,   114,
     115,     0,  1924,   116,   110,   111,   112,   113,   114,   115,
       0,  1931,   116,   110,   111,   112,   113,   114,   115,     0,
    1977,   116,   110,   111,   112,   113,   114,   115,     0,  2004,
     116,   110,   111,   112,   113,   114,   115,     0,  2010,   116,
     110,   111,   112,   113,   114,   115,     0,  2011,   116,   110,
     111,   112,   113,   114,   115,     0,  2013,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,  2014,     0,   110,   111,   112,   113,   114,   115,     0,
    2015,   116,   110,   111,   112,   113,   114,   115,     0,  2016,
     116,   110,   111,   112,   113,   114,   115,     0,  2018,   116,
     110,   111,   112,   113,   114,   115,     0,  2019,   116,   110,
     111,   112,   113,   114,   115,     0,  2020,   116,   110,   111,
     112,   113,   114,   115,     0,  2025,   116,   110,   111,   112,
     113,   114,   115,     0,  2069,   116,   110,   111,   112,   113,
     114,   115,     0,  2099,   116,   110,   111,   112,   113,   114,
     115,     0,  2100,   116,   110,   111,   112,   113,   114,   115,
       0,  2102,   116,   110,   111,   112,   113,   114,   115,     0,
    2103,   116,   110,   111,   112,   113,   114,   115,     0,  2104,
     116,   110,   111,   112,   113,   114,   115,     0,  2105,   116,
     110,   111,   112,   113,   114,   115,     0,  2107,   116,   110,
     111,   112,   113,   114,   115,     0,  2108,   116,   110,   111,
     112,   113,   114,   115,     0,  2109,   116,   110,   111,   112,
     113,   114,   115,     0,  2114,   116,   110,   111,   112,   113,
     114,   115,     0,  2176,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,  2181,     0,   110,   111,   112,   113,
     114,   115,     0,  2182,   116,   110,   111,   112,   113,   114,
     115,     0,  2184,   116,     0,   110,   111,   112,   113,   114,
     115,     0,  2185,   116,   110,   111,   112,   113,   114,   115,
       0,  2186,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,     0,     0,  2187,     0,   110,   111,
     112,   113,   114,   115,     0,  2189,   116,     0,   110,   111,
     112,   113,   114,   115,     0,  2190,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,  2195,     0,   110,   111,   112,   113,   114,   115,     0,
    2248,   116,   110,   111,   112,   113,   114,   115,     0,  2249,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,  2253,     0,   110,   111,   112,   113,   114,   115,     0,
    2254,   116,   110,   111,   112,   113,   114,   115,     0,  2256,
     116,   110,   111,   112,   113,   114,   115,     0,  2257,   116,
     110,   111,   112,   113,   114,   115,     0,  2258,   116,   110,
     111,   112,   113,   114,   115,     0,  2259,   116,   110,   111,
     112,   113,   114,   115,     0,  2261,   116,   110,   111,   112,
     113,   114,   115,     0,  2262,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,     0,  2267,
       0,   110,   111,   112,   113,   114,   115,     0,  2306,   116,
     110,   111,   112,   113,   114,   115,     0,  2307,   116,   110,
     111,   112,   113,   114,   115,     0,  2311,   116,   110,   111,
     112,   113,   114,   115,     0,  2312,   116,   110,   111,   112,
     113,   114,   115,     0,  2314,   116,   110,   111,   112,   113,
     114,   115,     0,  2315,   116,   110,   111,   112,   113,   114,
     115,     0,  2316,   116,   110,   111,   112,   113,   114,   115,
       0,  2317,   116,   110,   111,   112,   113,   114,   115,     0,
    2319,   116,   110,   111,   112,   113,   114,   115,     0,  2320,
     116,   110,   111,   112,   113,   114,   115,     0,  2322,   116,
     110,   111,   112,   113,   114,   115,     0,  2323,   116,   110,
     111,   112,   113,   114,   115,     0,  2324,   116,   110,   111,
     112,   113,   114,   115,     0,  2325,   116,   110,   111,   112,
     113,   114,   115,     0,  2350,   116,   110,   111,   112,   113,
     114,   115,     0,  2361,   116,   110,   111,   112,   113,   114,
     115,     0,  2362,   116,   110,   111,   112,   113,   114,   115,
       0,  2366,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,  2367,     0,   110,   111,   112,   113,   114,   115,
       0,  2369,   116,     0,   110,   111,   112,   113,   114,   115,
       0,  2370,   116,   110,   111,   112,   113,   114,   115,     0,
    2371,   116,   110,   111,   112,   113,   114,   115,     0,  2372,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,     0,     0,  2374,   110,   111,   112,   113,   114,
     115,     0,     0,   116,  2375,     0,   110,   111,   112,   113,
     114,   115,     0,  2377,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,     0,     0,  2378,
       0,   110,   111,   112,   113,   114,   115,     0,  2379,   116,
     110,   111,   112,   113,   114,   115,     0,  2380,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,  2402,
       0,   110,   111,   112,   113,   114,   115,     0,  2413,   116,
     110,   111,   112,   113,   114,   115,     0,  2414,   116,   110,
     111,   112,   113,   114,   115,     0,  2418,   116,   110,   111,
     112,   113,   114,   115,     0,  2419,   116,   110,   111,   112,
     113,   114,   115,     0,  2421,   116,   110,   111,   112,   113,
     114,   115,     0,  2422,   116,   110,   111,   112,   113,   114,
     115,     0,  2423,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,     0,  2424,     0,   110,
     111,   112,   113,   114,   115,     0,  2426,   116,   110,   111,
     112,   113,   114,   115,     0,  2427,   116,   110,   111,   112,
     113,   114,   115,     0,  2432,   116,   110,   111,   112,   113,
     114,   115,     0,  2459,   116,   110,   111,   112,   113,   114,
     115,     0,  2460,   116,   110,   111,   112,   113,   114,   115,
       0,  2464,   116,   110,   111,   112,   113,   114,   115,     0,
    2465,   116,   110,   111,   112,   113,   114,   115,     0,  2467,
     116,   110,   111,   112,   113,   114,   115,     0,  2468,   116,
     110,   111,   112,   113,   114,   115,     0,  2469,   116,   110,
     111,   112,   113,   114,   115,     0,  2470,   116,   110,   111,
     112,   113,   114,   115,     0,  2472,   116,   110,   111,   112,
     113,   114,   115,     0,  2473,   116,   110,   111,   112,   113,
     114,   115,     0,  2478,   116,   110,   111,   112,   113,   114,
     115,     0,  2505,   116,   110,   111,   112,   113,   114,   115,
       0,  2506,   116,   110,   111,   112,   113,   114,   115,     0,
    2510,   116,   110,   111,   112,   113,   114,   115,     0,  2511,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
    2513,   110,   111,   112,   113,   114,   115,     0,     0,   116,
    2514,     0,   110,   111,   112,   113,   114,   115,     0,  2515,
     116,   110,   111,   112,   113,   114,   115,     0,  2516,   116,
     110,   111,   112,   113,   114,   115,     0,  2518,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
       0,  2519,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  2524,     0,   110,   111,   112,   113,   114,   115,
       0,  2544,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,     0,     0,     0,  2545,     0,   110,
     111,   112,   113,   114,   115,     0,  2546,   116,   110,   111,
     112,   113,   114,   115,     0,  2551,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,  2556,     0,   110,
     111,   112,   113,   114,   115,     0,  2557,   116,   110,   111,
     112,   113,   114,   115,     0,  2560,   116,   110,   111,   112,
     113,   114,   115,     0,  2564,   116,   110,   111,   112,   113,
     114,   115,     0,  2565,   116,   110,   111,   112,   113,   114,
     115,     0,  2566,   116,   110,   111,   112,   113,   114,   115,
       0,  2570,   116,   110,   111,   112,   113,   114,   115,     0,
    2587,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,  2588,     0,   110,   111,   112,
     113,   114,   115,     0,  2589,   116,   110,   111,   112,   113,
     114,   115,     0,  2593,   116,   110,   111,   112,   113,   114,
     115,     0,  2596,   116,   110,   111,   112,   113,   114,   115,
       0,  2597,   116,   110,   111,   112,   113,   114,   115,     0,
    2600,   116,   110,   111,   112,   113,   114,   115,     0,  2604,
     116,   110,   111,   112,   113,   114,   115,     0,  2605,   116,
     110,   111,   112,   113,   114,   115,     0,  2606,   116,   110,
     111,   112,   113,   114,   115,     0,  2610,   116,   110,   111,
     112,   113,   114,   115,     0,  2634,   116,   110,   111,   112,
     113,   114,   115,     0,  2635,   116,   110,   111,   112,   113,
     114,   115,     0,  2638,   116,   110,   111,   112,   113,   114,
     115,     0,  2642,   116,   110,   111,   112,   113,   114,   115,
       0,  2643,   116,   110,   111,   112,   113,   114,   115,     0,
    2648,   116,   110,   111,   112,   113,   114,   115,     0,  2667,
     116,   110,   111,   112,   113,   114,   115,     0,  2668,   116,
     110,   111,   112,   113,   114,   115,     0,  2673,   116,     0,
     110,   111,   112,   113,   114,   115,     0,  2674,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,  2678,     0,
     110,   111,   112,   113,   114,   115,     0,  2695,   116,   110,
     111,   112,   113,   114,   115,     0,  2696,   116,   110,   111,
     112,   113,   114,   115,     0,  2699,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,  2700,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
    2701,     0,   110,   111,   112,   113,   114,   115,     0,  2702,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,     0,     0,     0,  2703,     0,   110,   111,   112,
     113,   114,   115,     0,  2704,   116,     0,     0,    70,    71,
      72,    73,    74,  2719,    75,    76,     0,    77,    78,     0,
       0,    79,     0,    80,     0,  2720,     0,    81,   110,   111,
     112,   113,   114,   115,  2723,     0,   116,   110,   111,   112,
     113,   114,   115,  2724,     0,   116,   110,   111,   112,   113,
     114,   115,  2725,     0,   116,   110,   111,   112,   113,   114,
     115,  2726,     0,   116,     0,     0,    82,     0,     0,     0,
    2727,     0,   110,   111,   112,   113,   114,   115,     0,  2728,
     116,   110,   111,   112,   113,   114,   115,     0,  2742,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,     0,  2743,     0,   110,   111,   112,   113,   114,
     115,     0,  2745,   116,   110,   111,   112,   113,   114,   115,
       0,  2746,   116,   110,   111,   112,   113,   114,   115,     0,
    2750,   116,     0,     0,     0,     0,    83,     0,     0,  2761,
       0,   110,   111,   112,   113,   114,   115,     0,  2764,   116,
     110,   111,   112,   113,   114,   115,     0,  2777,   116,   110,
     111,   112,   113,   114,   115,     0,  2780,   116,     0,     0,
       0,     0,  1165,     0,     0,  2793,     0,     0,     0,     0,
    1745,     0,     0,     0,  2796,     0,     0,     0,     0,  2171,
     110,   111,   112,   113,   114,   115,     0,  1248,   116,   110,
     111,   112,   113,   114,   115,     0,  1573,   116,   110,   111,
     112,   113,   114,   115,     0,  2042,   116,   110,   111,   112,
     113,   114,   115,  1022,     0,   116,   110,   111,   112,   113,
     114,   115,  1023,     0,   116,   110,   111,   112,   113,   114,
     115,     0,  1378,   116,   110,   111,   112,   113,   114,   115,
       0,  1900,   116,     0,   110,   111,   112,   113,   114,   115,
       0,  1021,   116,   110,   111,   112,   113,   114,   115,     0,
    1247,   116,   110,   111,   112,   113,   114,   115,     0,  1454,
     116,   110,   111,   112,   113,   114,   115,   767,     0,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   768,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,   769,     0,   110,   111,   112,   113,   114,   115,
       0,  1164,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,     0,     0,    84,  1746,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,  1154,
       0,   110,   111,   112,   113,   114,   115,     0,  1377,   116,
     110,   111,   112,   113,   114,   115,  1367,     0,   116,   110,
     111,   112,   113,   114,   115,  1368,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
     521,   110,   111,   112,   113,   114,   115,   431,     0,   116,
       0,     0,     0,     0,     0,   694,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,     0,   778,
     110,   111,   112,   113,   114,   115,     0,   516,   116,   110,
     111,   112,   113,   114,   115,   241,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,     0,  2172,     0,   110,   111,   112,   113,   114,   115,
       0,  2173,   116,   110,   111,   112,   113,   114,   115,  1747,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
    1749,     0,   110,   111,   112,   113,   114,   115,     0,  1750,
     116,   110,   111,   112,   113,   114,   115,  1575,     0,   116,
     110,   111,   112,   113,   114,   115,  1577,     0,   116,   110,
     111,   112,   113,   114,   115,  1578,     0,   116,     0,     0,
       0,     0,     0,  1380,     0,   110,   111,   112,   113,   114,
     115,     0,  1382,   116,     0,   110,   111,   112,   113,   114,
     115,     0,  1383,   116,   110,   111,   112,   113,   114,   115,
    1166,     0,   116,   110,   111,   112,   113,   114,   115,  1168,
       0,   116,   110,   111,   112,   113,   114,   115,  1169,     0,
     116,     0,     0,     0,     0,     0,   926,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   928,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
     929,   110,   111,   112,   113,   114,   115,     0,  1744,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,  1572,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  1163,   110,   111,   112,   113,   114,
     115,     0,   923,   116,   110,   111,   112,   113,   114,   115,
     663,     0,   116,     0,     0,     0,     0,   924,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
     925,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,  1735,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,     0,     0,  1736,   110,   111,   112,
     113,   114,   115,     0,   913,   116,   110,   111,   112,   113,
     114,   115,     0,   914,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,     0,   653,     0,
     110,   111,   112,   113,   114,   115,     0,   654,   116,   110,
     111,   112,   113,   114,   115,  2752,     0,   116,     0,     0,
       0,     0,     0,  2718,   110,   111,   112,   113,   114,   115,
       0,  2680,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,  2631,   110,   111,   112,   113,   114,   115,     0,
    2572,   116,   110,   111,   112,   113,   114,   115,  2504,     0,
     116,     0,     0,     0,     0,     0,  2435,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,  2359,     0,   110,   111,   112,   113,   114,   115,     0,
    2274,   116,   110,   111,   112,   113,   114,   115,  2166,     0,
     116,     0,     0,     0,     0,     0,  2036,   110,   111,   112,
     113,   114,   115,     0,  1893,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,  1737,     0,
     110,   111,   112,   113,   114,   115,     0,  1564,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
     915,   110,   111,   112,   113,   114,   115,     0,   652,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,   655,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  1369,   110,   111,   112,   113,   114,   115,   912,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  2847,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  2838,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,  2829,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,  2814,   110,
     111,   112,   113,   114,   115,     0,  2799,   116,   110,   111,
     112,   113,   114,   115,  2775,     0,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,  2751,   110,
     111,   112,   113,   114,   115,     0,  2717,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
    2679,   110,   111,   112,   113,   114,   115,     0,  2630,   116,
     110,   111,   112,   113,   114,   115,     0,  2571,   116,   110,
     111,   112,   113,   114,   115,  2502,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,  2433,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
       0,     0,     0,     0,  2356,   110,   111,   112,   113,   114,
     115,     0,  2271,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,  2163,   110,   111,   112,
     113,   114,   115,     0,  2033,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,  1890,   110,   111,
     112,   113,   114,   115,     0,  1734,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,  1152,   110,   111,   112,
     113,   114,   115,     0,  1561,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,  1366,   110,   111,   112,   113,
     114,   115,  1891,     0,   116,   110,   111,   112,   113,   114,
     115,  1892,     0,   116,   110,   111,   112,   113,   114,   115,
       0,   922,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   662,   110,   111,   112,   113,   114,   115,     0,
       0,   116,  1571,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  1376,   110,   111,   112,   113,   114,
     115,     0,  1162,   116,   110,   111,   112,   113,   114,   115,
    1360,     0,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,  1361,   110,   111,   112,   113,   114,   115,     0,
    1359,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,  1364,     0,   110,   111,   112,   113,   114,
     115,     0,  1370,   116,   110,   111,   112,   113,   114,   115,
       0,  1371,   116,   110,   111,   112,   113,   114,   115,  1358,
       0,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,  1363,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,     0,  1374,     0,   110,   111,
     112,   113,   114,   115,     0,  1375,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,  1381,     0,
     110,   111,   112,   113,   114,   115,     0,  1554,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,  1555,
       0,   110,   111,   112,   113,   114,   115,     0,  1556,   116,
       0,   110,   111,   112,   113,   114,   115,     0,  1559,   116,
     110,   111,   112,   113,   114,   115,     0,  1565,   116,     0,
     110,   111,   112,   113,   114,   115,     0,  1566,   116,   110,
     111,   112,   113,   114,   115,     0,  1730,   116,   110,   111,
     112,   113,   114,   115,     0,  1733,   116,   110,   111,   112,
     113,   114,   115,     0,  1740,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  1741,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  2034,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,  2035,     0,   110,   111,
     112,   113,   114,   115,     0,  2043,   116,   110,   111,   112,
     113,   114,   115,     0,  2044,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  2045,   116,   110,   111,   112,   113,
     114,   115,     0,  2776,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,  1557,     0,   110,   111,   112,
     113,   114,   115,     0,  1560,   116,   110,   111,   112,   113,
     114,   115,     0,  1567,   116,   110,   111,   112,   113,   114,
     115,     0,  1568,   116,   110,   111,   112,   113,   114,   115,
       0,  1362,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,     0,     0,     0,  1365,     0,   110,
     111,   112,   113,   114,   115,     0,  1372,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,  1373,
     110,   111,   112,   113,   114,   115,     0,  1156,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,  1157,
     110,   111,   112,   113,   114,   115,     0,     0,   116,  1158,
       0,   110,   111,   112,   113,   114,   115,     0,  1159,   116,
       0,   110,   111,   112,   113,   114,   115,     0,  1160,   116,
     110,   111,   112,   113,   114,   115,     0,  1161,   116,   110,
     111,   112,   113,   114,   115,  1144,     0,   116,   110,   111,
     112,   113,   114,   115,  1145,     0,   116,   110,   111,   112,
     113,   114,   115,     0,  1146,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  1147,   116,   110,   111,   112,   113,
     114,   115,     0,  1148,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,  1149,     0,   110,   111,   112,
     113,   114,   115,     0,  1150,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  1151,   116,   110,   111,   112,   113,
     114,   115,     0,  1167,   116,   110,   111,   112,   113,   114,
     115,   667,     0,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,   669,     0,   110,   111,   112,   113,   114,
     115,     0,   670,   116,   110,   111,   112,   113,   114,   115,
       0,   688,   116,   110,   111,   112,   113,   114,   115,     0,
     903,   116,   110,   111,   112,   113,   114,   115,     0,   904,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,     0,     0,     0,   905,     0,   110,   111,   112,
     113,   114,   115,     0,   906,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,   907,     0,   110,   111,
     112,   113,   114,   115,     0,   908,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   909,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,   910,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   911,     0,   110,
     111,   112,   113,   114,   115,     0,   916,   116,   110,   111,
     112,   113,   114,   115,     0,   917,   116,   110,   111,   112,
     113,   114,   115,     0,   918,   116,   110,   111,   112,   113,
     114,   115,     0,   919,   116,     0,   110,   111,   112,   113,
     114,   115,     0,   920,   116,   110,   111,   112,   113,   114,
     115,     0,   921,   116,   110,   111,   112,   113,   114,   115,
       0,   927,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,   930,   110,   111,   112,   113,   114,   115,
       0,     0,   116,  1155,     0,   110,   111,   112,   113,   114,
     115,     0,  1562,   116,   110,   111,   112,   113,   114,   115,
       0,  1563,   116,   110,   111,   112,   113,   114,   115,   664,
       0,   116,   110,   111,   112,   113,   114,   115,     0,   643,
     116,   110,   111,   112,   113,   114,   115,     0,   644,   116,
     110,   111,   112,   113,   114,   115,     0,   645,   116,   110,
     111,   112,   113,   114,   115,     0,   646,   116,   110,   111,
     112,   113,   114,   115,     0,   647,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,   648,     0,   110,   111,   112,   113,   114,   115,     0,
     649,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,   650,     0,   110,   111,   112,   113,   114,   115,
       0,   651,   116,     0,   110,   111,   112,   113,   114,   115,
       0,   656,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   657,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,   658,     0,   110,   111,   112,   113,   114,
     115,     0,   659,   116,   110,   111,   112,   113,   114,   115,
       0,   660,   116,   110,   111,   112,   113,   114,   115,     0,
     661,   116,     0,   110,   111,   112,   113,   114,   115,     0,
     665,   116,   110,   111,   112,   113,   114,   115,     0,   666,
     116,   110,   111,   112,   113,   114,   115,     0,   668,   116,
     110,   111,   112,   113,   114,   115,     0,   671,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,  1205,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,  1206,
       0,   110,   111,   112,   113,   114,   115,     0,  1208,   116,
     110,   111,   112,   113,   114,   115,  1181,     0,   116,   110,
     111,   112,   113,   114,   115,  1478,     0,   116,   110,   111,
     112,   113,   114,   115,  1486,     0,   116,   110,   111,   112,
     113,   114,   115,  1901,     0,   116,   110,   111,   112,   113,
     114,   115,  1903,     0,   116,   110,   111,   112,   113,   114,
     115,  1904,     0,   116,   110,   111,   112,   113,   114,   115,
    2066,     0,   116,     0,     0,     0,     0,   946,     0,     0,
     110,   111,   112,   113,   114,   115,  2067,  1419,   116,   110,
     111,   112,   113,   114,   115,  2068,  1545,   116,     0,   110,
     111,   112,   113,   114,   115,     0,  1546,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,  1548,     0,
       0,     0,     0,     0,  1775,     0,     0,     0,     0,  1823,
     110,   111,   112,   113,   114,   115,     0,     0,   116,  2192,
       0,   110,   111,   112,   113,   114,   115,     0,  2193,   116,
     110,   111,   112,   113,   114,   115,     0,  2194,   116,   110,
     111,   112,   113,   114,   115,  1610,     0,   116,     0,     0,
       0,     0,     0,  1422,     0,   110,   111,   112,   113,   114,
     115,     0,  1214,   116,   110,   111,   112,   113,   114,   115,
    1661,     0,   116,     0,     0,     0,     0,     0,  1772,     0,
     110,   111,   112,   113,   114,   115,     0,  1773,   116,   110,
     111,   112,   113,   114,   115,     0,  1774,   116,   110,   111,
     112,   113,   114,   115,  1926,     0,   116,   110,   111,   112,
     113,   114,   115,  1927,     0,   116,   110,   111,   112,   113,
     114,   115,  1928,     0,   116,     0,     0,     0,     0,     0,
    1603,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     110,   111,   112,   113,   114,   115,  1604,     0,   116,   110,
     111,   112,   113,   114,   115,  1605,     0,   116,   110,   111,
     112,   113,   114,   115,  1414,     0,   116,   110,   111,   112,
     113,   114,   115,  1415,     0,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,  1416,   110,   111,   112,   113,
     114,   115,     0,     0,   116,  1284,   110,   111,   112,   113,
     114,   115,  2654,     0,   116,   110,   111,   112,   113,   114,
     115,  2657,     0,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,     0,  2843,   110,   111,
     112,   113,   114,   115,     0,  2598,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,  2603,     0,   110,   111,   112,   113,   114,   115,     0,
    2834,   116,   110,   111,   112,   113,   114,   115,  2535,     0,
     116,   110,   111,   112,   113,   114,   115,  2540,     0,   116,
     110,   111,   112,   113,   114,   115,  2822,     0,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
    2466,     0,   110,   111,   112,   113,   114,   115,     0,  2471,
     116,   110,   111,   112,   113,   114,   115,     0,  2807,   116,
     110,   111,   112,   113,   114,   115,  2394,     0,   116,   110,
     111,   112,   113,   114,   115,  2399,     0,   116,   110,   111,
     112,   113,   114,   115,  2787,     0,   116,     0,     0,     0,
       0,     0,  2313,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  2318,   110,   111,   112,   113,   114,   115,
       0,     0,   116,  2763,   110,   111,   112,   113,   114,   115,
       0,  2219,   116,   110,   111,   112,   113,   114,   115,     0,
    2224,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,  2733,   110,   111,   112,   113,   114,
     115,     0,  2101,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,     0,     0,  2106,     0,
     110,   111,   112,   113,   114,   115,     0,  2697,   116,     0,
       0,     0,     0,     0,     0,  1965,     0,   531,     0,     0,
       0,     0,     0,     0,  1970,     0,   533,     0,     0,     0,
       0,     0,     0,  2655,     0,   537,     0,   110,   111,   112,
     113,   114,   115,     0,   541,   116,     0,  1887,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,     0,  1899,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  1902,   110,   111,   112,   113,   114,   115,
       0,     0,   116,  2029,     0,   110,   111,   112,   113,   114,
     115,     0,  2031,   116,   110,   111,   112,   113,   114,   115,
       0,  2038,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  2158,     0,   110,   111,   112,   113,
     114,   115,     0,  2162,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,     0,     0,  2169,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,  2170,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,  2268,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,     0,  2269,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  2275,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,  2355,     0,   110,   111,   112,   113,
     114,   115,     0,  2358,   116,   110,   111,   112,   113,   114,
     115,     0,  2360,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,  2503,     0,   110,   111,   112,
     113,   114,   115,     0,  2848,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,     0,  2866,
     110,   111,   112,   113,   114,   115,     0,  1731,   116,   110,
     111,   112,   113,   114,   115,     0,  1743,   116,   110,   111,
     112,   113,   114,   115,     0,  1748,   116,     0,   110,   111,
     112,   113,   114,   115,     0,  1885,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,  1888,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,  1895,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  2026,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  2032,     0,   110,   111,   112,   113,
     114,   115,     0,  2040,   116,   110,   111,   112,   113,   114,
     115,     0,  2041,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,  2159,     0,   110,   111,   112,
     113,   114,   115,     0,  2160,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,  2167,     0,
     110,   111,   112,   113,   114,   115,     0,  2270,   116,   110,
     111,   112,   113,   114,   115,     0,  2273,   116,     0,   110,
     111,   112,   113,   114,   115,     0,  2276,   116,   110,   111,
     112,   113,   114,   115,     0,  2434,   116,     0,   110,   111,
     112,   113,   114,   115,     0,  2839,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
    2863,   110,   111,   112,   113,   114,   115,     0,     0,   116,
    1558,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,     0,     0,  1570,     0,   110,   111,   112,   113,   114,
     115,     0,  1576,   116,   110,   111,   112,   113,   114,   115,
       0,  1729,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,     0,  1732,     0,   110,   111,   112,   113,
     114,   115,     0,  1739,   116,   110,   111,   112,   113,   114,
     115,     0,     0,   116,     0,     0,     0,  1882,     0,   110,
     111,   112,   113,   114,   115,     0,  1889,   116,     0,   110,
     111,   112,   113,   114,   115,     0,  1897,   116,   110,   111,
     112,   113,   114,   115,     0,  1898,   116,     0,   110,   111,
     112,   113,   114,   115,     0,  2027,   116,   110,   111,   112,
     113,   114,   115,     0,  2028,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,  2037,   110,
     111,   112,   113,   114,   115,     0,     0,   116,  2161,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
       0,     0,  2165,     0,   110,   111,   112,   113,   114,   115,
       0,  2168,   116,   110,   111,   112,   113,   114,   115,     0,
    2357,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,  2830,     0,   110,   111,   112,   113,   114,
     115,     0,  2860,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,  1726,   110,   111,   112,   113,
     114,   115,     0,     0,   116,  1742,     0,   110,   111,   112,
     113,   114,   115,     0,  1883,   116,     0,   110,   111,   112,
     113,   114,   115,     0,  1884,   116,   110,   111,   112,   113,
     114,   115,     0,  1894,   116,   110,   111,   112,   113,   114,
     115,     0,  2030,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,  2039,   110,   111,   112,   113,
     114,   115,     0,     0,   116,  2272,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,     0,     0,
    2815,     0,   110,   111,   112,   113,   114,   115,  1328,  2857,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,  1329,     0,   110,   111,   112,   113,   114,   115,     0,
    1330,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  1399,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  1460,     0,   110,   111,   112,   113,   114,
     115,     0,  1112,   116,   110,   111,   112,   113,   114,   115,
       0,  1113,   116,   110,   111,   112,   113,   114,   115,     0,
    1114,   116,   110,   111,   112,   113,   114,   115,     0,  1189,
     116,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,  1256,     0,   110,   111,   112,   113,   114,   115,     0,
     870,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,     0,   871,     0,   110,   111,
     112,   113,   114,   115,     0,   872,   116,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,   957,     0,   110,
     111,   112,   113,   114,   115,     0,  1033,   116,     0,     0,
       0,     0,     0,  1016,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,  1183,     0,   110,   111,   112,
     113,   114,   115,     0,  1243,   116,   110,   111,   112,   113,
     114,   115,     0,  1393,   116,   110,   111,   112,   113,   114,
     115,  2508,     0,   116,   110,   111,   112,   113,   114,   115,
    2633,     0,   116,     0,   110,   111,   112,   113,   114,   115,
       0,  1453,   116,   110,   111,   112,   113,   114,   115,     0,
    1642,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,  1806,     0,   110,   111,   112,
     113,   114,   115,     0,  1958,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,  2555,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,     0,
       0,  2574,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,  1020,     0,   110,   111,   112,   113,   114,
     115,     0,  1245,   116,   110,   111,   112,   113,   114,   115,
       0,  2486,   116,   110,   111,   112,   113,   114,   115,     0,
    2507,   116,   110,   111,   112,   113,   114,   115,     0,  2417,
     116,   110,   111,   112,   113,   114,   115,     0,  2438,   116,
     110,   111,   112,   113,   114,   115,     0,  2439,   116,   110,
     111,   112,   113,   114,   115,     0,     0,   116,     0,     0,
       0,  2339,     0,   110,   111,   112,   113,   114,   115,     0,
    2363,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,  2364,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,     0,  2252,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,  2279,     0,
     110,   111,   112,   113,   114,   115,     0,  2280,   116,   110,
     111,   112,   113,   114,   115,  2140,     0,   116,   110,   111,
     112,   113,   114,   115,  2177,     0,   116,   110,   111,   112,
     113,   114,   115,  2178,     0,   116,     0,     0,     0,     0,
       0,  2008,     0,   110,   111,   112,   113,   114,   115,     0,
    2051,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,     0,  2052,   110,   111,   112,   113,   114,
     115,     0,  1861,   116,     0,   110,   111,   112,   113,   114,
     115,     0,  1909,   116,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,  1910,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,     0,     0,  1704,
       0,   110,   111,   112,   113,   114,   115,     0,  1755,   116,
     110,   111,   112,   113,   114,   115,     0,  1756,   116,   110,
     111,   112,   113,   114,   115,  1523,     0,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,  1584,     0,   110,   111,   112,   113,   114,   115,     0,
    1585,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,  1322,   110,   111,   112,   113,   114,   115,     0,
       0,   116,  1390,     0,   110,   111,   112,   113,   114,   115,
       0,  1391,   116,   110,   111,   112,   113,   114,   115,     0,
       0,   116,     0,  1106,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,     0,  1178,     0,   110,
     111,   112,   113,   114,   115,     0,  1179,   116,   110,   111,
     112,   113,   114,   115,   859,     0,   116,   110,   111,   112,
     113,   114,   115,   942,     0,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,     0,   943,     0,
     110,   111,   112,   113,   114,   115,  1643,     0,   116,     0,
     110,   111,   112,   113,   114,   115,     0,  1805,   116,     0,
     110,   111,   112,   113,   114,   115,     0,  1246,   116,   110,
     111,   112,   113,   114,   115,     0,  1452,   116,     0,   110,
     111,   112,   113,   114,   115,     0,  1019,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
     597,     0,   110,   111,   112,   113,   114,   115,     0,   684,
     116,     0,     0,     0,     0,   233,     0,     0,   685,     0,
       0,   110,   111,   112,   113,   114,   115,  1182,   234,   116,
       0,   110,   111,   112,   113,   114,   115,     0,   228,   116,
       0,   110,   111,   112,   113,   114,   115,     0,   230,   116,
     110,   111,   112,   113,   114,   115,     0,   239,   116,     0,
     110,   111,   112,   113,   114,   115,     0,   588,   116,   110,
     111,   112,   113,   114,   115,     0,   589,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
     862,   110,   111,   112,   113,   114,   115,     0,     0,   116,
       0,   110,   111,   112,   113,   114,   115,   600,   169,   116,
       0,     0,   601,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   522,     0,   110,   111,   112,   113,   114,   115,
       0,   590,   116,     0,   110,   111,   112,   113,   114,   115,
       0,  1057,   116,   110,   111,   112,   113,   114,   115,  1130,
       0,   116,     0,   231,   110,   111,   112,   113,   114,   115,
       0,     0,   116,     0,     0,     0,   232,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   240,   110,   111,   112,
     113,   114,   115,     0,     0,   116,     0,   687,   110,   111,
     112,   113,   114,   115,     0,   246,   116,     0,   110,   111,
     112,   113,   114,   115,     0,   247,   116,   110,   111,   112,
     113,   114,   115,     0,   517,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,  1039,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,   332,     0,   110,
     111,   112,   113,   114,   115,     0,   594,   116,   110,   111,
     112,   113,   114,   115,     0,   227,   116,   110,   111,   112,
     113,   114,   115,     0,  1190,   116,   110,   111,   112,   113,
     114,   115,   702,     0,   116,     0,     0,     0,     0,     0,
     958,   110,   111,   112,   113,   114,   115,   327,     0,   116,
     110,   111,   112,   113,   114,   115,     0,     0,   116,     0,
     420,     0,     0,   110,   111,   112,   113,   114,   115,   421,
    1553,   116,   110,   111,   112,   113,   114,   115,  1015,     0,
     116,     0,     0,     0,     0,  1569,     0,   110,   111,   112,
     113,   114,   115,     0,  1727,   116,   110,   111,   112,   113,
     114,   115,     0,     0,   116,     0,     0,  1728,     0,   110,
     111,   112,   113,   114,   115,     0,  1738,   116,   110,   111,
     112,   113,   114,   115,     0,     0,   116,     0,     0,     0,
       0,  1886,     0,   110,   111,   112,   113,   114,   115,     0,
    1896,   116,   110,   111,   112,   113,   114,   115,     0,     0,
     116,     0,     0,  2164,     0,     0,     0,     0,     0,     0,
       0,     0,  2800,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2854,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286
  };

  const short int
  feiparser::yycheck_[] =
  {
       7,     8,   247,    34,    35,    36,    37,    38,    39,   107,
      95,    42,    19,    20,    21,    34,    35,    36,    37,    38,
      39,     0,   399,    42,    39,    92,    33,    42,    92,    92,
      37,     3,     4,     5,    59,     7,     8,   104,    65,    66,
     104,   104,    49,    50,    51,    52,    53,    54,    55,    56,
      57,   136,    85,    86,    26,    27,    28,    29,    30,    31,
      32,    68,   165,    35,   399,   168,    45,     3,   102,    41,
      34,    35,    36,    37,    38,    39,   110,    52,    42,    86,
     107,    50,    54,    55,   402,    57,    34,    35,    36,    37,
      38,    39,   337,    90,    42,    54,    93,   127,   128,   129,
      97,   131,    54,   110,   111,   112,   113,   114,   115,   116,
      34,    35,    36,    37,    38,    39,   163,   119,    42,   166,
     167,   370,   371,   372,   131,   132,   133,    38,    39,   136,
     137,    42,   394,   140,   141,   142,   143,    54,    55,   146,
     147,   148,    54,    55,   235,   236,   153,   154,   155,   156,
     157,   158,    34,    35,    36,    37,    38,    39,     0,   184,
      42,     3,     4,     5,   394,     7,     8,     9,   174,   175,
      12,   132,   133,    15,   405,    17,     3,    19,   128,   129,
     156,   157,    42,   190,    26,    27,    28,    29,    30,    31,
      32,   198,   107,    35,   107,   202,   156,   157,     5,    41,
     107,    43,    44,    45,    46,   107,   213,    49,   118,    51,
     107,    53,    54,    34,    35,    36,    37,    38,    39,   156,
     157,    42,   107,    34,    35,    36,    37,    38,    39,   236,
     237,    42,    74,    75,    76,    77,   107,   244,   107,   389,
     119,   248,     1,   250,     3,     4,     5,    89,     7,     8,
       9,   107,   107,    12,   107,   262,    15,   118,    17,   181,
      19,   109,   107,   107,   106,   389,   107,    26,    27,    28,
      29,    30,    31,    32,    54,    55,    35,   107,    58,   107,
      60,   107,    41,    63,    43,    44,    45,    46,   124,    79,
      49,    83,    51,    81,    53,    54,    94,    34,    35,    36,
      37,    38,    39,    88,   402,    42,    34,    35,    36,    37,
      38,    39,   162,    11,    42,    74,    75,    76,    77,   120,
      36,    37,    38,    39,   331,   332,    42,   104,   117,    63,
      89,    58,    54,   117,    54,   342,   343,   344,   345,   125,
     351,    34,    35,    36,    37,    38,    39,   106,   355,    42,
     357,    78,   104,    80,   361,    82,   104,    84,   365,    92,
      87,    78,    92,     5,    42,     3,     4,     5,   407,     7,
       8,   406,   390,   109,    33,   406,    34,    35,    36,    37,
      38,    39,   107,   107,    42,   357,   405,   404,    26,    27,
      28,    29,    30,    31,    32,   107,   107,    35,   353,   209,
      54,    96,   164,    41,   411,   412,   413,   414,   415,   165,
     399,    54,   419,    54,    33,   422,    54,    54,   425,    55,
       3,   428,   110,    33,    33,   107,    33,   399,   174,    34,
      35,    36,    37,    38,    39,   217,   443,    42,   169,   403,
      34,    35,    36,    37,    38,    39,    78,    78,    42,    78,
     267,   104,   400,   401,    92,    34,    35,    36,    37,    38,
      39,   267,   386,    42,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,    34,    35,    36,    37,    38,    39,    92,
     104,    42,   104,   510,   511,   104,   513,   514,   104,   104,
     267,   334,   519,   520,   104,   104,   104,   104,   400,   401,
     104,   528,   529,   161,   104,   532,   104,   534,   535,   536,
     104,   538,   539,   540,   104,   542,   543,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   104,   104,   104,   104,   397,   398,   399,   104,   267,
     267,   267,   267,   405,     3,     4,     5,   267,     7,     8,
     267,    34,    35,    36,    37,    38,    39,   267,   267,    42,
     401,   392,   267,   267,   591,   592,   593,    26,    27,    28,
      29,    30,    31,    32,   267,   602,    35,   267,   267,   606,
     267,   267,    41,   267,   267,   107,   268,   268,   615,    34,
      35,    36,    37,    38,    39,    54,    18,    42,    20,    21,
      22,    23,    24,    25,     3,     4,     5,   267,     7,     8,
     332,    33,   267,   332,   268,   268,   107,   107,   397,   398,
     399,   107,   107,   104,   104,   104,   405,    26,    27,    28,
      29,    30,    31,    32,   107,    33,    35,   268,   107,   107,
     237,   165,    41,   102,   401,   176,    34,    35,    36,    37,
      38,    39,    95,   401,    42,    54,   176,   176,   174,   686,
      95,   112,   689,   112,   112,   112,   112,    54,   112,   696,
     112,   698,   699,   700,   112,   112,   112,   355,   705,   112,
     707,   708,   709,   112,   711,   712,   713,   400,   715,   716,
     717,   718,   719,   720,   112,   722,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   112,   112,    34,    35,    36,    37,    38,
      39,   399,   112,    42,   771,   772,   112,   112,   775,   112,
     112,   112,   112,   780,   781,    33,    33,    33,    33,  1024,
     787,   788,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,    33,    33,    33,    33,   401,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    33,   401,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,    33,
      33,    33,   401,    33,    33,    33,    33,    33,    33,   373,
      34,    35,    36,    37,    38,    39,    33,    33,    42,   846,
     847,   848,   373,   373,   851,   852,   853,   854,   855,   856,
      33,    33,    33,   860,    33,    33,    33,   174,   865,   108,
     401,   108,   869,   176,    33,   108,    33,   399,   875,   122,
     877,   399,   879,   880,   881,   399,   883,   884,   885,   399,
     887,   888,   399,   399,   399,   399,   893,   399,   895,   896,
     897,   898,   399,   399,   901,   902,    34,    35,    36,    37,
      38,    39,   399,    33,    42,   399,    34,    35,    36,    37,
      38,    39,   399,   399,    42,    34,    35,    36,    37,    38,
      39,   399,   399,    42,   399,   399,   399,   934,   935,    33,
     399,    34,    35,    36,    37,    38,    39,   944,   401,    42,
      33,   399,   399,    33,   951,    33,   110,   954,   955,   956,
      34,    35,    36,    37,    38,    39,   176,    57,    42,    57,
     399,    57,   108,   131,    33,   110,   973,    34,    35,    36,
      37,    38,    39,    33,   110,    42,   401,   178,   178,   178,
     987,   988,   989,   990,   991,   992,   993,   994,   995,   996,
     997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,
    1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,    34,    35,
      36,    37,    38,    39,   391,   178,    42,   125,  1025,  1026,
     399,  1028,  1029,  1030,   110,  1032,   108,   107,   107,   107,
    1037,  1038,   400,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,  1051,  1052,    57,  1054,  1055,  1056,
      33,  1058,  1059,  1060,   180,  1062,   180,  1064,  1065,  1066,
      34,    35,    36,    37,    38,    39,   180,   180,    42,    33,
      33,    33,    33,    33,    33,    33,   113,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    33,    33,  1095,    33,
    1097,  1098,  1099,    33,  1101,  1102,     3,     4,     5,    33,
       7,     8,     9,    33,  1111,    12,    33,    33,    15,    33,
      17,    33,    19,   392,    33,    33,    33,    33,    33,    26,
      27,    28,    29,    30,    31,    32,   113,    33,    35,    33,
      33,    33,   113,  1140,    41,   108,    43,    44,    45,    46,
      54,    33,    49,    33,    51,   101,    53,    54,    57,   401,
      34,    35,    36,    37,    38,    39,    33,   122,    42,    33,
      33,    57,    63,    33,   401,   107,    33,    74,    75,    76,
      77,   233,   233,  1180,   233,   401,   233,   225,    33,  1186,
    1187,  1188,    89,    33,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1200,  1201,  1202,  1203,   401,    73,   106,
     112,    33,    33,    33,    33,    33,    33,   107,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,
    1237,  1238,  1239,  1240,  1241,  1242,   107,  1244,   183,   107,
     104,   104,  1249,  1250,   104,  1252,  1253,  1254,  1255,    33,
     104,   104,   399,  1260,  1261,   107,    34,    35,    36,    37,
      38,    39,    33,   401,    42,   127,   135,    33,   393,  1276,
    1277,    95,  1279,   401,    33,  1282,    33,   112,  1285,   112,
     218,   218,   401,    34,    35,    36,    37,    38,    39,   112,
     112,    42,    34,    35,    36,    37,    38,    39,   401,   104,
      42,   108,   225,    34,    35,    36,    37,    38,    39,  1316,
    1317,    42,  1319,   218,    33,   226,    33,   401,    33,    33,
    1327,    33,    33,    33,    33,   392,  1333,  1334,  1335,  1336,
    1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,
      34,    35,    36,    37,    38,    39,    33,  1354,    42,  1356,
    1357,    57,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    34,    35,    36,    37,    38,    39,    33,    33,    42,
      33,    33,    33,    33,    33,   401,    33,    33,  1385,    34,
      35,    36,    37,    38,    39,  1392,    33,    42,    33,  1396,
    1397,  1398,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    34,    35,    36,    37,    38,    39,    33,    33,    42,
    1417,  1418,    33,  1420,   388,    33,    33,    33,  1425,  1426,
    1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,
    1447,  1448,  1449,  1450,   401,    33,    33,    33,  1455,  1456,
      33,  1458,  1459,    33,    33,    33,   399,  1464,  1465,  1466,
    1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,
    1477,   399,  1479,  1480,  1481,    33,    33,  1484,   399,   399,
    1487,  1488,  1489,    33,    33,    33,    33,    33,    33,    33,
     397,   398,   399,    33,    33,    33,   110,   108,   405,   107,
     407,     5,    34,    35,    36,    37,    38,    39,   369,  1516,
      42,  1518,  1519,    33,    33,    33,    33,   401,   108,  1526,
      34,    35,    36,    37,    38,    39,    57,    33,    42,   110,
      33,    33,    33,    33,  1541,    34,    35,    36,    37,    38,
      39,    33,  1549,    42,    33,    33,    67,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    34,    35,    36,    37,
      38,    39,    33,    33,    42,   108,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,    33,    33,  1586,
      33,    33,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,
    1597,  1598,  1599,  1600,  1601,    33,    33,    33,    33,  1606,
    1607,  1608,    33,   381,  1611,  1612,  1613,  1614,  1615,  1616,
    1617,  1618,  1619,  1620,  1621,  1622,  1623,  1624,  1625,  1626,
    1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,
    1637,  1638,  1639,    33,  1641,    33,    33,  1644,  1645,    33,
    1647,    34,    35,    36,    37,    38,    39,    33,    33,    42,
     401,    33,    33,    33,    33,  1662,  1663,  1664,    33,   401,
      33,  1668,   108,    34,    35,    36,    37,    38,    39,   108,
     401,    42,    34,    35,    36,    37,    38,    39,   107,    33,
      42,    34,    35,    36,    37,    38,    39,    33,    33,    42,
     384,  1698,    33,  1700,  1701,    33,    33,    33,   107,  1706,
      57,  1708,  1709,    33,  1711,  1712,  1713,  1714,    57,  1716,
      57,  1718,  1719,     5,    33,   369,    33,    33,  1725,   401,
     107,    34,    35,    36,    37,    38,    39,    33,   401,    42,
     107,   107,    34,    35,    36,    37,    38,    39,    33,    33,
      42,   104,    33,    33,  1751,    33,   401,    33,    33,    33,
    1757,   399,    34,    35,    36,    37,    38,    39,    33,   401,
      42,    34,    35,    36,    37,    38,    39,    33,   401,    42,
      33,  1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,  1786,
    1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,  1796,
    1797,  1798,  1799,  1800,  1801,  1802,    33,  1804,    33,    33,
    1807,  1808,    33,  1810,    33,  1812,  1813,    33,  1815,  1816,
    1817,  1818,    33,  1820,    33,  1822,    33,  1824,  1825,  1826,
      33,    33,  1829,    33,   107,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,    33,   108,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,  1855,    33,
    1857,  1858,    33,    33,    33,    33,  1863,    33,    33,  1866,
      33,    33,    33,    33,  1871,    33,  1873,    33,    33,   401,
      33,   369,  1879,  1880,    33,   108,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,   401,   108,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    33,  1906,
      33,    33,   401,    33,  1911,  1912,    33,  1914,  1915,    33,
    1917,  1918,  1919,  1920,   401,  1922,    33,  1924,  1925,   387,
      33,    33,    33,   401,  1931,  1932,  1933,  1934,  1935,  1936,
    1937,  1938,  1939,  1940,  1941,  1942,  1943,  1944,  1945,  1946,
    1947,  1948,  1949,  1950,  1951,  1952,  1953,  1954,  1955,    33,
      33,    33,  1959,  1960,    34,    35,    36,    37,    38,    39,
      33,    33,    42,    33,    33,    33,    33,  1974,  1975,  1976,
    1977,    34,    35,    36,    37,    38,    39,    33,    33,    42,
      34,    35,    36,    37,    38,    39,    33,    33,    42,    33,
     383,    34,    35,    36,    37,    38,    39,  2004,  2005,    42,
      33,    33,    33,  2010,  2011,  2012,  2013,  2014,  2015,  2016,
    2017,  2018,  2019,  2020,    33,    33,    33,   108,  2025,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    33,   108,
     401,    57,    34,    35,    36,    37,    38,    39,    33,   401,
      42,  2048,    33,    33,    33,    33,  2053,    33,   401,    33,
      33,    33,    33,    33,    57,    34,    35,    36,    37,    38,
      39,    33,  2069,    42,  2071,  2072,  2073,  2074,  2075,  2076,
    2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2085,  2086,
    2087,  2088,  2089,  2090,    33,  2092,    33,   400,  2095,  2096,
      33,    33,  2099,  2100,    33,  2102,  2103,  2104,  2105,   401,
    2107,  2108,  2109,  2110,  2111,  2112,    33,  2114,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,   401,
      33,    57,    34,    35,    36,    37,    38,    39,   401,  2136,
      42,    33,    57,    33,  2141,    33,    33,  2144,    33,    33,
      57,    33,  2149,   107,    34,    35,    36,    37,    38,    39,
     107,   107,    42,    33,   399,    34,    35,    36,    37,    38,
      39,   107,    33,    42,    33,    33,    33,  2174,    33,  2176,
      33,   399,  2179,    33,  2181,  2182,   385,  2184,  2185,  2186,
    2187,    33,  2189,  2190,    33,    33,    33,    33,  2195,  2196,
    2197,  2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,  2206,
    2207,  2208,  2209,  2210,  2211,   401,    33,  2214,  2215,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    33,    33,
      33,   104,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    34,    35,    36,    37,    38,    39,    33,    33,    42,
      33,  2248,  2249,   401,    33,    33,  2253,  2254,  2255,  2256,
    2257,  2258,  2259,  2260,  2261,  2262,   401,  2264,  2265,  2266,
    2267,    33,   104,    34,    35,    36,    37,    38,    39,    33,
      33,    42,    33,    33,  2281,    33,    33,    33,   108,    34,
      35,    36,    37,    38,    39,  2292,    33,    42,    33,    33,
    2297,  2298,  2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,
    2307,  2308,  2309,    33,  2311,  2312,    33,  2314,  2315,  2316,
    2317,    33,  2319,  2320,    33,  2322,  2323,  2324,  2325,    33,
      33,   401,    33,   399,    34,    35,    36,    37,    38,    39,
      33,    33,    42,    33,    33,  2342,    33,    33,   401,    33,
    2347,    33,    33,  2350,    33,    33,    33,   401,    34,    35,
      36,    37,    38,    39,  2361,  2362,    42,   400,  2365,  2366,
    2367,    33,  2369,  2370,  2371,  2372,    33,  2374,  2375,    33,
    2377,  2378,  2379,  2380,  2381,  2382,  2383,  2384,  2385,  2386,
      57,   107,  2389,  2390,    33,    33,   401,   399,   231,    34,
      35,    36,    37,    38,    39,  2402,    33,    42,    33,   401,
      34,    35,    36,    37,    38,    39,  2413,  2414,    42,    33,
      33,  2418,  2419,  2420,  2421,  2422,  2423,  2424,  2425,  2426,
    2427,   400,    33,    33,    33,  2432,    34,    35,    36,    37,
      38,    39,    33,  2440,    42,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,    33,    33,    33,  2456,
    2457,  2458,  2459,  2460,  2461,  2462,    33,  2464,  2465,    33,
    2467,  2468,  2469,  2470,    33,  2472,  2473,    33,    33,    33,
      33,  2478,    33,    33,   400,    34,    35,    36,    37,    38,
      39,    33,  2489,    42,    33,   399,    33,  2494,   400,   399,
     399,  2498,  2499,  2500,   399,    33,    33,    33,  2505,  2506,
      33,    33,  2509,  2510,  2511,    33,  2513,  2514,  2515,  2516,
     400,  2518,  2519,  2520,    33,    33,    33,  2524,  2525,  2526,
    2527,   400,    33,  2530,  2531,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,    33,  2544,  2545,  2546,
      33,    33,    33,    33,  2551,    33,    33,    33,    33,  2556,
    2557,  2558,    33,  2560,    33,   231,  2563,  2564,  2565,  2566,
     231,   231,    33,  2570,    33,    33,    33,   232,  2575,    33,
      33,    33,    33,    33,    33,   400,   399,   380,   399,   399,
    2587,  2588,  2589,    33,  2591,  2592,  2593,  2594,   400,  2596,
    2597,   399,    33,  2600,    33,    33,    33,  2604,  2605,  2606,
      33,    33,    33,  2610,    34,    35,    36,    37,    38,    39,
    2617,  2618,    42,  2620,  2621,  2622,    34,    35,    36,    37,
      38,    39,    33,    33,    42,   108,   108,  2634,  2635,   400,
     108,  2638,    33,    33,    57,  2642,  2643,    57,    57,    33,
      33,  2648,  2649,  2650,  2651,   400,    34,    35,    36,    37,
      38,    39,   107,   107,    42,   107,   232,   232,    33,   232,
    2667,  2668,  2669,  2670,    33,  2672,  2673,  2674,  2675,  2676,
    2677,  2678,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,    33,    33,    33,    33,  2693,  2694,  2695,  2696,
     400,   399,  2699,  2700,  2701,  2702,  2703,  2704,   399,   108,
      33,   399,  2709,  2710,    33,    33,    33,    33,   107,    33,
      57,    33,  2719,  2720,   400,    33,  2723,  2724,  2725,  2726,
    2727,  2728,  2729,  2730,    34,    35,    36,    37,    38,    39,
     108,    33,    42,    33,   108,  2742,  2743,  2744,  2745,  2746,
     108,    57,    57,  2750,    57,    33,   107,   107,   107,    33,
      33,    33,  2759,  2760,  2761,   400,    33,  2764,    57,    57,
      33,    33,   108,   108,  2771,   107,   400,   107,    33,    33,
    2777,    33,    33,  2780,    33,    33,  2783,  2784,    33,  2786,
      33,    33,  2789,  2790,    33,    33,  2793,    33,  2795,  2796,
      33,    33,   400,   182,    -1,    -1,    -1,  2804,  2805,    -1,
      -1,   400,    -1,    -1,    -1,  2812,    -1,    -1,    -1,    -1,
      -1,    -1,  2819,  2820,  2821,    -1,  2823,    -1,    -1,    -1,
    2827,    -1,    -1,    -1,    -1,  2832,  2833,    -1,    -1,    -1,
    2837,    -1,    -1,    -1,  2841,  2842,    -1,    -1,    -1,  2846,
      -1,   400,    -1,  2850,  2851,    -1,    -1,    -1,  2855,    -1,
      -1,  2858,    -1,    -1,  2861,    -1,    -1,  2864,    -1,    -1,
    2867,     3,     4,     5,    -1,     7,     8,     9,    -1,    -1,
      12,    -1,    -1,    15,    -1,    17,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    31,
      32,   400,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    43,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    53,    54,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    74,    75,    76,    77,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    89,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,   106,    42,    -1,    -1,    -1,    -1,
     400,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   400,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   400,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     400,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   397,   398,   399,    -1,    -1,
      -1,    -1,    -1,   405,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,   400,
      -1,    42,    34,    35,    36,    37,    38,    39,   400,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   400,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   400,    -1,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,   400,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     400,    -1,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   400,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   400,    -1,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   400,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   400,    -1,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   400,    -1,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   400,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     400,    -1,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   400,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   400,    -1,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   400,    -1,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   400,    -1,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   400,    -1,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   400,    -1,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   400,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   400,    -1,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   400,    -1,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   400,    -1,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,   400,    -1,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   400,    -1,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   400,    -1,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   400,    -1,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   400,    -1,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   400,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,   400,    -1,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     400,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     400,    -1,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   400,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   400,    -1,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   400,    -1,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   400,    -1,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    34,    35,    36,    37,
      38,    39,    -1,   400,    42,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   400,    -1,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    34,    35,
      36,    37,    38,    39,    -1,   400,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   400,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     400,    -1,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   400,    -1,    34,    35,    36,
      37,    38,    39,    -1,   400,    42,    -1,    -1,    54,    55,
      56,    57,    58,   400,    60,    61,    -1,    63,    64,    -1,
      -1,    67,    -1,    69,    -1,   400,    -1,    73,    34,    35,
      36,    37,    38,    39,   400,    -1,    42,    34,    35,    36,
      37,    38,    39,   400,    -1,    42,    34,    35,    36,    37,
      38,    39,   400,    -1,    42,    34,    35,    36,    37,    38,
      39,   400,    -1,    42,    -1,    -1,   112,    -1,    -1,    -1,
     400,    -1,    34,    35,    36,    37,    38,    39,    -1,   400,
      42,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   400,    -1,    34,    35,    36,    37,    38,
      39,    -1,   400,    42,    34,    35,    36,    37,    38,    39,
      -1,   400,    42,    34,    35,    36,    37,    38,    39,    -1,
     400,    42,    -1,    -1,    -1,    -1,   182,    -1,    -1,   400,
      -1,    34,    35,    36,    37,    38,    39,    -1,   400,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    34,
      35,    36,    37,    38,    39,    -1,   400,    42,    -1,    -1,
      -1,    -1,   379,    -1,    -1,   400,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,   400,    -1,    -1,    -1,    -1,   378,
      34,    35,    36,    37,    38,    39,    -1,   377,    42,    34,
      35,    36,    37,    38,    39,    -1,   377,    42,    34,    35,
      36,    37,    38,    39,    -1,   377,    42,    34,    35,    36,
      37,    38,    39,   376,    -1,    42,    34,    35,    36,    37,
      38,    39,   376,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,   376,    42,    34,    35,    36,    37,    38,    39,
      -1,   376,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   375,    42,    34,    35,    36,    37,    38,    39,    -1,
     375,    42,    34,    35,    36,    37,    38,    39,    -1,   375,
      42,    34,    35,    36,    37,    38,    39,   374,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   374,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   374,    -1,    34,    35,    36,    37,    38,    39,
      -1,   374,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   382,   374,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   355,
      -1,    34,    35,    36,    37,    38,    39,    -1,   355,    42,
      34,    35,    36,    37,    38,    39,   354,    -1,    42,    34,
      35,    36,    37,    38,    39,   354,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     352,    34,    35,    36,    37,    38,    39,   350,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   349,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   348,
      34,    35,    36,    37,    38,    39,    -1,   347,    42,    34,
      35,    36,    37,    38,    39,   346,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,   345,    -1,    34,    35,    36,    37,    38,    39,
      -1,   345,    42,    34,    35,    36,    37,    38,    39,   344,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     344,    -1,    34,    35,    36,    37,    38,    39,    -1,   344,
      42,    34,    35,    36,    37,    38,    39,   343,    -1,    42,
      34,    35,    36,    37,    38,    39,   343,    -1,    42,    34,
      35,    36,    37,    38,    39,   343,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   342,    -1,    34,    35,    36,    37,    38,
      39,    -1,   342,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   342,    42,    34,    35,    36,    37,    38,    39,
     341,    -1,    42,    34,    35,    36,    37,    38,    39,   341,
      -1,    42,    34,    35,    36,    37,    38,    39,   341,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   340,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   340,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     340,    34,    35,    36,    37,    38,    39,    -1,   339,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   338,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   337,    34,    35,    36,    37,    38,
      39,    -1,   336,    42,    34,    35,    36,    37,    38,    39,
     335,    -1,    42,    -1,    -1,    -1,    -1,   333,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     333,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   330,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   330,    34,    35,    36,
      37,    38,    39,    -1,   329,    42,    34,    35,    36,    37,
      38,    39,    -1,   329,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   328,    -1,
      34,    35,    36,    37,    38,    39,    -1,   328,    42,    34,
      35,    36,    37,    38,    39,   327,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   326,    34,    35,    36,    37,    38,    39,
      -1,   325,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   324,    34,    35,    36,    37,    38,    39,    -1,
     323,    42,    34,    35,    36,    37,    38,    39,   322,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   321,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   320,    -1,    34,    35,    36,    37,    38,    39,    -1,
     319,    42,    34,    35,    36,    37,    38,    39,   318,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   317,    34,    35,    36,
      37,    38,    39,    -1,   316,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   315,    -1,
      34,    35,    36,    37,    38,    39,    -1,   314,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     313,    34,    35,    36,    37,    38,    39,    -1,   312,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   312,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   311,    34,    35,    36,    37,    38,    39,   309,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   308,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   307,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   306,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   305,    34,
      35,    36,    37,    38,    39,    -1,   304,    42,    34,    35,
      36,    37,    38,    39,   303,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   302,    34,
      35,    36,    37,    38,    39,    -1,   301,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     300,    34,    35,    36,    37,    38,    39,    -1,   299,    42,
      34,    35,    36,    37,    38,    39,    -1,   298,    42,    34,
      35,    36,    37,    38,    39,   297,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   296,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   295,    34,    35,    36,    37,    38,
      39,    -1,   294,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   293,    34,    35,    36,
      37,    38,    39,    -1,   292,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   291,    34,    35,
      36,    37,    38,    39,    -1,   290,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   289,    34,    35,    36,
      37,    38,    39,    -1,   288,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   287,    34,    35,    36,    37,
      38,    39,   283,    -1,    42,    34,    35,    36,    37,    38,
      39,   283,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,   282,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   281,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   280,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   279,    34,    35,    36,    37,    38,
      39,    -1,   278,    42,    34,    35,    36,    37,    38,    39,
     277,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   277,    34,    35,    36,    37,    38,    39,    -1,
     276,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   276,    -1,    34,    35,    36,    37,    38,
      39,    -1,   276,    42,    34,    35,    36,    37,    38,    39,
      -1,   276,    42,    34,    35,    36,    37,    38,    39,   275,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   275,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   275,    -1,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   275,    -1,
      34,    35,    36,    37,    38,    39,    -1,   275,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   275,
      -1,    34,    35,    36,    37,    38,    39,    -1,   275,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   275,    42,
      34,    35,    36,    37,    38,    39,    -1,   275,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   275,    42,    34,
      35,    36,    37,    38,    39,    -1,   275,    42,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   275,    -1,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    34,    35,    36,    37,
      38,    39,    -1,   275,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   274,    -1,    34,    35,    36,
      37,    38,    39,    -1,   274,    42,    34,    35,    36,    37,
      38,    39,    -1,   274,    42,    34,    35,    36,    37,    38,
      39,    -1,   274,    42,    34,    35,    36,    37,    38,    39,
      -1,   273,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   273,    -1,    34,
      35,    36,    37,    38,    39,    -1,   273,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   273,
      34,    35,    36,    37,    38,    39,    -1,   272,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   272,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   272,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      34,    35,    36,    37,    38,    39,    -1,   272,    42,    34,
      35,    36,    37,    38,    39,   271,    -1,    42,    34,    35,
      36,    37,    38,    39,   271,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,   271,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   271,    42,    34,    35,    36,    37,
      38,    39,    -1,   271,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   271,    -1,    34,    35,    36,
      37,    38,    39,    -1,   271,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   271,    42,    34,    35,    36,    37,
      38,    39,    -1,   271,    42,    34,    35,    36,    37,    38,
      39,   270,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   270,    -1,    34,    35,    36,    37,    38,
      39,    -1,   270,    42,    34,    35,    36,    37,    38,    39,
      -1,   270,    42,    34,    35,    36,    37,    38,    39,    -1,
     270,    42,    34,    35,    36,    37,    38,    39,    -1,   270,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   270,    -1,    34,    35,    36,
      37,    38,    39,    -1,   270,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   270,    -1,    34,    35,
      36,    37,    38,    39,    -1,   270,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   270,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   270,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   270,    -1,    34,
      35,    36,    37,    38,    39,    -1,   270,    42,    34,    35,
      36,    37,    38,    39,    -1,   270,    42,    34,    35,    36,
      37,    38,    39,    -1,   270,    42,    34,    35,    36,    37,
      38,    39,    -1,   270,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   270,    42,    34,    35,    36,    37,    38,
      39,    -1,   270,    42,    34,    35,    36,    37,    38,    39,
      -1,   270,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   270,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   270,    -1,    34,    35,    36,    37,    38,
      39,    -1,   270,    42,    34,    35,    36,    37,    38,    39,
      -1,   270,    42,    34,    35,    36,    37,    38,    39,   269,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,   268,
      42,    34,    35,    36,    37,    38,    39,    -1,   268,    42,
      34,    35,    36,    37,    38,    39,    -1,   268,    42,    34,
      35,    36,    37,    38,    39,    -1,   268,    42,    34,    35,
      36,    37,    38,    39,    -1,   268,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   268,    -1,    34,    35,    36,    37,    38,    39,    -1,
     268,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   268,    -1,    34,    35,    36,    37,    38,    39,
      -1,   268,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   268,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   268,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   268,    -1,    34,    35,    36,    37,    38,
      39,    -1,   268,    42,    34,    35,    36,    37,    38,    39,
      -1,   268,    42,    34,    35,    36,    37,    38,    39,    -1,
     268,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     268,    42,    34,    35,    36,    37,    38,    39,    -1,   268,
      42,    34,    35,    36,    37,    38,    39,    -1,   268,    42,
      34,    35,    36,    37,    38,    39,    -1,   268,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   268,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   268,
      -1,    34,    35,    36,    37,    38,    39,    -1,   268,    42,
      34,    35,    36,    37,    38,    39,   267,    -1,    42,    34,
      35,    36,    37,    38,    39,   267,    -1,    42,    34,    35,
      36,    37,    38,    39,   267,    -1,    42,    34,    35,    36,
      37,    38,    39,   267,    -1,    42,    34,    35,    36,    37,
      38,    39,   267,    -1,    42,    34,    35,    36,    37,    38,
      39,   267,    -1,    42,    34,    35,    36,    37,    38,    39,
     267,    -1,    42,    -1,    -1,    -1,    -1,   237,    -1,    -1,
      34,    35,    36,    37,    38,    39,   267,   237,    42,    34,
      35,    36,    37,    38,    39,   267,   237,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   237,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   237,    -1,
      -1,    -1,    -1,    -1,   234,    -1,    -1,    -1,    -1,   230,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   230,
      -1,    34,    35,    36,    37,    38,    39,    -1,   230,    42,
      34,    35,    36,    37,    38,    39,    -1,   230,    42,    34,
      35,    36,    37,    38,    39,   229,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   228,    -1,    34,    35,    36,    37,    38,
      39,    -1,   227,    42,    34,    35,    36,    37,    38,    39,
     223,    -1,    42,    -1,    -1,    -1,    -1,    -1,   222,    -1,
      34,    35,    36,    37,    38,    39,    -1,   222,    42,    34,
      35,    36,    37,    38,    39,    -1,   222,    42,    34,    35,
      36,    37,    38,    39,   221,    -1,    42,    34,    35,    36,
      37,    38,    39,   221,    -1,    42,    34,    35,    36,    37,
      38,    39,   221,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     220,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,   220,    -1,    42,    34,
      35,    36,    37,    38,    39,   220,    -1,    42,    34,    35,
      36,    37,    38,    39,   219,    -1,    42,    34,    35,    36,
      37,    38,    39,   219,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   219,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   218,    34,    35,    36,    37,
      38,    39,   216,    -1,    42,    34,    35,    36,    37,    38,
      39,   216,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   216,    34,    35,
      36,    37,    38,    39,    -1,   215,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   215,    -1,    34,    35,    36,    37,    38,    39,    -1,
     215,    42,    34,    35,    36,    37,    38,    39,   214,    -1,
      42,    34,    35,    36,    37,    38,    39,   214,    -1,    42,
      34,    35,    36,    37,    38,    39,   214,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     213,    -1,    34,    35,    36,    37,    38,    39,    -1,   213,
      42,    34,    35,    36,    37,    38,    39,    -1,   213,    42,
      34,    35,    36,    37,    38,    39,   212,    -1,    42,    34,
      35,    36,    37,    38,    39,   212,    -1,    42,    34,    35,
      36,    37,    38,    39,   212,    -1,    42,    -1,    -1,    -1,
      -1,    -1,   211,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   211,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   211,    34,    35,    36,    37,    38,    39,
      -1,   210,    42,    34,    35,    36,    37,    38,    39,    -1,
     210,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   210,    34,    35,    36,    37,    38,
      39,    -1,   209,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   209,    -1,
      34,    35,    36,    37,    38,    39,    -1,   209,    42,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,   179,    -1,    34,    35,    36,
      37,    38,    39,    -1,   179,    42,    -1,   173,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,   173,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   173,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   173,    -1,    34,    35,    36,    37,    38,
      39,    -1,   173,    42,    34,    35,    36,    37,    38,    39,
      -1,   173,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   173,    -1,    34,    35,    36,    37,
      38,    39,    -1,   173,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   173,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   173,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   173,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,   173,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   173,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,   173,    -1,    34,    35,    36,    37,
      38,    39,    -1,   173,    42,    34,    35,    36,    37,    38,
      39,    -1,   173,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   173,    -1,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   173,
      34,    35,    36,    37,    38,    39,    -1,   172,    42,    34,
      35,    36,    37,    38,    39,    -1,   172,    42,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   172,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   172,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   172,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   172,    -1,    34,    35,    36,    37,
      38,    39,    -1,   172,    42,    34,    35,    36,    37,    38,
      39,    -1,   172,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   172,    -1,    34,    35,    36,
      37,    38,    39,    -1,   172,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   172,    -1,
      34,    35,    36,    37,    38,    39,    -1,   172,    42,    34,
      35,    36,    37,    38,    39,    -1,   172,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   172,    42,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     172,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     171,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   171,    -1,    34,    35,    36,    37,    38,
      39,    -1,   171,    42,    34,    35,    36,    37,    38,    39,
      -1,   171,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   171,    -1,    34,    35,    36,    37,
      38,    39,    -1,   171,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,   171,    -1,    34,
      35,    36,    37,    38,    39,    -1,   171,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   171,    42,    34,    35,
      36,    37,    38,    39,    -1,   171,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   171,    42,    34,    35,    36,
      37,    38,    39,    -1,   171,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   171,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   171,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   171,    -1,    34,    35,    36,    37,    38,    39,
      -1,   171,    42,    34,    35,    36,    37,    38,    39,    -1,
     171,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   171,    -1,    34,    35,    36,    37,    38,
      39,    -1,   171,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   170,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   170,    -1,    34,    35,    36,
      37,    38,    39,    -1,   170,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   170,    42,    34,    35,    36,    37,
      38,    39,    -1,   170,    42,    34,    35,    36,    37,    38,
      39,    -1,   170,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   170,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   170,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    34,    35,    36,    37,    38,    39,   160,   170,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   160,    -1,    34,    35,    36,    37,    38,    39,    -1,
     160,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   160,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   160,    -1,    34,    35,    36,    37,    38,
      39,    -1,   159,    42,    34,    35,    36,    37,    38,    39,
      -1,   159,    42,    34,    35,    36,    37,    38,    39,    -1,
     159,    42,    34,    35,    36,    37,    38,    39,    -1,   159,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   159,    -1,    34,    35,    36,    37,    38,    39,    -1,
     158,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   158,    -1,    34,    35,
      36,    37,    38,    39,    -1,   158,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   158,    -1,    34,
      35,    36,    37,    38,    39,    -1,   158,    42,    -1,    -1,
      -1,    -1,    -1,   154,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   154,    -1,    34,    35,    36,
      37,    38,    39,    -1,   154,    42,    34,    35,    36,    37,
      38,    39,    -1,   154,    42,    34,    35,    36,    37,    38,
      39,   153,    -1,    42,    34,    35,    36,    37,    38,    39,
     153,    -1,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   152,    42,    34,    35,    36,    37,    38,    39,    -1,
     152,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   151,    -1,    34,    35,    36,
      37,    38,    39,    -1,   151,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   151,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   151,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   150,    -1,    34,    35,    36,    37,    38,
      39,    -1,   150,    42,    34,    35,    36,    37,    38,    39,
      -1,   150,    42,    34,    35,    36,    37,    38,    39,    -1,
     150,    42,    34,    35,    36,    37,    38,    39,    -1,   149,
      42,    34,    35,    36,    37,    38,    39,    -1,   149,    42,
      34,    35,    36,    37,    38,    39,    -1,   149,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   148,    -1,    34,    35,    36,    37,    38,    39,    -1,
     148,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   148,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   147,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   147,    -1,
      34,    35,    36,    37,    38,    39,    -1,   147,    42,    34,
      35,    36,    37,    38,    39,   146,    -1,    42,    34,    35,
      36,    37,    38,    39,   146,    -1,    42,    34,    35,    36,
      37,    38,    39,   146,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    34,    35,    36,    37,    38,    39,    -1,
     145,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   145,    34,    35,    36,    37,    38,
      39,    -1,   144,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   144,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   144,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,   143,
      -1,    34,    35,    36,    37,    38,    39,    -1,   143,    42,
      34,    35,    36,    37,    38,    39,    -1,   143,    42,    34,
      35,    36,    37,    38,    39,   142,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   142,    -1,    34,    35,    36,    37,    38,    39,    -1,
     142,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   141,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   141,    -1,    34,    35,    36,    37,    38,    39,
      -1,   141,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   140,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   140,    -1,    34,
      35,    36,    37,    38,    39,    -1,   140,    42,    34,    35,
      36,    37,    38,    39,   139,    -1,    42,    34,    35,    36,
      37,    38,    39,   139,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   139,    -1,
      34,    35,    36,    37,    38,    39,   138,    -1,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   138,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   137,    42,    34,
      35,    36,    37,    38,    39,    -1,   137,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   136,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     135,    -1,    34,    35,    36,    37,    38,    39,    -1,   135,
      42,    -1,    -1,    -1,    -1,   109,    -1,    -1,   135,    -1,
      -1,    34,    35,    36,    37,    38,    39,   135,   122,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   122,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   122,    42,
      34,    35,    36,    37,    38,    39,    -1,   122,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   122,    42,    34,
      35,    36,    37,    38,    39,    -1,   122,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     122,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    34,    35,    36,    37,    38,    39,   110,   121,    42,
      -1,    -1,   115,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   115,    -1,    34,    35,    36,    37,    38,    39,
      -1,   115,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   115,    42,    34,    35,    36,    37,    38,    39,   114,
      -1,    42,    -1,   109,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   109,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   109,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   108,    34,    35,
      36,    37,    38,    39,    -1,   105,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   105,    42,    34,    35,    36,
      37,    38,    39,    -1,   105,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   105,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   104,    -1,    34,
      35,    36,    37,    38,    39,    -1,   103,    42,    34,    35,
      36,    37,    38,    39,    -1,   101,    42,    34,    35,    36,
      37,    38,    39,    -1,   100,    42,    34,    35,    36,    37,
      38,    39,    99,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      98,    34,    35,    36,    37,    38,    39,    95,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      95,    -1,    -1,    34,    35,    36,    37,    38,    39,    95,
      78,    42,    34,    35,    36,    37,    38,    39,    95,    -1,
      42,    -1,    -1,    -1,    -1,    78,    -1,    34,    35,    36,
      37,    38,    39,    -1,    78,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    78,    -1,    34,
      35,    36,    37,    38,    39,    -1,    78,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    78,    -1,    34,    35,    36,    37,    38,    39,    -1,
      78,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,     9,    12,    15,
      17,    19,    26,    27,    28,    29,    30,    31,    32,    35,
      41,    43,    44,    45,    46,    49,    51,    53,    54,    74,
      75,    76,    77,    89,   106,   397,   398,   399,   405,   409,
     410,   411,   413,   414,   415,   418,   419,   420,    18,    20,
      21,    22,    23,    24,    25,    33,   399,   399,   419,   419,
       3,   419,   419,    54,    55,    57,   357,   419,    52,    50,
      54,    55,    56,    57,    58,    60,    61,    63,    64,    67,
      69,    73,   112,   182,   382,   416,   402,    54,    54,    54,
      55,    58,    60,    63,    58,    78,    80,    82,    84,    87,
      92,   161,   419,   119,   394,   394,   419,   405,     0,   420,
      34,    35,    36,    37,    38,    39,    42,   405,     3,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   406,   421,
     421,   107,   107,   107,   419,     5,   107,   107,   417,   118,
     107,   107,   107,   107,   389,   119,   107,   107,   107,   118,
     181,   109,   419,   107,   107,   107,   107,   107,   107,   389,
     124,    79,    81,    83,    85,    86,    88,    94,   162,   121,
     120,   401,   419,   419,   419,   419,   419,   419,   419,   401,
     401,   407,   420,   422,    11,   419,   419,   419,   419,   419,
     117,   419,   419,   419,   419,   104,    65,    66,   107,   419,
     419,   419,   117,    63,    54,   403,   419,   419,   419,   419,
     419,   419,    54,   125,   351,   104,   104,    92,    78,   163,
     166,   167,    92,     5,   422,   407,   421,   101,   122,   419,
     122,   109,   109,   109,   122,   390,   107,   107,   419,   122,
     109,   346,   419,   109,   107,   404,   105,   105,   107,   419,
      33,   209,   353,    54,    96,   164,   165,   165,   168,    90,
      93,    97,   399,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,    54,    55,    54,    54,
      55,   370,   371,   372,    54,   419,   419,    95,   235,   236,
      55,    33,   104,    54,   419,     3,   102,   110,   412,   412,
     419,   419,    33,    33,   107,    33,   217,   174,   174,   175,
     169,    78,    78,    78,   419,    92,   104,    92,   104,   104,
     104,    92,   104,   104,   104,    92,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   334,   268,   332,   332,   268,   267,   268,   268,
     267,   107,   107,   107,   107,   107,   104,   104,   104,   107,
      95,    95,    33,   237,   268,   107,   419,   419,   107,   383,
     412,   350,   419,   419,   419,   419,   165,   176,   176,   176,
     174,    95,    95,   400,   419,   112,   419,   112,   112,   112,
     419,   112,   112,   112,   419,   112,   112,   112,   112,   112,
     112,    54,   112,   112,   112,   112,   112,   112,   112,   112,
     112,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     102,   419,   419,   419,   419,   419,   373,   373,   373,   419,
      33,    33,   419,    33,    33,   419,   347,   105,   419,    33,
      33,   352,   115,   174,   108,   108,   108,   176,    33,    33,
     419,   179,   399,   179,   399,   399,   399,   179,   399,   399,
     399,   179,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   399,   399,   399,   399,   399,   399,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   122,   122,   122,
     115,    33,    33,    33,   103,   419,   419,   135,   419,   419,
     110,   115,    33,   392,   419,   419,    33,   110,   176,    57,
      57,    57,   108,   419,   419,   400,   178,   419,   178,   419,
     419,   419,   178,   419,   419,   419,   178,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   312,   328,   328,   312,   268,   268,   268,   268,
     268,   268,   281,   335,   269,   268,   268,   270,   268,   270,
     270,   268,   131,   127,   128,   129,   131,   132,   133,   110,
     419,   419,   419,   391,   135,   135,    33,   108,   270,    33,
     110,   419,   110,   384,   349,   419,   125,   108,   107,   107,
     107,    57,    99,   419,   180,   400,   180,   400,   400,   400,
     180,   400,   400,   400,   180,   400,   400,   400,   400,   400,
     400,   401,   400,   400,   400,   400,   400,   400,   400,   400,
     400,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     113,   113,   113,   108,   101,    95,   136,   374,   374,   374,
      54,    33,    33,   419,    57,    33,   419,   122,   348,    63,
      33,    33,   419,    57,   419,   419,   419,   107,    33,   401,
     233,   419,   233,   419,   419,   419,   233,   419,   419,   419,
     233,   419,   419,   419,   419,   419,   419,   225,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,    33,    33,    33,    73,
     112,    33,    33,    33,    33,    33,   107,   419,   419,   139,
     107,   419,   122,   128,   129,    33,   183,   419,   419,   107,
     158,   158,   158,   419,   419,   104,   104,   400,   104,   400,
     400,   400,   104,   400,   400,   400,   104,   400,   400,   401,
     401,   401,   401,    33,   401,   400,   400,   400,   400,   401,
     401,   400,   400,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   309,   329,   329,   313,   270,   270,   270,   270,
     270,   270,   282,   336,   333,   333,   340,   270,   340,   340,
     270,   419,   419,   419,   107,   399,   419,   419,   419,   419,
     419,   419,   139,   139,    33,   419,   237,   127,    95,   135,
     419,   393,   385,   419,    33,    33,    33,   158,    98,   419,
     112,   419,   112,   419,   419,   419,   112,   419,   419,   419,
     112,   419,   419,   104,   218,   218,   225,   419,   218,   419,
     419,   419,   419,   108,   226,   419,   419,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    95,   154,   419,   419,   136,
     150,   375,   376,   376,   392,    33,    33,   419,    33,    33,
      33,   419,    33,   158,   419,   419,   419,    33,    33,   105,
     399,   400,   399,   400,   400,   400,   399,   400,   400,   400,
     399,   400,   400,   419,    33,    33,    33,   115,    33,   400,
     400,   401,   400,    57,    33,   400,   400,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,    33,    33,   400,    33,    33,
      33,    33,    33,   412,   419,   419,   140,   419,   419,   419,
     419,    33,   159,   159,   159,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     114,   419,   419,   419,   110,   419,   419,   419,   108,   419,
     107,   419,   419,   419,   271,   271,   271,   271,   271,   271,
     271,   271,   289,   355,   355,   270,   272,   272,   272,   272,
     272,   272,   278,   337,   374,   379,   341,   271,   341,   341,
     419,     5,   419,   419,   419,   369,   419,   419,   140,   140,
      33,   267,   135,   154,   386,   419,    33,    33,    33,   159,
     100,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   108,   268,   268,   392,   268,   401,
     401,    57,   401,   419,   227,   400,   400,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   154,   400,   150,   137,   375,   377,    33,
      33,   419,    33,    33,    33,    33,   159,   419,   419,   419,
      33,    33,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,    67,    33,    33,   110,    33,
     108,   108,   107,   108,   218,    33,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,    33,   419,    33,    33,    33,    33,
     419,   419,   141,   419,   419,   419,   419,    33,   160,   160,
     160,   419,   419,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   107,   419,   419,   419,
      57,    57,   419,    57,    33,   419,   400,   400,   275,   276,
     277,   277,   273,   275,   276,   273,   287,   354,   354,   311,
     276,   276,   273,   273,   275,   275,   279,   355,   376,   380,
     342,   275,   342,   342,     5,   400,   419,   419,   369,   419,
     141,   141,    33,   154,   387,   419,    33,    33,    33,   160,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   219,   219,   219,   107,   107,   237,
     107,   419,   228,   419,   419,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,   419,   137,   152,   375,    33,    33,   419,    33,    33,
     160,   419,   419,   419,    33,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   267,    33,
      33,    33,   419,   419,    33,   419,   267,    33,   400,   400,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   400,   401,    33,    33,
      33,   419,   419,   142,   419,   419,    33,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,    33,   419,   419,   419,   237,   237,   419,   237,    33,
     419,   419,   419,    78,   275,   275,   275,   274,   171,   275,
     274,   288,   270,   270,   314,   275,   275,   274,   274,    78,
     171,   280,   338,   377,   381,   343,   171,   343,   343,   419,
     104,   419,   419,   369,   142,   142,    33,   388,   419,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   419,   220,   220,   220,    33,    33,    33,   419,
     229,   400,   400,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,   400,
     156,   157,   152,   138,    33,    33,   419,    33,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   223,    33,    33,    33,   419,   419,   419,    33,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   399,   419,
      33,    33,   419,   419,   143,   419,   400,   401,   400,   400,
     401,   400,   400,   400,   400,   401,   400,   401,   400,    33,
     419,   419,   419,   419,   401,   400,   170,    78,    78,   171,
     275,   172,   171,   275,   290,   330,   330,   315,    78,   171,
     275,   275,   170,   172,   339,   378,   374,   344,   172,   344,
     344,   400,   419,   419,   419,   143,   143,    33,   419,   108,
     419,   419,   108,   419,   419,   419,   419,   108,   419,   108,
     419,   419,   222,   222,   222,   234,   108,   419,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   419,   400,   138,   151,    33,    33,   419,
     400,    57,   400,   400,    57,   400,   400,   400,   400,    57,
     400,    57,   400,   230,    33,    33,    33,    33,    57,   400,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   400,   419,    33,    33,   419,
     419,   144,   419,   107,   419,   419,   107,   419,   419,   419,
     419,   107,   419,   107,   419,    33,   419,   419,   419,   399,
     107,   419,   171,   170,   170,   172,    78,   173,   172,   171,
     291,   283,   283,   316,   170,   172,    78,   171,   171,   173,
     376,   267,   173,   267,   267,   419,   400,   419,   419,   144,
     144,    33,   400,   419,   400,   400,   419,   400,   400,   400,
     400,   419,   400,   419,   400,   399,   221,   221,   221,   419,
     419,   400,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,   400,   401,   419,   151,    33,
      33,   419,   419,   419,   419,   208,   419,   419,   419,   419,
     208,   419,   419,   419,    33,    33,    33,   400,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   104,   400,    33,   419,   419,   145,   401,
     400,   400,    33,   400,   400,   400,   400,    33,   400,   400,
     400,   419,   419,   419,   419,   400,   172,   171,   171,   173,
     170,   173,   172,   292,   275,   275,   317,   171,   173,   170,
     172,   172,   377,   275,   275,   275,   401,   156,   157,   419,
     419,   145,   145,    33,   108,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   267,   267,   267,   400,
     419,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,   104,   399,   419,   401,    33,    33,   419,    57,   400,
     400,   209,   400,   400,   400,   400,   209,   400,   400,   400,
      33,    33,    33,   419,   400,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   156,   157,   419,   419,   419,
     146,   107,   419,   419,    33,   419,   419,   419,   419,    33,
     419,   419,   419,   419,   419,   419,   401,   419,   173,   172,
     172,   171,   173,   293,    78,   171,   318,   172,   171,   173,
     173,   378,   345,   345,   399,   419,   400,   146,   146,    33,
     419,   400,   400,   419,   400,   400,   400,   400,   419,   400,
     400,   401,   230,   230,   230,   400,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,   419,   419,    33,    33,   419,   419,   419,   210,
     419,   419,   419,   419,   210,   419,   419,   231,    33,    33,
      33,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   400,   400,
     419,   419,   147,   400,   400,    33,   400,   400,   400,   400,
      33,   400,   400,    33,   399,   399,   399,   400,   173,   173,
     172,   294,   170,   172,   319,   173,   172,   419,   419,   147,
     147,    33,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   399,   419,   419,   419,   419,    33,    33,    33,
      33,    33,    33,    33,    33,    33,   400,   400,    33,    33,
     419,   400,   400,   211,   400,   400,   400,   400,   211,   400,
     400,   419,   400,   400,   400,   400,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   148,
     419,   419,    33,   419,   419,   419,   419,    33,   419,   419,
     400,   419,   419,   419,   419,   173,   295,   171,   173,   320,
     173,   400,   400,   148,   148,    33,   400,   400,   419,   400,
     400,   400,   400,   419,   400,   400,   419,   400,   400,   400,
     400,    33,    33,    33,    33,    33,    33,   419,   419,    33,
      33,   419,   419,   419,   212,   419,   419,   419,   419,   212,
     419,   419,   400,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   400,   400,   419,   419,   149,   400,   400,
      33,   400,   400,   400,   400,    33,   400,   400,   419,   401,
     401,   401,   400,   296,   172,   321,   419,   419,   149,   149,
      33,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   401,   231,   231,   231,   419,    33,    33,    33,   400,
     400,    33,    33,   419,   400,   400,   213,   400,   400,   400,
     400,   213,   400,   400,   232,    33,    33,    33,   400,   419,
     419,   419,   419,   419,   419,   419,   150,   419,   419,    33,
     419,   419,   419,   419,    33,   419,   419,    33,   399,   399,
     399,   419,   297,   173,   322,   400,   400,   150,   153,    33,
     400,   400,   419,   400,   400,   400,   400,   419,   400,   400,
     399,   419,   419,   419,   400,    33,    33,    33,   419,   419,
      33,    33,   419,   419,   419,   214,   419,   419,   419,   419,
     214,   419,   419,   419,   400,   400,   400,   419,   419,   419,
     419,   400,   401,   419,   419,   151,   400,   400,    33,   401,
     400,   401,   401,    33,   400,   400,   400,   419,   419,   419,
     400,   298,   323,   419,   151,    33,   419,   419,   419,   108,
     419,   108,   108,   419,   419,   419,   419,   400,   400,   400,
     419,    33,    33,   400,    33,   419,   400,   400,   215,    57,
     400,    57,    57,   215,   400,   400,   400,   419,   419,   419,
     400,   419,   419,   419,   419,   419,   419,    33,   107,   419,
     107,   107,    33,   419,   419,   419,   401,   401,   401,   419,
     299,   324,   401,   153,   400,   400,   419,   419,   400,   419,
     419,   419,   400,   400,   401,   232,   232,   232,   400,    33,
      33,    33,   419,   419,   216,   208,   419,   216,   419,   419,
      33,    33,    33,   419,   419,   419,   419,   400,   400,    33,
      33,   401,    33,   400,   400,   399,   399,   399,   400,   300,
     325,   419,   419,   419,   419,   108,   419,   419,   419,   419,
     419,   419,   419,    33,    33,   400,   400,   209,    57,   400,
     400,   400,   400,   400,   400,   419,   419,   419,   419,    33,
     107,   419,   419,   419,   419,   419,   419,   301,   326,   400,
     400,   419,   419,   400,   400,   400,   400,   400,   400,    33,
      33,   419,   419,   210,   419,   419,   419,   419,   419,   419,
     419,   419,   400,   400,    33,   400,   400,   401,   401,   401,
     400,   302,   327,   419,   419,   419,   419,   419,   419,    33,
      33,   400,   401,   211,   400,   401,   401,   419,   419,   419,
     108,    33,   419,   108,   108,   303,   275,   400,    57,   419,
     400,    57,    57,    33,    33,   419,   107,   212,   419,   107,
     107,   419,   419,   400,   419,    33,   400,   419,   419,   304,
      78,   419,   419,   419,    33,    33,   401,   213,   401,   419,
     419,   108,    33,   108,   305,   170,    57,   419,    57,    33,
      33,   107,   214,   107,   419,   419,   419,    33,   419,   306,
     171,   419,    33,    33,   215,   419,   419,    33,   307,   172,
     419,    33,    33,   216,   419,   419,    33,   308,   173,   419,
      33,    33,   419,   419,    78,    33,   419,   170,    33,   419,
     171,    33,   419,   172,    33,   419,   173,    33,   419
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   408,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   411,   411,   411,   411,   412,   412,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   415,
     415,   415,   415,   415,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     418,   418,   418,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   421,   421,   422,   422
  };

  const unsigned char
  feiparser::yyr2_[] =
  {
       0,     2,     2,     2,     4,     4,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    15,     8,     2,
      13,     3,     5,    14,    24,    32,    32,    48,    34,    52,
      13,    16,    19,    10,    10,    31,    28,    13,    21,    18,
      16,    19,    22,     8,     8,    15,    24,    15,    50,    54,
      48,    13,    18,     6,     6,    14,     6,     1,     2,     2,
       2,     5,     3,    12,     3,    10,     7,    11,     3,     4,
       6,     9,    18,     7,    22,    20,    20,    21,    20,     4,
       4,     4,     4,     6,    14,    35,    29,    29,    38,    32,
      41,    38,    35,    32,    95,    47,    41,    80,    38,    32,
      41,    35,    35,    29,    23,    11,    26,    35,    26,    35,
      35,    14,    32,    27,    27,    27,    33,    65,    65,    71,
      65,    71,    51,    57,    51,    54,    54,    78,    59,    59,
      51,    14,    16,    59,    32,    22,    22,    20,    22,    21,
       1,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     3,     3,     4,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     1,     2,     2,     3,     5,     3,
       1,     1,     2,     2,     3,     1,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const feiparser::yytname_[] =
  {
  "$end", "error", "$undefined", "IDENT", "NUMBER", "STRING", "EOL",
  "ISUNITTYPE", "FUNCTION", "IF", "THEN", "ELSE", "WHILE", "DO", "LET",
  "EXITSAFEMODE", "CONTINUE", "BYE", "IN", "REQUIRE", "PLUSEQUAL",
  "MINUSEQUAL", "TIMESEQUAL", "DIVIDEEQUAL", "MODULUSEQUAL", "POWEQUAL",
  "NUMBER_OF_NODES", "NUMBER_OF_ELEMENTS", "CURRENT_TIME",
  "NUMBER_OF_SP_CONSTRAINTS", "NUMBER_OF_MP_CONSTRAINTS",
  "NUMBER_OF_LOADS", "IS_PARALLEL", "'='", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'^'", "UMINUS", "'!'", "CMP", "CMD_print", "CMD_help",
  "CMD_whos", "CMD_check", "CMD_save", "CMD_restore", "MODEL", "NAME",
  "RESTART", "MESH", "ADD", "NODE", "ELEMENT", "ELEMENTS", "MATERIAL",
  "LOAD", "TIMEHISTORY", "IMPOSEDMOTION", "DAMPING", "DAMPINGTYPE",
  "CONSTRAINT", "DRM", "SAVEFORCES", "RESTOREFORCES", "SECTION",
  "LOADPATTERN", "PENALTYDISPLACEMENT", "LOADVALUE", "ELEMENTNAME",
  "MATERIALNAME", "ACCELERATION_FIELD", "FIX", "FREE", "REMOVE", "DEFINE",
  "ALGORITHM", "ALGNAME", "CONVERGENCE_TEST", "TESTNAME", "SOLVER",
  "SOLVERNAME", "DYNAMICINTEGRATOR", "DYNAMICINTEGRATOR_HHT",
  "DYNAMICINTEGRATOR_NEWMARK", "STATICINTEGRATOR",
  "STATICINTEGRATOR_DISPLACEMENT", "SIMULATE", "STATIC", "DYNAMIC",
  "USING", "TRANSIENT", "EIGEN", "time_step", "number_of_modes",
  "VARIABLETRANSIENT", "maximum_time_step", "minimum_time_step",
  "number_of_iterations", "AT", "ALL", "AND", "WITH", "TEXTDOFS", "NEW",
  "TEXTNUMBER", "USE", "TO", "DOF", "TEXTWITH", "NODES", "FORCE",
  "INTEGRATIONPOINTS", "dof", "RESPONSE", "FILE", "FROM", "LOADING",
  "STAGE", "STEPS", "TYPE", "DOFS", "FACTOR", "INCREMENT",
  "TH_GROUNDMOTION", "TH_LINEAR", "TH_PATH_SERIES", "TH_PATH_TIME_SERIES",
  "TH_CONSTANT", "TH_FROM_REACTIONS", "self_weight", "surface",
  "load_value", "scale_factor", "displacement_scale_unit",
  "velocity_scale_unit", "acceleration_scale_unit", "number_of_steps",
  "number_of_drm_nodes", "number_of_drm_elements", "x_max", "x_min",
  "y_max", "y_min", "z_max", "z_min", "element_file", "nodes_file",
  "displacement_file", "acceleration_file", "velocity_file", "force_file",
  "series_file", "time_series_file", "MAGNITUDES", "MAGNITUDE",
  "strain_increment_size", "maximum_strain",
  "number_of_times_reaching_maximum_strain", "constitutive", "testing",
  "constant", "mean", "triaxial", "drained", "undrained", "simple",
  "shear", "number_of_subincrements", "maximum_number_of_iterations",
  "tolerance_1", "tolerance_2", "strain", "stress", "control", "Guass",
  "points", "Gauss", "each", "point", "single", "value", "EightNodeBrick",
  "TwentySevenNodeBrick", "EightNodeBrick_upU", "TwentyNodeBrick_uPU",
  "TwentyNodeBrick", "TwentyNodeBrickElastic", "EightNodeBrick_up",
  "variable_node_brick_8_to_27", "EightNodeBrickElastic",
  "TwentySevenNodeBrickElastic", "beam_displacement_based", "beam_elastic",
  "beam_elastic_lumped_mass", "penalty",
  "penalty_for_applying_generalized_displacement", "beam_9dof_elastic",
  "FourNodeShellMITC4", "FourNodeShellNewMITC4", "ThreeNodeShellANDES",
  "FourNodeShellANDES", "truss", "contact", "EightNodeBrickLT",
  "TwentySevenNodeBrickLT", "porosity", "alpha", "rho_s", "rho_f", "k_x",
  "k_y", "k_z", "K_s", "K_f", "pressure", "cross_section", "shear_modulus",
  "torsion_Jx", "bending_Iz", "bending_Iy", "IntegrationRule",
  "number_of_integration_points", "stiffness", "normal_stiffness",
  "tangential_stiffness", "friction_ratio", "maximum_gap",
  "xz_plane_vector", "joint_1_offset", "joint_2_offset", "direction",
  "contact_plane_vector", "MembranePlateFiber", "ElasticMembranePlate",
  "thickness", "linear_elastic_isotropic_3d", "vonmises_perfectly_plastic",
  "vonmises_isotropic_hardening", "vonmises_linear_kinematic_hardening",
  "vonmises_linear_kinematic_hardening_accelerated",
  "vonmises_kinematic_hardening", "vonmises_perfectly_plastic_accelerated",
  "vonmises_isotropic_hardening_accelerated",
  "vonmises_kinematic_hardening_accelerated", "sanisand2008", "camclay",
  "camclay_accelerated", "sanisand2004",
  "druckerprager_isotropic_hardening",
  "druckerprager_isotropic_hardening_accelerated",
  "druckerprager_kinematic_hardening",
  "druckerprager_kinematic_hardening_accelerated",
  "druckerprager_perfectly_plastic",
  "druckerprager_perfectly_plastic_accelerated",
  "linear_elastic_crossanisotropic", "uniaxial_concrete02",
  "uniaxial_elastic_1d", "uniaxial_steel01", "uniaxial_steel02", "pisano",
  "vonmises_perfectly_plastic_LT", "pisanoLT", "New_PisanoLT",
  "linear_elastic_isotropic_3d_LT", "mass_density", "elastic_modulus",
  "viscoelastic_modulus", "poisson_ratio", "von_mises_radius",
  "druckerprager_angle", "armstrong_frederick_ha",
  "armstrong_frederick_cr", "initial_confining_stress",
  "isotropic_hardening_rate", "kinematic_hardening_rate",
  "poisson_ratio_h_v", "poisson_ratio_h_h", "shear_modulus_h_v",
  "elastic_modulus_horizontal", "elastic_modulus_vertical",
  "pressure_reference_p0", "initial_void_ratio", "initial_shear_modulus",
  "initial_bulk_modulus", "sanisand2008_Pat", "sanisand2008_k_c",
  "sanisand2008_K0", "sanisand2008_alpha_cc", "sanisand2008_c",
  "sanisand2008_xi", "sanisand2008_lambda", "sanisand2008_ec_ref",
  "sanisand2008_m", "sanisand2008_h0", "sanisand2008_ch",
  "sanisand2008_nb", "sanisand2008_A0", "sanisand2008_nd",
  "sanisand2008_p_r", "sanisand2008_rho_c", "sanisand2008_theta_c",
  "sanisand2008_X", "sanisand2008_z_max", "sanisand2008_cz",
  "sanisand2008_p0", "sanisand2008_p_in", "sanisand2008_G0",
  "sanisand2004_K0", "sanisand2004_Pat", "e0", "sanisand2004_G0",
  "sanisand2004_p_cut", "sanisand2004_Mc", "sanisand2004_c",
  "sanisand2004_lambda_c", "sanisand2004_xi", "sanisand2004_ec_ref",
  "sanisand2004_m", "sanisand2004_h0", "sanisand2004_ch",
  "sanisand2004_nb", "sanisand2004_A0", "sanisand2004_nd",
  "sanisand2004_z_max", "sanisand2004_cz", "reference_void_ratio",
  "critical_stress_ratio_M", "minimum_bulk_modulus",
  "initial_mean_pressure", "yield_strength", "strain_hardening_ratio",
  "compressive_strength", "strain_at_compressive_strength",
  "crushing_strength", "strain_at_crushing_strength", "tensile_strength",
  "tension_softening_stiffness", "M_in", "kd_in", "xi_in", "h_in", "m_in",
  "beta_min", "ax", "ay", "az", "verbose_level", "maximum_iterations",
  "tolerance", "beta", "gamma", "kappa", "lambda", "delta", "DOMAIN_",
  "startTime", "endTime", "Period", "Phase", "Amplitude", "frequency",
  "MaxTime", "frequency1", "frequency2", "frequency3", "frequency4",
  "stiffness_to_use_opt", "DAMPING_RAYLEIGH", "DAMPING_CAUGHEY3",
  "DAMPING_CAUGHEY4", "a0", "a1", "stiffness_to_use", "a2", "a3", "a4",
  "R0", "cR1", "cR2", "MASS", "mx", "my", "mz", "Imx", "Imy", "Imz",
  "equaldof", "master", "slave", "dof_to_constrain", "of", "OUTPUT",
  "BINARY", "TEXT", "ENABLE", "DISABLE", "'('", "','", "')'", "'['", "']'",
  "'.'", "';'", "'{'", "'}'", "$accept", "dsl", "CMD_add", "CMD_fix",
  "dofchain", "CMD_define", "CMD_misc", "CMD_remove", "ADD_material",
  "ADD_element", "prompt", "exp", "stmt", "cpd_stmt", "list", YY_NULLPTR
  };


  const unsigned short int
  feiparser::yyrline_[] =
  {
       0,   265,   265,   279,   289,   312,   325,   336,   424,   432,
     438,   439,   440,   441,   442,   446,   456,   460,   501,   520,
     524,   537,   541,   561,   579,   605,   634,   664,   701,   732,
     772,   792,   825,   861,   876,   891,   943,   992,  1014,  1041,
    1066,  1084,  1103,  1122,  1138,  1154,  1172,  1193,  1230,  1282,
    1330,  1374,  1390,  1420,  1460,  1474,  1496,  1525,  1529,  1538,
    1547,  1556,  1569,  1617,  1654,  1683,  1697,  1710,  1737,  1775,
    1803,  1816,  1832,  1855,  1869,  1893,  1917,  1941,  1965,  1994,
    2007,  2020,  2034,  2046,  2067,  2085,  2119,  2148,  2177,  2211,
    2242,  2279,  2314,  2347,  2378,  2461,  2508,  2549,  2623,  2663,
    2698,  2741,  2780,  2818,  2851,  2876,  2891,  2920,  2955,  2984,
    3022,  3060,  3079,  3123,  3149,  3174,  3199,  3226,  3273,  3320,
    3369,  3416,  3467,  3507,  3549,  3589,  3636,  3674,  3728,  3792,
    3856,  3909,  3933,  3959,  4011,  4046,  4072,  4098,  4122,  4147,
    4339,  4381,  4423,  4438,  4483,  4490,  4497,  4504,  4511,  4518,
    4525,  4531,  4538,  4546,  4554,  4562,  4570,  4578,  4582,  4588,
    4593,  4599,  4605,  4611,  4617,  4623,  4631,  4637,  4642,  4647,
    4652,  4657,  4662,  4670,  4701,  4706,  4710,  4719,  4741,  4766,
    4786,  4804,  4815,  4825,  4831,  4839,  4843
  };

  // Print the state stack on the debug stream.
  void
  feiparser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  feiparser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  feiparser::token_number_type
  feiparser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,     2,     2,    38,     2,     2,
     399,   401,    36,    34,   400,    35,   404,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   405,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   402,     2,   403,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   406,     2,   407,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    40,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398
    };
    const unsigned int user_token_number_max_ = 645;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 8260 "feiparser.tab.cc" // lalr1.cc:1155
#line 4871 "feiparser.yy" // lalr1.cc:1156


// Deletes all the nodes that were allocated
void clear_stack ()
{
  while (!nodes.empty ())
  {
    delete nodes.top ();
    nodes.pop ();
  }
}

//Define what happens when an error is encountered.
//FIXME: More verbose error reporting... line number, column, etc. (jaabell)
//FIXME: command suggestions are not working properly.
//NOTE: This doesnt work on windows
namespace yy {
    void feiparser::error(location const& loc, const string& s)
    {
        cerr << " \n <!!!>  Error at ---> " << loc << ": " << s << endl << endl;
        ifstream file_for_error_reporting(loc.begin.filename->c_str());

        unsigned int begin_line = loc.begin.line;
        unsigned int end_line = loc.end.line;
        unsigned int begin_column = loc.begin.column;
        unsigned int end_column = loc.end.column;

        string line;
        string fullcommand;
        unsigned int linenumber = 1;
        unsigned int lastprintedline = 1;
        while(linenumber <= end_line)
        {
            getline(file_for_error_reporting, line);

            if (linenumber >= begin_line)
            {
                if(linenumber == begin_line)
                {
                    if( line.size() <= size_t(begin_column) )
                        line = "";
                    if (line.size() > size_t(begin_column) )
                        line = line.substr(begin_column);
                }

                fullcommand += line;

                if(line.size() > 0)
                {
                    if(linenumber > lastprintedline + 1)
                        cerr << ">skip " << setw(3) << linenumber - lastprintedline - 1<< endl;
                    cerr << "line " << setw(4) << linenumber << ": " << line << endl;
                    lastprintedline = linenumber;
                }
                if (linenumber == end_line)
                {
                    unsigned int columnnumber = 1;

                    while(columnnumber <= end_column+10)
                    {
                        cerr << " ";
                        columnnumber++;
                    }
                    cerr << "^";
                    //cerr << endl;
                }
            }
            linenumber ++;
        }

        file_for_error_reporting.close();
        //cerr << endl;
        //cerr << "Full command: " << fullcommand << endl << endl;
//        cmd_advisor.Suggest(fullcommand);

        cout << endl;
    }
}

// Transforms a dofstring into a dof number according to conventions described in the manual... and
// evident from reading below.
Number* dof2number(string dof)
{
    if (dof.compare("ux") == 0) return new Number(1, unitless);
    if (dof.compare("uy") == 0) return new Number(2, unitless);
    if (dof.compare("uz") == 0) return new Number(3, unitless);
    if (dof.compare("rx") == 0) return new Number(4, unitless);
    if (dof.compare("ry") == 0) return new Number(5, unitless);
    if (dof.compare("rz") == 0) return new Number(6, unitless);
    if (dof.compare("Ux") == 0) return new Number(5, unitless);
    if (dof.compare("Uy") == 0) return new Number(6, unitless);
    if (dof.compare("Uz") == 0) return new Number(7, unitless);
    if (dof.compare("p")  == 0) return new Number(4, unitless);

    return new Number(-100, unitless);
}


// Returns the appropiate unit checking function pointer according to the
// type of DOF specified in dof string.
UnitCheckerFunctionPointerType dof2signature(string dof)
{
    if (dof.compare("ux") == 0) return &isLength;
    if (dof.compare("uy") == 0) return &isLength;
    if (dof.compare("uz") == 0) return &isLength;
    if (dof.compare("rx") == 0) return &isAdimensional;
    if (dof.compare("ry") == 0) return &isAdimensional;
    if (dof.compare("rz") == 0) return &isAdimensional;
    if (dof.compare("Ux") == 0) return &isLength;
    if (dof.compare("Uy") == 0) return &isLength;
    if (dof.compare("Uz") == 0) return &isLength;
    if (dof.compare("p") == 0)  return &isPressure;

    return &isAdimensional;
}

// Transforms a force string into a dof number.
Number* force2number(string dof)
{
    if (dof.compare("Fx") == 0) return new Number(1, unitless);
    if (dof.compare("Fy") == 0) return new Number(2, unitless);
    if (dof.compare("Fz") == 0) return new Number(3, unitless);
    if (dof.compare("Mx") == 0) return new Number(4, unitless);
    if (dof.compare("My") == 0) return new Number(5, unitless);
    if (dof.compare("Mz") == 0) return new Number(6, unitless);
    if (dof.compare("F_fluid_x") == 0) return new Number(5, unitless);
    if (dof.compare("F_fluid_y") == 0) return new Number(6, unitless);
    if (dof.compare("F_fluid_z") == 0) return new Number(7, unitless);

    return new Number(-100, unitless);
}

// Returns the appropiate unit checking function pointer according to the
// type of DOF specified in dof string.
UnitCheckerFunctionPointerType force2signature(string dof)
{
    if (dof.compare("Fx") == 0) return &isForce;
    if (dof.compare("Fy") == 0) return &isForce;
    if (dof.compare("Fz") == 0) return &isForce;
    if (dof.compare("Mx") == 0) return &isTorque;
    if (dof.compare("My") == 0) return &isTorque;
    if (dof.compare("Mz") == 0) return &isTorque;
    if (dof.compare("F_fluid_x") == 0) return &isForce;
    if (dof.compare("F_fluid_y") == 0) return &isForce;
    if (dof.compare("F_fluid_z") == 0) return &isForce;

    return &isAdimensional;
}




// Returns appropriate stiffness which corresponds to a certain dof name.
UnitCheckerFunctionPointerType dof2stiffnesschecker(string dof)
{
    if (dof.compare("ux") == 0) return &isThisUnit<1, 0, -2>;   //Check for units of Force / Length = kg/s^2;
    if (dof.compare("uy") == 0) return &isThisUnit<1, 0, -2>;
    if (dof.compare("uz") == 0) return &isThisUnit<1, 0, -2>;
    if (dof.compare("rx") == 0) return &isTorque;
    if (dof.compare("ry") == 0) return &isTorque;
    if (dof.compare("rz") == 0) return &isTorque;
    if (dof.compare("Ux") == 0) return &isThisUnit<1, 0, -2>;
    if (dof.compare("Uy") == 0) return &isThisUnit<1, 0, -2>;
    if (dof.compare("Uz") == 0) return &isThisUnit<1, 0, -2>;
    if (dof.compare("p") == 0)  return &isPressure;

    return &isAdimensional;
}


void set_model_name(char* cstring)
{
    string modelNameStringFromInput(cstring);
    //ModelName = modelNameStringFromInput;
    define_model_name(modelNameStringFromInput);
}


void set_outcppfile_name(string newfilename)
{
    out_cpp_filename = newfilename;
    if (out_cpp_filename.find(".cpp") == string::npos )
    {
        out_cpp_filename += ".cpp";
    }

    if(FLAG_generate_cpp_output)
    {
        // Point output to this new file.
        fstream *outputcppfile = new fstream(out_cpp_filename.c_str(), ios::out);
        FeiDslCallerBase::cppfile = outputcppfile;

        *outputcppfile << "#include \"CPPIncludes.h\"" << endl ;
        *outputcppfile << endl;
        *outputcppfile << endl;
        *outputcppfile << "int main()" << endl;
        *outputcppfile << "{" << endl;
    }
}

void set_dry_run_mode(int flag)
{
    if( flag <= 0)
    {
        FeiDslCallerBase::dry_run = false;
    }
    else
    {
        FeiDslCallerBase::dry_run = true;
        cout << "\n\n\n";
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        cout << "!! NOTE: DRY RUN. No actual calls to ESSI API are being made. !!\n";
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        cout << "\n\n\n";
    }
}



























// That's all folks.



