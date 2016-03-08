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
#line 40 "feiparser.yy" // lalr1.cc:399

	// Basic
	#include <iostream>
	#include <iomanip>

	// STL
	#include <vector>
	#include <set>
	#include <string>
	#include <algorithm>
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
	#include "feivector.h"
	#include "dsl_actions.h"
	#include "siunit.h"
	#include "quantity.h"
	#include "unitdefinitions.h"
	#include "feidslcaller.h"
	#include "feimath.h"

/*     #include "command_suggestion.h" */

	// CompGeoMech API
	typedef std::vector<Number*> VectorNumber;          // This hack gets rid of a collision between std::vector and Vector classes used within FEI
	#include "../API/ESSI_API.h"

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
	Quantity q0(0.0, ESSIunits::unitless);

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


#line 137 "feiparser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "feiparser.tab.hh"

// User implementation prologue.
#line 278 "feiparser.yy" // lalr1.cc:407

extern int yylex(yy::feiparser::semantic_type *yylval,
				 yy::feiparser::location_type *yylloc);

#line 155 "feiparser.tab.cc" // lalr1.cc:407


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
#line 241 "feiparser.tab.cc" // lalr1.cc:474

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
    #line 283 "feiparser.yy" // lalr1.cc:725
{
	yyla.location.begin.filename = yyla.location.end.filename = new std::string(curfilename);
}

#line 544 "feiparser.tab.cc" // lalr1.cc:725

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
#line 304 "feiparser.yy" // lalr1.cc:847
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
#line 667 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 318 "feiparser.yy" // lalr1.cc:847
    {
		//theDomain.Print(cerr);
		cerr << "Not currently working. Should be implemented as API function and used here." << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 678 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 328 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "output_of_element_to_screen");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 692 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 351 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_node_to_screen, args, signature, "output_of_node_to_screen");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 706 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 364 "feiparser.yy" // lalr1.cc:847
    {
		cerr << "Not currently working. Should be implemented a API function and used here." << endl;
		//theDomain.getNDMaterial( $4->value().Getvalue() ) -> Print(cerr,0);
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 718 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 375 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("filename", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<string>(&check_mesh, args, signature,"check_mesh");

		nodes.pop();
		nodes.push((yylhs.value.exp));
		}
#line 732 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 388 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 396 "feiparser.yy" // lalr1.cc:847
    {
		cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 751 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 401 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 757 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 402 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 763 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 403 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 769 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 404 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 775 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 405 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 781 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 409 "feiparser.yy" // lalr1.cc:847
    {
									cout << "    *** Commands will now be execued. " << endl << endl;
									recovery_mode = 0;
									prompt = thePrompt;
									(yylhs.value.exp) = new Empty();
									nodes.push((yylhs.value.exp));
								}
#line 793 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 419 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 799 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 424 "feiparser.yy" // lalr1.cc:847
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
#line 841 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 461 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 847 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 466 "feiparser.yy" // lalr1.cc:847
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
#line 864 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 478 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 870 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 484 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 888 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 501 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[9].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_selfweight_to_all_elements, args, signature, "add_load_selfweight_to_all_elements");

		for(int ii = 1;ii <=2; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 904 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 517 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_node, args, signature, "add_load_selfweight_to_node");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 922 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 536 "feiparser.yy" // lalr1.cc:847
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
#line 947 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 562 "feiparser.yy" // lalr1.cc:847
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
#line 975 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 591 "feiparser.yy" // lalr1.cc:847
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
#line 1004 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 621 "feiparser.yy" // lalr1.cc:847
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
#line 1040 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 658 "feiparser.yy" // lalr1.cc:847
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
#line 1070 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 689 "feiparser.yy" // lalr1.cc:847
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
#line 1109 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 727 "feiparser.yy" // lalr1.cc:847
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
#line 1130 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 749 "feiparser.yy" // lalr1.cc:847
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
#line 1162 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 783 "feiparser.yy" // lalr1.cc:847
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
#line 1196 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 816 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[2].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 1212 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 831 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back(new Number(-1, ESSIunits::unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		nodes.pop(); //pop 1 exps
		nodes.push((yylhs.value.exp));
	}
#line 1228 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 853 "feiparser.yy" // lalr1.cc:847
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
#line 1274 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 904 "feiparser.yy" // lalr1.cc:847
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
#line 1318 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 951 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();


		args.push_back((yystack_[17].value.exp)); signature.push_back(this_signature("number",    &isAdimensional));


		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("time_step",    &isTime));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("scale_factor",    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("initial_velocity",    &isVelocity));


		//Add acceleration_file file.
		args.push_back( (yystack_[0].value.exp));                                          // Add to arguments of call signature
		signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature
		Expression* dof_number = dof2number(*(yystack_[12].value.ident));
		args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));


		//	add_time_history_of_ground_motion_for_uniform_excitation(
		//        int ExcitationNumber,
		//        double timestep,
		//        double scale,
		//        double vel0,
		//        string inputfilename_string,
		//        int direction)
		// Generate one command
	   (yylhs.value.exp) = new FeiDslCaller6<int, double,  double, double, string, int>(&add_time_history_of_ground_motion_for_uniform_excitation, args,signature, "add_time_history_of_ground_motion_for_uniform_excitation");


		for(int i = 1; i <= 5; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1357 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 989 "feiparser.yy" // lalr1.cc:847
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
#line 1375 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 1007 "feiparser.yy" // lalr1.cc:847
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
#line 1394 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 1026 "feiparser.yy" // lalr1.cc:847
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
#line 1414 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 1045 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_element(int elementNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("element_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1431 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 1061 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_node(int nodeNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("node_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1448 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 1077 "feiparser.yy" // lalr1.cc:847
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
#line 1467 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 1095 "feiparser.yy" // lalr1.cc:847
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
#line 1489 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 1116 "feiparser.yy" // lalr1.cc:847
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
#line 1527 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 1153 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("master",           &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("slave",           &isAdimensional));
		args.push_back(dof2number(*(yystack_[8].value.ident))); signature.push_back(this_signature("masterdof",           &isAdimensional));
		args.push_back(dof2number(*(yystack_[1].value.ident))); signature.push_back(this_signature("slavedof",           &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller4<int, int, int, int>(&add_equaldof_to_two_nodes_two_dofs, args, signature, "add_equaldof_to_two_nodes_two_dofs");
		
		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1545 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 1172 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,string>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1561 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 1190 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("scale_factor",            &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, string, double>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1578 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 1206 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("section_number",           &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material_number",           &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("thickness",           &isLength));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1595 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 1226 "feiparser.yy" // lalr1.cc:847
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
#line 1614 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 1250 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("section_number",          &isAdimensional));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("elastic_modulus",         &isPressure));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("cross_section",           &isArea));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("bending_Iz",           &isAreaMomentOfInertia));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("bending_Iy",           &isAreaMomentOfInertia));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("shear_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("torsion_Jx",           &isAreaMomentOfInertia));
		(yylhs.value.exp) = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_section_elastic_to_beam, args, signature, "add_section_elastic_to_beam");
		for(int i = 1; i <= 7; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1632 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 1270 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("section_number",          &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maxNumPatches",         &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("maxNumReinfLayers",           &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_section_to_model, args, signature, "add_section_to_model");
		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1647 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 1293 "feiparser.yy" // lalr1.cc:847
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
#line 1688 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 1333 "feiparser.yy" // lalr1.cc:847
    {
		args.clear();           signature.clear();
		args.push_back((yystack_[2].value.exp));     signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1703 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 1347 "feiparser.yy" // lalr1.cc:847
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
#line 1726 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1369 "feiparser.yy" // lalr1.cc:847
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
#line 1755 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1397 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.clear();
							dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
						}
#line 1764 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1401 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
						}
#line 1772 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1406 "feiparser.yy" // lalr1.cc:847
    {
							(yylhs.value.exp) = new FeiVector();
							dynamic_cast<FeiVector*>((yylhs.value.exp))->add_component((yystack_[0].value.exp)->value());
							nodes.pop();
							nodes.push((yylhs.value.exp));
						}
#line 1783 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1412 "feiparser.yy" // lalr1.cc:847
    {
							dynamic_cast<FeiVector*>((yystack_[2].value.exp))->add_component((yystack_[0].value.exp)->value());
							(yylhs.value.exp) = (yystack_[2].value.exp);
							nodes.pop();
						}
#line 1793 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1424 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1803 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1433 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1813 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1440 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1823 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1447 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
		nodes.push((yylhs.value.exp));
	}
#line 1834 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 1455 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("level", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
		nodes.push((yylhs.value.exp));
	}
#line 1845 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 1465 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_element_output,args,signature,"disable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1855 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 1474 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("tag", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&disable_element_output,args,signature,"disable_element_output");
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1868 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 1487 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("factor", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1882 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 1503 "feiparser.yy" // lalr1.cc:847
    {

		string fname;       // name of the DSL called to report
		args.clear(); signature.clear();
		
		//Read the string and turn into lower-case
		string algname(*(yystack_[9].value.ident));
		//std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		int method = -1;
		bool good = false;

		if( algname.compare("Euler_One_Step") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Euler_One_Step;
			good = true;
		}
		if( algname.compare("Euler_Multistep") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Euler_Multistep;
			good = true;
		}
		if( algname.compare("Modified_Euler_Error_Control") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Modified_Euler_Error_Control;
			good = true;
		}
		if( algname.compare("Runge_Kutta_45_Error_Control") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Runge_Kutta_45_Error_Control;
			good = true;
		}
		if( algname.compare("Backward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler;
			good = true;
		}
		else
		{
			(yylhs.value.exp) = new Number(-1, ESSIunits::unitless);
		}

		if(good)
		{
			args.push_back(new Number(method, ESSIunits::unitless)); signature.push_back(this_signature("method", &isAdimensional));
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("yield_function_relative_tolerance", &isAdimensional));
			args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("stress_relative_tolerance", &isAdimensional));
			args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
			(yylhs.value.exp) = new FeiDslCaller4<int, double, double, int>(&define_NDMaterialLT_constitutive_integration_algorithm,args, signature, "define_NDMaterialLT_constitutive_integration_algorithm");
		}
		
		for(int i = 0; i < 3; i++)
			nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1942 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 1562 "feiparser.yy" // lalr1.cc:847
    {
		// This case is a bit different. ALGNAME contains a string to
		// indicate which algorithm to choose. Instead of having different
		// tokens for each algorithm, just have one and decide in the
		// semantic action which DSL to call.
		// Usually different DSLs have different arguments, but in this
		// case all three take no arguments, which makes this approach easier.

		int (*f)() = NULL;         // function poiner to the algorithm DSL
		string fname;       // name of the DSL called to report

		//Read the string and turn into lower-case
		string algname(*(yystack_[0].value.ident));
		std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		args.clear(); signature.clear();
		if( algname.compare("with_no_convergence_check") == 0)
		{
			f = &define_algorithm_with_no_convergence_check_for_analysis;
			fname = "define_algorithm_with_no_convergence_check_for_analysis";
		}
		else if( algname.compare("modified_newton") == 0)
		{
			f = &define_algorithm_modifiednewton_for_analysis;
			fname = "define_algorithm_modifiednewton_for_analysis";
		}
		else if( algname.compare("newton") == 0)
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
#line 1994 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 1613 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		int (*f)(double, int, int) = NULL;         // function poiner to the test DSL
		string fname;       // name of the DSL called to report

		args.clear(); signature.clear();
		if((yystack_[9].value.ident)->compare("Norm_Displacement_Increment") == 0)
		{
			f = &define_convergence_test_normdisplacementincrement_for_analysis;
			fname = "define_convergence_test_normdisplacementincrement_for_analysis";
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if((yystack_[9].value.ident)->compare("Energy_Increment") == 0)
		{
			f = &define_convergence_test_energyincrement_for_analysis;
			fname = "define_convergence_test_energyincrement_for_analysis";
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if((yystack_[9].value.ident)->compare("Norm_Unbalance") == 0)
		{
			f = &define_convergence_test_normunbalance_for_analysis;
			fname = "define_convergence_test_normunbalance_for_analysis";
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("verbose_level", &isAdimensional));

		if(f == NULL)
		{
			(yylhs.value.exp) = new Empty();
		}
		else
		{
			(yylhs.value.exp) = new FeiDslCaller3<double, int, int> (f, args, signature, fname);
		}
		for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 3 exps
		nodes.push((yylhs.value.exp));
	}
#line 2038 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 1656 "feiparser.yy" // lalr1.cc:847
    {
		int (*f)() = NULL;         // function poiner to the algorithm DSL
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

		if(f == NULL)
		{
			(yylhs.value.exp) = new Empty();
		}
		else
		{
			(yylhs.value.exp) = new FeiDslCaller0<>(f, args, signature, fname);
		}
		nodes.push((yylhs.value.exp));
	}
#line 2074 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 1691 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("gamma", &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 2089 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 1705 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

		nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 2103 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 1718 "feiparser.yy" // lalr1.cc:847
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
#line 2125 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 1745 "feiparser.yy" // lalr1.cc:847
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
#line 2164 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1783 "feiparser.yy" // lalr1.cc:847
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
#line 2193 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1811 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2207 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 1824 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("time_step", &isTime));
		args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

		nodes.pop();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2224 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 1844 "feiparser.yy" // lalr1.cc:847
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
#line 2244 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 1863 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back
		(this_signature("number_of_modes", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2259 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 1880 "feiparser.yy" // lalr1.cc:847
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
#line 2281 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 1904 "feiparser.yy" // lalr1.cc:847
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
#line 2303 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 1928 "feiparser.yy" // lalr1.cc:847
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
#line 2325 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 1952 "feiparser.yy" // lalr1.cc:847
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
#line 2347 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 1976 "feiparser.yy" // lalr1.cc:847
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
#line 2369 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 1998 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		(yylhs.value.exp) = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

	}
#line 2380 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2014 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2394 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2027 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2408 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2040 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

		//pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2423 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2054 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
			 //pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2436 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2066 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2449 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2090 "feiparser.yy" // lalr1.cc:847
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
#line 2465 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2111 "feiparser.yy" // lalr1.cc:847
    {

		//int add_constitutive_model_NDMaterialLT_vonmises_linear_hardening(int MaterialNumber,
        //double rho,
        //double E,
        //double nu,
        //double k,
        //double H_alpha,
        //double H_k)


		args.clear(); signature.clear();
		args.push_back((yystack_[20].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));  // 5
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7



		(yylhs.value.exp) = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_vonmises_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_vonmises_linear_hardening");
		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2496 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2148 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		(yylhs.value.exp) = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2518 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2176 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		(yylhs.value.exp) = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2540 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2205 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[26].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 6
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		(yylhs.value.exp) = new FeiDslCaller9<int, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick");
		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2563 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2257 "feiparser.yy" // lalr1.cc:847
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
#line 2617 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2335 "feiparser.yy" // lalr1.cc:847
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
#line 2667 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2390 "feiparser.yy" // lalr1.cc:847
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
#line 2687 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2409 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		(yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push((yylhs.value.exp));
	}
#line 2703 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2431 "feiparser.yy" // lalr1.cc:847
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
#line 2726 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2463 "feiparser.yy" // lalr1.cc:847
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
#line 2752 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2495 "feiparser.yy" // lalr1.cc:847
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
#line 2775 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2523 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));

		// change the argument order for API
		args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("material_number",            &isAdimensional));
		args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("section_number",            &isAdimensional));
		
		args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("fiber_cross_section",          &isArea));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("fiber_location_Y",   &isLength));
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("fiber_location_Z",   &isLength));

		(yylhs.value.exp) = new FeiDslCaller6<int, int, int, double, double, double>(&add_single_fiber_to_section, args, signature, "add_single_fiber_to_section");
		for(int ii = 1;ii <=6; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2796 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2551 "feiparser.yy" // lalr1.cc:847
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
#line 2813 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2579 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("number"                    , &isAdimensional));
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("elastic_modulus_1atm"      , &isPressure));
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("poisson_ratio"            , &isAdimensional));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("M_in"                     , &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("kd_in"                    , &isAdimensional));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("xi_in"                    , &isAdimensional));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("h_in"                     , &isAdimensional));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("m_in"                     , &isAdimensional));
		args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("mass_density"             , &isDensity));
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
#line 2844 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2621 "feiparser.yy" // lalr1.cc:847
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
#line 2868 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2647 "feiparser.yy" // lalr1.cc:847
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
#line 2891 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 2672 "feiparser.yy" // lalr1.cc:847
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

		(yylhs.value.exp) = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node_ltensor_no_output, args, signature, "add_element_brick_8node_ltensor_no_output");

		for(int ii = 1;ii <=10; ii++) nodes.pop(); //pop 10 exps
		nodes.push((yylhs.value.exp));
	}
#line 2914 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 2696 "feiparser.yy" // lalr1.cc:847
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
#line 2932 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 2716 "feiparser.yy" // lalr1.cc:847
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
#line 2955 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 2740 "feiparser.yy" // lalr1.cc:847
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
#line 2981 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 2769 "feiparser.yy" // lalr1.cc:847
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
#line 3025 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 2816 "feiparser.yy" // lalr1.cc:847
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
#line 3069 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 2863 "feiparser.yy" // lalr1.cc:847
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
#line 3114 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 2912 "feiparser.yy" // lalr1.cc:847
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
#line 3158 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 2959 "feiparser.yy" // lalr1.cc:847
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
#line 3205 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3010 "feiparser.yy" // lalr1.cc:847
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
#line 3242 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3050 "feiparser.yy" // lalr1.cc:847
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
#line 3280 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3092 "feiparser.yy" // lalr1.cc:847
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
#line 3317 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3140 "feiparser.yy" // lalr1.cc:847
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
#line 3353 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3180 "feiparser.yy" // lalr1.cc:847
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
#line 3387 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3219 "feiparser.yy" // lalr1.cc:847
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
#line 3436 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3278 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back((yystack_[37].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));

		args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number_of_integration_points", &isAdimensional));
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("section_number", &isAdimensional));
		
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
		
		(yylhs.value.exp) = new FeiDslCaller15<int, int, int, 
								int, int, 
								double, double, double, double,
								double, double, double, double, double, double>(&add_element_DispBeamColumn3d, args, signature, "add_element_DispBeamColumn3d");

		for(int ii = 1;ii <=15; ii++) nodes.pop(); 
		nodes.push((yylhs.value.exp));
	}
#line 3472 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3336 "feiparser.yy" // lalr1.cc:847
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
								double, double, double>(&add_element_beam_elastic, args, signature, "add_element_beam_elastic");

		for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop        19 exps
		nodes.push((yylhs.value.exp));
	}
#line 3513 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3387 "feiparser.yy" // lalr1.cc:847
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
								double, double, double>(&add_element_beam_elastic_lumped_mass, args, signature, "beam_elastic_lumped_mass");

		for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop 19 exps
		nodes.push((yylhs.value.exp));
	}
