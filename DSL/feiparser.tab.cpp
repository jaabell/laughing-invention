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
	int bardet_type_parse(string type);

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


#line 138 "feiparser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "feiparser.tab.hh"

// User implementation prologue.
#line 283 "feiparser.yy" // lalr1.cc:407

extern int yylex(yy::feiparser::semantic_type *yylval,
				 yy::feiparser::location_type *yylloc);

#line 156 "feiparser.tab.cc" // lalr1.cc:407


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
#line 242 "feiparser.tab.cc" // lalr1.cc:474

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
    #line 288 "feiparser.yy" // lalr1.cc:725
{
	yyla.location.begin.filename = yyla.location.end.filename = new std::string(curfilename);
}

#line 545 "feiparser.tab.cc" // lalr1.cc:725

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
#line 308 "feiparser.yy" // lalr1.cc:847
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
#line 668 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 322 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "output_of_element_to_screen");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 682 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 335 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_node_to_screen, args, signature, "output_of_node_to_screen");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 696 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 348 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("filename", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<string>(&check_mesh, args, signature,"check_mesh");

		nodes.pop();
		nodes.push((yylhs.value.exp));
		}
#line 710 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 361 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 719 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 369 "feiparser.yy" // lalr1.cc:847
    {
		cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 729 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 374 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 735 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 375 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 376 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 747 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 377 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 753 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 378 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 759 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 382 "feiparser.yy" // lalr1.cc:847
    {
									cout << "    *** Commands will now be execued. " << endl << endl;
									recovery_mode = 0;
									prompt = thePrompt;
									(yylhs.value.exp) = new Empty();
									nodes.push((yylhs.value.exp));
								}
#line 771 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 392 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 777 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 397 "feiparser.yy" // lalr1.cc:847
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
#line 819 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 434 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 825 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 439 "feiparser.yy" // lalr1.cc:847
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
#line 842 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 451 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 848 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 457 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 866 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 474 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[9].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_selfweight_to_all_elements, args, signature, "add_load_selfweight_to_all_elements");

		for(int ii = 1;ii <=2; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 882 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 490 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_node, args, signature, "add_load_selfweight_to_node");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 900 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 509 "feiparser.yy" // lalr1.cc:847
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
#line 925 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 535 "feiparser.yy" // lalr1.cc:847
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
#line 953 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 564 "feiparser.yy" // lalr1.cc:847
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
#line 982 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 594 "feiparser.yy" // lalr1.cc:847
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
#line 1018 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 631 "feiparser.yy" // lalr1.cc:847
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
#line 1048 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 662 "feiparser.yy" // lalr1.cc:847
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
#line 1087 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 700 "feiparser.yy" // lalr1.cc:847
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
#line 1108 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 722 "feiparser.yy" // lalr1.cc:847
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
#line 1140 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 757 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		Expression* dof_number                         = force2number(*(yystack_[23].value.ident));
		UnitCheckerFunctionPointerType function_ptr    = force2signature(*(yystack_[23].value.ident));

		args.push_back((yystack_[30].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[26].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
		args.push_back((yystack_[18].value.exp));        signature.push_back(this_signature("time_step", &isTime));
		args.push_back((yystack_[21].value.exp));        signature.push_back(this_signature(*(yystack_[23].value.ident), function_ptr));

		//Get the string from the parsed string
		string modelname = *(yystack_[15].value.ident);
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
#line 1174 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 790 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[2].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 1190 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 805 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back(new Number(-1, ESSIunits::unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		nodes.pop(); //pop 1 exps
		nodes.push((yylhs.value.exp));
	}
#line 1206 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 827 "feiparser.yy" // lalr1.cc:847
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
#line 1252 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 878 "feiparser.yy" // lalr1.cc:847
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
#line 1296 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 925 "feiparser.yy" // lalr1.cc:847
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
#line 1335 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 963 "feiparser.yy" // lalr1.cc:847
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
#line 1353 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 981 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("number",   &isAdimensional));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a0",      &isFrequency));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a1",      &isTime));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a2",      &isThisUnit< 0, 0, 3>));
		args.push_back(new FeiString(*(yystack_[0].value.ident))); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller5<int,double,double,double,string>(&add_damping_caughey3rd, args, signature, "add_damping_caughey3rd");

		for(int i = 1; i <= 4; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1372 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 1000 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("number",   &isAdimensional));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("a0",      &isFrequency));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("a1",      &isTime));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("a2",      &isThisUnit< 0, 0, 3>));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("a3",      &isThisUnit< 0, 0, 5>));
		args.push_back(new FeiString(*(yystack_[0].value.ident))); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller6<int,double,double,double,double,string>(&add_damping_caughey4th, args, signature, "add_damping_caughey4th");

		for(int i = 1; i <= 5; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1392 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 1019 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_element(int elementNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("element_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1409 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 1035 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_node(int nodeNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("node_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1426 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 1051 "feiparser.yy" // lalr1.cc:847
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
#line 1445 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 1069 "feiparser.yy" // lalr1.cc:847
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
#line 1467 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 1090 "feiparser.yy" // lalr1.cc:847
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
#line 1505 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 1127 "feiparser.yy" // lalr1.cc:847
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
#line 1523 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 1146 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,string>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1539 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 1164 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("scale_factor",            &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, string, double>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1556 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 1180 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("section_number",           &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material_number",           &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("thickness",           &isLength));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1573 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 1200 "feiparser.yy" // lalr1.cc:847
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
#line 1592 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 1224 "feiparser.yy" // lalr1.cc:847
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
#line 1610 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 1243 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("section_number",          &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("TorsionConstant_GJ",           &isThisUnit< 1, 3,-2>));

		(yylhs.value.exp) = new FeiDslCaller2<int, double>(&add_section_to_model, args, signature, "add_section_to_model");
		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1624 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 1265 "feiparser.yy" // lalr1.cc:847
    {
		// This implementation is a bit uncommon. The need arises due to the unknown
		// number of fixed dofs the user will specify. Also since the add_support_to_node
		// and related DSLs take only two parameters, one command will generate
		// multiple calls to the DSL. The way to do this is by using ExpressionList
		// which is a linked list of expressions.
		Expression* cmd;
		Expression* command_list = 0;
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
#line 1665 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 1305 "feiparser.yy" // lalr1.cc:847
    {
		args.clear();           signature.clear();
		args.push_back((yystack_[2].value.exp));     signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1680 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 1319 "feiparser.yy" // lalr1.cc:847
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
#line 1703 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 1341 "feiparser.yy" // lalr1.cc:847
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
#line 1732 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 1369 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.clear();
							dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
						}
#line 1741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1373 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
						}
#line 1749 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1378 "feiparser.yy" // lalr1.cc:847
    {
							(yylhs.value.exp) = new FeiVector();
							dynamic_cast<FeiVector*>((yylhs.value.exp))->add_component((yystack_[0].value.exp)->value());
							nodes.pop();
							nodes.push((yylhs.value.exp));
						}
#line 1760 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1384 "feiparser.yy" // lalr1.cc:847
    {
							dynamic_cast<FeiVector*>((yystack_[2].value.exp))->add_component((yystack_[0].value.exp)->value());
							(yylhs.value.exp) = (yystack_[2].value.exp);
							nodes.pop();
						}
#line 1770 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1396 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1780 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1405 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1790 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1412 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1800 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1419 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
		nodes.push((yylhs.value.exp));
	}
#line 1811 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1427 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("level", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
		nodes.push((yylhs.value.exp));
	}
#line 1822 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1437 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("factor", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1836 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 1453 "feiparser.yy" // lalr1.cc:847
    {

		string fname;       // name of the DSL called to report
		args.clear(); signature.clear();
		
		//Read the string and turn into lower-case
		string algname(*(yystack_[9].value.ident));
		//std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		int method = -1;
		bool good = false;

		if( algname.compare("Forward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
			good = true;
		}
		// make essi back compatible. Keep the old DSLs
		if( algname.compare("Euler_One_Step") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
			good = true;
		}
		if( algname.compare("Forward_Euler_Crisfield") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler_Crisfield;
			good = true;
		}
		if( algname.compare("Multistep_Forward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler;
			good = true;
		}
		if( algname.compare("Multistep_Forward_Euler_Crisfield") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler_Crisfield;
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
		
		if( algname.compare("Full_Backward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Full_Backward_Euler;
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
#line 1918 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 1534 "feiparser.yy" // lalr1.cc:847
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
		else if( algname.compare("newtonlinesearch") == 0)
		{
			f = &define_algorithm_newtonlinesearch_for_analysis;
			fname = "define_algorithm_newtonlinesearch_for_analysis";
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
#line 1975 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 1590 "feiparser.yy" // lalr1.cc:847
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


		if((yystack_[9].value.ident)->compare("Relative_Norm_Displacement_Increment") == 0)
		{
			f = &define_convergence_test_RelativeNormdisplacementincrement_for_analysis;
			fname = "define_convergence_test_RelativeNormdisplacementincrement_for_analysis";
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if((yystack_[9].value.ident)->compare("Relative_Energy_Increment") == 0)
		{
			f = &define_convergence_test_RelativeEnergyincrement_for_analysis;
			fname = "define_convergence_test_RelativeEnergyincrement_for_analysis";
			args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if((yystack_[9].value.ident)->compare("Relative_Norm_Unbalance") == 0)
		{
			f = &define_convergence_test_RelativeNormunbalance_for_analysis;
			fname = "define_convergence_test_RelativeNormunbalance_for_analysis";
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
#line 2051 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 1665 "feiparser.yy" // lalr1.cc:847
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
#line 2087 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 1700 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("gamma", &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 2102 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 1714 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

		nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 2116 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 1727 "feiparser.yy" // lalr1.cc:847
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
#line 2138 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 1754 "feiparser.yy" // lalr1.cc:847
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
#line 2177 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 1792 "feiparser.yy" // lalr1.cc:847
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
#line 2206 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 1820 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2220 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 1833 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("time_step", &isTime));
		args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

		nodes.pop();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2237 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 1853 "feiparser.yy" // lalr1.cc:847
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
#line 2257 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1872 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(this_signature("non_converged_iterations", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller0<>(&save_non_converged_iterations, args, signature, "save_non_converged_iterations");
	}
#line 2268 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1882 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back
		(this_signature("number_of_modes", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2283 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 1899 "feiparser.yy" // lalr1.cc:847
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
#line 2305 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 1923 "feiparser.yy" // lalr1.cc:847
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
#line 2327 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 1947 "feiparser.yy" // lalr1.cc:847
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
#line 2349 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 1971 "feiparser.yy" // lalr1.cc:847
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
#line 2371 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 1995 "feiparser.yy" // lalr1.cc:847
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
#line 2393 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 2020 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		//int simulate_constitutive_testing_BardetLT_path(int MaterialNumber, int type, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, )

		//Get the string from the parsed string
		string filename = *(yystack_[18].value.ident);
		//Remove quotes
		filename.erase(0, 1);
		filename.erase(filename.length()-1, filename.length());

		//Parse BARDETMETHOD
		int btype = bardet_type_parse(*(yystack_[28].value.ident));
		Expression* type = new Number( btype, ESSIunits::unitless);

		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("MaterialNumber", &isAdimensional));
		args.push_back(type); signature.push_back(this_signature("type", &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("scale_factor", &isAdimensional));
		args.push_back( new FeiString(filename)); signature.push_back(this_signature("series_file", &isAdimensional));
		args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("sxx0", &isPressure));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("syy0", &isPressure));
		args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("szz0", &isPressure));
		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("sxy0", &isPressure));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("sxz0", &isPressure));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("syz0", &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("verbose_out", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller11<int, int, double, std::string, 
		   	double, double, double, double, double, double, int>(&simulate_constitutive_testing_BardetLT_path, args, signature, "simulate_constitutive_testing_BardetLT_path");

		for(int ii = 1;ii <=9; ii++) nodes.pop();

		nodes.push((yylhs.value.exp));
	}
#line 2432 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 2062 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		//int simulate_constitutive_testing_DirectStrain_path(int MaterialNumber, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, int verbose_output)

		//Get the string from the parsed string
		string filename = *(yystack_[18].value.ident);
		//Remove quotes
		filename.erase(0, 1);
		filename.erase(filename.length()-1, filename.length());


		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("MaterialNumber", &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("scale_factor", &isAdimensional));
		args.push_back( new FeiString(filename)); signature.push_back(this_signature("series_file", &isAdimensional));
		args.push_back((yystack_[14].value.exp)); signature.push_back(this_signature("sxx0", &isPressure));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("syy0", &isPressure));
		args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("szz0", &isPressure));
		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("sxy0", &isPressure));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("sxz0", &isPressure));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("syz0", &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("verbose_out", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller10<int,  double, std::string, 
		   	double, double, double, double, double, double, int>(&	simulate_constitutive_testing_DirectStrain_path, args, signature, "	simulate_constitutive_testing_DirectStrain_path");

		for(int ii = 1;ii <=9; ii++) nodes.pop();

		nodes.push((yylhs.value.exp));
	}
#line 2467 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 2096 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&run_test, args, signature, "run_test");
		nodes.push((yylhs.value.exp));
	}
#line 2477 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 2106 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		(yylhs.value.exp) = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

	}
#line 2488 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2122 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2502 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2135 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2516 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2148 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

		//pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2531 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2162 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
			 //pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2544 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2174 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2557 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2186 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_displacement_from_node, args, signature, "remove_displacement_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2570 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2198 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_strain_from_element, args, signature, "remove_strain_from_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2583 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2222 "feiparser.yy" // lalr1.cc:847
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
#line 2599 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2243 "feiparser.yy" // lalr1.cc:847
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
#line 2630 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2280 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[23].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));  // 5
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 7
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 8


		(yylhs.value.exp) = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_vonMises_ArmstrongFrederick, args, signature, "add_constitutive_model_NDMaterialLT_vonMises_ArmstrongFrederick");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2652 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2308 "feiparser.yy" // lalr1.cc:847
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
#line 2674 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2336 "feiparser.yy" // lalr1.cc:847
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
#line 2696 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2366 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",            &isAdimensional)); // 1
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 8
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("plastic_flow_xi",   &isAdimensional));  // 9
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("plastic_flow_kd",   &isAdimensional));  // 10


		(yylhs.value.exp) = new FeiDslCaller10<int, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_non_associate_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_non_associate_linear_hardening");
		for(int ii = 1;ii <=10; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2720 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2400 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
//		add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne(tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double K_in, double pa_in, double n_in, double sigma3_max_in, double nu_in, double rho_, double p0)
		args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number"                    , &isAdimensional));    // 1
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density"              , &isDensity));  // 2
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("DuncanCheng_K"            , &isAdimensional));  // 3
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("DuncanCheng_pa"           , &isPressure));  // 3
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("DuncanCheng_n"            , &isAdimensional));  // 3
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("DuncanCheng_sigma3_max"   , &isPressure));  // 3
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("DuncanCheng_nu"           , &isAdimensional));  // 3
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("druckerprager_k"          , &isAdimensional));  // 5
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha"   , &isPressure));  // 6
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr"   , &isPressure));  // 6
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate" , &isPressure));  // 7
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress" , &isPressure));  // 7


		(yylhs.value.exp) = new FeiDslCaller12<int, double, double, double, double, 
		double, double, double, double, 
		double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne");
		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2749 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2436 "feiparser.yy" // lalr1.cc:847
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


		(yylhs.value.exp) = new FeiDslCaller9<int, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_le, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_le");
		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2772 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2468 "feiparser.yy" // lalr1.cc:847
    {

		args.clear(); signature.clear();
		args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 7
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 8
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 9
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("plastic_flow_xi",   &isAdimensional));  // 10
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("plastic_flow_kd",   &isAdimensional));  // 11

		(yylhs.value.exp) = new FeiDslCaller11<int, double, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_non_associate_armstrong_frederick, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_non_associate_armstrong_frederick");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2796 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2499 "feiparser.yy" // lalr1.cc:847
    {
	//add_constitutive_model_NDMaterialLT_camclay(int MaterialNumber,
	//        double rho_,
	//        double M_,
	//        double lambda_,
	//        double kappa_,
	//        double e0_,
	//        double p0_,
	//        double nu_,
	//        double initial_confinement)
		args.clear(); signature.clear();
		args.push_back((yystack_[26].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("CriticalState_M",  &isAdimensional));  // 3
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("CriticalState_lambda",    &isAdimensional));  // 4
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("CriticalState_kappa",  &isAdimensional));  // 5
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("CriticalState_e0",   &isAdimensional));  // 6
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("CriticalState_p0",   &isPressure));  // 7
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("poisson_ratio",   &isAdimensional));  // 8
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 9

		(yylhs.value.exp) = new FeiDslCaller9<int, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_camclay, args, signature, "add_constitutive_model_NDMaterialLT_camclay");
		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2826 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2538 "feiparser.yy" // lalr1.cc:847
    {
		//add_constitutive_model_NDMaterialLT_roundedMohrCoulomb(int tag_in, 
		//double rho_in, double E_in, double nu_in, double m_in, double qa_in, 
		//double pc_in, double e_in, double H_k, double eta0_in, double p0)
		args.clear(); signature.clear();
		args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));    
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));  
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));  
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("RMC_m",  			&isAdimensional));  
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("RMC_qa",  			&isPressure));  
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("RMC_pc",  			&isPressure));  
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("RMC_e",   			&isAdimensional));  
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("RMC_eta0",   		&isAdimensional));  
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("RMC_Heta",   		&isPressure));  
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  

		(yylhs.value.exp) = new FeiDslCaller11<int, double, double, double, double, double, 
									 double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_roundedMohrCoulomb, args, signature, "add_constitutive_model_NDMaterialLT_roundedMohrCoulomb");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2853 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2594 "feiparser.yy" // lalr1.cc:847
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
#line 2907 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2672 "feiparser.yy" // lalr1.cc:847
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
#line 2957 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2727 "feiparser.yy" // lalr1.cc:847
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
#line 2977 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2746 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		(yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push((yylhs.value.exp));
	}
#line 2993 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 2768 "feiparser.yy" // lalr1.cc:847
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
#line 3016 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 2800 "feiparser.yy" // lalr1.cc:847
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
#line 3042 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 2832 "feiparser.yy" // lalr1.cc:847
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
#line 3065 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 2860 "feiparser.yy" // lalr1.cc:847
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
#line 3086 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 2888 "feiparser.yy" // lalr1.cc:847
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
#line 3104 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 2922 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[53].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
		args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
		args.push_back((yystack_[45].value.exp)); signature.push_back(this_signature("Niso3d_K",      &isAdimensional));
		args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("Niso3d_Kur",      &isAdimensional));
		args.push_back((yystack_[39].value.exp)); signature.push_back(this_signature("Niso3d_n",      &isAdimensional));
		args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("Niso3d_c",      &isAdimensional));
		args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("Niso3d_phi0",      &isAdimensional));
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("Niso3d_dphi",      &isAdimensional));
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("Niso3d_Rf",      &isAdimensional));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("Niso3d_K0",      &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("Niso3d_Kb",      &isAdimensional));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("Niso3d_m",      &isAdimensional));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("Niso3d_pa",      &isPressure));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("Niso3d_K2",      &isAdimensional));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("Niso3d_B",      &isAdimensional));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("Niso3d_Et",      &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("Niso3d_Ei",      &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("Niso3d_Er",      &isAdimensional));

	//tag, rho, K, Kur, n, c, phi0, dphi, Rf, K0, Kb, m, pa, K2, B, Et, Ei, Er
        
		(yylhs.value.exp) = new FeiDslCaller18<int, double, double,
        double, double, double, double, double,
        double, double, double, double, double,
        double, double, double, double, double>(&add_constitutive_model_NDMaterial_nonlinear_elastic_isotropic_3d, args,
			signature,"add_constitutive_model_NDMaterial_nonlinear_elastic_isotropic_3d");
		for(int ii = 1;ii <=18; ii++) nodes.pop(); //pop 4 exps
		nodes.push((yylhs.value.exp));
	}
#line 3140 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 2969 "feiparser.yy" // lalr1.cc:847
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
#line 3164 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 2994 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[16].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[10].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller5<int,int,int,double,int>(&add_element_shear_beam, args, signature, "add_element_shear_beam");

		for(int ii = 1;ii <=5; ii++) nodes.pop(); //pop 5 exps
		nodes.push((yylhs.value.exp));
	}
