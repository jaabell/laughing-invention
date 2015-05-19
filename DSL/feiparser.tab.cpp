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
    void set_profiling_results_filename(string filename);
    void sync_global_timers();

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


#line 135 "feiparser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "feiparser.tab.hh"

// User implementation prologue.
#line 244 "feiparser.yy" // lalr1.cc:407

extern int yylex(yy::feiparser::semantic_type *yylval,
                 yy::feiparser::location_type *yylloc);

#line 153 "feiparser.tab.cc" // lalr1.cc:407


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
#line 239 "feiparser.tab.cc" // lalr1.cc:474

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
    #line 249 "feiparser.yy" // lalr1.cc:725
{
    yyla.location.begin.filename = yyla.location.end.filename = new std::string(curfilename);
}

#line 542 "feiparser.tab.cc" // lalr1.cc:725

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
#line 270 "feiparser.yy" // lalr1.cc:847
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
#line 665 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 284 "feiparser.yy" // lalr1.cc:847
    {
        //theDomain.Print(cerr);
        cerr << "Not currently working. Should be implemented as API function and used here." << endl;
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 676 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 294 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "output_of_element_to_screen");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 690 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 317 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_node_to_screen, args, signature, "output_of_node_to_screen");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 704 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 330 "feiparser.yy" // lalr1.cc:847
    {
        cerr << "Not currently working. Should be implemented a API function and used here." << endl;
        //theDomain.getNDMaterial( $4->value().Getvalue() ) -> Print(cerr,0);
        (yylhs.value.exp) = new Empty();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 716 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 341 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("filename", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<string>(&check_mesh, args, signature,"check_mesh");

        nodes.pop();
        nodes.push((yylhs.value.exp));
        }
#line 730 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 429 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
        nodes.push((yylhs.value.exp));
    }
#line 739 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 437 "feiparser.yy" // lalr1.cc:847
    {
        cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 749 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 442 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 755 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 443 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 761 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 444 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 767 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 445 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 773 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 446 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 779 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 450 "feiparser.yy" // lalr1.cc:847
    {
                                    cout << "    *** Commands will now be execued. " << endl << endl;
                                    recovery_mode = 0;
                                    prompt = thePrompt;
                                    (yylhs.value.exp) = new Empty();
                                    nodes.push((yylhs.value.exp));
                                }
#line 791 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 460 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 797 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 465 "feiparser.yy" // lalr1.cc:847
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
#line 839 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 506 "feiparser.yy" // lalr1.cc:847
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
#line 862 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 524 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 868 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 529 "feiparser.yy" // lalr1.cc:847
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
#line 885 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 541 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 891 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 546 "feiparser.yy" // lalr1.cc:847
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
#line 912 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 567 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_element", &isAdimensional));
        args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

        for(int ii = 1;ii <=3; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 930 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 585 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();


        args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_node", &isAdimensional));
        args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_node, args, signature, "add_load_selfweight_to_node");

        for(int ii = 1;ii <=3; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 948 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 604 "feiparser.yy" // lalr1.cc:847
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
#line 973 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 630 "feiparser.yy" // lalr1.cc:847
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
#line 1001 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 659 "feiparser.yy" // lalr1.cc:847
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
#line 1030 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 689 "feiparser.yy" // lalr1.cc:847
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
#line 1066 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 726 "feiparser.yy" // lalr1.cc:847
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
#line 1096 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 757 "feiparser.yy" // lalr1.cc:847
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
#line 1135 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 795 "feiparser.yy" // lalr1.cc:847
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
#line 1156 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 817 "feiparser.yy" // lalr1.cc:847
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
#line 1188 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 851 "feiparser.yy" // lalr1.cc:847
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
#line 1222 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 884 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[2].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

        for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 1238 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 899 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
        args.push_back(new Number(-1, unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

        nodes.pop(); //pop 1 exps
        nodes.push((yylhs.value.exp));
    }
#line 1254 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 921 "feiparser.yy" // lalr1.cc:847
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
#line 1300 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 972 "feiparser.yy" // lalr1.cc:847
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
#line 1344 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 1089 "feiparser.yy" // lalr1.cc:847
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
#line 1362 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 1107 "feiparser.yy" // lalr1.cc:847
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
#line 1381 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 1126 "feiparser.yy" // lalr1.cc:847
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
#line 1401 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 1145 "feiparser.yy" // lalr1.cc:847
    {
        //add_damping_to_element(int elementNumber, int dampingNumber)
        args.clear(); signature.clear();

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("element_number",    &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1418 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 1161 "feiparser.yy" // lalr1.cc:847
    {
        //add_damping_to_node(int nodeNumber, int dampingNumber)
        args.clear(); signature.clear();

        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("node_number",    &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1435 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 1177 "feiparser.yy" // lalr1.cc:847
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
#line 1454 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 1195 "feiparser.yy" // lalr1.cc:847
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
#line 1476 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 1216 "feiparser.yy" // lalr1.cc:847
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
#line 1514 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 1265 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("time_step",            &isTime));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("scale_factor",         &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("number_of_steps",       &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("number_of_boundary_nodes",     &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number_of_exterior_nodes",     &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("number_of_drm_elements",   &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("element_file",       &isAdimensional));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("boundary_nodes_file",         &isAdimensional));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("exterior_nodes_file",         &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("displacement_file",  &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("acceleration_file",  &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller12<int,
        double,double,
            int,int,int,int,
            string,string,string,
            string,string>(&add_load_pattern_domain_reduction_method, args, signature, "add_load_pattern_domain_reduction_method");

        for(int i = 1; i <= 12; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1544 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 1296 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<int,string>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

        for(int i = 1; i <= 2; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1560 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 1311 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("section_number",           &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material_number",           &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("thickness",           &isLength));

        (yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

        for(int i = 1; i <= 3; i++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1577 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 1331 "feiparser.yy" // lalr1.cc:847
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
#line 1596 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 1357 "feiparser.yy" // lalr1.cc:847
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
#line 1637 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 1397 "feiparser.yy" // lalr1.cc:847
    {
        args.clear();           signature.clear();
        args.push_back((yystack_[2].value.exp));     signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1652 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 1411 "feiparser.yy" // lalr1.cc:847
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
#line 1675 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 1433 "feiparser.yy" // lalr1.cc:847
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
#line 1704 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 1461 "feiparser.yy" // lalr1.cc:847
    {
                            dofchain_holder.clear();
                            dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
                        }
#line 1713 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 1465 "feiparser.yy" // lalr1.cc:847
    {
                            dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
                        }
#line 1721 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1475 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
        nodes.push((yylhs.value.exp));
    }
#line 1731 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1484 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
        nodes.push((yylhs.value.exp));
    }
#line 1741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1491 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
        nodes.push((yylhs.value.exp));
    }
#line 1751 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1498 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
        (yylhs.value.exp) = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
        nodes.push((yylhs.value.exp));
    }
#line 1762 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1506 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("level", &isAdimensional));
        (yylhs.value.exp) = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
        nodes.push((yylhs.value.exp));
    }
#line 1773 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1516 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&disable_element_output,args,signature,"disable_element_output");
        nodes.push((yylhs.value.exp));
    }
#line 1783 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1526 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("factor", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 1797 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1539 "feiparser.yy" // lalr1.cc:847
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
#line 1846 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1587 "feiparser.yy" // lalr1.cc:847
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
#line 1884 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 1624 "feiparser.yy" // lalr1.cc:847
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
#line 1914 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 1653 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("gamma", &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

        for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 1929 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 1667 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

        nodes.pop(); //pop 2 exps
        nodes.push((yylhs.value.exp));
    }
#line 1943 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 1680 "feiparser.yy" // lalr1.cc:847
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
#line 1965 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 1707 "feiparser.yy" // lalr1.cc:847
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
#line 2004 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 1745 "feiparser.yy" // lalr1.cc:847
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
#line 2033 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 1773 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2047 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 1786 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("time_step", &isTime));
        args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));


        (yylhs.value.exp) = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

        nodes.pop();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2064 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 1806 "feiparser.yy" // lalr1.cc:847
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
#line 2084 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 1825 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back
        (this_signature("number_of_modes", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2099 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 1842 "feiparser.yy" // lalr1.cc:847
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
#line 2121 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 1866 "feiparser.yy" // lalr1.cc:847
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
#line 2143 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1890 "feiparser.yy" // lalr1.cc:847
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
#line 2165 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1914 "feiparser.yy" // lalr1.cc:847
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
#line 2187 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 1938 "feiparser.yy" // lalr1.cc:847
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
#line 2209 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 1960 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        (yylhs.value.exp) = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

    }
#line 2220 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 1976 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2234 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 1989 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2248 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 2002 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

        //pop 1
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2263 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 2016 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
             (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
             //pop 1
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2276 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 2028 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
             (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 2289 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 2052 "feiparser.yy" // lalr1.cc:847
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
#line 2305 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 2077 "feiparser.yy" // lalr1.cc:847
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
#line 2330 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2109 "feiparser.yy" // lalr1.cc:847
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
#line 2352 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2138 "feiparser.yy" // lalr1.cc:847
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
#line 2374 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2170 "feiparser.yy" // lalr1.cc:847
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
#line 2398 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2202 "feiparser.yy" // lalr1.cc:847
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
#line 2421 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2236 "feiparser.yy" // lalr1.cc:847
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
#line 2447 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2272 "feiparser.yy" // lalr1.cc:847
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
#line 2472 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2306 "feiparser.yy" // lalr1.cc:847
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
#line 2496 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2338 "feiparser.yy" // lalr1.cc:847
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
#line 2519 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2390 "feiparser.yy" // lalr1.cc:847
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
#line 2573 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2457 "feiparser.yy" // lalr1.cc:847
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
#line 2607 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2502 "feiparser.yy" // lalr1.cc:847
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
#line 2637 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2556 "feiparser.yy" // lalr1.cc:847
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
#line 2687 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2616 "feiparser.yy" // lalr1.cc:847
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
#line 2717 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2654 "feiparser.yy" // lalr1.cc:847
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
#line 2744 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2692 "feiparser.yy" // lalr1.cc:847
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
#line 2776 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2733 "feiparser.yy" // lalr1.cc:847
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
#line 2805 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2772 "feiparser.yy" // lalr1.cc:847
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
#line 2834 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2808 "feiparser.yy" // lalr1.cc:847
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
#line 2860 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2839 "feiparser.yy" // lalr1.cc:847
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
#line 2880 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2858 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
        (yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

        for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
            nodes.push((yylhs.value.exp));
    }
#line 2896 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2880 "feiparser.yy" // lalr1.cc:847
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
#line 2919 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2912 "feiparser.yy" // lalr1.cc:847
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
#line 2945 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 2944 "feiparser.yy" // lalr1.cc:847
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
#line 2968 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 2976 "feiparser.yy" // lalr1.cc:847
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
#line 2997 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 3014 "feiparser.yy" // lalr1.cc:847
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
#line 3026 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 3045 "feiparser.yy" // lalr1.cc:847
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
#line 3043 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 3073 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();

        args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("number"                    , &isAdimensional));
        args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("elastic_modulus_1atm"      , &isPressure));
        args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("poisson_ratio"            , &isAdimensional));
        args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("M_in"                     , &isAdimensional));
        args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("kd_in"                    , &isAdimensional));
        args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("xi_in"                    , &isAdimensional));
        args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("h_in"                     , &isAdimensional));
        args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("m_in"                     , &isAdimensional));
        args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("mass_density"             , &isDensity));
        args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress" , &isPressure));
        args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("n_in"                     , &isAdimensional));
        args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a_in"                     , &isAdimensional));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("eplcum_cr_in"             , &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller13<int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_constitutive_model_NDMaterialLT_New_Pisano, args, signature, "add_constitutive_model_NDMaterialLT_New_Pisano");

        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 3074 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 3115 "feiparser.yy" // lalr1.cc:847
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
#line 3098 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 3141 "feiparser.yy" // lalr1.cc:847
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
#line 3121 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 3165 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        args.push_back((yystack_[16].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
        args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
        args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
        args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
        args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

        (yylhs.value.exp) = new FeiDslCaller5<int,int,int,double,int>(&add_element_shear_beam_ltensor, args, signature, "add_element_shear_beam_ltensor");

        for(int ii = 1;ii <=5; ii++) nodes.pop(); //pop 5 exps
        nodes.push((yylhs.value.exp));
    }
#line 3139 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3185 "feiparser.yy" // lalr1.cc:847
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
#line 3162 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3209 "feiparser.yy" // lalr1.cc:847
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
#line 3188 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3238 "feiparser.yy" // lalr1.cc:847
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
#line 3232 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3285 "feiparser.yy" // lalr1.cc:847
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
#line 3276 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3332 "feiparser.yy" // lalr1.cc:847
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
#line 3321 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3381 "feiparser.yy" // lalr1.cc:847
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
#line 3365 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3428 "feiparser.yy" // lalr1.cc:847
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
#line 3412 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3479 "feiparser.yy" // lalr1.cc:847
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
#line 3449 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3519 "feiparser.yy" // lalr1.cc:847
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
#line 3487 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3561 "feiparser.yy" // lalr1.cc:847
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
#line 3524 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3609 "feiparser.yy" // lalr1.cc:847
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
#line 3560 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3649 "feiparser.yy" // lalr1.cc:847
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
#line 3594 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3688 "feiparser.yy" // lalr1.cc:847
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
#line 3643 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3747 "feiparser.yy" // lalr1.cc:847
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
#line 3697 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3811 "feiparser.yy" // lalr1.cc:847
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
#line 3751 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3872 "feiparser.yy" // lalr1.cc:847
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
#line 3797 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 3919 "feiparser.yy" // lalr1.cc:847
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
#line 3820 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 3943 "feiparser.yy" // lalr1.cc:847
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
#line 3845 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 3978 "feiparser.yy" // lalr1.cc:847
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
#line 3887 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 4025 "feiparser.yy" // lalr1.cc:847
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
#line 3917 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 4057 "feiparser.yy" // lalr1.cc:847
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
#line 3941 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 4083 "feiparser.yy" // lalr1.cc:847
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
#line 3965 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 4109 "feiparser.yy" // lalr1.cc:847
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
#line 3987 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4133 "feiparser.yy" // lalr1.cc:847
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
#line 4010 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4159 "feiparser.yy" // lalr1.cc:847
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
#line 4030 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4347 "feiparser.yy" // lalr1.cc:847
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
#line 4076 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4389 "feiparser.yy" // lalr1.cc:847
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
#line 4122 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4431 "feiparser.yy" // lalr1.cc:847
    {
        clear_stack ();

        if ( recovery_mode == 0)
        {
            //Tell lexer to discard every file in stack and go into interactive safe mode
            error_behavior();
        }

        (yylhs.value.errcode) = 0;
    }
#line 4138 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4446 "feiparser.yy" // lalr1.cc:847
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
#line 4187 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4491 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  //  The children are handled by Plus so we
        nodes.pop ();  // take them of the allocated nodes stack.
        nodes.push ((yylhs.value.exp));
    }
#line 4198 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4498 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  //  The children are handled by Plus so we
        nodes.pop ();  // take them of the allocated nodes stack.
        nodes.push ((yylhs.value.exp));
    }
#line 4209 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4505 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4220 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4512 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4231 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4519 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4242 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4526 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4253 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4533 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
        nodes.pop ();  // The same as above.
        nodes.push ((yylhs.value.exp));
    }
#line 4263 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4539 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4274 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4546 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4286 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4554 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4298 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4562 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4310 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4570 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4322 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4578 "feiparser.yy" // lalr1.cc:847
    {
        Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
        nodes.pop ();  // The same as above.
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4334 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4586 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4342 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4590 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
        nodes.pop ();
        nodes.push ((yylhs.value.exp));
    }
#line 4352 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4596 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Number (*(yystack_[0].value.value));
        nodes.push ((yylhs.value.exp));
    }
#line 4361 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4601 "feiparser.yy" // lalr1.cc:847
    {

        (yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
        nodes.push((yylhs.value.exp));
    }
#line 4371 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4607 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4381 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4613 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

        nodes.push((yylhs.value.exp));
    }
#line 4391 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4619 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4401 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4625 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4411 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4631 "feiparser.yy" // lalr1.cc:847
    {
        string temp = *(yystack_[0].value.ident);
        temp.erase(0, 1);                               //remove quotes
        temp.erase(temp.length()-1, temp.length());     //remove quotes
        (yylhs.value.exp) = new FeiString(temp);
        nodes.push((yylhs.value.exp));
    }
#line 4423 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4639 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
        nodes.push((yylhs.value.exp));
    }
#line 4433 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4645 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
        nodes.push((yylhs.value.exp));
    }
#line 4442 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4650 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
        nodes.push((yylhs.value.exp));
    }
#line 4451 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4655 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
        nodes.push((yylhs.value.exp));
    }
#line 4460 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4660 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
        nodes.push((yylhs.value.exp));
    }
#line 4469 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4665 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
        nodes.push((yylhs.value.exp));
    }
#line 4478 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4670 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
        nodes.push((yylhs.value.exp));
    }
#line 4487 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4678 "feiparser.yy" // lalr1.cc:847
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
#line 4519 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4709 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 4528 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4714 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4536 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4718 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[1].value.exp);
        args.clear();
        signature.clear();
    }
#line 4546 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4727 "feiparser.yy" // lalr1.cc:847
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
#line 4569 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4749 "feiparser.yy" // lalr1.cc:847
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
#line 4595 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4774 "feiparser.yy" // lalr1.cc:847
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
#line 4619 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4794 "feiparser.yy" // lalr1.cc:847
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
#line 4641 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4812 "feiparser.yy" // lalr1.cc:847
    {
        args.clear(); signature.clear();
        if (FLAG_generate_cpp_output == 1)
        {
            *FeiDslCallerBase::cppfile << "}" << endl;
        }

        (yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
        nodes.push((yylhs.value.exp));
    }
#line 4656 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4823 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = new Empty();
        nodes.pop();
        nodes.push((yylhs.value.exp));
    }
#line 4666 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4833 "feiparser.yy" // lalr1.cc:847
    {
        // empty statement generates an  empty expression
        (yylhs.value.exp) = new Empty();
        nodes.push((yylhs.value.exp));
    }
#line 4676 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 187:
#line 4839 "feiparser.yy" // lalr1.cc:847
    {
        // Used to initialize a list of statements (compound statement)
        (yylhs.value.exp) = (yystack_[1].value.exp);
    }
#line 4685 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 188:
#line 4847 "feiparser.yy" // lalr1.cc:847
    {
        (yylhs.value.exp) = (yystack_[0].value.exp);
    }