#line 3554 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3435 "feiparser.yy" // lalr1.cc:847
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
#line 3600 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3491 "feiparser.yy" // lalr1.cc:847
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
#line 3642 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3539 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number",                  &isAdimensional));

		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node1",                   &isAdimensional));
		args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node2",                  &isAdimensional));

		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("normal_stiffness",       &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("tangential_stiffness",   &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("normal_damping",       &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("tangential_damping",   &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("friction_ratio",         &isAdimensional));

		args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("x_local_1",              &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("x_local_2",              &isAdimensional));
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("x_local_3",              &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller11<int, int, int,
							   double, double, double, double, double,
							   double, double, double>(&add_element_frictional_penalty_contact, args, signature, "add_element_frictional_penalty_contact");

		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3673 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3572 "feiparser.yy" // lalr1.cc:847
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
#line 3697 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3598 "feiparser.yy" // lalr1.cc:847
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
#line 3721 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3624 "feiparser.yy" // lalr1.cc:847
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
#line 3743 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 3648 "feiparser.yy" // lalr1.cc:847
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
#line 3766 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 3674 "feiparser.yy" // lalr1.cc:847
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
#line 3786 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 3862 "feiparser.yy" // lalr1.cc:847
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
#line 3832 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 3904 "feiparser.yy" // lalr1.cc:847
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
#line 3878 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 3946 "feiparser.yy" // lalr1.cc:847
    {
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		(yylhs.value.errcode) = 0;
	}
#line 3894 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 3961 "feiparser.yy" // lalr1.cc:847
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
#line 3943 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 4006 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 3954 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4013 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 3965 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4020 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 3976 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4027 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 3987 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4034 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 3998 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4041 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4009 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4048 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.push ((yylhs.value.exp));
	}
#line 4019 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4054 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4030 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4061 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4042 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4069 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4054 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4077 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4066 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4085 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4078 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4093 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4090 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4101 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4098 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4105 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4108 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4111 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Number (*(yystack_[0].value.value));
		nodes.push ((yylhs.value.exp));
	}
#line 4117 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4116 "feiparser.yy" // lalr1.cc:847
    {

		(yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 4127 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4122 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4137 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4128 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

		nodes.push((yylhs.value.exp));
	}
#line 4147 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4134 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4157 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4140 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4167 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4146 "feiparser.yy" // lalr1.cc:847
    {
		string temp = *(yystack_[0].value.ident);
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		(yylhs.value.exp) = new FeiString(temp);
		nodes.push((yylhs.value.exp));
	}
#line 4179 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4154 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);//new FeiVector($3);
		//nodes.push($$);
	}
#line 4188 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4159 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push((yylhs.value.exp));
	}
#line 4198 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4165 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push((yylhs.value.exp));
	}
#line 4207 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4170 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push((yylhs.value.exp));
	}
#line 4216 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4175 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4225 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4180 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4234 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4185 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push((yylhs.value.exp));
	}
#line 4243 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4190 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push((yylhs.value.exp));
	}
#line 4252 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4198 "feiparser.yy" // lalr1.cc:847
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
#line 4284 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4229 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4293 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4234 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4301 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4238 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
		args.clear();
		signature.clear();
	}
#line 4311 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4247 "feiparser.yy" // lalr1.cc:847
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
#line 4334 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4269 "feiparser.yy" // lalr1.cc:847
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
#line 4360 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4294 "feiparser.yy" // lalr1.cc:847
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
#line 4384 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4314 "feiparser.yy" // lalr1.cc:847
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
#line 4406 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4332 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		(yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push((yylhs.value.exp));
	}
#line 4421 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4343 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4431 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4353 "feiparser.yy" // lalr1.cc:847
    {
		// empty statement generates an  empty expression
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4441 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4359 "feiparser.yy" // lalr1.cc:847
    {
		// Used to initialize a list of statements (compound statement)
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4450 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4367 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[0].value.exp);
	}