#line 3182 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3013 "feiparser.yy" // lalr1.cc:847
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
#line 3208 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3042 "feiparser.yy" // lalr1.cc:847
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
#line 3252 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3089 "feiparser.yy" // lalr1.cc:847
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
#line 3297 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3137 "feiparser.yy" // lalr1.cc:847
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
#line 3344 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3188 "feiparser.yy" // lalr1.cc:847
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
#line 3381 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3228 "feiparser.yy" // lalr1.cc:847
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
#line 3419 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3277 "feiparser.yy" // lalr1.cc:847
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
#line 3455 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3324 "feiparser.yy" // lalr1.cc:847
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
								double,double,double,double,double>(&add_element_brick_8node_upULT, args, signature, "add_element_brick_8node_upULT");

		for(int ii = 1;ii <=19; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3491 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3364 "feiparser.yy" // lalr1.cc:847
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
#line 3525 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3403 "feiparser.yy" // lalr1.cc:847
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
#line 3574 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3457 "feiparser.yy" // lalr1.cc:847
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
								double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_upULT, args, signature, "add_element_brick_20node_upULT");

		for(int ii = 1;ii <=31; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3623 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3511 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[90].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[84].value.exp)); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back((yystack_[82].value.exp)); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back((yystack_[80].value.exp)); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back((yystack_[78].value.exp)); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back((yystack_[76].value.exp)); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back((yystack_[74].value.exp)); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back((yystack_[72].value.exp)); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back((yystack_[70].value.exp)); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back((yystack_[68].value.exp)); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back((yystack_[66].value.exp)); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back((yystack_[64].value.exp)); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back((yystack_[62].value.exp)); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back((yystack_[60].value.exp)); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back((yystack_[58].value.exp)); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back((yystack_[56].value.exp)); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back((yystack_[54].value.exp)); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back((yystack_[52].value.exp)); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back((yystack_[50].value.exp)); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back((yystack_[48].value.exp)); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back((yystack_[46].value.exp)); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back((yystack_[44].value.exp)); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back((yystack_[42].value.exp)); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back((yystack_[40].value.exp)); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back((yystack_[38].value.exp)); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back((yystack_[34].value.exp)); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back((yystack_[32].value.exp)); signature.push_back(this_signature("node27", &isAdimensional));

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

		(yylhs.value.exp) = new FeiDslCaller38<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,
								int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_27node_upULT, args, signature, "add_element_brick_27node_upULT");

		for(int ii = 1;ii <=38; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3680 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3574 "feiparser.yy" // lalr1.cc:847
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
#line 3716 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3632 "feiparser.yy" // lalr1.cc:847
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
#line 3757 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3683 "feiparser.yy" // lalr1.cc:847
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
#line 3798 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3731 "feiparser.yy" // lalr1.cc:847
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
#line 3844 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 3787 "feiparser.yy" // lalr1.cc:847
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
#line 3886 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 3835 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[33].value.exp)); signature.push_back(this_signature("number",                  &isAdimensional));

		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("node1",                   &isAdimensional));
		args.push_back((yystack_[25].value.exp)); signature.push_back(this_signature("node2",                  &isAdimensional));

		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("normal_stiffness",       &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("tangential_stiffness",   &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("normal_damping",         &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("tangential_damping",     &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("friction_ratio",         &isAdimensional));

		args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("x_local_1",              &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("x_local_2",              &isAdimensional));
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("x_local_3",              &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller11<int, int, int,
							   double, double, double, double, double,
							   double, double, double>(&add_element_hard_contact, args, signature, "add_element_hard_contact");

		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3917 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 3872 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[36].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));

		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("node1",                     &isAdimensional));
		args.push_back((yystack_[28].value.exp)); signature.push_back(this_signature("node2",                    &isAdimensional));
 
 		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("initial_normal_stiffness", &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("stiffning_rate",           &isThisUnit<0, -1, 0>));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("tangential_stiffness",     &isThisUnit<1, 0, -2>));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("normal_damping",           &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("tangential_damping",       &isThisUnit<1, 0, -1>));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("friction_ratio",           &isAdimensional));
  
		args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("x_local_1",                &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("x_local_2",                &isAdimensional));
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("x_local_3",                &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller12<int, int, int, double, double, double, double, double, double,
				   double, double, double>(&add_element_soft_contact, args, signature, "add_element_soft_contact");

		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 3949 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 3906 "feiparser.yy" // lalr1.cc:847
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
#line 3973 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 3932 "feiparser.yy" // lalr1.cc:847
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
#line 3997 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 3958 "feiparser.yy" // lalr1.cc:847
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
#line 4019 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 3982 "feiparser.yy" // lalr1.cc:847
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
#line 4042 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4008 "feiparser.yy" // lalr1.cc:847
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
#line 4062 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4196 "feiparser.yy" // lalr1.cc:847
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
#line 4108 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4238 "feiparser.yy" // lalr1.cc:847
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
#line 4154 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4280 "feiparser.yy" // lalr1.cc:847
    {
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		(yylhs.value.errcode) = 0;
	}
#line 4170 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4295 "feiparser.yy" // lalr1.cc:847
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
#line 4219 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4340 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4230 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4347 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4241 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4354 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4252 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4361 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4263 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4368 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4274 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4375 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4285 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4382 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.push ((yylhs.value.exp));
	}
#line 4295 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4388 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4306 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4395 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4318 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4403 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4330 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4411 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4342 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4419 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4354 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4427 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4366 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4435 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4374 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4439 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4384 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4445 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Number (*(yystack_[0].value.value));
		nodes.push ((yylhs.value.exp));
	}
#line 4393 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4450 "feiparser.yy" // lalr1.cc:847
    {

		(yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 4403 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4456 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4413 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4462 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

		nodes.push((yylhs.value.exp));
	}
#line 4423 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4468 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4433 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4474 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4443 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4480 "feiparser.yy" // lalr1.cc:847
    {
		string temp = *(yystack_[0].value.ident);
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		(yylhs.value.exp) = new FeiString(temp);
		nodes.push((yylhs.value.exp));
	}
#line 4455 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4488 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);//new FeiVector($3);
		//nodes.push($$);
	}
#line 4464 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4493 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push((yylhs.value.exp));
	}
#line 4474 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4499 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push((yylhs.value.exp));
	}
#line 4483 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4504 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push((yylhs.value.exp));
	}
#line 4492 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4509 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4501 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4514 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4510 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4519 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push((yylhs.value.exp));
	}
#line 4519 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4524 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push((yylhs.value.exp));
	}
#line 4528 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4532 "feiparser.yy" // lalr1.cc:847
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
#line 4560 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4569 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4569 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4574 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4577 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4578 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
		args.clear();
		signature.clear();
	}
#line 4587 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4588 "feiparser.yy" // lalr1.cc:847
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
#line 4610 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4610 "feiparser.yy" // lalr1.cc:847
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
#line 4636 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4635 "feiparser.yy" // lalr1.cc:847
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
#line 4660 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4655 "feiparser.yy" // lalr1.cc:847
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
#line 4682 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4673 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		(yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push((yylhs.value.exp));
	}