#line 4693 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 189:
#line 4851 "feiparser.yy" // lalr1.cc:847
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
#line 4709 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4713 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -363;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
     273,  -363,   347,  -363,  -363,  -362,  -350,   612,   612,  -363,
    -363,    66,  -363,  -363,  -363,  -363,  -363,  -363,  -363,   612,
     612,    31,  -363,  -363,    32,    41,  -363,  5721,  -309,  -276,
      76,    90,   185,   208,   370,    98,    46,    56,   -46,   -45,
     612,  -363,  -232,  -363,  -363,  -363,  -363,  -363,   178,   -19,
    -363,   181,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   -31,   -31,  -363,   146,   146,   -88,    80,    82,  -363,
    9987,   612,   187,    87,    89,    79,    92,    93,   102,   103,
    -181,    94,   107,   109,   100,    43,   115,  -363,   612,  -177,
     121,   133,   141,   143,   155,   158,  -125,   144,   191,   193,
     189,   -44,   190,   188,   120,  9634,   224,   170,   612,  -103,
    -363,  -102,  -363,   128,  -363,  -363,  -363,   612,   612,   612,
     612,   612,   612,   612,  -363,  -363,  9987,  9987,  9987,  9987,
    9987,  9987,  9987,   349,   396,  2767,   295,  -363,   612,   612,
     612,  9987,  -363,   612,   612,  -363,   192,   612,   612,   612,
     612,   201,   200,   612,   612,   194,   247,   258,    40,   198,
     612,   612,   612,   612,   612,   612,   261,   196,  -363,   -34,
    -363,   222,   233,   235,   251,   -41,   246,  -363,   342,  9654,
    -363,  -363,  -363,    67,    67,    51,    51,    29,   146,   310,
    -363,  -363,  -363,  2820,   -61,   -56,  9987,  9987,  9987,  9822,
    9582,   612,  9591,  9710,  9719,  9569,   -35,   612,  9606,  5871,
     612,   248,   267,   -51,   268,  9748,  9761,  9987,  9987,  9987,
    9987,   280,   612,   357,   184,    33,   350,   305,   241,   242,
     -21,   -12,  -363,  -363,  -363,  -363,  -363,     1,  9902,  9987,
    9818,     0,   355,    53,  -346,   356,  9327,  -109,   379,  9798,
     364,   612,   416,   612,   -39,   308,   612,  9987,   612,   392,
     393,   318,   403,   210,   264,   -37,   270,   362,   363,   372,
     612,   -69,   -66,   353,   354,   -65,   359,   360,   368,   365,
     401,   402,   404,   405,   406,   409,   411,   414,   434,   458,
     459,   461,   462,   469,   470,   473,   311,   314,   316,   317,
     319,   320,   325,   326,   327,   328,   336,   345,   352,   366,
     367,   376,   377,   380,   382,   383,   127,   348,   135,   182,
     378,   385,   391,   237,   395,   520,   523,   525,   553,   556,
     527,   568,   572,   571,   648,   650,   445,   417,   612,   612,
     577,  1902,  -363,  9987,  -363,   308,  -363,  -363,  9987,  5834,
     612,   612,   612,   612,   522,   512,   514,   518,   521,  -363,
     601,   607,   223,   612,   594,   612,   641,   642,   644,   612,
     651,   654,   656,   612,   657,   660,   661,   663,   664,   673,
     734,   676,   677,   678,   685,   686,   688,   689,   691,   692,
     693,   682,   781,   782,   784,   786,   795,   803,   804,   809,
     810,   811,   818,   820,   822,   823,   824,   826,   827,   828,
     829,   830,   832,   834,   838,   845,   846,   847,   848,   850,
     596,   612,   612,   612,   612,   441,   509,   510,   612,   612,
     612,   859,   860,  5861,  9770,   612,   861,  -363,   869,  9987,
    5820,  9663,  9987,   728,   801,   802,   813,   743,   888,   894,
     612,  8435,   528,  8446,   529,   530,   532,  8455,   533,   538,
     545,  8465,   546,   548,   549,   550,   551,   552,   560,   561,
     563,   564,   565,   566,   567,   603,   604,   606,   609,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   805,  9615,
    9625,  9676,  9987,  9987,   895,   896,   897,  9808,  9555,  9987,
     612,   612,   980,  -363,   299,   612,   612,   984,   908,   868,
     966,   990,  1001,   927,   612,   612,  1039,   889,   612,   891,
     612,   612,   612,   892,   612,   612,   612,   893,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,  7555,  7564,  7577,  7586,  7597,
    7608,  7617,  7627,  7636,  6385,  6223,  6232,  6399,  7645,  7660,
    7670,  7679,  7693,  7702,  6705,  6145,  7543,  7712,  7724,  7293,
    7737,  7302,  7315,  7747,   924,   477,    -2,   955,   612,   612,
     612,   694,  1046,  9728,  7324,   612,   973,  1484,  5843,   612,
     959,   979,   981,   991,   999,  1042,  9987,  9845,   612,   928,
    1465,   936,  1875,  1893,  1913,   937,  1954,  1965,  2062,   939,
    2071,  2127,  2136,  2148,  2178,  2228,   410,  2251,  2287,  2301,
    2329,  2410,  2447,  2494,  2522,  2552,  2581,  1095,  1096,  1098,
    1099,  1107,  1108,  1116,  1117,  1119,  1120,  1121,  1123,  1124,
    1125,  1126,  1128,  1129,  1130,  1135,  1156,  1159,  1189,  1191,
    1192,  1193,  1194,  1195,  1198,  1199,  -363,  1118,  1131,  1132,
    -363,  1133,  1134,  1145,   -73,  2188,  5697,  5706,  1180,   612,
    1196,  1216,  5852,  1187,  1222,  1223,  9987,   612,  1201,   612,
     612,   612,  1150,  1234,   419,  1033,   612,  1036,   612,   612,
     612,  1043,   612,   612,   612,  1052,   612,   612,   612,   612,
     612,   612,  1063,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,  1258,  1261,  1263,  1224,  1226,  1183,  1268,  1269,
    1270,  1271,  1287,  1212,  9496,  1213,   612,  1290,  1140,   612,
     612,  9987,  1218,  9259,  9270,  9280,   612,   612,  1219,  1225,
    2633,  1235,  2842,  2870,  2879,  1236,  2888,  2897,  2906,  1244,
    2915,  2926,   467,   487,   524,   535,  1299,   633,  2935,  2944,
    2955,  2964,   725,   747,  2973,  2982,   759,  7337,  7346,  7358,
    7368,  7377,  7386,  7396,  7405,  7420,  6419,  6205,  6214,  6374,
    7429,  7439,  7453,  7462,  7473,  7485,  6690,  6135,  9987,  6157,
    6166,  6063,  7497,  6076,  6090,  7506,   612,   612,   612,  1242,
    1245,   947,   612,   612,   612,   612,   612,   612,  1300,   612,
    7874,   612,   956,  1149,  9987,   612,  1323,  1324,  1326,  9289,
    9855,   612,  1246,   612,  1248,   612,   612,   612,  1250,   612,
     612,   612,  1251,   612,   612,  1255,  1160,  1171,  1142,   612,
    1172,   612,   612,   612,   612,  1266,  1151,   612,   612,  1173,
    1361,  1362,  1389,  1390,  1391,  1395,  1396,  1397,  1398,  1417,
    1418,  1420,  1421,  1425,  1433,  1442,  1445,  1446,  1447,  1448,
    1449,  1450,  1455,  1456,  1457,  1459,  1460,  1461,  9987,  9864,
    9307,   612,   612,   612,  9545,  9396,  5670,  5634,  5643,   621,
     612,  9987,  1462,  9987,   612,  1464,  9298,   612,   612,   612,
    1472,  1476,  9789,  1053,  2991,  1092,  3000,  3009,  3018,  1104,
    3027,  3036,  3045,  1105,  3054,  3063,   612,  1479,  1480,  1482,
    9685,  1483,  3072,  3081,   774,  3090,  1467,  1492,  3099,  3108,
    1494,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,  1495,
    1496,  9987,  9987,  3117,  1497,  1498,  1506,  1515,  1518,   308,
    9487,   612,  9987,   612,  1532,  9207,  9221,  9230,   612,   612,
    -363,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,  9700,   612,   612,   612,  1454,   612,
     612,   612,  1422,   612,  1458,   612,   612,   612,   612,  9987,
    7199,  7212,  7221,  7233,  7245,  7256,  7265,  7274,  6640,  5762,
    5773,  7516,  7135,  7144,  7153,  7163,  7178,  7187,  6742,  6120,
    5715,  1942,  6029,  7284,  6045,  6054,  9987,   612,  1563,   612,
     612,   612,  1197,   612,   612,  -363,  1540,  7784,  1336,   612,
    1570,  1572,  1573,  9240,  9836,  3126,  3148,  3157,  3166,  3175,
    3199,  3208,  3217,  3226,  3236,  3245,  3254,  3263,  1500,  7756,
    7765,  1027,  -363,  7774,   787,   862,  1550,   871,   612,  8003,
    3272,  3281,  9739,  1576,  1585,  1586,  1588,  1589,  1593,  1594,
    1595,  1597,  1598,  1599,  1600,  1601,  1609,  1610,  1612,  1621,
    1624,  1625,  1627,  1630,  1631,  1633,  1636,  1641,  1643,  9318,
    -363,  3290,  9407,  9525,  -363,  5679,  1614,   612,  1646,  1647,
    9249,   612,   612,   612,  1648,  1649,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,  1582,
    1650,  1658,  1580,  1667,  1591,  1603,  1606,  1607,  8150,  1679,
     612,   612,  1659,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,  1692,
     612,  1693,  1696,  1697,  1698,  9478,   612,   612,  1699,  9159,
    9168,  9177,   612,   612,  3301,  3310,  3319,  3330,  3339,  3348,
    3357,  3366,  3375,  3384,  3393,  3402,  3411,  1653,   612,   612,
    -363,   612,  1677,  1703,   612,  1706,  1731,   612,  3420,  3429,
    1657,  6813,  6774,  6754,  6763,  7094,  6826,  6783,  7103,  6658,
    5791,  5808,  6410,  6792,  6804,  7116,  7126,  6835,  6849,  6727,
    5782,  5652,  1347,  6002,  6858,  6011,  6020,  1764,  3438,   612,
     612,  1399,   612,  1743,  9987,   137,   612,  1745,  1749,  1750,
    9189,  9987,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,  8115,  8124,  8141,  1682,
    1683,  7883,  1691,   612,  7994,   612,   612,   612,  1768,  1776,
    1781,  1782,  1784,  1785,  1787,  1788,  1789,  1790,  1792,  1794,
    1795,  1796,  1797,  1798,  1809,  1818,  1819,  1821,  1822,  1823,
    1824,  1825,  1827,  1828,  -363,   612,  9534,  9336,  -363,  5688,
     612,  1831,  9198,   612,   612,   612,  1834,  3447,  3456,  3465,
    3474,  3483,  3492,  3501,  3510,  3523,  3532,  3541,  3550,  3559,
    7793,  1844,  1845,  1846,   612,   612,  1847,   612,  7802,  1848,
    3574,  3583,  9987,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,    60,
    1882,  1885,  1886,  9469,   612,  1887,  9987,  9987,  9987,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,  1888,   612,   612,   612,  7897,  7908,   612,
    7917,  1889,   612,   612,   612,  9873,  6871,  6880,  6890,  7049,
    8869,  6899,  7058,  6649,  7525,  7534,  6365,  6919,  6933,  7071,
    7080,  9884,  8878,  6718,  6111,  2367,    83,  5955,  8888,  5964,
    5993,   612,  1729,   612,   612,  1533,  1900,   796,   612,  9987,
    3592,  3602,  3611,  3620,  3629,  3638,  3647,  3656,  3665,  3676,
    3685,  3694,  3705,   612,  8082,  8093,  8103,  1901,  1909,  9987,
    1910,   612,  7985,  3714,  3723,  1912,  1923,  1935,  1936,  1937,
    1938,  1940,  1941,  1949,  1950,  1952,  1961,  1972,  1973,  1975,
    1976,  1978,  1979,  1980,  1982,  2003,  2005,  2006,  2007,  2008,
    2011,  3732,    -8,  9349,  9505,  -363,   612,  2016,  9987,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,  8012,  2023,  2025,  2034,   612,   612,   612,  9987,
    2043,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
    1569,   612,  2047,  2048,  9455,   612,  3741,   880,  3750,  3759,
     911,  3768,  3777,  3786,  3795,   926,  3804,   969,  3813,  2050,
     612,   612,   612,  9987,  9987,  9987,   612,  1018,  3822,  9059,
    9893,  9905,  8897,  6942,  8678,  8910,  6955,  6628,  6182,  6191,
    6356,  9931,  8919,  6967,  6976,  9068,  8689,  9987,  6102,  2027,
    5724,  5926,  8698,  5935,  5944,  3831,   612,  9987,   612,   612,
    2051,  9987,   612,  1836,   612,   612,  1877,   612,   612,   612,
     612,  1977,   612,  1981,   612,   612,  8021,  8030,  8039,  7931,
    1984,   612,  2053,  2055,  2056,  2059,  2069,  2070,  2078,  2079,
    2081,  2082,  2084,  2087,  2090,  2095,  2098,  2099,  2100,  2101,
    2120,  2121,  2122,  2123,  2124,  2125,  2134,   612,  3840,  9514,
    9359,   612,  3849,  2038,  3858,  3867,  2103,  3876,  3885,  3898,
    3907,  2119,  3916,  2144,  3925,  7941,  2146,  2147,  2173,  2174,
    2151,  3934,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,  3949,   612,  2176,
    2195,  9440,   612,  1951,   612,   612,  2072,   612,   612,   612,
     612,  2102,   612,  2112,   612,  2198,   612,   612,   612,  1826,
    2137,   612,  8928,  9082,  9091,  8709,  9940,  8474,  8719,  8942,
    6619,  6667,  6680,  6341,  9100,  8728,  9951,  8951,  8960,  8485,
    9987,  9987,  5661,  7815,  8494,  7824,  7834,   612,  3958,   612,
     612,  2200,  3968,   612,  3977,  3986,   612,  3995,  4004,  4013,
    4022,   612,  4031,   612,  4040,  1843,  8052,  8062,  8072,   612,
     612,  4051,  2221,  2224,  2235,  2236,  2238,  2239,  2240,  2241,
    2242,  2244,  2245,  2246,  2247,  2250,  2259,  2261,  2262,  2272,
    2275,  2295,  2297,  2298,  2300,   199,   612,  9368,  9987,   612,
     612,  9987,   612,   612,  8407,   612,   612,   612,   612,  8417,
     612,  9987,   612,   612,  2308,  2309,  2323,  4060,  9987,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,  2252,  4069,  2324,  9429,  1059,  4080,
    4089,  2326,  4098,  4107,  4116,  4125,  2336,  4134,  4143,  4152,
     612,   612,   612,   612,  4161,  8737,  8971,  8981,  8503,  9110,
    9987,  8517,  8746,  6610,  6989,  7001,  6332,  8990,  8526,  9121,
    8756,  8769,  9987,  5625,  7011,  9987,  7020,  7029,  1068,    -6,
     612,   612,  2337,  2263,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,  7846,  7856,  7865,  4170,   612,
    2341,  2342,  2343,  2344,  2345,  2346,  2347,  2355,  2360,  2363,
    2364,  2365,  2366,  2374,  2377,  2379,  2381,  2382,  2383,  2388,
    2256,  1928,   612,  1077,  9987,   612,  2371,  4179,  4188,  8377,
    4197,  4206,  4215,  4224,  8389,  4233,  4242,  4251,  2389,  2390,
    2400,   612,  4260,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,     3,   612,  9987,  -363,  9416,  2328,   612,
     612,  2405,   612,   612,   612,   612,  2406,   612,   612,   612,
     612,   612,   612,  1088,   612,  8537,  8778,  8787,  9987,  9000,
    9987,  8547,  6595,  9960,  9009,  6323,  8801,  9987,  9019,  8556,
    8568,  2036,  5907,  5917,  5898,  2044,   612,  4273,  2418,   612,
    4282,  4291,   612,  4300,  4309,  4324,  4334,   612,  4343,  4352,
    1100,  7950,  7961,  7974,  -363,  4361,  2420,  2421,  2422,  2423,
    2427,  2428,  2429,  2430,  2432,  2438,  2439,  2444,  2446,  2454,
    2455,  2457,  2458,   612,  9987,   612,   612,  9987,   612,   612,
    8346,   612,   612,   612,   612,  8358,   612,   612,  2260,  2459,
    2461,  2465,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   612,   612,
    4370,  4379,  9377,  4388,  4397,  2471,  4406,  4415,  4426,  4435,
    2472,  4444,  4455,  2474,  2107,  2108,  2109,  4464,  9987,  8586,
    8595,  8810,  9987,  6586,  9131,  8819,  6314,  8604,  8830,  9987,
    9987,  9987,  9987,  9987,  5889,   612,   612,  2475,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,  2111,   612,
     612,   612,   612,  2476,  2485,  2486,  2487,  2488,  2491,  2502,
    2504,  2507,  2508,  4473,  4482,   612,  4491,  4500,  8313,  4509,
    4518,  4527,  4536,  8322,  4545,  4554,   612,  4563,  4572,  4581,
    4590,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,  9987,   612,   612,  2511,   612,   612,   612,
     612,  2512,   612,   612,  4599,   612,   612,   612,   612,  9987,
    9987,  8615,  6570,  9028,  8627,  6305,  9987,  8636,  5880,  4608,
    4617,  4626,  4635,   612,  4648,  4657,  4666,  4675,   612,  4684,
    4698,   612,  4709,  4718,  4727,  4736,  2515,  2516,  2517,  2518,
    2529,  2530,  2532,   612,   612,   612,   612,  8282,   612,   612,
     612,   612,  8292,   612,   612,  4745,   612,   612,   612,   612,
     612,   612,   612,   612,   612,   612,   612,  4754,  4763,  4772,
    4781,  2533,  4790,  4801,  4810,  4819,  2534,  4830,  4839,   612,
    1109,  1203,  1227,  4848,  9987,  6561,  8839,  9987,  6295,  9987,
    9987,   612,   612,   612,   612,   612,   612,   612,   612,   612,
     612,   612,   612,  1238,  2340,  2348,  2349,   612,  2550,  2551,
    2559,  4857,  4866,  4875,  4884,  8252,  4893,  4902,  4911,  4920,
    8263,  4929,  4938,  2359,  2566,  2567,  2574,  4947,   612,   612,
     612,   612,   612,   612,   612,  2575,   612,   612,   612,   612,
    2576,   612,   612,  2577,  2207,  2208,  2218,   612,  6546,  8645,
    6286,  4956,  4965,  4974,  4983,   612,  4992,  5001,  5010,  5023,
     612,  5032,  5041,  2227,   612,   612,   612,  5050,  2601,  2603,
    2604,   612,   612,   612,   612,  8222,   612,   612,   612,   612,
    8231,   612,   612,   612,  5059,  5072,  5084,   612,   612,   612,
     612,  5093,  1247,  5102,  5111,  2605,  1301,  5120,  1310,  1426,
    2606,  5129,  5138,  5147,   612,   612,   612,  5156,  6537,  9987,
    6277,   612,  -363,   612,   612,   612,  2531,   612,  2539,  2540,
     612,   612,   612,   612,  5165,  5176,  5185,   612,  2607,  2609,
    5194,  5205,  5214,  8192,  2586,  5223,  2594,  2596,  8204,  5232,
    5241,  5250,   612,   612,   612,  5259,   612,   612,   612,   612,
     612,  2621,  2546,   612,  2547,  2548,  2626,   612,   612,   612,
    1435,  1499,  1508,   612,  6522,  6268,  1578,  5268,  5277,   612,
     612,  5286,   612,   612,   612,  5295,  5304,  1602,  2426,  2431,
    2440,  5313,  2631,  2643,  -363,   612,   612,  8160,  8426,   612,
    9987,  9987,  8173,   612,   612,  -363,  2644,  2645,  2647,   612,
     612,   612,  5322,  5331,  2648,  2650,  1651,  2651,  5340,  5349,
    2281,  2283,  2284,  5358,  6513,  6259,   612,   612,   612,   612,
    2583,   612,   612,   612,   612,   612,   612,   612,  2663,  2666,
    5367,  5376,  9987,  8398,  2646,  9987,  5385,  5398,  5407,  5416,
    5425,  5434,   612,   612,   612,   612,  2667,  2592,   612,   612,
     612,   612,   612,   612,  6504,  6250,  5446,  5459,   612,   612,
    5468,  5477,  5486,  5495,  5504,  5513,  2669,  2671,   612,   612,
    8368,  9987,   612,   612,   612,   612,   612,   612,   612,   612,
    5522,  5531,  2672,  5540,  5551,  1660,  1669,  1685,  5560,  6491,
    6241,   612,   612,   612,   612,   612,  -363,  -363,  -363,   612,
    2676,  2677,  5569,  1751,  8337,  5580,  1760,  1769,   612,   612,
     612,  2602,  2678,   612,  2608,  2610,  6482,  7038,  5589,  2656,
     612,  5598,  2658,  2659,  2684,  2686,   612,  2615,  8304,   612,
    2617,  2618,   612,   612,  5607,   612,  2695,  5616,   612,   612,
    6469,  9969,   612,  9987,   612,   612,  9987,  9987,  2697,  2698,
    1802,  8272,  1811,   612,   612,  2622,  2700,  2627,  6460,  9140,
    2681,   612,  2683,  2708,  2709,  2636,  8242,  2637,   612,   612,
     612,  2714,   612,  6451,  9037,  9987,   612,  9987,  2716,  2718,
    8213,   612,   612,  2721,  6441,  8849,   612,  2722,  2723,  8183,
     612,   612,  2725,  6428,  8659,   612,  2726,  2728,  9987,   612,
     612,  9978,  9987,  2729,   612,  9150,  2731,   612,  9050,  2732,
     612,  8860,  2734,   612,  8668,  2735,   612,  9987
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   145,   163,   162,   168,     0,     0,     0,     0,    15,
     183,     0,   169,   170,   171,   172,   173,   174,   175,     0,
       0,     0,     9,     8,     0,     0,   184,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,     0,    10,    12,    13,    11,    14,     0,     0,
     143,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   185,   161,   153,     0,     0,     0,     3,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,    57,     0,   179,     1,   144,     0,     0,     0,
       0,     0,     0,     0,   178,   165,   154,   155,   156,   157,
     158,   159,   164,     0,     0,     0,   180,   182,     0,     0,
       0,     7,    69,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      65,     0,     0,     0,     0,     0,     0,    80,     0,     0,
      58,    61,   160,   147,   148,   149,   150,   151,   152,   146,
     167,   166,   186,   188,     0,     0,     5,     4,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,    81,    84,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    70,    59,   189,   187,   181,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,    60,    51,    54,    50,    53,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,    67,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,    41,     0,     0,     0,     0,     0,    47,
       0,     0,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,     0,
       0,     0,     0,     0,     0,    45,     0,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,    79,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,     0,     0,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   138,   139,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
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
     110,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   118,     0,     0,     0,     0,     0,     0,   116,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,    26,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
      91,     0,     0,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,     0,   119,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,   103,
     104,   109,   111,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      93,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    98,     0,   102,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,     0,     0,     0,     0,
     125,   127,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   128,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,   132,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,     0,   123,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -363,  -363,  -363,  -363,  -253,  -363,  -363,  -363,  -363,  -363,
    -363,    -7,    22,   -36,  2580
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    42,    43,    44,   346,    45,    46,    47,    87,   145,
      48,    49,   193,   136,   194
  };

  const short int
  feiparser::yytable_[] =
  {
      61,    62,   347,   117,   118,   119,   120,   121,   122,   109,
     111,   123,    64,    65,    70,   117,   118,   119,   120,   121,
     122,   138,    50,   123,   768,   363,   137,   105,   365,   369,
     330,   331,   332,   113,     2,     3,     4,   364,     5,     6,
     366,   370,   171,   172,    59,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   325,   326,    60,    12,    13,    14,
      15,    16,    17,    18,   141,   344,    19,   769,   122,    63,
     116,   123,    20,   345,   117,   118,   119,   120,   121,   122,
     267,   158,   123,   268,    71,    66,    67,   269,    68,   121,
     122,    72,   437,   123,   117,   118,   119,   120,   121,   122,
      88,   179,   123,   119,   120,   121,   122,   328,   329,   123,
     183,   184,   185,   186,   187,   188,   189,   117,   118,   119,
     120,   121,   122,   228,    89,   123,   229,   230,   336,   337,
      90,   196,   197,   198,   682,   683,   199,   200,   356,   357,
     202,   203,   204,   205,    91,   265,   208,   209,   266,  1610,
    1611,  2001,  2002,   215,   216,   217,   218,   219,   220,   236,
    2085,  2086,   117,   118,   119,   120,   121,   122,   106,   107,
     123,   117,   118,   119,   120,   121,   122,   108,   115,   123,
     114,     2,     3,     4,   125,     5,     6,     7,   123,   139,
       8,   140,   142,     9,   239,    10,   143,    11,   144,   146,
     246,   147,   148,   249,    12,    13,    14,    15,    16,    17,
      18,   149,   150,    19,   151,   257,   153,   152,   154,    20,
     155,    21,    22,    23,    24,   156,   157,    25,   159,    26,
     160,    27,    28,   117,   118,   119,   120,   121,   122,    92,
      93,   123,   161,    94,   341,    95,   343,    29,    96,   348,
     162,   349,   163,    30,    31,    32,    33,   117,   118,   119,
     120,   121,   122,   362,   164,   123,    97,   165,    34,    35,
     166,   168,   167,   170,     1,   169,     2,     3,     4,   173,
       5,     6,     7,   175,   174,     8,    36,    98,     9,    99,
      10,   100,    11,   101,   178,   177,   102,   180,   181,    12,
      13,    14,    15,    16,    17,    18,   195,   206,    19,   207,
     211,   201,   212,   210,    20,   221,    21,    22,    23,    24,
     214,    88,    25,   223,    26,   222,    27,    28,   224,   226,
     227,   433,   434,   117,   118,   119,   120,   121,   122,   225,
     231,   123,    29,   439,   440,   441,   442,   232,    30,    31,
      32,    33,    -1,   235,   110,   112,   451,   135,   453,   250,
     252,   245,   457,    34,    35,    51,   461,    52,    53,    54,
      55,    56,    57,     2,     3,     4,   251,     5,     6,   253,
      58,    36,   135,   117,   118,   119,   120,   121,   122,   256,
     258,   123,   260,   124,    69,   259,    12,    13,    14,    15,
      16,    17,    18,   262,   261,    19,   263,   270,   264,   327,
     333,    20,   338,   509,   510,   511,   512,   513,   340,   342,
     345,   517,   518,   519,    28,   350,   351,   352,   524,   354,
     117,   118,   119,   120,   121,   122,   353,    40,   123,   355,
     358,   359,   360,   536,   117,   118,   119,   120,   121,   122,
     213,   361,   123,   117,   118,   119,   120,   121,   122,   367,
     368,   123,   373,   411,   103,   371,   372,  1491,  1492,   413,
    1546,   374,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   117,   118,   119,   120,   121,   122,   375,   376,   123,
     377,   378,   379,   603,   604,   380,   414,   381,   607,   608,
     382,   117,   118,   119,   120,   121,   122,   616,   617,   123,
    1371,   620,   104,   622,   623,   624,   182,   626,   627,   628,
     383,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,   117,   118,
     119,   120,   121,   122,   384,   385,   123,   386,   387,   117,
     118,   119,   120,   121,   122,   388,   389,   123,    37,   390,
     391,    38,    39,   392,    40,   393,   394,   418,   395,   396,
      41,   685,   686,   687,   397,   398,   399,   400,   692,     2,
       3,     4,   696,     5,     6,   401,  1913,  1914,   677,   678,
     679,   704,   680,   681,   402,     2,     3,     4,   412,     5,
       6,   403,    12,    13,    14,    15,    16,    17,    18,   420,
     450,    19,   421,   425,   422,   404,   405,    20,    12,    13,
      14,    15,    16,    17,    18,   406,   407,    19,   415,   408,
      28,   409,   410,    20,   416,   117,   118,   119,   120,   121,
     122,   417,   423,   123,   419,   424,    28,   117,   118,   119,
     120,   121,   122,    37,   426,   123,    38,    39,   427,    40,
     428,   429,   774,   430,   431,    41,   435,   432,   443,   444,
     781,   445,   783,   784,   785,   446,   447,   606,   448,   790,
     508,   792,   793,   794,   449,   796,   797,   798,   452,   800,
     801,   802,   803,   804,   805,   479,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   835,   836,   837,   838,   839,   840,
     841,   842,   843,   844,   845,   454,   455,   190,   456,   117,
     118,   119,   120,   121,   122,   458,  1095,   123,   459,   860,
     460,   462,   863,   864,   463,   464,    40,   465,   466,   869,
     870,   117,   118,   119,   120,   121,   122,   467,   468,   123,
     469,   470,   471,   117,   118,   119,   120,   121,   122,   472,
     473,   123,   474,   475,   191,   476,   477,   478,   117,   118,
     119,   120,   121,   122,   480,   481,   123,   482,   722,   483,
     514,   117,   118,   119,   120,   121,   122,   788,   484,   123,
     117,   118,   119,   120,   121,   122,   485,   486,   123,   928,
     929,   930,   487,   488,   489,   934,   935,   936,   937,   938,
     939,   490,   941,   491,   943,   492,   493,   494,   946,   495,
     496,   497,   498,   499,   952,   500,   954,   501,   956,   957,
     958,   502,   960,   961,   962,   885,   964,   965,   503,   504,
     505,   506,   970,   507,   972,   973,   974,   975,   515,   516,
     978,   979,   520,   521,   525,   886,   117,   118,   119,   120,
     121,   122,   526,   529,   123,   117,   118,   119,   120,   121,
     122,   530,   531,   123,   117,   118,   119,   120,   121,   122,
     533,   534,   123,   532,  1011,  1012,  1013,   535,   598,   599,
     600,   594,   887,  1020,   538,   540,   541,  1022,   542,   544,
    1025,  1026,  1027,   888,   545,   117,   118,   119,   120,   121,
     122,   546,   548,   123,   549,   550,   551,   552,   553,  1044,
     117,   118,   119,   120,   121,   122,   554,   555,   123,   556,
     557,   558,   559,   560,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,    40,   117,   118,   119,   120,   121,   122,   561,
     562,   123,   563,   605,  1097,   564,  1098,   609,    40,  1019,
     610,  1103,  1104,   612,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1117,   615,  1119,  1120,
    1121,   890,  1123,  1124,  1125,   611,  1127,   613,  1129,  1130,
    1131,  1132,   117,   118,   119,   120,   121,   122,   614,   676,
     123,   117,   118,   119,   120,   121,   122,   684,   619,   123,
     621,   625,   629,   117,   118,   119,   120,   121,   122,   689,
    1159,   123,  1161,  1162,  1163,   693,  1165,  1166,   697,   698,
     699,   688,  1170,   117,   118,   119,   120,   121,   122,   702,
     700,   123,   117,   118,   119,   120,   121,   122,   701,   705,
     123,   117,   118,   119,   120,   121,   122,   707,   711,   123,
     715,  1198,   117,   118,   119,   120,   121,   122,   733,   734,
     123,   735,   736,   895,   117,   118,   119,   120,   121,   122,
     737,   738,   123,   117,   118,   119,   120,   121,   122,   739,
     740,   123,   741,   742,   743,   896,   744,   745,   746,   747,
    1235,   748,   749,   750,  1239,  1240,  1241,   899,   751,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1052,   117,   118,   119,   120,   121,   122,   752,
    1557,   123,   753,  1268,  1269,  1194,  1271,  1272,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,
    1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,
    1295,  1296,   754,  1298,   755,   756,   757,   758,   759,  1304,
    1305,   760,   761,   762,   773,  1310,  1311,   117,   118,   119,
     120,   121,   122,   765,   766,   123,   763,   764,   767,   776,
     778,  1326,  1327,   775,  1328,   779,   780,  1331,   782,   786,
    1334,   117,   118,   119,   120,   121,   122,   787,   789,   123,
    1195,   791,   117,   118,   119,   120,   121,   122,   795,  1197,
     123,   117,   118,   119,   120,   121,   122,   799,  1673,   123,
     806,   846,  1366,  1367,   847,  1369,   848,   851,   849,  1372,
     850,   852,   853,   854,   855,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1676,
     856,   857,   859,   861,   862,   871,  1398,   865,  1400,  1401,
    1402,   872,   889,   940,  1681,   117,   118,   119,   120,   121,
     122,   874,   878,   123,   117,   118,   119,   120,   121,   122,
     882,   931,   123,   933,   932,   944,   947,   948,  1429,   949,
     953,   966,   955,  1433,   959,   963,  1436,  1437,  1438,   969,
     117,   118,   119,   120,   121,   122,   976,  1683,   123,   977,
     967,   117,   118,   119,   120,   121,   122,  1457,  1458,   123,
    1460,   968,   971,   980,   981,   982,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,
    1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,
    1489,  1490,   983,   984,   985,  1192,  1690,  1497,   986,   987,
     988,   989,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1510,  1511,  1512,   618,  1514,  1515,  1516,
     990,   991,  1519,   992,   993,  1522,  1523,  1524,   994,  1031,
     117,   118,   119,   120,   121,   122,   995,  1963,   123,   117,
     118,   119,   120,   121,   122,   996,  2000,   123,   997,   998,
     999,  1000,  1001,  1002,  1551,  2046,  1553,  1554,  1003,  1004,
    1005,  1558,  1006,  1007,  1008,  1021,  2104,  1023,  1033,   117,
     118,   119,   120,   121,   122,  1028,  1572,   123,  2138,  1029,
    1037,  1041,  1045,  1046,  1579,  1047,  1049,  2364,   117,   118,
     119,   120,   121,   122,  1054,  1055,   123,  1058,  1087,  1088,
    1090,  1091,  1126,   117,   118,   119,   120,   121,   122,  1092,
     945,   123,   117,   118,   119,   120,   121,   122,  1093,  1614,
     123,  1094,  1616,  1617,  1618,  1619,  1620,  1621,  1622,  1623,
    1624,  1625,  1626,  1627,  1628,  1099,  1122,  1128,  1160,  1633,
    1634,  1635,  1164,  1167,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1650,  1651,  1652,
    1653,  1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,
    1663,  1664,  1665,  1171,  1667,  1172,  1173,  1196,  1671,  1203,
    1189,  2365,   117,   118,   119,   120,   121,   122,  1204,  1205,
     123,  1206,  1207,  1686,  1687,  1688,  1208,  1209,  1210,  1689,
    1211,  1212,  1213,  1214,  1215,  2366,   117,   118,   119,   120,
     121,   122,  1216,  1217,   123,  1218,  2383,  1257,   117,   118,
     119,   120,   121,   122,  1219,  2472,   123,  1220,  1221,  1718,
    1222,  1719,  1720,  1223,  1224,  1722,  1225,  1724,  1725,  1226,
    1727,  1728,  1729,  1730,  1227,  1732,  1228,  1734,  1735,  1236,
    1237,  1242,  1243,  1258,  1741,   117,   118,   119,   120,   121,
     122,  1259,  1260,   123,   117,   118,   119,   120,   121,   122,
    1261,  1262,   123,   117,   118,   119,   120,   121,   122,  2476,
    1767,   123,  1267,  1263,  1771,  1264,  1270,  1265,  2478,   117,
     118,   119,   120,   121,   122,  1297,  1299,   123,  1169,  1300,
    1301,  1302,  1306,  1359,  1329,  1792,  1793,  1794,  1795,  1796,
    1797,  1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,
    1807,  1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,
    1330,  1818,  1325,  1332,  1333,  1822,  1337,  1824,  1825,  1364,
    1827,  1828,  1829,  1830,  1368,  1832,  1370,  1834,  1373,  1836,
    1837,  1838,  1374,  1375,  1841,   117,   118,   119,   120,   121,
     122,  1394,  1395,   123,   117,   118,   119,   120,   121,   122,
    1397,  1403,   123,   117,   118,   119,   120,   121,   122,  1404,
    1865,   123,  1867,  1868,  1405,  1406,  1871,  1407,  1408,  1874,
    1409,  1410,  1411,  1412,  1879,  1413,  1881,  1414,  1415,  1416,
    1417,  1418,  1887,  1888,  2479,  1552,   117,   118,   119,   120,
     121,   122,  1419,  2538,   123,   117,   118,   119,   120,   121,
     122,  1420,  1421,   123,  1422,  1423,  1424,  1425,  1426,  1915,
    1427,  1428,  1917,  1918,  1434,  1919,  1920,  1439,  1922,  1923,
    1924,  1925,   706,  1927,   694,  1928,  1929,  1454,  1455,  1456,
    1459,  1462,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,
    1942,  1943,  1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,
    1952,  1953,  1954,  1955,  1956,  1957,  1958,  2539,  1555,   117,
     118,   119,   120,   121,   122,  1493,  2540,   123,  1494,  1495,
    1498,  1513,  1521,  1975,  1976,  1977,  1978,   117,   118,   119,
     120,   121,   122,  1556,  1576,   123,   117,   118,   119,   120,
     121,   122,  1577,  1578,   123,  1583,  1723,   117,   118,   119,
     120,   121,   122,  2003,  2004,   123,  1584,  2007,  2008,  2009,
    2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  1585,  1586,
    1587,  1588,  2022,  1589,  1590,  1666,   117,   118,   119,   120,
     121,   122,  1591,  1592,   123,  1593,  2544,  1726,   117,   118,
     119,   120,   121,   122,  1594,  2045,   123,  1234,  2047,   117,
     118,   119,   120,   121,   122,  1595,  1596,   123,  1597,  1598,
    2555,  1599,  1600,  1601,  2063,  1602,  2065,  2066,  2067,  2068,
    2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,
    2079,  2080,  2081,  2082,  2083,  2084,  1603,  2087,  1604,  1605,
    1606,  1607,  2090,  2091,  1608,  2093,  2094,  2095,  2096,  1615,
    2098,  2099,  2100,  2101,  2102,  2103,  1630,  2105,  1631,  2580,
    1823,   117,   118,   119,   120,   121,   122,  1632,  2656,   123,
     117,   118,   119,   120,   121,   122,  1636,  2657,   123,  2124,
    1668,  1669,  2127,  1685,  1721,  2130,  1742,  1731,  1743,  1744,
    2135,  1733,  1745,  2658,  1740,  1773,   117,   118,   119,   120,
     121,   122,  1746,  1747,   123,   117,   118,   119,   120,   121,
     122,  1748,  1749,   123,  1750,  1751,  2160,  1752,  2161,  2162,
    1753,  2163,  2164,  1754,  2166,  2167,  2168,  2169,  1755,  2171,
    2172,  1756,  1757,  1758,  1759,  2177,  2178,  2179,  2180,  2181,
    2182,  2183,  2184,  2185,  2186,  2187,  2188,  2189,  2190,  2191,
    2192,  2193,  2194,  1760,  1761,  1762,  1763,  1764,  1765,  2671,
    1776,   117,   118,   119,   120,   121,   122,  1766,  2674,   123,
     117,   118,   119,   120,   121,   122,  1781,  2675,   123,  1786,
    1787,  1826,   117,   118,   119,   120,   121,   122,  2223,  2224,
     123,  2226,  2227,  2228,  2229,  2230,  2231,  2232,  2233,  2234,
    2235,  1783,  2237,  2238,  2239,  2240,  1788,  1789,  1790,  1819,
    2715,  1831,   117,   118,   119,   120,   121,   122,  2253,  2717,
     123,  1833,   117,   118,   119,   120,   121,   122,  1820,  2264,
     123,  1835,  1839,  1869,  2269,  2270,  2271,  2272,  2273,  2274,
    2275,  2276,  2277,  2278,  2279,  2280,  1840,  2281,  2282,  1883,
    2284,  2285,  2286,  2287,  1890,  2289,  2290,  1891,  2292,  2293,
    2294,  2295,   117,   118,   119,   120,   121,   122,  1892,  1893,
     123,  1894,  1895,  1896,  1897,  1898,  2307,  1899,  1900,  1901,
    1902,  2312,   708,  1903,  2315,   117,   118,   119,   120,   121,
     122,   436,  1904,   123,  1905,  1906,  2327,  2328,  2329,  2330,
     709,  2332,  2333,  2334,  2335,  1907,  2337,  2338,  1908,  2340,
    2341,  2342,  2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,
     710,   117,   118,   119,   120,   121,   122,  1154,  1909,   123,
    1910,  1911,  2363,  1912,  2044,   117,   118,   119,   120,   121,
     122,  1930,  1931,   123,  2371,  2372,  2373,  2374,  2375,  2376,
    2377,  2378,  2379,  2380,  2381,  2382,  1932,  1961,  1959,  1966,
    2387,   712,  2043,   117,   118,   119,   120,   121,   122,  1971,
    2005,   123,   713,  2006,  2023,  2024,  2025,  2026,  2027,  2028,
    2029,  2408,  2409,  2410,  2411,  2412,  2413,  2414,  2030,  2416,
    2417,  2418,  2419,  2031,  2421,  2422,  2032,  2033,  2034,  2035,
    2427,   117,   118,   119,   120,   121,   122,  2036,  2435,   123,
    2037,  1711,  2038,  2440,  2039,  2040,  2041,  2444,  2445,  2446,
    2119,  2042,  2060,  2061,  2451,  2452,  2453,  2454,  2048,  2456,
    2457,  2458,  2459,  2062,  2461,  2462,  2463,  2089,  2092,  2097,
    2467,  2468,  2469,  2470,   117,   118,   119,   120,   121,   122,
    2123,  2126,   123,  2143,  2144,  2145,  2146,  2484,  2485,  2486,
    2147,  2148,  2149,  2150,  2490,  2151,  2491,  2492,  2493,   714,
    2495,  2152,  2153,  2498,  2499,  2500,  2501,  2154,   716,  2155,
    2505,   117,   118,   119,   120,   121,   122,  2156,  2157,   123,
    2158,  2159,  2174,  2173,  2175,  2520,  2521,  2522,  2176,  2524,
    2525,  2526,  2527,  2528,  2200,  2205,  2531,  2208,  2225,  2241,
    2535,  2536,  2537,  2209,  2210,  2211,  2541,  2236,  2242,  2243,
    2244,  2245,  2547,  2548,  2246,  2550,  2551,  2552,   117,   118,
     119,   120,   121,   122,   717,  2247,   123,  2248,  2562,  2563,
    2249,  2250,  2566,   718,  2283,  2288,  2568,  2569,  2320,  2321,
    2322,  2323,  2573,  2574,  2575,   719,   117,   118,   119,   120,
     121,   122,  2324,  2325,   123,  2326,  2355,  2360,   770,  2590,
    2591,  2592,  2593,  2384,  2595,  2596,  2597,  2598,  2599,  2600,
    2601,  2385,  2386,  2388,  2389,   720,   117,   118,   119,   120,
     121,   122,  2390,  2403,   123,  2614,  2615,  2616,  2617,  2404,
    2405,  2620,  2621,  2622,  2623,  2624,  2625,  2406,  2415,  2420,
    2423,  2630,  2631,  2424,  2425,   117,   118,   119,   120,   121,
     122,  2640,  2641,   123,  2426,  2643,  2644,  2645,  2646,  2647,
    2648,  2649,  2650,  2443,  2448,   721,  2449,  2450,  2475,  2480,
    2506,  2494,  2507,  2512,  2662,  2663,  2664,  2665,  2666,  2496,
    2497,  2514,  2667,  2515,  2529,  2530,  2532,  2533,   723,  2534,
    2556,  2676,  2677,  2678,  2560,  2557,  2681,   117,   118,   119,
     120,   121,   122,  2688,  2558,   123,  2561,  2570,  2571,  2694,
    2572,  2578,  2697,  2579,  2581,  2700,  2701,  2584,  2703,  2585,
    2586,  2706,  2707,  2594,   724,  2710,  2602,  2711,  2712,  2603,
    2618,  2619,  2638,  2607,  2639,  2653,  2718,  2719,   725,  2668,
    2669,  2680,  2679,  2687,  2726,  2690,  2691,  2692,  2682,  2693,
    2683,  2733,  2734,  2735,  2695,  2737,  2698,  2699,  2704,  2740,
    2713,  2714,  2720,  2721,  2744,  2745,   726,  2722,  2725,  2749,
    2727,  2728,  2729,  2753,  2754,  2730,  2732,  2736,  2758,  2741,
    1545,  2742,  2761,  2762,  2746,  2750,  2751,  2765,  2755,  2759,
    2768,  2760,  2764,  2771,  2767,  2770,  2774,  2773,  2776,  2777,
       2,     3,     4,   234,     5,     6,     7,     0,     0,     8,
       0,     0,     9,     0,    10,     0,    11,     0,     0,     0,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
       0,     0,    19,     0,     0,     0,     0,     0,    20,     0,
      21,    22,    23,    24,     0,     0,    25,   727,    26,     0,
      27,    28,     0,     2,     3,     4,     0,     5,     6,     7,
       0,     0,     8,     0,     0,     9,    29,    10,     0,    11,
       0,     0,    30,    31,    32,    33,    12,    13,    14,    15,
      16,    17,    18,     0,   728,    19,     0,    34,    35,     0,
       0,    20,     0,    21,    22,    23,    24,     0,     0,    25,
       0,    26,     0,    27,    28,    36,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,     0,     0,    29,
       0,     0,     0,     0,     0,    30,    31,    32,    33,     0,
       0,   729,     0,     0,   117,   118,   119,   120,   121,   122,
      34,    35,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,   117,   118,   119,   120,   121,   122,    36,   730,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,   731,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,   732,   117,
     118,   119,   120,   121,   122,     0,     0,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,   117,   118,   119,   120,   121,   122,
     873,     0,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,    37,   123,     0,
      38,    39,     0,    40,     0,     0,     0,     0,     0,    41,
       0,   192,   117,   118,   119,   120,   121,   122,     0,     0,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
      37,     0,     0,    38,    39,     0,    40,     0,     0,     0,
       0,     0,    41,   117,   118,   119,   120,   121,   122,     0,
       0,   123,   117,   118,   119,   120,   121,   122,     0,   875,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
     117,   118,   119,   120,   121,   122,     0,   876,   123,   117,
     118,   119,   120,   121,   122,     0,   877,   123,   117,   118,
     119,   120,   121,   122,     0,   879,   123,   117,   118,   119,
     120,   121,   122,     0,   880,   123,   117,   118,   119,   120,
     121,   122,     0,   881,   123,   117,   118,   119,   120,   121,
     122,     0,   883,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,   884,     0,   117,   118,   119,   120,   121,
     122,     0,   891,   123,   117,   118,   119,   120,   121,   122,
       0,   892,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,   893,     0,   117,   118,   119,   120,   121,   122,
       0,   894,   123,   117,   118,   119,   120,   121,   122,     0,
     897,   123,   117,   118,   119,   120,   121,   122,     0,   898,
     123,   117,   118,   119,   120,   121,   122,     0,  1032,   123,
     117,   118,   119,   120,   121,   122,     0,  1034,   123,   117,
     118,   119,   120,   121,   122,     0,  1035,   123,   117,   118,
     119,   120,   121,   122,     0,  1036,   123,   117,   118,   119,
     120,   121,   122,     0,  1038,   123,   117,   118,   119,   120,
     121,   122,     0,  1039,   123,   117,   118,   119,   120,   121,
     122,     0,  1040,   123,   117,   118,   119,   120,   121,   122,
       0,  1042,   123,   117,   118,   119,   120,   121,   122,     0,
    1043,   123,   117,   118,   119,   120,   121,   122,     0,  1050,
     123,   117,   118,   119,   120,   121,   122,     0,  1051,   123,
     117,   118,   119,   120,   121,   122,     0,  1053,   123,   117,
     118,   119,   120,   121,   122,     0,  1056,   123,   117,   118,
     119,   120,   121,   122,     0,  1057,   123,   117,   118,   119,
     120,   121,   122,     0,  1089,   123,   117,   118,   119,   120,
     121,   122,     0,  1176,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,  1177,     0,   117,   118,   119,
     120,   121,   122,     0,  1178,   123,   117,   118,   119,   120,
     121,   122,     0,  1179,   123,   117,   118,   119,   120,   121,
     122,     0,  1180,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,     0,  1181,     0,   117,   118,
     119,   120,   121,   122,     0,  1182,   123,   117,   118,   119,
     120,   121,   122,     0,  1183,   123,   117,   118,   119,   120,
     121,   122,     0,  1184,   123,     0,   117,   118,   119,   120,
     121,   122,     0,  1185,   123,   117,   118,   119,   120,   121,
     122,     0,  1186,   123,   117,   118,   119,   120,   121,   122,
       0,  1187,   123,   117,   118,   119,   120,   121,   122,     0,
    1188,   123,   117,   118,   119,   120,   121,   122,     0,  1200,
     123,   117,   118,   119,   120,   121,   122,     0,  1201,   123,
     117,   118,   119,   120,   121,   122,     0,  1230,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  1312,     0,
     117,   118,   119,   120,   121,   122,     0,  1313,   123,   117,
     118,   119,   120,   121,   122,     0,  1314,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  1315,     0,   117,
     118,   119,   120,   121,   122,     0,  1316,   123,   117,   118,
     119,   120,   121,   122,     0,  1317,   123,   117,   118,   119,
     120,   121,   122,     0,  1318,   123,   117,   118,   119,   120,
     121,   122,     0,  1319,   123,   117,   118,   119,   120,   121,
     122,     0,  1320,   123,   117,   118,   119,   120,   121,   122,
       0,  1321,   123,   117,   118,   119,   120,   121,   122,     0,
    1322,   123,   117,   118,   119,   120,   121,   122,     0,  1323,
     123,   117,   118,   119,   120,   121,   122,     0,  1324,   123,
     117,   118,   119,   120,   121,   122,     0,  1335,   123,   117,
     118,   119,   120,   121,   122,     0,  1336,   123,   117,   118,
     119,   120,   121,   122,     0,  1365,   123,   117,   118,   119,
     120,   121,   122,     0,  1440,   123,   117,   118,   119,   120,
     121,   122,     0,  1441,   123,   117,   118,   119,   120,   121,
     122,     0,  1442,   123,   117,   118,   119,   120,   121,   122,
       0,  1443,   123,   117,   118,   119,   120,   121,   122,     0,
    1444,   123,   117,   118,   119,   120,   121,   122,     0,  1445,
     123,   117,   118,   119,   120,   121,   122,     0,  1446,   123,
     117,   118,   119,   120,   121,   122,     0,  1447,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
    1448,     0,   117,   118,   119,   120,   121,   122,     0,  1449,
     123,   117,   118,   119,   120,   121,   122,     0,  1450,   123,
     117,   118,   119,   120,   121,   122,     0,  1451,   123,   117,
     118,   119,   120,   121,   122,     0,  1452,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,     0,
       0,  1463,     0,   117,   118,   119,   120,   121,   122,     0,
    1464,   123,   117,   118,   119,   120,   121,   122,     0,  1559,
     123,     0,   117,   118,   119,   120,   121,   122,     0,  1560,
     123,   117,   118,   119,   120,   121,   122,     0,  1561,   123,
     117,   118,   119,   120,   121,   122,     0,  1562,   123,   117,
     118,   119,   120,   121,   122,     0,  1563,   123,   117,   118,
     119,   120,   121,   122,     0,  1564,   123,   117,   118,   119,
     120,   121,   122,     0,  1565,   123,   117,   118,   119,   120,
     121,   122,     0,  1566,   123,   117,   118,   119,   120,   121,
     122,     0,  1567,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  1568,     0,   117,   118,   119,   120,   121,
     122,     0,  1569,   123,   117,   118,   119,   120,   121,   122,
       0,  1570,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1571,     0,   117,   118,   119,   120,   121,   122,
       0,  1581,   123,   117,   118,   119,   120,   121,   122,     0,
    1582,   123,   117,   118,   119,   120,   121,   122,     0,  1609,
     123,   117,   118,   119,   120,   121,   122,     0,  1672,   123,
     117,   118,   119,   120,   121,   122,     0,  1674,   123,   117,
     118,   119,   120,   121,   122,     0,  1675,   123,   117,   118,
     119,   120,   121,   122,     0,  1677,   123,   117,   118,   119,
     120,   121,   122,     0,  1678,   123,   117,   118,   119,   120,
     121,   122,     0,  1679,   123,   117,   118,   119,   120,   121,
     122,     0,  1680,   123,   117,   118,   119,   120,   121,   122,
       0,  1682,   123,   117,   118,   119,   120,   121,   122,     0,
    1684,   123,   117,   118,   119,   120,   121,   122,     0,  1691,
     123,   117,   118,   119,   120,   121,   122,     0,  1717,   123,
     117,   118,   119,   120,   121,   122,     0,  1768,   123,   117,
     118,   119,   120,   121,   122,     0,  1772,   123,   117,   118,
     119,   120,   121,   122,     0,  1774,   123,   117,   118,   119,
     120,   121,   122,     0,  1775,   123,   117,   118,   119,   120,
     121,   122,     0,  1777,   123,   117,   118,   119,   120,   121,
     122,     0,  1778,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,  1779,     0,   117,   118,   119,
     120,   121,   122,     0,  1780,   123,   117,   118,   119,   120,
     121,   122,     0,  1782,   123,   117,   118,   119,   120,   121,
     122,     0,  1784,   123,   117,   118,   119,   120,   121,   122,
       0,  1791,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,     0,  1817,     0,   117,   118,
     119,   120,   121,   122,     0,  1866,   123,     0,   117,   118,
     119,   120,   121,   122,     0,  1870,   123,   117,   118,   119,
     120,   121,   122,     0,  1872,   123,   117,   118,   119,   120,
     121,   122,     0,  1873,   123,   117,   118,   119,   120,   121,
     122,     0,  1875,   123,   117,   118,   119,   120,   121,   122,
       0,  1876,   123,   117,   118,   119,   120,   121,   122,     0,
    1877,   123,   117,   118,   119,   120,   121,   122,     0,  1878,
     123,   117,   118,   119,   120,   121,   122,     0,  1880,   123,
     117,   118,   119,   120,   121,   122,     0,  1882,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  1889,     0,
     117,   118,   119,   120,   121,   122,     0,  1933,   123,   117,
     118,   119,   120,   121,   122,     0,  1960,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  1964,     0,   117,
     118,   119,   120,   121,   122,     0,  1965,   123,   117,   118,
     119,   120,   121,   122,     0,  1967,   123,   117,   118,   119,
     120,   121,   122,     0,  1968,   123,   117,   118,   119,   120,
     121,   122,     0,  1969,   123,   117,   118,   119,   120,   121,
     122,     0,  1970,   123,   117,   118,   119,   120,   121,   122,
       0,  1972,   123,   117,   118,   119,   120,   121,   122,     0,
    1973,   123,   117,   118,   119,   120,   121,   122,     0,  1974,
     123,   117,   118,   119,   120,   121,   122,     0,  1979,   123,
     117,   118,   119,   120,   121,   122,     0,  2021,   123,   117,
     118,   119,   120,   121,   122,     0,  2049,   123,   117,   118,
     119,   120,   121,   122,     0,  2050,   123,   117,   118,   119,
     120,   121,   122,     0,  2052,   123,   117,   118,   119,   120,
     121,   122,     0,  2053,   123,   117,   118,   119,   120,   121,
     122,     0,  2054,   123,   117,   118,   119,   120,   121,   122,
       0,  2055,   123,   117,   118,   119,   120,   121,   122,     0,
    2057,   123,   117,   118,   119,   120,   121,   122,     0,  2058,
     123,   117,   118,   119,   120,   121,   122,     0,  2059,   123,
     117,   118,   119,   120,   121,   122,     0,  2064,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
    2125,     0,   117,   118,   119,   120,   121,   122,     0,  2128,
     123,   117,   118,   119,   120,   121,   122,     0,  2129,   123,
     117,   118,   119,   120,   121,   122,     0,  2131,   123,   117,
     118,   119,   120,   121,   122,     0,  2132,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,     0,
       0,  2133,   117,   118,   119,   120,   121,   122,     0,     0,
     123,  2134,     0,   117,   118,   119,   120,   121,   122,     0,
    2136,   123,   117,   118,   119,   120,   121,   122,     0,  2137,
     123,   117,   118,   119,   120,   121,   122,     0,  2142,   123,
     117,   118,   119,   120,   121,   122,     0,  2195,   123,   117,
     118,   119,   120,   121,   122,     0,  2196,   123,   117,   118,
     119,   120,   121,   122,     0,  2198,   123,   117,   118,   119,
     120,   121,   122,     0,  2199,   123,   117,   118,   119,   120,
     121,   122,     0,  2201,   123,   117,   118,   119,   120,   121,
     122,     0,  2202,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  2203,     0,   117,   118,   119,   120,   121,
     122,     0,  2204,   123,   117,   118,   119,   120,   121,   122,
       0,  2206,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  2207,     0,   117,   118,   119,   120,   121,   122,
       0,  2212,   123,   117,   118,   119,   120,   121,   122,     0,
    2251,   123,   117,   118,   119,   120,   121,   122,     0,  2252,
     123,   117,   118,   119,   120,   121,   122,     0,  2254,   123,
     117,   118,   119,   120,   121,   122,     0,  2255,   123,   117,
     118,   119,   120,   121,   122,     0,  2257,   123,   117,   118,
     119,   120,   121,   122,     0,  2258,   123,   117,   118,   119,
     120,   121,   122,     0,  2259,   123,   117,   118,   119,   120,
     121,   122,     0,  2260,   123,   117,   118,   119,   120,   121,
     122,     0,  2262,   123,   117,   118,   119,   120,   121,   122,
       0,  2263,   123,   117,   118,   119,   120,   121,   122,     0,
    2265,   123,   117,   118,   119,   120,   121,   122,     0,  2266,
     123,   117,   118,   119,   120,   121,   122,     0,  2267,   123,
     117,   118,   119,   120,   121,   122,     0,  2268,   123,   117,
     118,   119,   120,   121,   122,     0,  2291,   123,   117,   118,
     119,   120,   121,   122,     0,  2303,   123,   117,   118,   119,
     120,   121,   122,     0,  2304,   123,   117,   118,   119,   120,
     121,   122,     0,  2305,   123,   117,   118,   119,   120,   121,
     122,     0,  2306,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,  2308,     0,   117,   118,   119,
     120,   121,   122,     0,  2309,   123,   117,   118,   119,   120,
     121,   122,     0,  2310,   123,   117,   118,   119,   120,   121,
     122,     0,  2311,   123,   117,   118,   119,   120,   121,   122,
       0,  2313,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,  2314,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,  2316,     0,   117,   118,
     119,   120,   121,   122,     0,  2317,   123,   117,   118,   119,
     120,   121,   122,     0,  2318,   123,   117,   118,   119,   120,
     121,   122,     0,  2319,   123,   117,   118,   119,   120,   121,
     122,     0,  2339,   123,   117,   118,   119,   120,   121,   122,
       0,  2351,   123,   117,   118,   119,   120,   121,   122,     0,
    2352,   123,   117,   118,   119,   120,   121,   122,     0,  2353,
     123,   117,   118,   119,   120,   121,   122,     0,  2354,   123,
     117,   118,   119,   120,   121,   122,     0,  2356,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  2357,     0,
     117,   118,   119,   120,   121,   122,     0,  2358,   123,   117,
     118,   119,   120,   121,   122,     0,  2359,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  2361,     0,   117,
     118,   119,   120,   121,   122,     0,  2362,   123,   117,   118,
     119,   120,   121,   122,     0,  2367,   123,   117,   118,   119,
     120,   121,   122,     0,  2391,   123,   117,   118,   119,   120,
     121,   122,     0,  2392,   123,   117,   118,   119,   120,   121,
     122,     0,  2393,   123,   117,   118,   119,   120,   121,   122,
       0,  2394,   123,   117,   118,   119,   120,   121,   122,     0,
    2396,   123,   117,   118,   119,   120,   121,   122,     0,  2397,
     123,   117,   118,   119,   120,   121,   122,     0,  2398,   123,
     117,   118,   119,   120,   121,   122,     0,  2399,   123,   117,
     118,   119,   120,   121,   122,     0,  2401,   123,   117,   118,
     119,   120,   121,   122,     0,  2402,   123,   117,   118,   119,
     120,   121,   122,     0,  2407,   123,   117,   118,   119,   120,
     121,   122,     0,  2431,   123,   117,   118,   119,   120,   121,
     122,     0,  2432,   123,   117,   118,   119,   120,   121,   122,
       0,  2433,   123,   117,   118,   119,   120,   121,   122,     0,
    2434,   123,   117,   118,   119,   120,   121,   122,     0,  2436,
     123,   117,   118,   119,   120,   121,   122,     0,  2437,   123,
     117,   118,   119,   120,   121,   122,     0,  2438,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
    2439,     0,   117,   118,   119,   120,   121,   122,     0,  2441,
     123,   117,   118,   119,   120,   121,   122,     0,  2442,   123,
     117,   118,   119,   120,   121,   122,     0,  2447,   123,   117,
     118,   119,   120,   121,   122,     0,  2464,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,  2465,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,  2466,     0,   117,   118,   119,   120,   121,   122,     0,
    2471,   123,   117,   118,   119,   120,   121,   122,     0,  2473,
     123,   117,   118,   119,   120,   121,   122,     0,  2474,   123,
     117,   118,   119,   120,   121,   122,     0,  2477,   123,   117,
     118,   119,   120,   121,   122,     0,  2481,   123,   117,   118,
     119,   120,   121,   122,     0,  2482,   123,   117,   118,   119,
     120,   121,   122,     0,  2483,   123,   117,   118,   119,   120,
     121,   122,     0,  2487,   123,   117,   118,   119,   120,   121,
     122,     0,  2502,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  2503,     0,   117,   118,   119,   120,   121,
     122,     0,  2504,   123,   117,   118,   119,   120,   121,   122,
       0,  2508,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  2509,     0,   117,   118,   119,   120,   121,   122,
       0,  2510,   123,   117,   118,   119,   120,   121,   122,     0,
    2513,   123,   117,   118,   119,   120,   121,   122,     0,  2517,
     123,   117,   118,   119,   120,   121,   122,     0,  2518,   123,
     117,   118,   119,   120,   121,   122,     0,  2519,   123,   117,
     118,   119,   120,   121,   122,     0,  2523,   123,   117,   118,
     119,   120,   121,   122,     0,  2545,   123,   117,   118,   119,
     120,   121,   122,     0,  2546,   123,   117,   118,   119,   120,
     121,   122,     0,  2549,   123,   117,   118,   119,   120,   121,
     122,     0,  2553,   123,   117,   118,   119,   120,   121,   122,
       0,  2554,   123,   117,   118,   119,   120,   121,   122,     0,
    2559,   123,   117,   118,   119,   120,   121,   122,     0,  2576,
     123,   117,   118,   119,   120,   121,   122,     0,  2577,   123,
     117,   118,   119,   120,   121,   122,     0,  2582,   123,   117,
     118,   119,   120,   121,   122,     0,  2583,   123,   117,   118,
     119,   120,   121,   122,     0,  2587,   123,     0,     0,     0,
       0,     0,     0,     0,  2604,    73,    74,    75,    76,    77,
       0,    78,    79,  2605,    80,    81,    82,     0,     0,     0,
       0,     0,  2608,     0,     0,    83,   117,   118,   119,   120,
     121,   122,     0,     0,   123,  2609,     0,   117,   118,   119,
     120,   121,   122,     0,  2610,   123,   117,   118,   119,   120,
     121,   122,     0,  2611,   123,   117,   118,   119,   120,   121,
     122,     0,  2612,   123,     0,    84,     0,     0,     0,     0,
       0,  2613,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,  2628,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,     0,  2629,     0,   117,   118,
     119,   120,   121,   122,     0,  2632,   123,   117,   118,   119,
     120,   121,   122,     0,  2633,   123,   117,   118,   119,   120,
     121,   122,     0,  2634,   123,   117,   118,   119,   120,   121,
     122,     0,  2635,   123,    85,   117,   118,   119,   120,   121,
     122,  2636,     0,   123,   117,   118,   119,   120,   121,   122,
    2637,     0,   123,   117,   118,   119,   120,   121,   122,  2651,
       0,   123,   117,   118,   119,   120,   121,   122,  2652,     0,
     123,   117,   118,   119,   120,   121,   122,  2654,     0,   123,
       0,   117,   118,   119,   120,   121,   122,     0,  2655,   123,
     117,   118,   119,   120,   121,   122,     0,  2659,   123,   117,
     118,   119,   120,   121,   122,     0,  2670,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  2673,     0,   117,
     118,   119,   120,   121,   122,     0,  2686,   123,   117,   118,
     119,   120,   121,   122,     0,  2689,   123,     0,  1996,     0,
       0,     0,     0,     0,  2702,     0,  1017,     0,     0,     0,
       0,     0,     0,  2705,     0,  1018,     0,   117,   118,   119,
     120,   121,   122,     0,  1358,   123,   117,   118,   119,   120,
     121,   122,     0,  1860,   123,   117,   118,   119,   120,   121,
     122,  1016,     0,   123,   117,   118,   119,   120,   121,   122,
    1233,     0,   123,   117,   118,   119,   120,   121,   122,  1432,
       0,   123,     0,     0,     0,     0,     0,   771,     0,   117,
     118,   119,   120,   121,   122,     0,   772,   123,   117,   118,
     119,   120,   121,   122,     0,  1153,   123,   117,   118,   119,
     120,   121,   122,     0,  1712,   123,     0,     0,     0,    86,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,     0,     0,  1142,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,  1143,     0,   117,   118,   119,   120,
     121,   122,     0,  1357,   123,   117,   118,   119,   120,   121,
     122,  1347,     0,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,     0,     0,     0,  1348,   117,
     118,   119,   120,   121,   122,     0,     0,   123,   527,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
     438,   117,   118,   119,   120,   121,   122,     0,   695,   123,
     117,   118,   119,   120,   121,   122,   777,     0,   123,     0,
       0,     0,     0,     0,   522,     0,   117,   118,   119,   120,
     121,   122,     0,   248,   123,   117,   118,   119,   120,   121,
     122,  2302,     0,   123,     0,     0,     0,     0,  2222,   117,
     118,   119,   120,   121,   122,     0,  2122,   123,   117,   118,
     119,   120,   121,   122,  2120,     0,   123,   117,   118,   119,
     120,   121,   122,     0,  2121,   123,   117,   118,   119,   120,
     121,   122,  1713,     0,   123,   117,   118,   119,   120,   121,
     122,  1715,     0,   123,   117,   118,   119,   120,   121,   122,
    1716,     0,   123,   117,   118,   119,   120,   121,   122,     0,
    1547,   123,   117,   118,   119,   120,   121,   122,     0,  1549,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  1550,   117,
     118,   119,   120,   121,   122,     0,  1360,   123,   117,   118,
     119,   120,   121,   122,     0,  1362,   123,   117,   118,   119,
     120,   121,   122,     0,  1363,   123,   117,   118,   119,   120,
     121,   122,  1155,     0,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,     0,     0,     0,     0,  1157,     0,
     117,   118,   119,   120,   121,   122,     0,  1158,   123,   117,
     118,   119,   120,   121,   122,   923,     0,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,   925,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,   926,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,  1710,   117,   118,   119,   120,   121,   122,
       0,  1544,   123,   117,   118,   119,   120,   121,   122,  1152,
       0,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,   920,     0,   117,   118,   119,   120,   121,
     122,     0,   667,   123,     0,   117,   118,   119,   120,   121,
     122,     0,   921,   123,   117,   118,   119,   120,   121,   122,
       0,   922,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,  1701,     0,   117,   118,   119,   120,
     121,   122,     0,  1702,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,     0,     0,   910,     0,   117,   118,
     119,   120,   121,   122,     0,   911,   123,   117,   118,   119,
     120,   121,   122,   657,     0,   123,   117,   118,   119,   120,
     121,   122,   658,     0,   123,     0,     0,     0,     0,     0,
    2661,   117,   118,   119,   120,   121,   122,     0,  2627,   123,
     117,   118,   119,   120,   121,   122,  2589,     0,   123,     0,
       0,     0,     0,     0,  2543,   117,   118,   119,   120,   121,
     122,     0,  2489,   123,   117,   118,   119,   120,   121,   122,
    2430,     0,   123,     0,     0,     0,     0,     0,  2370,     0,
     117,   118,   119,   120,   121,   122,     0,  2300,   123,   117,
     118,   119,   120,   121,   122,  2219,     0,   123,     0,     0,
       0,     0,     0,  2114,   117,   118,   119,   120,   121,   122,
       0,  1990,   123,   117,   118,   119,   120,   121,   122,  1853,
       0,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,  1703,   117,   118,   119,   120,   121,   122,
       0,  1536,   123,   117,   118,   119,   120,   121,   122,   912,
       0,   123,   117,   118,   119,   120,   121,   122,     0,   656,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,     0,   659,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  1349,   117,   118,   119,   120,   121,   122,
     909,     0,   123,     0,     0,     0,     0,     0,  2756,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
    2747,     0,   117,   118,   119,   120,   121,   122,     0,  2738,
     123,   117,   118,   119,   120,   121,   122,  2723,     0,   123,
       0,     0,     0,     0,     0,  2708,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,  2684,   117,   118,
     119,   120,   121,   122,     0,  2660,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,  2626,   117,   118,
     119,   120,   121,   122,     0,  2588,   123,   117,   118,   119,
     120,   121,   122,  2542,     0,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,  2488,   117,   118,
     119,   120,   121,   122,     0,  2428,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,     0,  2368,
     117,   118,   119,   120,   121,   122,     0,  2297,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,  2216,   117,   118,   119,   120,   121,   122,     0,
    2111,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,     0,  1987,   117,   118,   119,   120,   121,
     122,     0,  1850,   123,   117,   118,   119,   120,   121,   122,
    1700,     0,   123,     0,   117,   118,   119,   120,   121,   122,
       0,  1141,   123,   117,   118,   119,   120,   121,   122,  1533,
       0,   123,     0,     0,     0,     0,     0,  1346,     0,     0,
       0,     0,  1851,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,  1852,     0,   117,   118,   119,
     120,   121,   122,     0,   919,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,     0,   666,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
    1543,   117,   118,   119,   120,   121,   122,     0,  1356,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,     0,  1151,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,  1340,     0,   117,   118,   119,   120,   121,
     122,     0,  1341,   123,     0,   117,   118,   119,   120,   121,
     122,     0,  1339,   123,   117,   118,   119,   120,   121,   122,
       0,  1344,   123,   117,   118,   119,   120,   121,   122,     0,
    1350,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,  1351,   117,   118,   119,   120,   121,   122,     0,
    1338,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,  1343,     0,   117,   118,   119,   120,   121,
     122,     0,  1354,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,     0,  1355,     0,   117,   118,
     119,   120,   121,   122,     0,  1361,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,  1526,     0,
     117,   118,   119,   120,   121,   122,     0,  1527,   123,     0,
     117,   118,   119,   120,   121,   122,     0,  1528,   123,   117,
     118,   119,   120,   121,   122,     0,  1531,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,  1537,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,     0,     0,
    1538,     0,   117,   118,   119,   120,   121,   122,     0,  1696,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,  1699,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,  1706,     0,   117,   118,   119,   120,
     121,   122,     0,  1707,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,     0,     0,  1988,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,  1989,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  1997,     0,
     117,   118,   119,   120,   121,   122,     0,  1998,   123,   117,
     118,   119,   120,   121,   122,     0,  1999,   123,   117,   118,
     119,   120,   121,   122,     0,  2685,   123,     0,   117,   118,
     119,   120,   121,   122,     0,  1529,   123,   117,   118,   119,
     120,   121,   122,     0,  1532,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,  1539,     0,   117,
     118,   119,   120,   121,   122,     0,  1540,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,  1342,
       0,   117,   118,   119,   120,   121,   122,     0,  1345,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,  1352,   117,   118,   119,   120,   121,   122,     0,     0,
     123,  1353,   117,   118,   119,   120,   121,   122,     0,  1145,
     123,   117,   118,   119,   120,   121,   122,     0,  1146,   123,
     117,   118,   119,   120,   121,   122,     0,  1147,   123,     0,
     117,   118,   119,   120,   121,   122,     0,  1148,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,  1149,     0,   117,   118,   119,   120,   121,   122,
       0,  1150,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1133,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,  1134,     0,   117,   118,   119,
     120,   121,   122,     0,  1135,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,  1136,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,  1137,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,  1138,
       0,   117,   118,   119,   120,   121,   122,     0,  1139,   123,
     117,   118,   119,   120,   121,   122,     0,  1140,   123,     0,
     117,   118,   119,   120,   121,   122,     0,  1156,   123,   117,
     118,   119,   120,   121,   122,   671,     0,   123,   117,   118,
     119,   120,   121,   122,   673,     0,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,   674,     0,   117,
     118,   119,   120,   121,   122,     0,   691,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,   900,
       0,   117,   118,   119,   120,   121,   122,     0,   901,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
     902,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     903,     0,   117,   118,   119,   120,   121,   122,     0,   904,
     123,   117,   118,   119,   120,   121,   122,     0,   905,   123,
       0,   117,   118,   119,   120,   121,   122,     0,   906,   123,
     117,   118,   119,   120,   121,   122,     0,   907,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,   908,     0,   117,   118,   119,   120,   121,   122,
       0,   913,   123,     0,   117,   118,   119,   120,   121,   122,
       0,   914,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,   915,     0,   117,   118,   119,
     120,   121,   122,     0,   916,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,   917,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,   918,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,     0,   924,
       0,   117,   118,   119,   120,   121,   122,     0,   927,   123,
       0,   117,   118,   119,   120,   121,   122,     0,  1144,   123,
     117,   118,   119,   120,   121,   122,     0,  1534,   123,   117,
     118,   119,   120,   121,   122,     0,  1535,   123,   117,   118,
     119,   120,   121,   122,   668,     0,   123,     0,   117,   118,
     119,   120,   121,   122,     0,   647,   123,   117,   118,   119,
     120,   121,   122,     0,   648,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,   649,     0,   117,
     118,   119,   120,   121,   122,     0,   650,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,   651,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,   652,     0,
     117,   118,   119,   120,   121,   122,     0,   653,   123,     0,
     117,   118,   119,   120,   121,   122,     0,   654,   123,   117,
     118,   119,   120,   121,   122,     0,   655,   123,   117,   118,
     119,   120,   121,   122,     0,   660,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,     0,     0,
     661,   117,   118,   119,   120,   121,   122,     0,     0,   123,
     662,     0,   117,   118,   119,   120,   121,   122,     0,   663,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,     0,   664,     0,   117,   118,   119,   120,   121,
     122,     0,   665,   123,     0,   117,   118,   119,   120,   121,
     122,     0,   669,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,   670,   117,   118,   119,   120,   121,
     122,     0,     0,   123,     0,     0,     0,   672,   117,   118,
     119,   120,   121,   122,     0,     0,   123,   675,     0,   117,
     118,   119,   120,   121,   122,     0,  1190,   123,   117,   118,
     119,   120,   121,   122,     0,  1191,   123,   117,   118,   119,
     120,   121,   122,     0,  1193,   123,   117,   118,   119,   120,
     121,   122,     0,  1168,   123,   117,   118,   119,   120,   121,
     122,     0,  1453,   123,   117,   118,   119,   120,   121,   122,
       0,  1461,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,  1861,     0,   117,   118,   119,   120,
     121,   122,     0,  1863,   123,     0,   117,   118,   119,   120,
     121,   122,     0,  1864,   123,     0,   117,   118,   119,   120,
     121,   122,     0,   942,   123,  2018,   117,   118,   119,   120,
     121,   122,  1396,     0,   123,  2019,     0,   117,   118,   119,
     120,   121,   122,     0,  2020,   123,  1517,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,  1518,     0,   117,
     118,   119,   120,   121,   122,     0,  1520,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  1739,     0,     0,
       0,     0,     0,  1785,     0,   117,   118,   119,   120,   121,
     122,     0,  2139,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  2140,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,     0,  2141,   117,   118,   119,
     120,   121,   122,     0,     0,   123,  1580,   117,   118,   119,
     120,   121,   122,     0,  1399,   123,   117,   118,   119,   120,
     121,   122,  1199,     0,   123,     0,     0,  1629,   117,   118,
     119,   120,   121,   122,     0,  1736,   123,   117,   118,   119,
     120,   121,   122,     0,  1737,   123,   117,   118,   119,   120,
     121,   122,     0,  1738,   123,   117,   118,   119,   120,   121,
     122,     0,     0,   123,     0,  1884,   117,   118,   119,   120,
     121,   122,     0,     0,   123,  1885,   117,   118,   119,   120,
     121,   122,     0,     0,   123,  1886,     0,   117,   118,   119,
     120,   121,   122,     0,  1573,   123,   117,   118,   119,   120,
     121,   122,     0,     0,   123,  1574,   117,   118,   119,   120,
     121,   122,     0,     0,   123,  1575,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,  1391,     0,   117,   118,
     119,   120,   121,   122,     0,  1392,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,   117,   118,   119,   120,
     121,   122,  1393,     0,   123,     0,     0,     0,     0,     0,
    1266,   117,   118,   119,   120,   121,   122,     0,  2564,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,  2567,   117,   118,   119,   120,   121,   122,     0,     0,
     123,  2752,   117,   118,   119,   120,   121,   122,     0,  2511,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,  2516,     0,   117,   118,   119,   120,   121,   122,     0,
    2743,   123,   117,   118,   119,   120,   121,   122,  2455,     0,
     123,   117,   118,   119,   120,   121,   122,  2460,     0,   123,
       0,   117,   118,   119,   120,   121,   122,     0,  2731,   123,
     117,   118,   119,   120,   121,   122,     0,  2395,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  2400,     0,
     117,   118,   119,   120,   121,   122,     0,  2716,   123,   117,
     118,   119,   120,   121,   122,     0,  2331,   123,     0,   117,
     118,   119,   120,   121,   122,     0,  2336,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,  2696,   117,
     118,   119,   120,   121,   122,     0,  2256,   123,   117,   118,
     119,   120,   121,   122,     0,  2261,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,     0,     0,
    2672,   117,   118,   119,   120,   121,   122,     0,  2165,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
    2170,   117,   118,   119,   120,   121,   122,     0,     0,   123,
    2642,   117,   118,   119,   120,   121,   122,     0,  2051,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
    2056,     0,   117,   118,   119,   120,   121,   122,     0,  2606,
     123,     0,     0,     0,     0,   537,     0,  1921,     0,     0,
     117,   118,   119,   120,   121,   122,   539,  1926,   123,   117,
     118,   119,   120,   121,   122,   543,  2565,   123,   117,   118,
     119,   120,   121,   122,     0,   547,   123,     0,  1847,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,  1859,
       0,   117,   118,   119,   120,   121,   122,     0,  1862,   123,
     117,   118,   119,   120,   121,   122,     0,  1983,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,  1985,     0,   117,   118,   119,   120,   121,   122,     0,
    1992,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,  2106,   117,   118,   119,   120,   121,   122,     0,     0,
     123,  2110,     0,   117,   118,   119,   120,   121,   122,     0,
    2117,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,  2118,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,   117,   118,   119,   120,   121,   122,     0,
    2213,   123,   117,   118,   119,   120,   121,   122,     0,  2214,
     123,   117,   118,   119,   120,   121,   122,     0,  2220,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,  2296,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,  2299,     0,   117,   118,   119,   120,   121,   122,     0,
    2301,   123,   117,   118,   119,   120,   121,   122,     0,  2429,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,     0,  2757,     0,   117,   118,   119,   120,   121,
     122,     0,  2775,   123,   117,   118,   119,   120,   121,   122,
       0,  1697,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1709,     0,   117,   118,   119,   120,   121,   122,
       0,  1714,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1845,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1848,     0,   117,   118,   119,   120,   121,   122,
       0,  1855,   123,   117,   118,   119,   120,   121,   122,     0,
    1980,   123,   117,   118,   119,   120,   121,   122,     0,  1986,
     123,     0,   117,   118,   119,   120,   121,   122,     0,  1994,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,  1995,     0,   117,   118,   119,   120,   121,   122,
       0,  2107,   123,   117,   118,   119,   120,   121,   122,     0,
    2108,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,     0,  2115,     0,   117,   118,   119,   120,
     121,   122,     0,  2215,   123,   117,   118,   119,   120,   121,
     122,     0,  2218,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  2221,     0,   117,   118,   119,   120,   121,
     122,     0,  2369,   123,     0,   117,   118,   119,   120,   121,
     122,     0,  2748,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  2772,   117,   118,   119,   120,   121,   122,
       0,  1530,   123,   117,   118,   119,   120,   121,   122,     0,
    1542,   123,     0,   117,   118,   119,   120,   121,   122,     0,
    1548,   123,   117,   118,   119,   120,   121,   122,     0,  1695,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,  1698,     0,   117,   118,   119,   120,   121,   122,
       0,  1705,   123,   117,   118,   119,   120,   121,   122,     0,
    1842,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,     0,  1849,     0,   117,   118,   119,   120,
     121,   122,     0,  1857,   123,   117,   118,   119,   120,   121,
     122,     0,  1858,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  1981,   117,   118,   119,   120,   121,   122,
       0,     0,   123,  1982,     0,   117,   118,   119,   120,   121,
     122,     0,  1991,   123,     0,   117,   118,   119,   120,   121,
     122,     0,  2109,   123,   117,   118,   119,   120,   121,   122,
       0,  2113,   123,     0,   117,   118,   119,   120,   121,   122,
       0,  2116,   123,   117,   118,   119,   120,   121,   122,     0,
    2298,   123,   117,   118,   119,   120,   121,   122,     0,  2739,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,  2769,   117,   118,   119,   120,   121,   122,     0,
    1692,   123,   117,   118,   119,   120,   121,   122,     0,  1708,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
       0,     0,     0,  1843,     0,   117,   118,   119,   120,   121,
     122,     0,  1844,   123,   117,   118,   119,   120,   121,   122,
       0,  1854,   123,     0,   117,   118,   119,   120,   121,   122,
       0,  1984,   123,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  1993,   117,   118,   119,   120,   121,   122,     0,
       0,   123,  2217,     0,   117,   118,   119,   120,   121,   122,
       0,  2724,   123,     0,   117,   118,   119,   120,   121,   122,
    1307,  2766,   123,   117,   118,   119,   120,   121,   122,  1308,
       0,   123,   117,   118,   119,   120,   121,   122,  1309,     0,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
    1376,     0,   117,   118,   119,   120,   121,   122,     0,  1435,
     123,   117,   118,   119,   120,   121,   122,  1100,     0,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,     0,
       0,  1101,     0,   117,   118,   119,   120,   121,   122,     0,
    1102,   123,     0,   117,   118,   119,   120,   121,   122,     0,
    1174,   123,   117,   118,   119,   120,   121,   122,     0,  1238,
     123,   117,   118,   119,   120,   121,   122,     0,   866,   123,
       0,     0,     0,     0,   334,     0,     0,     0,     0,   867,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   868,
       0,   117,   118,   119,   120,   121,   122,     0,   950,   123,
     117,   118,   119,   120,   121,   122,     0,  1024,   123,     0,
       0,     0,  1010,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,  1229,   117,   118,   119,   120,   121,   122,
       0,   335,   123,     0,     0,     0,     0,     0,  1431,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,  1612,     0,   117,   118,   119,   120,   121,   122,     0,
    1770,   123,   117,   118,   119,   120,   121,   122,     0,  1916,
     123,   117,   118,   119,   120,   121,   122,     0,  2197,   123,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   117,
     118,   119,   120,   121,   122,     0,  1015,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,  1231,     0,   117,
     118,   119,   120,   121,   122,     0,  2088,   123,   117,   118,
     119,   120,   121,   122,     0,     0,   123,     0,  1962,   117,
     118,   119,   120,   121,   122,     0,     0,   123,  1821,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,  1670,   117,   118,   119,   120,   121,   122,     0,
       0,   123,     0,     0,     0,  1496,   117,   118,   119,   120,
     121,   122,     0,  1303,   123,   117,   118,   119,   120,   121,
     122,  1096,     0,   123,     0,     0,     0,     0,     0,   858,
     117,   118,   119,   120,   121,   122,     0,  1613,   123,   117,
     118,   119,   120,   121,   122,     0,  1769,   123,     0,   117,
     118,   119,   120,   121,   122,     0,  1232,   123,   117,   118,
     119,   120,   121,   122,     0,  1430,   123,     0,     0,     0,
     243,     0,     0,     0,     0,  1014,     0,     0,   117,   118,
     119,   120,   121,   122,   602,   244,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,   238,     0,
     117,   118,   119,   120,   121,   122,     0,   240,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,     0,
       0,     0,   247,     0,   117,   118,   119,   120,   121,   122,
       0,   595,   123,     0,   117,   118,   119,   120,   121,   122,
       0,   596,   123,   117,   118,   119,   120,   121,   122,   176,
       0,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,   117,   118,   119,   120,   121,   122,   233,
     528,   123,   117,   118,   119,   120,   121,   122,     0,     0,
     123,     0,     0,   597,     0,   117,   118,   119,   120,   121,
     122,     0,  1048,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,     0,     0,  1118,     0,     0,     0,
       0,   241,     0,   117,   118,   119,   120,   121,   122,     0,
     242,   123,   117,   118,   119,   120,   121,   122,   690,     0,
     123,     0,   117,   118,   119,   120,   121,   122,     0,  1202,
     123,     0,     0,     0,     0,   254,   117,   118,   119,   120,
     121,   122,     0,     0,   123,     0,     0,     0,   255,     0,
     117,   118,   119,   120,   121,   122,     0,   523,   123,   117,
     118,   119,   120,   121,   122,     0,     0,   123,     0,   117,
     118,   119,   120,   121,   122,     0,  1030,   123,   117,   118,
     119,   120,   121,   122,   339,     0,   123,   117,   118,   119,
     120,   121,   122,   601,     0,   123,     0,     0,   117,   118,
     119,   120,   121,   122,     0,   237,   123,   117,   118,   119,
     120,   121,   122,     0,     0,   123,     0,     0,  1175,   117,
     118,   119,   120,   121,   122,     0,   703,   123,     0,     0,
       0,     0,  1525,     0,     0,   951,     0,     0,     0,     0,
       0,  1009,     0,  1541,     0,   117,   118,   119,   120,   121,
     122,     0,  1693,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,     0,  1694,   117,   118,   119,   120,   121,
     122,     0,     0,   123,   117,   118,   119,   120,   121,   122,
       0,     0,   123,   117,   118,   119,   120,   121,   122,     0,
    1704,   123,   117,   118,   119,   120,   121,   122,     0,  1846,
     123,   117,   118,   119,   120,   121,   122,     0,     0,   123,
    1856,     0,     0,     0,     0,     0,     0,     0,     0,  2112,
       0,     0,     0,     0,     0,     0,     0,     0,  2709,     0,
       0,     0,     0,     0,     0,     0,     0,  2763,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295
  };

  const short int
  feiparser::yycheck_[] =
  {
       7,     8,   255,    34,    35,    36,    37,    38,    39,    55,
      55,    42,    19,    20,    21,    34,    35,    36,    37,    38,
      39,   109,     0,    42,    97,    94,    62,    34,    94,    94,
     376,   377,   378,    40,     3,     4,     5,   106,     7,     8,
     106,   106,    86,    87,   406,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    54,    55,   406,    26,    27,    28,
      29,    30,    31,    32,    71,   104,    35,   140,    39,     3,
      48,    42,    41,   112,    34,    35,    36,    37,    38,    39,
      92,    88,    42,    95,    52,    54,    55,    99,    57,    38,
      39,    50,   345,    42,    34,    35,    36,    37,    38,    39,
     409,   108,    42,    36,    37,    38,    39,    54,    55,    42,
     117,   118,   119,   120,   121,   122,   123,    34,    35,    36,
      37,    38,    39,   164,   400,    42,   167,   168,   237,   238,
      54,   138,   139,   140,   136,   137,   143,   144,   175,   176,
     147,   148,   149,   150,    54,   166,   153,   154,   169,   157,
     158,   157,   158,   160,   161,   162,   163,   164,   165,   195,
     157,   158,    34,    35,    36,    37,    38,    39,    70,   123,
      42,    34,    35,    36,    37,    38,    39,   121,     0,    42,
     412,     3,     4,     5,     3,     7,     8,     9,    42,   109,
      12,   109,     5,    15,   201,    17,   109,    19,   109,   120,
     207,   109,   109,   210,    26,    27,    28,    29,    30,    31,
      32,   109,   109,    35,   395,   222,   109,   123,   109,    41,
     120,    43,    44,    45,    46,   182,   111,    49,   405,    51,
     109,    53,    54,    34,    35,    36,    37,    38,    39,    54,
      55,    42,   109,    58,   251,    60,   253,    69,    63,   256,
     109,   258,   109,    75,    76,    77,    78,    34,    35,    36,
      37,    38,    39,   270,   109,    42,    58,   109,    90,    91,
     395,    80,   128,    84,     1,    82,     3,     4,     5,    89,
       7,     8,     9,   163,    96,    12,   108,    79,    15,    81,
      17,    83,    19,    85,   124,    71,    88,   400,   400,    26,
      27,    28,    29,    30,    31,    32,    11,   106,    35,   109,
      63,   119,    54,   119,    41,    54,    43,    44,    45,    46,
     122,   409,    49,   357,    51,   129,    53,    54,   106,    94,
      79,   338,   339,    34,    35,    36,    37,    38,    39,   106,
      94,    42,    69,   350,   351,   352,   353,     5,    75,    76,
      77,    78,    42,   414,   400,   400,   363,   413,   365,   111,
     411,   396,   369,    90,    91,    18,   373,    20,    21,    22,
      23,    24,    25,     3,     4,     5,   109,     7,     8,   111,
      33,   108,   413,    34,    35,    36,    37,    38,    39,   109,
      33,    42,   359,   412,   363,   211,    26,    27,    28,    29,
      30,    31,    32,    98,    54,    35,   165,   406,   166,    54,
      54,    41,    33,   420,   421,   422,   423,   424,    54,     3,
     112,   428,   429,   430,    54,    33,    33,   109,   435,   219,
      34,    35,    36,    37,    38,    39,    33,   406,    42,   175,
     170,    79,    79,   450,    34,    35,    36,    37,    38,    39,
     410,    79,    42,    34,    35,    36,    37,    38,    39,   106,
     106,    42,    94,   336,    94,   106,   106,   407,   408,   334,
     387,   106,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,    34,    35,    36,    37,    38,    39,   106,   106,    42,
     106,   106,   106,   520,   521,   106,   334,   106,   525,   526,
     106,    34,    35,    36,    37,    38,    39,   534,   535,    42,
     393,   538,   162,   540,   541,   542,   408,   544,   545,   546,
     106,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,    34,    35,
      36,    37,    38,    39,   106,   106,    42,   106,   106,    34,
      35,    36,    37,    38,    39,   106,   106,    42,   400,   106,
     269,   403,   404,   269,   406,   269,   269,   350,   269,   269,
     412,   598,   599,   600,   269,   269,   269,   269,   605,     3,
       4,     5,   609,     7,     8,   269,   407,   408,   131,   132,
     133,   618,   135,   136,   269,     3,     4,     5,   270,     7,
       8,   269,    26,    27,    28,    29,    30,    31,    32,   109,
     407,    35,   109,   106,   109,   269,   269,    41,    26,    27,
      28,    29,    30,    31,    32,   269,   269,    35,   270,   269,
      54,   269,   269,    41,   269,    34,    35,    36,    37,    38,
      39,   270,   109,    42,   269,   109,    54,    34,    35,    36,
      37,    38,    39,   400,   106,    42,   403,   404,   106,   406,
     109,    33,   689,    33,   239,   412,   109,   270,   166,   177,
     697,   177,   699,   700,   701,   177,   175,   398,    97,   706,
     104,   708,   709,   710,    97,   712,   713,   714,   114,   716,
     717,   718,   719,   720,   721,    33,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   760,   761,   114,   114,   408,   114,    34,
      35,    36,    37,    38,    39,   114,  1019,    42,   114,   776,
     114,   114,   779,   780,   114,   114,   406,   114,   114,   786,
     787,    34,    35,    36,    37,    38,    39,   114,    54,    42,
     114,   114,   114,    34,    35,    36,    37,    38,    39,   114,
     114,    42,   114,   114,   408,   114,   114,   114,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,   408,    33,
     379,    34,    35,    36,    37,    38,    39,   408,    33,    42,
      34,    35,    36,    37,    38,    39,    33,    33,    42,   846,
     847,   848,    33,    33,    33,   852,   853,   854,   855,   856,
     857,    33,   859,    33,   861,    33,    33,    33,   865,    33,
      33,    33,    33,    33,   871,    33,   873,    33,   875,   876,
     877,    33,   879,   880,   881,   408,   883,   884,    33,    33,
      33,    33,   889,    33,   891,   892,   893,   894,   379,   379,
     897,   898,    33,    33,    33,   408,    34,    35,    36,    37,
      38,    39,    33,   175,    42,    34,    35,    36,    37,    38,
      39,   110,   110,    42,    34,    35,    36,    37,    38,    39,
     177,    33,    42,   110,   931,   932,   933,    33,    33,    33,
      33,   126,   408,   940,   406,   406,   406,   944,   406,   406,
     947,   948,   949,   408,   406,    34,    35,    36,    37,    38,
      39,   406,   406,    42,   406,   406,   406,   406,   406,   966,
      34,    35,    36,    37,    38,    39,   406,   406,    42,   406,
     406,   406,   406,   406,   981,   982,   983,   984,   985,   986,
     987,   988,   989,   990,   991,   992,   993,   994,   995,   996,
     997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,
    1007,  1008,   406,    34,    35,    36,    37,    38,    39,   406,
     406,    42,   406,    33,  1021,   406,  1023,    33,   406,   398,
     112,  1028,  1029,    57,  1031,  1032,  1033,  1034,  1035,  1036,
    1037,  1038,  1039,  1040,  1041,  1042,  1043,   110,  1045,  1046,
    1047,   408,  1049,  1050,  1051,   177,  1053,    57,  1055,  1056,
    1057,  1058,    34,    35,    36,    37,    38,    39,    57,   135,
      42,    34,    35,    36,    37,    38,    39,   112,   179,    42,
     179,   179,   179,    34,    35,    36,    37,    38,    39,    33,
    1087,    42,  1089,  1090,  1091,   112,  1093,  1094,   129,   110,
     109,   397,  1099,    34,    35,    36,    37,    38,    39,    57,
     109,    42,    34,    35,    36,    37,    38,    39,   109,   181,
      42,    34,    35,    36,    37,    38,    39,   181,   181,    42,
     181,  1128,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,    33,   408,    34,    35,    36,    37,    38,    39,
      33,    33,    42,    34,    35,    36,    37,    38,    39,    33,
      33,    42,    33,    33,    33,   408,    33,    33,    33,    33,
    1167,    33,    33,    33,  1171,  1172,  1173,   408,    33,  1176,
    1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,
    1187,  1188,   408,    34,    35,    36,    37,    38,    39,    33,
     394,    42,    33,  1200,  1201,   408,  1203,  1204,  1205,  1206,
    1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,
    1227,  1228,    33,  1230,    33,    33,    33,    33,    33,  1236,
    1237,    33,    33,   115,    54,  1242,  1243,    34,    35,    36,
      37,    38,    39,   110,   110,    42,   115,   115,   103,    33,
      63,  1258,  1259,    57,  1261,    33,    33,  1264,    57,   109,
    1267,    34,    35,    36,    37,    38,    39,    33,   235,    42,
     408,   235,    34,    35,    36,    37,    38,    39,   235,   408,
      42,    34,    35,    36,    37,    38,    39,   235,   408,    42,
     227,    33,  1299,  1300,    33,  1302,    33,   114,    74,  1306,
      74,    33,    33,    33,    33,  1312,  1313,  1314,  1315,  1316,
    1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,   408,
      33,   109,   109,    33,   184,   106,  1333,   109,  1335,  1336,
    1337,   106,    33,    33,   408,    34,    35,    36,    37,    38,
      39,   106,   106,    42,    34,    35,    36,    37,    38,    39,
     106,   109,    42,   406,   109,   399,    33,    33,  1365,    33,
     114,   106,   114,  1370,   114,   114,  1373,  1374,  1375,   227,
      34,    35,    36,    37,    38,    39,   110,   408,    42,   228,
     220,    34,    35,    36,    37,    38,    39,  1394,  1395,    42,
    1397,   220,   220,   220,    33,    33,  1403,  1404,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,
    1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,
    1427,  1428,    33,    33,    33,   398,   408,  1434,    33,    33,
      33,    33,  1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,
    1447,  1448,  1449,  1450,  1451,  1452,   407,  1454,  1455,  1456,
      33,    33,  1459,    33,    33,  1462,  1463,  1464,    33,   406,
      34,    35,    36,    37,    38,    39,    33,   408,    42,    34,
      35,    36,    37,    38,    39,    33,   408,    42,    33,    33,
      33,    33,    33,    33,  1491,   408,  1493,  1494,    33,    33,
      33,  1498,    33,    33,    33,    33,   408,    33,   406,    34,
      35,    36,    37,    38,    39,    33,  1513,    42,   408,    33,
     406,   406,    33,    33,  1521,    33,    33,   408,    34,    35,
      36,    37,    38,    39,    57,    33,    42,    33,    33,    33,
      33,    33,   110,    34,    35,    36,    37,    38,    39,    33,
     391,    42,    34,    35,    36,    37,    38,    39,    33,  1556,
      42,    33,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,    33,   112,   109,     5,  1576,
    1577,  1578,   375,    33,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,
    1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,  1606,
    1607,  1608,  1609,    33,  1611,    33,    33,    57,  1615,    33,
     110,   408,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,    33,  1630,  1631,  1632,    33,    33,    33,  1636,
      33,    33,    33,    33,    33,   408,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,   408,    65,    34,    35,
      36,    37,    38,    39,    33,   408,    42,    33,    33,  1666,
      33,  1668,  1669,    33,    33,  1672,    33,  1674,  1675,    33,
    1677,  1678,  1679,  1680,    33,  1682,    33,  1684,  1685,    33,
      33,    33,    33,    33,  1691,    34,    35,    36,    37,    38,
      39,    33,   112,    42,    34,    35,    36,    37,    38,    39,
      33,   110,    42,    34,    35,    36,    37,    38,    39,   408,
    1717,    42,    33,   110,  1721,   109,    57,   110,   408,    34,
      35,    36,    37,    38,    39,    33,    33,    42,   392,    33,
      33,    33,    33,   386,    57,  1742,  1743,  1744,  1745,  1746,
    1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,  1755,  1756,
    1757,  1758,  1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,
      57,  1768,   109,    57,    33,  1772,   109,  1774,  1775,     5,
    1777,  1778,  1779,  1780,   375,  1782,    33,  1784,    33,  1786,
    1787,  1788,    33,    33,  1791,    34,    35,    36,    37,    38,
      39,   109,   109,    42,    34,    35,    36,    37,    38,    39,
     109,    33,    42,    34,    35,    36,    37,    38,    39,    33,
    1817,    42,  1819,  1820,    33,    33,  1823,    33,    33,  1826,
      33,    33,    33,    33,  1831,    33,  1833,    33,    33,    33,
      33,    33,  1839,  1840,   408,   106,    34,    35,    36,    37,
      38,    39,    33,   408,    42,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,    33,    33,    33,  1866,
      33,    33,  1869,  1870,    33,  1872,  1873,    33,  1875,  1876,
    1877,  1878,   407,  1880,   390,  1882,  1883,    33,    33,    33,
      33,    33,  1889,  1890,  1891,  1892,  1893,  1894,  1895,  1896,
    1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,  1905,  1906,
    1907,  1908,  1909,  1910,  1911,  1912,  1913,   408,   375,    34,
      35,    36,    37,    38,    39,    33,   408,    42,    33,    33,
      33,    33,    33,  1930,  1931,  1932,  1933,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,   110,    34,    35,    36,
      37,    38,    39,  1960,  1961,    42,    33,  1964,  1965,  1966,
    1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,    33,    33,
      33,    33,  1979,    33,    33,   406,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,   408,   110,    34,    35,
      36,    37,    38,    39,    33,  2002,    42,   383,  2005,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    33,    33,
     408,    33,    33,    33,  2021,    33,  2023,  2024,  2025,  2026,
    2027,  2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2039,  2040,  2041,  2042,    33,  2044,    33,    33,
      33,    33,  2049,  2050,    33,  2052,  2053,  2054,  2055,    33,
    2057,  2058,  2059,  2060,  2061,  2062,    33,  2064,    33,   408,
     109,    34,    35,    36,    37,    38,    39,    33,   408,    42,
      34,    35,    36,    37,    38,    39,    33,   408,    42,  2086,
      33,    33,  2089,    33,    33,  2092,    33,   110,    33,    33,
    2097,   110,    33,   408,   110,    57,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,  2123,    33,  2125,  2126,
      33,  2128,  2129,    33,  2131,  2132,  2133,  2134,    33,  2136,
    2137,    33,    33,    33,    33,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2159,    33,    33,    33,    33,    33,    33,   408,
      57,    34,    35,    36,    37,    38,    39,    33,   408,    42,
      34,    35,    36,    37,    38,    39,    57,   408,    42,    33,
      33,   109,    34,    35,    36,    37,    38,    39,  2195,  2196,
      42,  2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,  2206,
    2207,    57,  2209,  2210,  2211,  2212,    33,    33,    57,    33,
     408,   109,    34,    35,    36,    37,    38,    39,  2225,   408,
      42,   109,    34,    35,    36,    37,    38,    39,    33,  2236,
      42,    33,   406,    33,  2241,  2242,  2243,  2244,  2245,  2246,
    2247,  2248,  2249,  2250,  2251,  2252,   109,  2254,  2255,   406,
    2257,  2258,  2259,  2260,    33,  2262,  2263,    33,  2265,  2266,
    2267,  2268,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,    33,    33,    33,    33,  2283,    33,    33,    33,
      33,  2288,   407,    33,  2291,    34,    35,    36,    37,    38,
      39,   389,    33,    42,    33,    33,  2303,  2304,  2305,  2306,
     407,  2308,  2309,  2310,  2311,    33,  2313,  2314,    33,  2316,
    2317,  2318,  2319,  2320,  2321,  2322,  2323,  2324,  2325,  2326,
     407,    34,    35,    36,    37,    38,    39,   385,    33,    42,
      33,    33,  2339,    33,   406,    34,    35,    36,    37,    38,
      39,    33,    33,    42,  2351,  2352,  2353,  2354,  2355,  2356,
    2357,  2358,  2359,  2360,  2361,  2362,    33,    33,   106,    33,
    2367,   407,   106,    34,    35,    36,    37,    38,    39,    33,
      33,    42,   407,   110,    33,    33,    33,    33,    33,    33,
      33,  2388,  2389,  2390,  2391,  2392,  2393,  2394,    33,  2396,
    2397,  2398,  2399,    33,  2401,  2402,    33,    33,    33,    33,
    2407,    34,    35,    36,    37,    38,    39,    33,  2415,    42,
      33,   384,    33,  2420,    33,    33,    33,  2424,  2425,  2426,
     384,    33,    33,    33,  2431,  2432,  2433,  2434,    57,  2436,
    2437,  2438,  2439,    33,  2441,  2442,  2443,   109,    33,    33,
    2447,  2448,  2449,  2450,    34,    35,    36,    37,    38,    39,
     406,    33,    42,    33,    33,    33,    33,  2464,  2465,  2466,
      33,    33,    33,    33,  2471,    33,  2473,  2474,  2475,   407,
    2477,    33,    33,  2480,  2481,  2482,  2483,    33,   407,    33,
    2487,    34,    35,    36,    37,    38,    39,    33,    33,    42,
      33,    33,    33,   233,    33,  2502,  2503,  2504,    33,  2506,
    2507,  2508,  2509,  2510,    33,    33,  2513,    33,    33,    33,
    2517,  2518,  2519,   406,   406,   406,  2523,   406,    33,    33,
      33,    33,  2529,  2530,    33,  2532,  2533,  2534,    34,    35,
      36,    37,    38,    39,   407,    33,    42,    33,  2545,  2546,
      33,    33,  2549,   407,    33,    33,  2553,  2554,    33,    33,
      33,    33,  2559,  2560,  2561,   407,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,    33,   380,  2576,
    2577,  2578,  2579,   233,  2581,  2582,  2583,  2584,  2585,  2586,
    2587,   233,   233,    33,    33,   407,    34,    35,    36,    37,
      38,    39,    33,   234,    42,  2602,  2603,  2604,  2605,    33,
      33,  2608,  2609,  2610,  2611,  2612,  2613,    33,    33,    33,
      33,  2618,  2619,   406,   406,    34,    35,    36,    37,    38,
      39,  2628,  2629,    42,   406,  2632,  2633,  2634,  2635,  2636,
    2637,  2638,  2639,   406,    33,   407,    33,    33,    33,    33,
      33,   110,    33,    57,  2651,  2652,  2653,  2654,  2655,   110,
     110,    57,  2659,    57,    33,   109,   109,   109,   407,    33,
     234,  2668,  2669,  2670,    33,   234,  2673,    34,    35,    36,
      37,    38,    39,  2680,   234,    42,    33,    33,    33,  2686,
      33,    33,  2689,    33,    33,  2692,  2693,   406,  2695,   406,
     406,  2698,  2699,   110,   407,  2702,    33,  2704,  2705,    33,
      33,   109,    33,    57,    33,    33,  2713,  2714,   407,    33,
      33,    33,   110,    57,  2721,    57,    57,    33,   110,    33,
     110,  2728,  2729,  2730,   109,  2732,   109,   109,    33,  2736,
      33,    33,   110,    33,  2741,  2742,   407,   110,    57,  2746,
      57,    33,    33,  2750,  2751,   109,   109,    33,  2755,    33,
     383,    33,  2759,  2760,    33,    33,    33,  2764,    33,    33,
    2767,    33,    33,  2770,    33,    33,  2773,    33,    33,  2776,
       3,     4,     5,   193,     7,     8,     9,    -1,    -1,    12,
      -1,    -1,    15,    -1,    17,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      43,    44,    45,    46,    -1,    -1,    49,   407,    51,    -1,
      53,    54,    -1,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    12,    -1,    -1,    15,    69,    17,    -1,    19,
      -1,    -1,    75,    76,    77,    78,    26,    27,    28,    29,
      30,    31,    32,    -1,   407,    35,    -1,    90,    91,    -1,
      -1,    41,    -1,    43,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    53,    54,   108,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    -1,
      -1,   407,    -1,    -1,    34,    35,    36,    37,    38,    39,
      90,    91,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,   108,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   407,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   407,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
     407,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,   400,    42,    -1,
     403,   404,    -1,   406,    -1,    -1,    -1,    -1,    -1,   412,
      -1,   414,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     400,    -1,    -1,   403,   404,    -1,   406,    -1,    -1,    -1,
      -1,    -1,   412,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   407,    -1,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   407,    -1,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   407,    -1,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   407,    -1,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   407,    -1,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   407,    -1,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     407,    -1,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   407,    -1,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    -1,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   407,    -1,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   407,    -1,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   407,    -1,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   407,    -1,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   407,    -1,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   407,    -1,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     407,    -1,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   407,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   407,    -1,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   407,    -1,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   407,    -1,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   407,    -1,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   407,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   407,    -1,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   407,    -1,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   407,    -1,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     407,    -1,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   407,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   407,    -1,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   407,    -1,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   407,    -1,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    34,    35,    36,    37,    38,    39,
      -1,   407,    42,    34,    35,    36,    37,    38,    39,    -1,
     407,    42,    34,    35,    36,    37,    38,    39,    -1,   407,
      42,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   407,    54,    55,    56,    57,    58,
      -1,    60,    61,   407,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,   407,    -1,    -1,    74,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   407,    -1,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,    -1,   114,    -1,    -1,    -1,    -1,
      -1,   407,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   407,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   407,    -1,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    34,    35,    36,
      37,    38,    39,    -1,   407,    42,    34,    35,    36,    37,
      38,    39,    -1,   407,    42,    34,    35,    36,    37,    38,
      39,    -1,   407,    42,   183,    34,    35,    36,    37,    38,
      39,   407,    -1,    42,    34,    35,    36,    37,    38,    39,
     407,    -1,    42,    34,    35,    36,    37,    38,    39,   407,
      -1,    42,    34,    35,    36,    37,    38,    39,   407,    -1,
      42,    34,    35,    36,    37,    38,    39,   407,    -1,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   407,    42,
      34,    35,    36,    37,    38,    39,    -1,   407,    42,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   407,    -1,    34,
      35,    36,    37,    38,    39,    -1,   407,    42,    34,    35,
      36,    37,    38,    39,    -1,   407,    42,    -1,   383,    -1,
      -1,    -1,    -1,    -1,   407,    -1,   382,    -1,    -1,    -1,
      -1,    -1,    -1,   407,    -1,   382,    -1,    34,    35,    36,
      37,    38,    39,    -1,   382,    42,    34,    35,    36,    37,
      38,    39,    -1,   382,    42,    34,    35,    36,    37,    38,
      39,   381,    -1,    42,    34,    35,    36,    37,    38,    39,
     381,    -1,    42,    34,    35,    36,    37,    38,    39,   381,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   380,    -1,    34,
      35,    36,    37,    38,    39,    -1,   380,    42,    34,    35,
      36,    37,    38,    39,    -1,   380,    42,    34,    35,    36,
      37,    38,    39,    -1,   380,    42,    -1,    -1,    -1,   388,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   361,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,   361,    -1,    34,    35,    36,    37,
      38,    39,    -1,   361,    42,    34,    35,    36,    37,    38,
      39,   360,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   360,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   358,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     356,    34,    35,    36,    37,    38,    39,    -1,   355,    42,
      34,    35,    36,    37,    38,    39,   354,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   353,    -1,    34,    35,    36,    37,
      38,    39,    -1,   352,    42,    34,    35,    36,    37,    38,
      39,   351,    -1,    42,    -1,    -1,    -1,    -1,   349,    34,
      35,    36,    37,    38,    39,    -1,   348,    42,    34,    35,
      36,    37,    38,    39,   347,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,   347,    42,    34,    35,    36,    37,
      38,    39,   346,    -1,    42,    34,    35,    36,    37,    38,
      39,   346,    -1,    42,    34,    35,    36,    37,    38,    39,
     346,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
     345,    42,    34,    35,    36,    37,    38,    39,    -1,   345,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   345,    34,
      35,    36,    37,    38,    39,    -1,   344,    42,    34,    35,
      36,    37,    38,    39,    -1,   344,    42,    34,    35,    36,
      37,    38,    39,    -1,   344,    42,    34,    35,    36,    37,
      38,    39,   343,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   343,    -1,
      34,    35,    36,    37,    38,    39,    -1,   343,    42,    34,
      35,    36,    37,    38,    39,   342,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   342,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   342,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   341,    34,    35,    36,    37,    38,    39,
      -1,   340,    42,    34,    35,    36,    37,    38,    39,   339,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   338,    -1,    34,    35,    36,    37,    38,
      39,    -1,   337,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   335,    42,    34,    35,    36,    37,    38,    39,
      -1,   335,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   332,    -1,    34,    35,    36,    37,
      38,    39,    -1,   332,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,   331,    -1,    34,    35,
      36,    37,    38,    39,    -1,   331,    42,    34,    35,    36,
      37,    38,    39,   330,    -1,    42,    34,    35,    36,    37,
      38,    39,   330,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     329,    34,    35,    36,    37,    38,    39,    -1,   328,    42,
      34,    35,    36,    37,    38,    39,   327,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   326,    34,    35,    36,    37,    38,
      39,    -1,   325,    42,    34,    35,    36,    37,    38,    39,
     324,    -1,    42,    -1,    -1,    -1,    -1,    -1,   323,    -1,
      34,    35,    36,    37,    38,    39,    -1,   322,    42,    34,
      35,    36,    37,    38,    39,   321,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   320,    34,    35,    36,    37,    38,    39,
      -1,   319,    42,    34,    35,    36,    37,    38,    39,   318,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   317,    34,    35,    36,    37,    38,    39,
      -1,   316,    42,    34,    35,    36,    37,    38,    39,   315,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,   314,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   314,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   313,    34,    35,    36,    37,    38,    39,
     311,    -1,    42,    -1,    -1,    -1,    -1,    -1,   310,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     309,    -1,    34,    35,    36,    37,    38,    39,    -1,   308,
      42,    34,    35,    36,    37,    38,    39,   307,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   306,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   305,    34,    35,
      36,    37,    38,    39,    -1,   304,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   303,    34,    35,
      36,    37,    38,    39,    -1,   302,    42,    34,    35,    36,
      37,    38,    39,   301,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   300,    34,    35,
      36,    37,    38,    39,    -1,   299,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,   298,
      34,    35,    36,    37,    38,    39,    -1,   297,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   296,    34,    35,    36,    37,    38,    39,    -1,
     295,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   294,    34,    35,    36,    37,    38,
      39,    -1,   293,    42,    34,    35,    36,    37,    38,    39,
     292,    -1,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   291,    42,    34,    35,    36,    37,    38,    39,   290,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   289,    -1,    -1,
      -1,    -1,   285,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   285,    -1,    34,    35,    36,
      37,    38,    39,    -1,   284,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   283,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     282,    34,    35,    36,    37,    38,    39,    -1,   281,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   280,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   279,    -1,    34,    35,    36,    37,    38,
      39,    -1,   279,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   278,    42,    34,    35,    36,    37,    38,    39,
      -1,   278,    42,    34,    35,    36,    37,    38,    39,    -1,
     278,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   278,    34,    35,    36,    37,    38,    39,    -1,
     277,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   277,    -1,    34,    35,    36,    37,    38,
      39,    -1,   277,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   277,    -1,    34,    35,
      36,    37,    38,    39,    -1,   277,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   277,    -1,
      34,    35,    36,    37,    38,    39,    -1,   277,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   277,    42,    34,
      35,    36,    37,    38,    39,    -1,   277,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   277,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     277,    -1,    34,    35,    36,    37,    38,    39,    -1,   277,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   277,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   277,    -1,    34,    35,    36,    37,
      38,    39,    -1,   277,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,   277,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   277,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   277,    -1,
      34,    35,    36,    37,    38,    39,    -1,   277,    42,    34,
      35,    36,    37,    38,    39,    -1,   277,    42,    34,    35,
      36,    37,    38,    39,    -1,   277,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   276,    42,    34,    35,    36,
      37,    38,    39,    -1,   276,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   276,    -1,    34,
      35,    36,    37,    38,    39,    -1,   276,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   275,
      -1,    34,    35,    36,    37,    38,    39,    -1,   275,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   275,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   275,    34,    35,    36,    37,    38,    39,    -1,   274,
      42,    34,    35,    36,    37,    38,    39,    -1,   274,    42,
      34,    35,    36,    37,    38,    39,    -1,   274,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   274,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   274,    -1,    34,    35,    36,    37,    38,    39,
      -1,   274,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   273,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   273,    -1,    34,    35,    36,
      37,    38,    39,    -1,   273,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   273,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   273,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   273,
      -1,    34,    35,    36,    37,    38,    39,    -1,   273,    42,
      34,    35,    36,    37,    38,    39,    -1,   273,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   273,    42,    34,
      35,    36,    37,    38,    39,   272,    -1,    42,    34,    35,
      36,    37,    38,    39,   272,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   272,    -1,    34,
      35,    36,    37,    38,    39,    -1,   272,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   272,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     272,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     272,    -1,    34,    35,    36,    37,    38,    39,    -1,   272,
      42,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      34,    35,    36,    37,    38,    39,    -1,   272,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   272,    -1,    34,    35,    36,    37,    38,    39,
      -1,   272,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   272,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   272,    -1,    34,    35,    36,
      37,    38,    39,    -1,   272,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   272,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   272,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   272,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   272,    42,
      34,    35,    36,    37,    38,    39,    -1,   272,    42,    34,
      35,    36,    37,    38,    39,    -1,   272,    42,    34,    35,
      36,    37,    38,    39,   271,    -1,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   270,    42,    34,    35,    36,
      37,    38,    39,    -1,   270,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   270,    -1,    34,
      35,    36,    37,    38,    39,    -1,   270,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   270,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   270,    -1,
      34,    35,    36,    37,    38,    39,    -1,   270,    42,    -1,
      34,    35,    36,    37,    38,    39,    -1,   270,    42,    34,
      35,    36,    37,    38,    39,    -1,   270,    42,    34,    35,
      36,    37,    38,    39,    -1,   270,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     270,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     270,    -1,    34,    35,    36,    37,    38,    39,    -1,   270,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   270,    -1,    34,    35,    36,    37,    38,
      39,    -1,   270,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   270,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,   270,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,   270,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   270,    -1,    34,
      35,    36,    37,    38,    39,    -1,   270,    42,    34,    35,
      36,    37,    38,    39,    -1,   270,    42,    34,    35,    36,
      37,    38,    39,    -1,   270,    42,    34,    35,    36,    37,
      38,    39,    -1,   269,    42,    34,    35,    36,    37,    38,
      39,    -1,   269,    42,    34,    35,    36,    37,    38,    39,
      -1,   269,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   269,    -1,    34,    35,    36,    37,
      38,    39,    -1,   269,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   269,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   239,    42,   269,    34,    35,    36,    37,
      38,    39,   239,    -1,    42,   269,    -1,    34,    35,    36,
      37,    38,    39,    -1,   269,    42,   239,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   239,    -1,    34,
      35,    36,    37,    38,    39,    -1,   239,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   236,    -1,    -1,
      -1,    -1,    -1,   232,    -1,    34,    35,    36,    37,    38,
      39,    -1,   232,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   232,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   232,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   231,    34,    35,    36,
      37,    38,    39,    -1,   230,    42,    34,    35,    36,    37,
      38,    39,   229,    -1,    42,    -1,    -1,   225,    34,    35,
      36,    37,    38,    39,    -1,   224,    42,    34,    35,    36,
      37,    38,    39,    -1,   224,    42,    34,    35,    36,    37,
      38,    39,    -1,   224,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   223,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   223,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   223,    -1,    34,    35,    36,
      37,    38,    39,    -1,   222,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   222,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   222,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   221,    -1,    34,    35,
      36,    37,    38,    39,    -1,   221,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,   221,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     220,    34,    35,    36,    37,    38,    39,    -1,   218,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   218,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   218,    34,    35,    36,    37,    38,    39,    -1,   217,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   217,    -1,    34,    35,    36,    37,    38,    39,    -1,
     217,    42,    34,    35,    36,    37,    38,    39,   216,    -1,
      42,    34,    35,    36,    37,    38,    39,   216,    -1,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   216,    42,
      34,    35,    36,    37,    38,    39,    -1,   215,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   215,    -1,
      34,    35,    36,    37,    38,    39,    -1,   215,    42,    34,
      35,    36,    37,    38,    39,    -1,   214,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   214,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   214,    34,
      35,    36,    37,    38,    39,    -1,   213,    42,    34,    35,
      36,    37,    38,    39,    -1,   213,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     213,    34,    35,    36,    37,    38,    39,    -1,   212,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     212,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     212,    34,    35,    36,    37,    38,    39,    -1,   211,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     211,    -1,    34,    35,    36,    37,    38,    39,    -1,   211,
      42,    -1,    -1,    -1,    -1,   180,    -1,   210,    -1,    -1,
      34,    35,    36,    37,    38,    39,   180,   210,    42,    34,
      35,    36,    37,    38,    39,   180,   210,    42,    34,    35,
      36,    37,    38,    39,    -1,   180,    42,    -1,   174,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   174,
      -1,    34,    35,    36,    37,    38,    39,    -1,   174,    42,
      34,    35,    36,    37,    38,    39,    -1,   174,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   174,    -1,    34,    35,    36,    37,    38,    39,    -1,
     174,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   174,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   174,    -1,    34,    35,    36,    37,    38,    39,    -1,
     174,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   174,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     174,    42,    34,    35,    36,    37,    38,    39,    -1,   174,
      42,    34,    35,    36,    37,    38,    39,    -1,   174,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   174,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   174,    -1,    34,    35,    36,    37,    38,    39,    -1,
     174,    42,    34,    35,    36,    37,    38,    39,    -1,   174,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   174,    -1,    34,    35,    36,    37,    38,
      39,    -1,   174,    42,    34,    35,    36,    37,    38,    39,
      -1,   173,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   173,    -1,    34,    35,    36,    37,    38,    39,
      -1,   173,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   173,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   173,    -1,    34,    35,    36,    37,    38,    39,
      -1,   173,    42,    34,    35,    36,    37,    38,    39,    -1,
     173,    42,    34,    35,    36,    37,    38,    39,    -1,   173,
      42,    -1,    34,    35,    36,    37,    38,    39,    -1,   173,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   173,    -1,    34,    35,    36,    37,    38,    39,
      -1,   173,    42,    34,    35,    36,    37,    38,    39,    -1,
     173,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   173,    -1,    34,    35,    36,    37,
      38,    39,    -1,   173,    42,    34,    35,    36,    37,    38,
      39,    -1,   173,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   173,    -1,    34,    35,    36,    37,    38,
      39,    -1,   173,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   173,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   173,    34,    35,    36,    37,    38,    39,
      -1,   172,    42,    34,    35,    36,    37,    38,    39,    -1,
     172,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     172,    42,    34,    35,    36,    37,    38,    39,    -1,   172,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   172,    -1,    34,    35,    36,    37,    38,    39,
      -1,   172,    42,    34,    35,    36,    37,    38,    39,    -1,
     172,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   172,    -1,    34,    35,    36,    37,
      38,    39,    -1,   172,    42,    34,    35,    36,    37,    38,
      39,    -1,   172,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   172,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   172,    -1,    34,    35,    36,    37,    38,
      39,    -1,   172,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   172,    42,    34,    35,    36,    37,    38,    39,
      -1,   172,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   172,    42,    34,    35,    36,    37,    38,    39,    -1,
     172,    42,    34,    35,    36,    37,    38,    39,    -1,   172,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   172,    34,    35,    36,    37,    38,    39,    -1,
     171,    42,    34,    35,    36,    37,    38,    39,    -1,   171,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   171,    -1,    34,    35,    36,    37,    38,
      39,    -1,   171,    42,    34,    35,    36,    37,    38,    39,
      -1,   171,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   171,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   171,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   171,    -1,    34,    35,    36,    37,    38,    39,
      -1,   171,    42,    -1,    34,    35,    36,    37,    38,    39,
     161,   171,    42,    34,    35,    36,    37,    38,    39,   161,
      -1,    42,    34,    35,    36,    37,    38,    39,   161,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     161,    -1,    34,    35,    36,    37,    38,    39,    -1,   161,
      42,    34,    35,    36,    37,    38,    39,   160,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   160,    -1,    34,    35,    36,    37,    38,    39,    -1,
     160,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     160,    42,    34,    35,    36,    37,    38,    39,    -1,   160,
      42,    34,    35,    36,    37,    38,    39,    -1,   159,    42,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,   159,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   159,
      -1,    34,    35,    36,    37,    38,    39,    -1,   159,    42,
      34,    35,    36,    37,    38,    39,    -1,   159,    42,    -1,
      -1,    -1,   155,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   155,    34,    35,    36,    37,    38,    39,
      -1,   154,    42,    -1,    -1,    -1,    -1,    -1,   152,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   152,    -1,    34,    35,    36,    37,    38,    39,    -1,
     151,    42,    34,    35,    36,    37,    38,    39,    -1,   151,
      42,    34,    35,    36,    37,    38,    39,    -1,   151,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,   150,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   150,    -1,    34,
      35,    36,    37,    38,    39,    -1,   150,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   149,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   148,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   147,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   146,    34,    35,    36,    37,
      38,    39,    -1,   145,    42,    34,    35,    36,    37,    38,
      39,   144,    -1,    42,    -1,    -1,    -1,    -1,    -1,   143,
      34,    35,    36,    37,    38,    39,    -1,   142,    42,    34,
      35,    36,    37,    38,    39,    -1,   142,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   141,    42,    34,    35,
      36,    37,    38,    39,    -1,   141,    42,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,   140,    -1,    -1,    34,    35,
      36,    37,    38,    39,   139,   126,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   126,    -1,
      34,    35,    36,    37,    38,    39,    -1,   126,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   126,    -1,    34,    35,    36,    37,    38,    39,
      -1,   126,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   126,    42,    34,    35,    36,    37,    38,    39,   125,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    34,    35,    36,    37,    38,    39,   125,
     117,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   117,    -1,    34,    35,    36,    37,    38,
      39,    -1,   117,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,   111,    -1,    34,    35,    36,    37,    38,    39,    -1,
     111,    42,    34,    35,    36,    37,    38,    39,   110,    -1,
      42,    -1,    34,    35,    36,    37,    38,    39,    -1,   110,
      42,    -1,    -1,    -1,    -1,   107,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   107,    -1,
      34,    35,    36,    37,    38,    39,    -1,   107,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   107,    42,    34,    35,
      36,    37,    38,    39,   106,    -1,    42,    34,    35,    36,
      37,    38,    39,   105,    -1,    42,    -1,    -1,    34,    35,
      36,    37,    38,    39,    -1,   103,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   102,    34,
      35,    36,    37,    38,    39,    -1,   101,    42,    -1,    -1,
      -1,    -1,    79,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    79,    -1,    34,    35,    36,    37,    38,
      39,    -1,    79,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    79,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      79,    42,    34,    35,    36,    37,    38,    39,    -1,    79,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,     9,    12,    15,
      17,    19,    26,    27,    28,    29,    30,    31,    32,    35,
      41,    43,    44,    45,    46,    49,    51,    53,    54,    69,
      75,    76,    77,    78,    90,    91,   108,   400,   403,   404,
     406,   412,   416,   417,   418,   420,   421,   422,   425,   426,
     427,    18,    20,    21,    22,    23,    24,    25,    33,   406,
     406,   426,   426,     3,   426,   426,    54,    55,    57,   363,
     426,    52,    50,    54,    55,    56,    57,    58,    60,    61,
      63,    64,    65,    74,   114,   183,   388,   423,   409,   400,
      54,    54,    54,    55,    58,    60,    63,    58,    79,    81,
      83,    85,    88,    94,   162,   426,    70,   123,   121,    55,
     400,    55,   400,   426,   412,     0,   427,    34,    35,    36,
      37,    38,    39,    42,   412,     3,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   413,   428,   428,   109,   109,
     109,   426,     5,   109,   109,   424,   120,   109,   109,   109,
     109,   395,   123,   109,   109,   120,   182,   111,   426,   405,
     109,   109,   109,   109,   109,   109,   395,   128,    80,    82,
      84,    86,    87,    89,    96,   163,   125,    71,   124,   426,
     400,   400,   408,   426,   426,   426,   426,   426,   426,   426,
     408,   408,   414,   427,   429,    11,   426,   426,   426,   426,
     426,   119,   426,   426,   426,   426,   106,   109,   426,   426,
     119,    63,    54,   410,   122,   426,   426,   426,   426,   426,
     426,    54,   129,   357,   106,   106,    94,    79,   164,   167,
     168,    94,     5,   125,   429,   414,   428,   103,   126,   426,
     126,   111,   111,   111,   126,   396,   426,   126,   352,   426,
     111,   109,   411,   111,   107,   107,   109,   426,    33,   211,
     359,    54,    98,   165,   166,   166,   169,    92,    95,    99,
     406,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,    54,    55,    54,    54,    55,
     376,   377,   378,    54,    97,   154,   237,   238,    33,   106,
      54,   426,     3,   426,   104,   112,   419,   419,   426,   426,
      33,    33,   109,    33,   219,   175,   175,   176,   170,    79,
      79,    79,   426,    94,   106,    94,   106,   106,   106,    94,
     106,   106,   106,    94,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   336,   270,   334,   334,   270,   269,   270,   350,   269,
     109,   109,   109,   109,   109,   106,   106,   106,   109,    33,
      33,   239,   270,   426,   426,   109,   389,   419,   356,   426,
     426,   426,   426,   166,   177,   177,   177,   175,    97,    97,
     407,   426,   114,   426,   114,   114,   114,   426,   114,   114,
     114,   426,   114,   114,   114,   114,   114,   114,    54,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,   104,   426,
     426,   426,   426,   426,   379,   379,   379,   426,   426,   426,
      33,    33,   353,   107,   426,    33,    33,   358,   117,   175,
     110,   110,   110,   177,    33,    33,   426,   180,   406,   180,
     406,   406,   406,   180,   406,   406,   406,   180,   406,   406,
     406,   406,   406,   406,   406,   406,   406,   406,   406,   406,
     406,   406,   406,   406,   406,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   126,   126,   126,   117,    33,    33,
      33,   105,   139,   426,   426,    33,   398,   426,   426,    33,
     112,   177,    57,    57,    57,   110,   426,   426,   407,   179,
     426,   179,   426,   426,   426,   179,   426,   426,   426,   179,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   314,   330,   330,   314,
     270,   270,   270,   270,   270,   270,   283,   337,   271,   270,
     270,   272,   270,   272,   272,   270,   135,   131,   132,   133,
     135,   136,   136,   137,   112,   426,   426,   426,   397,    33,
     110,   272,   426,   112,   390,   355,   426,   129,   110,   109,
     109,   109,    57,   101,   426,   181,   407,   181,   407,   407,
     407,   181,   407,   407,   407,   181,   407,   407,   407,   407,
     407,   407,   408,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,   115,   115,   115,   110,   110,   103,    97,   140,
     380,   380,   380,    54,   426,    57,    33,   354,    63,    33,
      33,   426,    57,   426,   426,   426,   109,    33,   408,   235,
     426,   235,   426,   426,   426,   235,   426,   426,   426,   235,
     426,   426,   426,   426,   426,   426,   227,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,    33,    33,    33,    74,
      74,   114,    33,    33,    33,    33,    33,   109,   143,   109,
     426,    33,   184,   426,   426,   109,   159,   159,   159,   426,
     426,   106,   106,   407,   106,   407,   407,   407,   106,   407,
     407,   407,   106,   407,   407,   408,   408,   408,   408,    33,
     408,   407,   407,   407,   407,   408,   408,   407,   407,   408,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   311,
     331,   331,   315,   272,   272,   272,   272,   272,   272,   284,
     338,   335,   335,   342,   272,   342,   342,   272,   426,   426,
     426,   109,   109,   406,   426,   426,   426,   426,   426,   426,
      33,   426,   239,   426,   399,   391,   426,    33,    33,    33,
     159,   100,   426,   114,   426,   114,   426,   426,   426,   114,
     426,   426,   426,   114,   426,   426,   106,   220,   220,   227,
     426,   220,   426,   426,   426,   426,   110,   228,   426,   426,
     220,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    97,
     155,   426,   426,   426,   140,   150,   381,   382,   382,   398,
     426,    33,   426,    33,   159,   426,   426,   426,    33,    33,
     107,   406,   407,   406,   407,   407,   407,   406,   407,   407,
     407,   406,   407,   407,   426,    33,    33,    33,   117,    33,
     407,   407,   408,   407,    57,    33,   407,   407,    33,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,    33,    33,   407,
      33,    33,    33,    33,    33,   419,   144,   426,   426,    33,
     160,   160,   160,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   116,   426,
     426,   426,   112,   426,   426,   426,   110,   426,   109,   426,
     426,   426,   426,   273,   273,   273,   273,   273,   273,   273,
     273,   291,   361,   361,   272,   274,   274,   274,   274,   274,
     274,   280,   339,   380,   385,   343,   273,   343,   343,   426,
       5,   426,   426,   426,   375,   426,   426,    33,   269,   392,
     426,    33,    33,    33,   160,   102,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   110,
     270,   270,   398,   270,   408,   408,    57,   408,   426,   229,
     407,   407,   110,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,   155,
     407,   150,   141,   381,   383,   426,    33,    33,   160,   426,
     426,   426,    33,    33,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,    65,    33,    33,
     112,    33,   110,   110,   109,   110,   220,    33,   426,   426,
      57,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,    33,   426,    33,
      33,    33,    33,   145,   426,   426,    33,   161,   161,   161,
     426,   426,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   109,   426,   426,   426,    57,
      57,   426,    57,    33,   426,   407,   407,   109,   277,   278,
     279,   279,   275,   277,   278,   275,   289,   360,   360,   313,
     278,   278,   275,   275,   277,   277,   281,   361,   382,   386,
     344,   277,   344,   344,     5,   407,   426,   426,   375,   426,
      33,   393,   426,    33,    33,    33,   161,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   221,   221,   221,   109,   109,   239,   109,   426,   230,
     426,   426,   426,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,   426,
     141,   152,   381,   426,    33,   161,   426,   426,   426,    33,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   269,    33,    33,    33,   426,   426,    33,
     426,   269,    33,   407,   407,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   407,   408,    33,    33,    33,   146,   426,    33,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,    33,   426,   426,   426,   239,   239,   426,
     239,    33,   426,   426,   426,    79,   277,   277,   277,   276,
     172,   277,   276,   290,   272,   272,   316,   277,   277,   276,
     276,    79,   172,   282,   340,   383,   387,   345,   172,   345,
     345,   426,   106,   426,   426,   375,    33,   394,   426,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   426,   222,   222,   222,    33,    33,    33,   426,
     231,   407,   407,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,   407,
     157,   158,   152,   142,   426,    33,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   225,
      33,    33,    33,   426,   426,   426,    33,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   406,   426,    33,    33,
     147,   426,   407,   408,   407,   407,   408,   407,   407,   407,
     407,   408,   407,   408,   407,    33,   426,   426,   426,   426,
     408,   407,   171,    79,    79,   172,   277,   173,   172,   277,
     292,   332,   332,   317,    79,   172,   277,   277,   171,   173,
     341,   384,   380,   346,   173,   346,   346,   407,   426,   426,
     426,    33,   426,   110,   426,   426,   110,   426,   426,   426,
     426,   110,   426,   110,   426,   426,   224,   224,   224,   236,
     110,   426,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,   426,   407,   142,
     151,   426,   407,    57,   407,   407,    57,   407,   407,   407,
     407,    57,   407,    57,   407,   232,    33,    33,    33,    33,
      57,   407,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   407,   426,    33,
      33,   148,   426,   109,   426,   426,   109,   426,   426,   426,
     426,   109,   426,   109,   426,    33,   426,   426,   426,   406,
     109,   426,   172,   171,   171,   173,    79,   174,   173,   172,
     293,   285,   285,   318,   171,   173,    79,   172,   172,   174,
     382,   269,   174,   269,   269,   426,   407,   426,   426,    33,
     407,   426,   407,   407,   426,   407,   407,   407,   407,   426,
     407,   426,   407,   406,   223,   223,   223,   426,   426,   407,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   407,   408,   426,   151,   426,   426,   426,
     426,   210,   426,   426,   426,   426,   210,   426,   426,   426,
      33,    33,    33,   407,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   106,
     407,    33,   149,   408,   407,   407,    33,   407,   407,   407,
     407,    33,   407,   407,   407,   426,   426,   426,   426,   407,
     173,   172,   172,   174,   171,   174,   173,   294,   277,   277,
     319,   172,   174,   171,   173,   173,   383,   277,   277,   277,
     408,   157,   158,   426,   426,    33,   110,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   269,   269,
     269,   407,   426,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   106,   406,   426,   408,   426,    57,   407,
     407,   211,   407,   407,   407,   407,   211,   407,   407,   407,
      33,    33,    33,   426,   407,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   157,   158,   426,   150,   109,
     426,   426,    33,   426,   426,   426,   426,    33,   426,   426,
     426,   426,   426,   426,   408,   426,   174,   173,   173,   172,
     174,   295,    79,   172,   320,   173,   172,   174,   174,   384,
     347,   347,   348,   406,   426,   407,    33,   426,   407,   407,
     426,   407,   407,   407,   407,   426,   407,   407,   408,   232,
     232,   232,   407,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     426,   426,   426,   426,   426,   212,   426,   426,   426,   426,
     212,   426,   426,   233,    33,    33,    33,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   407,   407,   151,   407,   407,
      33,   407,   407,   407,   407,    33,   407,   407,    33,   406,
     406,   406,   407,   174,   174,   173,   296,   171,   173,   321,
     174,   173,   349,   426,   426,    33,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   406,   426,   426,   426,
     426,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,   407,   407,   426,   407,   407,   213,   407,   407,   407,
     407,   213,   407,   407,   426,   407,   407,   407,   407,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,    33,   426,   426,   426,   426,    33,   426,
     426,   407,   426,   426,   426,   426,   174,   297,   172,   174,
     322,   174,   351,   407,   407,   407,   407,   426,   407,   407,
     407,   407,   426,   407,   407,   426,   407,   407,   407,   407,
      33,    33,    33,    33,    33,    33,    33,   426,   426,   426,
     426,   214,   426,   426,   426,   426,   214,   426,   426,   407,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   407,   407,   407,   407,    33,   407,   407,   407,   407,
      33,   407,   407,   426,   408,   408,   408,   407,   298,   173,
     323,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   408,   233,   233,   233,   426,    33,    33,
      33,   407,   407,   407,   407,   215,   407,   407,   407,   407,
     215,   407,   407,   234,    33,    33,    33,   407,   426,   426,
     426,   426,   426,   426,   426,    33,   426,   426,   426,   426,
      33,   426,   426,    33,   406,   406,   406,   426,   299,   174,
     324,   407,   407,   407,   407,   426,   407,   407,   407,   407,
     426,   407,   407,   406,   426,   426,   426,   407,    33,    33,
      33,   426,   426,   426,   426,   216,   426,   426,   426,   426,
     216,   426,   426,   426,   407,   407,   407,   426,   426,   426,
     426,   407,   408,   407,   407,    33,   408,   407,   408,   408,
      33,   407,   407,   407,   426,   426,   426,   407,   300,   325,
     426,   426,   426,   426,   110,   426,   110,   110,   426,   426,
     426,   426,   407,   407,   407,   426,    33,    33,   407,   407,
     407,   217,    57,   407,    57,    57,   217,   407,   407,   407,
     426,   426,   426,   407,   426,   426,   426,   426,   426,    33,
     109,   426,   109,   109,    33,   426,   426,   426,   408,   408,
     408,   426,   301,   326,   408,   407,   407,   426,   426,   407,
     426,   426,   426,   407,   407,   408,   234,   234,   234,   407,
      33,    33,   426,   426,   218,   210,   426,   218,   426,   426,
      33,    33,    33,   426,   426,   426,   407,   407,    33,    33,
     408,    33,   407,   407,   406,   406,   406,   407,   302,   327,
     426,   426,   426,   426,   110,   426,   426,   426,   426,   426,
     426,   426,    33,    33,   407,   407,   211,    57,   407,   407,
     407,   407,   407,   407,   426,   426,   426,   426,    33,   109,
     426,   426,   426,   426,   426,   426,   303,   328,   407,   407,
     426,   426,   407,   407,   407,   407,   407,   407,    33,    33,
     426,   426,   212,   426,   426,   426,   426,   426,   426,   426,
     426,   407,   407,    33,   407,   407,   408,   408,   408,   407,
     304,   329,   426,   426,   426,   426,   426,   426,    33,    33,
     407,   408,   213,   407,   408,   408,   426,   426,   426,   110,
      33,   426,   110,   110,   305,   277,   407,    57,   426,   407,
      57,    57,    33,    33,   426,   109,   214,   426,   109,   109,
     426,   426,   407,   426,    33,   407,   426,   426,   306,    79,
     426,   426,   426,    33,    33,   408,   215,   408,   426,   426,
     110,    33,   110,   307,   171,    57,   426,    57,    33,    33,
     109,   216,   109,   426,   426,   426,    33,   426,   308,   172,
     426,    33,    33,   217,   426,   426,    33,   309,   173,   426,
      33,    33,   218,   426,   426,    33,   310,   174,   426,    33,
      33,   426,   426,    79,    33,   426,   171,    33,   426,   172,
      33,   426,   173,    33,   426,   174,    33,   426
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   415,   416,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     418,   418,   418,   418,   419,   419,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   422,   422,   422,   422,   422,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   425,   425,   425,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   427,   427,
     427,   427,   427,   427,   427,   427,   428,   428,   429,   429
  };

  const unsigned char
  feiparser::yyr2_[] =
  {
       0,     2,     2,     2,     4,     4,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    15,     8,     2,
      13,     3,     5,    14,    14,    24,    32,    32,    48,    34,
      52,    13,    16,    19,    10,    10,    31,    28,    16,    19,
      22,     8,     8,    15,    24,    15,    38,     8,    13,    18,
       6,     6,    14,     6,     1,     2,     2,     2,     3,     4,
       6,     3,     5,     3,    12,     3,    10,     7,    11,     3,
       4,     6,     9,    18,     7,    22,    20,    20,    21,    20,
       3,     4,     4,     4,     4,     6,    14,    35,    29,    29,
      38,    32,    41,    38,    35,    32,    95,    47,    41,    80,
      38,    32,    41,    35,    35,    29,    23,    11,    26,    35,
      26,    35,    35,    14,    41,    27,    27,    18,    27,    33,
      65,    65,    71,    65,    71,    51,    57,    51,    54,    54,
      78,    59,    59,    51,    14,    16,    59,    32,    22,    22,
      20,    22,    21,     1,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     3,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     1,     2,     2,
       3,     5,     3,     1,     1,     2,     2,     3,     1,     2
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
  "CONSTRAINT", "DRM", "SECTION", "LOADPATTERN", "PENALTYDISPLACEMENT",
  "LOADVALUE", "SET", "REACTION", "FORCES", "ELEMENTNAME", "MATERIALNAME",
  "ACCELERATION_FIELD", "FIX", "FREE", "REMOVE", "DEFINE", "ALGORITHM",
  "ALGNAME", "CONVERGENCE_TEST", "TESTNAME", "SOLVER", "SOLVERNAME",
  "DYNAMICINTEGRATOR", "DYNAMICINTEGRATOR_HHT",
  "DYNAMICINTEGRATOR_NEWMARK", "STATICINTEGRATOR",
  "STATICINTEGRATOR_DISPLACEMENT", "SIMULATE", "COMPUTE", "STATIC",
  "DYNAMIC", "USING", "TRANSIENT", "EIGEN", "time_step", "number_of_modes",
  "VARIABLETRANSIENT", "maximum_time_step", "minimum_time_step",
  "number_of_iterations", "AT", "ALL", "AND", "WITH", "TEXTDOFS", "NEW",
  "TEXTNUMBER", "USE", "TO", "DOF", "TEXTWITH", "NODES", "FORCE",
  "INTEGRATIONPOINTS", "dof", "RESPONSE", "FILE", "FROM", "EVERY", "LEVEL",
  "LOADING", "STAGE", "STEPS", "TYPE", "DOFS", "FACTOR", "INCREMENT",
  "TH_GROUNDMOTION", "TH_LINEAR", "TH_PATH_SERIES", "TH_PATH_TIME_SERIES",
  "TH_CONSTANT", "TH_FROM_REACTIONS", "self_weight", "surface",
  "load_value", "scale_factor", "displacement_scale_unit",
  "velocity_scale_unit", "acceleration_scale_unit", "number_of_steps",
  "number_of_boundary_nodes", "number_of_exterior_nodes",
  "number_of_drm_elements", "element_file", "boundary_nodes_file",
  "exterior_nodes_file", "displacement_file", "acceleration_file",
  "velocity_file", "force_file", "hdf5_file", "series_file",
  "time_series_file", "MAGNITUDES", "MAGNITUDE", "strain_increment_size",
  "maximum_strain", "number_of_times_reaching_maximum_strain",
  "constitutive", "testing", "constant", "mean", "triaxial", "drained",
  "undrained", "simple", "shear", "number_of_subincrements",
  "maximum_number_of_iterations", "tolerance_1", "tolerance_2", "strain",
  "stress", "control", "Guass", "points", "Gauss", "each", "point",
  "single", "value", "EightNodeBrick", "TwentySevenNodeBrick",
  "EightNodeBrick_upU", "TwentyNodeBrick_uPU", "TwentyNodeBrick",
  "TwentyNodeBrickElastic", "EightNodeBrick_up",
  "variable_node_brick_8_to_27", "EightNodeBrickElastic",
  "TwentySevenNodeBrickElastic", "beam_displacement_based", "beam_elastic",
  "beam_elastic_lumped_mass", "penalty",
  "penalty_for_applying_generalized_displacement", "beam_9dof_elastic",
  "FourNodeShellMITC4", "FourNodeShellNewMITC4", "ThreeNodeShellANDES",
  "FourNodeShellANDES", "truss", "contact", "EightNodeBrickLT",
  "TwentySevenNodeBrickLT", "ShearBeamLT", "porosity", "alpha", "rho_s",
  "rho_f", "k_x", "k_y", "k_z", "K_s", "K_f", "pressure", "cross_section",
  "shear_modulus", "torsion_Jx", "bending_Iz", "bending_Iy",
  "IntegrationRule", "number_of_integration_points", "stiffness",
  "normal_stiffness", "tangential_stiffness", "friction_ratio",
  "maximum_gap", "xz_plane_vector", "joint_1_offset", "joint_2_offset",
  "direction", "contact_plane_vector", "MembranePlateFiber",
  "ElasticMembranePlate", "thickness", "linear_elastic_isotropic_3d",
  "vonmises_perfectly_plastic", "vonmises_isotropic_hardening",
  "vonmises_linear_kinematic_hardening",
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
  "beta_min", "n_in", "a_in", "elastic_modulus_1atm", "eplcum_cr_in", "ax",
  "ay", "az", "verbose_level", "maximum_iterations", "tolerance", "beta",
  "gamma", "kappa", "lambda", "delta", "DOMAIN_", "startTime", "endTime",
  "Period", "Phase", "Amplitude", "frequency", "MaxTime", "frequency1",
  "frequency2", "frequency3", "frequency4", "stiffness_to_use_opt",
  "DAMPING_RAYLEIGH", "DAMPING_CAUGHEY3", "DAMPING_CAUGHEY4", "a0", "a1",
  "stiffness_to_use", "a2", "a3", "a4", "R0", "cR1", "cR2", "MASS", "mx",
  "my", "mz", "Imx", "Imy", "Imz", "equaldof", "master", "slave",
  "dof_to_constrain", "of", "OUTPUT", "BINARY", "TEXT", "ENABLE",
  "DISABLE", "COMPRESSION", "'('", "','", "')'", "'['", "']'", "'.'",
  "';'", "'{'", "'}'", "$accept", "dsl", "CMD_add", "CMD_fix", "dofchain",
  "CMD_define", "CMD_misc", "CMD_remove", "ADD_material", "ADD_element",
  "prompt", "exp", "stmt", "cpd_stmt", "list", YY_NULLPTR
  };


  const unsigned short int
  feiparser::yyrline_[] =
  {
       0,   269,   269,   283,   293,   316,   329,   340,   428,   436,
     442,   443,   444,   445,   446,   450,   460,   464,   505,   524,
     528,   541,   545,   565,   583,   601,   627,   656,   686,   723,
     754,   794,   814,   847,   883,   898,   913,   965,  1088,  1106,
    1125,  1144,  1160,  1176,  1194,  1215,  1252,  1293,  1310,  1326,
    1356,  1396,  1410,  1432,  1461,  1465,  1474,  1483,  1490,  1497,
    1505,  1515,  1525,  1538,  1586,  1623,  1652,  1666,  1679,  1706,
    1744,  1772,  1785,  1801,  1824,  1838,  1862,  1886,  1910,  1934,
    1959,  1975,  1988,  2001,  2015,  2027,  2048,  2066,  2100,  2129,
    2158,  2192,  2223,  2260,  2295,  2328,  2359,  2442,  2489,  2530,
    2604,  2644,  2679,  2722,  2761,  2799,  2832,  2857,  2872,  2901,
    2936,  2965,  3003,  3041,  3060,  3111,  3137,  3162,  3181,  3206,
    3233,  3280,  3327,  3376,  3423,  3474,  3514,  3556,  3596,  3643,
    3681,  3735,  3799,  3863,  3916,  3940,  3966,  4018,  4053,  4079,
    4105,  4129,  4154,  4346,  4388,  4430,  4445,  4490,  4497,  4504,
    4511,  4518,  4525,  4532,  4538,  4545,  4553,  4561,  4569,  4577,
    4585,  4589,  4595,  4600,  4606,  4612,  4618,  4624,  4630,  4638,
    4644,  4649,  4654,  4659,  4664,  4669,  4677,  4708,  4713,  4717,
    4726,  4748,  4773,  4793,  4811,  4822,  4832,  4838,  4846,  4850
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
     406,   408,    36,    34,   407,    35,   411,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   412,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   409,     2,   410,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   413,     2,   414,     2,     2,     2,     2,
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
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405
    };
    const unsigned int user_token_number_max_ = 652;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 8092 "feiparser.tab.cc" // lalr1.cc:1155
#line 4878 "feiparser.yy" // lalr1.cc:1156


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

void set_profiling_results_filename(char* cstring)
{
    string filename(cstring);
    cout << "Setting timing report file to : " << filename << endl;
    profiling_results_filename = filename;
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