#line 4458 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4371 "feiparser.yy" // lalr1.cc:847
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
#line 4474 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4478 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -408;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
     189,  -408,  1445,  -408,  -408,  -407,  -397,  -363,   594,   594,
    -408,  -408,    54,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
     594,   594,    31,  -408,  -408,    13,    34,  -408,  5822,  -331,
    -320,    46,    65,    73,  2201,    66,    59,     5,    16,   -48,
     -47,   594,  -408,  -285,  -408,  -408,  -408,  -408,  -408,  2371,
     -20,  -408,   152,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   -33,   -33,  -408,   105,   105,   -58,    61,
      64,  -408,  8682,   594,   186,    82,    99,   110,   115,   118,
     126,   128,  -162,   127,   143,   148,    89,   172,   177,  -408,
     594,  -158,   180,   207,   209,   212,   221,   229,  -104,   223,
     262,   273,   272,    72,   267,   280,   199,   268,  8382,   305,
     257,   594,   -25,  -408,   -92,  -408,   145,  -408,  -408,  -408,
     594,   594,   594,   594,   594,   594,   594,  -408,  -408,  8682,
    8682,  8682,  8682,  8682,  8682,  8682,  -233,  8682,   211,   235,
    2427,   377,  -408,   594,   594,   594,  8682,  -408,   594,   594,
    -408,   594,   594,   594,   594,   594,   288,   276,   594,   594,
     326,   594,   354,    41,   283,   594,   594,   594,   594,   594,
     594,   356,   266,  -408,    45,  -408,   304,   306,   317,   329,
       4,   338,   330,  -408,   423,  8391,  -408,   594,  -408,  -408,
      67,    67,   161,   161,     1,   105,   395,   594,  -408,  -408,
    -408,  -408,  2483,    14,    23,  8682,  8682,  8682,  8587,  8331,
    8340,  8471,  8480,  8495,  8322,   -31,   594,  8354,  5925,   325,
    8642,   337,    29,   343,  8541,  8554,  8682,  8682,  8682,  8682,
     346,   594,   428,   247,    94,   425,   400,   312,   320,    32,
     380,    52,  -408,  -408,   995,  8682,  -408,  -408,  -408,    80,
    8565,  8520,   -26,   437,   390,   182,  -109,   386,   448,  8163,
     -18,   470,   453,   451,   594,   507,   594,     2,   397,   594,
    8682,   594,   479,   482,   406,   441,   318,   362,    84,   384,
     526,   492,   494,   495,  -408,   594,   -73,   -68,   473,   474,
     -11,   483,   484,   510,   497,   499,   500,   501,   517,   521,
     522,   523,   524,   534,   537,   539,   540,   545,   547,   548,
     552,   328,   340,   351,   359,   376,   322,   385,   365,   366,
     433,   434,   436,   438,   440,   601,   605,   664,   609,   610,
     614,   621,   613,   625,   626,   594,   624,   714,   485,   472,
     475,   498,   594,   635,   639,  5654,  -408,  8682,  -408,   397,
    -408,  -408,  8682,  5876,   594,   594,   594,   389,   584,   582,
     591,   592,   596,   594,  -408,   675,   677,   314,   594,   661,
     594,   662,   663,   668,   594,   669,   670,   674,   594,   678,
     681,   682,   683,   685,   686,   687,   689,   691,   692,   693,
     694,   695,   699,   703,   710,   711,   749,   797,   798,   799,
     805,   815,   816,   818,   819,   820,   821,   822,   823,   824,
     611,   594,   728,   594,   750,   594,   594,   486,   487,   488,
    8407,   594,   594,   839,   840,   841,   846,  5916,   594,   594,
     856,  -408,   857,  8682,  5836,  8429,   859,   715,   779,   781,
     782,   716,  8682,   865,   873,   594,  7838,   490,  7847,   493,
     496,   512,  7864,   513,   514,   515,  7874,   516,   519,   525,
     529,   530,   549,   556,   558,   570,   571,   573,   574,   575,
     576,   578,   583,   586,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   809,  8363,
    8373,   769,  8438,   795,  8682,  8682,   879,   894,   902,   883,
    8578,  8297,   594,   594,   594,   594,   905,   543,  8504,   594,
     594,   980,   898,   594,   834,   959,   960,   967,   911,   594,
     594,   407,   844,   594,   852,   594,   594,   594,   853,   594,
     594,   594,   858,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
    6945,  6954,  6294,  6305,  6585,  6089,  6931,  6963,  6974,  5943,
    6983,  6992,  7003,  7012,   900,    74,   111,   926,   933,   941,
     594,   594,   594,   643,   650,  1026,  8518,  6829,  7523,  7200,
     594,   951,  1002,  1570,  5894,   594,   940,  5867,   974,   961,
     976,   983,  1040,  8682,  8609,   594,   917,   450,   929,   531,
    1133,  1730,   931,  2015,  2051,  2098,   932,  2124,  2151,  2184,
    2202,  2236,  2509,  2529,  2545,  2554,  2569,  2579,  2588,  2597,
    2606,  2621,  2630,  2639,  1086,  1087,  1088,  1091,  1094,  1095,
    1096,  1105,  1106,  1108,  1110,  1111,  1112,  1117,  -408,  1034,
    1035,  1037,  -408,  1043,  1050,  1058,  1101,   -77,  1145,  5790,
    5799,  5808,  1070,  1127,   594,  1126,  1151,  1152,  1153,  5907,
    1123,  1089,  1176,  1183,  8682,   594,  1191,  1160,   594,   594,
     594,  1122,  1192,   252,  1000,   594,  1001,   594,   594,   594,
    1003,   594,   594,   594,  1004,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,  1203,  1210,  1213,  1179,
    1180,  1139,  1146,  1225,  1229,   594,  1231,  1232,  1233,  1215,
    1154,  8682,  1168,   594,   594,   594,  1249,  1102,   594,   594,
     594,  8682,   594,  1170,  8075,  8084,  8099,   594,   594,  1181,
    1182,  2648,  1197,  2657,  2666,  2675,  1202,  2684,  2693,  2702,
    1205,  2711,  2720,   293,   302,   336,   345,   358,  2729,  2738,
    2761,  2770,   387,   396,  2779,  2788,  2797,   418,  6845,  6854,
    6323,  6285,  6576,  6080,  8682,  6098,  6107,  6867,  6876,  6889,
    6904,  6913,   594,   594,   594,  1200,  1217,   872,   594,   594,
     594,  8313,   594,   594,   594,  1218,   594,   594,  7152,  7393,
    8682,   594,   906,  7220,  1406,  8682,  5885,   594,  1294,  1295,
    1300,  8108,  8619,   594,  1219,   594,  1220,   594,   594,   594,
    1221,   594,   594,   594,  1222,   594,   594,  1226,  1090,  1128,
    1129,  1130,   594,   594,   594,   594,  1235,  1119,   594,   594,
     594,  1132,  1319,  1341,  1342,  1350,  1351,  1353,  1354,  1355,
    1371,  1372,  1374,  1378,  1379,  8682,  8633,  8142,   594,   594,
     594,  8682,  8288,  8217,  1397,  5763,  5727,  5736,   594,  1535,
    8682,  1398,  1401,  8682,   594,  1402,  1403,  1405,  8117,   594,
     594,   594,  1414,  1419,  8563,  1022,  2819,  1036,  2828,  2837,
    2846,  1045,  2873,  2882,  2894,  1055,  2903,  2914,   594,  1421,
    1443,  1448,  1453,  2923,  2932,   509,  2941,  1301,  1455,  2954,
    2965,  2974,  1459,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,  1460,  1461,  8682,  8682,
    2983,  1462,  1463,   594,  1464,  1472,  1473,  8452,   397,   594,
     594,  8682,   594,   594,   594,  1476,  8030,  8039,  8048,   594,
     594,  -408,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,  8462,   594,   594,   594,   594,
     594,   594,  1396,   594,  1407,   594,   594,   594,   594,   594,
    8682,  8682,  6540,  6922,  6619,  6071,  5817,  5676,  6039,  6820,
    6778,  6793,  6807,   594,  1514,   594,   594,   594,  8133,  1135,
     594,   594,  1413,  -408,  7057,  7347,  7209,   261,  8682,   594,
    1497,  1499,  1500,  8057,  8600,  2992,  3007,  3016,  3025,  3034,
    3043,  3052,  3061,  3070,  3079,  3088,  3097,  3106,  1408,  7136,
    7021,  7030,  7043,   518,   707,  1488,   723,   594,  7322,  3115,
    3124,  3133,  8527,  1513,  1515,  1523,  1525,  1527,  1533,  1534,
    1542,  1543,  1546,  1549,  8151,  -408,  3147,  8231,  8269,  1551,
    -408,  5772,  5694,  1177,  1554,  1556,  1557,  1559,  8066,   594,
     594,   594,  1561,  1562,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,  1530,  1564,  1565,
    1566,  1567,  1503,  1508,  1498,  1510,  7535,  1595,   594,   594,
     594,  1572,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,  1597,   594,  1598,  1599,   594,  1608,  1609,
    -408,   594,   594,  1227,   594,  1610,  7965,  7974,  7999,   594,
     594,  3156,  3165,  3174,  3183,  3192,  3205,  3214,  3223,  3232,
    3241,  3250,  3259,  3268,  1538,   594,   594,   594,   594,  1603,
    1604,   594,  1605,  1619,   594,  3280,  3289,  3300,  1541,  6563,
    6314,  6610,   279,  5745,  5667,  6021,  6634,  6648,  6657,  6769,
    1651,  3309,   594,   594,  8190,  1270,   594,  8682,  7483,   594,
    5644,   594,  1650,  1652,  1653,  8008,  8682,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
     594,  7068,  7492,  7503,  7514,  1571,  1583,  7161,  1585,   594,
    7313,   594,   594,   594,   594,  1672,  1675,  1676,  1677,  1678,
    1679,  1680,  1681,  1682,  1687,  1689,  -408,   594,  8278,  8172,
    1690,  -408,  5781,  1691,  3318,  1699,  8017,   594,   594,   594,
    1703,  3327,  3339,  3351,  3360,  3369,  3378,  3393,  3402,  3411,
    3420,  3429,  3438,  3447,  7077,  1709,  1710,  1712,  1713,   594,
     594,  1715,   594,  7093,  1717,  3456,  3465,  3474,  8682,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
      87,  1718,  1721,   594,  1722,   594,   594,   594,  1723,  8682,
    8682,  8682,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,  1725,   594,   594,   594,
     594,  7170,  7179,   594,  7191,  1726,   594,   594,   594,   594,
    6554,  6270,  6595,  6057,  5709,  1970,  6001,  6667,  6676,  6690,
    6759,   594,  1660,   594,   594,  8682,  1377,  7443,   732,   270,
     594,  8682,  3483,  3492,  3501,  3510,  3519,  3533,  3542,  3551,
    3560,  3569,  3578,  3591,  3600,   594,  7238,  7455,  7464,  7473,
    1727,  1741,  8682,  1744,   594,  7301,  3609,  3618,  3627,  1749,
    1752,  1753,  1754,  1755,  1756,  1763,  1764,  1772,  1773,  1783,
    3636,   120,  8181,  8240,  -408,  1784,  -408,  1798,  8682,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,  7338,  1799,  1803,  1805,  1806,   594,   594,   594,
    8682,  1807,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,  1381,   594,  1815,
    1816,   594,   594,  3645,   808,  3654,  3666,   828,  3675,  3686,
    3695,  3704,   849,  3713,   866,  3724,  1818,  1435,   594,   594,
     594,  8682,  8682,  8682,   594,   947,   970,  3737,  6525,  6261,
    8682,  6048,   553,  5826,  5992,  8682,  6708,  6720,  6699,  3746,
     594,  8682,   594,   594,  8682,  8682,   594,  1742,   594,   594,
    1743,   594,   594,   594,   594,  1757,   594,  1760,   594,   594,
     594,  7360,  7375,  7384,  7288,  1761,  1766,   594,  1822,  1825,
    1833,  1848,  1849,  1850,  1852,  1853,  1855,   594,  3755,  8249,
    8199,  3764,  1832,  3779,  3788,  1834,  3797,  3806,  3815,  3824,
    1836,  3833,  1837,  3842,  7249,  3851,  1857,  1864,  1865,  1866,
    1843,  1844,  3860,   594,   594,   594,   594,   594,   594,   594,
     594,   594,  3869,   594,  1869,  1870,   594,  1800,   594,   594,
    1804,   594,   594,   594,   594,  1812,   594,  1819,   594,  1883,
     594,   594,   594,   594,   594,  1820,  1823,   594,  6516,  6246,
    8682,  8682,  5754,  5982,  8682,  8682,  6729,   594,  3878,   594,
     594,  3887,   594,  3896,  3905,   594,  3919,  3928,  3937,  3946,
     594,  3955,   594,  3964,  1518,  3977,  7402,  7413,  7428,  7229,
     594,   594,  3986,  1905,  1906,  1907,  1908,  1909,   131,   594,
    8208,  8682,   594,  8682,   594,   594,  7799,   594,   594,   594,
     594,  7811,   594,  8682,   594,   594,   594,  1910,  1912,  1925,
    1927,  8682,  8682,   594,   594,   594,   594,   594,   594,   594,
    1851,  3995,  1929,   984,  4004,  4013,  1937,  4022,  4031,  4040,
    4052,  1946,  4061,  4072,  4081,  1009,   594,   594,   594,  1563,
    4090,  6507,  6237,  5718,  6738,  8682,  1018,   149,   594,   594,
    1868,   594,   594,   594,   594,   594,   594,   594,   594,   594,
     594,   594,  1751,  7102,  7115,  7124,   594,   594,  1950,  1962,
    1965,  1967,  1891,  1594,   594,  1027,  8682,  1945,  4099,  4109,
    7771,  4123,  4132,  4141,  4150,  7781,  4165,  4174,  4183,  1980,
    1981,  1983,  1984,  4192,  4201,   594,   594,   594,   594,   162,
     594,  8682,  -408,  1911,   594,   594,  1985,   594,   594,   594,
     594,  1986,   594,   594,   594,  1606,   594,   594,   594,   594,
     594,  6496,  6227,  5685,  5961,  1618,   594,  4210,   594,  4219,
    4228,   594,  4237,  4246,  4255,  4264,   594,  4273,  4282,  1056,
     594,  7258,  7267,  7276,  4291,  4305,  1992,  2007,  2008,  2009,
     594,  8682,   594,  8682,   594,   594,  7736,   594,   594,   594,
     594,  7746,   594,   594,  1817,  4314,  2013,  2014,  2022,   594,
     594,   594,   594,   594,   594,  4323,  4332,  4341,  4350,  2025,
    4363,  4372,  4381,  4390,  2026,  4399,  4408,  2030,   594,  1647,
    1648,  1654,  1098,  4417,  6487,  6218,  8682,  5952,   594,   594,
     594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
    1656,  4426,   594,   594,   594,  -408,   594,  2034,  2046,  2047,
    4438,  4447,  4458,  4467,  7707,  4476,  4486,  4495,  4509,  7716,
    4518,  4527,   594,   594,  4536,  4551,  4560,  4569,   594,   594,
     594,   594,   594,   594,   594,  2048,   594,   594,   594,   594,
    2049,   594,   594,  4578,  1124,   594,   594,   594,   594,  6476,
    6209,  5934,  4587,  4596,  4605,  4614,   594,  4623,  4632,  4641,
    4650,   594,  4659,  4668,   594,  1854,  4677,  4691,  4700,  4709,
    2058,  2059,  2062,   594,   594,   594,   594,  7677,   594,   594,
     594,   594,  7686,   594,   594,  4718,  2063,   594,   594,   594,
     594,   594,   594,   594,  4727,  4736,  4749,  4758,  2064,  4767,
    4776,  4785,  4794,  2068,  4803,  4812,   594,  1693,  1169,  1184,
    1195,  4824,  6462,  6188,  8682,   594,   594,   594,   594,   594,
     594,   594,   594,   594,   594,   594,   594,  1214,   594,  1872,
    1873,  1877,   594,  2074,  2075,  4833,  4844,  4853,  4863,  7650,
    4872,  4881,  4895,  4904,  7659,  4913,  4922,  1878,  4937,  2084,
    2087,  2088,  4946,   594,   594,   594,   594,   594,   594,  2089,
     594,   594,   594,   594,  2090,   594,   594,  2091,   594,  1714,
    1729,  1734,   594,  6453,  6176,  4955,  4964,  4973,  4982,   594,
    4991,  5000,  5009,  5018,   594,  5027,  5036,  1735,  5045,   594,
     594,   594,  5054,  2094,  2105,   594,   594,   594,   594,  7609,
     594,   594,   594,   594,  7623,   594,   594,   594,   594,  5063,
    5077,  5086,   594,   594,   594,  5095,  1287,  5104,  5113,  2106,
    1308,  5122,  1325,  1334,  2110,  5135,  5144,  5153,  1343,   594,
     594,   594,  5162,  6444,  6159,   594,  -408,   594,   594,   594,
    2033,   594,  2039,  2040,   594,   594,   594,   594,  -408,  5171,
    5180,  5189,   594,  2111,  2131,  5198,  5210,  5219,  7581,  2108,
    5230,  2112,  2113,  7590,  5240,  5249,  5258,   594,   594,   594,
    5267,   594,   594,   594,   594,   594,  2134,  2055,   594,  2078,
    2079,  2138,   594,   594,   594,  1422,  1465,  1477,   594,  6429,
    6150,  1489,  5281,  5290,   594,   594,  5299,   594,   594,   594,
    5308,  5323,  1611,  1936,  1954,  1956,  5332,  2164,  2169,  -408,
     594,   594,  7544,  7825,   594,  8682,  8682,  7556,   594,   594,
    -408,  2170,  2177,  2178,   594,   594,   594,  5341,  5350,  2179,
    2180,  1629,  2183,  5359,  5368,  1826,  1828,  1829,  5377,  6420,
    6141,   594,   594,   594,   594,  2103,   594,   594,   594,   594,
     594,   594,   594,  2209,  2219,  5386,  5395,  8682,  7790,  2196,
    8682,  5404,  5413,  5422,  5431,  5440,  5449,   594,   594,   594,
     594,  2221,  2143,   594,   594,   594,   594,   594,   594,  6410,
    6132,  5463,  5472,   594,   594,  5481,  5490,  5499,  5508,  5521,
    5530,  2224,  2225,   594,   594,  7755,  8682,   594,   594,   594,
     594,   594,   594,   594,   594,  5539,  5548,  2227,  5557,  5566,
    1655,  1665,  1765,  5575,  6395,  6123,   594,   594,   594,   594,
     594,  -408,  -408,  -408,   594,  2228,  2229,  5584,  1776,  7727,
    5596,  1808,  1892,   594,   594,   594,  2149,  2234,   594,  2154,
    2163,  6386,  6749,  5605,  2222,   594,  5615,  2223,  2226,  2254,
    2259,   594,  2186,  7696,   594,  2189,  2190,   594,   594,  5626,
     594,  2263,  5635,   594,   594,  6374,  8661,   594,  8682,   594,
     594,  8682,  8682,  2264,  2271,  1930,  7668,  1939,   594,   594,
    2191,  2273,  2193,  6365,  7940,  2251,   594,  2255,  2280,  2281,
    2203,  7637,  2205,   594,   594,   594,  2282,   594,  6350,  7922,
    8682,   594,  8682,  2286,  2287,  7600,   594,   594,  2288,  6341,
    7902,   594,  2289,  2290,  7572,   594,   594,  2294,  6332,  7884,
     594,  2296,  2297,  8682,   594,   594,  8673,  8682,  2298,   594,
    7954,  2300,   594,  7931,  2301,   594,  7912,  2302,   594,  7893,
    2303,   594,  8682
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   138,   156,   155,   161,     0,     0,     0,     0,     0,
      15,   177,     0,   163,   164,   165,   166,   167,   168,   169,
       0,     0,     0,     9,     8,     0,     0,   178,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,    10,    12,    13,    11,    14,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   179,   154,   146,     0,     0,
       0,     3,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,    62,     0,   173,     1,   137,
       0,     0,     0,     0,     0,     0,     0,   172,   158,   147,
     148,   149,   150,   151,   152,   157,     0,    59,     0,     0,
       0,   174,   176,     0,     0,     0,     7,    76,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    70,     0,    72,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,    63,     0,    66,   153,
     140,   141,   142,   143,   144,   145,   139,     0,   162,   160,
     159,   180,   182,     0,     0,     5,     4,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    88,    91,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    64,     0,    60,   183,   181,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,    65,    54,    57,
      53,    56,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    41,     0,     0,     0,     0,
       0,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,    71,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      49,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,     0,     0,     0,    43,    69,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    32,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      86,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,     0,   105,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,   132,   134,     0,     0,     0,     0,     0,     0,
     100,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,    24,     0,     0,    51,    44,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   102,     0,     0,    95,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,   109,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    26,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,     0,     0,
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
       0,     0,     0,     0,   107,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,     0,     0,     0,     0,   119,   121,     0,     0,     0,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   122,     0,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,   127,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,   117,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,     0,   118,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -408,  -408,  -408,  -408,  -240,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,    -8,    42,   -32,  2139
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    43,    44,    45,   350,   136,    46,    47,    48,    89,
     150,    49,    50,   202,   141,   203
  };

  const short int
  feiparser::yytable_[] =
  {
      63,    64,   120,   121,   122,   123,   124,   125,   112,   114,
     126,    60,    66,    67,    72,   120,   121,   122,   123,   124,
     125,    61,   187,   126,   257,   723,   368,   108,   351,   325,
     326,   370,   142,   116,     2,     3,     4,     5,   369,     6,
       7,   125,    51,   371,   126,   129,   130,   131,   132,   133,
     134,   135,   137,   138,   139,    62,   143,    65,    13,    14,
      15,    16,    17,    18,    19,   146,    73,    20,   724,     2,
       3,     4,     5,    21,     6,     7,   120,   121,   122,   123,
     124,   125,   163,   327,   126,    74,    68,    69,   374,    70,
      90,   119,    91,    13,    14,    15,    16,    17,    18,    19,
     375,    92,    20,   185,   122,   123,   124,   125,    21,   431,
     126,   348,   190,   191,   192,   193,   194,   195,   196,   349,
      93,    29,   120,   121,   122,   123,   124,   125,    94,    95,
     126,   109,    96,   110,    97,   205,   206,   207,    98,   117,
     208,   209,   111,   210,   211,   212,   213,   214,   126,   281,
     217,   218,   282,   220,   106,   128,   283,   224,   225,   226,
     227,   228,   229,   176,   177,   107,   120,   121,   122,   123,
     124,   125,   248,   237,   126,   144,   238,   239,   145,   244,
     120,   121,   122,   123,   124,   125,   197,   198,   126,   245,
       1,   147,     2,     3,     4,     5,   148,     6,     7,     8,
     124,   125,     9,   278,   126,    10,   279,    11,   259,    12,
     639,   640,   641,   149,   642,   643,    13,    14,    15,    16,
      17,    18,    19,   270,   151,    20,   338,   339,   340,   152,
     341,    21,   153,    22,    23,    24,    25,   330,   331,    26,
     154,    27,   155,    28,    29,   156,   120,   121,   122,   123,
     124,   125,   644,   645,   126,   157,   345,   158,   347,   164,
      30,   352,   159,   353,   360,   361,    31,    32,    33,    34,
     120,   121,   122,   123,   124,   125,   160,   367,   126,   332,
     333,   334,  1437,  1438,    35,    36,   161,   120,   121,   122,
     123,   124,   125,   162,   165,   126,   120,   121,   122,   123,
     124,   125,    37,   171,   126,   120,   121,   122,   123,   124,
     125,  1703,  1704,   126,   120,   121,   122,   123,   124,   125,
     188,   166,   126,   167,  1755,  1756,   168,   420,   120,   121,
     122,   123,   124,   125,   427,   169,   126,   120,   121,   122,
     123,   124,   125,   170,   173,   126,   433,   434,   435,   120,
     121,   122,   123,   124,   125,   442,   172,   126,   174,   175,
     446,   178,   448,    90,   113,   115,   452,   180,   179,   181,
     456,   120,   121,   122,   123,   124,   125,   258,   183,   126,
     120,   121,   122,   123,   124,   125,   184,   186,   126,   204,
     216,   219,   140,   120,   121,   122,   123,   124,   125,   215,
     231,   126,   489,   490,   127,   492,    71,   494,   495,   221,
     223,   230,   232,   500,   501,   233,   235,   234,   236,   240,
     507,   508,   120,   121,   122,   123,   124,   125,   242,   241,
     126,   120,   121,   122,   123,   124,   125,   521,    -1,   126,
     247,   262,   120,   121,   122,   123,   124,   125,   140,    41,
     126,   264,   265,   120,   121,   122,   123,   124,   125,   266,
     269,   126,   271,   222,   272,   273,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     274,   275,   276,   280,    41,   120,   121,   122,   123,   124,
     125,   277,   328,   126,   576,   577,   578,   579,   285,   329,
     335,   583,   584,   336,   342,   587,  1341,  1342,   343,   344,
     346,   593,   594,   354,   349,   597,   355,   599,   600,   601,
     356,   603,   604,   605,   357,   607,   608,   609,   610,   611,
     612,   613,   614,   615,   616,   617,   618,   619,   620,   621,
     622,   623,   359,   358,   120,   121,   122,   123,   124,   125,
    1649,  1650,   126,   120,   121,   122,   123,   124,   125,   362,
     363,   126,   649,   650,   651,   189,   120,   121,   122,   123,
     124,   125,   659,   364,   126,   365,   366,   664,   120,   121,
     122,   123,   124,   125,   372,   373,   126,   673,   120,   121,
     122,   123,   124,   125,   376,   377,   126,     2,     3,     4,
       5,    38,     6,     7,    39,    40,   396,    41,   379,   378,
     380,   381,   382,    42,     2,     3,     4,     5,   397,     6,
       7,    13,    14,    15,    16,    17,    18,    19,   383,   398,
      20,   199,   384,   385,   386,   387,    21,   399,    13,    14,
      15,    16,    17,    18,    19,   388,   731,    20,   389,    29,
     390,   391,  1228,    21,   400,   200,   392,   741,   393,   394,
     744,   745,   746,   395,   402,  1077,    29,   751,   401,   753,
     754,   755,   749,   757,   758,   759,  1397,   761,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   789,   790,   791,  1013,   403,
     404,   405,   406,   837,   407,   410,   408,   801,   409,   411,
     488,   412,   838,   413,   417,   808,   809,   810,   415,   414,
     813,   814,   815,   445,   816,   416,   418,   419,   421,   821,
     822,   423,   120,   121,   122,   123,   124,   125,   422,   428,
     126,   424,   426,   429,   425,   437,   839,   436,   120,   121,
     122,   123,   124,   125,   438,   840,   126,   120,   121,   122,
     123,   124,   125,   439,   440,   126,   441,   443,   841,   444,
     447,   449,   450,   474,   865,   866,   867,   451,   453,   454,
     871,   872,   873,   455,   875,   876,   877,   457,   879,   880,
     458,   459,   460,   883,   461,   462,   463,   846,   464,   888,
     465,   466,   467,   468,   469,   894,   847,   896,   470,   898,
     899,   900,   471,   902,   903,   904,   595,   906,   907,   472,
     473,   475,   476,   477,   913,   914,   915,   916,   851,   478,
     919,   920,   921,   120,   121,   122,   123,   124,   125,   479,
     480,   126,   481,   482,   483,   484,   485,   486,   487,   491,
     938,   939,   940,   120,   121,   122,   123,   124,   125,   675,
     947,   126,   493,   502,   503,   504,   951,   496,   497,   498,
     505,   956,   957,   958,   120,   121,   122,   123,   124,   125,
     509,   510,   126,   513,   515,   514,   516,   517,   518,   519,
     975,   120,   121,   122,   123,   124,   125,   520,   523,   126,
     567,   525,   569,   570,   526,   990,   991,   992,   993,   994,
     995,   996,   997,   998,   999,  1000,  1001,  1002,   571,   982,
     527,   529,   530,   531,   533,  1008,   572,   534,  1102,   580,
     564,  1014,  1015,   535,  1016,  1017,  1018,   536,   537,  1511,
     677,  1023,  1024,   581,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1035,  1036,  1037,   538,  1039,  1040,
    1041,  1042,  1043,  1044,   539,  1046,   540,  1048,  1049,  1050,
    1051,  1052,   120,   121,   122,   123,   124,   125,   541,   542,
     126,   543,   544,   545,   546,  1064,   547,  1066,  1067,  1068,
     573,   548,  1071,  1072,   549,   120,   121,   122,   123,   124,
     125,  1078,    41,   126,   585,   586,   588,   589,   590,   120,
     121,   122,   123,   124,   125,   591,   592,   126,   596,    41,
     120,   121,   122,   123,   124,   125,   598,   602,   126,  1106,
     638,   646,   606,   648,   120,   121,   122,   123,   124,   125,
     647,   652,   126,   120,   121,   122,   123,   124,   125,   653,
     654,   126,   120,   121,   122,   123,   124,   125,   660,   661,
     126,  1136,  1137,  1138,   665,   668,  1141,  1142,  1143,  1144,
    1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,   667,
     669,   120,   121,   122,   123,   124,   125,   670,   671,   126,
    1165,  1166,  1167,   674,  1169,  1170,  1171,  1172,  1173,  1174,
    1175,  1176,  1177,  1178,  1179,   676,  1181,   680,   684,  1184,
     702,   703,   704,  1187,  1188,   705,  1190,  1103,   706,   707,
     708,  1195,  1196,   120,   121,   122,   123,   124,   125,   709,
     710,   126,   711,  1105,   712,   713,   714,  1211,  1212,  1213,
    1214,   715,  1396,  1217,   716,   717,  1220,   718,   719,   120,
     121,   122,   123,   124,   125,   720,   721,   126,   120,   121,
     122,   123,   124,   125,  1238,  1239,   126,   722,  1242,   725,
     729,  1244,   730,  1246,   732,   733,   734,   735,   737,  1251,
    1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,
    1262,  1263,  1264,   738,   120,   121,   122,   123,   124,   125,
     739,  1273,   126,  1275,  1276,  1277,  1278,   740,   743,   120,
     121,   122,   123,   124,   125,   742,   748,   126,  1487,  1290,
     120,   121,   122,   123,   124,   125,   747,   792,   126,  1299,
    1300,  1301,   750,   752,   793,   756,   760,   794,  1490,   120,
     121,   122,   123,   124,   125,   795,   796,   126,   797,   799,
     798,  1321,  1322,   800,  1324,   802,   803,   804,   806,  1495,
     805,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,   807,   811,   817,  1345,  1497,  1347,  1348,  1349,
     870,   812,   823,   824,  1351,  1352,  1353,  1354,  1355,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,   826,  1366,
    1367,  1368,  1369,   830,   868,  1372,   834,   884,  1375,  1376,
    1377,  1378,   120,   121,   122,   123,   124,   125,   889,   890,
     126,   869,   878,  1390,   891,  1392,  1393,   908,   895,   897,
     901,   905,  1398,   120,   121,   122,   123,   124,   125,   909,
     917,   126,   918,   923,   910,   911,   912,  1412,   922,   984,
     120,   121,   122,   123,   124,   125,  1420,  1505,   126,   120,
     121,   122,   123,   124,   125,   924,   925,   126,   120,   121,
     122,   123,   124,   125,   926,   927,   126,   928,   929,   930,
    1506,  1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1454,  1455,  1680,   931,   932,   284,   933,  1461,
    1462,  1463,   934,   935,  1465,  1466,  1467,  1468,  1469,  1470,
    1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,  1692,
    1481,   943,   949,  1484,  1485,   950,   952,   953,  1702,   954,
     962,   120,   121,   122,   123,   124,   125,  1732,   959,   126,
    1501,  1502,  1503,   960,   964,   976,  1504,   120,   121,   122,
     123,   124,   125,   968,    52,   126,    53,    54,    55,    56,
      57,    58,  1518,   972,  1519,  1520,  1794,   977,  1521,    59,
    1523,  1524,   978,  1526,  1527,  1528,  1529,   979,  1531,   985,
    1533,  1534,  1535,   989,  1003,  1004,  1006,  1007,  1009,  1542,
     120,   121,   122,   123,   124,   125,  1010,  1011,   126,  1552,
    1019,  1045,   120,   121,   122,   123,   124,   125,  1845,  1065,
     126,  1047,  1070,  1097,   120,   121,   122,   123,   124,   125,
    1073,  1079,   126,  1080,  1081,  1578,  1579,  1580,  1581,  1582,
    1583,  1584,  1585,  1586,  1905,  1588,  1104,  1112,  1591,  1113,
    1593,  1594,   678,  1596,  1597,  1598,  1599,  1114,  1601,  1115,
    1603,  1116,  1605,  1606,  1607,  1608,  1609,  1117,  1118,  1612,
     120,   121,   122,   123,   124,   125,  1119,  1120,   126,  1618,
    1121,  1620,  1621,  1122,  1623,  1127,  1130,  1626,  1131,  1969,
    1132,  1133,  1631,  1134,  1633,  1139,  1140,  1154,  1155,  1156,
    1157,  1158,  1641,  1642,  1970,   120,   121,   122,   123,   124,
     125,  1651,  1161,   126,  1653,  1971,  1654,  1655,  1159,  1657,
    1658,  1659,  1660,  1160,  1662,  1162,  1663,  1664,  1665,  1164,
    1168,  1180,  1182,  1183,  1987,  1670,  1671,  1672,  1673,  1674,
    1675,  1676,  1185,  1186,  1191,  1189,   120,   121,   122,   123,
     124,   125,  1210,  1219,   126,  1224,  1236,  1241,  1693,  1694,
    1695,  1215,  1216,  1218,   120,   121,   122,   123,   124,   125,
    1705,  1706,   126,  1708,  1709,  1710,  1711,  1712,  1713,  1714,
    1715,  1716,  1717,  1718,  1247,  1269,  1248,  1249,  1723,  1724,
     120,   121,   122,   123,   124,   125,  1731,  1270,   126,  1272,
     120,   121,   122,   123,   124,   125,  1279,  2076,   126,  1280,
    1281,  1282,  1283,  1284,  1285,  1286,  1287,  1751,  1752,  1753,
    1754,  1288,  1757,  1289,  1293,  1295,  1759,  1760,  2080,  1762,
    1763,  1764,  1765,  1297,  1767,  1768,  1769,  1302,  1771,  1772,
    1773,  1774,  1775,  1317,  1318,  2082,  1319,  1320,  1781,  1323,
    1783,  1326,  1343,  1786,  2083,  1344,  1346,  1350,  1791,  1365,
    1374,  1417,  1795,  2088,  1394,   120,   121,   122,   123,   124,
     125,  1391,  1805,   126,  1806,  1418,  1807,  1808,  1419,  1810,
    1811,  1812,  1813,  1425,  1815,  1816,  1426,  1427,  1428,  1429,
    1430,  1822,  1823,  1824,  1825,  1826,  1827,  1431,  1432,  1480,
     120,   121,   122,   123,   124,   125,  1433,  1434,   126,   886,
    1841,   120,   121,   122,   123,   124,   125,  1435,  1441,   126,
    1850,  1851,  1852,  1853,  1854,  1855,  1856,  1857,  1858,  1859,
    1860,  1861,  1442,  1457,  1864,  1865,  1866,  1458,  1867,  1459,
    1460,  1464,  2143,   120,   121,   122,   123,   124,   125,  1482,
    1483,   126,  1499,  1500,  1883,  1884,  1543,  1522,  1525,  1544,
    1889,  1890,  1891,  1892,  1893,  1894,  1895,  1545,  1897,  1898,
    1899,  1900,  1530,  1902,  1903,  1532,  1540,  1906,  1907,  1908,
    1909,  1541,  1546,  1547,  1548,  2144,  1549,  1550,  1917,  1551,
    1557,  1571,  1560,  1922,  1565,  1567,  1925,  2145,  1572,  1573,
    1574,  1575,  1576,  1589,  1590,  1934,  1935,  1936,  1937,  2149,
    1939,  1940,  1941,  1942,  1592,  1944,  1945,  1604,  1595,  1948,
    1949,  1950,  1951,  1952,  1953,  1954,  1600,   120,   121,   122,
     123,   124,   125,  1602,  1610,   126,  1635,  1611,  1967,  1644,
    1645,  1646,  1647,  1648,  1666,   948,  1667,  1975,  1976,  1977,
    1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,  1986,  1668,
    1988,  1669,  1677,  1679,  1992,   120,   121,   122,   123,   124,
     125,  1683,   662,   126,   120,   121,   122,   123,   124,   125,
    1688,  1696,   126,  1707,  1725,  2013,  2014,  2015,  2016,  2017,
    2018,  1719,  2020,  2021,  2022,  2023,  1726,  2025,  2026,  1727,
    2028,  1728,  1729,  1733,  2032,   120,   121,   122,   123,   124,
     125,  2039,  1730,   126,  1745,  1746,  2044,  1747,  1748,  1761,
    1766,  2049,  2050,  2051,  1770,  1758,  1801,  2055,  2056,  2057,
    2058,  2160,  2060,  2061,  2062,  2063,  1780,  2065,  2066,  2067,
    2068,  1802,  1803,  1804,  2072,  2073,  2074,  1819,  1820,  2185,
     120,   121,   122,   123,   124,   125,  1821,  1817,   126,  1832,
    1837,  2089,  2090,  2091,  1840,  1842,  1843,  2095,  1868,  2096,
    2097,  2098,  1844,  2100,  1862,  2261,  2103,  2104,  2105,  2106,
    1869,  1870,  1896,  1901,  2110,  2262,   120,   121,   122,   123,
     124,   125,  1931,  1932,   126,  1926,  1933,  1947,  1959,  2125,
    2126,  2127,  1964,  2129,  2130,  2131,  2132,  2133,  1993,  1994,
    2136,  1968,  1989,  1990,  2140,  2141,  2142,  1991,  2009,  2007,
    2146,  2010,  2011,  2019,  2024,  2027,  2152,  2153,  2053,  2155,
    2156,  2157,  2029,   120,   121,   122,   123,   124,   125,  2054,
    2079,   126,  2167,  2168,  2084,  2111,  2171,  2030,  2099,   679,
    2173,  2174,  2031,  2047,  2101,  2102,  2178,  2179,  2180,   120,
     121,   122,   123,   124,   125,  2112,  2117,   126,  2134,  2135,
    2119,  2120,  2139,  2195,  2196,  2197,  2198,  2161,  2200,  2201,
    2202,  2203,  2204,  2205,  2206,  2263,   120,   121,   122,   123,
     124,   125,  2137,  2138,   126,  2162,  2276,  2163,  2165,  2219,
    2220,  2221,  2222,  2166,  2175,  2225,  2226,  2227,  2228,  2229,
    2230,  2176,  2177,  2183,  2184,  2235,  2236,  2186,  2199,   120,
     121,   122,   123,   124,   125,  2245,  2246,   126,  2279,  2248,
    2249,  2250,  2251,  2252,  2253,  2254,  2255,   120,   121,   122,
     123,   124,   125,  2207,  2189,   126,  2190,  2191,  2267,  2268,
    2269,  2270,  2271,  2208,  2212,  2223,  2272,  2224,  2243,  2244,
      99,  2258,  2273,  2274,  2284,  2281,  2282,  2283,  2285,  2287,
    2286,   120,   121,   122,   123,   124,   125,  2293,  2288,   126,
    2292,  2295,   100,  2299,  2296,   101,  2302,   102,  2297,  2305,
    2306,   103,  2308,  2298,   104,  2311,  2312,  2309,  2318,  2315,
    2300,  2316,  2317,  2303,  2304,  2319,  2325,  2326,  2327,  2330,
    2323,  2324,  2280,  2332,  2333,  2334,  2341,  2335,  2331,  2337,
    2346,  2347,  2351,  2355,  2356,  2338,  2339,  2340,  2360,  2342,
    2364,  2365,  2369,  2345,  2372,  2375,  2378,  2381,  2349,  2350,
       0,   246,     0,  2354,     0,     0,     0,  2358,  2359,     0,
    2320,     0,  2363,     0,     0,     0,  2366,  2367,     0,  2322,
       0,  2370,     0,     0,  2373,     0,     0,  2376,     0,  1384,
    2379,   118,     0,  2382,     2,     3,     4,     5,     0,     6,
       7,     8,     0,     0,     9,     0,     0,    10,     0,    11,
       0,    12,     0,     0,     0,     0,     0,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,    20,     0,     0,
       0,     0,     0,    21,     0,    22,    23,    24,    25,     0,
       0,    26,     0,    27,     0,    28,    29,     0,     0,     0,
       2,     3,     4,     5,   681,     6,     7,     8,     0,     0,
       9,     0,    30,    10,     0,    11,     0,    12,    31,    32,
      33,    34,     0,     0,    13,    14,    15,    16,    17,    18,
      19,   105,     0,    20,     0,     0,    35,    36,     0,    21,
     682,    22,    23,    24,    25,     0,     0,    26,     0,    27,
       0,    28,    29,     0,    37,     0,     2,     3,     4,     5,
       0,     6,     7,     8,     0,     0,     9,     0,    30,    10,
       0,    11,     0,    12,    31,    32,    33,    34,     0,     0,
      13,    14,    15,    16,    17,    18,    19,   683,     0,    20,
       0,     0,    35,    36,     0,    21,     0,    22,    23,    24,
      25,     0,     0,    26,     0,    27,     0,    28,    29,     0,
      37,     0,     0,   685,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,    30,     0,     0,     0,     0,     0,
      31,    32,    33,    34,   120,   121,   122,   123,   124,   125,
     686,     0,   126,     0,     0,     0,     0,     0,    35,    36,
     120,   121,   122,   123,   124,   125,     0,     0,   126,   120,
     121,   122,   123,   124,   125,     0,    37,   126,     0,     0,
       0,     0,     0,   687,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,   120,   121,   122,   123,   124,   125,
       0,   688,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,     0,     0,   689,   120,   121,   122,   123,
     124,   125,     0,     0,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,    38,     0,     0,    39,    40,     0,    41,
       0,     0,     0,     0,     0,    42,   120,   121,   122,   123,
     124,   125,     0,     0,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   120,   121,   122,   123,   124,   125,     0,    38,
     126,     0,    39,    40,     0,    41,     0,     0,     0,     0,
       0,    42,     0,   201,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,     0,     0,    38,     0,     0,    39,    40,
       0,    41,     0,     0,     0,     0,     0,    42,   120,   121,
     122,   123,   124,   125,     0,     0,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,   690,   120,
     121,   122,   123,   124,   125,     0,     0,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,   691,   120,
     121,   122,   123,   124,   125,     0,     0,   126,   120,   121,
     122,   123,   124,   125,   692,     0,   126,   120,   121,   122,
     123,   124,   125,   693,     0,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,     0,   694,   120,
     121,   122,   123,   124,   125,     0,     0,   126,   695,     0,
     120,   121,   122,   123,   124,   125,     0,   696,   126,   120,
     121,   122,   123,   124,   125,     0,   697,   126,   120,   121,
     122,   123,   124,   125,     0,   698,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,     0,     0,
     699,     0,   120,   121,   122,   123,   124,   125,     0,   700,
     126,   120,   121,   122,   123,   124,   125,     0,   701,   126,
     120,   121,   122,   123,   124,   125,     0,   825,   126,   120,
     121,   122,   123,   124,   125,     0,   827,   126,   120,   121,
     122,   123,   124,   125,     0,   828,   126,   120,   121,   122,
     123,   124,   125,     0,   829,   126,   120,   121,   122,   123,
     124,   125,     0,   831,   126,   120,   121,   122,   123,   124,
     125,     0,   832,   126,   120,   121,   122,   123,   124,   125,
       0,   833,   126,   120,   121,   122,   123,   124,   125,     0,
     835,   126,   120,   121,   122,   123,   124,   125,     0,   836,
     126,   120,   121,   122,   123,   124,   125,     0,   842,   126,
     120,   121,   122,   123,   124,   125,     0,   843,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
     844,     0,   120,   121,   122,   123,   124,   125,     0,   845,
     126,   120,   121,   122,   123,   124,   125,     0,   848,   126,
     120,   121,   122,   123,   124,   125,     0,   849,   126,   120,
     121,   122,   123,   124,   125,     0,   850,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,   963,     0,
     120,   121,   122,   123,   124,   125,     0,   965,   126,   120,
     121,   122,   123,   124,   125,     0,   966,   126,   120,   121,
     122,   123,   124,   125,     0,   967,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,   120,   121,   122,   123,   124,
     125,     0,   969,   126,   120,   121,   122,   123,   124,   125,
       0,   970,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,   971,     0,   120,   121,   122,   123,   124,
     125,     0,   973,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   974,     0,   120,   121,   122,   123,   124,
     125,     0,   980,   126,   120,   121,   122,   123,   124,   125,
       0,   981,   126,   120,   121,   122,   123,   124,   125,     0,
     983,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,   986,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,   987,     0,   120,   121,   122,   123,
     124,   125,     0,   988,   126,   120,   121,   122,   123,   124,
     125,     0,  1005,   126,   120,   121,   122,   123,   124,   125,
       0,  1084,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,     0,     0,  1085,     0,   120,   121,
     122,   123,   124,   125,     0,  1086,   126,   120,   121,   122,
     123,   124,   125,     0,  1087,   126,   120,   121,   122,   123,
     124,   125,     0,  1088,   126,   120,   121,   122,   123,   124,
     125,     0,  1089,   126,   120,   121,   122,   123,   124,   125,
       0,  1090,   126,   120,   121,   122,   123,   124,   125,     0,
    1091,   126,   120,   121,   122,   123,   124,   125,     0,  1092,
     126,   120,   121,   122,   123,   124,   125,     0,  1093,   126,
     120,   121,   122,   123,   124,   125,     0,  1094,   126,   120,
     121,   122,   123,   124,   125,     0,  1095,   126,   120,   121,
     122,   123,   124,   125,     0,  1096,   126,   120,   121,   122,
     123,   124,   125,     0,  1108,   126,   120,   121,   122,   123,
     124,   125,     0,  1109,   126,   120,   121,   122,   123,   124,
     125,     0,  1110,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,  1124,     0,   120,   121,
     122,   123,   124,   125,     0,  1197,   126,   120,   121,   122,
     123,   124,   125,     0,  1198,   126,   120,   121,   122,   123,
     124,   125,     0,  1199,   126,   120,   121,   122,   123,   124,
     125,     0,  1200,   126,   120,   121,   122,   123,   124,   125,
       0,  1201,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,  1202,     0,   120,   121,   122,   123,
     124,   125,     0,  1203,   126,   120,   121,   122,   123,   124,
     125,     0,  1204,   126,   120,   121,   122,   123,   124,   125,
       0,  1205,   126,   120,   121,   122,   123,   124,   125,     0,
    1206,   126,   120,   121,   122,   123,   124,   125,     0,  1207,
     126,   120,   121,   122,   123,   124,   125,     0,  1208,   126,
     120,   121,   122,   123,   124,   125,     0,  1209,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,  1221,
       0,   120,   121,   122,   123,   124,   125,     0,  1222,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,  1223,
       0,   120,   121,   122,   123,   124,   125,     0,  1237,   126,
     120,   121,   122,   123,   124,   125,     0,  1296,   126,   120,
     121,   122,   123,   124,   125,     0,  1303,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,  1304,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    1305,     0,   120,   121,   122,   123,   124,   125,     0,  1306,
     126,   120,   121,   122,   123,   124,   125,     0,  1307,   126,
     120,   121,   122,   123,   124,   125,     0,  1308,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
       0,     0,  1309,     0,   120,   121,   122,   123,   124,   125,
       0,  1310,   126,   120,   121,   122,   123,   124,   125,     0,
    1311,   126,   120,   121,   122,   123,   124,   125,     0,  1312,
     126,   120,   121,   122,   123,   124,   125,     0,  1313,   126,
     120,   121,   122,   123,   124,   125,     0,  1314,   126,   120,
     121,   122,   123,   124,   125,     0,  1315,   126,   120,   121,
     122,   123,   124,   125,     0,  1327,   126,   120,   121,   122,
     123,   124,   125,     0,  1328,   126,   120,   121,   122,   123,
     124,   125,     0,  1329,   126,   120,   121,   122,   123,   124,
     125,     0,  1399,   126,   120,   121,   122,   123,   124,   125,
       0,  1400,   126,   120,   121,   122,   123,   124,   125,     0,
    1401,   126,   120,   121,   122,   123,   124,   125,     0,  1402,
     126,   120,   121,   122,   123,   124,   125,     0,  1403,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,     0,
       0,     0,  1404,     0,   120,   121,   122,   123,   124,   125,
       0,  1405,   126,   120,   121,   122,   123,   124,   125,     0,
    1406,   126,   120,   121,   122,   123,   124,   125,     0,  1407,
     126,   120,   121,   122,   123,   124,   125,     0,  1408,   126,
     120,   121,   122,   123,   124,   125,     0,  1409,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    1410,     0,   120,   121,   122,   123,   124,   125,     0,  1411,
     126,   120,   121,   122,   123,   124,   125,     0,  1422,   126,
     120,   121,   122,   123,   124,   125,     0,  1423,   126,   120,
     121,   122,   123,   124,   125,     0,  1424,   126,   120,   121,
     122,   123,   124,   125,     0,  1436,   126,   120,   121,   122,
     123,   124,   125,     0,  1486,   126,   120,   121,   122,   123,
     124,   125,     0,  1488,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,  1489,     0,   120,   121,   122,
     123,   124,   125,     0,  1491,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,  1492,     0,   120,   121,   122,
     123,   124,   125,     0,  1493,   126,   120,   121,   122,   123,
     124,   125,     0,  1494,   126,   120,   121,   122,   123,   124,
     125,     0,  1496,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,  1498,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,  1507,     0,   120,   121,
     122,   123,   124,   125,     0,  1517,   126,   120,   121,   122,
     123,   124,   125,     0,  1553,   126,   120,   121,   122,   123,
     124,   125,     0,  1556,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,     0,     0,  1558,     0,
     120,   121,   122,   123,   124,   125,     0,  1559,   126,   120,
     121,   122,   123,   124,   125,     0,  1561,   126,   120,   121,
     122,   123,   124,   125,     0,  1562,   126,   120,   121,   122,
     123,   124,   125,     0,  1563,   126,   120,   121,   122,   123,
     124,   125,     0,  1564,   126,   120,   121,   122,   123,   124,
     125,     0,  1566,   126,   120,   121,   122,   123,   124,   125,
       0,  1568,   126,   120,   121,   122,   123,   124,   125,     0,
    1570,   126,   120,   121,   122,   123,   124,   125,     0,  1577,
     126,   120,   121,   122,   123,   124,   125,     0,  1587,   126,
     120,   121,   122,   123,   124,   125,     0,  1619,   126,   120,
     121,   122,   123,   124,   125,     0,  1622,   126,   120,   121,
     122,   123,   124,   125,     0,  1624,   126,   120,   121,   122,
     123,   124,   125,     0,  1625,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,     0,  1627,     0,
     120,   121,   122,   123,   124,   125,     0,  1628,   126,   120,
     121,   122,   123,   124,   125,     0,  1629,   126,   120,   121,
     122,   123,   124,   125,     0,  1630,   126,   120,   121,   122,
     123,   124,   125,     0,  1632,   126,   120,   121,   122,   123,
     124,   125,     0,  1634,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,  1636,     0,   120,   121,
     122,   123,   124,   125,     0,  1643,   126,   120,   121,   122,
     123,   124,   125,     0,  1678,   126,   120,   121,   122,   123,
     124,   125,     0,  1681,   126,   120,   121,   122,   123,   124,
     125,     0,  1682,   126,   120,   121,   122,   123,   124,   125,
       0,  1684,   126,   120,   121,   122,   123,   124,   125,     0,
    1685,   126,   120,   121,   122,   123,   124,   125,     0,  1686,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,  1687,     0,   120,   121,   122,   123,   124,   125,     0,
    1689,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,  1690,     0,   120,   121,   122,   123,   124,   125,     0,
    1691,   126,   120,   121,   122,   123,   124,   125,     0,  1697,
     126,   120,   121,   122,   123,   124,   125,     0,  1734,   126,
       0,   120,   121,   122,   123,   124,   125,     0,  1735,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,     0,
       0,     0,  1737,     0,   120,   121,   122,   123,   124,   125,
       0,  1738,   126,   120,   121,   122,   123,   124,   125,     0,
    1739,   126,   120,   121,   122,   123,   124,   125,     0,  1740,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,     0,  1742,     0,   120,   121,   122,   123,
     124,   125,     0,  1743,   126,   120,   121,   122,   123,   124,
     125,     0,  1744,   126,   120,   121,   122,   123,   124,   125,
       0,  1749,   126,   120,   121,   122,   123,   124,   125,     0,
    1750,   126,   120,   121,   122,   123,   124,   125,     0,  1782,
     126,   120,   121,   122,   123,   124,   125,     0,  1784,   126,
     120,   121,   122,   123,   124,   125,     0,  1785,   126,   120,
     121,   122,   123,   124,   125,     0,  1787,   126,   120,   121,
     122,   123,   124,   125,     0,  1788,   126,   120,   121,   122,
     123,   124,   125,     0,  1789,   126,   120,   121,   122,   123,
     124,   125,     0,  1790,   126,   120,   121,   122,   123,   124,
     125,     0,  1792,   126,   120,   121,   122,   123,   124,   125,
       0,  1793,   126,   120,   121,   122,   123,   124,   125,     0,
    1799,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,  1800,     0,   120,   121,   122,   123,
     124,   125,     0,  1818,   126,   120,   121,   122,   123,   124,
     125,     0,  1828,   126,   120,   121,   122,   123,   124,   125,
       0,  1829,   126,   120,   121,   122,   123,   124,   125,     0,
    1830,   126,   120,   121,   122,   123,   124,   125,     0,  1831,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,  1833,     0,   120,   121,   122,   123,   124,   125,
       0,  1834,   126,   120,   121,   122,   123,   124,   125,     0,
    1835,   126,   120,   121,   122,   123,   124,   125,     0,  1836,
     126,   120,   121,   122,   123,   124,   125,     0,  1838,   126,
     120,   121,   122,   123,   124,   125,     0,  1839,   126,   120,
     121,   122,   123,   124,   125,     0,  1846,   126,   120,   121,
     122,   123,   124,   125,     0,  1863,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,  1871,     0,   120,
     121,   122,   123,   124,   125,     0,  1872,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,  1873,     0,   120,
     121,   122,   123,   124,   125,     0,  1874,   126,   120,   121,
     122,   123,   124,   125,     0,  1876,   126,     0,   120,   121,
     122,   123,   124,   125,     0,  1877,   126,   120,   121,   122,
     123,   124,   125,     0,  1878,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,     0,  1879,     0,
     120,   121,   122,   123,   124,   125,     0,  1881,   126,   120,
     121,   122,   123,   124,   125,     0,  1882,   126,   120,   121,
     122,   123,   124,   125,     0,  1885,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,     0,     0,
    1886,     0,   120,   121,   122,   123,   124,   125,     0,  1887,
     126,   120,   121,   122,   123,   124,   125,     0,  1888,   126,
     120,   121,   122,   123,   124,   125,     0,  1904,   126,   120,
     121,   122,   123,   124,   125,     0,  1913,   126,   120,   121,
     122,   123,   124,   125,     0,  1914,   126,   120,   121,   122,
     123,   124,   125,     0,  1915,   126,   120,   121,   122,   123,
     124,   125,     0,  1916,   126,   120,   121,   122,   123,   124,
     125,     0,  1918,   126,   120,   121,   122,   123,   124,   125,
       0,  1919,   126,   120,   121,   122,   123,   124,   125,     0,
    1920,   126,   120,   121,   122,   123,   124,   125,     0,  1921,
     126,   120,   121,   122,   123,   124,   125,     0,  1923,   126,
     120,   121,   122,   123,   124,   125,     0,  1924,   126,   120,
     121,   122,   123,   124,   125,     0,  1927,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
    1928,     0,   120,   121,   122,   123,   124,   125,     0,  1929,
     126,   120,   121,   122,   123,   124,   125,     0,  1930,   126,
     120,   121,   122,   123,   124,   125,     0,  1946,   126,   120,
     121,   122,   123,   124,   125,     0,  1955,   126,   120,   121,
     122,   123,   124,   125,     0,  1956,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,  1957,     0,
     120,   121,   122,   123,   124,   125,     0,  1958,   126,   120,
     121,   122,   123,   124,   125,     0,  1960,   126,   120,   121,
     122,   123,   124,   125,     0,  1961,   126,   120,   121,   122,
     123,   124,   125,     0,  1962,   126,   120,   121,   122,   123,
     124,   125,     0,  1963,   126,   120,   121,   122,   123,   124,
     125,     0,  1965,   126,   120,   121,   122,   123,   124,   125,
       0,  1966,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,  1972,     0,   120,   121,   122,   123,   124,
     125,     0,  1995,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,  1996,     0,   120,   121,   122,   123,   124,
     125,     0,  1997,   126,     0,   120,   121,   122,   123,   124,
     125,     0,  1998,   126,   120,   121,   122,   123,   124,   125,
       0,  2000,   126,   120,   121,   122,   123,   124,   125,     0,
    2001,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,  2002,     0,   120,   121,   122,   123,
     124,   125,     0,  2003,   126,   120,   121,   122,   123,   124,
     125,     0,  2005,   126,   120,   121,   122,   123,   124,   125,
       0,  2006,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,     0,     0,  2008,     0,   120,   121,
     122,   123,   124,   125,     0,  2012,   126,   120,   121,   122,
     123,   124,   125,     0,  2035,   126,   120,   121,   122,   123,
     124,   125,     0,  2036,   126,   120,   121,   122,   123,   124,
     125,     0,  2037,   126,   120,   121,   122,   123,   124,   125,
       0,  2038,   126,   120,   121,   122,   123,   124,   125,     0,
    2040,   126,   120,   121,   122,   123,   124,   125,     0,  2041,
     126,   120,   121,   122,   123,   124,   125,     0,  2042,   126,
     120,   121,   122,   123,   124,   125,     0,  2043,   126,   120,
     121,   122,   123,   124,   125,     0,  2045,   126,   120,   121,
     122,   123,   124,   125,     0,  2046,   126,   120,   121,   122,
     123,   124,   125,     0,  2048,   126,   120,   121,   122,   123,
     124,   125,     0,  2052,   126,   120,   121,   122,   123,   124,
     125,     0,  2069,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,  2070,     0,   120,   121,
     122,   123,   124,   125,     0,  2071,   126,   120,   121,   122,
     123,   124,   125,     0,  2075,   126,   120,   121,   122,   123,
     124,   125,     0,  2077,   126,   120,   121,   122,   123,   124,
     125,     0,  2078,   126,   120,   121,   122,   123,   124,   125,
       0,  2081,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,  2085,     0,   120,   121,   122,   123,
     124,   125,     0,  2086,   126,   120,   121,   122,   123,   124,
     125,     0,  2087,   126,   120,   121,   122,   123,   124,   125,
       0,  2092,   126,   120,   121,   122,   123,   124,   125,     0,
    2107,   126,   120,   121,   122,   123,   124,   125,     0,  2108,
     126,   120,   121,   122,   123,   124,   125,     0,  2109,   126,
     120,   121,   122,   123,   124,   125,     0,  2113,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,  2114,
       0,   120,   121,   122,   123,   124,   125,     0,  2115,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,  2118,
     120,   121,   122,   123,   124,   125,     0,     0,   126,  2122,
       0,   120,   121,   122,   123,   124,   125,     0,  2123,   126,
     120,   121,   122,   123,   124,   125,     0,  2124,   126,   120,
     121,   122,   123,   124,   125,     0,  2128,   126,     0,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    2150,     0,   120,   121,   122,   123,   124,   125,     0,  2151,
     126,   120,   121,   122,   123,   124,   125,     0,  2154,   126,
     120,   121,   122,   123,   124,   125,     0,  2158,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
       0,     0,  2159,     0,   120,   121,   122,   123,   124,   125,
       0,  2164,   126,   120,   121,   122,   123,   124,   125,     0,
    2181,   126,   120,   121,   122,   123,   124,   125,     0,  2182,
     126,   120,   121,   122,   123,   124,   125,     0,  2187,   126,
     120,   121,   122,   123,   124,   125,     0,  2188,   126,   120,
     121,   122,   123,   124,   125,     0,  2192,   126,   120,   121,
     122,   123,   124,   125,     0,  2209,   126,   120,   121,   122,
     123,   124,   125,     0,  2210,   126,   120,   121,   122,   123,
     124,   125,     0,  2213,   126,   120,   121,   122,   123,   124,
     125,     0,  2214,   126,   120,   121,   122,   123,   124,   125,
       0,  2215,   126,   120,   121,   122,   123,   124,   125,     0,
    2216,   126,   120,   121,   122,   123,   124,   125,     0,  2217,
     126,   120,   121,   122,   123,   124,   125,     0,  2218,   126,
       0,   120,   121,   122,   123,   124,   125,    75,    76,   126,
      77,    78,  2233,    79,    80,    81,     0,    82,    83,    84,
       0,  2234,     0,     0,     0,     0,     0,     0,    85,     0,
    2237,     0,   120,   121,   122,   123,   124,   125,     0,  2238,
     126,   120,   121,   122,   123,   124,   125,     0,  2239,   126,
     120,   121,   122,   123,   124,   125,     0,  2240,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    2241,     0,   120,   121,   122,   123,   124,   125,     0,  2242,
     126,   120,   121,   122,   123,   124,   125,     0,  2256,   126,
     120,   121,   122,   123,   124,   125,     0,  2257,   126,   120,
     121,   122,   123,   124,   125,     0,  2259,   126,   120,   121,
     122,   123,   124,   125,     0,  2260,   126,   120,   121,   122,
     123,   124,   125,     0,  2264,   126,   120,   121,   122,   123,
     124,   125,     0,  2275,   126,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,  2278,     0,   120,   121,   122,
     123,   124,   125,     0,  2291,   126,     0,   120,   121,   122,
     123,   124,   125,     0,  2294,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,  2307,     0,     0,     0,  1245,
       0,     0,     0,     0,  2310,   430,   120,   121,   122,   123,
     124,   125,     0,     0,   126,  1230,     0,     0,     0,    87,
       0,     0,     0,  1058,   120,   121,   122,   123,   124,   125,
       0,  1778,   126,   120,   121,   122,   123,   124,   125,  1129,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,  1383,     0,   120,   121,   122,   123,
     124,   125,     0,  1700,   126,   120,   121,   122,   123,   124,
     125,   945,     0,   126,   120,   121,   122,   123,   124,   125,
     946,     0,   126,   120,   121,   122,   123,   124,   125,  1229,
       0,   126,   120,   121,   122,   123,   124,   125,  1615,     0,
     126,     0,     0,     0,     0,     0,   944,     0,   120,   121,
     122,   123,   124,   125,     0,  1128,   126,   120,   121,   122,
     123,   124,   125,     0,  1294,   126,   120,   121,   122,   123,
     124,   125,   726,     0,   126,   120,   121,   122,   123,   124,
     125,   727,     0,   126,   120,   121,   122,   123,   124,   125,
     728,     0,   126,     0,     0,     0,   511,     0,     0,  1057,
       0,   120,   121,   122,   123,   124,   125,     0,  1512,   126,
       0,     0,    88,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,     0,     0,   666,     0,     0,     0,
       0,     0,   432,     0,   120,   121,   122,   123,   124,   125,
       0,   887,   126,   120,   121,   122,   123,   124,   125,   663,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,   736,   120,   121,   122,   123,   124,   125,     0,   506,
     126,   120,   121,   122,   123,   124,   125,   261,     0,   126,
       0,     0,     0,     0,     0,  1912,   120,   121,   122,   123,
     124,   125,     0,   633,   126,   120,   121,   122,   123,   124,
     125,  1849,     0,   126,     0,     0,     0,     0,     0,  1779,
     120,   121,   122,   123,   124,   125,     0,     0,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,  1616,     0,
     120,   121,   122,   123,   124,   125,     0,  1513,   126,   120,
     121,   122,   123,   124,   125,  1385,     0,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,   120,   121,   122,
     123,   124,   125,     0,  1231,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,   120,   121,   122,   123,   124,
     125,  1059,     0,   126,     0,     0,     0,     0,     0,  1510,
     120,   121,   122,   123,   124,   125,     0,  1382,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    1056,   120,   121,   122,   123,   124,   125,     0,   857,   126,
     120,   121,   122,   123,   124,   125,   629,     0,   126,     0,
       0,     0,     0,   858,     0,   120,   121,   122,   123,   124,
     125,     0,   859,   126,     0,   120,   121,   122,   123,   124,
     125,     0,  2266,   126,   120,   121,   122,   123,   124,   125,
    2232,     0,   126,     0,     0,     0,     0,     0,  2194,   120,
     121,   122,   123,   124,   125,     0,  2148,   126,   120,   121,
     122,   123,   124,   125,  2094,     0,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,     0,     0,
    2034,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,  1974,   120,   121,   122,   123,   124,   125,     0,     0,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,  1911,   120,   121,   122,   123,   124,   125,     0,  1848,
     126,   120,   121,   122,   123,   124,   125,  1777,     0,   126,
     120,   121,   122,   123,   124,   125,  1699,     0,   126,     0,
       0,     0,     0,     0,  1614,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,     0,     0,  1509,   120,
     121,   122,   123,   124,   125,     0,  1380,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
     855,   120,   121,   122,   123,   124,   125,     0,   626,   126,
     120,   121,   122,   123,   124,   125,     0,     0,   126,   627,
     120,   121,   122,   123,   124,   125,     0,  1226,   126,     0,
       0,     0,     0,     0,   854,   120,   121,   122,   123,   124,
     125,     0,  2361,   126,   120,   121,   122,   123,   124,   125,
    2352,     0,   126,     0,     0,     0,     0,     0,  2343,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
       0,     0,  2328,   120,   121,   122,   123,   124,   125,     0,
    2313,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,  2289,   120,   121,   122,   123,   124,   125,     0,  2265,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,  2231,     0,   120,   121,   122,   123,   124,
     125,     0,  2193,   126,   120,   121,   122,   123,   124,   125,
    2147,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,  2093,   120,   121,   122,   123,   124,
     125,     0,  2033,   126,   120,   121,   122,   123,   124,   125,
    1973,     0,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,  1910,   120,   121,   122,   123,   124,   125,
       0,     0,   126,  1847,   120,   121,   122,   123,   124,   125,
       0,  1776,   126,     0,   120,   121,   122,   123,   124,   125,
       0,  1698,   126,   120,   121,   122,   123,   124,   125,  1613,
       0,   126,     0,     0,     0,     0,     0,  1508,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
       0,  1053,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,  1379,   120,   121,   122,   123,   124,
     125,     0,  1225,   126,   120,   121,   122,   123,   124,   125,
     856,     0,   126,     0,     0,     0,     0,     0,   628,     0,
     120,   121,   122,   123,   124,   125,     0,  1381,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
       0,  1227,   120,   121,   122,   123,   124,   125,     0,  1055,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,  1232,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,     0,  1233,     0,   120,
     121,   122,   123,   124,   125,     0,  1234,   126,   120,   121,
     122,   123,   124,   125,     0,  1386,   126,   120,   121,   122,
     123,   124,   125,     0,  1387,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,     0,  1388,     0,
     120,   121,   122,   123,   124,   125,     0,  1516,   126,   120,
     121,   122,   123,   124,   125,  1514,     0,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,  1515,     0,   120,
     121,   122,   123,   124,   125,     0,  1617,   126,   120,   121,
     122,   123,   124,   125,     0,  1701,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,  2290,     0,   120,   121,
     122,   123,   124,   125,     0,  1389,   126,   120,   121,   122,
     123,   124,   125,     0,  1235,   126,   120,   121,   122,   123,
     124,   125,  1061,     0,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,     0,  1062,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
       0,  1063,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,  1060,   120,   121,   122,   123,   124,   125,     0,
     656,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,     0,     0,   852,     0,   120,   121,
     122,   123,   124,   125,     0,   853,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,   860,     0,
     120,   121,   122,   123,   124,   125,     0,   861,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
     862,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,     0,     0,   863,     0,   120,   121,   122,
     123,   124,   125,     0,   864,   126,   120,   121,   122,   123,
     124,   125,     0,  1054,   126,   120,   121,   122,   123,   124,
     125,   630,     0,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,   624,     0,   120,   121,   122,   123,
     124,   125,     0,   625,   126,   120,   121,   122,   123,   124,
     125,     0,   631,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   632,     0,   120,   121,   122,   123,   124,
     125,     0,   634,   126,   120,   121,   122,   123,   124,   125,
       0,   635,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   636,     0,   120,   121,   122,   123,   124,   125,
       0,   637,   126,   120,   121,   122,   123,   124,   125,     0,
    1099,   126,   120,   121,   122,   123,   124,   125,     0,  1100,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,  1101,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,     0,  1074,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,  1265,     0,   120,   121,
     122,   123,   124,   125,     0,  1316,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,     0,     0,
       0,  1325,     0,   120,   121,   122,   123,   124,   125,     0,
    1720,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,  1721,  1098,   120,   121,   122,   123,   124,
     125,     0,  1722,   126,     0,     0,     0,     0,   881,     0,
     120,   121,   122,   123,   124,   125,     0,  1271,   126,   120,
     121,   122,   123,   124,   125,     0,  1370,   126,   120,   121,
     122,   123,   124,   125,     0,  1371,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,  1373,   120,   121,
     122,   123,   124,   125,     0,   658,   126,     0,     0,     0,
       0,     0,  1076,   120,   121,   122,   123,   124,   125,     0,
     885,   126,  1640,     0,     0,     0,     0,  1413,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,  1569,     0,
     120,   121,   122,   123,   124,   125,     0,  1796,   126,   120,
     121,   122,   123,   124,   125,     0,  1797,   126,   120,   121,
     122,   123,   124,   125,     0,  1798,   126,     0,   120,   121,
     122,   123,   124,   125,     0,  1539,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,  1421,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,  1274,   120,
     121,   122,   123,   124,   125,     0,  1107,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,  1456,
     120,   121,   122,   123,   124,   125,     0,  1075,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    1536,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,     0,     0,     0,     0,  1537,     0,   120,   121,   122,
     123,   124,   125,     0,  1538,   126,   120,   121,   122,   123,
     124,   125,   882,     0,   126,   120,   121,   122,   123,   124,
     125,  1637,     0,   126,     0,   120,   121,   122,   123,   124,
     125,     0,  1638,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,     0,  1639,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
       0,  1395,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,  1414,     0,   120,   121,   122,   123,   124,
     125,     0,  1415,   126,   120,   121,   122,   123,   124,   125,
       0,  1416,   126,   120,   121,   122,   123,   124,   125,     0,
    1243,   126,   120,   121,   122,   123,   124,   125,     0,  1266,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
    1267,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,  1268,   120,   121,   122,   123,   124,   125,     0,   657,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
       0,  1163,   120,   121,   122,   123,   124,   125,  2169,     0,
     126,   120,   121,   122,   123,   124,   125,     0,     0,   126,
    2172,   120,   121,   122,   123,   124,   125,     0,     0,   126,
     120,   121,   122,   123,   124,   125,  2357,     0,   126,     0,
       0,     0,     0,     0,  2116,     0,   120,   121,   122,   123,
     124,   125,     0,  2121,   126,     0,   120,   121,   122,   123,
     124,   125,     0,  2348,   126,   120,   121,   122,   123,   124,
     125,  2059,     0,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,  2064,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,     0,     0,  2336,
     120,   121,   122,   123,   124,   125,     0,     0,   126,     0,
       0,  1999,     0,   120,   121,   122,   123,   124,   125,     0,
    2004,   126,   120,   121,   122,   123,   124,   125,     0,  2321,
     126,     0,     0,     0,     0,     0,     0,  1938,     0,   120,
     121,   122,   123,   124,   125,     0,  1943,   126,     0,   120,
     121,   122,   123,   124,   125,     0,  2301,   126,     0,   120,
     121,   122,   123,   124,   125,     0,  1875,   126,   120,   121,
     122,   123,   124,   125,     0,  1880,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,  2277,   120,   121,   122,
     123,   124,   125,     0,  1809,   126,     0,   120,   121,   122,
     123,   124,   125,     0,  1814,   126,   120,   121,   122,   123,
     124,   125,     0,  2247,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,     0,     0,  1736,   120,
     121,   122,   123,   124,   125,     0,     0,   126,  1741,     0,
     120,   121,   122,   123,   124,   125,     0,  2211,   126,   120,
     121,   122,   123,   124,   125,  1656,     0,   126,     0,     0,
       0,     0,     0,   522,     0,     0,     0,  1661,     0,     0,
       0,     0,   524,     0,   120,   121,   122,   123,   124,   125,
       0,  2170,   126,   120,   121,   122,   123,   124,   125,   528,
       0,   126,   120,   121,   122,   123,   124,   125,     0,   532,
     126,     0,     0,  2362,     0,   120,   121,   122,   123,   124,
     125,     0,  2380,   126,   120,   121,   122,   123,   124,   125,
    2353,     0,   126,   120,   121,   122,   123,   124,   125,     0,
    2377,   126,   120,   121,   122,   123,   124,   125,     0,  2344,
     126,   120,   121,   122,   123,   124,   125,     0,  2374,   126,
     120,   121,   122,   123,   124,   125,  2329,     0,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
    2371,     0,  1192,     0,   120,   121,   122,   123,   124,   125,
       0,  1193,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,     0,     0,  1194,     0,   120,   121,
     122,   123,   124,   125,     0,  1250,   126,   120,   121,   122,
     123,   124,   125,     0,  1298,   126,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,  1020,     0,   120,   121,
     122,   123,   124,   125,     0,  1021,   126,   120,   121,   122,
     123,   124,   125,     0,  1022,   126,   120,   121,   122,   123,
     124,   125,     0,  1082,   126,   120,   121,   122,   123,   124,
     125,     0,  1135,   126,   120,   121,   122,   123,   124,   125,
     818,     0,   126,   120,   121,   122,   123,   124,   125,   819,
       0,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,   820,     0,   120,   121,   122,   123,
     124,   125,     0,   892,   126,   120,   121,   122,   123,   124,
     125,     0,   955,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,     0,  1069,     0,     0,
       0,     0,   937,     0,   120,   121,   122,   123,   124,   125,
       0,  1123,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,   337,   120,   121,   122,   123,   124,   125,  1292,
       0,   126,   120,   121,   122,   123,   124,   125,  1439,     0,
     126,     0,     0,     0,     0,     0,  1240,     0,   120,   121,
     122,   123,   124,   125,     0,  1555,   126,   120,   121,   122,
     123,   124,   125,     0,  1652,   126,   120,   121,   122,   123,
     124,   125,   942,     0,   126,   120,   121,   122,   123,   124,
     125,     0,     0,   126,     0,     0,  1125,  1440,     0,   120,
     121,   122,   123,   124,   125,     0,  1554,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,   120,   121,
     122,   123,   124,   125,     0,  1126,   126,   120,   121,   122,
     123,   124,   125,     0,  1291,   126,   120,   121,   122,   123,
     124,   125,     0,   941,   126,     0,     0,     0,   255,     0,
       0,   575,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,   256,     0,     0,     0,   874,     0,     0,
       0,     0,   250,     0,   120,   121,   122,   123,   124,   125,
       0,   251,   126,   120,   121,   122,   123,   124,   125,     0,
       0,   126,     0,     0,     0,   260,     0,   120,   121,   122,
     123,   124,   125,     0,   565,   126,     0,   120,   121,   122,
     123,   124,   125,     0,   566,   126,   120,   121,   122,   123,
     124,   125,   182,     0,   126,   120,   121,   122,   123,   124,
     125,   243,     0,   126,     0,     0,     0,     0,     0,   499,
     120,   121,   122,   123,   124,   125,     0,     0,   126,   120,
     121,   122,   123,   124,   125,     0,     0,   126,     0,     0,
       0,   512,     0,   120,   121,   122,   123,   124,   125,     0,
     568,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,     0,  1012,     0,   120,   121,   122,   123,
     124,   125,     0,  1038,   126,     0,     0,   252,     0,   120,
     121,   122,   123,   124,   125,     0,   253,   126,   120,   121,
     122,   123,   124,   125,     0,     0,   126,     0,     0,     0,
       0,   254,     0,   120,   121,   122,   123,   124,   125,     0,
     582,   126,   120,   121,   122,   123,   124,   125,     0,     0,
     126,     0,     0,   655,     0,   120,   121,   122,   123,   124,
     125,     0,  1111,   126,   120,   121,   122,   123,   124,   125,
       0,     0,   126,   267,   120,   121,   122,   123,   124,   125,
       0,     0,   126,     0,     0,     0,   268,     0,   120,   121,
     122,   123,   124,   125,     0,   961,   126,   120,   121,   122,
     123,   124,   125,     0,     0,   126,     0,     0,   574,     0,
       0,     0,     0,     0,     0,   249,   120,   121,   122,   123,
     124,   125,     0,     0,   126,     0,     0,  1083,   120,   121,
     122,   123,   124,   125,     0,   672,   126,   120,   121,   122,
     123,   124,   125,     0,   893,   126,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   936,     0,     0,     0,     0,
       0,   263,  2314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2368,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,     0,   306,   307,   308,   309,
     310,   311,   312,   313,     0,     0,   314,   315,   316,   317,
     318,   319,     0,   320,   321,   322,   323,   324
  };

  const short int
  feiparser::yycheck_[] =
  {
       8,     9,    35,    36,    37,    38,    39,    40,    56,    56,
      43,   418,    20,    21,    22,    35,    36,    37,    38,    39,
      40,   418,   114,    43,    55,   102,    99,    35,   268,    55,
      56,    99,    64,    41,     3,     4,     5,     6,   111,     8,
       9,    40,     0,   111,    43,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   418,   114,     3,    27,    28,
      29,    30,    31,    32,    33,    73,    53,    36,   145,     3,
       4,     5,     6,    42,     8,     9,    35,    36,    37,    38,
      39,    40,    90,   109,    43,    51,    55,    56,    99,    58,
     421,    49,   412,    27,    28,    29,    30,    31,    32,    33,
     111,    55,    36,   111,    37,    38,    39,    40,    42,   349,
      43,   109,   120,   121,   122,   123,   124,   125,   126,   117,
      55,    55,    35,    36,    37,    38,    39,    40,    55,    56,
      43,    72,    59,   128,    61,   143,   144,   145,    65,   424,
     148,   149,   126,   151,   152,   153,   154,   155,    43,    97,
     158,   159,   100,   161,    88,     3,   104,   165,   166,   167,
     168,   169,   170,    91,    92,    99,    35,    36,    37,    38,
      39,    40,   204,   169,    43,   114,   172,   173,   114,   187,
      35,    36,    37,    38,    39,    40,   419,   420,    43,   197,
       1,     5,     3,     4,     5,     6,   114,     8,     9,    10,
      39,    40,    13,   171,    43,    16,   174,    18,   216,    20,
     136,   137,   138,   114,   140,   141,    27,    28,    29,    30,
      31,    32,    33,   231,   114,    36,   244,   245,   246,   114,
     248,    42,   114,    44,    45,    46,    47,    55,    56,    50,
     114,    52,   114,    54,    55,   407,    35,    36,    37,    38,
      39,    40,   141,   142,    43,   128,   264,   114,   266,   417,
      71,   269,   114,   271,   180,   181,    77,    78,    79,    80,
      35,    36,    37,    38,    39,    40,   187,   285,    43,   388,
     389,   390,   162,   163,    95,    96,   114,    35,    36,    37,
      38,    39,    40,   116,   114,    43,    35,    36,    37,    38,
      39,    40,   113,   407,    43,    35,    36,    37,    38,    39,
      40,   162,   163,    43,    35,    36,    37,    38,    39,    40,
     412,   114,    43,   114,   162,   163,   114,   335,    35,    36,
      37,    38,    39,    40,   342,   114,    43,    35,    36,    37,
      38,    39,    40,   114,    82,    43,   354,   355,   356,    35,
      36,    37,    38,    39,    40,   363,   133,    43,    85,    87,
     368,    94,   370,   421,   412,   412,   374,   168,    88,   101,
     378,    35,    36,    37,    38,    39,    40,   408,    73,    43,
      35,    36,    37,    38,    39,    40,   129,   412,    43,    12,
     114,    65,   425,    35,    36,    37,    38,    39,    40,   111,
     134,    43,   410,   411,   424,   413,   375,   415,   416,    55,
     127,    55,   367,   421,   422,   111,    99,   111,    89,    81,
     428,   429,    35,    36,    37,    38,    39,    40,     5,    99,
      43,    35,    36,    37,    38,    39,    40,   445,    43,    43,
     426,   116,    35,    36,    37,    38,    39,    40,   425,   418,
      43,   114,   423,    35,    36,    37,    38,    39,    40,   116,
     114,    43,    34,   422,   217,   371,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
      55,    81,   170,   103,   418,    35,    36,    37,    38,    39,
      40,   171,    55,    43,   502,   503,   504,   505,   418,   109,
     114,   509,   510,    55,    34,   513,   419,   420,    55,    58,
       3,   519,   520,    34,   117,   523,    34,   525,   526,   527,
     114,   529,   530,   531,    83,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   180,   225,    35,    36,    37,    38,    39,    40,
     419,   420,    43,    35,    36,    37,    38,    39,    40,   175,
      34,    43,   570,   571,   572,   420,    35,    36,    37,    38,
      39,    40,   580,    81,    43,    81,    81,   585,    35,    36,
      37,    38,    39,    40,   111,   111,    43,   595,    35,    36,
      37,    38,    39,    40,   111,   111,    43,     3,     4,     5,
       6,   412,     8,     9,   415,   416,   278,   418,   111,    99,
     111,   111,   111,   424,     3,     4,     5,     6,   278,     8,
       9,    27,    28,    29,    30,    31,    32,    33,   111,   278,
      36,   420,   111,   111,   111,   111,    42,   278,    27,    28,
      29,    30,    31,    32,    33,   111,   654,    36,   111,    55,
     111,   111,   373,    42,   278,   420,   111,   665,   111,   111,
     668,   669,   670,   111,   279,   404,    55,   675,   346,   677,
     678,   679,   420,   681,   682,   683,   406,   685,   686,   687,
     688,   689,   690,   691,   692,   693,   694,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   948,   344,
     344,   278,   278,   420,   278,   114,   278,   725,   278,   114,
     109,    57,   420,   114,   111,   733,   734,   735,   114,   119,
     738,   739,   740,   419,   742,   114,   111,   111,   114,   747,
     748,   256,    35,    36,    37,    38,    39,    40,    34,   114,
      43,   279,   254,   114,   279,   171,   420,   368,    35,    36,
      37,    38,    39,    40,   182,   420,    43,    35,    36,    37,
      38,    39,    40,   182,   182,    43,   180,   102,   420,   102,
     119,   119,   119,    34,   792,   793,   794,   119,   119,   119,
     798,   799,   800,   119,   802,   803,   804,   119,   806,   807,
     119,   119,   119,   811,   119,   119,   119,   420,   119,   817,
     119,   119,   119,   119,   119,   823,   420,   825,   119,   827,
     828,   829,   119,   831,   832,   833,   419,   835,   836,   119,
     119,    34,    34,    34,   842,   843,   844,   845,   420,    34,
     848,   849,   850,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,    34,    34,    34,    34,    34,    34,   131,
     868,   869,   870,    35,    36,    37,    38,    39,    40,   419,
     878,    43,   122,    34,    34,    34,   884,   391,   391,   391,
      34,   889,   890,   891,    35,    36,    37,    38,    39,    40,
      34,    34,    43,    34,   115,   180,   115,   115,   182,    34,
     908,    35,    36,    37,    38,    39,    40,    34,   418,    43,
     141,   418,   117,    34,   418,   923,   924,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   934,   935,    34,   420,
     418,   418,   418,   418,   418,   943,    34,   418,   420,    34,
     131,   949,   950,   418,   952,   953,   954,   418,   418,   396,
     419,   959,   960,   410,   962,   963,   964,   965,   966,   967,
     968,   969,   970,   971,   972,   973,   974,   418,   976,   977,
     978,   979,   980,   981,   418,   983,   418,   985,   986,   987,
     988,   989,    35,    36,    37,    38,    39,    40,   418,   418,
      43,   418,   418,   418,   418,  1003,   418,  1005,  1006,  1007,
     117,   418,  1010,  1011,   418,    35,    36,    37,    38,    39,
      40,  1019,   418,    43,    34,   117,   182,    58,    58,    35,
      36,    37,    38,    39,    40,    58,   115,    43,   184,   418,
      35,    36,    37,    38,    39,    40,   184,   184,    43,  1047,
     140,   115,   184,   102,    35,    36,    37,    38,    39,    40,
     117,   408,    43,    35,    36,    37,    38,    39,    40,   409,
      34,    43,    35,    36,    37,    38,    39,    40,   117,    67,
      43,  1079,  1080,  1081,   134,   114,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,   115,
     114,    35,    36,    37,    38,    39,    40,   114,    58,    43,
    1108,  1109,  1110,   186,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,   186,  1124,   186,   186,  1127,
      34,    34,    34,  1131,  1132,    34,  1134,   420,    34,    34,
      34,  1139,  1140,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,   420,    34,    34,    34,  1155,  1156,  1157,
    1158,    34,   420,  1161,   120,   120,  1164,   120,   115,    35,
      36,    37,    38,    39,    40,   115,   108,    43,    35,    36,
      37,    38,    39,    40,  1182,  1183,    43,    76,  1186,    34,
     110,  1189,    55,  1191,    58,    34,    34,    34,    65,  1197,
    1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,
    1208,  1209,  1210,   114,    35,    36,    37,    38,    39,    40,
      34,  1219,    43,  1221,  1222,  1223,  1224,    34,    58,    35,
      36,    37,    38,    39,    40,    34,    34,    43,   420,  1237,
      35,    36,    37,    38,    39,    40,   114,    34,    43,  1247,
    1248,  1249,   242,   242,    34,   242,   242,    34,   420,    35,
      36,    37,    38,    39,    40,    76,    76,    43,   119,    34,
     114,  1269,  1270,    34,  1272,    34,    34,    34,   114,   420,
      55,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,
    1288,  1289,   114,    34,   114,  1293,   420,  1295,  1296,  1297,
     418,   189,   111,   111,  1302,  1303,  1304,  1305,  1306,  1307,
    1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,   111,  1317,
    1318,  1319,  1320,   111,   114,  1323,   111,   411,  1326,  1327,
    1328,  1329,    35,    36,    37,    38,    39,    40,    34,    34,
      43,   114,   114,  1341,    34,  1343,  1344,   111,   119,   119,
     119,   119,  1350,    35,    36,    37,    38,    39,    40,   259,
     115,    43,   233,    34,   226,   226,   226,  1365,   226,    58,
      35,    36,    37,    38,    39,    40,  1374,   420,    43,    35,
      36,    37,    38,    39,    40,    34,    34,    43,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    34,    34,    34,
     420,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1411,   420,    34,    34,   412,    34,  1417,
    1418,  1419,    34,    34,  1422,  1423,  1424,  1425,  1426,  1427,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,   420,
    1438,    34,    34,  1441,  1442,    34,    34,    34,   420,    34,
     418,    35,    36,    37,    38,    39,    40,   420,    34,    43,
    1458,  1459,  1460,    34,   418,    34,  1464,    35,    36,    37,
      38,    39,    40,   418,    19,    43,    21,    22,    23,    24,
      25,    26,  1480,   418,  1482,  1483,   420,    34,  1486,    34,
    1488,  1489,    34,  1491,  1492,  1493,  1494,    34,  1496,    34,
    1498,  1499,  1500,    34,    34,    34,    34,    34,    34,  1507,
      35,    36,    37,    38,    39,    40,    34,    34,    43,  1517,
      34,   115,    35,    36,    37,    38,    39,    40,   420,     5,
      43,   114,   387,   115,    35,    36,    37,    38,    39,    40,
     117,    34,    43,    34,    34,  1543,  1544,  1545,  1546,  1547,
    1548,  1549,  1550,  1551,   420,  1553,    58,    34,  1556,    34,
    1558,  1559,   419,  1561,  1562,  1563,  1564,    34,  1566,    34,
    1568,    34,  1570,  1571,  1572,  1573,  1574,    34,    34,  1577,
      35,    36,    37,    38,    39,    40,    34,    34,    43,  1587,
      34,  1589,  1590,    34,  1592,    34,   409,  1595,    34,   420,
      34,    34,  1600,    34,  1602,    34,    34,    67,    34,    34,
      34,    34,  1610,  1611,   420,    35,    36,    37,    38,    39,
      40,  1619,   114,    43,  1622,   420,  1624,  1625,   115,  1627,
    1628,  1629,  1630,   115,  1632,   115,  1634,  1635,  1636,    34,
      58,    34,    34,    34,   420,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,    34,    34,    34,   418,    35,    36,    37,    38,
      39,    40,   114,    34,    43,   114,     5,   387,  1666,  1667,
    1668,    58,    58,    58,    35,    36,    37,    38,    39,    40,
    1678,  1679,    43,  1681,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1689,  1690,  1691,    34,   114,    34,    34,  1696,  1697,
      35,    36,    37,    38,    39,    40,  1704,   114,    43,   114,
      35,    36,    37,    38,    39,    40,    34,   420,    43,    34,
      34,    34,    34,    34,    34,    34,    34,  1725,  1726,  1727,
    1728,    34,  1730,    34,    34,    34,  1734,  1735,   420,  1737,
    1738,  1739,  1740,    34,  1742,  1743,  1744,    34,  1746,  1747,
    1748,  1749,  1750,    34,    34,   420,    34,    34,  1756,    34,
    1758,    34,    34,  1761,   420,    34,    34,    34,  1766,    34,
      34,    34,  1770,   420,   387,    35,    36,    37,    38,    39,
      40,   111,  1780,    43,  1782,    34,  1784,  1785,    34,  1787,
    1788,  1789,  1790,    34,  1792,  1793,    34,    34,    34,    34,
      34,  1799,  1800,  1801,  1802,  1803,  1804,    34,    34,   418,
      35,    36,    37,    38,    39,    40,    34,    34,    43,   403,
    1818,    35,    36,    37,    38,    39,    40,    34,    34,    43,
    1828,  1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,  1837,
    1838,  1839,    34,    34,  1842,  1843,  1844,    34,  1846,    34,
      34,    34,   420,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,   418,  1862,  1863,    34,   115,   115,    34,
    1868,  1869,  1870,  1871,  1872,  1873,  1874,    34,  1876,  1877,
    1878,  1879,   115,  1881,  1882,   115,   115,  1885,  1886,  1887,
    1888,   115,    34,    34,    34,   420,    34,    34,  1896,    34,
      58,    34,    58,  1901,    58,    58,  1904,   420,    34,    34,
      34,    58,    58,    34,    34,  1913,  1914,  1915,  1916,   420,
    1918,  1919,  1920,  1921,   114,  1923,  1924,    34,   114,  1927,
    1928,  1929,  1930,  1931,  1932,  1933,   114,    35,    36,    37,
      38,    39,    40,   114,   114,    43,   418,   114,  1946,    34,
      34,    34,    34,    34,    34,   410,    34,  1955,  1956,  1957,
    1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,  1966,    34,
    1968,    34,   111,    34,  1972,    35,    36,    37,    38,    39,
      40,    34,   402,    43,    35,    36,    37,    38,    39,    40,
      34,   418,    43,   115,    34,  1993,  1994,  1995,  1996,  1997,
    1998,   240,  2000,  2001,  2002,  2003,    34,  2005,  2006,    34,
    2008,    34,   111,    58,  2012,    35,    36,    37,    38,    39,
      40,  2019,   418,    43,    34,    34,  2024,    34,    34,    34,
      34,  2029,  2030,  2031,   418,   114,    34,  2035,  2036,  2037,
    2038,   420,  2040,  2041,  2042,  2043,   418,  2045,  2046,  2047,
    2048,    34,    34,    34,  2052,  2053,  2054,    34,    34,   420,
      35,    36,    37,    38,    39,    40,    34,   240,    43,    34,
      34,  2069,  2070,  2071,    34,   418,   418,  2075,    34,  2077,
    2078,  2079,   418,  2081,   418,   420,  2084,  2085,  2086,  2087,
      34,    34,    34,    34,  2092,   420,    35,    36,    37,    38,
      39,    40,    34,    34,    43,   241,    34,    34,    34,  2107,
    2108,  2109,    34,  2111,  2112,  2113,  2114,  2115,    34,    34,
    2118,   418,   240,   240,  2122,  2123,  2124,   240,    34,   241,
    2128,    34,    34,    34,    34,    34,  2134,  2135,    34,  2137,
    2138,  2139,   418,    35,    36,    37,    38,    39,    40,    34,
      34,    43,  2150,  2151,    34,    34,  2154,   418,   115,   419,
    2158,  2159,   418,   418,   115,   115,  2164,  2165,  2166,    35,
      36,    37,    38,    39,    40,    34,    58,    43,    34,   114,
      58,    58,    34,  2181,  2182,  2183,  2184,   241,  2186,  2187,
    2188,  2189,  2190,  2191,  2192,   420,    35,    36,    37,    38,
      39,    40,   114,   114,    43,   241,   420,   241,    34,  2207,
    2208,  2209,  2210,    34,    34,  2213,  2214,  2215,  2216,  2217,
    2218,    34,    34,    34,    34,  2223,  2224,    34,   115,    35,
      36,    37,    38,    39,    40,  2233,  2234,    43,   420,  2237,
    2238,  2239,  2240,  2241,  2242,  2243,  2244,    35,    36,    37,
      38,    39,    40,    34,   418,    43,   418,   418,  2256,  2257,
    2258,  2259,  2260,    34,    58,    34,  2264,   114,    34,    34,
      59,    34,    34,    34,   115,  2273,  2274,  2275,    34,   115,
    2278,    35,    36,    37,    38,    39,    40,  2285,   115,    43,
      58,    58,    81,  2291,    58,    84,  2294,    86,    34,  2297,
    2298,    90,  2300,    34,    93,  2303,  2304,    34,    34,  2307,
     114,  2309,  2310,   114,   114,    34,   115,    34,   115,    58,
    2318,  2319,   420,    58,    34,    34,    34,   114,  2326,   114,
      34,    34,    34,    34,    34,  2333,  2334,  2335,    34,  2337,
      34,    34,    34,  2341,    34,    34,    34,    34,  2346,  2347,
      -1,   202,    -1,  2351,    -1,    -1,    -1,  2355,  2356,    -1,
     420,    -1,  2360,    -1,    -1,    -1,  2364,  2365,    -1,   420,
      -1,  2369,    -1,    -1,  2372,    -1,    -1,  2375,    -1,   399,
    2378,     0,    -1,  2381,     3,     4,     5,     6,    -1,     8,
       9,    10,    -1,    -1,    13,    -1,    -1,    16,    -1,    18,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    44,    45,    46,    47,    -1,
      -1,    50,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,
       3,     4,     5,     6,   419,     8,     9,    10,    -1,    -1,
      13,    -1,    71,    16,    -1,    18,    -1,    20,    77,    78,
      79,    80,    -1,    -1,    27,    28,    29,    30,    31,    32,
      33,   260,    -1,    36,    -1,    -1,    95,    96,    -1,    42,
     419,    44,    45,    46,    47,    -1,    -1,    50,    -1,    52,
      -1,    54,    55,    -1,   113,    -1,     3,     4,     5,     6,
      -1,     8,     9,    10,    -1,    -1,    13,    -1,    71,    16,
      -1,    18,    -1,    20,    77,    78,    79,    80,    -1,    -1,
      27,    28,    29,    30,    31,    32,    33,   419,    -1,    36,
      -1,    -1,    95,    96,    -1,    42,    -1,    44,    45,    46,
      47,    -1,    -1,    50,    -1,    52,    -1,    54,    55,    -1,
     113,    -1,    -1,   419,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    35,    36,    37,    38,    39,    40,
     419,    -1,    43,    -1,    -1,    -1,    -1,    -1,    95,    96,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,   113,    43,    -1,    -1,
      -1,    -1,    -1,   419,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   419,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   412,    -1,    -1,   415,   416,    -1,   418,
      -1,    -1,    -1,    -1,    -1,   424,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,   412,
      43,    -1,   415,   416,    -1,   418,    -1,    -1,    -1,    -1,
      -1,   424,    -1,   426,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   412,    -1,    -1,   415,   416,
      -1,   418,    -1,    -1,    -1,    -1,    -1,   424,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   419,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   419,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,   419,    -1,    43,    35,    36,    37,
      38,    39,    40,   419,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   419,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   419,    -1,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   419,    -1,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   419,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   419,
      -1,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   419,
      -1,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   419,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   419,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   419,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   419,    -1,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   419,    -1,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   419,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   419,    -1,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   419,    -1,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   419,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   419,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   419,    -1,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   419,    -1,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   419,    -1,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   419,    -1,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   419,    -1,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   419,    -1,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   419,    -1,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   419,
      -1,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   419,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   419,
      -1,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   419,    -1,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    35,    36,    37,    38,    39,
      40,    -1,   419,    43,    35,    36,    37,    38,    39,    40,
      -1,   419,    43,    35,    36,    37,    38,    39,    40,    -1,
     419,    43,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      -1,    35,    36,    37,    38,    39,    40,    55,    56,    43,
      58,    59,   419,    61,    62,    63,    -1,    65,    66,    67,
      -1,   419,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     419,    -1,    35,    36,    37,    38,    39,    40,    -1,   419,
      43,    35,    36,    37,    38,    39,    40,    -1,   419,    43,
      35,    36,    37,    38,    39,    40,    -1,   419,    43,    35,
      36,    37,    38,    39,    40,    -1,   419,    43,    35,    36,
      37,    38,    39,    40,    -1,   419,    43,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,   419,    43,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,   419,    -1,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   419,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   419,    -1,    -1,    -1,   405,
      -1,    -1,    -1,    -1,   419,   401,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   398,    -1,    -1,    -1,   247,
      -1,    -1,    -1,   397,    35,    36,    37,    38,    39,    40,
      -1,   396,    43,    35,    36,    37,    38,    39,    40,   395,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   395,    -1,    35,    36,    37,    38,
      39,    40,    -1,   395,    43,    35,    36,    37,    38,    39,
      40,   394,    -1,    43,    35,    36,    37,    38,    39,    40,
     394,    -1,    43,    35,    36,    37,    38,    39,    40,   394,
      -1,    43,    35,    36,    37,    38,    39,    40,   394,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   393,    -1,    35,    36,
      37,    38,    39,    40,    -1,   393,    43,    35,    36,    37,
      38,    39,    40,    -1,   393,    43,    35,    36,    37,    38,
      39,    40,   392,    -1,    43,    35,    36,    37,    38,    39,
      40,   392,    -1,    43,    35,    36,    37,    38,    39,    40,
     392,    -1,    43,    -1,    -1,    -1,   370,    -1,    -1,   392,
      -1,    35,    36,    37,    38,    39,    40,    -1,   392,    43,
      -1,    -1,   400,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   369,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    35,    36,    37,    38,    39,    40,
      -1,   366,    43,    35,    36,    37,    38,    39,    40,   365,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   364,    35,    36,    37,    38,    39,    40,    -1,   363,
      43,    35,    36,    37,    38,    39,    40,   362,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   361,    35,    36,    37,    38,
      39,    40,    -1,   360,    43,    35,    36,    37,    38,    39,
      40,   359,    -1,    43,    -1,    -1,    -1,    -1,    -1,   358,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   356,    -1,
      35,    36,    37,    38,    39,    40,    -1,   355,    43,    35,
      36,    37,    38,    39,    40,   354,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,   353,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,   352,    -1,    43,    -1,    -1,    -1,    -1,    -1,   351,
      35,    36,    37,    38,    39,    40,    -1,   350,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     349,    35,    36,    37,    38,    39,    40,    -1,   348,    43,
      35,    36,    37,    38,    39,    40,   347,    -1,    43,    -1,
      -1,    -1,    -1,   345,    -1,    35,    36,    37,    38,    39,
      40,    -1,   345,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   339,    43,    35,    36,    37,    38,    39,    40,
     338,    -1,    43,    -1,    -1,    -1,    -1,    -1,   337,    35,
      36,    37,    38,    39,    40,    -1,   336,    43,    35,    36,
      37,    38,    39,    40,   335,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
     334,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   333,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   332,    35,    36,    37,    38,    39,    40,    -1,   331,
      43,    35,    36,    37,    38,    39,    40,   330,    -1,    43,
      35,    36,    37,    38,    39,    40,   329,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   328,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   327,    35,
      36,    37,    38,    39,    40,    -1,   326,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     325,    35,    36,    37,    38,    39,    40,    -1,   324,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   324,
      35,    36,    37,    38,    39,    40,    -1,   323,    43,    -1,
      -1,    -1,    -1,    -1,   321,    35,    36,    37,    38,    39,
      40,    -1,   320,    43,    35,    36,    37,    38,    39,    40,
     319,    -1,    43,    -1,    -1,    -1,    -1,    -1,   318,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   317,    35,    36,    37,    38,    39,    40,    -1,
     316,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   315,    35,    36,    37,    38,    39,    40,    -1,   314,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,   313,    -1,    35,    36,    37,    38,    39,
      40,    -1,   312,    43,    35,    36,    37,    38,    39,    40,
     311,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   310,    35,    36,    37,    38,    39,
      40,    -1,   309,    43,    35,    36,    37,    38,    39,    40,
     308,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   307,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   306,    35,    36,    37,    38,    39,    40,
      -1,   305,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   304,    43,    35,    36,    37,    38,    39,    40,   303,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   302,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   301,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   300,    35,    36,    37,    38,    39,
      40,    -1,   299,    43,    35,    36,    37,    38,    39,    40,
     294,    -1,    43,    -1,    -1,    -1,    -1,    -1,   293,    -1,
      35,    36,    37,    38,    39,    40,    -1,   292,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   291,    35,    36,    37,    38,    39,    40,    -1,   290,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,   289,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   289,    -1,    35,
      36,    37,    38,    39,    40,    -1,   289,    43,    35,    36,
      37,    38,    39,    40,    -1,   288,    43,    35,    36,    37,
      38,    39,    40,    -1,   288,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   288,    -1,
      35,    36,    37,    38,    39,    40,    -1,   288,    43,    35,
      36,    37,    38,    39,    40,   287,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   287,    -1,    35,
      36,    37,    38,    39,    40,    -1,   287,    43,    35,    36,
      37,    38,    39,    40,    -1,   287,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   287,    -1,    35,    36,
      37,    38,    39,    40,    -1,   286,    43,    35,    36,    37,
      38,    39,    40,    -1,   285,    43,    35,    36,    37,    38,
      39,    40,   284,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   284,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   284,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   282,    35,    36,    37,    38,    39,    40,    -1,
     281,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   281,    -1,    35,    36,
      37,    38,    39,    40,    -1,   281,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   281,    -1,
      35,    36,    37,    38,    39,    40,    -1,   281,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     281,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   281,    -1,    35,    36,    37,
      38,    39,    40,    -1,   281,    43,    35,    36,    37,    38,
      39,    40,    -1,   281,    43,    35,    36,    37,    38,    39,
      40,   280,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   279,    -1,    35,    36,    37,    38,
      39,    40,    -1,   279,    43,    35,    36,    37,    38,    39,
      40,    -1,   279,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   279,    -1,    35,    36,    37,    38,    39,
      40,    -1,   279,    43,    35,    36,    37,    38,    39,    40,
      -1,   279,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   279,    -1,    35,    36,    37,    38,    39,    40,
      -1,   279,    43,    35,    36,    37,    38,    39,    40,    -1,
     279,    43,    35,    36,    37,    38,    39,    40,    -1,   279,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   279,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,   278,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   278,    -1,    35,    36,
      37,    38,    39,    40,    -1,   278,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,   278,    -1,    35,    36,    37,    38,    39,    40,    -1,
     278,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   278,   258,    35,    36,    37,    38,    39,
      40,    -1,   278,    43,    -1,    -1,    -1,    -1,   256,    -1,
      35,    36,    37,    38,    39,    40,    -1,   256,    43,    35,
      36,    37,    38,    39,    40,    -1,   256,    43,    35,    36,
      37,    38,    39,    40,    -1,   256,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   256,    35,    36,
      37,    38,    39,    40,    -1,   255,    43,    -1,    -1,    -1,
      -1,    -1,   253,    35,    36,    37,    38,    39,    40,    -1,
     250,    43,   243,    -1,    -1,    -1,    -1,   239,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   239,    -1,
      35,    36,    37,    38,    39,    40,    -1,   239,    43,    35,
      36,    37,    38,    39,    40,    -1,   239,    43,    35,    36,
      37,    38,    39,    40,    -1,   239,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   237,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   236,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   235,    35,
      36,    37,    38,    39,    40,    -1,   234,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   231,
      35,    36,    37,    38,    39,    40,    -1,   230,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     230,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   230,    -1,    35,    36,    37,
      38,    39,    40,    -1,   230,    43,    35,    36,    37,    38,
      39,    40,   229,    -1,    43,    35,    36,    37,    38,    39,
      40,   229,    -1,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   229,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   229,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   228,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   228,    -1,    35,    36,    37,    38,    39,
      40,    -1,   228,    43,    35,    36,    37,    38,    39,    40,
      -1,   228,    43,    35,    36,    37,    38,    39,    40,    -1,
     227,    43,    35,    36,    37,    38,    39,    40,    -1,   227,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     227,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   227,    35,    36,    37,    38,    39,    40,    -1,   226,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   226,    35,    36,    37,    38,    39,    40,   224,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     224,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,   224,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   223,    -1,    35,    36,    37,    38,
      39,    40,    -1,   223,    43,    -1,    35,    36,    37,    38,
      39,    40,    -1,   223,    43,    35,    36,    37,    38,    39,
      40,   222,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   222,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   222,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   221,    -1,    35,    36,    37,    38,    39,    40,    -1,
     221,    43,    35,    36,    37,    38,    39,    40,    -1,   221,
      43,    -1,    -1,    -1,    -1,    -1,    -1,   220,    -1,    35,
      36,    37,    38,    39,    40,    -1,   220,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   220,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   219,    43,    35,    36,
      37,    38,    39,    40,    -1,   219,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   219,    35,    36,    37,
      38,    39,    40,    -1,   218,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   218,    43,    35,    36,    37,    38,
      39,    40,    -1,   218,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   217,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   217,    -1,
      35,    36,    37,    38,    39,    40,    -1,   217,    43,    35,
      36,    37,    38,    39,    40,   216,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   185,    -1,    35,    36,    37,    38,    39,    40,
      -1,   216,    43,    35,    36,    37,    38,    39,    40,   185,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,   185,
      43,    -1,    -1,   179,    -1,    35,    36,    37,    38,    39,
      40,    -1,   179,    43,    35,    36,    37,    38,    39,    40,
     178,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     178,    43,    35,    36,    37,    38,    39,    40,    -1,   177,
      43,    35,    36,    37,    38,    39,    40,    -1,   177,    43,
      35,    36,    37,    38,    39,    40,   176,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     176,    -1,   167,    -1,    35,    36,    37,    38,    39,    40,
      -1,   167,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   167,    -1,    35,    36,
      37,    38,    39,    40,    -1,   167,    43,    35,    36,    37,
      38,    39,    40,    -1,   167,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   166,    -1,    35,    36,
      37,    38,    39,    40,    -1,   166,    43,    35,    36,    37,
      38,    39,    40,    -1,   166,    43,    35,    36,    37,    38,
      39,    40,    -1,   166,    43,    35,    36,    37,    38,    39,
      40,    -1,   166,    43,    35,    36,    37,    38,    39,    40,
     165,    -1,    43,    35,    36,    37,    38,    39,    40,   165,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   165,    -1,    35,    36,    37,    38,
      39,    40,    -1,   165,    43,    35,    36,    37,    38,    39,
      40,    -1,   165,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,   160,    -1,    35,    36,    37,    38,    39,    40,
      -1,   160,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   159,    35,    36,    37,    38,    39,    40,   157,
      -1,    43,    35,    36,    37,    38,    39,    40,   157,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   156,    -1,    35,    36,
      37,    38,    39,    40,    -1,   156,    43,    35,    36,    37,
      38,    39,    40,    -1,   156,    43,    35,    36,    37,    38,
      39,    40,   155,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   155,   147,    -1,    35,
      36,    37,    38,    39,    40,    -1,   147,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   146,    43,    35,    36,    37,
      38,    39,    40,    -1,   146,    43,    35,    36,    37,    38,
      39,    40,    -1,   145,    43,    -1,    -1,    -1,   116,    -1,
      -1,   144,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   131,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,   131,    -1,    35,    36,    37,    38,    39,    40,
      -1,   131,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,   131,    -1,    35,    36,    37,
      38,    39,    40,    -1,   131,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   131,    43,    35,    36,    37,    38,
      39,    40,   130,    -1,    43,    35,    36,    37,    38,    39,
      40,   130,    -1,    43,    -1,    -1,    -1,    -1,    -1,   122,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   122,    -1,    35,    36,    37,    38,    39,    40,    -1,
     122,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   122,    -1,    35,    36,    37,    38,
      39,    40,    -1,   121,    43,    -1,    -1,   116,    -1,    35,
      36,    37,    38,    39,    40,    -1,   116,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   116,    -1,    35,    36,    37,    38,    39,    40,    -1,
     116,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   115,    -1,    35,    36,    37,    38,    39,
      40,    -1,   115,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   112,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   112,    -1,    35,    36,
      37,    38,    39,    40,    -1,   112,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   107,    35,    36,
      37,    38,    39,    40,    -1,   106,    43,    35,    36,    37,
      38,    39,    40,    -1,   105,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    99,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,    -1,   211,   212,   213,   214,
     215,   261,   262,   263,    -1,    -1,   266,   267,   268,   269,
     270,   271,    -1,   273,   274,   275,   276,   277
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     6,     8,     9,    10,    13,
      16,    18,    20,    27,    28,    29,    30,    31,    32,    33,
      36,    42,    44,    45,    46,    47,    50,    52,    54,    55,
      71,    77,    78,    79,    80,    95,    96,   113,   412,   415,
     416,   418,   424,   428,   429,   430,   433,   434,   435,   438,
     439,   440,    19,    21,    22,    23,    24,    25,    26,    34,
     418,   418,   418,   439,   439,     3,   439,   439,    55,    56,
      58,   375,   439,    53,    51,    55,    56,    58,    59,    61,
      62,    63,    65,    66,    67,    76,   188,   247,   400,   436,
     421,   412,    55,    55,    55,    56,    59,    61,    65,    59,
      81,    84,    86,    90,    93,   260,    88,    99,   439,    72,
     128,   126,    56,   412,    56,   412,   439,   424,     0,   440,
      35,    36,    37,    38,    39,    40,    43,   424,     3,   439,
     439,   439,   439,   439,   439,   439,   432,   439,   439,   439,
     425,   441,   441,   114,   114,   114,   439,     5,   114,   114,
     437,   114,   114,   114,   114,   114,   407,   128,   114,   114,
     187,   114,   116,   439,   417,   114,   114,   114,   114,   114,
     114,   407,   133,    82,    85,    87,    91,    92,    94,    88,
     168,   101,   130,    73,   129,   439,   412,   114,   412,   420,
     439,   439,   439,   439,   439,   439,   439,   419,   420,   420,
     420,   426,   440,   442,    12,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   111,   114,   439,   439,    65,
     439,    55,   422,   127,   439,   439,   439,   439,   439,   439,
      55,   134,   367,   111,   111,    99,    89,   169,   172,   173,
      81,    99,     5,   130,   439,   439,   442,   426,   441,   108,
     131,   131,   116,   116,   116,   116,   131,    55,   408,   439,
     131,   362,   116,    99,   114,   423,   116,   112,   112,   114,
     439,    34,   217,   371,    55,    81,   170,   171,   171,   174,
     103,    97,   100,   104,   412,   418,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   211,   212,   213,   214,
     215,   261,   262,   263,   266,   267,   268,   269,   270,   271,
     273,   274,   275,   276,   277,    55,    56,   109,    55,   109,
      55,    56,   388,   389,   390,   114,    55,   159,   244,   245,
     246,   248,    34,    55,    58,   439,     3,   439,   109,   117,
     431,   431,   439,   439,    34,    34,   114,    83,   225,   180,
     180,   181,   175,    34,    81,    81,    81,   439,    99,   111,
      99,   111,   111,   111,    99,   111,   111,   111,    99,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   278,   278,   278,   278,
     278,   346,   279,   344,   344,   278,   278,   278,   278,   278,
     114,   114,    57,   114,   119,   114,   114,   111,   111,   111,
     439,   114,    34,   256,   279,   279,   254,   439,   114,   114,
     401,   431,   366,   439,   439,   439,   368,   171,   182,   182,
     182,   180,   439,   102,   102,   419,   439,   119,   439,   119,
     119,   119,   439,   119,   119,   119,   439,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,   109,   439,
     439,   131,   439,   122,   439,   439,   391,   391,   391,   122,
     439,   439,    34,    34,    34,    34,   363,   439,   439,    34,
      34,   370,   122,    34,   180,   115,   115,   115,   182,    34,
      34,   439,   185,   418,   185,   418,   418,   418,   185,   418,
     418,   418,   185,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   131,   131,   131,   141,   122,   117,
      34,    34,    34,   117,   110,   144,   439,   439,   439,   439,
      34,   410,   116,   439,   439,    34,   117,   439,   182,    58,
      58,    58,   115,   439,   439,   419,   184,   439,   184,   439,
     439,   439,   184,   439,   439,   439,   184,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   279,   279,   324,   324,   293,   347,
     280,   279,   279,   360,   279,   279,   279,   279,   140,   136,
     137,   138,   140,   141,   141,   142,   115,   117,   102,   439,
     439,   439,   408,   409,    34,   115,   281,   226,   255,   439,
     117,    67,   402,   365,   439,   134,   369,   115,   114,   114,
     114,    58,   106,   439,   186,   419,   186,   419,   419,   419,
     186,   419,   419,   419,   186,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,   120,   120,   120,   115,
     115,   108,    76,   102,   145,    34,   392,   392,   392,   110,
      55,   439,    58,    34,    34,    34,   364,    65,   114,    34,
      34,   439,    34,    58,   439,   439,   439,   114,    34,   420,
     242,   439,   242,   439,   439,   439,   242,   439,   439,   439,
     242,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,    34,    34,    34,    76,    76,   119,   114,    34,
      34,   439,    34,    34,    34,    55,   114,   114,   439,   439,
     439,    34,   189,   439,   439,   439,   439,   114,   165,   165,
     165,   439,   439,   111,   111,   419,   111,   419,   419,   419,
     111,   419,   419,   419,   111,   419,   419,   420,   420,   420,
     420,   420,   419,   419,   419,   419,   420,   420,   419,   419,
     419,   420,   281,   281,   321,   325,   294,   348,   345,   345,
     281,   281,   281,   281,   281,   439,   439,   439,   114,   114,
     418,   439,   439,   439,   144,   439,   439,   439,   114,   439,
     439,   256,   229,   439,   411,   250,   403,   366,   439,    34,
      34,    34,   165,   105,   439,   119,   439,   119,   439,   439,
     439,   119,   439,   439,   439,   119,   439,   439,   111,   259,
     226,   226,   226,   439,   439,   439,   439,   115,   233,   439,
     439,   439,   226,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,   102,   160,   439,   439,
     439,   145,   155,    34,   393,   394,   394,   439,   410,    34,
      34,   439,    34,    34,    34,   165,   439,   439,   439,    34,
      34,   112,   418,   419,   418,   419,   419,   419,   418,   419,
     419,   419,   418,   419,   419,   439,    34,    34,    34,    34,
     419,   419,   420,   419,    58,    34,   419,   419,   419,    34,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,    34,    34,   419,    34,    34,   439,    34,
      34,    34,   122,   431,   439,   439,   439,   439,   439,    34,
     166,   166,   166,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   121,   439,
     439,   439,   439,   439,   439,   115,   439,   114,   439,   439,
     439,   439,   439,   301,   281,   290,   349,   392,   397,   352,
     282,   284,   284,   284,   439,     5,   439,   439,   439,   164,
     387,   439,   439,   117,   278,   230,   253,   404,   439,    34,
      34,    34,   166,   107,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   115,   258,   279,
     279,   279,   420,   420,    58,   420,   439,   234,   419,   419,
     419,   115,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,   160,   419,   155,   146,    34,   393,   395,
     409,    34,    34,    34,    34,   166,   439,   439,   439,    34,
      34,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,    67,    34,    34,    34,    34,   115,
     115,   114,   115,   226,    34,   439,   439,   439,    58,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
      34,   439,    34,    34,   439,    34,    34,   439,   439,   418,
     439,    34,   167,   167,   167,   439,   439,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     114,   439,   439,   439,   439,    58,    58,   439,    58,    34,
     439,   419,   419,   419,   114,   299,   323,   291,   373,   394,
     398,   353,   289,   289,   289,   285,     5,   419,   439,   439,
     156,   387,   439,   227,   439,   405,   439,    34,    34,    34,
     167,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   278,   227,   227,   227,   114,
     114,   256,   114,   439,   235,   439,   439,   439,   439,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
     439,   146,   157,    34,   393,    34,   419,    34,   167,   439,
     439,   439,    34,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   278,    34,    34,    34,
      34,   439,   439,    34,   439,   278,    34,   419,   419,   419,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   419,   420,    34,    34,   439,    34,   439,   439,   439,
      34,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,    34,   439,   439,   439,   439,
     256,   256,   439,   256,    34,   439,   439,   439,   439,   300,
     326,   292,   350,   395,   399,   354,   288,   288,   288,   286,
     439,   111,   439,   439,   387,   228,   420,   406,   439,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   439,   239,   228,   228,   228,    34,    34,    34,
     439,   236,   419,   419,   419,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,   419,   162,   163,   157,
     147,    34,    34,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   231,    34,    34,    34,
      34,   439,   439,   439,    34,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     418,   439,    34,    34,   439,   439,   419,   420,   419,   419,
     420,   419,   419,   419,   419,   420,   419,   420,   419,    34,
     418,   439,   439,   439,   439,   420,   420,   419,   302,   327,
     351,   396,   392,   355,   287,   287,   288,   419,   439,   439,
     439,   439,   115,   439,   439,   115,   439,   439,   439,   439,
     115,   439,   115,   439,   439,   439,   230,   230,   230,   237,
     115,   115,   439,    34,    34,    34,    34,    34,    34,    34,
      34,    34,   439,   419,   147,   156,   419,    58,   419,   419,
      58,   419,   419,   419,   419,    58,   419,    58,   419,   239,
     419,    34,    34,    34,    34,    58,    58,   419,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   419,   439,    34,
      34,   439,   114,   439,   439,   114,   439,   439,   439,   439,
     114,   439,   114,   439,    34,   439,   439,   439,   439,   439,
     114,   114,   439,   303,   328,   394,   356,   287,   439,   419,
     439,   439,   419,   439,   419,   419,   439,   419,   419,   419,
     419,   439,   419,   439,   419,   418,   419,   229,   229,   229,
     243,   439,   439,   419,    34,    34,    34,    34,    34,   419,
     420,   439,   156,   439,   439,   439,   216,   439,   439,   439,
     439,   216,   439,   439,   439,   439,    34,    34,    34,    34,
     439,   439,   439,   439,   439,   439,   439,   111,   419,    34,
     420,   419,   419,    34,   419,   419,   419,   419,    34,   419,
     419,   419,   420,   439,   439,   439,   418,   419,   304,   329,
     395,   287,   420,   162,   163,   439,   439,   115,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   240,
     278,   278,   278,   439,   439,    34,    34,    34,    34,   111,
     418,   439,   420,    58,   419,   419,   217,   419,   419,   419,
     419,   217,   419,   419,   419,    34,    34,    34,    34,   419,
     419,   439,   439,   439,   439,   162,   163,   439,   114,   439,
     439,    34,   439,   439,   439,   439,    34,   439,   439,   439,
     418,   439,   439,   439,   439,   439,   305,   330,   396,   358,
     418,   439,   419,   439,   419,   419,   439,   419,   419,   419,
     419,   439,   419,   419,   420,   439,   239,   239,   239,   419,
     419,    34,    34,    34,    34,   439,   439,   439,   439,   218,
     439,   439,   439,   439,   218,   439,   439,   240,   419,    34,
      34,    34,   439,   439,   439,   439,   439,   439,   419,   419,
     419,   419,    34,   419,   419,   419,   419,    34,   419,   419,
      34,   439,   418,   418,   418,   420,   419,   306,   331,   359,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   418,   419,   439,   439,   439,   439,    34,    34,
      34,   419,   419,   419,   419,   219,   419,   419,   419,   419,
     219,   419,   419,   439,   439,   419,   419,   419,   419,   439,
     439,   439,   439,   439,   439,   439,    34,   439,   439,   439,
     439,    34,   439,   439,   419,   420,   439,   439,   439,   439,
     307,   332,   361,   419,   419,   419,   419,   439,   419,   419,
     419,   419,   439,   419,   419,   439,   241,   419,   419,   419,
     419,    34,    34,    34,   439,   439,   439,   439,   220,   439,
     439,   439,   439,   220,   439,   439,   419,    34,   439,   439,
     439,   439,   439,   439,   439,   419,   419,   419,   419,    34,
     419,   419,   419,   419,    34,   419,   419,   439,   418,   420,
     420,   420,   419,   308,   333,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   420,   439,   240,
     240,   240,   439,    34,    34,   419,   419,   419,   419,   221,
     419,   419,   419,   419,   221,   419,   419,   241,   419,    34,
      34,    34,   419,   439,   439,   439,   439,   439,   439,    34,
     439,   439,   439,   439,    34,   439,   439,    34,   439,   418,
     418,   418,   439,   309,   334,   419,   419,   419,   419,   439,
     419,   419,   419,   419,   439,   419,   419,   418,   419,   439,
     439,   439,   419,    34,    34,   439,   439,   439,   439,   222,
     439,   439,   439,   439,   222,   439,   439,   439,   439,   419,
     419,   419,   439,   439,   439,   419,   420,   419,   419,    34,
     420,   419,   420,   420,    34,   419,   419,   419,   420,   439,
     439,   439,   419,   310,   335,   439,   439,   439,   439,   115,
     439,   115,   115,   439,   439,   439,   439,   419,   419,   419,
     439,    34,    34,   419,   419,   419,   223,    58,   419,    58,
      58,   223,   419,   419,   419,   439,   439,   439,   419,   439,
     439,   439,   439,   439,    34,   114,   439,   114,   114,    34,
     439,   439,   439,   420,   420,   420,   439,   311,   336,   420,
     419,   419,   439,   439,   419,   439,   439,   439,   419,   419,
     420,   241,   241,   241,   419,    34,    34,   439,   439,   224,
     216,   439,   224,   439,   439,    34,    34,    34,   439,   439,
     439,   419,   419,    34,    34,   420,    34,   419,   419,   418,
     418,   418,   419,   312,   337,   439,   439,   439,   439,   115,
     439,   439,   439,   439,   439,   439,   439,    34,    34,   419,
     419,   217,    58,   419,   419,   419,   419,   419,   419,   439,
     439,   439,   439,    34,   114,   439,   439,   439,   439,   439,
     439,   313,   338,   419,   419,   439,   439,   419,   419,   419,
     419,   419,   419,    34,    34,   439,   439,   218,   439,   439,
     439,   439,   439,   439,   439,   439,   419,   419,    34,   419,
     419,   420,   420,   420,   419,   314,   339,   439,   439,   439,
     439,   439,   439,    34,    34,   419,   420,   219,   419,   420,
     420,   439,   439,   439,   115,    34,   439,   115,   115,   315,
     287,   419,    58,   439,   419,    58,    58,    34,    34,   439,
     114,   220,   439,   114,   114,   439,   439,   419,   439,    34,
     419,   439,   439,   316,    81,   439,   439,   439,    34,    34,
     420,   221,   420,   439,   439,   115,    34,   115,   317,   176,
      58,   439,    58,    34,    34,   114,   222,   114,   439,   439,
     439,    34,   439,   318,   177,   439,    34,    34,   223,   439,
     439,    34,   319,   178,   439,    34,    34,   224,   439,   439,
      34,   320,   179,   439,    34,    34,   439,   439,    81,    34,
     439,   176,    34,   439,   177,    34,   439,   178,    34,   439,
     179,    34,   439
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   427,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   430,   430,   430,   430,   431,   431,   432,
     432,   433,   433,   433,   433,   433,   433,   433,   433,   433,
     433,   433,   433,   433,   433,   433,   434,   434,   434,   434,
     434,   434,   434,   434,   434,   434,   434,   434,   435,   435,
     435,   435,   435,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   436,   436,   436,   436,   436,   437,   437,
     437,   437,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   437,   438,   438,   438,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     441,   441,   442,   442
  };

  const unsigned char
  feiparser::yyr2_[] =
  {
       0,     2,     2,     2,     4,     4,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    15,     2,    13,
       3,    14,    13,    14,    24,    32,    32,    48,    34,    52,
      13,    16,    19,    10,    10,    31,    28,    21,    16,    19,
      22,     8,     8,    15,    24,    15,    17,     8,    11,    13,
      18,    24,    12,     6,     6,    14,     6,     1,     2,     1,
       3,     2,     2,     3,     4,     6,     3,     5,     5,    15,
       3,    12,     3,    10,     7,    11,     3,     4,     6,     9,
      18,     7,    22,    20,    20,    21,    20,     3,     4,     4,
       4,     4,     6,    14,    23,    26,    26,    29,    95,    80,
      23,    11,    26,    35,    26,    21,    14,    41,    27,    27,
      27,    18,    27,    33,    65,    65,    71,    65,    71,    51,
      57,    51,    54,    54,    78,    47,    59,    59,    51,    59,
      35,    22,    22,    20,    22,    21,     1,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     3,     4,
       4,     1,     4,     1,     1,     1,     1,     1,     1,     1,
       6,     1,     2,     2,     3,     5,     3,     1,     1,     2,
       2,     3,     1,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const feiparser::yytname_[] =
  {
  "$end", "error", "$undefined", "IDENT", "NUMBER", "STRING", "VECTOR",
  "EOL", "ISUNITTYPE", "FUNCTION", "IF", "THEN", "ELSE", "WHILE", "DO",
  "LET", "EXITSAFEMODE", "CONTINUE", "BYE", "IN", "REQUIRE", "PLUSEQUAL",
  "MINUSEQUAL", "TIMESEQUAL", "DIVIDEEQUAL", "MODULUSEQUAL", "POWEQUAL",
  "NUMBER_OF_NODES", "NUMBER_OF_ELEMENTS", "CURRENT_TIME",
  "NUMBER_OF_SP_CONSTRAINTS", "NUMBER_OF_MP_CONSTRAINTS",
  "NUMBER_OF_LOADS", "IS_PARALLEL", "'='", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'^'", "UMINUS", "'!'", "CMP", "CMD_print", "CMD_help",
  "CMD_whos", "CMD_check", "CMD_save", "CMD_restore", "MODEL", "NAME",
  "RESTART", "MESH", "ADD", "NODE", "ELEMENT", "ELEMENTS", "MATERIAL",
  "LOAD", "TIMEHISTORY", "IMPOSEDMOTION", "UNIFORMACCELERATION", "DAMPING",
  "DAMPINGTYPE", "CONSTRAINT", "DRM", "SECTION", "LOADPATTERN",
  "PENALTYDISPLACEMENT", "LOADVALUE", "SET", "REACTION", "FORCES",
  "ELEMENTNAME", "MATERIALNAME", "ACCELERATION_FIELD", "FIX", "FREE",
  "REMOVE", "DEFINE", "ALGORITHM", "ALGNAME", "CONSTITUTIVE_ALGNAME",
  "CONVERGENCE_TEST", "TESTNAME", "SOLVER", "SOLVERNAME", "CONSTITUTIVE",
  "INTEGRATION", "DYNAMICINTEGRATOR", "DYNAMICINTEGRATOR_HHT",
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
  "time_series_file", "MAGNITUDES", "MAGNITUDE", "initial_velocity",
  "strain_increment_size", "maximum_strain",
  "number_of_times_reaching_maximum_strain", "testing", "constant", "mean",
  "triaxial", "drained", "undrained", "simple", "shear",
  "number_of_subincrements", "maximum_number_of_iterations", "tolerance_1",
  "tolerance_2", "strain", "stress", "control", "Guass", "points", "Gauss",
  "each", "point", "single", "value", "EightNodeBrick",
  "TwentySevenNodeBrick", "EightNodeBrick_upU", "TwentyNodeBrick_uPU",
  "TwentyNodeBrick", "TwentyNodeBrickElastic", "EightNodeBrick_up",
  "variable_node_brick_8_to_27", "EightNodeBrickElastic",
  "TwentySevenNodeBrickElastic", "beam_displacement_based",
  "BeamColumnDispFiber3d", "beam_elastic", "beam_elastic_lumped_mass",
  "beam_9dof_elastic", "FourNodeShellMITC4", "FourNodeShellNewMITC4",
  "ThreeNodeShellANDES", "FourNodeShellANDES", "truss", "contact",
  "FrictionalPenaltyContact", "EightNodeBrickLT",
  "EightNodeBrickLTNoOutput", "TwentySevenNodeBrickLT", "ShearBeamLT",
  "porosity", "alpha", "rho_s", "rho_f", "k_x", "k_y", "k_z", "K_s", "K_f",
  "pressure", "cross_section", "shear_modulus", "torsion_Jx", "bending_Iz",
  "bending_Iy", "IntegrationRule", "stiffness", "normal_stiffness",
  "tangential_stiffness", "normal_damping", "tangential_damping",
  "friction_ratio", "maximum_gap", "xz_plane_vector", "joint_1_offset",
  "joint_2_offset", "direction", "contact_plane_vector",
  "MembranePlateFiber", "ElasticMembranePlate", "elastic3d", "FIBER",
  "FiberSection", "Section", "fiber_cross_section", "fiber_location_Y",
  "fiber_location_Z", "fiber_location", "maxNumPatches",
  "maxNumReinfLayers", "thickness", "integration_rule", "section_number",
  "number_of_integration_points", "NDMaterialLT",
  "linear_elastic_isotropic_3d", "linear_elastic_isotropic_3d_LT",
  "sanisand2008", "camclay", "camclay_accelerated", "sanisand2004",
  "linear_elastic_crossanisotropic", "uniaxial_concrete02",
  "uniaxial_elastic_1d", "uniaxial_steel01", "uniaxial_steel02", "pisano",
  "PisanoLT", "VonMisesLT", "DruckerPragerLT", "DruckerPragerVonMisesLT",
  "DruckerPragerArmstrongFrederickLT", "mass_density", "elastic_modulus",
  "viscoelastic_modulus", "poisson_ratio", "von_mises_radius",
  "druckerprager_angle", "druckerprager_k", "armstrong_frederick_ha",
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
  "ay", "az", "verbose_level", "maximum_iterations", "tolerance",
  "yield_function_relative_tolerance", "stress_relative_tolerance", "beta",
  "gamma", "kappa", "lambda", "delta", "DOMAIN_", "startTime", "endTime",
  "Period", "Phase", "Amplitude", "frequency", "MaxTime", "frequency1",
  "frequency2", "frequency3", "frequency4", "stiffness_to_use_opt",
  "DAMPING_RAYLEIGH", "DAMPING_CAUGHEY3", "DAMPING_CAUGHEY4", "a0", "a1",
  "stiffness_to_use", "a2", "a3", "a4", "R0", "cR1", "cR2", "MASS", "mx",
  "my", "mz", "Imx", "Imy", "Imz", "equaldof", "master", "slave",
  "dof_to_constrain", "of", "OUTPUT", "BINARY", "TEXT", "ENABLE",
  "DISABLE", "COMPRESSION", "'('", "','", "')'", "'['", "']'", "'.'",
  "';'", "'{'", "'}'", "$accept", "dsl", "CMD_add", "CMD_fix", "dofchain",
  "explist", "CMD_define", "CMD_misc", "CMD_remove", "ADD_material",
  "ADD_element", "prompt", "exp", "stmt", "cpd_stmt", "list", YY_NULLPTR
  };


  const unsigned short int
  feiparser::yyrline_[] =
  {
       0,   303,   303,   317,   327,   350,   363,   374,   387,   395,
     401,   402,   403,   404,   405,   409,   419,   423,   461,   465,
     478,   482,   500,   515,   533,   559,   588,   618,   655,   686,
     726,   746,   779,   815,   830,   845,   897,   946,   988,  1006,
    1025,  1044,  1060,  1076,  1094,  1115,  1152,  1169,  1186,  1205,
    1221,  1243,  1267,  1292,  1332,  1346,  1368,  1397,  1401,  1406,
    1412,  1423,  1432,  1439,  1446,  1454,  1464,  1473,  1486,  1499,
    1561,  1612,  1655,  1690,  1704,  1717,  1744,  1782,  1810,  1823,
    1839,  1862,  1876,  1900,  1924,  1948,  1972,  1997,  2013,  2026,
    2039,  2053,  2065,  2086,  2104,  2140,  2168,  2196,  2226,  2309,
    2383,  2408,  2423,  2452,  2487,  2520,  2547,  2566,  2617,  2643,
    2668,  2693,  2712,  2737,  2764,  2811,  2858,  2907,  2954,  3005,
    3045,  3087,  3127,  3174,  3212,  3270,  3324,  3375,  3426,  3479,
    3531,  3568,  3594,  3620,  3644,  3669,  3861,  3903,  3945,  3960,
    4005,  4012,  4019,  4026,  4033,  4040,  4047,  4053,  4060,  4068,
    4076,  4084,  4092,  4100,  4104,  4110,  4115,  4121,  4127,  4133,
    4139,  4145,  4153,  4158,  4164,  4169,  4174,  4179,  4184,  4189,
    4197,  4228,  4233,  4237,  4246,  4268,  4293,  4313,  4331,  4342,
    4352,  4358,  4366,  4370
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
       2,     2,     2,    42,     2,     2,     2,    39,     2,     2,
     418,   420,    37,    35,   419,    36,   423,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   424,
       2,    34,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   421,     2,   422,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   425,     2,   426,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    41,
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
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417
    };
    const unsigned int user_token_number_max_ = 664;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 7472 "feiparser.tab.cc" // lalr1.cc:1155
#line 4398 "feiparser.yy" // lalr1.cc:1156


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
	if (dof.compare("ux") == 0) return new Number(1, ESSIunits::unitless);
	if (dof.compare("uy") == 0) return new Number(2, ESSIunits::unitless);
	if (dof.compare("uz") == 0) return new Number(3, ESSIunits::unitless);
	if (dof.compare("rx") == 0) return new Number(4, ESSIunits::unitless);
	if (dof.compare("ry") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("rz") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("Ux") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("Uy") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("Uz") == 0) return new Number(7, ESSIunits::unitless);
	if (dof.compare("p")  == 0) return new Number(4, ESSIunits::unitless);

	return new Number(-100, ESSIunits::unitless);
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
	if (dof.compare("Fx") == 0) return new Number(1, ESSIunits::unitless);
	if (dof.compare("Fy") == 0) return new Number(2, ESSIunits::unitless);
	if (dof.compare("Fz") == 0) return new Number(3, ESSIunits::unitless);
	if (dof.compare("Mx") == 0) return new Number(4, ESSIunits::unitless);
	if (dof.compare("My") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("Mz") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("F_fluid_x") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("F_fluid_y") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("F_fluid_z") == 0) return new Number(7, ESSIunits::unitless);

	return new Number(-100, ESSIunits::unitless);
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

		*outputcppfile << "#include \"ESSI_API.h\"" << endl ;
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