#line 4697 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4684 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4707 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4694 "feiparser.yy" // lalr1.cc:847
    {
		// empty statement generates an  empty expression
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4717 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 187:
#line 4700 "feiparser.yy" // lalr1.cc:847
    {
		// Used to initialize a list of statements (compound statement)
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4726 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 188:
#line 4708 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[0].value.exp);
	}
#line 4734 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 189:
#line 4712 "feiparser.yy" // lalr1.cc:847
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
#line 4750 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4754 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -451;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
    2637,  -451,  1439,  -451,  -451,  -450,  -409,  -390,   568,   568,
    -451,  -451,    82,  -451,  -451,  -451,  -451,  -451,  -451,  -451,
     568,   568,   286,  -451,  -451,    39,    55,  -451,  5810,  -363,
    -341,    80,    98,   128,   270,    66,    71,  -451,    34,    58,
     -47,  -298,  -284,   568,  -451,  -287,  -451,  -451,  -451,  -451,
    -451,   191,   -19,  -451,   195,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   -32,   -32,  -451,   160,   160,
    -114,    94,  9641,   568,   210,   110,   115,   118,   126,   144,
     147,   151,  -216,   153,   166,   169,    95,   177,   183,  -451,
     568,  -167,   188,   189,   192,   193,   194,   204,  -153,   184,
     196,   185,   241,   239,   248,    47,   231,   250,   155,   249,
    9361,   267,   211,   568,  -111,  -451,  -451,  -451,    91,  -451,
    -451,  -451,   568,   568,   568,   568,   568,   568,   568,  -451,
    -451,  9641,  9641,  9641,  9641,  9641,  9641,  9641,  -325,  9641,
     133,   142,  2693,   344,  -451,   568,   568,  9641,  -451,   568,
     568,  -451,   568,   568,   568,   568,   568,   260,   247,   568,
     568,   307,   568,   320,    -7,   252,   568,   568,   568,   568,
     568,   568,   322,   323,   326,   242,  -451,   -34,  -451,   272,
     273,   287,   298,   -87,   314,   297,  -451,   394,  9372,  -451,
    -451,   259,   259,    93,    93,    69,   160,   357,   568,  -451,
    -451,  -451,  -451,  2791,   -80,   -78,  9641,  9641,  9547,  9310,
    9322,  9443,  9452,  9461,  9301,   -45,   568,  9333,  5909,   285,
    9601,   289,   -66,   301,  9502,  9512,  9641,  9641,  9641,  9641,
     299,   306,   308,   568,   379,   203,     5,   374,   348,   316,
     261,   262,   -53,   317,   332,     6,  -451,  -451,  9641,  -451,
    -451,  -451,   -33,  9515,  9467,    12,   395,   340,   101,  -185,
     336,   399,  9119,   -28,   421,   401,   400,   568,   456,   568,
     -85,   349,   568,   568,   568,  9641,   568,   433,   437,   356,
     390,   417,   246,   296,   -10,   300,   423,   449,   428,   429,
     431,   568,   -56,   -13,   404,   409,   -12,   410,   414,   411,
     415,   418,   419,   420,   422,   436,   454,   455,   457,   462,
     465,   466,   467,   468,   476,   232,   235,   251,   274,   311,
     318,   319,   321,   171,   294,   179,   219,   325,   327,   335,
     345,   353,   354,   358,   359,   487,   489,   477,   493,   490,
     496,   499,   505,   508,   518,   568,   523,   617,   385,   361,
     365,   389,   568,   542,   547,  5629,  -451,  9641,  -451,   349,
    -451,  -451,  9641,  9641,  9641,  5864,   568,   568,   568,   258,
     549,   491,   481,   482,   483,   494,   564,   568,  -451,   578,
     580,   238,   568,   563,   568,   565,   567,   569,   568,   570,
     572,   568,   574,   581,   582,   584,   588,   629,   630,   640,
     641,   642,   643,   644,   647,   652,   654,   655,   651,   665,
     667,   743,   744,   745,   747,   749,   750,   758,   759,   767,
     768,   776,   777,   785,   786,   788,   789,   790,   362,   568,
     555,   568,   701,   568,   568,   382,   383,   384,  9393,   568,
     568,   795,   799,   800,   805,  5900,   568,   568,   809,  -451,
     813,  9641,  5846,  9402,   822,   568,   674,   742,   746,   748,
     675,   568,  9641,   826,   838,   568,  8770,   402,  8784,   403,
     412,   416,  8793,   424,   427,  8802,   435,   447,   450,   451,
     452,   453,   458,   459,   463,   470,   473,   474,   475,   484,
     485,   488,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   739,  9342,  9351,   730,  9411,   763,  9641,  9641,
     843,   849,   850,   780,  9537,  9259,   568,   568,   568,   568,
     862,   406,  9471,   568,   568,   875,   810,   568,  9269,   774,
     870,   872,   891,   845,  9278,   568,   568,   295,   775,   568,
     803,   568,   568,   568,   806,   568,   568,   807,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,  7461,  7470,  6257,  6533,  6090,  6542,
    7483,  6866,  6311,  7452,  7495,  7504,  7517,  7531,  7540,  7555,
    7565,  7578,  5961,  7588,   821,    73,    61,   848,   877,   894,
     568,   568,   568,   539,   543,   970,  9484,  7313,  8185,  9641,
     568,   887,   940,  5620,  5882,   568,   871,  5855,   976,   895,
     914,   923,   924,   974,  1014,  9641,  9568,   568,   859,   916,
     873,  1041,  1211,  1283,   878,  1680,  1960,   880,  2006,  2099,
    2138,  2147,  2180,  2226,  2267,  2302,  2343,  2373,  2428,  2497,
    2552,  2718,  2742,  2813,  1023,  1033,  1035,  1037,  1038,  1040,
    1049,  1057,  1058,  1060,  1061,  1062,  1063,  1064,  1065,  1066,
    1074,  1075,  1084,  1085,  -451,  1005,  1006,  1008,  -451,  1021,
    1022,  1034,  1054,   -57,  1111,  5765,  5774,  5783,  1045,  1091,
     568,  1106,  1131,  1133,  5891,  1103,  1053,  1156,  1159,  9641,
     568,  1161,   568,  1138,   568,   568,   568,  1081,   568,  1166,
     212,   954,   568,   955,   568,   568,   568,   956,   568,   568,
     957,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,  1182,  1183,  1185,
    1143,  1148,  1107,  1113,  1198,  1199,   568,  1200,  1209,  1218,
    1181,  1141,  9641,  1142,   568,   568,  1226,  1068,   568,   568,
     568,  9641,   568,  9077,  1147,  9017,  9026,  9041,   568,  9087,
     568,  1151,  1153,  2838,  1154,  2854,  2872,  2881,  1155,  2890,
    2899,  1157,   310,   371,   426,   469,   548,  2908,  2917,  2926,
    2937,   589,   598,   607,   636,  2946,  2955,  2964,  7322,  7332,
    6248,  6560,  6081,  6524,  7346,  6852,  6302,  9641,  6327,  6336,
    7355,  7364,  7373,  7384,  7399,  7412,  5951,  7422,   568,   568,
     568,  1167,  1168,   796,   568,   568,   568,  9287,   568,   568,
     568,  1169,   568,   568,  7716,  8052,   568,   823,  7794,  5603,
    9641,  5873,  1237,   568,  1252,  1254,  1256,  9050,  1258,  9582,
     568,  1172,   568,  1184,   568,   568,   568,  1192,   568,   568,
    1193,  1191,  1121,  1099,  1110,  1112,   568,   568,   568,   568,
    1207,  1100,  1145,  1116,   568,   568,   568,  1310,  1312,  1317,
    1318,  1319,  1321,  1322,  1323,  1341,  1342,  1344,  1346,  1350,
    1351,  1352,  1353,  1354,  1367,  1372,  9641,  9592,  9096,   568,
     568,   568,  9641,  9247,  9185,  1373,  5737,  5701,  5710,   568,
    1050,  9641,  1375,  1376,  9641,   568,  1382,  1390,  1399,  1433,
    9059,   568,   568,   568,  1405,  1435,  1407,  9528,   944,  2973,
     969,  2982,  2992,  3001,   972,  3010,  3019,   980,   568,  1420,
    1422,  1425,  1440,  3028,  3037,   717,  3046,  1427,  1449,  1452,
    1453,  3055,  3082,  3091,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,  1454,  1456,  9641,  9641,  3100,  1464,  1465,
     568,  1477,  1478,  1480,  9422,   349,   568,   568,  9641,   568,
     568,   568,  1092,  1486,  8960,  8969,  8984,   568,  1094,   568,
    -451,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,  9434,   568,   568,   568,   568,   568,   568,  1429,   568,
    1428,   568,   568,   568,   568,   568,   568,  9641,  9641,  6234,
    6776,  6072,  7431,  6017,  6899,  6290,  5796,  1441,  7290,  7300,
    7227,  7236,  7250,  7263,  5942,  7272,   568,  1542,   568,   568,
     568,  9068,  1120,   568,   568,  1434,  -451,  7629,  8002,  7779,
    1547,  9641,  1529,   568,  1533,  1534,  1535,  8997,  1545,  9556,
    3109,  3140,  3150,  3159,  3168,  3182,  3191,  3200,  3209,  3236,
    1463,  7707,  7597,  7611,  7620,   751,   760,  1530,   769,   568,
    7965,  8760,  9493,  3245,  3254,  3263,  1555,  1557,  1559,  1561,
    1565,  1566,  1567,  1569,  1572,  1576,  1595,  1596,  1605,  1607,
    1617,  1625,  1628,  9105,  -451,  3278,  9194,  9226,  1629,  -451,
    5746,  5674,  1220,  1646,  1649,  1650,  1653,  1217,  9006,   568,
     568,   568,  1660,  1223,  1669,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,  1645,  1679,  1687,  1694,  1696,
    1614,  1616,  1619,  1620,  8197,  1700,  1705,  1682,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,  1707,   568,
    1708,  1709,   568,  1710,  1712,  -451,   568,   568,  1281,   568,
     568,  1721,  8903,  8913,  8931,   568,   568,   568,  3287,  3296,
    3305,  3314,  3323,  3332,  3341,  3350,  3359,  3369,  1643,   568,
     568,   568,   568,  1703,  1704,   568,  1706,  1729,   568,   568,
    1661,  3378,  3387,  3396,  6217,  6799,  6056,  6551,  6006,  6887,
    5814,  5719,  5647,  6910,  7183,  6919,  6928,  6943,  7192,  5933,
    7201,  1761,  3405,   568,   568,  9152,  1329,   568,  9641,  8141,
     568,  1202,  3414,   568,  1744,  1745,  1746,  8940,  3424,  9641,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,  7638,  8150,  8159,  8172,  1665,  1666,  7732,  1688,   568,
    7924,  7978,   568,   568,   568,   568,  1769,  1772,  1773,  1774,
    1775,  1776,  1777,  1778,  1782,  1784,  1792,  1793,  1795,  1796,
    1798,  1799,  1802,  1368,   568,  9235,  9133,  1803,  -451,  5756,
    1804,  3433,  1814,   568,  8949,   568,   568,   568,  1815,   568,
    3442,  3451,  3460,  3469,  3478,  3487,  3496,  3505,  3514,  3523,
    7648,  1816,  1817,  1819,  1823,   568,   568,  1825,   568,  7663,
    1836,  1839,  9641,  3532,  3541,  3550,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,  1847,     2,  1848,  1850,   568,  1851,   568,
     568,   568,  3559,  1852,  9641,  9641,  9641,   568,  3568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,  1854,
     568,   568,   568,   568,  7745,  7758,   568,  7767,  1855,   568,
     568,   568,   568,   568,  6208,  6790,  6041,  6509,  5997,  6875,
    6281,  5683,  5638,  6955,  7142,  6964,  6979,  6991,  7151,  5924,
    7161,  1387,   568,  1788,   568,   568,  9641,  1379,  8095,   778,
      23,   568,   568,  9641,   568,  3580,  3591,  3600,  3609,  3623,
    3632,  3641,  3650,  3659,  3668,   568,  7827,  8111,  8120,  8132,
    1856,  1869,  9641,  1870,   568,  7901,  7950,  3677,  3686,  3695,
    1871,  1872,  1876,  1880,  1883,  1888,  1889,  1899,  1901,  1903,
    1905,  1906,  1908,  1909,  1910,  1912,  1913,   568,  3704,    74,
    9143,  9206,  -451,  1915,  -451,  1916,  3719,  9641,  3728,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,  7992,
    1917,  1918,  1921,  1922,   568,   568,   568,  9641,  1930,  1934,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
    3737,   568,  1401,   568,  1937,  1941,   568,   568,   568,   568,
    3746,   814,  3755,  3764,   855,  3773,  3782,  3791,   867,  3801,
    1943,  1484,   568,   568,   568,  9641,  9641,  9641,   568,   568,
     879,  3810,  3819,  6197,  6763,  7441,  6500,  5988,  9641,  6272,
    5656,  5805,  9641,  7000,  7042,  7051,  7062,  7009,  7281,  7018,
     568,  3828,   568,  9641,   568,   568,  9641,  9641,  3837,  3846,
     568,  1861,   568,   568,  1862,   568,   568,   568,  1863,   568,
     568,   568,  8011,  8022,  8039,  7883,  7910,  1864,   568,   568,
    1949,  1950,  1967,  1968,  1972,  1973,  1974,  1975,  1977,  1978,
    1980,  1981,  1982,  1983,  1984,  3856,   568,  3865,  9217,  9161,
     568,   568,  3874,  1961,  3883,  3892,  1962,  3901,  3910,  3919,
    1979,  3928,  7840,  3937,  1997,  1998,  2002,  2013,  2014,  1992,
    3946,  3955,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,  3964,   568,
    2017,  2018,  3973,  3982,   568,  1942,   568,   568,  1945,   568,
     568,   568,  1946,   568,  2019,   568,   568,   568,   568,   568,
     568,  1963,   568,   568,  6187,  6754,  7075,  6485,  5979,  9641,
    9641,  5728,  9641,  9641,  6812,  9641,  7084,  7215,  7093,  3991,
     568,  4000,   568,   568,   568,   568,  4009,   568,  4020,  4032,
     568,  4041,  4050,  4064,   568,  4073,  1584,  4082,  8064,  8073,
    8086,  7803,  7892,   568,  4091,  4100,  2025,  2042,  2044,  2049,
    2053,  2054,  2055,  2056,  2057,  2058,   568,    41,   568,  9176,
    9641,   902,   950,   568,  9641,   568,   568,  8704,   568,   568,
     568,  8713,   568,   568,   568,  2060,  2062,  2063,  2064,  2066,
    8722,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,  4109,   568,  1989,  4118,  2069,  1683,  1684,
    1007,  4127,  4136,  2072,  4145,  4160,  4169,  2074,  4178,  4187,
    1016,   568,   568,   568,  1636,   568,  2076,  4196,  4205,  6178,
    6740,  9641,  6476,  5970,  5692,  6834,  9641,  7172,  6821,   568,
    1067,    88,   568,   568,  2077,  2078,  2000,   568,   568,   568,
     568,   568,   568,   568,   568,   568,  1874,  7676,  7687,  7697,
     568,  7817,   568,   568,   568,  2089,  2090,  2094,  2106,  2107,
    2109,  2111,  2113,  4214,  2035,  1676,   568,  1077,  9641,   568,
     568,  2092,  4223,  4233,  8636,  4242,  4251,  4260,  8645,  4269,
    4278,  2117,  2118,  2119,  2131,  4288,  2133,  8658,  4297,  4306,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   114,
     568,  9641,  -451,  9641,  9641,  2052,   568,   568,  2135,   568,
     568,   568,  2145,   568,   568,  1716,   568,   568,   568,   568,
    1718,  2146,   568,   568,  6169,  6727,  6461,  7102,  5665,  9641,
    7029,  6843,  1086,  1719,   568,  4315,   568,  4324,  4333,   568,
    4342,  4351,  4360,   568,  4369,  1097,   568,  7851,  7860,  7874,
    4378,   568,   568,  4387,  4396,  2159,  2167,  2171,  2173,  2175,
    2176,  2177,  -451,   568,  9641,   568,  9641,   568,   568,  8579,
     568,   568,   568,  8591,   568,  1965,  4405,  2187,  2190,  2191,
     568,  4414,  8600,   568,   568,   568,   568,   568,   568,   568,
     568,   568,  4423,  4432,  4441,  4450,  2193,  4460,  4473,  4482,
    2194,  4491,  2195,   568,  1763,  1765,  1770,  1123,   568,  2200,
    4505,  4514,  6160,  6716,  6446,  9641,  9641,  7114,  9641,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,  1779,
    4523,   568,   568,   568,  -451,  4532,   568,   568,   568,  2201,
    2213,  2217,  2219,  4541,  4550,  4559,  4568,  8520,  4577,  4586,
    4601,  8529,  4610,   568,   568,  4619,  4628,  4637,   568,  8538,
    4646,  4655,   568,   568,   568,   568,   568,   568,   568,   568,
    2220,   568,   568,   568,  2233,   568,  4665,  1175,   568,   568,
     568,  1263,  2234,   568,   568,  6151,  6707,  6437,  9641,  4674,
    4683,  4692,  4701,   568,  4710,  4720,  4729,   568,  4738,   568,
    2022,  4747,  4756,  4765,  -451,   568,  4774,  4783,  2237,  2241,
    2242,   568,   568,   568,   568,  8451,   568,   568,   568,  8464,
     568,  4792,  2243,   568,   568,   568,  8473,   568,   568,   568,
     568,   568,  4801,  4810,  4819,  4828,  2244,  4837,  4846,  4855,
    2245,  4864,   568,  1807,  1272,  1298,  1383,  2248,  4873,  4882,
    6142,  6698,  6422,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,  1392,   568,  2039,  2041,  2045,   568,   568,
     568,  2259,  2260,  2261,  4892,  4901,  4914,  4923,  8396,  4932,
    4946,  4955,  8405,  4964,  2051,  4973,  2263,  2266,  2274,  8414,
    4982,  4991,   568,   568,   568,   568,   568,   568,   568,  2275,
     568,   568,   568,  2277,   568,  2282,   568,  1844,  1858,  1859,
    2290,   568,   568,  6133,  6685,  6408,  5000,  5009,  5018,  5027,
     568,  5042,  5051,  5060,   568,  5069,  1860,  5078,   568,   568,
     568,   568,  5087,  5097,  2291,  2293,  2294,   568,   568,   568,
     568,  8333,   568,   568,   568,  8342,   568,   568,   568,  5106,
    5115,  5124,  8351,   568,   568,   568,   568,   568,  5133,  1412,
    5142,  5152,  2300,  1432,  5161,  1457,  2301,  5170,  5179,  1519,
     568,   568,   568,  2314,  1538,  5188,  6124,  6676,  6394,   568,
    -451,   568,   568,   568,  2232,   568,  2235,   568,   568,   568,
    -451,  5197,  5206,  5215,   568,  2238,   568,  2316,  2317,  2322,
    5224,  5233,  5242,  8267,  2299,  5251,  2304,  8276,  5260,  5269,
     568,   568,   568,  8288,  2305,  5278,   568,   568,   568,   568,
     568,   568,  2324,  2249,   568,  2252,  2325,   568,   568,  1579,
    1588,  1597,  2335,  2254,   568,  6115,  6667,  6385,  1609,  5287,
    5296,   568,   568,  5305,   568,   568,  5314,  1618,  2123,  2124,
    2125,   568,   568,  5324,  2350,  2351,  2353,  -451,   568,   568,
    8206,  8731,   568,  9641,  8215,   568,  -451,  2354,  2355,  2356,
    8225,  8740,   568,   568,   568,   568,  5333,  5342,  2357,  2358,
    1671,  2359,  5355,  1944,  1947,  1948,  2380,  2381,  5364,  6099,
    6658,  6375,   568,   568,   568,   568,  2311,   568,   568,   568,
     568,   568,   568,   568,   568,  2384,  2385,  2388,  5373,  5387,
    9641,  8667,  2349,  9641,  5396,  5405,  5414,  5423,  9641,  8679,
    5432,   568,   568,   568,   568,   568,  2395,  2320,   568,   568,
     568,   568,  2403,   568,  9641,  6645,  6360,  5441,  5450,   568,
     568,  5459,  5468,  5483,  5492,   568,  5501,  2404,  2405,   568,
     568,  8609,  9641,   568,   568,   568,   568,  8618,   568,   568,
     568,  5510,  5519,  2406,  5529,  1713,  1762,  1785,  2409,  5538,
    6636,  6351,   568,   568,   568,   568,  -451,  -451,  -451,   568,
     568,  2410,  2411,  5547,  1826,  8547,  5556,  8556,  1840,   568,
     568,   568,  2329,  2414,   568,  2415,  2333,  6627,  7129,  5565,
    2396,   568,  5574,   568,  2401,  2419,  2435,   568,  2362,  8485,
     568,  8494,  2363,   568,   568,  5584,   568,  2436,  5593,  2438,
     568,  6617,  9615,   568,  9641,   568,   568,   568,  8749,  2439,
    2449,  1891,  8423,  1923,  8432,  2450,   568,   568,  2368,  2452,
    2370,  2454,   568,  6608,  8879,  2431,   568,  2432,   568,  8688,
    2460,  2461,  2382,  8360,  2383,  8372,  2463,   568,   568,   568,
    2466,   568,  2467,   568,  6596,  8858,  9641,   568,  9641,   568,
    8627,  2468,  2470,  8297,  8313,  2472,   568,   568,  2473,  2474,
     568,  6583,  8836,   568,   568,  8567,  2476,  2477,  8234,  8243,
    2479,   568,   568,  2480,  2483,   568,  6569,  8811,   568,   568,
    8510,  2485,  2486,  9641,  9641,  2487,   568,   568,   568,  9630,
    9641,  8441,  2489,  2490,   568,   568,  8894,  8384,  2492,  2496,
     568,   568,  8870,  8322,  2507,  2508,   568,   568,  8849,  8254,
    2509,  2511,   568,   568,  8827,  9641,  2512,   568,  9641
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   144,   162,   161,   167,     0,     0,     0,     0,     0,
      13,   183,     0,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     7,     6,     0,     0,   184,    14,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,   177,     0,     8,    10,    11,     9,
      12,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,   160,   152,
       0,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    77,     0,   179,
       1,   143,     0,     0,     0,     0,     0,     0,     0,   178,
     164,   153,   154,   155,   156,   157,   158,   163,     0,    57,
       0,     0,     0,   180,   182,     0,     0,     5,    72,     0,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    68,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,    61,
     159,   146,   147,   148,   149,   150,   151,   145,     0,   168,
     166,   165,   186,   188,     0,     0,     4,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    88,    91,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    62,    58,   189,
     187,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,    63,    52,    55,
      51,    54,    92,    93,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,    39,
       0,     0,     0,     0,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,     0,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    47,     0,     0,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,    19,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
      41,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,    83,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,   113,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   137,   138,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,    49,    42,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
     110,     0,    97,    98,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,     0,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    23,    84,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,    26,     0,   118,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,   111,     0,   103,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   136,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,     0,   122,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,     0,     0,   126,     0,     0,     0,     0,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,   132,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,   128,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,     0,     0,   106
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -451,  -451,  -451,  -451,  -256,  -451,  -451,  -451,  -451,  -451,
    -451,  -451,    -8,    22,   -41,  2328
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    45,    46,    47,   360,   138,    48,    49,    50,    89,
     151,    51,    52,   203,   143,   204
  };

  const short int
  feiparser::yytable_[] =
  {
      65,    66,   145,   122,   123,   124,   125,   126,   127,   114,
     260,   128,    68,    69,    72,   361,   122,   123,   124,   125,
     126,   127,    53,    62,   128,   144,   358,   110,   122,   123,
     124,   125,   126,   127,   359,   118,   128,   122,   123,   124,
     125,   126,   127,   239,   382,   128,   764,   131,   132,   133,
     134,   135,   136,   137,   139,   140,   141,   383,   122,   123,
     124,   125,   126,   127,    63,   147,   128,   335,   336,     2,
       3,     4,     5,   121,     6,     7,   122,   123,   124,   125,
     126,   127,   164,    64,   128,    67,   240,   384,   388,   241,
     242,   765,    73,    13,    14,    15,    16,    17,    18,    19,
     385,   389,    20,   449,   288,   188,    74,   289,    21,   127,
     243,   290,   128,    90,   191,   192,   193,   194,   195,   196,
     197,    29,   284,   337,    91,   285,   122,   123,   124,   125,
     126,   127,   126,   127,   128,    92,   128,   206,   207,   179,
     180,   208,   209,   111,   210,   211,   212,   213,   214,   198,
     199,   217,   218,    93,   220,   108,   340,   341,   224,   225,
     226,   227,   228,   229,   251,   112,   109,   116,   122,   123,
     124,   125,   126,   127,   373,   374,   128,   122,   123,   124,
     125,   126,   127,    94,    95,   128,   113,    96,   117,    97,
     248,   120,   119,    98,     2,     3,     4,     5,   130,     6,
       7,     8,    99,   128,     9,   680,   681,    10,   262,    11,
     146,    12,   675,   676,   677,   148,   678,   679,    13,    14,
      15,    16,    17,    18,    19,   275,   149,    20,   348,   349,
     350,   150,   351,    21,   152,    22,    23,    24,    25,  1552,
    1553,    26,   153,    27,   157,    28,    29,   122,   123,   124,
     125,   126,   127,  1875,  1876,   128,   342,   343,   344,   355,
     154,   357,    30,   155,   362,   363,   364,   156,   365,    31,
      32,    33,    34,   122,   123,   124,   125,   126,   127,  1943,
    1944,   128,   159,   381,   158,   160,   161,    35,    36,     2,
       3,     4,     5,   162,     6,     7,   124,   125,   126,   127,
      37,   163,   128,   165,   166,   167,    38,   172,   168,   169,
     170,   173,   100,    13,    14,    15,    16,    17,    18,    19,
     171,   175,    20,   174,   176,   177,   181,   183,    21,   101,
     122,   123,   124,   125,   126,   127,   178,   438,   128,   182,
     186,    70,    71,   187,   445,   122,   123,   124,   125,   126,
     127,   184,   102,   128,   189,   103,   205,   104,   451,   452,
     453,   105,    90,   216,   106,     2,     3,     4,     5,   462,
       6,     7,   219,   215,   466,   221,   468,   230,   231,   233,
     472,   223,   232,   475,   234,   235,   236,   237,   238,    13,
      14,    15,    16,    17,    18,    19,   244,   245,    20,   246,
      -1,   250,   142,   265,    21,   267,   122,   123,   124,   125,
     126,   127,   268,   276,   128,   272,   261,    29,   115,   269,
     513,   514,   273,   516,   274,   518,   519,   277,   278,   279,
     280,   524,   525,   281,   286,   282,   287,   283,   531,   532,
     291,   122,   123,   124,   125,   126,   127,   538,   142,   128,
     338,   339,   345,   544,   346,   352,   353,   547,   354,   356,
     129,   122,   123,   124,   125,   126,   127,   366,   359,   128,
     222,   367,   368,   512,   369,   370,  1442,  1443,   371,   375,
     372,   376,  1505,   377,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   122,   123,   124,   125,   126,   127,
     378,   379,   128,   380,   391,  1804,  1805,   386,   606,   607,
     608,   609,   387,   390,   392,   613,   614,   408,   393,   617,
     409,   394,   395,   396,   430,   397,   416,   625,   626,    43,
     107,   629,   418,   631,   632,   633,   410,   635,   636,   398,
     638,   639,   640,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,   651,   652,   653,   190,   399,   400,   411,
     401,     2,     3,     4,     5,   402,     6,     7,   403,   404,
     405,   406,   419,   122,   123,   124,   125,   126,   127,   407,
     417,   128,   685,   686,   687,    13,    14,    15,    16,    17,
      18,    19,   694,   428,    20,   429,   412,   699,   200,   431,
      21,   432,   433,   413,   414,   434,   415,   201,   435,   710,
     420,   436,   421,    29,   122,   123,   124,   125,   126,   127,
     422,   437,   128,   122,   123,   124,   125,   126,   127,   439,
     423,   128,   122,   123,   124,   125,   126,   127,   424,   425,
     128,   440,   441,   426,   427,   444,    39,   442,   446,    40,
      41,   443,    42,   447,    43,   455,   456,   457,   458,   459,
      44,   122,   123,   124,   125,   126,   127,   454,   460,   128,
     461,   463,   772,   464,   467,   492,   469,   791,   470,   515,
     471,   473,   781,   474,   783,   476,   785,   786,   787,   493,
     789,   494,   477,   478,   793,   479,   795,   796,   797,   480,
     799,   800,   465,   802,   803,   804,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,   830,   831,   832,   833,   834,   835,   836,   837,  1076,
     481,   482,   122,   123,   124,   125,   126,   127,   847,    43,
     128,   483,   484,   485,   486,   487,   854,   855,   488,   627,
     858,   859,   860,   489,   861,   490,   491,   495,   496,   497,
     867,   498,   869,   499,   500,   881,   122,   123,   124,   125,
     126,   127,   501,   502,   128,   122,   123,   124,   125,   126,
     127,   503,   504,   128,   122,   123,   124,   125,   126,   127,
     505,   506,   128,   122,   123,   124,   125,   126,   127,   507,
     508,   128,   509,   510,   511,   517,   520,   521,   522,   526,
     916,   917,   918,   527,   528,    43,   922,   923,   924,   529,
     926,   927,   928,   533,   930,   931,   882,   534,   934,   122,
     123,   124,   125,   126,   127,   940,   537,   128,   539,   540,
     545,   543,   947,   541,   949,   542,   951,   952,   953,   611,
     955,   956,   546,   594,   597,   549,   551,   600,   963,   964,
     965,   966,   599,   601,   602,   552,   971,   972,   973,   553,
     122,   123,   124,   125,   126,   127,   610,   555,   128,   603,
     556,   883,   122,   123,   124,   125,   126,   127,   558,   615,
     128,   995,   996,   997,   122,   123,   124,   125,   126,   127,
     559,  1004,   128,   560,   561,   562,   563,  1008,   620,   616,
     621,   564,   565,  1014,  1015,  1016,   566,   122,   123,   124,
     125,   126,   127,   567,   884,   128,   568,   569,   570,   622,
    1031,   122,   123,   124,   125,   126,   127,   571,   572,   128,
     619,   573,   623,   628,   674,   682,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1065,   122,   123,   124,   125,   126,
     127,   630,  1071,   128,   634,   637,   683,   684,  1077,  1078,
     688,  1079,  1080,  1081,   690,   689,   695,   696,   700,  1087,
     702,  1089,   703,  1090,  1091,  1092,  1093,  1094,  1095,  1096,
    1097,  1098,  1099,   885,  1101,  1102,  1103,  1104,  1105,  1106,
     704,  1108,   707,  1110,  1111,  1112,  1113,  1114,  1115,   705,
     706,    43,   122,   123,   124,   125,   126,   127,   708,   711,
     128,   122,   123,   124,   125,   126,   127,   737,  1133,   128,
    1135,  1136,  1137,   713,   890,  1140,  1141,   738,   717,   739,
     720,   740,   741,   891,   742,  1148,   122,   123,   124,   125,
     126,   127,   892,   743,   128,   122,   123,   124,   125,   126,
     127,   744,   745,   128,   746,   747,   748,   749,   750,   751,
     752,  1174,   122,   123,   124,   125,   126,   127,   753,   754,
     128,   893,   122,   123,   124,   125,   126,   127,   755,   756,
     128,   122,   123,   124,   125,   126,   127,   757,   758,   128,
     759,   763,   122,   123,   124,   125,   126,   127,   760,   761,
     128,  1212,  1213,  1214,   762,   766,   771,  1218,  1219,  1220,
    1221,  1222,  1223,  1224,  1225,  1226,  1227,   770,   122,   123,
     124,   125,   126,   127,   773,   774,   128,   775,   777,   778,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,
     779,  1262,  1038,   780,  1265,   782,   784,   788,  1268,  1269,
     790,  1271,  1272,   792,   794,   798,   801,  1277,  1278,  1279,
     122,   123,   124,   125,   126,   127,   838,   839,   128,   840,
     841,  1291,  1292,  1293,  1294,   842,  1170,  1297,   843,   844,
    1300,  1301,   845,   846,   848,  1171,   851,   122,   123,   124,
     125,   126,   127,   849,  1173,   128,   122,   123,   124,   125,
     126,   127,   850,  1504,   128,  1325,  1326,   852,   853,  1329,
     856,   857,  1331,   863,   870,  1334,   871,   873,   877,   921,
     880,   939,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,
    1348,  1349,  1350,   919,   920,   929,   941,   935,   942,  1611,
     943,  1359,   945,   948,  1362,  1363,  1364,  1365,   122,   123,
     124,   125,   126,   127,   958,   950,   128,   122,   123,   124,
     125,   126,   127,   954,   957,   128,  1384,   959,   122,   123,
     124,   125,   126,   127,   967,  1392,   128,  1394,  1395,  1396,
    1614,  1398,   960,   122,   123,   124,   125,   126,   127,   969,
     968,   128,  1618,   961,   974,   962,   975,  1414,  1415,   970,
    1417,   976,   977,   978,  1627,   979,   980,   981,  1424,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,
    1436,  1437,  1438,  1439,  1440,   982,   983,  1808,   984,  1446,
     985,  1448,  1449,  1450,   986,   987,   988,   989,   990,  1453,
     712,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,   991,  1466,  1467,  1468,  1469,   992,  1000,  1472,  1006,
    1007,  1475,  1476,  1477,  1478,  1479,  1009,  1021,   122,   123,
     124,   125,   126,   127,  1010,  1809,   128,   122,   123,   124,
     125,   126,   127,  1011,  1498,   128,  1500,  1501,  1012,  1017,
    1018,  1019,  1023,  1506,  1507,  1027,  1508,   122,   123,   124,
     125,   126,   127,  1030,  1032,   128,  1033,  1519,    54,  1034,
      55,    56,    57,    58,    59,    60,  1527,   122,   123,   124,
     125,   126,   127,    61,  1035,   128,   122,   123,   124,   125,
     126,   127,  1846,  1041,   128,  1040,  1042,  1043,  1066,  1550,
    1067,  1856,   122,   123,   124,   125,   126,   127,  1069,  1070,
     128,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,
    1569,  1072,  1073,  1005,  1074,   714,  1575,  1576,  1577,  1082,
    1083,  1088,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,
    1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,  1597,
    1598,  1599,  1874,  1601,  1109,  1603,  1107,  1134,  1606,  1607,
    1608,  1609,  1912,  1142,   122,   123,   124,   125,   126,   127,
    1139,  1972,   128,  1147,  1622,  1623,  1624,  1149,  1150,  1151,
    1625,  1626,  1985,   122,   123,   124,   125,   126,   127,  1153,
    1165,   128,   122,   123,   124,   125,   126,   127,  1172,  1181,
     128,  1182,  1645,  1183,  1647,  1184,  1648,  1649,  2044,  1185,
    1186,  1187,  1652,  1188,  1654,  1655,  1189,  1657,  1658,  1659,
    1190,  1661,  1662,  1663,   122,   123,   124,   125,   126,   127,
    1670,  1671,   128,   122,   123,   124,   125,   126,   127,  1191,
    1192,   128,   122,   123,   124,   125,   126,   127,  1688,  1193,
     128,  1194,  1692,  1693,   122,   123,   124,   125,   126,   127,
    2110,  1195,   128,   122,   123,   124,   125,   126,   127,  1196,
    1332,   128,  1197,  1202,  1714,  1715,  1716,  1717,  1718,  1719,
    1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,
    1206,  1731,  1205,  1207,  1208,   715,  1736,  1209,  1738,  1739,
    1210,  1741,  1742,  1743,  1215,  1745,  1216,  1747,  1748,  1749,
    1750,  1751,  1752,  1217,  1754,  1755,   122,   123,   124,   125,
     126,   127,  1228,  1229,   128,   122,   123,   124,   125,   126,
     127,  1230,  1767,   128,  1769,  1770,  1771,  1772,  1231,  1774,
    1232,  1233,  1777,  1234,  1238,  1235,  1781,  1236,  2114,  1239,
    1240,  1261,  1263,  1264,  1266,  1790,  1267,  2175,   122,   123,
     124,   125,   126,   127,  1270,  1273,   128,   716,  1803,  1290,
    1806,  1295,  1296,  1299,  1298,  1810,  1323,  1811,  1812,  1328,
    1814,  1815,  1816,  2176,  1818,  1819,  1820,  1302,  1335,  1336,
    1337,  1355,  1356,  1827,  1828,  1829,  1830,  1831,  1832,  1833,
    1834,  1835,  1836,  1837,  1838,  1383,  1840,   122,   123,   124,
     125,   126,   127,  1366,  1358,   128,  1367,  1368,  1369,  1370,
    1371,  1372,  1373,  1857,  1858,  1859,  1374,  1861,  1375,  1502,
     122,   123,   124,   125,   126,   127,  1376,  1377,   128,  1378,
    1379,  1873,  1380,  1381,  1877,  1878,  1382,  1387,  1389,  1882,
    1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,  1391,  1397,
    1410,  1411,  1895,  1412,  1897,  1898,  1899,  1413,  2177,  1416,
    1497,   122,   123,   124,   125,   126,   127,  2194,  1911,   128,
    1419,  1913,  1914,  1420,  1602,   122,   123,   124,   125,   126,
     127,  1441,  1444,   128,  1445,  1447,  1452,  2290,  1465,  1474,
    1524,  1124,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,
    1942,  1499,  1945,  1525,  1526,  1533,  1534,  2294,  1947,  1948,
    1535,  1950,  1951,  1952,  1536,  1954,  1955,  1537,  1957,  1958,
    1959,  1960,  1538,  1539,  1963,  1964,   122,   123,   124,   125,
     126,   127,  2296,  1540,   128,  1541,  1974,  1542,  1976,  1543,
    1544,  1979,  1545,  1546,  1547,  1983,  1548,  1549,  1986,  1556,
    1557,  1571,  1572,  1991,  1992,  1573,  1574,  1621,   122,   123,
     124,   125,   126,   127,  1578,  2002,   128,  2003,  1579,  2004,
    2005,  1604,  2007,  2008,  2009,  1605,  2011,  1620,  1653,  1656,
    1660,  1669,  2017,  1672,  1673,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2027,  2028,  2300,   122,   123,   124,   125,   126,
     127,  1674,  1675,   128,  1146,  2040,  1676,  1677,  1678,  1679,
    2045,  1680,  1681,  2305,  1682,  1683,  1684,  1685,  1686,  1695,
    1698,  2053,  2054,  2055,  2056,  2057,  2058,  2059,  2060,  2061,
    2062,  1706,  1707,  2065,  2066,  2067,  1708,  1702,  2069,  2070,
    2071,   122,   123,   124,   125,   126,   127,  1709,  1710,   128,
    1711,  1732,  1733,  1746,  2358,  2086,  2087,  1783,  1737,  1793,
    2091,  1740,  1744,  2359,  2095,  2096,  2097,  2098,  2099,  2100,
    2101,  2102,  2360,  2104,  2105,  2106,  1794,  2108,  1795,  1753,
    2111,  2112,  2113,  1796,  2367,  2116,  2117,  1797,  1798,  1799,
    1800,  1801,  1802,  2376,  1821,  2125,  1822,  1823,  1824,  2129,
    1825,  2131,  1841,  1843,  1844,  1845,  1849,  2136,  1853,  1860,
    1862,  1879,  1880,  2142,  2143,  2144,  2145,  1881,  2147,  2148,
    2149,  1891,  2151,  1900,  1901,  2154,  2155,  2156,  1902,  2158,
    2159,  2160,  2161,  2162,   122,   123,   124,   125,   126,   127,
    1903,  1904,   128,  1905,  2173,  1906,  2406,  1907,  1909,  1910,
    1915,  1925,  1926,  1927,   718,  2184,  2185,  2186,  2187,  2188,
    2189,  2190,  2191,  2192,  2193,  1928,  2195,  1930,  1946,  1949,
    2199,  2200,  2201,   122,   123,   124,   125,   126,   127,  1953,
    1962,   128,   122,   123,   124,   125,   126,   127,  2486,  1956,
     128,  1961,  1973,  1995,  2223,  2224,  2225,  2226,  2227,  2228,
    2229,  1996,  2231,  2232,  2233,  1997,  2235,  1998,  2237,  1999,
    2000,  2001,  2012,  2242,  2243,   122,   123,   124,   125,   126,
     127,  2014,  2251,   128,  2015,  2016,  2255,  2033,  2037,  2039,
    2259,  2260,  2261,  2262,  2046,  2072,  2041,  2487,  2042,  2268,
    2269,  2270,  2271,  2043,  2273,  2274,  2275,  2073,  2277,  2278,
    2279,  2074,  2063,  2075,  2103,  2284,  2285,  2286,  2287,  2288,
    2488,   122,   123,   124,   125,   126,   127,  2107,  2115,   128,
    2132,  2139,  2301,  2302,  2303,  2140,  2141,  2153,  2167,  2171,
    2174,  2310,  2178,  2311,  2312,  2313,  2196,  2315,  2197,  2317,
    2318,  2319,  2198,  2202,  2203,  2204,  2323,  2217,  2325,  2215,
    2218,  2502,   122,   123,   124,   125,   126,   127,  2219,  2230,
     128,  2234,  2339,  2340,  2341,  2506,  2236,  2238,  2345,  2346,
    2347,  2348,  2349,  2350,  2241,  2265,  2353,  2266,  2267,  2356,
    2357,  2239,  2240,  2257,  2293,  2297,  2363,   122,   123,   124,
     125,   126,   127,  2370,  2371,   128,  2373,  2374,  2304,  2314,
    2326,  2327,  2316,  2380,  2381,  2324,  2328,  2333,  2351,  2355,
    2386,  2387,  2335,  2343,  2390,  2352,  2548,  2392,  2354,  2361,
    2362,  2377,  2378,  2379,  2398,  2399,  2400,  2401,   122,   123,
     124,   125,   126,   127,  2383,  2384,   128,  2385,  2393,  2394,
    2395,  2404,  2405,  2407,  2418,  2419,  2420,  2421,  2550,  2423,
    2424,  2425,  2426,  2427,  2428,  2429,  2430,  2437,   122,   123,
     124,   125,   126,   127,  2412,  2413,   128,  2409,  2431,  2432,
    2410,  2411,  2433,  2444,  2445,  2446,  2447,  2448,  2422,  2449,
    2451,  2452,  2453,  2454,   719,  2456,  2450,  2455,  2469,  2470,
    2484,  2461,  2462,  2489,  2499,  2500,  2510,  2467,  2511,  2513,
    2514,  2471,  2472,  2523,  2518,  2474,  2475,  2476,  2477,  2522,
    2479,  2480,  2481,   122,   123,   124,   125,   126,   127,  2524,
    2535,   128,  2537,  2546,  2493,  2494,  2495,  2496,  2526,  2530,
     721,  2497,  2498,  2547,  2552,  2555,  2556,  2557,  2558,  2562,
    2564,  2507,  2508,  2509,  2567,  2568,  2512,  2573,  2569,  2571,
    2577,  2579,  2586,  2519,  2587,  2521,  2590,  2593,  2594,  2525,
    2601,  2602,  2528,  2605,  2608,  2531,  2532,  2609,  2534,  2616,
    2617,  2618,  2538,  2624,  2625,  2541,  2630,  2542,  2543,  2544,
    2631,   249,   122,   123,   124,   125,   126,   127,  2553,  2554,
     128,  2636,  2637,  2642,  2559,  2643,  2647,     0,  2563,     0,
    2565,     0,     0,     0,     0,     0,     0,     0,     0,  2574,
    2575,  2576,     0,  2578,     0,  2580,     0,     0,     0,  2583,
       0,  2584,     0,   722,     0,     0,     0,     0,  2591,  2592,
       0,     0,  2595,     0,     0,  2598,  2599,   122,   123,   124,
     125,   126,   127,  2606,  2607,   128,     0,  2610,     0,     0,
    2613,  2614,     0,     0,     0,     0,     0,     0,  2619,  2620,
    2621,     0,   723,     0,     0,     0,  2626,  2627,     0,     0,
       0,   724,  2632,  2633,     0,     0,     0,     0,  2638,  2639,
       0,     0,     0,     0,  2644,  2645,     0,     0,     1,  2648,
       2,     3,     4,     5,     0,     6,     7,     8,     0,     0,
       9,     0,     0,    10,   725,    11,     0,    12,     0,     0,
       0,     0,     0,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    20,     0,     0,     0,     0,     0,    21,
       0,    22,    23,    24,    25,     0,     0,    26,     0,    27,
       0,    28,    29,     0,     0,     0,     2,     3,     4,     5,
     726,     6,     7,     8,     0,     0,     9,     0,    30,    10,
       0,    11,     0,    12,     0,    31,    32,    33,    34,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
       0,     0,     0,    35,    36,    21,     0,    22,    23,    24,
      25,   727,     0,    26,     0,    27,    37,    28,    29,     0,
       0,     0,    38,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,    30,     0,     0,     0,     0,     0,
       0,    31,    32,    33,    34,     0,   728,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,    35,
      36,     0,     0,     0,     2,     3,     4,     5,     0,     6,
       7,     8,    37,     0,     9,     0,     0,    10,    38,    11,
       0,    12,     0,     0,     0,     0,     0,   729,    13,    14,
      15,    16,    17,    18,    19,     0,     0,    20,     0,     0,
       0,     0,     0,    21,     0,    22,    23,    24,    25,     0,
       0,    26,     0,    27,     0,    28,    29,   730,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,     0,    30,     0,     0,     0,     0,     0,     0,    31,
      32,    33,    34,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,    35,    36,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
      37,     0,   731,     0,     0,     0,    38,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,   732,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   733,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,    39,     0,     0,    40,    41,     0,    42,     0,
      43,     0,     0,     0,     0,     0,    44,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,     0,    39,     0,
       0,    40,    41,     0,    42,     0,    43,     0,     0,     0,
       0,     0,    44,     0,   202,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,   122,   123,   124,   125,   126,
     127,     0,   734,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,   735,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,    39,     0,     0,    40,
      41,     0,    42,     0,    43,     0,     0,     0,     0,     0,
      44,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,   736,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,     0,   872,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,   874,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,   875,     0,   128,   122,
     123,   124,   125,   126,   127,   876,     0,   128,   122,   123,
     124,   125,   126,   127,   878,     0,   128,   122,   123,   124,
     125,   126,   127,   879,     0,   128,   122,   123,   124,   125,
     126,   127,   886,     0,   128,   122,   123,   124,   125,   126,
     127,   887,     0,   128,   122,   123,   124,   125,   126,   127,
     888,     0,   128,     0,   122,   123,   124,   125,   126,   127,
       0,   889,   128,   122,   123,   124,   125,   126,   127,     0,
     894,   128,   122,   123,   124,   125,   126,   127,     0,   895,
     128,   122,   123,   124,   125,   126,   127,     0,   896,   128,
     122,   123,   124,   125,   126,   127,     0,  1022,   128,   122,
     123,   124,   125,   126,   127,     0,  1024,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1025,   128,   122,   123,
     124,   125,   126,   127,     0,  1026,   128,   122,   123,   124,
     125,   126,   127,     0,  1028,   128,   122,   123,   124,   125,
     126,   127,     0,  1029,   128,   122,   123,   124,   125,   126,
     127,     0,  1036,   128,   122,   123,   124,   125,   126,   127,
       0,  1037,   128,   122,   123,   124,   125,   126,   127,     0,
    1039,   128,   122,   123,   124,   125,   126,   127,     0,  1044,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,  1045,   128,   122,   123,
     124,   125,   126,   127,     0,  1046,   128,   122,   123,   124,
     125,   126,   127,     0,  1068,   128,   122,   123,   124,   125,
     126,   127,     0,  1155,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,  1156,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1157,     0,   122,   123,   124,   125,
     126,   127,     0,  1158,   128,   122,   123,   124,   125,   126,
     127,     0,  1159,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1160,     0,   122,   123,
     124,   125,   126,   127,     0,  1161,   128,   122,   123,   124,
     125,   126,   127,     0,  1162,   128,   122,   123,   124,   125,
     126,   127,     0,  1163,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
    1164,   128,   122,   123,   124,   125,   126,   127,     0,  1178,
     128,   122,   123,   124,   125,   126,   127,     0,  1179,   128,
     122,   123,   124,   125,   126,   127,     0,  1180,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,  1199,     0,   122,   123,   124,   125,   126,   127,
       0,  1280,   128,   122,   123,   124,   125,   126,   127,     0,
    1281,   128,   122,   123,   124,   125,   126,   127,     0,  1282,
     128,   122,   123,   124,   125,   126,   127,     0,  1283,   128,
     122,   123,   124,   125,   126,   127,     0,  1284,   128,   122,
     123,   124,   125,   126,   127,     0,  1285,   128,   122,   123,
     124,   125,   126,   127,     0,  1286,   128,   122,   123,   124,
     125,   126,   127,     0,  1287,   128,   122,   123,   124,   125,
     126,   127,     0,  1288,   128,     0,   122,   123,   124,   125,
     126,   127,     0,  1289,   128,   122,   123,   124,   125,   126,
     127,     0,  1303,   128,   122,   123,   124,   125,   126,   127,
       0,  1304,   128,   122,   123,   124,   125,   126,   127,     0,
    1305,   128,   122,   123,   124,   125,   126,   127,     0,  1324,
     128,   122,   123,   124,   125,   126,   127,     0,  1333,   128,
       0,   122,   123,   124,   125,   126,   127,     0,  1339,   128,
     122,   123,   124,   125,   126,   127,     0,  1390,   128,   122,
     123,   124,   125,   126,   127,     0,  1399,   128,   122,   123,
     124,   125,   126,   127,     0,  1400,   128,   122,   123,   124,
     125,   126,   127,     0,  1401,   128,   122,   123,   124,   125,
     126,   127,     0,  1402,   128,   122,   123,   124,   125,   126,
     127,     0,  1403,   128,   122,   123,   124,   125,   126,   127,
       0,  1404,   128,   122,   123,   124,   125,   126,   127,     0,
    1405,   128,   122,   123,   124,   125,   126,   127,     0,  1406,
     128,   122,   123,   124,   125,   126,   127,     0,  1407,   128,
     122,   123,   124,   125,   126,   127,     0,  1408,   128,   122,
     123,   124,   125,   126,   127,     0,  1421,   128,   122,   123,
     124,   125,   126,   127,     0,  1422,   128,   122,   123,   124,
     125,   126,   127,     0,  1423,   128,   122,   123,   124,   125,
     126,   127,     0,  1451,   128,   122,   123,   124,   125,   126,
     127,     0,  1454,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  1509,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  1510,     0,   122,   123,   124,
     125,   126,   127,     0,  1511,   128,   122,   123,   124,   125,
     126,   127,     0,  1512,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  1513,     0,   122,
     123,   124,   125,   126,   127,     0,  1514,   128,   122,   123,
     124,   125,   126,   127,     0,  1515,   128,   122,   123,   124,
     125,   126,   127,     0,  1516,   128,   122,   123,   124,   125,
     126,   127,     0,  1517,   128,   122,   123,   124,   125,   126,
     127,     0,  1518,   128,   122,   123,   124,   125,   126,   127,
       0,  1530,   128,   122,   123,   124,   125,   126,   127,     0,
    1531,   128,   122,   123,   124,   125,   126,   127,     0,  1532,
     128,   122,   123,   124,   125,   126,   127,     0,  1551,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,  1558,     0,   122,   123,   124,   125,   126,
     127,     0,  1559,   128,   122,   123,   124,   125,   126,   127,
       0,  1600,   128,   122,   123,   124,   125,   126,   127,     0,
    1610,   128,   122,   123,   124,   125,   126,   127,     0,  1612,
     128,   122,   123,   124,   125,   126,   127,     0,  1613,   128,
     122,   123,   124,   125,   126,   127,     0,  1615,   128,   122,
     123,   124,   125,   126,   127,     0,  1616,   128,   122,   123,
     124,   125,   126,   127,     0,  1617,   128,     0,   122,   123,
     124,   125,   126,   127,     0,  1619,   128,   122,   123,   124,
     125,   126,   127,     0,  1628,   128,   122,   123,   124,   125,
     126,   127,     0,  1629,   128,   122,   123,   124,   125,   126,
     127,     0,  1646,   128,   122,   123,   124,   125,   126,   127,
       0,  1650,   128,   122,   123,   124,   125,   126,   127,     0,
    1651,   128,     0,   122,   123,   124,   125,   126,   127,     0,
    1687,   128,   122,   123,   124,   125,   126,   127,     0,  1689,
     128,   122,   123,   124,   125,   126,   127,     0,  1694,   128,
     122,   123,   124,   125,   126,   127,     0,  1696,   128,   122,
     123,   124,   125,   126,   127,     0,  1697,   128,   122,   123,
     124,   125,   126,   127,     0,  1699,   128,   122,   123,   124,
     125,   126,   127,     0,  1700,   128,   122,   123,   124,   125,
     126,   127,     0,  1701,   128,   122,   123,   124,   125,   126,
     127,     0,  1703,   128,   122,   123,   124,   125,   126,   127,
       0,  1705,   128,   122,   123,   124,   125,   126,   127,     0,
    1712,   128,   122,   123,   124,   125,   126,   127,     0,  1713,
     128,   122,   123,   124,   125,   126,   127,     0,  1730,   128,
     122,   123,   124,   125,   126,   127,     0,  1734,   128,   122,
     123,   124,   125,   126,   127,     0,  1735,   128,   122,   123,
     124,   125,   126,   127,     0,  1766,   128,   122,   123,   124,
     125,   126,   127,     0,  1768,   128,   122,   123,   124,   125,
     126,   127,     0,  1773,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1775,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1776,     0,   122,   123,
     124,   125,   126,   127,     0,  1778,   128,   122,   123,   124,
     125,   126,   127,     0,  1779,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,  1780,     0,
     122,   123,   124,   125,   126,   127,     0,  1782,   128,   122,
     123,   124,   125,   126,   127,     0,  1784,   128,   122,   123,
     124,   125,   126,   127,     0,  1791,   128,   122,   123,   124,
     125,   126,   127,     0,  1792,   128,   122,   123,   124,   125,
     126,   127,     0,  1839,   128,   122,   123,   124,   125,   126,
     127,     0,  1842,   128,   122,   123,   124,   125,   126,   127,
       0,  1847,   128,   122,   123,   124,   125,   126,   127,     0,
    1848,   128,   122,   123,   124,   125,   126,   127,     0,  1850,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1851,     0,   122,   123,   124,   125,
     126,   127,     0,  1852,   128,   122,   123,   124,   125,   126,
     127,     0,  1854,   128,   122,   123,   124,   125,   126,   127,
       0,  1855,   128,   122,   123,   124,   125,   126,   127,     0,
    1863,   128,   122,   123,   124,   125,   126,   127,     0,  1864,
     128,   122,   123,   124,   125,   126,   127,     0,  1908,   128,
     122,   123,   124,   125,   126,   127,     0,  1916,   128,     0,
     122,   123,   124,   125,   126,   127,     0,  1917,   128,   122,
     123,   124,   125,   126,   127,     0,  1919,   128,   122,   123,
     124,   125,   126,   127,     0,  1920,   128,   122,   123,   124,
     125,   126,   127,     0,  1921,   128,   122,   123,   124,   125,
     126,   127,     0,  1923,   128,   122,   123,   124,   125,   126,
     127,     0,  1924,   128,     0,   122,   123,   124,   125,   126,
     127,     0,  1929,   128,   122,   123,   124,   125,   126,   127,
       0,  1932,   128,   122,   123,   124,   125,   126,   127,     0,
    1933,   128,   122,   123,   124,   125,   126,   127,     0,  1975,
     128,   122,   123,   124,   125,   126,   127,     0,  1977,   128,
     122,   123,   124,   125,   126,   127,     0,  1978,   128,   122,
     123,   124,   125,   126,   127,     0,  1980,   128,   122,   123,
     124,   125,   126,   127,     0,  1981,   128,   122,   123,   124,
     125,   126,   127,     0,  1982,   128,   122,   123,   124,   125,
     126,   127,     0,  1984,   128,   122,   123,   124,   125,   126,
     127,     0,  1990,   128,   122,   123,   124,   125,   126,   127,
       0,  1993,   128,   122,   123,   124,   125,   126,   127,     0,
    1994,   128,   122,   123,   124,   125,   126,   127,     0,  2013,
     128,   122,   123,   124,   125,   126,   127,     0,  2018,   128,
     122,   123,   124,   125,   126,   127,     0,  2029,   128,   122,
     123,   124,   125,   126,   127,     0,  2030,   128,   122,   123,
     124,   125,   126,   127,     0,  2031,   128,   122,   123,   124,
     125,   126,   127,     0,  2032,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  2034,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,  2035,     0,   122,
     123,   124,   125,   126,   127,     0,  2036,   128,   122,   123,
     124,   125,   126,   127,     0,  2038,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2047,
       0,   122,   123,   124,   125,   126,   127,     0,  2048,   128,
     122,   123,   124,   125,   126,   127,     0,  2064,   128,   122,
     123,   124,   125,   126,   127,     0,  2068,   128,   122,   123,
     124,   125,   126,   127,     0,  2076,   128,   122,   123,   124,
     125,   126,   127,     0,  2077,   128,   122,   123,   124,   125,
     126,   127,     0,  2078,   128,   122,   123,   124,   125,   126,
     127,     0,  2079,   128,   122,   123,   124,   125,   126,   127,
       0,  2081,   128,   122,   123,   124,   125,   126,   127,     0,
    2082,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,  2083,     0,   122,   123,   124,
     125,   126,   127,     0,  2085,   128,   122,   123,   124,   125,
     126,   127,     0,  2088,   128,   122,   123,   124,   125,   126,
     127,     0,  2089,   128,   122,   123,   124,   125,   126,   127,
       0,  2090,   128,   122,   123,   124,   125,   126,   127,     0,
    2093,   128,   122,   123,   124,   125,   126,   127,     0,  2094,
     128,     0,   122,   123,   124,   125,   126,   127,     0,  2109,
     128,   122,   123,   124,   125,   126,   127,     0,  2121,   128,
     122,   123,   124,   125,   126,   127,     0,  2122,   128,   122,
     123,   124,   125,   126,   127,     0,  2123,   128,   122,   123,
     124,   125,   126,   127,     0,  2124,   128,   122,   123,   124,
     125,   126,   127,     0,  2126,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  2127,   128,   122,   123,   124,   125,
     126,   127,     0,  2128,   128,   122,   123,   124,   125,   126,
     127,     0,  2130,   128,   122,   123,   124,   125,   126,   127,
       0,  2133,   128,   122,   123,   124,   125,   126,   127,     0,
    2134,   128,   122,   123,   124,   125,   126,   127,     0,  2135,
     128,   122,   123,   124,   125,   126,   127,     0,  2137,   128,
     122,   123,   124,   125,   126,   127,     0,  2138,   128,   122,
     123,   124,   125,   126,   127,     0,  2152,   128,   122,   123,
     124,   125,   126,   127,     0,  2163,   128,   122,   123,   124,
     125,   126,   127,     0,  2164,   128,   122,   123,   124,   125,
     126,   127,     0,  2165,   128,   122,   123,   124,   125,   126,
     127,     0,  2166,   128,   122,   123,   124,   125,   126,   127,
       0,  2168,   128,   122,   123,   124,   125,   126,   127,     0,
    2169,   128,   122,   123,   124,   125,   126,   127,     0,  2170,
     128,   122,   123,   124,   125,   126,   127,     0,  2172,   128,
     122,   123,   124,   125,   126,   127,     0,  2179,   128,   122,
     123,   124,   125,   126,   127,     0,  2180,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  2205,   128,   122,   123,
     124,   125,   126,   127,     0,  2206,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  2207,     0,
     122,   123,   124,   125,   126,   127,     0,  2208,   128,   122,
     123,   124,   125,   126,   127,     0,  2210,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
    2211,     0,   122,   123,   124,   125,   126,   127,     0,  2212,
     128,   122,   123,   124,   125,   126,   127,     0,  2214,   128,
     122,   123,   124,   125,   126,   127,     0,  2216,   128,   122,
     123,   124,   125,   126,   127,     0,  2221,   128,   122,   123,
     124,   125,   126,   127,     0,  2222,   128,   122,   123,   124,
     125,   126,   127,     0,  2247,   128,   122,   123,   124,   125,
     126,   127,     0,  2248,   128,   122,   123,   124,   125,   126,
     127,     0,  2249,   128,   122,   123,   124,   125,   126,   127,
       0,  2250,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  2252,     0,   122,   123,
     124,   125,   126,   127,     0,  2253,   128,   122,   123,   124,
     125,   126,   127,     0,  2254,   128,   122,   123,   124,   125,
     126,   127,     0,  2256,   128,   122,   123,   124,   125,   126,
     127,     0,  2258,   128,   122,   123,   124,   125,   126,   127,
       0,  2263,   128,     0,   122,   123,   124,   125,   126,   127,
       0,  2264,   128,   122,   123,   124,   125,   126,   127,     0,
    2280,   128,   122,   123,   124,   125,   126,   127,     0,  2281,
     128,   122,   123,   124,   125,   126,   127,     0,  2282,   128,
     122,   123,   124,   125,   126,   127,     0,  2289,   128,   122,
     123,   124,   125,   126,   127,     0,  2291,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  2292,   128,   122,   123,
     124,   125,   126,   127,     0,  2295,   128,     0,   122,   123,
     124,   125,   126,   127,  2298,     0,   128,     0,     0,     0,
       0,     0,     0,  2299,     0,   122,   123,   124,   125,   126,
     127,     0,  2306,   128,   122,   123,   124,   125,   126,   127,
       0,  2320,   128,   122,   123,   124,   125,   126,   127,     0,
    2321,   128,   122,   123,   124,   125,   126,   127,     0,  2322,
     128,   122,   123,   124,   125,   126,   127,     0,  2329,   128,
     122,   123,   124,   125,   126,   127,     0,  2330,   128,   122,
     123,   124,   125,   126,   127,     0,  2331,   128,   122,   123,
     124,   125,   126,   127,     0,  2334,   128,   122,   123,   124,
     125,   126,   127,     0,  2337,   128,   122,   123,   124,   125,
     126,   127,     0,  2338,   128,   122,   123,   124,   125,   126,
     127,     0,  2344,   128,   122,   123,   124,   125,   126,   127,
       0,  2368,   128,   122,   123,   124,   125,   126,   127,     0,
    2369,   128,   122,   123,   124,   125,   126,   127,     0,  2372,
     128,   122,   123,   124,   125,   126,   127,     0,  2375,   128,
       0,   122,   123,   124,   125,   126,   127,     0,  2382,   128,
     122,   123,   124,   125,   126,   127,     0,  2402,   128,   122,
     123,   124,   125,   126,   127,     0,  2403,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,  2408,
       0,   122,   123,   124,   125,   126,   127,     0,  2414,   128,
     122,   123,   124,   125,   126,   127,     0,  2434,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  2435,     0,     0,     0,    75,    76,     0,    77,    78,
    2438,    79,    80,    81,     0,    82,    83,    84,     0,  2439,
       0,   122,   123,   124,   125,   126,   127,    85,  2440,   128,
     122,   123,   124,   125,   126,   127,     0,  2441,   128,   122,
     123,   124,   125,   126,   127,     0,  2443,   128,   122,   123,
     124,   125,   126,   127,     0,  2459,   128,   122,   123,   124,
     125,   126,   127,     0,  2460,   128,   122,   123,   124,   125,
     126,   127,     0,  2463,   128,   122,   123,   124,   125,   126,
     127,     0,  2464,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,  2465,     0,   122,
     123,   124,   125,   126,   127,     0,  2466,   128,   122,   123,
     124,   125,   126,   127,     0,  2468,   128,   122,   123,   124,
     125,   126,   127,     0,  2482,   128,   122,   123,   124,   125,
     126,   127,     0,  2483,   128,     0,   122,   123,   124,   125,
     126,   127,    86,  2485,   128,   122,   123,   124,   125,   126,
     127,     0,  2490,   128,   122,   123,   124,   125,   126,   127,
       0,  2501,   128,   122,   123,   124,   125,   126,   127,     0,
    2504,   128,   122,   123,   124,   125,   126,   127,     0,  2517,
     128,   122,   123,   124,   125,   126,   127,     0,  2520,   128,
       0,     0,   122,   123,   124,   125,   126,   127,  2533,   937,
     128,     0,     0,     0,     0,     0,     0,  2536,     0,    87,
       0,     0,     0,     0,     0,   697,   122,   123,   124,   125,
     126,   127,     0,   448,   128,     0,     0,     0,     0,     0,
    1488,   122,   123,   124,   125,   126,   127,     0,  1314,   128,
       0,     0,     0,     0,     0,  1636,     0,   122,   123,   124,
     125,   126,   127,     0,  1969,   128,   122,   123,   124,   125,
     126,   127,  1204,     0,   128,   122,   123,   124,   125,   126,
     127,  1487,     0,   128,   122,   123,   124,   125,   126,   127,
    1869,     0,   128,     0,     0,     0,     0,     0,  1002,     0,
     122,   123,   124,   125,   126,   127,     0,  1003,   128,   122,
     123,   124,   125,   126,   127,     0,  1313,   128,   122,   123,
     124,   125,   126,   127,     0,  1761,   128,   122,   123,   124,
     125,   126,   127,  1001,     0,   128,   122,   123,   124,   125,
     126,   127,  1203,     0,   128,   122,   123,   124,   125,   126,
     127,     0,  1388,   128,   122,   123,   124,   125,   126,   127,
     767,     0,   128,   122,   123,   124,   125,   126,   127,   768,
       0,   128,   122,   123,   124,   125,   126,   127,   769,     0,
     128,     0,   122,   123,   124,   125,   126,   127,     0,  1312,
     128,  1123,     0,   122,   123,   124,   125,   126,   127,     0,
    1637,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,    88,     0,     0,     0,     0,   535,   122,
     123,   124,   125,   126,   127,   701,     0,   128,     0,     0,
       0,   450,     0,   122,   123,   124,   125,   126,   127,     0,
     938,   128,   122,   123,   124,   125,   126,   127,   698,     0,
     128,     0,     0,     0,     0,     0,   776,   122,   123,   124,
     125,   126,   127,     0,   530,   128,   122,   123,   124,   125,
     126,   127,   264,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1495,   122,   123,   124,
     125,   126,   127,     0,  1321,   128,   122,   123,   124,   125,
     126,   127,  1131,     0,   128,     0,     0,     0,     0,     0,
     914,     0,   122,   123,   124,   125,   126,   127,     0,   672,
     128,   122,   123,   124,   125,   126,   127,  1868,     0,   128,
       0,     0,     0,     0,     0,  1760,   122,   123,   124,   125,
     126,   127,     0,  1634,   128,   122,   123,   124,   125,   126,
     127,  1484,     0,   128,     0,     0,     0,     0,     0,  1310,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1120,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,  1482,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  1308,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,     0,
       0,  1118,   122,   123,   124,   125,   126,   127,     0,   901,
     128,   122,   123,   124,   125,   126,   127,   658,     0,   128,
       0,     0,     0,     0,     0,  2415,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,     0,
    2364,   122,   123,   124,   125,   126,   127,     0,  2307,   128,
     122,   123,   124,   125,   126,   127,  2244,     0,   128,     0,
       0,     0,     0,     0,  2181,   122,   123,   124,   125,   126,
     127,     0,  2118,   128,   122,   123,   124,   125,   126,   127,
    2049,     0,   128,     0,     0,     0,     0,     0,  1965,   122,
     123,   124,   125,   126,   127,     0,  1865,   128,   122,   123,
     124,   125,   126,   127,  1756,     0,   128,   122,   123,   124,
     125,   126,   127,  1630,     0,   128,   122,   123,   124,   125,
     126,   127,     0,  1480,   128,   122,   123,   124,   125,   126,
     127,  1306,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,  1116,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
     899,   122,   123,   124,   125,   126,   127,     0,   656,   128,
       0,     0,  1635,   122,   123,   124,   125,   126,   127,     0,
    1486,   128,   122,   123,   124,   125,   126,   127,  1122,     0,
     128,     0,   122,   123,   124,   125,   126,   127,     0,   905,
     128,   122,   123,   124,   125,   126,   127,   662,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,   906,     0,   122,   123,   124,   125,   126,   127,     0,
     907,   128,   122,   123,   124,   125,   126,   127,     0,  2492,
     128,   122,   123,   124,   125,   126,   127,  2458,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  2417,     0,   122,   123,   124,   125,   126,   127,     0,
    2366,   128,   122,   123,   124,   125,   126,   127,  2309,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2246,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,  2183,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,  2120,   122,
     123,   124,   125,   126,   127,     0,  2051,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
    1967,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1867,   122,   123,   124,   125,   126,
     127,     0,  1759,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1633,   122,   123,   124,
     125,   126,   127,     0,  1483,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,   902,   122,
     123,   124,   125,   126,   127,     0,   657,   128,   122,   123,
     124,   125,   126,   127,     0,   659,   128,   122,   123,   124,
     125,   126,   127,  1309,     0,   128,     0,     0,     0,     0,
     900,   122,   123,   124,   125,   126,   127,     0,  2611,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  2596,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  2581,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  2560,   122,   123,   124,   125,   126,
     127,     0,  2539,   128,   122,   123,   124,   125,   126,   127,
       0,  2515,   128,   122,   123,   124,   125,   126,   127,  2491,
       0,   128,     0,     0,     0,     0,     0,  2457,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,  2416,
     122,   123,   124,   125,   126,   127,     0,  2365,   128,   122,
     123,   124,   125,   126,   127,  2308,     0,   128,     0,     0,
       0,     0,     0,  2245,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  2182,   122,   123,   124,   125,
     126,   127,     0,  2119,   128,   122,   123,   124,   125,   126,
     127,  2050,     0,   128,   122,   123,   124,   125,   126,   127,
       0,  1966,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1866,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1757,   122,   123,   124,
     125,   126,   127,     0,  1631,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  1117,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1481,
     122,   123,   124,   125,   126,   127,     0,  1307,   128,   122,
     123,   124,   125,   126,   127,     0,  1762,   128,   122,   123,
     124,   125,   126,   127,     0,  1872,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1870,     0,   122,
     123,   124,   125,   126,   127,     0,  1971,   128,     0,     0,
       0,     0,     0,   904,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,   661,   122,   123,   124,
     125,   126,   127,     0,  1485,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  1311,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  1121,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1315,     0,   122,   123,
     124,   125,   126,   127,     0,  1317,   128,   122,   123,   124,
     125,   126,   127,     0,  1318,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,  1319,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
    1489,     0,   122,   123,   124,   125,   126,   127,     0,  1491,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1492,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1493,     0,   122,   123,
     124,   125,   126,   127,     0,  1638,   128,   122,   123,   124,
     125,   126,   127,     0,  1642,   128,   122,   123,   124,   125,
     126,   127,     0,  1644,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1970,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1639,     0,   122,   123,
     124,   125,   126,   127,     0,  1640,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1641,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1758,
       0,   122,   123,   124,   125,   126,   127,     0,  1763,   128,
     122,   123,   124,   125,   126,   127,     0,  1765,   128,   122,
     123,   124,   125,   126,   127,     0,  1968,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2052,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,     0,  2516,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  1490,     0,   122,   123,   124,
     125,   126,   127,     0,  1494,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  1496,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  1871,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  1316,     0,   122,   123,   124,
     125,   126,   127,     0,  1320,   128,   122,   123,   124,   125,
     126,   127,     0,  1322,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  1764,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  1127,     0,
     122,   123,   124,   125,   126,   127,     0,  1128,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  1129,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,  1130,     0,   122,   123,   124,   125,
     126,   127,     0,  1132,   128,   122,   123,   124,   125,   126,
     127,     0,  1643,   128,     0,     0,     0,     0,     0,  1125,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1126,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,   692,     0,   122,   123,   124,   125,   126,   127,     0,
     897,   128,     0,   122,   123,   124,   125,   126,   127,     0,
     898,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,   903,     0,   122,   123,   124,   125,
     126,   127,     0,   908,   128,   122,   123,   124,   125,   126,
     127,     0,   909,   128,   122,   123,   124,   125,   126,   127,
       0,   910,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   911,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,   912,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
     913,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     915,     0,   122,   123,   124,   125,   126,   127,     0,  1119,
     128,     0,   122,   123,   124,   125,   126,   127,     0,  1632,
     128,     0,   122,   123,   124,   125,   126,   127,     0,   663,
     128,   122,   123,   124,   125,   126,   127,   654,     0,   128,
       0,     0,     0,     0,     0,     0,   655,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,   660,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,   664,     0,   122,   123,   124,   125,   126,   127,     0,
     665,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,   666,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,   667,     0,   122,
     123,   124,   125,   126,   127,     0,   668,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,   669,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   670,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,   671,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   673,     0,   122,   123,   124,   125,
     126,   127,     0,  1167,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  1168,     0,   122,
     123,   124,   125,   126,   127,     0,  1169,   128,   122,   123,
     124,   125,   126,   127,  1143,     0,   128,   122,   123,   124,
     125,   126,   127,  1351,     0,   128,   122,   123,   124,   125,
     126,   127,     0,  1409,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,  1418,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  1892,     0,     0,     0,     0,  1166,     0,     0,     0,
       0,     0,  1893,   932,     0,   122,   123,   124,   125,   126,
     127,     0,  1894,   128,     0,     0,     0,     0,     0,  1357,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  1470,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,  1471,     0,   122,   123,   124,
     125,   126,   127,     0,  1473,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  1145,   128,   122,   123,   124,   125,
     126,   127,     0,  1788,   128,     0,   936,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1896,     0,     0,
       0,     0,     0,  1520,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1704,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1987,     0,   122,
     123,   124,   125,   126,   127,     0,  1988,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
    1989,   122,   123,   124,   125,   126,   127,  1667,     0,   128,
     122,   123,   124,   125,   126,   127,  1789,     0,   128,     0,
       0,     0,     0,     0,  1528,     0,   122,   123,   124,   125,
     126,   127,     0,  1668,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1360,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,  1529,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1175,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1361,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
    1570,     0,   122,   123,   124,   125,   126,   127,     0,  1144,
     128,   122,   123,   124,   125,   126,   127,     0,  1664,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1665,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,  1666,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,   933,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    1785,     0,   122,   123,   124,   125,   126,   127,     0,  1786,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,  1787,   122,   123,   124,   125,   126,   127,     0,
    1503,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,  1521,     0,   122,   123,
     124,   125,   126,   127,     0,  1522,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1523,   122,   123,
     124,   125,   126,   127,     0,  1330,   128,   122,   123,   124,
     125,   126,   127,     0,  1352,   128,   122,   123,   124,   125,
     126,   127,     0,  1353,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1354,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,   693,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    1237,   122,   123,   124,   125,   126,   127,  2388,     0,   128,
     122,   123,   124,   125,   126,   127,  2391,     0,   128,   122,
     123,   124,   125,   126,   127,     0,  2396,   128,   122,   123,
     124,   125,   126,   127,     0,  2603,   128,   122,   123,   124,
     125,   126,   127,     0,  2604,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  2641,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,  2332,     0,   122,
     123,   124,   125,   126,   127,     0,  2336,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2342,     0,
     122,   123,   124,   125,   126,   127,     0,  2588,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,     0,  2589,     0,   122,   123,   124,   125,   126,
     127,     0,  2635,   128,     0,   122,   123,   124,   125,   126,
     127,     0,  2272,   128,   122,   123,   124,   125,   126,   127,
       0,  2276,   128,   122,   123,   124,   125,   126,   127,     0,
    2283,   128,   122,   123,   124,   125,   126,   127,     0,  2570,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2572,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  2629,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  2209,     0,   122,   123,   124,   125,
     126,   127,     0,  2213,   128,   122,   123,   124,   125,   126,
     127,     0,  2220,   128,   122,   123,   124,   125,   126,   127,
       0,  2549,   128,   122,   123,   124,   125,   126,   127,     0,
    2551,   128,   122,   123,   124,   125,   126,   127,     0,  2623,
     128,   122,   123,   124,   125,   126,   127,     0,  2146,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  2150,     0,   122,   123,   124,   125,   126,   127,     0,
    2157,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  2527,     0,   122,   123,   124,   125,   126,   127,
       0,  2529,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,  2615,     0,   122,
     123,   124,   125,   126,   127,     0,  2080,   128,   122,   123,
     124,   125,   126,   127,     0,  2084,   128,   122,   123,   124,
     125,   126,   127,     0,  2092,   128,   122,   123,   124,   125,
     126,   127,     0,  2503,   128,   122,   123,   124,   125,   126,
     127,     0,  2505,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  2600,     0,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  2006,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  2010,     0,     0,   122,
     123,   124,   125,   126,   127,  2019,     0,   128,   122,   123,
     124,   125,   126,   127,  2473,     0,   128,   122,   123,   124,
     125,   126,   127,  2478,     0,   128,   122,   123,   124,   125,
     126,   127,  2585,     0,   128,     0,     0,     0,     0,     0,
    1918,     0,   122,   123,   124,   125,   126,   127,     0,  1922,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,  1931,     0,   122,   123,   124,   125,   126,   127,
       0,  2436,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  2442,     0,   122,   123,   124,   125,   126,
     127,     0,  2566,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,  1813,     0,   122,
     123,   124,   125,   126,   127,     0,  1817,   128,   122,   123,
     124,   125,   126,   127,     0,  1826,   128,     0,   122,   123,
     124,   125,   126,   127,  2389,  1176,   128,     0,     0,   548,
       0,     0,     0,  2397,     0,     0,   122,   123,   124,   125,
     126,   127,  2545,   550,   128,   122,   123,   124,   125,   126,
     127,     0,   554,   128,   122,   123,   124,   125,   126,   127,
       0,   557,   128,     0,  2612,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
    2646,     0,   128,     0,     0,     0,     0,     0,  2597,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  2640,   122,   123,   124,   125,   126,   127,     0,  2582,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2634,   122,   123,   124,   125,   126,   127,     0,  2561,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,  1274,  2628,     0,   122,   123,   124,   125,
     126,   127,     0,  1275,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,  1276,   128,   122,   123,   124,   125,   126,   127,     0,
    1338,   128,   122,   123,   124,   125,   126,   127,     0,  1393,
     128,     0,   122,   123,   124,   125,   126,   127,     0,  1084,
     128,   122,   123,   124,   125,   126,   127,     0,  1085,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,  1086,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,  1152,     0,   122,   123,
     124,   125,   126,   127,     0,  1211,   128,     0,   122,   123,
     124,   125,   126,   127,     0,   864,   128,   122,   123,   124,
     125,   126,   127,     0,   865,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,   866,
       0,   122,   123,   124,   125,   126,   127,     0,   944,   128,
     122,   123,   124,   125,   126,   127,     0,  1013,   128,   122,
     123,   124,   125,   126,   127,  1138,     0,   128,     0,     0,
     862,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     868,     0,   122,   123,   124,   125,   126,   127,     0,   994,
     128,   122,   123,   124,   125,   126,   127,     0,  1198,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,   347,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  1386,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  1554,   122,   123,   124,   125,   126,   127,
       0,  1327,   128,   122,   123,   124,   125,   126,   127,     0,
    1691,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,  1807,   122,   123,   124,   125,
     126,   127,     0,   999,   128,   122,   123,   124,   125,   126,
     127,     0,  1200,   128,     0,     0,  1555,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1690,   122,   123,
     124,   125,   126,   127,     0,  1201,   128,   122,   123,   124,
     125,   126,   127,     0,  1385,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   998,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,   605,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   618,     0,     0,   258,
       0,     0,     0,     0,     0,   624,     0,     0,   122,   123,
     124,   125,   126,   127,   925,   259,   128,   122,   123,   124,
     125,   126,   127,     0,   253,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,   254,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,   263,     0,   122,
     123,   124,   125,   126,   127,     0,   595,   128,   122,   123,
     124,   125,   126,   127,     0,   596,   128,   122,   123,   124,
     125,   126,   127,     0,   185,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   247,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,   523,     0,   122,
     123,   124,   125,   126,   127,     0,   536,   128,   122,   123,
     124,   125,   126,   127,     0,   598,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1075,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1100,     0,     0,
       0,   255,     0,   122,   123,   124,   125,   126,   127,     0,
     256,   128,   122,   123,   124,   125,   126,   127,     0,   257,
     128,     0,   122,   123,   124,   125,   126,   127,     0,   612,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,   691,     0,   122,   123,   124,   125,   126,   127,     0,
    1177,   128,     0,     0,     0,     0,   270,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   271,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,  1020,     0,   128,     0,     0,     0,     0,   604,
     122,   123,   124,   125,   126,   127,     0,   252,   128,     0,
       0,     0,     0,     0,  1154,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,   709,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,   946,     0,
       0,     0,     0,     0,     0,   993,     0,  2540,     0,     0,
       0,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  2622,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
       0,   309,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,     0,   320,   321,   322,   323,   324,   325,
     326,     0,     0,     0,   327,   328,   329,   330,   331,   332,
     333,   334
  };

  const short int
  feiparser::yycheck_[] =
  {
       8,     9,   116,    35,    36,    37,    38,    39,    40,    56,
      55,    43,    20,    21,    22,   271,    35,    36,    37,    38,
      39,    40,     0,   473,    43,    66,   111,    35,    35,    36,
      37,    38,    39,    40,   119,    43,    43,    35,    36,    37,
      38,    39,    40,   130,   100,    43,   103,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,   113,    35,    36,
      37,    38,    39,    40,   473,    73,    43,    55,    56,     3,
       4,     5,     6,    51,     8,     9,    35,    36,    37,    38,
      39,    40,    90,   473,    43,     3,   173,   100,   100,   176,
     177,   148,    53,    27,    28,    29,    30,    31,    32,    33,
     113,   113,    36,   359,    98,   113,    51,   101,    42,    40,
     197,   105,    43,   476,   122,   123,   124,   125,   126,   127,
     128,    55,   175,   111,   465,   178,    35,    36,    37,    38,
      39,    40,    39,    40,    43,    55,    43,   145,   146,    92,
      93,   149,   150,    72,   152,   153,   154,   155,   156,   474,
     475,   159,   160,    55,   162,    89,    55,    56,   166,   167,
     168,   169,   170,   171,   205,   131,   100,   465,    35,    36,
      37,    38,    39,    40,   184,   185,    43,    35,    36,    37,
      38,    39,    40,    55,    56,    43,   128,    59,   472,    61,
     198,     0,   479,    65,     3,     4,     5,     6,     3,     8,
       9,    10,    74,    43,    13,   144,   145,    16,   216,    18,
     116,    20,   139,   140,   141,     5,   143,   144,    27,    28,
      29,    30,    31,    32,    33,   233,   116,    36,   256,   257,
     258,   116,   260,    42,   116,    44,    45,    46,    47,   165,
     166,    50,   116,    52,   460,    54,    55,    35,    36,    37,
      38,    39,    40,   165,   166,    43,   441,   442,   443,   267,
     116,   269,    71,   116,   272,   273,   274,   116,   276,    78,
      79,    80,    81,    35,    36,    37,    38,    39,    40,   165,
     166,    43,   116,   291,   131,   116,   191,    96,    97,     3,
       4,     5,     6,   116,     8,     9,    37,    38,    39,    40,
     109,   118,    43,   470,   116,   116,   115,   460,   116,   116,
     116,   127,   184,    27,    28,    29,    30,    31,    32,    33,
     116,   136,    36,   127,    83,    86,    95,   172,    42,    59,
      35,    36,    37,    38,    39,    40,    88,   345,    43,    89,
      73,    55,    56,   132,   352,    35,    36,    37,    38,    39,
      40,   102,    82,    43,   465,    85,    12,    87,   366,   367,
     368,    91,   476,   116,    94,     3,     4,     5,     6,   377,
       8,     9,    65,   113,   382,    55,   384,    55,    55,   137,
     388,   129,    56,   391,   418,   113,   113,   100,    90,    27,
      28,    29,    30,    31,    32,    33,    82,   100,    36,     5,
      43,   481,   480,   118,    42,   116,    35,    36,    37,    38,
      39,    40,   478,    34,    43,   116,   461,    55,   465,   118,
     428,   429,   116,   431,   116,   433,   434,   224,   423,    55,
      82,   439,   440,   117,   117,   174,   104,   175,   446,   447,
     473,    35,    36,    37,    38,    39,    40,   455,   480,    43,
      55,   111,   116,   461,    55,    34,    55,   465,    58,     3,
     479,    35,    36,    37,    38,    39,    40,    34,   119,    43,
     477,    34,   116,   111,    84,    58,   474,   475,   232,   179,
     184,    58,   459,    34,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,    35,    36,    37,    38,    39,    40,
      82,    82,    43,    82,   100,   474,   475,   113,   526,   527,
     528,   529,   113,   113,   113,   533,   534,   295,   113,   537,
     295,   113,   113,   113,    57,   113,   365,   545,   546,   473,
     270,   549,   363,   551,   552,   553,   295,   555,   556,   113,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   475,   113,   113,   295,
     113,     3,     4,     5,     6,   113,     8,     9,   113,   113,
     113,   113,   363,    35,    36,    37,    38,    39,    40,   113,
     296,    43,   600,   601,   602,    27,    28,    29,    30,    31,
      32,    33,   610,   116,    36,   116,   295,   615,   475,   116,
      42,   121,   116,   295,   295,   116,   295,   475,   113,   627,
     295,   113,   295,    55,    35,    36,    37,    38,    39,    40,
     295,   113,    43,    35,    36,    37,    38,    39,    40,   116,
     295,    43,    35,    36,    37,    38,    39,    40,   295,   295,
      43,    34,   267,   295,   295,   266,   465,   296,   116,   468,
     469,   296,   471,   116,   473,   116,   175,   186,   186,   186,
     479,    35,    36,    37,    38,    39,    40,   419,   184,    43,
     116,   103,   690,   103,   121,    34,   121,   475,   121,   134,
     121,   121,   700,   121,   702,   121,   704,   705,   706,    34,
     708,    34,   121,   121,   712,   121,   714,   715,   716,   121,
     718,   719,   474,   721,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   750,   751,   752,   753,   754,   755,   756,  1005,
     121,   121,    35,    36,    37,    38,    39,    40,   766,   473,
      43,   121,   121,   121,   121,   121,   774,   775,   121,   474,
     778,   779,   780,   121,   782,   121,   121,    34,    34,    34,
     788,    34,   790,    34,    34,   475,    35,    36,    37,    38,
      39,    40,    34,    34,    43,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    35,    36,    37,    38,    39,    40,
      34,    34,    43,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,    34,    34,   124,   444,   444,   444,    34,
     838,   839,   840,    34,    34,   473,   844,   845,   846,    34,
     848,   849,   850,    34,   852,   853,   475,    34,   856,    35,
      36,    37,    38,    39,    40,   863,    34,    43,   184,   117,
      34,   186,   870,   117,   872,   117,   874,   875,   876,   463,
     878,   879,    34,   134,   144,   473,   473,    34,   886,   887,
     888,   889,   119,    34,    34,   473,   894,   895,   896,   473,
      35,    36,    37,    38,    39,    40,    34,   473,    43,   119,
     473,   475,    35,    36,    37,    38,    39,    40,   473,    34,
      43,   919,   920,   921,    35,    36,    37,    38,    39,    40,
     473,   929,    43,   473,   473,   473,   473,   935,    58,   119,
      58,   473,   473,   941,   942,   943,   473,    35,    36,    37,
      38,    39,    40,   473,   475,    43,   473,   473,   473,    58,
     958,    35,    36,    37,    38,    39,    40,   473,   473,    43,
     186,   473,   117,   188,   143,   117,   974,   975,   976,   977,
     978,   979,   980,   981,   982,   983,   984,   985,   986,   987,
     988,   989,   990,   991,   992,    35,    36,    37,    38,    39,
      40,   188,  1000,    43,   188,   188,   119,   103,  1006,  1007,
     461,  1009,  1010,  1011,    34,   462,   119,    67,   137,  1017,
      34,  1019,   117,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,   475,  1032,  1033,  1034,  1035,  1036,  1037,
     116,  1039,    58,  1041,  1042,  1043,  1044,  1045,  1046,   116,
     116,   473,    35,    36,    37,    38,    39,    40,    34,   190,
      43,    35,    36,    37,    38,    39,    40,    34,  1066,    43,
    1068,  1069,  1070,   190,   475,  1073,  1074,    34,   190,    34,
     190,    34,    34,   475,    34,  1083,    35,    36,    37,    38,
      39,    40,   475,    34,    43,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,    34,    34,    34,    34,    34,
      34,  1109,    35,    36,    37,    38,    39,    40,    34,    34,
      43,   475,    35,    36,    37,    38,    39,    40,    34,    34,
      43,    35,    36,    37,    38,    39,    40,   122,   122,    43,
     122,    77,    35,    36,    37,    38,    39,    40,   117,   117,
      43,  1149,  1150,  1151,   110,    34,    55,  1155,  1156,  1157,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,   112,    35,    36,
      37,    38,    39,    40,    58,    34,    43,    34,    65,   116,
    1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,
    1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,
      34,  1199,   475,    34,  1202,    34,    58,   116,  1206,  1207,
      34,  1209,  1210,   249,   249,   249,   249,  1215,  1216,  1217,
      35,    36,    37,    38,    39,    40,    34,    34,    43,    34,
      77,  1229,  1230,  1231,  1232,    77,   475,  1235,   121,   116,
    1238,  1239,    34,    34,    34,   475,    55,    35,    36,    37,
      38,    39,    40,    34,   475,    43,    35,    36,    37,    38,
      39,    40,    34,   475,    43,  1263,  1264,   116,   116,  1267,
      34,   193,  1270,   116,   113,  1273,   113,   113,   113,   473,
     113,    34,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,
    1288,  1289,  1290,   116,   116,   116,    34,   464,    34,   475,
      34,  1299,    34,   121,  1302,  1303,  1304,  1305,    35,    36,
      37,    38,    39,    40,   113,   121,    43,    35,    36,    37,
      38,    39,    40,   121,   121,    43,  1324,   196,    35,    36,
      37,    38,    39,    40,   117,  1333,    43,  1335,  1336,  1337,
     475,  1339,   233,    35,    36,    37,    38,    39,    40,   194,
     240,    43,   475,   233,    34,   233,    34,  1355,  1356,   233,
    1358,    34,    34,    34,   475,    34,    34,    34,  1366,  1367,
    1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,  1377,
    1378,  1379,  1380,  1381,  1382,    34,    34,   475,    34,  1387,
      34,  1389,  1390,  1391,    34,    34,    34,    34,    34,  1397,
     474,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,
    1408,    34,  1410,  1411,  1412,  1413,    34,    34,  1416,    34,
      34,  1419,  1420,  1421,  1422,  1423,    34,   473,    35,    36,
      37,    38,    39,    40,    34,   475,    43,    35,    36,    37,
      38,    39,    40,    34,  1442,    43,  1444,  1445,     5,    34,
       5,    34,   473,  1451,  1452,   473,  1454,    35,    36,    37,
      38,    39,    40,   473,    34,    43,    34,  1465,    19,    34,
      21,    22,    23,    24,    25,    26,  1474,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,   475,    34,    43,    58,    34,    34,    34,  1497,
      34,   475,    35,    36,    37,    38,    39,    40,    34,    34,
      43,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,    34,    34,   463,    34,   474,  1524,  1525,  1526,   427,
      34,   427,  1530,  1531,  1532,  1533,  1534,  1535,  1536,  1537,
    1538,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,  1547,
    1548,  1549,   475,  1551,   116,  1553,   117,     5,  1556,  1557,
    1558,  1559,   475,   119,    35,    36,    37,    38,    39,    40,
     440,   475,    43,    34,  1572,  1573,  1574,    34,    34,    34,
    1578,  1579,   475,    35,    36,    37,    38,    39,    40,    34,
     117,    43,    35,    36,    37,    38,    39,    40,    58,    34,
      43,    34,  1600,    34,  1602,    34,  1604,  1605,   475,    34,
      34,    34,  1610,    34,  1612,  1613,    34,  1615,  1616,  1617,
      34,  1619,  1620,  1621,    35,    36,    37,    38,    39,    40,
    1628,  1629,    43,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    35,    36,    37,    38,    39,    40,  1646,    34,
      43,    34,  1650,  1651,    35,    36,    37,    38,    39,    40,
     475,    34,    43,    35,    36,    37,    38,    39,    40,    34,
     458,    43,    34,    34,  1672,  1673,  1674,  1675,  1676,  1677,
    1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1686,  1687,
      34,  1689,   462,    34,    34,   474,  1694,    34,  1696,  1697,
     473,  1699,  1700,  1701,    34,  1703,   473,  1705,  1706,  1707,
    1708,  1709,  1710,    34,  1712,  1713,    35,    36,    37,    38,
      39,    40,    67,    34,    43,    35,    36,    37,    38,    39,
      40,    34,  1730,    43,  1732,  1733,  1734,  1735,    34,  1737,
      34,   117,  1740,   117,    34,   116,  1744,   117,   475,    34,
      58,    34,    34,    34,    34,  1753,    34,   475,    35,    36,
      37,    38,    39,    40,   473,    34,    43,   474,  1766,   116,
    1768,    58,    58,    34,    58,  1773,     5,  1775,  1776,   440,
    1778,  1779,  1780,   475,  1782,  1783,  1784,   116,    34,    34,
      34,   116,   116,  1791,  1792,  1793,  1794,  1795,  1796,  1797,
    1798,  1799,  1800,  1801,  1802,   427,  1804,    35,    36,    37,
      38,    39,    40,    34,   116,    43,    34,    34,    34,    34,
      34,    34,    34,  1821,  1822,  1823,    34,  1825,    34,   440,
      35,    36,    37,    38,    39,    40,    34,    34,    43,    34,
      34,  1839,    34,    34,  1842,  1843,    34,    34,    34,  1847,
    1848,  1849,  1850,  1851,  1852,  1853,  1854,  1855,    34,    34,
      34,    34,  1860,    34,  1862,  1863,  1864,    34,   475,    34,
     473,    35,    36,    37,    38,    39,    40,   475,  1876,    43,
      34,  1879,  1880,    34,   473,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,    34,    34,   475,    34,    34,
      34,   450,  1900,  1901,  1902,  1903,  1904,  1905,  1906,  1907,
    1908,   113,  1910,    34,    34,    34,    34,   475,  1916,  1917,
      34,  1919,  1920,  1921,    34,  1923,  1924,    34,  1926,  1927,
    1928,  1929,    34,    34,  1932,  1933,    35,    36,    37,    38,
      39,    40,   475,    34,    43,    34,  1944,    34,  1946,    34,
      34,  1949,    34,    34,    34,  1953,    34,    34,  1956,    34,
      34,    34,    34,  1961,  1962,    34,    34,   473,    35,    36,
      37,    38,    39,    40,    34,  1973,    43,  1975,    34,  1977,
    1978,    34,  1980,  1981,  1982,    34,  1984,    34,   117,   117,
     117,   117,  1990,    34,    34,  1993,  1994,  1995,  1996,  1997,
    1998,  1999,  2000,  2001,   475,    35,    36,    37,    38,    39,
      40,    34,    34,    43,   457,  2013,    34,    34,    34,    34,
    2018,    34,    34,   475,    34,    34,    34,    34,    34,    58,
      58,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,  2037,
    2038,    34,    34,  2041,  2042,  2043,    34,    58,  2046,  2047,
    2048,    35,    36,    37,    38,    39,    40,    34,    34,    43,
      58,    34,    34,    34,   475,  2063,  2064,   473,   116,    34,
    2068,   116,   116,   475,  2072,  2073,  2074,  2075,  2076,  2077,
    2078,  2079,   475,  2081,  2082,  2083,    34,  2085,    34,   116,
    2088,  2089,  2090,    34,   475,  2093,  2094,    34,    34,    34,
      34,    34,    34,   475,    34,  2103,    34,    34,    34,  2107,
      34,  2109,   113,    34,   421,   421,    34,  2115,    34,   473,
      34,    34,    34,  2121,  2122,  2123,  2124,   117,  2126,  2127,
    2128,   247,  2130,    34,    34,  2133,  2134,  2135,    34,  2137,
    2138,  2139,  2140,  2141,    35,    36,    37,    38,    39,    40,
      34,    34,    43,    34,  2152,    34,   475,    34,   113,   473,
      58,    34,    34,    34,   474,  2163,  2164,  2165,  2166,  2167,
    2168,  2169,  2170,  2171,  2172,    34,  2174,    34,   116,    34,
    2178,  2179,  2180,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    35,    36,    37,    38,    39,    40,   475,   473,
      43,   473,   473,    34,  2202,  2203,  2204,  2205,  2206,  2207,
    2208,    34,  2210,  2211,  2212,    34,  2214,    34,  2216,    34,
      34,    34,   247,  2221,  2222,    35,    36,    37,    38,    39,
      40,    34,  2230,    43,    34,    34,  2234,    34,    34,    34,
    2238,  2239,  2240,  2241,    34,    34,   473,   475,   473,  2247,
    2248,  2249,  2250,   473,  2252,  2253,  2254,    34,  2256,  2257,
    2258,    34,   473,    34,    34,  2263,  2264,  2265,  2266,  2267,
     475,    35,    36,    37,    38,    39,    40,    34,    34,    43,
     248,    34,  2280,  2281,  2282,    34,    34,    34,    34,    34,
     473,  2289,    34,  2291,  2292,  2293,   247,  2295,   247,  2297,
    2298,  2299,   247,    34,    34,    34,  2304,    34,  2306,   248,
      34,   475,    35,    36,    37,    38,    39,    40,    34,    34,
      43,    34,  2320,  2321,  2322,   475,    34,   473,  2326,  2327,
    2328,  2329,  2330,  2331,    34,    34,  2334,    34,    34,  2337,
    2338,   473,   473,   473,    34,    34,  2344,    35,    36,    37,
      38,    39,    40,  2351,  2352,    43,  2354,  2355,    34,   117,
      34,    34,   117,  2361,  2362,   117,    34,    58,    34,    34,
    2368,  2369,    58,    58,  2372,   116,   475,  2375,   116,    34,
     116,   248,   248,   248,  2382,  2383,  2384,  2385,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    34,    34,    34,
      34,    34,    34,    34,  2402,  2403,  2404,  2405,   475,  2407,
    2408,  2409,  2410,  2411,  2412,  2413,  2414,    58,    35,    36,
      37,    38,    39,    40,    34,    34,    43,   473,    34,    34,
     473,   473,    34,  2431,  2432,  2433,  2434,  2435,   117,    34,
    2438,  2439,  2440,  2441,   474,  2443,   116,    34,    34,    34,
      34,  2449,  2450,    34,    34,    34,   117,  2455,    34,    34,
     117,  2459,  2460,    34,    58,  2463,  2464,  2465,  2466,    58,
    2468,  2469,  2470,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,    34,  2482,  2483,  2484,  2485,   116,   116,
     474,  2489,  2490,    34,    34,   117,    34,   117,    34,    58,
      58,  2499,  2500,  2501,    34,    34,  2504,    34,   116,   116,
      34,    34,    34,  2511,    34,  2513,    34,    34,    34,  2517,
      34,    34,  2520,    34,    34,  2523,  2524,    34,  2526,    34,
      34,    34,  2530,    34,    34,  2533,    34,  2535,  2536,  2537,
      34,   203,    35,    36,    37,    38,    39,    40,  2546,  2547,
      43,    34,    34,    34,  2552,    34,    34,    -1,  2556,    -1,
    2558,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2567,
    2568,  2569,    -1,  2571,    -1,  2573,    -1,    -1,    -1,  2577,
      -1,  2579,    -1,   474,    -1,    -1,    -1,    -1,  2586,  2587,
      -1,    -1,  2590,    -1,    -1,  2593,  2594,    35,    36,    37,
      38,    39,    40,  2601,  2602,    43,    -1,  2605,    -1,    -1,
    2608,  2609,    -1,    -1,    -1,    -1,    -1,    -1,  2616,  2617,
    2618,    -1,   474,    -1,    -1,    -1,  2624,  2625,    -1,    -1,
      -1,   474,  2630,  2631,    -1,    -1,    -1,    -1,  2636,  2637,
      -1,    -1,    -1,    -1,  2642,  2643,    -1,    -1,     1,  2647,
       3,     4,     5,     6,    -1,     8,     9,    10,    -1,    -1,
      13,    -1,    -1,    16,   474,    18,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    44,    45,    46,    47,    -1,    -1,    50,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,     3,     4,     5,     6,
     474,     8,     9,    10,    -1,    -1,    13,    -1,    71,    16,
      -1,    18,    -1,    20,    -1,    78,    79,    80,    81,    -1,
      27,    28,    29,    30,    31,    32,    33,    -1,    -1,    36,
      -1,    -1,    -1,    96,    97,    42,    -1,    44,    45,    46,
      47,   474,    -1,    50,    -1,    52,   109,    54,    55,    -1,
      -1,    -1,   115,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,   474,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,     3,     4,     5,     6,    -1,     8,
       9,    10,   109,    -1,    13,    -1,    -1,    16,   115,    18,
      -1,    20,    -1,    -1,    -1,    -1,    -1,   474,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    44,    45,    46,    47,    -1,
      -1,    50,    -1,    52,    -1,    54,    55,   474,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    96,    97,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     109,    -1,   474,    -1,    -1,    -1,   115,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   474,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   474,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   465,    -1,    -1,   468,   469,    -1,   471,    -1,
     473,    -1,    -1,    -1,    -1,    -1,   479,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   465,    -1,
      -1,   468,   469,    -1,   471,    -1,   473,    -1,    -1,    -1,
      -1,    -1,   479,    -1,   481,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   474,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   465,    -1,    -1,   468,
     469,    -1,   471,    -1,   473,    -1,    -1,    -1,    -1,    -1,
     479,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   474,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,   474,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,   474,    -1,    43,    35,
      36,    37,    38,    39,    40,   474,    -1,    43,    35,    36,
      37,    38,    39,    40,   474,    -1,    43,    35,    36,    37,
      38,    39,    40,   474,    -1,    43,    35,    36,    37,    38,
      39,    40,   474,    -1,    43,    35,    36,    37,    38,    39,
      40,   474,    -1,    43,    35,    36,    37,    38,    39,    40,
     474,    -1,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   474,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   474,    -1,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   474,    -1,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   474,    -1,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    -1,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   474,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   474,    -1,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   474,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,   474,    -1,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    -1,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   474,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   474,    -1,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   474,    -1,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   474,    -1,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   474,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   474,
      -1,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,   474,    -1,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   474,    -1,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     474,    -1,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   474,    -1,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    -1,    35,    36,
      37,    38,    39,    40,   474,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,   474,    -1,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   474,
      -1,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   474,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
     474,    61,    62,    63,    -1,    65,    66,    67,    -1,   474,
      -1,    35,    36,    37,    38,    39,    40,    77,   474,    43,
      35,    36,    37,    38,    39,    40,    -1,   474,    43,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   474,    -1,    35,
      36,    37,    38,    39,    40,    -1,   474,    43,    35,    36,
      37,    38,    39,    40,    -1,   474,    43,    35,    36,    37,
      38,    39,    40,    -1,   474,    43,    35,    36,    37,    38,
      39,    40,    -1,   474,    43,    -1,    35,    36,    37,    38,
      39,    40,   192,   474,    43,    35,    36,    37,    38,    39,
      40,    -1,   474,    43,    35,    36,    37,    38,    39,    40,
      -1,   474,    43,    35,    36,    37,    38,    39,    40,    -1,
     474,    43,    35,    36,    37,    38,    39,    40,    -1,   474,
      43,    35,    36,    37,    38,    39,    40,    -1,   474,    43,
      -1,    -1,    35,    36,    37,    38,    39,    40,   474,   456,
      43,    -1,    -1,    -1,    -1,    -1,    -1,   474,    -1,   259,
      -1,    -1,    -1,    -1,    -1,   455,    35,    36,    37,    38,
      39,    40,    -1,   454,    43,    -1,    -1,    -1,    -1,    -1,
     452,    35,    36,    37,    38,    39,    40,    -1,   451,    43,
      -1,    -1,    -1,    -1,    -1,   449,    -1,    35,    36,    37,
      38,    39,    40,    -1,   449,    43,    35,    36,    37,    38,
      39,    40,   448,    -1,    43,    35,    36,    37,    38,    39,
      40,   448,    -1,    43,    35,    36,    37,    38,    39,    40,
     448,    -1,    43,    -1,    -1,    -1,    -1,    -1,   447,    -1,
      35,    36,    37,    38,    39,    40,    -1,   447,    43,    35,
      36,    37,    38,    39,    40,    -1,   447,    43,    35,    36,
      37,    38,    39,    40,    -1,   447,    43,    35,    36,    37,
      38,    39,    40,   446,    -1,    43,    35,    36,    37,    38,
      39,    40,   446,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,   446,    43,    35,    36,    37,    38,    39,    40,
     445,    -1,    43,    35,    36,    37,    38,    39,    40,   445,
      -1,    43,    35,    36,    37,    38,    39,    40,   445,    -1,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   425,
      43,   445,    -1,    35,    36,    37,    38,    39,    40,    -1,
     445,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   453,    -1,    -1,    -1,    -1,   422,    35,
      36,    37,    38,    39,    40,   420,    -1,    43,    -1,    -1,
      -1,   417,    -1,    35,    36,    37,    38,    39,    40,    -1,
     417,    43,    35,    36,    37,    38,    39,    40,   416,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   415,    35,    36,    37,
      38,    39,    40,    -1,   414,    43,    35,    36,    37,    38,
      39,    40,   413,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   412,    35,    36,    37,
      38,    39,    40,    -1,   411,    43,    35,    36,    37,    38,
      39,    40,   410,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     409,    -1,    35,    36,    37,    38,    39,    40,    -1,   408,
      43,    35,    36,    37,    38,    39,    40,   407,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   406,    35,    36,    37,    38,
      39,    40,    -1,   405,    43,    35,    36,    37,    38,    39,
      40,   404,    -1,    43,    -1,    -1,    -1,    -1,    -1,   403,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   402,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   401,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   400,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,   399,    35,    36,    37,    38,    39,    40,    -1,   398,
      43,    35,    36,    37,    38,    39,    40,   397,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   396,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     395,    35,    36,    37,    38,    39,    40,    -1,   394,    43,
      35,    36,    37,    38,    39,    40,   393,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   392,    35,    36,    37,    38,    39,
      40,    -1,   391,    43,    35,    36,    37,    38,    39,    40,
     390,    -1,    43,    -1,    -1,    -1,    -1,    -1,   389,    35,
      36,    37,    38,    39,    40,    -1,   388,    43,    35,    36,
      37,    38,    39,    40,   387,    -1,    43,    35,    36,    37,
      38,    39,    40,   386,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,   385,    43,    35,    36,    37,    38,    39,
      40,   384,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   383,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     382,    35,    36,    37,    38,    39,    40,    -1,   381,    43,
      -1,    -1,   370,    35,    36,    37,    38,    39,    40,    -1,
     369,    43,    35,    36,    37,    38,    39,    40,   368,    -1,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   367,
      43,    35,    36,    37,    38,    39,    40,   366,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   364,    -1,    35,    36,    37,    38,    39,    40,    -1,
     364,    43,    35,    36,    37,    38,    39,    40,    -1,   358,
      43,    35,    36,    37,    38,    39,    40,   357,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   356,    -1,    35,    36,    37,    38,    39,    40,    -1,
     355,    43,    35,    36,    37,    38,    39,    40,   354,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   353,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   352,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   351,    35,
      36,    37,    38,    39,    40,    -1,   350,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     349,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   348,    35,    36,    37,    38,    39,
      40,    -1,   347,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   346,    35,    36,    37,
      38,    39,    40,    -1,   345,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   344,    35,
      36,    37,    38,    39,    40,    -1,   343,    43,    35,    36,
      37,    38,    39,    40,    -1,   343,    43,    35,    36,    37,
      38,    39,    40,   342,    -1,    43,    -1,    -1,    -1,    -1,
     340,    35,    36,    37,    38,    39,    40,    -1,   339,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   338,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   337,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   336,    35,    36,    37,    38,    39,
      40,    -1,   335,    43,    35,    36,    37,    38,    39,    40,
      -1,   334,    43,    35,    36,    37,    38,    39,    40,   333,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   332,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   331,
      35,    36,    37,    38,    39,    40,    -1,   330,    43,    35,
      36,    37,    38,    39,    40,   329,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   328,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   327,    35,    36,    37,    38,
      39,    40,    -1,   326,    43,    35,    36,    37,    38,    39,
      40,   325,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,   324,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   323,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   322,    35,    36,    37,
      38,    39,    40,    -1,   321,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   320,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   319,
      35,    36,    37,    38,    39,    40,    -1,   318,    43,    35,
      36,    37,    38,    39,    40,    -1,   314,    43,    35,    36,
      37,    38,    39,    40,    -1,   314,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   313,    -1,    35,
      36,    37,    38,    39,    40,    -1,   313,    43,    -1,    -1,
      -1,    -1,    -1,   311,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   310,    35,    36,    37,
      38,    39,    40,    -1,   309,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   308,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   307,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   306,    -1,    35,    36,
      37,    38,    39,    40,    -1,   306,    43,    35,    36,    37,
      38,    39,    40,    -1,   306,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   306,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
     305,    -1,    35,    36,    37,    38,    39,    40,    -1,   305,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   305,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   305,    -1,    35,    36,
      37,    38,    39,    40,    -1,   305,    43,    35,    36,    37,
      38,    39,    40,    -1,   305,    43,    35,    36,    37,    38,
      39,    40,    -1,   305,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   305,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   304,    -1,    35,    36,
      37,    38,    39,    40,    -1,   304,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   304,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   304,
      -1,    35,    36,    37,    38,    39,    40,    -1,   304,    43,
      35,    36,    37,    38,    39,    40,    -1,   304,    43,    35,
      36,    37,    38,    39,    40,    -1,   304,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   304,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   304,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   303,    -1,    35,    36,    37,
      38,    39,    40,    -1,   303,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   303,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   303,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   302,    -1,    35,    36,    37,
      38,    39,    40,    -1,   302,    43,    35,    36,    37,    38,
      39,    40,    -1,   302,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   302,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   301,    -1,
      35,    36,    37,    38,    39,    40,    -1,   301,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   301,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   301,    -1,    35,    36,    37,    38,
      39,    40,    -1,   301,    43,    35,    36,    37,    38,    39,
      40,    -1,   301,    43,    -1,    -1,    -1,    -1,    -1,   299,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   299,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   298,    -1,    35,    36,    37,    38,    39,    40,    -1,
     298,    43,    -1,    35,    36,    37,    38,    39,    40,    -1,
     298,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   298,    -1,    35,    36,    37,    38,
      39,    40,    -1,   298,    43,    35,    36,    37,    38,    39,
      40,    -1,   298,    43,    35,    36,    37,    38,    39,    40,
      -1,   298,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   298,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   298,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     298,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     298,    -1,    35,    36,    37,    38,    39,    40,    -1,   298,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   298,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   297,
      43,    35,    36,    37,    38,    39,    40,   296,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   296,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   296,    -1,    35,    36,    37,    38,    39,    40,    -1,
     296,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   296,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   296,    -1,    35,
      36,    37,    38,    39,    40,    -1,   296,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   296,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   296,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   296,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   296,    -1,    35,    36,    37,    38,
      39,    40,    -1,   296,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   296,    -1,    35,
      36,    37,    38,    39,    40,    -1,   296,    43,    35,    36,
      37,    38,    39,    40,   295,    -1,    43,    35,    36,    37,
      38,    39,    40,   295,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,   295,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   295,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   295,    -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,
      -1,    -1,   295,   267,    -1,    35,    36,    37,    38,    39,
      40,    -1,   295,    43,    -1,    -1,    -1,    -1,    -1,   267,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   267,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,   267,    -1,    35,    36,    37,
      38,    39,    40,    -1,   267,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   265,    43,    35,    36,    37,    38,
      39,    40,    -1,   250,    43,    -1,   262,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   250,    -1,    -1,
      -1,    -1,    -1,   246,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   246,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   246,    -1,    35,
      36,    37,    38,    39,    40,    -1,   246,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     246,    35,    36,    37,    38,    39,    40,   244,    -1,    43,
      35,    36,    37,    38,    39,    40,   244,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   243,    -1,    35,    36,    37,    38,
      39,    40,    -1,   243,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   242,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,   242,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   241,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   241,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
     238,    -1,    35,    36,    37,    38,    39,    40,    -1,   237,
      43,    35,    36,    37,    38,    39,    40,    -1,   237,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   237,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,   237,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   236,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     236,    -1,    35,    36,    37,    38,    39,    40,    -1,   236,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   236,    35,    36,    37,    38,    39,    40,    -1,
     235,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   235,    -1,    35,    36,
      37,    38,    39,    40,    -1,   235,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   235,    35,    36,
      37,    38,    39,    40,    -1,   234,    43,    35,    36,    37,
      38,    39,    40,    -1,   234,    43,    35,    36,    37,    38,
      39,    40,    -1,   234,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   234,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   233,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     233,    35,    36,    37,    38,    39,    40,   231,    -1,    43,
      35,    36,    37,    38,    39,    40,   231,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,   231,    43,    35,    36,
      37,    38,    39,    40,    -1,   231,    43,    35,    36,    37,
      38,    39,    40,    -1,   231,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   231,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   230,    -1,    35,
      36,    37,    38,    39,    40,    -1,   230,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   230,    -1,
      35,    36,    37,    38,    39,    40,    -1,   230,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    35,    36,    37,    38,    39,
      40,    -1,   230,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   229,    43,    35,    36,    37,    38,    39,    40,
      -1,   229,    43,    35,    36,    37,    38,    39,    40,    -1,
     229,    43,    35,    36,    37,    38,    39,    40,    -1,   229,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   229,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   229,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   228,    -1,    35,    36,    37,    38,
      39,    40,    -1,   228,    43,    35,    36,    37,    38,    39,
      40,    -1,   228,    43,    35,    36,    37,    38,    39,    40,
      -1,   228,    43,    35,    36,    37,    38,    39,    40,    -1,
     228,    43,    35,    36,    37,    38,    39,    40,    -1,   228,
      43,    35,    36,    37,    38,    39,    40,    -1,   227,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   227,    -1,    35,    36,    37,    38,    39,    40,    -1,
     227,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   227,    -1,    35,    36,    37,    38,    39,    40,
      -1,   227,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   227,    -1,    35,
      36,    37,    38,    39,    40,    -1,   226,    43,    35,    36,
      37,    38,    39,    40,    -1,   226,    43,    35,    36,    37,
      38,    39,    40,    -1,   226,    43,    35,    36,    37,    38,
      39,    40,    -1,   226,    43,    35,    36,    37,    38,    39,
      40,    -1,   226,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   226,    -1,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   225,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   225,    -1,    -1,    35,
      36,    37,    38,    39,    40,   225,    -1,    43,    35,    36,
      37,    38,    39,    40,   225,    -1,    43,    35,    36,    37,
      38,    39,    40,   225,    -1,    43,    35,    36,    37,    38,
      39,    40,   225,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     224,    -1,    35,    36,    37,    38,    39,    40,    -1,   224,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   224,    -1,    35,    36,    37,    38,    39,    40,
      -1,   224,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   224,    -1,    35,    36,    37,    38,    39,
      40,    -1,   224,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   223,    -1,    35,
      36,    37,    38,    39,    40,    -1,   223,    43,    35,    36,
      37,    38,    39,    40,    -1,   223,    43,    -1,    35,    36,
      37,    38,    39,    40,   223,   195,    43,    -1,    -1,   189,
      -1,    -1,    -1,   223,    -1,    -1,    35,    36,    37,    38,
      39,    40,   223,   189,    43,    35,    36,    37,    38,    39,
      40,    -1,   189,    43,    35,    36,    37,    38,    39,    40,
      -1,   189,    43,    -1,   183,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
     183,    -1,    43,    -1,    -1,    -1,    -1,    -1,   182,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   182,    35,    36,    37,    38,    39,    40,    -1,   181,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   181,    35,    36,    37,    38,    39,    40,    -1,   180,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,   170,   180,    -1,    35,    36,    37,    38,
      39,    40,    -1,   170,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,   170,    43,    35,    36,    37,    38,    39,    40,    -1,
     170,    43,    35,    36,    37,    38,    39,    40,    -1,   170,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   169,
      43,    35,    36,    37,    38,    39,    40,    -1,   169,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,   169,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,   169,    -1,    35,    36,
      37,    38,    39,    40,    -1,   169,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   168,    43,    35,    36,    37,
      38,    39,    40,    -1,   168,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   168,
      -1,    35,    36,    37,    38,    39,    40,    -1,   168,    43,
      35,    36,    37,    38,    39,    40,    -1,   168,    43,    35,
      36,    37,    38,    39,    40,   167,    -1,    43,    -1,    -1,
     163,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     163,    -1,    35,    36,    37,    38,    39,    40,    -1,   163,
      43,    35,    36,    37,    38,    39,    40,    -1,   163,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   162,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   160,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   160,    35,    36,    37,    38,    39,    40,
      -1,   159,    43,    35,    36,    37,    38,    39,    40,    -1,
     159,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,   159,    35,    36,    37,    38,
      39,    40,    -1,   158,    43,    35,    36,    37,    38,    39,
      40,    -1,   158,    43,    -1,    -1,   150,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   150,    35,    36,
      37,    38,    39,    40,    -1,   149,    43,    35,    36,    37,
      38,    39,    40,    -1,   149,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   148,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   147,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   147,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,   147,    -1,    -1,    35,    36,
      37,    38,    39,    40,   147,   134,    43,    35,    36,    37,
      38,    39,    40,    -1,   134,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   134,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   134,    -1,    35,
      36,    37,    38,    39,    40,    -1,   134,    43,    35,    36,
      37,    38,    39,    40,    -1,   134,    43,    35,    36,    37,
      38,    39,    40,    -1,   133,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   133,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   124,    -1,    35,
      36,    37,    38,    39,    40,    -1,   124,    43,    35,    36,
      37,    38,    39,    40,    -1,   124,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   124,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   123,    -1,    -1,
      -1,   118,    -1,    35,    36,    37,    38,    39,    40,    -1,
     118,    43,    35,    36,    37,    38,    39,    40,    -1,   118,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   118,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   117,    -1,    35,    36,    37,    38,    39,    40,    -1,
     117,    43,    -1,    -1,    -1,    -1,   114,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   114,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,   114,    -1,    43,    -1,    -1,    -1,    -1,   112,
      35,    36,    37,    38,    39,    40,    -1,   110,    43,    -1,
      -1,    -1,    -1,    -1,   108,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   107,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    82,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
      -1,   216,    -1,   218,   219,   220,   221,   222,   271,   272,
     273,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,    -1,   287,   288,   289,   290,   291,   292,
     293,   294
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     6,     8,     9,    10,    13,
      16,    18,    20,    27,    28,    29,    30,    31,    32,    33,
      36,    42,    44,    45,    46,    47,    50,    52,    54,    55,
      71,    78,    79,    80,    81,    96,    97,   109,   115,   465,
     468,   469,   471,   473,   479,   483,   484,   485,   488,   489,
     490,   493,   494,   495,    19,    21,    22,    23,    24,    25,
      26,    34,   473,   473,   473,   494,   494,     3,   494,   494,
      55,    56,   494,    53,    51,    55,    56,    58,    59,    61,
      62,    63,    65,    66,    67,    77,   192,   259,   453,   491,
     476,   465,    55,    55,    55,    56,    59,    61,    65,    74,
     184,    59,    82,    85,    87,    91,    94,   270,    89,   100,
     494,    72,   131,   128,    56,   465,   465,   472,   494,   479,
       0,   495,    35,    36,    37,    38,    39,    40,    43,   479,
       3,   494,   494,   494,   494,   494,   494,   494,   487,   494,
     494,   494,   480,   496,   496,   116,   116,   494,     5,   116,
     116,   492,   116,   116,   116,   116,   116,   460,   131,   116,
     116,   191,   116,   118,   494,   470,   116,   116,   116,   116,
     116,   116,   460,   127,   127,   136,    83,    86,    88,    92,
      93,    95,    89,   172,   102,   133,    73,   132,   494,   465,
     475,   494,   494,   494,   494,   494,   494,   494,   474,   475,
     475,   475,   481,   495,   497,    12,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   113,   116,   494,   494,    65,
     494,    55,   477,   129,   494,   494,   494,   494,   494,   494,
      55,    55,    56,   137,   418,   113,   113,   100,    90,   130,
     173,   176,   177,   197,    82,   100,     5,   133,   494,   497,
     481,   496,   110,   134,   134,   118,   118,   118,   118,   134,
      55,   461,   494,   134,   413,   118,   100,   116,   478,   118,
     114,   114,   116,   116,   116,   494,    34,   224,   423,    55,
      82,   117,   174,   175,   175,   178,   117,   104,    98,   101,
     105,   473,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   216,
     218,   219,   220,   221,   222,   271,   272,   273,   274,   275,
     277,   278,   279,   280,   281,   282,   283,   287,   288,   289,
     290,   291,   292,   293,   294,    55,    56,   111,    55,   111,
      55,    56,   441,   442,   443,   116,    55,   162,   256,   257,
     258,   260,    34,    55,    58,   494,     3,   494,   111,   119,
     486,   486,   494,   494,   494,   494,    34,    34,   116,    84,
      58,   232,   184,   184,   185,   179,    58,    34,    82,    82,
      82,   494,   100,   113,   100,   113,   113,   113,   100,   113,
     113,   100,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   295,   295,
     295,   295,   295,   295,   295,   295,   365,   296,   363,   363,
     295,   295,   295,   295,   295,   295,   295,   295,   116,   116,
      57,   116,   121,   116,   116,   113,   113,   113,   494,   116,
      34,   267,   296,   296,   266,   494,   116,   116,   454,   486,
     417,   494,   494,   494,   419,   116,   175,   186,   186,   186,
     184,   116,   494,   103,   103,   474,   494,   121,   494,   121,
     121,   121,   494,   121,   121,   494,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,   111,   494,   494,   134,   494,   124,   494,   494,
     444,   444,   444,   124,   494,   494,    34,    34,    34,    34,
     414,   494,   494,    34,    34,   422,   124,    34,   494,   184,
     117,   117,   117,   186,   494,    34,    34,   494,   189,   473,
     189,   473,   473,   473,   189,   473,   473,   189,   473,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   473,   473,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   134,   134,   134,   144,   124,   119,
      34,    34,    34,   119,   112,   147,   494,   494,   494,   494,
      34,   463,   118,   494,   494,    34,   119,   494,   147,   186,
      58,    58,    58,   117,   147,   494,   494,   474,   188,   494,
     188,   494,   494,   494,   188,   494,   494,   188,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   296,   296,   381,   343,   397,   343,
     296,   310,   366,   297,   296,   296,   296,   296,   296,   296,
     296,   296,   408,   296,   143,   139,   140,   141,   143,   144,
     144,   145,   117,   119,   103,   494,   494,   494,   461,   462,
      34,   117,   298,   233,   494,   119,    67,   455,   416,   494,
     137,   420,    34,   117,   116,   116,   116,    58,    34,   107,
     494,   190,   474,   190,   474,   474,   474,   190,   474,   474,
     190,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,   122,   122,   122,
     117,   117,   110,    77,   103,   148,    34,   445,   445,   445,
     112,    55,   494,    58,    34,    34,   415,    65,   116,    34,
      34,   494,    34,   494,    58,   494,   494,   494,   116,   494,
      34,   475,   249,   494,   249,   494,   494,   494,   249,   494,
     494,   249,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,    34,    34,
      34,    77,    77,   121,   116,    34,    34,   494,    34,    34,
      34,    55,   116,   116,   494,   494,    34,   193,   494,   494,
     494,   494,   163,   116,   168,   168,   168,   494,   163,   494,
     113,   113,   474,   113,   474,   474,   474,   113,   474,   474,
     113,   475,   475,   475,   475,   475,   474,   474,   474,   474,
     475,   475,   475,   475,   474,   474,   474,   298,   298,   382,
     340,   398,   344,   298,   311,   367,   364,   364,   298,   298,
     298,   298,   298,   298,   409,   298,   494,   494,   494,   116,
     116,   473,   494,   494,   494,   147,   494,   494,   494,   116,
     494,   494,   267,   236,   494,   464,   262,   456,   417,    34,
     494,    34,    34,    34,   168,    34,   106,   494,   121,   494,
     121,   494,   494,   494,   121,   494,   494,   121,   113,   196,
     233,   233,   233,   494,   494,   494,   494,   117,   240,   194,
     233,   494,   494,   494,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,   103,   163,   494,   494,   494,   148,   158,
      34,   446,   447,   447,   494,   463,    34,    34,   494,    34,
      34,    34,     5,   168,   494,   494,   494,    34,     5,    34,
     114,   473,   474,   473,   474,   474,   474,   473,   474,   474,
     473,   494,    34,    34,    34,    34,   474,   474,   475,   474,
      58,    34,    34,    34,   474,   474,   474,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,    34,    34,   474,    34,
      34,   494,    34,    34,    34,   124,   486,   494,   494,   494,
     494,   494,   427,    34,   169,   169,   169,   494,   427,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     123,   494,   494,   494,   494,   494,   494,   117,   494,   116,
     494,   494,   494,   494,   494,   494,   383,   320,   399,   298,
     402,   307,   368,   445,   450,   299,   299,   301,   301,   301,
     301,   410,   301,   494,     5,   494,   494,   494,   167,   440,
     494,   494,   119,   295,   237,   265,   457,    34,   494,    34,
      34,    34,   169,    34,   108,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   117,   269,   296,   296,   296,
     475,   475,    58,   475,   494,   241,   195,   117,   474,   474,
     474,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,   163,   474,
     158,   149,    34,   446,   448,   462,    34,    34,    34,    34,
     473,   169,   494,   494,   494,    34,   473,    34,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,    67,    34,
      34,    34,    34,   117,   117,   116,   117,   233,    34,    34,
      58,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,    34,   494,    34,    34,   494,    34,    34,   494,   494,
     473,   494,   494,    34,   170,   170,   170,   494,   494,   494,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     116,   494,   494,   494,   494,    58,    58,   494,    58,    34,
     494,   494,   116,   474,   474,   474,   384,   318,   400,   342,
     403,   308,   425,   447,   451,   306,   302,   306,   306,   306,
     302,   411,   302,     5,   474,   494,   494,   159,   440,   494,
     234,   494,   458,   474,   494,    34,    34,    34,   170,   474,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   295,   234,   234,   234,   116,   116,   267,   116,   494,
     242,   241,   494,   494,   494,   494,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,   427,   494,   149,   160,    34,   446,    34,
     474,    34,   494,   170,   494,   494,   494,    34,   494,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   295,
      34,    34,    34,    34,   494,   494,    34,   494,   295,    34,
      34,   474,   474,   474,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,    34,   474,   475,    34,    34,   494,    34,   494,   494,
     494,   474,    34,   494,   474,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,    34,   494,   494,   494,   494,
     267,   267,   494,   267,    34,   494,   494,   494,   494,   494,
     385,   319,   401,   345,   404,   309,   369,   448,   452,   305,
     303,   305,   305,   305,   303,   412,   303,   473,   494,   113,
     494,   494,   440,   235,   475,   459,   494,   494,   494,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   494,
     246,   235,   235,   235,    34,    34,    34,   494,   243,   242,
     474,   474,   474,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
     494,   474,   165,   166,   160,   150,    34,    34,   474,   474,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     238,    34,    34,    34,    34,   494,   494,   494,    34,    34,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     474,   494,   473,   494,    34,    34,   494,   494,   494,   494,
     474,   475,   474,   474,   475,   474,   474,   474,   475,   474,
      34,   473,   494,   494,   494,   494,   494,   475,   474,   474,
     386,   321,   298,   346,   405,   370,   449,   445,   305,   304,
     304,   304,   305,   301,   305,   494,   474,   494,   494,   494,
     474,   474,   494,   117,   494,   494,   117,   494,   494,   494,
     117,   494,   494,   494,   237,   237,   237,   244,   243,   117,
     494,   494,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,   474,   494,   474,
     150,   159,   494,   494,   474,    58,   474,   474,    58,   474,
     474,   474,    58,   474,   246,   474,    34,    34,    34,    34,
      34,    58,   474,   474,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     474,   494,    34,    34,   474,   474,   494,   116,   494,   494,
     116,   494,   494,   494,   116,   494,    34,   494,   494,   494,
     494,   494,   494,   116,   494,   494,   387,   322,   304,   347,
     406,   447,   314,   304,   302,   304,   474,   494,   474,   494,
     494,   494,   494,   474,   494,   474,   474,   494,   474,   474,
     474,   494,   474,   473,   474,   236,   236,   236,   250,   244,
     494,   474,   474,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,   494,   474,   475,   494,   159,   475,   475,
     494,   494,   494,   223,   494,   494,   494,   223,   494,   494,
     494,    34,    34,    34,    34,    34,   223,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   474,
     494,   113,   474,    34,   421,   421,   475,   474,   474,    34,
     474,   474,   474,    34,   474,   474,   475,   494,   494,   494,
     473,   494,    34,   474,   474,   388,   323,   348,   407,   448,
     313,   303,   314,   494,   475,   165,   166,   494,   494,    34,
      34,   117,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   247,   295,   295,   295,   494,   250,   494,   494,   494,
      34,    34,    34,    34,    34,    34,    34,    34,   474,   113,
     473,   494,   475,   494,   494,    58,   474,   474,   224,   474,
     474,   474,   224,   474,   474,    34,    34,    34,    34,   474,
      34,   224,   474,   474,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   165,   166,   494,   116,   494,   494,    34,
     494,   494,   494,    34,   494,   494,   473,   494,   494,   494,
     494,   473,    34,   494,   494,   389,   324,   349,   304,   449,
     305,   313,   475,   473,   494,   474,   494,   474,   474,   494,
     474,   474,   474,   494,   474,   475,   494,   246,   246,   246,
     474,   494,   494,   474,   474,    34,    34,    34,    34,    34,
      34,    34,   494,   494,   494,   494,   225,   494,   494,   494,
     225,   494,   247,   474,    34,    34,    34,   494,   474,   225,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   474,
     474,   474,   474,    34,   474,   474,   474,    34,   474,    34,
     494,   473,   473,   473,   475,   494,    34,   474,   474,   390,
     325,   350,   304,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   473,   474,   494,   494,   494,   474,   494,
     494,   494,    34,    34,    34,    34,   474,   474,   474,   474,
     226,   474,   474,   474,   226,   474,   494,   494,   474,   474,
     474,   494,   226,   474,   474,   494,   494,   494,   494,   494,
     494,   494,   494,    34,   494,   494,   494,    34,   494,   474,
     475,   494,   494,   494,   475,    34,   494,   494,   391,   326,
     351,   474,   474,   474,   474,   494,   474,   474,   474,   494,
     474,   494,   248,   474,   474,   474,   494,   474,   474,    34,
      34,    34,   494,   494,   494,   494,   227,   494,   494,   494,
     227,   494,   474,    34,   494,   494,   494,   227,   494,   494,
     494,   494,   494,   474,   474,   474,   474,    34,   474,   474,
     474,    34,   474,   494,   473,   475,   475,   475,    34,   474,
     474,   392,   327,   352,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   475,   494,   247,   247,   247,   494,
     494,   494,    34,    34,    34,   474,   474,   474,   474,   228,
     474,   474,   474,   228,   474,   248,   474,    34,    34,    34,
     228,   474,   474,   494,   494,   494,   494,   494,   494,   494,
      34,   494,   494,   494,    34,   494,    34,   494,   473,   473,
     473,    34,   494,   494,   393,   328,   353,   474,   474,   474,
     474,   494,   474,   474,   474,   494,   474,   473,   474,   494,
     494,   494,   494,   474,   474,    34,    34,    34,   494,   494,
     494,   494,   229,   494,   494,   494,   229,   494,   494,   494,
     474,   474,   474,   229,   494,   494,   494,   494,   494,   474,
     475,   474,   474,    34,   475,   474,   475,    34,   474,   474,
     475,   494,   494,   494,    34,   475,   474,   394,   329,   354,
     494,   494,   494,   494,   117,   494,   117,   494,   494,   494,
     474,   474,   474,   494,   117,   494,    34,    34,    34,   474,
     474,   474,   230,    58,   474,    58,   230,   474,   474,   494,
     494,   494,   230,    58,   474,   494,   494,   494,   494,   494,
     494,    34,   116,   494,   116,    34,   494,   494,   475,   475,
     475,    34,   116,   494,   395,   330,   355,   475,   474,   474,
     494,   494,   474,   494,   494,   474,   475,   248,   248,   248,
     494,   494,   474,    34,    34,    34,   494,   494,   231,   223,
     494,   231,   494,    34,    34,    34,   231,   223,   494,   494,
     494,   494,   474,   474,    34,    34,   475,    34,   474,   473,
     473,   473,    34,    34,   474,   396,   331,   356,   494,   494,
     494,   494,   117,   494,   494,   494,   494,   494,   494,   494,
     494,    34,    34,    34,   474,   474,   224,    58,   474,   474,
     474,   474,   224,   474,   494,   494,   494,   494,   494,    34,
     116,   494,   494,   494,   494,    34,   494,   332,   357,   474,
     474,   494,   494,   474,   474,   474,   474,   494,   474,    34,
      34,   494,   494,   225,   494,   494,   494,   494,   225,   494,
     494,   494,   474,   474,    34,   474,   475,   475,   475,    34,
     474,   333,   358,   494,   494,   494,   494,   494,   494,    34,
      34,   474,   475,   226,   474,   226,   475,   494,   494,   494,
     117,    34,   494,    34,   117,   334,   304,   474,    58,   494,
     474,   494,    58,    34,    34,   494,   116,   227,   494,   227,
     116,   494,   494,   474,   494,    34,   474,    34,   494,   335,
      82,   494,   494,   494,   494,   223,    34,    34,   475,   228,
     475,   228,    34,   494,   494,   117,    34,   117,    34,   494,
     336,   180,    58,   494,    58,   494,   224,    34,    34,   116,
     229,   116,   229,    34,   494,   494,   494,    34,   494,    34,
     494,   337,   181,   494,   494,   225,    34,    34,   230,   230,
      34,   494,   494,    34,    34,   494,   338,   182,   494,   494,
     226,    34,    34,   231,   231,    34,   494,   494,    34,    34,
     494,   339,   183,   494,   494,   227,    34,    34,    34,   494,
     494,   494,    82,   228,    34,    34,   494,   494,   180,   229,
      34,    34,   494,   494,   181,   230,    34,    34,   494,   494,
     182,   231,    34,    34,   494,   494,   183,    34,   494
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   482,   483,   483,   483,   483,   483,   483,   483,   483,
     483,   483,   483,   483,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   485,   485,   485,   485,   486,   486,   487,   487,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   489,   489,   489,   490,   490,
     490,   490,   490,   490,   490,   491,   491,   491,   491,   491,
     491,   491,   491,   491,   491,   491,   491,   491,   491,   491,
     491,   491,   491,   491,   491,   491,   492,   492,   492,   492,
     492,   492,   492,   492,   492,   492,   492,   492,   492,   492,
     492,   492,   492,   492,   492,   492,   492,   492,   492,   492,
     492,   492,   493,   493,   493,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   495,   495,   495,
     495,   495,   495,   495,   495,   495,   496,   496,   497,   497
  };

  const unsigned char
  feiparser::yyr2_[] =
  {
       0,     2,     2,     4,     4,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,    15,     2,    13,     3,    14,
      13,    14,    24,    32,    32,    48,    34,    52,    13,    16,
      34,    10,    10,    31,    28,    21,    16,    19,    22,     8,
       8,    15,    24,    15,    17,     8,    11,    13,    18,    24,
       9,     6,     6,    14,     6,     1,     2,     1,     3,     2,
       2,     3,     4,     6,     5,    15,     3,    12,     3,    10,
       7,    11,     3,     4,     6,     9,    18,     2,     7,    22,
      20,    20,    21,    20,    32,    32,     1,     3,     4,     4,
       4,     4,     6,     6,     6,    14,    23,    26,    26,    26,
      32,    38,    29,    35,    29,    35,    95,    80,    23,    11,
      26,    35,    26,    21,    14,    56,    27,    18,    33,    65,
      71,    71,    51,    57,    54,    54,    54,    78,    78,    92,
      47,    59,    59,    51,    59,    35,    38,    22,    22,    20,
      22,    21,     1,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     3,     4,     4,     1,     4,     1,
       1,     1,     1,     1,     1,     1,     6,     1,     2,     2,
       3,     5,     3,     1,     1,     2,     2,     3,     1,     2
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
  "DISPLACEMENT", "ELEMENTNAME", "MATERIALNAME", "ACCELERATION_FIELD",
  "FIX", "FREE", "REMOVE", "DEFINE", "ALGORITHM", "ALGNAME",
  "CONSTITUTIVE_ALGNAME", "CONVERGENCE_TEST", "TESTNAME", "SOLVER",
  "SOLVERNAME", "CONSTITUTIVE", "INTEGRATION", "DYNAMICINTEGRATOR",
  "DYNAMICINTEGRATOR_HHT", "DYNAMICINTEGRATOR_NEWMARK", "STATICINTEGRATOR",
  "STATICINTEGRATOR_DISPLACEMENT", "SIMULATE", "COMPUTE", "STATIC",
  "DYNAMIC", "USING", "TRANSIENT", "EIGEN", "time_step", "number_of_modes",
  "VARIABLETRANSIENT", "maximum_time_step", "minimum_time_step",
  "number_of_iterations", "RUNTEST", "AT", "ALL", "AND", "WITH",
  "TEXTDOFS", "NEW", "TEXTNUMBER", "USE", "TO", "DOF", "TEXTWITH", "NODES",
  "FORCE", "INTEGRATIONPOINTS", "dof", "RESPONSE", "FILE", "FROM", "EVERY",
  "LEVEL", "BARDETMETHOD", "LOADING", "STAGE", "STEPS", "TYPE", "DOFS",
  "FACTOR", "INCREMENT", "TH_GROUNDMOTION", "TH_LINEAR", "TH_PATH_SERIES",
  "TH_PATH_TIME_SERIES", "TH_CONSTANT", "TH_FROM_REACTIONS", "self_weight",
  "surface", "load_value", "scale_factor", "displacement_scale_unit",
  "velocity_scale_unit", "acceleration_scale_unit", "number_of_steps",
  "number_of_boundary_nodes", "number_of_exterior_nodes",
  "number_of_drm_elements", "element_file", "boundary_nodes_file",
  "exterior_nodes_file", "displacement_file", "acceleration_file",
  "velocity_file", "force_file", "hdf5_file", "series_file",
  "time_series_file", "MAGNITUDES", "MAGNITUDE", "initial_velocity",
  "strain_increment_size", "maximum_strain",
  "number_of_times_reaching_maximum_strain", "constitutive", "testing",
  "constant", "mean", "triaxial", "drained", "undrained", "simple",
  "shear", "number_of_subincrements", "maximum_number_of_iterations",
  "tolerance_1", "tolerance_2", "strain", "stress", "control", "Guass",
  "points", "Gauss", "each", "point", "single", "value",
  "initial_normal_stiffness", "stiffning_rate",
  "number_of_integration_points", "DIRECT_STRAIN", "EightNodeBrick",
  "TwentySevenNodeBrick", "EightNodeBrick_upU", "TwentyNodeBrick_uPU",
  "TwentyNodeBrick", "EightNodeBrick_up", "variable_node_brick_8_to_27",
  "beam_displacement_based", "BeamColumnDispFiber3d", "beam_elastic",
  "beam_elastic_lumped_mass", "beam_9dof_elastic", "FourNodeShellMITC4",
  "FourNodeShellNewMITC4", "ThreeNodeShellANDES", "FourNodeShellANDES",
  "truss", "contact", "HardContact", "FrictionalPenaltyContact",
  "SoftContact", "ShearBeam", "EightNodeBrick_upULT",
  "TwentyNodeBrick_uPULT", "TwentySevenNodeBrick_uPULT", "porosity",
  "alpha", "rho_s", "rho_f", "k_x", "k_y", "k_z", "K_s", "K_f", "pressure",
  "cross_section", "shear_modulus", "torsion_Jx", "bending_Iz",
  "bending_Iy", "IntegrationRule", "stiffness", "normal_stiffness",
  "tangential_stiffness", "normal_damping", "tangential_damping",
  "friction_ratio", "maximum_gap", "xz_plane_vector", "joint_1_offset",
  "joint_2_offset", "direction", "contact_plane_vector", "power", "linear",
  "exponential", "factorial", "inverse", "MembranePlateFiber",
  "ElasticMembranePlate", "elastic3d", "FIBER", "FiberSection", "Section",
  "fiber_cross_section", "fiber_location_Y", "fiber_location_Z",
  "fiber_location", "TorsionConstant_GJ", "thickness", "integration_rule",
  "section_number", "NDMaterialLT", "linear_elastic_isotropic_3d",
  "linear_elastic_isotropic_3d_LT", "NonlinearIsotropic3DLT",
  "sanisand2008", "CamClay", "sanisand2004", "sanisand2004_legacy",
  "roundedMohrCoulomb", "linear_elastic_crossanisotropic",
  "uniaxial_concrete02", "uniaxial_elastic_1d", "uniaxial_steel01",
  "uniaxial_steel02", "pisano", "PisanoLT", "CamClayLT", "VonMisesLT",
  "VonMisesArmstrongFrederickLT", "DruckerPragerLT",
  "DruckerPragerNonAssociateLinearHardeningLT", "DruckerPragerVonMisesLT",
  "DruckerPragerArmstrongFrederickLE", "DruckerPragerArmstrongFrederickNE",
  "DruckerPragerNonAssociateArmstrongFrederickLT", "mass_density",
  "elastic_modulus", "viscoelastic_modulus", "poisson_ratio",
  "von_mises_radius", "druckerprager_angle", "druckerprager_k",
  "armstrong_frederick_ha", "armstrong_frederick_cr",
  "initial_confining_stress", "isotropic_hardening_rate",
  "kinematic_hardening_rate", "poisson_ratio_h_v", "poisson_ratio_h_h",
  "shear_modulus_h_v", "elastic_modulus_horizontal",
  "elastic_modulus_vertical", "pressure_reference_p0", "plastic_flow_kd",
  "plastic_flow_xi", "initial_void_ratio", "initial_shear_modulus",
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
  "beta_min", "n_in", "a_in", "elastic_modulus_1atm", "eplcum_cr_in",
  "Niso3d_K", "Niso3d_Kur", "Niso3d_n", "Niso3d_c", "Niso3d_phi0",
  "Niso3d_dphi", "Niso3d_Rf", "Niso3d_K0", "Niso3d_Kb", "Niso3d_m",
  "Niso3d_pa", "Niso3d_K2", "Niso3d_B", "Niso3d_Et", "Niso3d_Ei",
  "Niso3d_Er", "CriticalState_M", "CriticalState_lambda",
  "CriticalState_kappa", "CriticalState_e0", "CriticalState_p0", "RMC_m",
  "RMC_qa", "RMC_pc", "RMC_e", "RMC_eta0", "RMC_Heta", "DuncanCheng_K",
  "DuncanCheng_pa", "DuncanCheng_n", "DuncanCheng_sigma3_max",
  "DuncanCheng_nu", "ax", "ay", "az", "verbose_level",
  "maximum_iterations", "tolerance", "yield_function_relative_tolerance",
  "stress_relative_tolerance", "verbose_output", "beta", "gamma", "kappa",
  "lambda", "delta", "sigma0", "DOMAIN_", "startTime", "endTime", "Period",
  "Phase", "Amplitude", "frequency", "MaxTime", "frequency1", "frequency2",
  "frequency3", "frequency4", "stiffness_to_use_opt", "DAMPING_RAYLEIGH",
  "DAMPING_CAUGHEY3", "DAMPING_CAUGHEY4", "a0", "a1", "stiffness_to_use",
  "a2", "a3", "a4", "R0", "cR1", "cR2", "MASS", "mx", "my", "mz", "Imx",
  "Imy", "Imz", "equal_dof", "master", "slave", "dof_to_constrain", "of",
  "OUTPUT", "BINARY", "TEXT", "ENABLE", "DISABLE", "COMPRESSION", "SAVE",
  "NON_CONVERGED_ITERATIONS", "'('", "','", "')'", "'['", "']'", "'.'",
  "';'", "'{'", "'}'", "$accept", "dsl", "CMD_add", "CMD_fix", "dofchain",
  "explist", "CMD_define", "CMD_misc", "CMD_remove", "ADD_material",
  "ADD_element", "prompt", "exp", "stmt", "cpd_stmt", "list", YY_NULLPTR
  };


  const unsigned short int
  feiparser::yyrline_[] =
  {
       0,   307,   307,   321,   334,   347,   360,   368,   374,   375,
     376,   377,   378,   382,   392,   396,   434,   438,   451,   455,
     473,   488,   506,   532,   561,   591,   628,   659,   699,   719,
     752,   789,   804,   819,   871,   920,   962,   980,   999,  1018,
    1034,  1050,  1068,  1089,  1126,  1143,  1160,  1179,  1195,  1217,
    1241,  1264,  1304,  1318,  1340,  1369,  1373,  1378,  1384,  1395,
    1404,  1411,  1418,  1426,  1436,  1449,  1533,  1589,  1664,  1699,
    1713,  1726,  1753,  1791,  1819,  1832,  1848,  1871,  1881,  1895,
    1919,  1943,  1967,  1991,  2015,  2057,  2095,  2105,  2121,  2134,
    2147,  2161,  2173,  2185,  2197,  2218,  2236,  2272,  2300,  2328,
    2356,  2388,  2427,  2457,  2490,  2527,  2563,  2646,  2720,  2745,
    2760,  2789,  2824,  2857,  2884,  2904,  2965,  2991,  3010,  3037,
    3084,  3132,  3183,  3223,  3264,  3311,  3358,  3396,  3450,  3504,
    3566,  3620,  3671,  3722,  3775,  3827,  3864,  3902,  3928,  3954,
    3978,  4003,  4195,  4237,  4279,  4294,  4339,  4346,  4353,  4360,
    4367,  4374,  4381,  4387,  4394,  4402,  4410,  4418,  4426,  4434,
    4438,  4444,  4449,  4455,  4461,  4467,  4473,  4479,  4487,  4492,
    4498,  4503,  4508,  4513,  4518,  4523,  4531,  4568,  4573,  4577,
    4587,  4609,  4634,  4654,  4672,  4683,  4693,  4699,  4707,  4711
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
     473,   475,    37,    35,   474,    36,   478,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   479,
       2,    34,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   476,     2,   477,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   480,     2,   481,     2,     2,     2,     2,
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
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472
    };
    const unsigned int user_token_number_max_ = 719;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 8038 "feiparser.tab.cc" // lalr1.cc:1155
#line 4739 "feiparser.yy" // lalr1.cc:1156


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
		cerr << " <!!!>  Error at ---> " << loc << ": " << s << endl << endl;
		// printf("\033[6;3HHello\n");
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

// Returns appropriate bardet-type code
int bardet_type_parse(string type)
{
	if (type.compare("CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 0;
	if (type.compare("DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL") == 0) return 1;
	if (type.compare("DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 2;
	if (type.compare("UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 3;
	if (type.compare("UNDRAINED_TRIAXIAL_LOADING_STRESS_CONTROL") == 0) return 4;
	if (type.compare("UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL") == 0) return 5;

	return -1;
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



