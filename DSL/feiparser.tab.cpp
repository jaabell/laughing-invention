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
#line 2397 "feiparser.yy" // lalr1.cc:847
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
#line 2743 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2429 "feiparser.yy" // lalr1.cc:847
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
#line 2767 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2460 "feiparser.yy" // lalr1.cc:847
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
#line 2797 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2499 "feiparser.yy" // lalr1.cc:847
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
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("RMC_eta0",   		&isPressure));  
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("RMC_Heta",   		&isPressure));  
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  

		(yylhs.value.exp) = new FeiDslCaller11<int, double, double, double, double, double, 
									 double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_roundedMohrCoulomb, args, signature, "add_constitutive_model_NDMaterialLT_roundedMohrCoulomb");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2824 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2555 "feiparser.yy" // lalr1.cc:847
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
#line 2878 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2633 "feiparser.yy" // lalr1.cc:847
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
#line 2928 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2688 "feiparser.yy" // lalr1.cc:847
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
#line 2948 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2707 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		(yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push((yylhs.value.exp));
	}
#line 2964 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2729 "feiparser.yy" // lalr1.cc:847
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
#line 2987 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 2761 "feiparser.yy" // lalr1.cc:847
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
#line 3013 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 2793 "feiparser.yy" // lalr1.cc:847
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
#line 3036 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 2821 "feiparser.yy" // lalr1.cc:847
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
#line 3057 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 2849 "feiparser.yy" // lalr1.cc:847
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
#line 3075 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 2883 "feiparser.yy" // lalr1.cc:847
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
#line 3111 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 2930 "feiparser.yy" // lalr1.cc:847
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
#line 3135 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 2955 "feiparser.yy" // lalr1.cc:847
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
#line 3153 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 2974 "feiparser.yy" // lalr1.cc:847
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
#line 3179 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3003 "feiparser.yy" // lalr1.cc:847
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
#line 3223 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3050 "feiparser.yy" // lalr1.cc:847
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
#line 3268 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3098 "feiparser.yy" // lalr1.cc:847
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
#line 3315 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3149 "feiparser.yy" // lalr1.cc:847
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
#line 3352 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3189 "feiparser.yy" // lalr1.cc:847
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
#line 3390 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3238 "feiparser.yy" // lalr1.cc:847
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
#line 3426 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3285 "feiparser.yy" // lalr1.cc:847
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
#line 3462 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3325 "feiparser.yy" // lalr1.cc:847
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
#line 3496 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3364 "feiparser.yy" // lalr1.cc:847
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
#line 3545 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3418 "feiparser.yy" // lalr1.cc:847
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
#line 3594 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3472 "feiparser.yy" // lalr1.cc:847
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
#line 3651 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3535 "feiparser.yy" // lalr1.cc:847
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
#line 3687 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3593 "feiparser.yy" // lalr1.cc:847
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
#line 3728 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3644 "feiparser.yy" // lalr1.cc:847
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
#line 3769 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3692 "feiparser.yy" // lalr1.cc:847
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
#line 3815 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3748 "feiparser.yy" // lalr1.cc:847
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
#line 3857 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 3796 "feiparser.yy" // lalr1.cc:847
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
#line 3888 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 3833 "feiparser.yy" // lalr1.cc:847
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
#line 3920 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 3867 "feiparser.yy" // lalr1.cc:847
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
#line 3944 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 3893 "feiparser.yy" // lalr1.cc:847
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
#line 3968 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 3919 "feiparser.yy" // lalr1.cc:847
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
#line 3990 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 3943 "feiparser.yy" // lalr1.cc:847
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
#line 4013 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 3969 "feiparser.yy" // lalr1.cc:847
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
#line 4033 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4157 "feiparser.yy" // lalr1.cc:847
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
#line 4079 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4199 "feiparser.yy" // lalr1.cc:847
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
#line 4125 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4241 "feiparser.yy" // lalr1.cc:847
    {
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		(yylhs.value.errcode) = 0;
	}
#line 4141 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4256 "feiparser.yy" // lalr1.cc:847
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
#line 4190 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4301 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4201 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4308 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4212 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4315 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4223 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4322 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4234 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4329 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4245 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4336 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4256 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4343 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.push ((yylhs.value.exp));
	}
#line 4266 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4349 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4277 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4356 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4289 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4364 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4301 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4372 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4313 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4380 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4325 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4388 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4337 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4396 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4345 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4400 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4355 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4406 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Number (*(yystack_[0].value.value));
		nodes.push ((yylhs.value.exp));
	}
#line 4364 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4411 "feiparser.yy" // lalr1.cc:847
    {

		(yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 4374 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4417 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4384 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4423 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

		nodes.push((yylhs.value.exp));
	}
#line 4394 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4429 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4404 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4435 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4414 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4441 "feiparser.yy" // lalr1.cc:847
    {
		string temp = *(yystack_[0].value.ident);
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		(yylhs.value.exp) = new FeiString(temp);
		nodes.push((yylhs.value.exp));
	}
#line 4426 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4449 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);//new FeiVector($3);
		//nodes.push($$);
	}
#line 4435 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4454 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push((yylhs.value.exp));
	}
#line 4445 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4460 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push((yylhs.value.exp));
	}
#line 4454 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4465 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push((yylhs.value.exp));
	}
#line 4463 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4470 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4472 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4475 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4481 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4480 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push((yylhs.value.exp));
	}
#line 4490 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4485 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push((yylhs.value.exp));
	}
#line 4499 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4493 "feiparser.yy" // lalr1.cc:847
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
#line 4531 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4530 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4540 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4535 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4548 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4539 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
		args.clear();
		signature.clear();
	}
#line 4558 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4549 "feiparser.yy" // lalr1.cc:847
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
#line 4581 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4571 "feiparser.yy" // lalr1.cc:847
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
#line 4607 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4596 "feiparser.yy" // lalr1.cc:847
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
#line 4631 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4616 "feiparser.yy" // lalr1.cc:847
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
#line 4653 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4634 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		(yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push((yylhs.value.exp));
	}
#line 4668 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4645 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4678 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4655 "feiparser.yy" // lalr1.cc:847
    {
		// empty statement generates an  empty expression
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4688 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4661 "feiparser.yy" // lalr1.cc:847
    {
		// Used to initialize a list of statements (compound statement)
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4697 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 187:
#line 4669 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[0].value.exp);
	}
#line 4705 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 188:
#line 4673 "feiparser.yy" // lalr1.cc:847
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
#line 4721 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4725 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -442;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
    2603,  -442,  1213,  -442,  -442,  -441,  -431,  -422,   564,   564,
    -442,  -442,    54,  -442,  -442,  -442,  -442,  -442,  -442,  -442,
     564,   564,   286,  -442,  -442,    15,    32,  -442,  5905,  -378,
    -356,    88,    94,   379,  1649,    66,   126,  -442,    75,    77,
     -48,  -249,  -251,   564,  -442,  -247,  -442,  -442,  -442,  -442,
    -442,   191,   -15,  -442,   228,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   -33,   -33,  -442,   199,   199,
    -107,   118,  9508,   564,   239,   137,   138,   144,   147,   151,
     163,   164,  -172,   153,   169,   170,   102,   185,   186,  -442,
     564,  -161,   189,   192,   193,   194,   195,   196,  -147,   197,
     198,   184,   240,   235,   247,    36,   231,   249,   155,   237,
    9218,   267,   218,   564,  -108,  -442,  -442,  -442,   132,  -442,
    -442,  -442,   564,   564,   564,   564,   564,   564,   564,  -442,
    -442,  9508,  9508,  9508,  9508,  9508,  9508,  9508,  -312,  9508,
     141,   212,  2659,   341,  -442,   564,   564,  9508,  -442,   564,
     564,  -442,   564,   564,   564,   564,   564,   241,   244,   564,
     564,   290,   564,   301,    -6,   232,   564,   564,   564,   564,
     564,   564,   307,   316,   318,   243,  -442,   -36,  -442,   264,
     265,   272,   291,   -86,   302,   283,  -442,   380,  9229,  -442,
    -442,   259,   259,    46,    46,    67,   199,   343,   564,  -442,
    -442,  -442,  -442,  2757,   -88,   -79,  9508,  9508,  9416,  9166,
    9176,  9310,  9320,  9331,  9157,   -40,   564,  9191,  5911,   278,
    9474,   282,   -73,   284,  9367,  9382,  9508,  9508,  9508,  9508,
     285,   288,   296,   564,   366,   190,     5,   362,   338,   308,
     252,   253,     8,   310,   327,    84,  -442,  -442,  9508,  -442,
    -442,  -442,   -35,  9384,  9336,   -38,   378,   328,   109,  -179,
     326,   388,  8981,   -28,   412,   392,   390,   564,   446,   564,
      -7,   331,   564,   564,   564,  9508,   564,   418,   420,   340,
     373,   408,   236,   295,   -11,   322,   411,   436,   430,   431,
     432,   564,   -12,     0,   389,   406,    40,   407,   421,   414,
     415,   416,   417,   419,   423,   433,   453,   458,   467,   470,
     471,   472,   473,   474,   486,   230,   287,   311,   313,   314,
     315,   317,   319,   161,   242,   181,   250,   320,   323,   324,
     332,   333,   342,   350,   488,   500,   545,   519,   410,   529,
     531,   535,   536,   538,   564,   541,   618,   393,   360,   364,
     395,   564,   546,   547,  2194,  -442,  9508,  -442,   331,  -442,
    -442,  9508,  9508,  9508,  5866,   564,   564,   564,   258,   556,
     499,   489,   490,   492,   495,   566,   564,  -442,   577,   580,
       3,   564,   563,   564,   565,   567,   569,   564,   573,   575,
     564,   576,   579,   583,   622,   624,   634,   635,   636,   637,
     638,   641,   646,   648,   649,   650,   651,   664,   739,   741,
     743,   751,   752,   760,   761,   769,   770,   778,   779,   781,
     782,   783,   784,   785,   786,   787,   361,   564,   551,   564,
     698,   564,   564,   394,   398,   402,  9241,   564,   564,   792,
     793,   815,   817,  5902,   564,   564,   818,  -442,   819,  9508,
    5857,  9266,   820,   564,   672,   745,   748,   749,   681,   564,
    9508,   824,   834,   564,  8650,   403,  8659,   409,   422,   425,
    8670,   434,   439,  8686,   441,   442,   443,   444,   445,   447,
     448,   449,   450,   451,   454,   455,   456,   457,   482,   483,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   735,
    9200,  9209,   731,  9276,   758,  9508,  9508,   844,   854,   857,
     801,  9404,  9116,   564,   564,   564,   564,   866,    23,  9340,
     564,   564,   900,   816,   564,  9126,   754,   880,   881,   883,
     835,  9138,   564,   564,  1976,   765,   564,   766,   564,   564,
     564,   767,   564,   564,   768,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,  7352,  7366,  6253,  6502,  6043,  6512,  7375,  6830,  6304,
    7343,  7384,  7395,  7404,  7413,  7423,  7432,  7447,  7459,  7468,
     814,    73,    43,   865,   867,   882,   564,   564,   564,   532,
     542,   954,  9349,  7204,  8036,  9508,   564,   872,   928,  1376,
    5884,   564,   860,  1930,   965,   884,   887,   898,   905,   972,
     989,  9508,  9434,   564,   848,  2065,   849,  2104,  2113,  2146,
     863,  2268,  2310,   868,  2338,  2394,  2463,  2503,  2518,  2684,
    2706,  2778,  2787,  2804,  2820,  2838,  2847,  2856,  2865,  2874,
    1013,  1023,  1025,  1026,  1027,  1028,  1030,  1039,  1041,  1042,
    1043,  1051,  1052,  1054,  1055,  1056,  1064,  1073,  1074,  -442,
     956,   994,   995,  -442,  1002,  1009,  1017,  1058,   -39,  1095,
    5795,  5806,  5815,  1021,  1079,   564,  1094,  1119,  1121,  5893,
    1091,  1062,  1123,  1124,  9508,   564,  1147,   564,  1125,   564,
     564,   564,  1068,   564,  1151,   238,   939,   564,   942,   564,
     564,   564,   943,   564,   564,   944,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,  1160,  1170,  1171,  1130,  1131,  1093,  1100,  1183,  1186,
     564,  1187,  1189,  1207,  1169,  1128,  9508,  1129,   564,   564,
    1214,  1060,   564,   564,   564,  9508,   564,  8945,  1134,  8886,
    8897,  8906,   564,  8954,   564,  1138,  1141,  2883,  1142,  2897,
    2906,  2915,  1143,  2924,  2937,  1144,   294,   309,   370,   424,
     438,  2952,  2961,  2970,  2979,   468,   585,   594,   603,  2988,
    2997,  3006,  7220,  7229,  6244,  6536,  6034,  6493,  7239,  6821,
    6295,  9508,  6323,  6332,  7248,  7258,  7267,  7276,  7287,  7296,
    7310,   564,   564,   564,  1154,  1155,   791,   564,   564,   564,
    9147,   564,   564,   564,  1156,   564,   564,  7597,  7907,   564,
     821,  7657,  1044,  9508,  5875,  1239,   564,  1240,  1241,  1243,
    8915,  1246,  9447,   564,  1173,   564,  1180,   564,   564,   564,
    1197,   564,   564,  1198,  1178,  1096,  1077,  1087,  1097,   564,
     564,   564,   564,  1210,  1092,  1137,  1102,   564,   564,   564,
    1303,  1304,  1305,  1306,  1307,  1308,  1309,  1326,  1329,  1331,
    1335,  1336,  1337,  1338,  1339,  1341,  1352,  1357,  9508,  9462,
    8963,   564,   564,   564,  9508,  9102,  9041,  1358,  5768,  5729,
    5738,   564,   539,  9508,  1360,  1361,  9508,   564,  1373,  1374,
    1383,  1417,  8924,   564,   564,   564,  1389,  1419,  1391,  9394,
     959,  3015,   962,  3042,  3051,  3060,   970,  3069,  3100,   971,
     564,  1406,  1415,  1425,  1426,  3110,  3119,   630,  3128,  1384,
    1428,  1429,  1430,  3140,  3149,  3158,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,  1431,  1433,  9508,  9508,  3167,  1434,
    1435,   564,  1436,  1437,  1439,  9292,   331,   564,   564,  9508,
     564,   564,   564,  1053,  1442,  8826,  8845,  8854,   564,  1059,
     564,  -442,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,  9301,   564,   564,   564,   564,   564,   564,  1362,
     564,  1365,   564,   564,   564,   564,   564,   564,  9508,  9508,
    6221,  6755,  6025,  7319,  5998,  6864,  6286,  5824,  5672,  7183,
    7195,  7137,  7146,  7155,  7165,  7174,   564,  1447,   564,   564,
     564,  8935,  1049,   564,   564,  1359,  -442,  7513,  7849,  7646,
    1031,  9508,  1443,   564,  1461,  1462,  1463,  8865,  1464,  9425,
    3196,  3205,  3214,  3223,  3238,  3247,  3256,  3265,  3274,  3283,
    1385,  7583,  7484,  7494,  7503,   711,   744,  1445,   753,   564,
    7801,  8637,  9358,  3292,  3301,  3310,  1470,  1490,  1494,  1495,
    1500,  1509,  1515,  1516,  1517,  1527,  1528,  1530,  1538,  1542,
    1545,  1547,  8972,  -442,  3319,  9050,  9084,  1548,  -442,  5777,
    5699,  1080,  1550,  1553,  1557,  1577,  1098,  8877,   564,   564,
     564,  1578,  1148,  1580,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,  1537,  1582,  1583,  1585,  1586,  1504,
    1507,  1524,  1514,  8045,  1607,  1609,  1603,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,  1628,   564,  1629,  1630,
     564,  1632,  1635,  -442,   564,   564,  1206,   564,   564,  1641,
    8777,  8786,  8798,   564,   564,   564,  3332,  3341,  3350,  3359,
    3371,  3387,  3396,  3405,  3414,  3423,  1566,   564,   564,   564,
     564,  1633,  1634,   564,  1636,  1661,   564,   564,  1581,  3432,
    3441,  3450,  6212,  6773,  6016,  6521,  5989,  6854,  5848,  5750,
    2077,  6873,  7107,  6883,  6892,  6902,  7116,  7128,  1691,  3459,
     564,   564,  9012,  1264,   564,  9508,  7993,   564,    87,  3468,
     564,  1665,  1667,  1672,  8807,  3477,  9508,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,  7522,  8008,
    8017,  8027,  1593,  1595,  7606,  1596,   564,  7783,  7820,   564,
     564,   564,   564,  1679,  1681,  1682,  1683,  1684,  1686,  1687,
    1688,  1690,  1692,  1693,  1694,  1696,  1698,  1699,  1705,  1320,
     564,  9093,  8990,  1712,  -442,  5786,  1716,  3486,  1720,   564,
    8817,   564,   564,   564,  1722,   564,  3495,  3504,  3513,  3522,
    3534,  3545,  3554,  3563,  3575,  3584,  7531,  1723,  1724,  1725,
    1737,   564,   564,  1739,   564,  7540,  1747,  1748,  9508,  3593,
    3602,  3611,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,  1750,    41,
    1751,  1752,   564,  1753,   564,   564,   564,  3620,  1754,  9508,
    9508,  9508,   564,  3631,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,  1758,   564,   564,   564,   564,  7615,
    7628,   564,  7637,  1766,   564,   564,   564,   564,   564,  6200,
    6764,  6007,  6483,  5980,  6841,  6277,  5708,  1892,  6911,  7074,
    6924,  6933,  6949,  7083,  7097,  1258,   564,  1695,   564,   564,
    9508,  1340,  7955,   762,   908,   564,   564,  9508,   564,  3640,
    3649,  3658,  3673,  3682,  3691,  3700,  3709,  3718,  3727,   564,
    7685,  7966,  7975,  7984,  1767,  1769,  9508,  1770,   564,  7765,
    7792,  3736,  3745,  3754,  1771,  1773,  1777,  1778,  1779,  1789,
    1790,  1792,  1794,  1798,  1799,  1800,  1809,  1819,  1820,  1828,
     564,  3767,    27,  9000,  9060,  -442,  1829,  -442,  1840,  3776,
    9508,  3785,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,  7840,  1842,  1844,  1845,  1846,   564,   564,   564,
    9508,  1849,  1856,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,  3794,   564,  1432,   564,  1861,  1862,   564,   564,
     564,   564,  3805,   771,  3814,  3823,   807,  3832,  3841,  3850,
     847,  3859,  1873,  1444,   564,   564,   564,  9508,  9508,  9508,
     564,   564,   859,  3868,  3877,  6185,  6739,  7334,  6474,  5969,
    9508,  6268,  5681,  5833,  9508,  6958,  6986,  6997,  7006,  6967,
    6977,   564,  3886,   564,  9508,   564,   564,  9508,  9508,  3895,
    3904,   564,  1783,   564,   564,  1795,   564,   564,   564,  1797,
     564,   564,   564,  7862,  7885,  7898,  7745,  7774,  1804,   564,
     564,  1881,  1882,  1884,  1888,  1889,  1890,  1899,  1900,  1903,
    1905,  1908,  1912,  1914,  1915,  3913,   564,  3922,  9075,  9021,
     564,   564,  3931,  1893,  3940,  3949,  1894,  3958,  3968,  3980,
    1896,  3989,  7694,  3998,  1916,  1921,  1938,  1940,  1942,  1906,
    4010,  4019,   564,   564,   564,   564,   564,   564,   564,   564,
     564,   564,   564,   564,   564,   564,   564,  4028,   564,  1943,
    1944,  4037,  4046,   564,  1863,   564,   564,  1865,   564,   564,
     564,  1867,   564,  1948,   564,   564,   564,   564,   564,   564,
    1868,   564,   564,  6176,  6730,  7017,  6459,  5960,  9508,  9508,
    5759,  9508,  9508,  6785,  9508,  7026,  7040,  4055,   564,  4066,
     564,   564,   564,   564,  4075,   564,  4084,  4093,   564,  4108,
    4117,  4126,   564,  4135,  1518,  4144,  7920,  7934,  7943,  7666,
    7756,   564,  4153,  4162,  1952,  1953,  1954,  1967,  1971,  1972,
    1983,  1984,  1986,   564,    96,   564,  9032,  9508,   893,   941,
     564,  9508,   564,   564,  8577,   564,   564,   564,  8586,   564,
     564,   564,  1987,  1988,  1991,  1992,  1993,  8596,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,  4171,
     564,  1897,  4180,  1996,  1616,  1631,   997,  4189,  4202,  2009,
    4211,  4220,  4229,  2011,  4240,  4249,  1006,   564,   564,   564,
    1584,   564,  2016,  4258,  4267,  6167,  6717,  9508,  6450,  5951,
    5720,  6803,  9508,  6794,   564,  1057,    37,   564,   564,  2020,
    2021,  1939,   564,   564,   564,   564,   564,   564,   564,   564,
     564,  1810,  7551,  7560,  7574,   564,  7676,   564,   564,   564,
    2024,  2026,  2028,  2029,  2030,  2032,  2035,  4276,  1957,  1604,
     564,  1066,  9508,   564,   564,  2014,  4285,  4294,  8512,  4303,
    4312,  4321,  8527,  4330,  4339,  2040,  2041,  2042,  2043,  4348,
    2044,  8538,  4357,  4366,   564,   564,   564,   564,   564,   564,
     564,   564,    74,   564,  9508,  -442,  9508,  9508,  1973,   564,
     564,  2049,   564,   564,   564,  2053,   564,   564,  1623,   564,
     564,   564,   564,  1627,  2072,   564,   564,  6158,  6708,  6440,
    7049,  5690,  9508,  6812,  1075,  1640,   564,  4375,   564,  4384,
    4394,   564,  4403,  4415,  4424,   564,  4433,  1085,   564,  7708,
    7721,  7730,  4445,   564,   564,  4454,  4463,  2075,  2084,  2097,
    2099,  2100,  2101,  -442,   564,  9508,   564,  9508,   564,   564,
    8445,   564,   564,   564,  8458,   564,  1864,  4472,  2111,  2112,
    2120,   564,  4481,  8467,   564,   564,   564,   564,   564,   564,
     564,   564,  4490,  4501,  4510,  4519,  2121,  4528,  4543,  4552,
    2123,  4561,  2124,   564,  1700,  1704,  1706,  1111,   564,  2125,
    4570,  4579,  6146,  6695,  6431,  9508,  9508,  9508,   564,   564,
     564,   564,   564,   564,   564,   564,   564,   564,  1708,  4588,
     564,   564,   564,  -442,  4597,   564,   564,   564,  2142,  2143,
    2144,  4606,  4615,  4624,  4637,  8383,  4646,  4655,  4664,  8392,
    4675,   564,   564,  4684,  4693,  4702,   564,  8404,  4711,  4720,
     564,   564,   564,   564,   564,   564,   564,  2153,   564,   564,
     564,  2156,   564,  4729,  1163,   564,   564,   564,  1190,  2157,
     564,   564,  6137,  6686,  6422,  4738,  4747,  4756,  4765,   564,
    4774,  4783,  4792,   564,  4801,   564,  1945,  4810,  4820,  4829,
    -442,   564,  4838,  4850,  2160,  2161,  2166,   564,   564,   564,
     564,  8318,   564,   564,   564,  8334,   564,  4859,  2167,   564,
     564,   564,  8344,   564,   564,   564,   564,   564,  4868,  4880,
    4889,  4898,  2168,  4907,  4916,  4925,  2169,  4936,   564,  1746,
    1250,  1260,  1269,  2170,  4945,  4954,  6117,  6673,  6413,   564,
     564,   564,   564,   564,   564,   564,   564,   564,   564,  1286,
     564,  1970,  1979,  1980,   564,   564,   564,  2184,  2185,  2186,
    4963,  4978,  4987,  4996,  8251,  5005,  5014,  5023,  8263,  5032,
    1994,  5041,  2201,  2202,  2207,  8272,  5050,  5059,   564,   564,
     564,   564,   564,   564,   564,  2209,   564,   564,   564,  2210,
     564,  2211,   564,  1781,  1785,  1787,  2212,   564,   564,  6108,
    6664,  6398,  5072,  5081,  5090,  5099,   564,  5110,  5119,  5128,
     564,  5137,  1791,  5146,   564,   564,   564,   564,  5155,  5164,
    2225,  2226,  2227,   564,   564,   564,   564,  8186,   564,   564,
     564,  8195,   564,   564,   564,  5173,  5182,  5191,  8209,   564,
     564,   564,   564,   564,  5200,  1366,  5209,  5218,  2229,  1396,
    5227,  1408,  2231,  5236,  5246,  1418,   564,   564,   564,  2232,
    1502,  5255,  6099,  6654,  6389,   564,  -442,   564,   564,   564,
    2150,   564,  2151,   564,   564,   564,  -442,  5264,  5273,  5285,
     564,  2152,   564,  2237,  2238,  2239,  5294,  5303,  5315,  8122,
    2216,  5324,  2217,  8137,  5333,  5342,   564,   564,   564,  8146,
    2218,  5351,   564,   564,   564,   564,   564,   564,  2243,  2175,
     564,  2177,  2248,   564,   564,  1520,  1531,  1560,  2249,  2178,
     564,  6070,  6645,  6374,  1570,  5360,  5371,   564,   564,  5380,
     564,   564,  5389,  1590,  2050,  2051,  2052,   564,   564,  5398,
    2263,  2267,  2280,  -442,   564,   564,  8054,  8608,   564,  9508,
    8065,   564,  -442,  2281,  2282,  2283,  8074,  8617,   564,   564,
     564,   564,  5413,  5422,  2284,  2287,  1599,  2288,  5431,  1857,
    1858,  1869,  2289,  2294,  5440,  6052,  6636,  6365,   564,   564,
     564,   564,  2214,   564,   564,   564,   564,   564,   564,   564,
     564,  2295,  2298,  2300,  5449,  5458,  9508,  8547,  2279,  9508,
    5467,  5476,  5485,  5494,  9508,  8559,  5507,   564,   564,   564,
     564,   564,  2305,  2235,   564,   564,   564,   564,  2318,   564,
    9508,  6627,  6350,  5516,  5525,   564,   564,  5534,  5545,  5554,
    5563,   564,  5572,  2320,  2321,   564,   564,  8479,  9508,   564,
     564,   564,   564,  8494,   564,   564,   564,  5581,  5590,  2322,
    5599,  1650,  1740,  1759,  2323,  5608,  6612,  6341,   564,   564,
     564,   564,  -442,  -442,  -442,   564,   564,  2324,  2325,  5617,
    1801,  8415,  5626,  8424,  1812,   564,   564,   564,  2247,  2345,
     564,  2346,  2265,  6603,  7065,  5635,  2326,   564,  5644,   564,
    2327,  2349,  2352,   564,  2271,  8353,   564,  8365,  2272,   564,
     564,  5653,   564,  2360,  5662,  2361,   564,  6591,  9484,   564,
    9508,   564,   564,   564,  8626,  2366,  2368,  1821,  8285,  1866,
    8299,  2369,   564,   564,  2292,  2370,  2293,  2371,   564,  6579,
    8759,  2348,   564,  2353,   564,  8568,  2378,  2380,  2299,  8219,
    2303,  8228,  2382,   564,   564,   564,  2386,   564,  2391,   564,
    6570,  8738,  9508,   564,  9508,   564,  8503,  2401,  2402,  8156,
    8168,  2404,   564,   564,  2405,  2411,   564,  6561,  8717,   564,
     564,  8433,  2412,  2415,  8088,  8101,  2416,   564,   564,  2417,
    2418,   564,  6550,  8695,   564,   564,  8374,  2419,  2420,  9508,
    9508,  2421,   564,   564,   564,  9499,  9508,  8308,  2422,  2426,
     564,   564,  8768,  8237,  2427,  2429,   564,   564,  8750,  8177,
    2430,  2431,   564,   564,  8729,  8110,  2432,  2433,   564,   564,
    8704,  9508,  2434,   564,  9508
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   143,   161,   160,   166,     0,     0,     0,     0,     0,
      13,   182,     0,   168,   169,   170,   171,   172,   173,   174,
       0,     0,     0,     7,     6,     0,     0,   183,    14,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,   176,     0,     8,    10,    11,     9,
      12,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,   159,   151,
       0,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    77,     0,   178,
       1,   142,     0,     0,     0,     0,     0,     0,     0,   177,
     163,   152,   153,   154,   155,   156,   157,   162,     0,    57,
       0,     0,     0,   179,   181,     0,     0,     5,    72,     0,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    68,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,    61,
     158,   145,   146,   147,   148,   149,   150,   144,     0,   167,
     165,   164,   185,   187,     0,     0,     4,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    88,    91,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    62,    58,   188,
     186,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,    63,    52,    55,    51,
      54,    92,    93,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    39,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
       0,     0,     0,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
      41,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
      83,    81,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,   112,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   136,   137,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,    49,    42,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,   111,   109,
       0,    97,    98,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    23,    84,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,    26,     0,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,   110,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,     0,   121,
       0,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   123,     0,     0,   125,
       0,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   126,
     127,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   128,     0,     0,   105
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -442,  -442,  -442,  -442,  -252,  -442,  -442,  -442,  -442,  -442,
    -442,  -442,    -8,    16,   -55,  2269
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    45,    46,    47,   359,   138,    48,    49,    50,    89,
     151,    51,    52,   203,   143,   204
  };

  const short int
  feiparser::yytable_[] =
  {
      65,    66,   122,   123,   124,   125,   126,   127,   114,   145,
     128,   144,    68,    69,    72,   260,    53,   334,   335,   360,
     122,   123,   124,   125,   126,   127,    62,   110,   128,   122,
     123,   124,   125,   126,   127,   118,    63,   128,   122,   123,
     124,   125,   126,   127,   239,    64,   128,   131,   132,   133,
     134,   135,   136,   137,   139,   140,   141,    67,   122,   123,
     124,   125,   126,   127,   758,   147,   128,   121,    73,     2,
       3,     4,     5,   336,     6,     7,   122,   123,   124,   125,
     126,   127,   164,    74,   128,   126,   127,   240,   381,   128,
     241,   242,    90,    13,    14,    15,    16,    17,    18,    19,
     383,   382,    20,    91,   357,   188,   447,   127,    21,   759,
     128,   243,   358,   384,   191,   192,   193,   194,   195,   196,
     197,    29,   122,   123,   124,   125,   126,   127,   179,   180,
     128,   122,   123,   124,   125,   126,   127,   206,   207,   128,
     387,   208,   209,    92,   210,   211,   212,   213,   214,    93,
     251,   217,   218,   388,   220,   108,   198,   199,   224,   225,
     226,   227,   228,   229,   339,   340,   109,   122,   123,   124,
     125,   126,   127,   372,   373,   128,   122,   123,   124,   125,
     126,   127,   288,   284,   128,   289,   285,   675,   676,   290,
     248,   120,  1534,  1535,     2,     3,     4,     5,   111,     6,
       7,     8,  1849,  1850,     9,   113,   112,    10,   262,    11,
     116,    12,   670,   671,   672,   117,   673,   674,    13,    14,
      15,    16,    17,    18,    19,   275,   119,    20,   347,   348,
     349,   130,   350,    21,   146,    22,    23,    24,    25,  1915,
    1916,    26,   128,    27,   148,    28,    29,   122,   123,   124,
     125,   126,   127,   149,   150,   128,   341,   342,   343,   354,
     152,   356,    30,   153,   361,   362,   363,   154,   364,    31,
      32,    33,    34,   122,   123,   124,   125,   126,   127,   155,
     156,   128,   157,   380,   158,   159,   160,    35,    36,     2,
       3,     4,     5,   161,     6,     7,   124,   125,   126,   127,
      37,   162,   128,   165,   163,   166,    38,   172,   167,   168,
     169,   170,   171,    13,    14,    15,    16,    17,    18,    19,
     175,   177,    20,   176,   173,   174,   181,   183,    21,   122,
     123,   124,   125,   126,   127,   178,   436,   128,   182,   184,
     186,    70,    71,   443,   122,   123,   124,   125,   126,   127,
     187,   189,   128,   205,   215,   219,   221,   449,   450,   451,
     216,   223,   230,    90,     2,     3,     4,     5,   460,     6,
       7,   231,   237,   464,   232,   466,   234,   235,   236,   470,
     233,   238,   473,   245,   244,   246,    -1,   250,    13,    14,
      15,    16,    17,    18,    19,   142,   265,    20,   267,   268,
     276,   272,   269,    21,   273,   122,   123,   124,   125,   126,
     127,   115,   274,   128,   277,   261,    29,   279,   510,   511,
     280,   513,   278,   515,   516,   281,   282,   286,   283,   521,
     522,   287,   291,   337,    94,    95,   528,   529,    96,   338,
      97,   142,   344,   345,    98,   535,   351,   352,   353,   355,
     358,   541,   365,    99,   366,   544,   367,   368,   129,   122,
     123,   124,   125,   126,   127,   222,   369,   128,   370,   375,
     376,   463,   509,   122,   123,   124,   125,   126,   127,   371,
     607,   128,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     589,   374,   385,   122,   123,   124,   125,   126,   127,  1426,
    1427,   128,   377,   378,   379,   602,   603,   604,   605,   386,
     389,   390,   609,   610,   407,   415,   613,   391,   392,   393,
     394,   430,   395,    43,   621,   622,   396,   416,   625,  1318,
     627,   628,   629,   417,   631,   632,   397,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   100,  1780,  1781,   398,     2,     3,     4,
       5,   399,     6,     7,   122,   123,   124,   125,   126,   127,
     400,   408,   128,   401,   402,   403,   404,   405,   680,   681,
     682,    13,    14,    15,    16,    17,    18,    19,   689,   406,
      20,   190,   428,   694,   426,   409,    21,   410,   411,   412,
     200,   413,   418,   414,   419,   705,   427,   420,   421,    29,
     122,   123,   124,   125,   126,   127,   422,   423,   128,   122,
     123,   124,   125,   126,   127,   429,   424,   128,   122,   123,
     124,   125,   126,   127,   425,   431,   128,   432,   433,   434,
      39,   435,   438,    40,    41,   440,    42,   437,    43,   441,
     439,   442,   444,   445,    44,   122,   123,   124,   125,   126,
     127,   452,   453,   128,   454,   455,   456,   766,   457,   458,
     461,   201,   459,   462,   465,   512,   467,   775,   468,   777,
     469,   779,   780,   781,   471,   783,   472,   474,   490,   787,
     475,   789,   790,   791,   476,   793,   794,   785,   796,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   830,   477,  1066,   478,   122,   123,   124,   125,
     126,   127,   840,    43,   128,   479,   480,   481,   482,   483,
     847,   848,   484,   874,   851,   852,   853,   485,   854,   486,
     487,   488,   489,   491,   860,   492,   862,   493,   875,   122,
     123,   124,   125,   126,   127,   494,   495,   128,   122,   123,
     124,   125,   126,   127,   496,   497,   128,   122,   123,   124,
     125,   126,   127,   498,   499,   128,   122,   123,   124,   125,
     126,   127,   500,   501,   128,   502,   503,   504,   505,   506,
     507,   508,   514,   908,   909,   910,   523,   524,    43,   914,
     915,   916,   517,   918,   919,   920,   518,   922,   923,   876,
     519,   926,   122,   123,   124,   125,   126,   127,   932,   525,
     128,   526,   530,   531,   534,   939,   536,   941,   542,   943,
     944,   945,   537,   947,   948,   538,   539,   540,   543,   590,
     546,   955,   956,   957,   958,   593,   548,   595,   596,   963,
     964,   965,   122,   123,   124,   125,   126,   127,   597,   549,
     128,   598,   550,   877,   122,   123,   124,   125,   126,   127,
     606,   552,   128,   986,   987,   988,   553,   878,   555,   556,
     557,   558,   559,   995,   560,   561,   562,   563,   564,   999,
     599,   565,   566,   567,   568,  1005,  1006,  1007,   122,   123,
     124,   125,   126,   127,   611,   612,   128,   883,   616,   617,
     615,   618,  1022,   122,   123,   124,   125,   126,   127,   569,
     570,   128,   619,   624,   626,   630,   633,   669,  1038,  1039,
    1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,  1051,  1052,  1053,  1054,  1055,   122,   123,   124,   125,
     126,   127,   677,  1061,   128,   679,   678,   683,   685,  1067,
    1068,   690,  1069,  1070,  1071,   691,   996,   695,   684,   697,
    1077,   698,  1079,   699,  1080,  1081,  1082,  1083,  1084,  1085,
    1086,  1087,  1088,  1089,   700,  1091,  1092,  1093,  1094,  1095,
    1096,   701,  1098,   703,  1100,  1101,  1102,  1103,  1104,  1105,
     702,    43,   122,   123,   124,   125,   126,   127,   706,   708,
     128,   122,   123,   124,   125,   126,   127,   732,  1122,   128,
    1124,  1125,  1126,   712,   884,  1129,  1130,   733,   715,   734,
     735,   736,   737,   885,   738,  1137,   122,   123,   124,   125,
     126,   127,   886,   739,   128,   740,   741,   742,   751,   122,
     123,   124,   125,   126,   127,   743,   744,   128,   745,   746,
     747,  1163,   122,   123,   124,   125,   126,   127,   748,  1029,
     128,   122,   123,   124,   125,   126,   127,   749,   750,   128,
     122,   123,   124,   125,   126,   127,   752,   753,   128,   754,
     122,   123,   124,   125,   126,   127,   755,   756,   128,   760,
    1200,  1201,  1202,   764,   765,   757,  1206,  1207,  1208,  1209,
    1210,  1211,  1212,  1213,  1214,  1215,   122,   123,   124,   125,
     126,   127,   767,   768,   128,   769,   771,   773,   774,  1229,
    1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,   772,  1249,
    1159,   776,  1252,   778,   782,   784,  1255,  1256,   786,  1258,
    1259,   788,   792,   795,   831,  1264,  1265,  1266,   122,   123,
     124,   125,   126,   127,   832,   833,   128,   834,   835,  1278,
    1279,  1280,  1281,  1160,   836,  1284,   837,   838,  1287,  1288,
     839,   841,  1162,   842,   844,   122,   123,   124,   125,   126,
     127,  1487,    54,   128,    55,    56,    57,    58,    59,    60,
    1592,   843,  1311,  1312,   845,   846,  1315,    61,   849,  1317,
     856,   863,  1320,   850,   864,   866,   870,   873,   913,  1326,
    1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,
     911,   912,   921,   931,   933,   934,  1595,   935,  1345,   927,
     937,  1348,  1349,  1350,  1351,   122,   123,   124,   125,   126,
     127,   950,   951,   128,   940,   122,   123,   124,   125,   126,
     127,   942,  1369,   128,   122,   123,   124,   125,   126,   127,
     952,  1377,   128,  1379,  1380,  1381,  1599,  1383,   946,   949,
     953,   122,   123,   124,   125,   126,   127,   959,  1608,   128,
     954,   961,   960,  1399,  1400,   962,  1402,   966,   967,   968,
     969,   970,   971,   972,  1409,  1410,  1411,  1412,  1413,  1414,
    1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
     973,  1488,  1784,   974,  1430,   975,  1432,  1433,  1434,   976,
     977,   978,   979,   980,  1437,   981,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,   982,  1450,  1451,  1452,
    1453,   983,   991,  1456,   997,   998,  1459,  1460,  1461,  1462,
    1463,   122,   123,   124,   125,   126,   127,  1000,  1001,   128,
    1785,   122,   123,   124,   125,   126,   127,  1002,  1481,   128,
    1483,  1484,  1003,  1008,  1009,  1010,  1012,  1489,  1490,  1014,
    1491,   122,   123,   124,   125,   126,   127,  1018,  1021,   128,
    1023,  1502,  1031,   122,   123,   124,   125,   126,   127,  1024,
    1510,   128,  1123,   122,   123,   124,   125,   126,   127,  1025,
    1026,   128,  1032,  1033,  1034,  1056,  1821,  1057,  1059,  1060,
    1062,  1063,  1532,  1064,  1072,  1831,  1073,  1136,  1131,  1097,
    1078,  1099,  1135,  1128,  1542,  1543,  1544,  1545,  1546,  1547,
    1548,  1549,  1550,  1551,   929,  1138,  1139,  1140,  1142,  1557,
    1558,  1559,  1154,  1161,  1170,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1579,  1580,  1171,  1582,  1848,  1584,  1172,  1173,
    1587,  1588,  1589,  1590,  1174,  1885,  1193,   122,   123,   124,
     125,   126,   127,  1175,  1943,   128,  1603,  1604,  1605,  1176,
    1177,  1178,  1606,  1607,  1956,   122,   123,   124,   125,   126,
     127,  1179,  1180,   128,  1181,  1198,   122,   123,   124,   125,
     126,   127,  1182,  1625,   128,  1627,  1183,  1628,  1629,  1184,
    2013,  1185,  1190,  1632,  1194,  1634,  1635,  1195,  1637,  1638,
    1639,  1196,  1641,  1642,  1643,   122,   123,   124,   125,   126,
     127,  1650,  1651,   128,  1216,   122,   123,   124,   125,   126,
     127,  1197,  1203,   128,  1205,  1204,  1217,  1218,  1667,  1219,
    1220,  1221,  1671,  1672,  1222,   122,   123,   124,   125,   126,
     127,  1224,  2076,   128,   122,   123,   124,   125,   126,   127,
    1223,  1226,   128,  1227,  1693,  1694,  1695,  1696,  1697,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,  2080,
    1709,  1228,  1248,  1250,  1251,  1714,  1253,  1716,  1717,  1254,
    1719,  1720,  1721,  1257,  1723,  1260,  1725,  1726,  1727,  1728,
    1729,  1730,  1277,  1732,  1733,   122,   123,   124,   125,   126,
     127,  1282,  1283,   128,  1285,  1286,  1309,  1289,  1314,  1321,
    1744,  1322,  1746,  1747,  1748,  1749,  1323,  1751,   101,  1341,
    1754,  1342,  1344,  1352,  1758,  1353,  1354,  1355,  1356,  2141,
    1357,  1358,  1359,  1767,  1360,  1480,  1361,  1362,  1363,  2142,
    1364,   102,  1365,  1366,   103,  1779,   104,  1782,  2143,  1367,
     105,  1368,  1786,   106,  1787,  1788,  1372,  1790,  1791,  1792,
    1374,  1794,  1795,  1796,  1376,  2160,  1382,  1395,  1396,  1397,
    1803,  1804,  1805,  1806,  1807,  1808,  1809,  1810,  1811,  1812,
    1813,  1398,  1815,  1401,  1485,   122,   123,   124,   125,   126,
     127,  1404,  1405,   128,  1425,  1428,  1429,  1431,  1436,  1832,
    1833,  1834,  1449,  1836,   122,   123,   124,   125,   126,   127,
    1458,  1507,   128,  1508,  1509,  1516,  1847,  1517,  1482,  1851,
    1852,  1518,  1519,  1520,  1856,  1857,  1858,  1859,  1860,  1861,
    1862,  1863,  1864,  1521,  1522,   692,  1523,  1869,  1524,  1871,
    1872,  1873,  1525,  1526,  1527,  2256,   122,   123,   124,   125,
     126,   127,  1884,  1528,   128,  1886,  1887,   122,   123,   124,
     125,   126,   127,  1529,  1530,   128,   122,   123,   124,   125,
     126,   127,  1531,  1538,   128,  2260,  1907,  1908,  1909,  1910,
    1911,  1912,  1913,  1914,  1539,  1917,  1553,  2262,  1554,  1555,
    1556,  1919,  1920,  1560,  1922,  1923,  1924,  2266,  1926,  1927,
    1561,  1929,  1930,  1931,  1932,  1585,  1586,  1935,  1936,  1583,
    1633,   122,   123,   124,   125,   126,   127,  1601,  1945,   128,
    1947,  1602,  1636,  1950,  1640,  1652,  1653,  1954,  1654,   107,
    1957,  1649,  1655,  1656,  1657,  1962,  1963,   122,   123,   124,
     125,   126,   127,  1658,  1659,   128,  1972,  1660,  1973,  1661,
    1974,  1975,  1662,  1977,  1978,  1979,  1663,  1981,  1664,  1665,
    1685,  1674,  1677,  1987,  1681,  1686,  1990,  1991,  1992,  1993,
    1994,  1995,  1996,  1997,  1690,   122,   123,   124,   125,   126,
     127,  2271,  1687,   128,  1688,  2009,  1689,  1710,  1711,  1715,
    2014,  1718,  1724,  1722,  1731,  1760,  1770,  1771,  1772,  2324,
    2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,
    2325,  1773,  2033,  2034,  2035,  1774,  1775,  2037,  2038,  2039,
    1816,   122,   123,   124,   125,   126,   127,  1776,  1777,   128,
    1778,  1797,  1798,  2053,  2054,  1799,  1800,  1801,  2058,  2326,
    1818,  1819,  2062,  2063,  2064,  2065,  2066,  2067,  2068,  2333,
    2070,  2071,  2072,  1824,  2074,  1828,  1820,  2077,  2078,  2079,
    1837,  1835,  2082,  2083,  1853,  1854,  1855,  1865,  1874,  2342,
    1875,  2091,  1876,  1877,  1878,  2095,  1879,  2097,  2372,  1880,
    1882,  1883,  1888,  2102,  1898,  1899,  1900,  1901,  1903,  2108,
    2109,  2110,  2111,  1921,  2113,  2114,  2115,  1925,  2117,  1918,
    1928,  2120,  2121,  2122,  1933,  2124,  2125,  2126,  2127,  2128,
     122,   123,   124,   125,   126,   127,  1934,  1944,   128,  1966,
    2139,  1982,   122,   123,   124,   125,   126,   127,  1967,  2452,
     128,  2150,  2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,
    2159,  1968,  2161,  1969,  1970,  1971,  2165,  2166,  2167,   122,
     123,   124,   125,   126,   127,  1984,  1985,   128,   122,   123,
     124,   125,   126,   127,  1986,  2002,   128,  2006,  2008,  2015,
    2189,  2190,  2191,  2192,  2193,  2194,  2195,  2010,  2197,  2198,
    2199,  2011,  2201,  2012,  2203,  2031,  2040,  2041,  2042,  2208,
    2209,   122,   123,   124,   125,   126,   127,  2069,  2217,   128,
    2073,  2081,  2221,  2098,  2105,  2106,  2225,  2226,  2227,  2228,
    2107,  2119,  2133,  2137,  2144,  2234,  2235,  2236,  2237,  2453,
    2239,  2240,  2241,  2140,  2243,  2244,  2245,  2162,  2168,  2169,
    2170,  2250,  2251,  2252,  2253,  2254,  2163,  2164,  2454,   122,
     123,   124,   125,   126,   127,  2183,  2184,   128,  2267,  2268,
    2269,  2185,  2181,  2196,  2200,  2202,  2207,  2276,  2204,  2277,
    2278,  2279,  2205,  2281,  2206,  2283,  2284,  2285,  2223,  2231,
    2232,  2233,  2289,  2259,  2291,  2263,  2270,  2280,  2282,  2290,
    2468,  2292,  2293,  2294,  2299,  2301,  2309,  2317,  2305,  2306,
    2307,  2472,  2321,  2327,  2311,  2312,  2313,  2314,  2315,  2316,
    2514,  2318,  2319,  2320,  2328,  2322,  2323,  2349,  2343,  2344,
    2345,  2350,  2329,   122,   123,   124,   125,   126,   127,  2336,
    2337,   128,  2339,  2340,  2351,  2359,  2360,  2361,  2370,  2346,
    2347,  2371,  2373,  2378,  2375,  2376,  2352,  2353,  2379,  2397,
    2356,  2388,  2398,  2358,  2399,  2516,  2377,  2403,  1472,  2415,
    2364,  2365,  2366,  2367,   696,   122,   123,   124,   125,   126,
     127,  2416,  2421,   128,  2435,  2436,  2450,  2455,  2465,  2466,
    2384,  2385,  2386,  2387,  2476,  2389,  2390,  2391,  2392,  2393,
    2394,  2395,  2396,   122,   123,   124,   125,   126,   127,  2477,
    2479,   128,  2480,  2489,  2484,  2488,  2490,  2492,  2496,  2410,
    2411,  2412,  2413,  2414,  2501,  2503,  2417,  2418,  2419,  2420,
    2512,  2422,  2513,  2518,  2522,  2524,  2528,  2427,  2428,  2521,
    2523,  2530,  2533,  2433,  2534,  2535,  2539,  2437,  2438,  2537,
    2543,  2440,  2441,  2442,  2443,  2545,  2445,  2446,  2447,   122,
     123,   124,   125,   126,   127,  2552,  2553,   128,  2556,  2559,
    2459,  2460,  2461,  2462,   623,  2560,  2567,  2463,  2464,  2568,
    2571,  2574,  2575,  2582,  2583,  2584,  2590,  2473,  2474,  2475,
    2591,  2596,  2478,  2597,  2602,  2603,  2608,  2609,  2613,  2485,
       0,  2487,   249,     0,     0,  2491,     0,     0,  2494,     0,
       0,  2497,  2498,     0,  2500,     0,     0,     0,  2504,     0,
       0,  2507,     0,  2508,  2509,  2510,     0,     0,   122,   123,
     124,   125,   126,   127,  2519,  2520,   128,     0,     0,     0,
    2525,     0,     0,     0,  2529,     0,  2531,     0,     0,     0,
       0,     0,  1301,     0,     0,  2540,  2541,  2542,     0,  2544,
       0,  2546,     0,   707,     0,  2549,     0,  2550,   122,   123,
     124,   125,   126,   127,  2557,  2558,   128,     0,  2561,     0,
       0,  2564,  2565,   122,   123,   124,   125,   126,   127,  2572,
    2573,   128,     0,  2576,     0,     0,  2579,  2580,     0,     0,
       0,     0,   709,     0,  2585,  2586,  2587,     0,     0,     0,
       0,   710,  2592,  2593,     0,     0,     0,     0,  2598,  2599,
       0,     0,     0,     0,  2604,  2605,     0,     0,     0,     0,
    2610,  2611,     0,     0,     1,  2614,     2,     3,     4,     5,
       0,     6,     7,     8,   711,     0,     9,     0,     0,    10,
       0,    11,     0,    12,     0,     0,     0,     0,     0,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
       0,     0,   446,     0,     0,    21,     0,    22,    23,    24,
      25,     0,     0,    26,     0,    27,     0,    28,    29,     0,
       0,     0,     2,     3,     4,     5,     0,     6,     7,     8,
       0,     0,     9,     0,    30,    10,     0,    11,     0,    12,
       0,    31,    32,    33,    34,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    20,     0,     0,     0,    35,
      36,    21,     0,    22,    23,    24,    25,     0,     0,    26,
       0,    27,    37,    28,    29,     0,     0,     0,    38,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
      30,     0,     0,     0,     0,     0,   713,    31,    32,    33,
      34,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,     0,    35,    36,     0,     0,     0,
       2,     3,     4,     5,     0,     6,     7,     8,    37,     0,
       9,     0,     0,    10,    38,    11,     0,    12,   714,     0,
       0,     0,     0,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    20,     0,     0,     0,     0,     0,    21,
       0,    22,    23,    24,    25,     0,   716,    26,     0,    27,
       0,    28,    29,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,    30,     0,
     128,     0,     0,     0,     0,    31,    32,    33,    34,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,     0,    35,    36,   122,   123,   124,   125,   126,
     127,     0,   717,   128,     0,     0,    37,     0,     0,     0,
       0,     0,    38,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,   718,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,   719,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,   720,   122,   123,   124,
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
     127,     0,   721,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,   722,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,    39,     0,     0,    40,
      41,     0,    42,     0,    43,     0,     0,     0,     0,     0,
      44,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,   723,     0,   128,   122,
     123,   124,   125,   126,   127,   724,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,     0,   725,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,   726,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,   727,     0,   128,   122,
     123,   124,   125,   126,   127,   728,     0,   128,   122,   123,
     124,   125,   126,   127,   729,     0,   128,   122,   123,   124,
     125,   126,   127,   730,     0,   128,   122,   123,   124,   125,
     126,   127,   731,     0,   128,   122,   123,   124,   125,   126,
     127,   865,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,   867,     0,   122,   123,   124,
     125,   126,   127,     0,   868,   128,   122,   123,   124,   125,
     126,   127,     0,   869,   128,   122,   123,   124,   125,   126,
     127,     0,   871,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,   872,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,     0,
     879,     0,   122,   123,   124,   125,   126,   127,     0,   880,
     128,   122,   123,   124,   125,   126,   127,     0,   881,   128,
     122,   123,   124,   125,   126,   127,     0,   882,   128,   122,
     123,   124,   125,   126,   127,     0,   887,   128,   122,   123,
     124,   125,   126,   127,     0,   888,   128,   122,   123,   124,
     125,   126,   127,     0,   889,   128,   122,   123,   124,   125,
     126,   127,     0,  1013,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
    1015,   128,   122,   123,   124,   125,   126,   127,     0,  1016,
     128,   122,   123,   124,   125,   126,   127,     0,  1017,   128,
     122,   123,   124,   125,   126,   127,     0,  1019,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  1020,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  1027,     0,
     122,   123,   124,   125,   126,   127,     0,  1028,   128,   122,
     123,   124,   125,   126,   127,     0,  1030,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  1035,     0,
     122,   123,   124,   125,   126,   127,     0,  1036,   128,   122,
     123,   124,   125,   126,   127,     0,  1037,   128,   122,   123,
     124,   125,   126,   127,     0,  1058,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1144,     0,   122,   123,   124,   125,
     126,   127,     0,  1145,   128,   122,   123,   124,   125,   126,
     127,     0,  1146,   128,   122,   123,   124,   125,   126,   127,
       0,  1147,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  1148,     0,   122,   123,
     124,   125,   126,   127,     0,  1149,   128,   122,   123,   124,
     125,   126,   127,     0,  1150,   128,   122,   123,   124,   125,
     126,   127,     0,  1151,   128,   122,   123,   124,   125,   126,
     127,     0,  1152,   128,   122,   123,   124,   125,   126,   127,
       0,  1153,   128,   122,   123,   124,   125,   126,   127,     0,
    1167,   128,   122,   123,   124,   125,   126,   127,     0,  1168,
     128,   122,   123,   124,   125,   126,   127,     0,  1169,   128,
     122,   123,   124,   125,   126,   127,     0,  1187,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    1267,     0,   122,   123,   124,   125,   126,   127,     0,  1268,
     128,   122,   123,   124,   125,   126,   127,     0,  1269,   128,
     122,   123,   124,   125,   126,   127,     0,  1270,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1271,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,  1272,     0,   128,   122,   123,
     124,   125,   126,   127,  1273,     0,   128,   122,   123,   124,
     125,   126,   127,  1274,     0,   128,   122,   123,   124,   125,
     126,   127,  1275,     0,   128,   122,   123,   124,   125,   126,
     127,  1276,     0,   128,   122,   123,   124,   125,   126,   127,
    1290,     0,   128,   122,   123,   124,   125,   126,   127,  1291,
       0,   128,   122,   123,   124,   125,   126,   127,  1292,     0,
     128,   122,   123,   124,   125,   126,   127,  1310,     0,   128,
     122,   123,   124,   125,   126,   127,  1319,     0,   128,   122,
     123,   124,   125,   126,   127,  1325,     0,   128,   122,   123,
     124,   125,   126,   127,  1375,     0,   128,   122,   123,   124,
     125,   126,   127,  1384,     0,   128,   122,   123,   124,   125,
     126,   127,  1385,     0,   128,   122,   123,   124,   125,   126,
     127,  1386,     0,   128,   122,   123,   124,   125,   126,   127,
    1387,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,  1388,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1389,     0,   122,   123,   124,   125,   126,
     127,     0,  1390,   128,   122,   123,   124,   125,   126,   127,
       0,  1391,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1392,     0,   122,   123,   124,   125,   126,
     127,     0,  1393,   128,   122,   123,   124,   125,   126,   127,
       0,  1406,   128,   122,   123,   124,   125,   126,   127,     0,
    1407,   128,   122,   123,   124,   125,   126,   127,     0,  1408,
     128,   122,   123,   124,   125,   126,   127,     0,  1435,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1438,
       0,   122,   123,   124,   125,   126,   127,     0,  1492,   128,
     122,   123,   124,   125,   126,   127,     0,  1493,   128,   122,
     123,   124,   125,   126,   127,     0,  1494,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  1495,     0,   122,   123,   124,   125,   126,   127,     0,
    1496,   128,   122,   123,   124,   125,   126,   127,     0,  1497,
     128,   122,   123,   124,   125,   126,   127,     0,  1498,   128,
     122,   123,   124,   125,   126,   127,     0,  1499,   128,   122,
     123,   124,   125,   126,   127,     0,  1500,   128,   122,   123,
     124,   125,   126,   127,     0,  1501,   128,   122,   123,   124,
     125,   126,   127,     0,  1513,   128,   122,   123,   124,   125,
     126,   127,     0,  1514,   128,   122,   123,   124,   125,   126,
     127,     0,  1515,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  1533,     0,   122,   123,   124,
     125,   126,   127,     0,  1540,   128,   122,   123,   124,   125,
     126,   127,     0,  1541,   128,   122,   123,   124,   125,   126,
     127,     0,  1581,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  1591,     0,   122,   123,   124,   125,   126,
     127,     0,  1593,   128,   122,   123,   124,   125,   126,   127,
       0,  1594,   128,   122,   123,   124,   125,   126,   127,     0,
    1596,   128,   122,   123,   124,   125,   126,   127,     0,  1597,
     128,   122,   123,   124,   125,   126,   127,     0,  1598,   128,
     122,   123,   124,   125,   126,   127,     0,  1600,   128,   122,
     123,   124,   125,   126,   127,     0,  1609,   128,   122,   123,
     124,   125,   126,   127,     0,  1610,   128,   122,   123,   124,
     125,   126,   127,     0,  1626,   128,   122,   123,   124,   125,
     126,   127,     0,  1630,   128,   122,   123,   124,   125,   126,
     127,     0,  1631,   128,   122,   123,   124,   125,   126,   127,
       0,  1666,   128,   122,   123,   124,   125,   126,   127,     0,
    1668,   128,   122,   123,   124,   125,   126,   127,     0,  1673,
     128,   122,   123,   124,   125,   126,   127,     0,  1675,   128,
     122,   123,   124,   125,   126,   127,     0,  1676,   128,   122,
     123,   124,   125,   126,   127,     0,  1678,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1679,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  1680,     0,
     122,   123,   124,   125,   126,   127,     0,  1682,   128,   122,
     123,   124,   125,   126,   127,     0,  1684,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  1691,     0,
     122,   123,   124,   125,   126,   127,     0,  1692,   128,   122,
     123,   124,   125,   126,   127,     0,  1708,   128,   122,   123,
     124,   125,   126,   127,     0,  1712,   128,   122,   123,   124,
     125,   126,   127,     0,  1713,   128,   122,   123,   124,   125,
     126,   127,     0,  1743,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1745,     0,   122,   123,   124,   125,
     126,   127,     0,  1750,   128,   122,   123,   124,   125,   126,
     127,     0,  1752,   128,   122,   123,   124,   125,   126,   127,
       0,  1753,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  1755,     0,   122,   123,
     124,   125,   126,   127,     0,  1756,   128,   122,   123,   124,
     125,   126,   127,     0,  1757,   128,   122,   123,   124,   125,
     126,   127,     0,  1759,   128,   122,   123,   124,   125,   126,
     127,     0,  1761,   128,   122,   123,   124,   125,   126,   127,
       0,  1768,   128,   122,   123,   124,   125,   126,   127,     0,
    1769,   128,   122,   123,   124,   125,   126,   127,     0,  1814,
     128,   122,   123,   124,   125,   126,   127,     0,  1817,   128,
     122,   123,   124,   125,   126,   127,     0,  1822,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    1823,     0,   122,   123,   124,   125,   126,   127,     0,  1825,
     128,   122,   123,   124,   125,   126,   127,     0,  1826,   128,
     122,   123,   124,   125,   126,   127,     0,  1827,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  1829,     0,
     122,   123,   124,   125,   126,   127,     0,  1830,   128,   122,
     123,   124,   125,   126,   127,     0,  1838,   128,   122,   123,
     124,   125,   126,   127,     0,  1839,   128,   122,   123,   124,
     125,   126,   127,     0,  1881,   128,   122,   123,   124,   125,
     126,   127,     0,  1889,   128,   122,   123,   124,   125,   126,
     127,     0,  1890,   128,   122,   123,   124,   125,   126,   127,
       0,  1892,   128,   122,   123,   124,   125,   126,   127,     0,
    1893,   128,   122,   123,   124,   125,   126,   127,     0,  1894,
     128,   122,   123,   124,   125,   126,   127,     0,  1896,   128,
     122,   123,   124,   125,   126,   127,     0,  1897,   128,   122,
     123,   124,   125,   126,   127,     0,  1902,   128,   122,   123,
     124,   125,   126,   127,     0,  1905,   128,   122,   123,   124,
     125,   126,   127,     0,  1906,   128,   122,   123,   124,   125,
     126,   127,     0,  1946,   128,   122,   123,   124,   125,   126,
     127,     0,  1948,   128,     0,   122,   123,   124,   125,   126,
     127,     0,  1949,   128,   122,   123,   124,   125,   126,   127,
       0,  1951,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1952,     0,   122,   123,   124,   125,   126,
     127,     0,  1953,   128,   122,   123,   124,   125,   126,   127,
       0,  1955,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1961,     0,   122,   123,   124,   125,   126,
     127,     0,  1964,   128,   122,   123,   124,   125,   126,   127,
       0,  1965,   128,   122,   123,   124,   125,   126,   127,     0,
    1983,   128,   122,   123,   124,   125,   126,   127,     0,  1988,
     128,   122,   123,   124,   125,   126,   127,     0,  1998,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1999,
       0,   122,   123,   124,   125,   126,   127,     0,  2000,   128,
     122,   123,   124,   125,   126,   127,     0,  2001,   128,   122,
     123,   124,   125,   126,   127,     0,  2003,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  2004,     0,   122,   123,   124,   125,   126,   127,     0,
    2005,   128,   122,   123,   124,   125,   126,   127,     0,  2007,
     128,   122,   123,   124,   125,   126,   127,     0,  2016,   128,
     122,   123,   124,   125,   126,   127,     0,  2017,   128,   122,
     123,   124,   125,   126,   127,     0,  2032,   128,   122,   123,
     124,   125,   126,   127,     0,  2036,   128,   122,   123,   124,
     125,   126,   127,     0,  2043,   128,   122,   123,   124,   125,
     126,   127,     0,  2044,   128,   122,   123,   124,   125,   126,
     127,     0,  2045,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  2046,     0,   122,   123,   124,
     125,   126,   127,     0,  2048,   128,   122,   123,   124,   125,
     126,   127,     0,  2049,   128,   122,   123,   124,   125,   126,
     127,     0,  2050,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  2052,     0,   122,   123,   124,   125,   126,
     127,     0,  2055,   128,   122,   123,   124,   125,   126,   127,
       0,  2056,   128,   122,   123,   124,   125,   126,   127,     0,
    2057,   128,   122,   123,   124,   125,   126,   127,     0,  2060,
     128,   122,   123,   124,   125,   126,   127,     0,  2061,   128,
     122,   123,   124,   125,   126,   127,     0,  2075,   128,   122,
     123,   124,   125,   126,   127,     0,  2087,   128,   122,   123,
     124,   125,   126,   127,     0,  2088,   128,   122,   123,   124,
     125,   126,   127,     0,  2089,   128,   122,   123,   124,   125,
     126,   127,     0,  2090,   128,   122,   123,   124,   125,   126,
     127,     0,  2092,   128,   122,   123,   124,   125,   126,   127,
       0,  2093,   128,   122,   123,   124,   125,   126,   127,     0,
    2094,   128,   122,   123,   124,   125,   126,   127,     0,  2096,
     128,   122,   123,   124,   125,   126,   127,     0,  2099,   128,
       0,   122,   123,   124,   125,   126,   127,     0,  2100,   128,
     122,   123,   124,   125,   126,   127,     0,  2101,   128,   122,
     123,   124,   125,   126,   127,     0,  2103,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2104,     0,
     122,   123,   124,   125,   126,   127,     0,  2118,   128,   122,
     123,   124,   125,   126,   127,     0,  2129,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2130,     0,
     122,   123,   124,   125,   126,   127,     0,  2131,   128,   122,
     123,   124,   125,   126,   127,     0,  2132,   128,   122,   123,
     124,   125,   126,   127,     0,  2134,   128,   122,   123,   124,
     125,   126,   127,     0,  2135,   128,   122,   123,   124,   125,
     126,   127,     0,  2136,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  2138,     0,   122,   123,   124,   125,
     126,   127,     0,  2145,   128,   122,   123,   124,   125,   126,
     127,     0,  2146,   128,   122,   123,   124,   125,   126,   127,
       0,  2171,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  2172,     0,   122,   123,
     124,   125,   126,   127,     0,  2173,   128,   122,   123,   124,
     125,   126,   127,     0,  2174,   128,   122,   123,   124,   125,
     126,   127,     0,  2176,   128,   122,   123,   124,   125,   126,
     127,     0,  2177,   128,   122,   123,   124,   125,   126,   127,
       0,  2178,   128,   122,   123,   124,   125,   126,   127,     0,
    2180,   128,   122,   123,   124,   125,   126,   127,     0,  2182,
     128,   122,   123,   124,   125,   126,   127,     0,  2187,   128,
     122,   123,   124,   125,   126,   127,     0,  2188,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    2213,     0,   122,   123,   124,   125,   126,   127,     0,  2214,
     128,   122,   123,   124,   125,   126,   127,     0,  2215,   128,
     122,   123,   124,   125,   126,   127,     0,  2216,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  2218,     0,
     122,   123,   124,   125,   126,   127,     0,  2219,   128,   122,
     123,   124,   125,   126,   127,     0,  2220,   128,   122,   123,
     124,   125,   126,   127,     0,  2222,   128,   122,   123,   124,
     125,   126,   127,     0,  2224,   128,   122,   123,   124,   125,
     126,   127,     0,  2229,   128,   122,   123,   124,   125,   126,
     127,     0,  2230,   128,   122,   123,   124,   125,   126,   127,
       0,  2246,   128,   122,   123,   124,   125,   126,   127,     0,
    2247,   128,   122,   123,   124,   125,   126,   127,     0,  2248,
     128,   122,   123,   124,   125,   126,   127,     0,  2255,   128,
     122,   123,   124,   125,   126,   127,     0,  2257,   128,   122,
     123,   124,   125,   126,   127,     0,  2258,   128,   122,   123,
     124,   125,   126,   127,     0,  2261,   128,   122,   123,   124,
     125,   126,   127,     0,  2264,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  2265,   128,   122,   123,   124,   125,
     126,   127,     0,  2272,   128,   122,   123,   124,   125,   126,
     127,     0,  2286,   128,   122,   123,   124,   125,   126,   127,
       0,  2287,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  2288,     0,   122,   123,   124,   125,   126,
     127,     0,  2295,   128,   122,   123,   124,   125,   126,   127,
       0,  2296,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  2297,     0,   122,   123,   124,   125,   126,
     127,     0,  2300,   128,   122,   123,   124,   125,   126,   127,
       0,  2303,   128,   122,   123,   124,   125,   126,   127,     0,
    2304,   128,   122,   123,   124,   125,   126,   127,     0,  2310,
     128,   122,   123,   124,   125,   126,   127,     0,  2334,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  2335,
       0,   122,   123,   124,   125,   126,   127,     0,  2338,   128,
     122,   123,   124,   125,   126,   127,     0,  2341,   128,   122,
     123,   124,   125,   126,   127,     0,  2348,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  2368,     0,   122,   123,   124,   125,   126,   127,     0,
    2369,   128,   122,   123,   124,   125,   126,   127,     0,  2374,
     128,   122,   123,   124,   125,   126,   127,     0,  2380,   128,
     122,   123,   124,   125,   126,   127,     0,  2400,   128,   122,
     123,   124,   125,   126,   127,     0,  2401,   128,   122,   123,
     124,   125,   126,   127,     0,  2404,   128,   122,   123,   124,
     125,   126,   127,     0,  2405,   128,   122,   123,   124,   125,
     126,   127,     0,  2406,   128,     0,     0,     0,     0,     0,
      75,    76,  2407,    77,    78,     0,    79,    80,    81,     0,
      82,    83,    84,     0,     0,  2409,     0,     0,     0,     0,
       0,     0,    85,     0,  2425,     0,   122,   123,   124,   125,
     126,   127,     0,  2426,   128,   122,   123,   124,   125,   126,
     127,     0,  2429,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  2430,     0,   122,   123,   124,   125,   126,
     127,     0,  2431,   128,   122,   123,   124,   125,   126,   127,
       0,  2432,   128,   122,   123,   124,   125,   126,   127,     0,
    2434,   128,   122,   123,   124,   125,   126,   127,     0,  2448,
     128,   122,   123,   124,   125,   126,   127,     0,  2449,   128,
     122,   123,   124,   125,   126,   127,     0,  2451,   128,   122,
     123,   124,   125,   126,   127,     0,  2456,   128,   122,   123,
     124,   125,   126,   127,     0,  2467,   128,   122,   123,   124,
     125,   126,   127,     0,  2470,   128,     0,    86,     0,     0,
       0,     0,     0,  2483,     0,   122,   123,   124,   125,   126,
     127,     0,  2486,   128,     0,     0,  1114,     0,     0,     0,
       0,  2499,     0,     0,  1617,     0,     0,     0,     0,     0,
    2502,     0,     0,  1941,   122,   123,   124,   125,   126,   127,
       0,  1192,   128,   122,   123,   124,   125,   126,   127,     0,
    1471,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  1844,     0,    87,     0,     0,     0,     0,     0,
     993,     0,   122,   123,   124,   125,   126,   127,     0,   994,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1300,     0,   122,   123,   124,   125,   126,   127,     0,
    1739,   128,   122,   123,   124,   125,   126,   127,   992,     0,
     128,   122,   123,   124,   125,   126,   127,  1191,     0,   128,
     122,   123,   124,   125,   126,   127,  1373,     0,   128,     0,
       0,     0,     0,     0,   761,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,   762,     0,   122,   123,   124,
     125,   126,   127,     0,   763,   128,   122,   123,   124,   125,
     126,   127,     0,  1113,   128,     0,     0,  1299,     0,     0,
       0,     0,  1618,   532,     0,     0,     0,   448,     0,   122,
     123,   124,   125,   126,   127,     0,   930,   128,   122,   123,
     124,   125,   126,   127,   693,     0,   128,     0,     0,     0,
       0,     0,   770,   122,   123,   124,   125,   126,   127,     0,
     527,   128,   122,   123,   124,   125,   126,   127,   264,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,    88,     0,     0,     0,     0,  1843,   122,   123,
     124,   125,   126,   127,     0,  1738,   128,   122,   123,   124,
     125,   126,   127,  1615,     0,   128,   122,   123,   124,   125,
     126,   127,     0,  1468,   128,   122,   123,   124,   125,   126,
     127,  1297,     0,   128,     0,     0,     0,     0,     0,  1110,
     122,   123,   124,   125,   126,   127,     0,  1466,   128,   122,
     123,   124,   125,   126,   127,  1295,     0,   128,     0,     0,
       0,     0,     0,  1108,   122,   123,   124,   125,   126,   127,
       0,   894,   128,   122,   123,   124,   125,   126,   127,   654,
       0,   128,     0,     0,     0,     0,     0,  2381,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,  2330,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,   122,   123,   124,   125,   126,
     127,     0,  2273,   128,   122,   123,   124,   125,   126,   127,
    2210,     0,   128,     0,     0,     0,     0,     0,  2147,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,  2084,   122,   123,
     124,   125,   126,   127,     0,  2018,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1937,   122,   123,   124,
     125,   126,   127,     0,  1840,   128,   122,   123,   124,   125,
     126,   127,  1734,     0,   128,     0,     0,     0,     0,     0,
    1611,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1464,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  1293,   122,   123,   124,   125,
     126,   127,     0,  1106,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,   892,   122,   123,   124,   125,
     126,   127,     0,   652,   128,     0,     0,  1616,   122,   123,
     124,   125,   126,   127,     0,  1470,   128,   122,   123,   124,
     125,   126,   127,  1112,     0,   128,     0,     0,     0,     0,
       0,   898,   122,   123,   124,   125,   126,   127,     0,   658,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,   899,     0,   128,   122,
     123,   124,   125,   126,   127,   900,     0,   128,  2458,   122,
     123,   124,   125,   126,   127,     0,  2424,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
    2383,   122,   123,   124,   125,   126,   127,     0,  2332,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  2275,   122,   123,   124,   125,   126,   127,     0,
    2212,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,  2149,   122,   123,   124,   125,   126,
     127,     0,  2086,   128,   122,   123,   124,   125,   126,   127,
    2020,     0,   128,     0,     0,     0,     0,     0,  1939,     0,
     122,   123,   124,   125,   126,   127,     0,  1842,   128,   122,
     123,   124,   125,   126,   127,  1737,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,  1614,
     122,   123,   124,   125,   126,   127,     0,  1467,   128,   122,
     123,   124,   125,   126,   127,     0,   895,   128,   122,   123,
     124,   125,   126,   127,   653,     0,   128,   122,   123,   124,
     125,   126,   127,     0,   655,   128,   122,   123,   124,   125,
     126,   127,  1296,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,   893,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,  2577,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  2562,   122,
     123,   124,   125,   126,   127,     0,  2547,   128,   122,   123,
     124,   125,   126,   127,  2526,     0,   128,     0,   122,   123,
     124,   125,   126,   127,     0,  2505,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  2481,   122,   123,   124,
     125,   126,   127,     0,  2457,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,  2423,   122,
     123,   124,   125,   126,   127,     0,  2382,   128,   122,   123,
     124,   125,   126,   127,  2331,     0,   128,     0,     0,     0,
       0,     0,  2274,     0,   122,   123,   124,   125,   126,   127,
       0,  2211,   128,   122,   123,   124,   125,   126,   127,  2148,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,  2085,   122,   123,   124,   125,   126,   127,     0,  2019,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1938,   122,   123,   124,   125,   126,   127,     0,  1841,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1735,   122,   123,   124,   125,   126,   127,     0,  1612,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1107,   122,   123,   124,   125,   126,
     127,     0,  1465,   128,   122,   123,   124,   125,   126,   127,
    1294,     0,   128,     0,     0,     0,     0,     0,  1740,     0,
     122,   123,   124,   125,   126,   127,     0,  1846,   128,   122,
     123,   124,   125,   126,   127,  1845,     0,   128,   122,   123,
     124,   125,   126,   127,  1942,     0,   128,     0,     0,     0,
       0,   897,   122,   123,   124,   125,   126,   127,     0,   657,
     128,     0,   122,   123,   124,   125,   126,   127,     0,  1469,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1298,     0,   122,   123,   124,   125,   126,   127,     0,
    1111,   128,   122,   123,   124,   125,   126,   127,  1302,     0,
     128,   122,   123,   124,   125,   126,   127,     0,  1304,   128,
     122,   123,   124,   125,   126,   127,     0,  1305,   128,     0,
     122,   123,   124,   125,   126,   127,     0,  1306,   128,   122,
     123,   124,   125,   126,   127,  1473,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  1475,     0,
     122,   123,   124,   125,   126,   127,     0,  1476,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,     0,  1477,     0,   122,   123,   124,   125,   126,
     127,     0,  1619,   128,   122,   123,   124,   125,   126,   127,
       0,  1623,   128,     0,   122,   123,   124,   125,   126,   127,
       0,  1624,   128,   122,   123,   124,   125,   126,   127,  1620,
       0,   128,     0,   122,   123,   124,   125,   126,   127,     0,
    1621,   128,   122,   123,   124,   125,   126,   127,     0,  1622,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    1736,     0,   122,   123,   124,   125,   126,   127,     0,  1741,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,  1742,     0,   122,   123,   124,   125,   126,
     127,     0,  1940,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,     0,  2482,   122,
     123,   124,   125,   126,   127,     0,  1474,   128,   122,   123,
     124,   125,   126,   127,     0,  1478,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1479,
       0,   122,   123,   124,   125,   126,   127,     0,  1303,   128,
     122,   123,   124,   125,   126,   127,     0,  1307,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1308,
     122,   123,   124,   125,   126,   127,     0,  1117,   128,   122,
     123,   124,   125,   126,   127,     0,  1118,   128,   122,   123,
     124,   125,   126,   127,     0,  1119,   128,     0,   122,   123,
     124,   125,   126,   127,     0,  1120,   128,   122,   123,   124,
     125,   126,   127,     0,  1121,   128,     0,     0,     0,     0,
       0,  1115,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  1116,   122,   123,   124,   125,   126,   127,
       0,   687,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,   890,     0,   122,
     123,   124,   125,   126,   127,     0,   891,   128,     0,   122,
     123,   124,   125,   126,   127,     0,   896,   128,   122,   123,
     124,   125,   126,   127,     0,   901,   128,     0,   122,   123,
     124,   125,   126,   127,     0,   902,   128,   122,   123,   124,
     125,   126,   127,     0,   903,   128,   122,   123,   124,   125,
     126,   127,     0,   904,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   905,     0,   122,   123,   124,   125,
     126,   127,     0,   906,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,   907,     0,   122,
     123,   124,   125,   126,   127,     0,  1109,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  1613,   122,   123,   124,   125,   126,   127,     0,   659,
     128,   122,   123,   124,   125,   126,   127,   650,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,   651,     0,   122,   123,   124,   125,   126,   127,     0,
     656,   128,   122,   123,   124,   125,   126,   127,     0,   660,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     661,     0,   122,   123,   124,   125,   126,   127,     0,   662,
     128,   122,   123,   124,   125,   126,   127,     0,   663,   128,
       0,   122,   123,   124,   125,   126,   127,     0,   664,   128,
     122,   123,   124,   125,   126,   127,     0,   665,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,   666,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,   667,     0,   122,   123,   124,   125,
     126,   127,     0,   668,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,     0,  1156,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1157,
       0,   122,   123,   124,   125,   126,   127,     0,  1158,   128,
     122,   123,   124,   125,   126,   127,     0,  1132,   128,   122,
     123,   124,   125,   126,   127,     0,  1337,   128,   122,   123,
     124,   125,   126,   127,     0,  1394,   128,   122,   123,   124,
     125,   126,   127,     0,  1403,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  1866,     0,     0,     0,     0,
       0,     0,  1155,     0,  1867,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   924,     0,     0,     0,  1868,     0,
       0,     0,     0,  1343,     0,   122,   123,   124,   125,   126,
     127,     0,  1454,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  1455,     0,   122,   123,   124,
     125,   126,   127,     0,  1457,   128,     0,     0,     0,     0,
       0,  1134,     0,     0,     0,     0,  1765,     0,     0,   928,
     122,   123,   124,   125,   126,   127,  1870,     0,   128,     0,
       0,  1503,     0,   122,   123,   124,   125,   126,   127,     0,
    1683,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,  1958,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  1959,     0,   122,
     123,   124,   125,   126,   127,     0,  1960,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,  1647,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
    1766,   122,   123,   124,   125,   126,   127,     0,  1511,   128,
     122,   123,   124,   125,   126,   127,     0,  1648,   128,   122,
     123,   124,   125,   126,   127,  1346,     0,   128,   122,   123,
     124,   125,   126,   127,  1512,     0,   128,     0,     0,     0,
       0,     0,  1164,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,  1347,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,  1552,   128,
     122,   123,   124,   125,   126,   127,  1133,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1644,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,  1645,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,  1646,   122,   123,   124,   125,
     126,   127,     0,   925,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1762,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,     0,
    1763,     0,   122,   123,   124,   125,   126,   127,     0,  1764,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    1486,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1504,     0,   122,   123,   124,   125,   126,   127,     0,
    1505,   128,   122,   123,   124,   125,   126,   127,     0,  1506,
     128,   122,   123,   124,   125,   126,   127,  1316,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  1338,     0,   122,   123,   124,   125,   126,   127,
       0,  1339,   128,     0,   122,   123,   124,   125,   126,   127,
       0,  1340,   128,   122,   123,   124,   125,   126,   127,   688,
       0,   128,   122,   123,   124,   125,   126,   127,  1225,     0,
     128,     0,     0,     0,     0,  2354,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  2357,     0,   122,   123,
     124,   125,   126,   127,     0,  2362,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2569,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  2570,     0,   122,   123,   124,   125,   126,   127,
       0,  2607,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,  2298,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,  2302,     0,   122,
     123,   124,   125,   126,   127,     0,  2308,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  2554,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2555,     0,
     122,   123,   124,   125,   126,   127,     0,  2601,   128,   122,
     123,   124,   125,   126,   127,  2238,     0,   128,   122,   123,
     124,   125,   126,   127,  2242,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  2249,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  2536,     0,
     122,   123,   124,   125,   126,   127,     0,  2538,   128,   122,
     123,   124,   125,   126,   127,     0,  2595,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,  2175,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  2179,     0,   122,   123,   124,   125,   126,   127,     0,
    2186,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  2515,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,  2517,     0,   122,
     123,   124,   125,   126,   127,     0,  2589,   128,   122,   123,
     124,   125,   126,   127,     0,  2112,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,     0,
       0,  2116,   122,   123,   124,   125,   126,   127,     0,     0,
     128,  2123,     0,   122,   123,   124,   125,   126,   127,     0,
    2493,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  2495,     0,   122,   123,   124,   125,   126,   127,
       0,  2581,   128,   122,   123,   124,   125,   126,   127,  2047,
       0,   128,   122,   123,   124,   125,   126,   127,  2051,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    2059,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2469,     0,   122,   123,   124,   125,   126,   127,     0,
    2471,   128,   122,   123,   124,   125,   126,   127,     0,  2566,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    1976,     0,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  1980,     0,   122,   123,   124,   125,   126,
     127,     0,  1989,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  2439,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,     0,  2444,
       0,   122,   123,   124,   125,   126,   127,     0,  2551,   128,
     122,   123,   124,   125,   126,   127,  1891,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  1895,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  1904,     0,   122,   123,   124,   125,   126,   127,
       0,  2402,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  2408,     0,   122,   123,   124,   125,   126,
     127,     0,  2532,   128,   122,   123,   124,   125,   126,   127,
    1789,     0,   128,   122,   123,   124,   125,   126,   127,  1793,
       0,   128,   122,   123,   124,   125,   126,   127,     0,  1802,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2355,  1165,   122,   123,   124,   125,   126,   127,   545,
    2363,   128,   122,   123,   124,   125,   126,   127,   547,  2511,
     128,     0,   122,   123,   124,   125,   126,   127,     0,   551,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,     0,   554,     0,     0,  2578,     0,
     122,   123,   124,   125,   126,   127,     0,  2612,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  2563,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  2606,   122,   123,   124,   125,   126,   127,     0,  2548,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2600,   122,   123,   124,   125,   126,   127,     0,  2527,
     128,   122,   123,   124,   125,   126,   127,  1261,  2594,   128,
     122,   123,   124,   125,   126,   127,  1262,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  1263,     0,
     122,   123,   124,   125,   126,   127,     0,  1324,   128,     0,
     122,   123,   124,   125,   126,   127,     0,  1378,   128,   122,
     123,   124,   125,   126,   127,  1074,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,  1075,   128,   122,   123,   124,   125,
     126,   127,     0,  1076,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1141,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1199,   122,   123,   124,
     125,   126,   127,     0,   857,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   858,     0,   122,   123,   124,
     125,   126,   127,     0,   859,   128,   122,   123,   124,   125,
     126,   127,     0,   936,   128,   122,   123,   124,   125,   126,
     127,     0,  1004,   128,     0,   122,   123,   124,   125,   126,
     127,     0,  1127,   128,     0,     0,     0,     0,   855,     0,
     122,   123,   124,   125,   126,   127,     0,   861,   128,   122,
     123,   124,   125,   126,   127,     0,   985,   128,   122,   123,
     124,   125,   126,   127,     0,  1186,   128,   122,   123,   124,
     125,   126,   127,   346,     0,   128,     0,     0,     0,     0,
    1371,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    1536,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1313,     0,   122,   123,   124,   125,   126,   127,     0,
    1670,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,  1783,   122,   123,   124,   125,   126,   127,     0,   990,
     128,   122,   123,   124,   125,   126,   127,     0,  1188,   128,
    1537,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,     0,  1669,   122,   123,   124,   125,
     126,   127,     0,  1189,   128,   122,   123,   124,   125,   126,
     127,     0,  1370,   128,   122,   123,   124,   125,   126,   127,
     989,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,   601,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   614,     0,   258,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   620,     0,     0,     0,     0,
       0,   259,     0,     0,   917,     0,     0,     0,     0,     0,
     253,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     254,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,     0,   263,     0,   122,   123,   124,
     125,   126,   127,     0,   591,   128,   122,   123,   124,   125,
     126,   127,     0,   592,   128,   122,   123,   124,   125,   126,
     127,   185,     0,   128,     0,   122,   123,   124,   125,   126,
     127,     0,   247,   128,     0,   520,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
     533,     0,   128,   122,   123,   124,   125,   126,   127,     0,
     594,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,  1065,   122,   123,   124,
     125,   126,   127,     0,  1090,   128,     0,     0,   255,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   256,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,   257,
       0,   122,   123,   124,   125,   126,   127,     0,   608,   128,
     122,   123,   124,   125,   126,   127,   686,     0,   128,   122,
     123,   124,   125,   126,   127,  1166,     0,   128,     0,     0,
       0,   270,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,   271,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  1011,   122,
     123,   124,   125,   126,   127,     0,   600,   128,     0,   122,
     123,   124,   125,   126,   127,     0,   252,   128,     0,     0,
       0,     0,     0,  1143,   122,   123,   124,   125,   126,   127,
       0,   704,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,   938,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   984,  2506,     0,     0,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,     0,
       0,  2588,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,     0,
     309,     0,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,     0,   320,   321,   322,   323,   324,   325,   326,
       0,     0,     0,   327,   328,   329,   330,   331,   332,   333
  };

  const short int
  feiparser::yycheck_[] =
  {
       8,     9,    35,    36,    37,    38,    39,    40,    56,   116,
      43,    66,    20,    21,    22,    55,     0,    55,    56,   271,
      35,    36,    37,    38,    39,    40,   467,    35,    43,    35,
      36,    37,    38,    39,    40,    43,   467,    43,    35,    36,
      37,    38,    39,    40,   130,   467,    43,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     3,    35,    36,
      37,    38,    39,    40,   103,    73,    43,    51,    53,     3,
       4,     5,     6,   111,     8,     9,    35,    36,    37,    38,
      39,    40,    90,    51,    43,    39,    40,   173,   100,    43,
     176,   177,   470,    27,    28,    29,    30,    31,    32,    33,
     100,   113,    36,   459,   111,   113,   358,    40,    42,   148,
      43,   197,   119,   113,   122,   123,   124,   125,   126,   127,
     128,    55,    35,    36,    37,    38,    39,    40,    92,    93,
      43,    35,    36,    37,    38,    39,    40,   145,   146,    43,
     100,   149,   150,    55,   152,   153,   154,   155,   156,    55,
     205,   159,   160,   113,   162,    89,   468,   469,   166,   167,
     168,   169,   170,   171,    55,    56,   100,    35,    36,    37,
      38,    39,    40,   184,   185,    43,    35,    36,    37,    38,
      39,    40,    98,   175,    43,   101,   178,   144,   145,   105,
     198,     0,   165,   166,     3,     4,     5,     6,    72,     8,
       9,    10,   165,   166,    13,   128,   131,    16,   216,    18,
     459,    20,   139,   140,   141,   466,   143,   144,    27,    28,
      29,    30,    31,    32,    33,   233,   473,    36,   256,   257,
     258,     3,   260,    42,   116,    44,    45,    46,    47,   165,
     166,    50,    43,    52,     5,    54,    55,    35,    36,    37,
      38,    39,    40,   116,   116,    43,   435,   436,   437,   267,
     116,   269,    71,   116,   272,   273,   274,   116,   276,    78,
      79,    80,    81,    35,    36,    37,    38,    39,    40,   116,
     116,    43,   454,   291,   131,   116,   116,    96,    97,     3,
       4,     5,     6,   191,     8,     9,    37,    38,    39,    40,
     109,   116,    43,   464,   118,   116,   115,   454,   116,   116,
     116,   116,   116,    27,    28,    29,    30,    31,    32,    33,
     136,    86,    36,    83,   127,   127,    95,   172,    42,    35,
      36,    37,    38,    39,    40,    88,   344,    43,    89,   102,
      73,    55,    56,   351,    35,    36,    37,    38,    39,    40,
     132,   459,    43,    12,   113,    65,    55,   365,   366,   367,
     116,   129,    55,   470,     3,     4,     5,     6,   376,     8,
       9,    55,   100,   381,    56,   383,   412,   113,   113,   387,
     137,    90,   390,   100,    82,     5,    43,   475,    27,    28,
      29,    30,    31,    32,    33,   474,   118,    36,   116,   472,
      34,   116,   118,    42,   116,    35,    36,    37,    38,    39,
      40,   459,   116,    43,   224,   455,    55,    55,   426,   427,
      82,   429,   417,   431,   432,   117,   174,   117,   175,   437,
     438,   104,   467,    55,    55,    56,   444,   445,    59,   111,
      61,   474,   116,    55,    65,   453,    34,    55,    58,     3,
     119,   459,    34,    74,    34,   463,   116,    84,   473,    35,
      36,    37,    38,    39,    40,   471,    58,    43,   232,    58,
      34,   468,   111,    35,    36,    37,    38,    39,    40,   184,
     457,    43,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   179,   113,    35,    36,    37,    38,    39,    40,   468,
     469,    43,    82,    82,    82,   523,   524,   525,   526,   113,
     113,   100,   530,   531,   294,   364,   534,   113,   113,   113,
     113,   121,   113,   467,   542,   543,   113,   295,   546,   452,
     548,   549,   550,   362,   552,   553,   113,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   184,   468,   469,   113,     3,     4,     5,
       6,   113,     8,     9,    35,    36,    37,    38,    39,    40,
     113,   294,    43,   113,   113,   113,   113,   113,   596,   597,
     598,    27,    28,    29,    30,    31,    32,    33,   606,   113,
      36,   469,    57,   611,   116,   294,    42,   294,   294,   294,
     469,   294,   362,   294,   294,   623,   116,   294,   294,    55,
      35,    36,    37,    38,    39,    40,   294,   294,    43,    35,
      36,    37,    38,    39,    40,   116,   294,    43,    35,    36,
      37,    38,    39,    40,   294,   116,    43,   116,   113,   113,
     459,   113,    34,   462,   463,   295,   465,   116,   467,   295,
     267,   266,   116,   116,   473,    35,    36,    37,    38,    39,
      40,   413,   116,    43,   175,   186,   186,   685,   186,   184,
     103,   469,   116,   103,   121,   134,   121,   695,   121,   697,
     121,   699,   700,   701,   121,   703,   121,   121,    34,   707,
     121,   709,   710,   711,   121,   713,   714,   469,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   750,   121,   996,   121,    35,    36,    37,    38,
      39,    40,   760,   467,    43,   121,   121,   121,   121,   121,
     768,   769,   121,   469,   772,   773,   774,   121,   776,   121,
     121,   121,   121,    34,   782,    34,   784,    34,   469,    35,
      36,    37,    38,    39,    40,    34,    34,    43,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,    34,    34,    43,    34,    34,    34,    34,    34,
      34,    34,   124,   831,   832,   833,    34,    34,   467,   837,
     838,   839,   438,   841,   842,   843,   438,   845,   846,   469,
     438,   849,    35,    36,    37,    38,    39,    40,   856,    34,
      43,    34,    34,    34,    34,   863,   184,   865,    34,   867,
     868,   869,   117,   871,   872,   117,   117,   186,    34,   134,
     467,   879,   880,   881,   882,   144,   467,   119,    34,   887,
     888,   889,    35,    36,    37,    38,    39,    40,    34,   467,
      43,    34,   467,   469,    35,    36,    37,    38,    39,    40,
      34,   467,    43,   911,   912,   913,   467,   469,   467,   467,
     467,   467,   467,   921,   467,   467,   467,   467,   467,   927,
     119,   467,   467,   467,   467,   933,   934,   935,    35,    36,
      37,    38,    39,    40,    34,   119,    43,   469,    58,    58,
     186,    58,   950,    35,    36,    37,    38,    39,    40,   467,
     467,    43,   117,   188,   188,   188,   188,   143,   966,   967,
     968,   969,   970,   971,   972,   973,   974,   975,   976,   977,
     978,   979,   980,   981,   982,   983,    35,    36,    37,    38,
      39,    40,   117,   991,    43,   103,   119,   455,    34,   997,
     998,   119,  1000,  1001,  1002,    67,   457,   137,   456,    34,
    1008,   117,  1010,   116,  1012,  1013,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,   116,  1023,  1024,  1025,  1026,  1027,
    1028,   116,  1030,    34,  1032,  1033,  1034,  1035,  1036,  1037,
      58,   467,    35,    36,    37,    38,    39,    40,   190,   190,
      43,    35,    36,    37,    38,    39,    40,    34,  1056,    43,
    1058,  1059,  1060,   190,   469,  1063,  1064,    34,   190,    34,
      34,    34,    34,   469,    34,  1073,    35,    36,    37,    38,
      39,    40,   469,    34,    43,    34,    34,    34,   122,    35,
      36,    37,    38,    39,    40,    34,    34,    43,    34,    34,
      34,  1099,    35,    36,    37,    38,    39,    40,    34,   469,
      43,    35,    36,    37,    38,    39,    40,    34,    34,    43,
      35,    36,    37,    38,    39,    40,   122,   122,    43,   117,
      35,    36,    37,    38,    39,    40,   117,   110,    43,    34,
    1138,  1139,  1140,   112,    55,    77,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,    35,    36,    37,    38,
      39,    40,    58,    34,    43,    34,    65,    34,    34,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,
    1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,   116,  1187,
     469,    34,  1190,    58,   116,    34,  1194,  1195,   249,  1197,
    1198,   249,   249,   249,    34,  1203,  1204,  1205,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    77,    77,  1217,
    1218,  1219,  1220,   469,   121,  1223,   116,    34,  1226,  1227,
      34,    34,   469,    34,    55,    35,    36,    37,    38,    39,
      40,   469,    19,    43,    21,    22,    23,    24,    25,    26,
     469,    34,  1250,  1251,   116,   116,  1254,    34,    34,  1257,
     116,   113,  1260,   193,   113,   113,   113,   113,   467,  1267,
    1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,
     116,   116,   116,    34,    34,    34,   469,    34,  1286,   458,
      34,  1289,  1290,  1291,  1292,    35,    36,    37,    38,    39,
      40,   113,   196,    43,   121,    35,    36,    37,    38,    39,
      40,   121,  1310,    43,    35,    36,    37,    38,    39,    40,
     233,  1319,    43,  1321,  1322,  1323,   469,  1325,   121,   121,
     233,    35,    36,    37,    38,    39,    40,   117,   469,    43,
     233,   194,   240,  1341,  1342,   233,  1344,    34,    34,    34,
      34,    34,    34,    34,  1352,  1353,  1354,  1355,  1356,  1357,
    1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,
      34,   453,   469,    34,  1372,    34,  1374,  1375,  1376,    34,
      34,    34,    34,    34,  1382,    34,  1384,  1385,  1386,  1387,
    1388,  1389,  1390,  1391,  1392,  1393,    34,  1395,  1396,  1397,
    1398,    34,    34,  1401,    34,    34,  1404,  1405,  1406,  1407,
    1408,    35,    36,    37,    38,    39,    40,    34,    34,    43,
     469,    35,    36,    37,    38,    39,    40,    34,  1426,    43,
    1428,  1429,     5,    34,     5,    34,   467,  1435,  1436,   467,
    1438,    35,    36,    37,    38,    39,    40,   467,   467,    43,
      34,  1449,    58,    35,    36,    37,    38,    39,    40,    34,
    1458,    43,     5,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,    34,    34,    34,   469,    34,    34,    34,
      34,    34,  1480,    34,   421,   469,    34,    34,   119,   117,
     421,   116,   451,   434,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,   450,    34,    34,    34,    34,  1507,
    1508,  1509,   117,    58,    34,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1529,  1530,  1531,    34,  1533,   469,  1535,    34,    34,
    1538,  1539,  1540,  1541,    34,   469,   456,    35,    36,    37,
      38,    39,    40,    34,   469,    43,  1554,  1555,  1556,    34,
      34,    34,  1560,  1561,   469,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,   467,    35,    36,    37,    38,
      39,    40,    34,  1581,    43,  1583,    34,  1585,  1586,    34,
     469,    34,    34,  1591,    34,  1593,  1594,    34,  1596,  1597,
    1598,    34,  1600,  1601,  1602,    35,    36,    37,    38,    39,
      40,  1609,  1610,    43,    67,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,   467,    34,    34,  1626,    34,
      34,   117,  1630,  1631,   117,    35,    36,    37,    38,    39,
      40,   117,   469,    43,    35,    36,    37,    38,    39,    40,
     116,    34,    43,    34,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,   469,
    1668,    58,    34,    34,    34,  1673,    34,  1675,  1676,    34,
    1678,  1679,  1680,   467,  1682,    34,  1684,  1685,  1686,  1687,
    1688,  1689,   116,  1691,  1692,    35,    36,    37,    38,    39,
      40,    58,    58,    43,    58,    34,     5,   116,   434,    34,
    1708,    34,  1710,  1711,  1712,  1713,    34,  1715,    59,   116,
    1718,   116,   116,    34,  1722,    34,    34,    34,    34,   469,
      34,    34,    34,  1731,    34,   467,    34,    34,    34,   469,
      34,    82,    34,    34,    85,  1743,    87,  1745,   469,    34,
      91,   421,  1750,    94,  1752,  1753,    34,  1755,  1756,  1757,
      34,  1759,  1760,  1761,    34,   469,    34,    34,    34,    34,
    1768,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,
    1778,    34,  1780,    34,   434,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,    34,    34,    34,    34,  1797,
    1798,  1799,    34,  1801,    35,    36,    37,    38,    39,    40,
      34,    34,    43,    34,    34,    34,  1814,    34,   113,  1817,
    1818,    34,    34,    34,  1822,  1823,  1824,  1825,  1826,  1827,
    1828,  1829,  1830,    34,    34,   449,    34,  1835,    34,  1837,
    1838,  1839,    34,    34,    34,   469,    35,    36,    37,    38,
      39,    40,  1850,    34,    43,  1853,  1854,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,    34,    34,    43,   469,  1874,  1875,  1876,  1877,
    1878,  1879,  1880,  1881,    34,  1883,    34,   469,    34,    34,
      34,  1889,  1890,    34,  1892,  1893,  1894,   469,  1896,  1897,
      34,  1899,  1900,  1901,  1902,    34,    34,  1905,  1906,   467,
     117,    35,    36,    37,    38,    39,    40,    34,  1916,    43,
    1918,   467,   117,  1921,   117,    34,    34,  1925,    34,   270,
    1928,   117,    34,    34,    34,  1933,  1934,    35,    36,    37,
      38,    39,    40,    34,    34,    43,  1944,    34,  1946,    34,
    1948,  1949,    34,  1951,  1952,  1953,    34,  1955,    34,    34,
      34,    58,    58,  1961,    58,    34,  1964,  1965,  1966,  1967,
    1968,  1969,  1970,  1971,    58,    35,    36,    37,    38,    39,
      40,   469,    34,    43,    34,  1983,    34,    34,    34,   116,
    1988,   116,    34,   116,   116,   467,    34,    34,    34,   469,
    1998,  1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,
     469,    34,  2010,  2011,  2012,    34,    34,  2015,  2016,  2017,
     113,    35,    36,    37,    38,    39,    40,    34,    34,    43,
      34,    34,    34,  2031,  2032,    34,    34,    34,  2036,   469,
      34,   415,  2040,  2041,  2042,  2043,  2044,  2045,  2046,   469,
    2048,  2049,  2050,    34,  2052,    34,   415,  2055,  2056,  2057,
      34,   467,  2060,  2061,    34,    34,   117,   247,    34,   469,
      34,  2069,    34,    34,    34,  2073,    34,  2075,   469,    34,
     113,   467,    58,  2081,    34,    34,    34,    34,    34,  2087,
    2088,  2089,  2090,    34,  2092,  2093,  2094,    34,  2096,   116,
     467,  2099,  2100,  2101,   467,  2103,  2104,  2105,  2106,  2107,
      35,    36,    37,    38,    39,    40,    34,   467,    43,    34,
    2118,   247,    35,    36,    37,    38,    39,    40,    34,   469,
      43,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2136,  2137,
    2138,    34,  2140,    34,    34,    34,  2144,  2145,  2146,    35,
      36,    37,    38,    39,    40,    34,    34,    43,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    34,    34,    34,
    2168,  2169,  2170,  2171,  2172,  2173,  2174,   467,  2176,  2177,
    2178,   467,  2180,   467,  2182,   467,    34,    34,    34,  2187,
    2188,    35,    36,    37,    38,    39,    40,    34,  2196,    43,
      34,    34,  2200,   248,    34,    34,  2204,  2205,  2206,  2207,
      34,    34,    34,    34,    34,  2213,  2214,  2215,  2216,   469,
    2218,  2219,  2220,   467,  2222,  2223,  2224,   247,    34,    34,
      34,  2229,  2230,  2231,  2232,  2233,   247,   247,   469,    35,
      36,    37,    38,    39,    40,    34,    34,    43,  2246,  2247,
    2248,    34,   248,    34,    34,    34,    34,  2255,   467,  2257,
    2258,  2259,   467,  2261,   467,  2263,  2264,  2265,   467,    34,
      34,    34,  2270,    34,  2272,    34,    34,   117,   117,   117,
     469,    34,    34,    34,    58,    58,    58,    34,  2286,  2287,
    2288,   469,    34,    34,  2292,  2293,  2294,  2295,  2296,  2297,
     469,   116,  2300,   116,   116,  2303,  2304,    34,   248,   248,
     248,    34,  2310,    35,    36,    37,    38,    39,    40,  2317,
    2318,    43,  2320,  2321,    34,    34,    34,    34,    34,  2327,
    2328,    34,    34,    34,   467,   467,  2334,  2335,    34,    34,
    2338,   117,    34,  2341,    34,   469,   467,    58,   446,    34,
    2348,  2349,  2350,  2351,   414,    35,    36,    37,    38,    39,
      40,   116,    34,    43,    34,    34,    34,    34,    34,    34,
    2368,  2369,  2370,  2371,   117,  2373,  2374,  2375,  2376,  2377,
    2378,  2379,  2380,    35,    36,    37,    38,    39,    40,    34,
      34,    43,   117,    34,    58,    58,    34,   116,   116,  2397,
    2398,  2399,  2400,  2401,    34,    34,  2404,  2405,  2406,  2407,
      34,  2409,    34,    34,    34,    34,    58,  2415,  2416,   117,
     117,    58,    34,  2421,    34,   116,    34,  2425,  2426,   116,
      34,  2429,  2430,  2431,  2432,    34,  2434,  2435,  2436,    35,
      36,    37,    38,    39,    40,    34,    34,    43,    34,    34,
    2448,  2449,  2450,  2451,   468,    34,    34,  2455,  2456,    34,
      34,    34,    34,    34,    34,    34,    34,  2465,  2466,  2467,
      34,    34,  2470,    34,    34,    34,    34,    34,    34,  2477,
      -1,  2479,   203,    -1,    -1,  2483,    -1,    -1,  2486,    -1,
      -1,  2489,  2490,    -1,  2492,    -1,    -1,    -1,  2496,    -1,
      -1,  2499,    -1,  2501,  2502,  2503,    -1,    -1,    35,    36,
      37,    38,    39,    40,  2512,  2513,    43,    -1,    -1,    -1,
    2518,    -1,    -1,    -1,  2522,    -1,  2524,    -1,    -1,    -1,
      -1,    -1,   445,    -1,    -1,  2533,  2534,  2535,    -1,  2537,
      -1,  2539,    -1,   468,    -1,  2543,    -1,  2545,    35,    36,
      37,    38,    39,    40,  2552,  2553,    43,    -1,  2556,    -1,
      -1,  2559,  2560,    35,    36,    37,    38,    39,    40,  2567,
    2568,    43,    -1,  2571,    -1,    -1,  2574,  2575,    -1,    -1,
      -1,    -1,   468,    -1,  2582,  2583,  2584,    -1,    -1,    -1,
      -1,   468,  2590,  2591,    -1,    -1,    -1,    -1,  2596,  2597,
      -1,    -1,    -1,    -1,  2602,  2603,    -1,    -1,    -1,    -1,
    2608,  2609,    -1,    -1,     1,  2613,     3,     4,     5,     6,
      -1,     8,     9,    10,   468,    -1,    13,    -1,    -1,    16,
      -1,    18,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    31,    32,    33,    -1,    -1,    36,
      -1,    -1,   448,    -1,    -1,    42,    -1,    44,    45,    46,
      47,    -1,    -1,    50,    -1,    52,    -1,    54,    55,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,     8,     9,    10,
      -1,    -1,    13,    -1,    71,    16,    -1,    18,    -1,    20,
      -1,    78,    79,    80,    81,    -1,    27,    28,    29,    30,
      31,    32,    33,    -1,    -1,    36,    -1,    -1,    -1,    96,
      97,    42,    -1,    44,    45,    46,    47,    -1,    -1,    50,
      -1,    52,   109,    54,    55,    -1,    -1,    -1,   115,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,   468,    78,    79,    80,
      81,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,     8,     9,    10,   109,    -1,
      13,    -1,    -1,    16,   115,    18,    -1,    20,   468,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    44,    45,    46,    47,    -1,   468,    50,    -1,    52,
      -1,    54,    55,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    71,    -1,
      43,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    96,    97,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,   115,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   468,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   468,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   468,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   459,    -1,    -1,   462,   463,    -1,   465,    -1,
     467,    -1,    -1,    -1,    -1,    -1,   473,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   459,    -1,
      -1,   462,   463,    -1,   465,    -1,   467,    -1,    -1,    -1,
      -1,    -1,   473,    -1,   475,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   468,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   459,    -1,    -1,   462,
     463,    -1,   465,    -1,   467,    -1,    -1,    -1,    -1,    -1,
     473,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,   468,    -1,    43,    35,
      36,    37,    38,    39,    40,   468,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   468,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,   468,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,   468,    -1,    43,    35,
      36,    37,    38,    39,    40,   468,    -1,    43,    35,    36,
      37,    38,    39,    40,   468,    -1,    43,    35,    36,    37,
      38,    39,    40,   468,    -1,    43,    35,    36,    37,    38,
      39,    40,   468,    -1,    43,    35,    36,    37,    38,    39,
      40,   468,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   468,    -1,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   468,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     468,    -1,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   468,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   468,    -1,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   468,    -1,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     468,    -1,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   468,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,   468,    -1,    43,    35,    36,
      37,    38,    39,    40,   468,    -1,    43,    35,    36,    37,
      38,    39,    40,   468,    -1,    43,    35,    36,    37,    38,
      39,    40,   468,    -1,    43,    35,    36,    37,    38,    39,
      40,   468,    -1,    43,    35,    36,    37,    38,    39,    40,
     468,    -1,    43,    35,    36,    37,    38,    39,    40,   468,
      -1,    43,    35,    36,    37,    38,    39,    40,   468,    -1,
      43,    35,    36,    37,    38,    39,    40,   468,    -1,    43,
      35,    36,    37,    38,    39,    40,   468,    -1,    43,    35,
      36,    37,    38,    39,    40,   468,    -1,    43,    35,    36,
      37,    38,    39,    40,   468,    -1,    43,    35,    36,    37,
      38,    39,    40,   468,    -1,    43,    35,    36,    37,    38,
      39,    40,   468,    -1,    43,    35,    36,    37,    38,    39,
      40,   468,    -1,    43,    35,    36,    37,    38,    39,    40,
     468,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   468,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   468,
      -1,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   468,    -1,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   468,    -1,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   468,    -1,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   468,    -1,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     468,    -1,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   468,
      -1,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   468,    -1,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   468,    -1,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   468,    -1,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   468,    -1,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     468,    -1,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   468,    -1,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   468,
      -1,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   468,    -1,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    -1,    -1,    -1,    -1,    -1,
      55,    56,   468,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,   468,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,   468,    -1,    35,    36,    37,    38,
      39,    40,    -1,   468,    43,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    35,    36,    37,    38,    39,    40,
      -1,   468,    43,    35,    36,    37,    38,    39,    40,    -1,
     468,    43,    35,    36,    37,    38,    39,    40,    -1,   468,
      43,    35,    36,    37,    38,    39,    40,    -1,   468,    43,
      35,    36,    37,    38,    39,    40,    -1,   468,    43,    35,
      36,    37,    38,    39,    40,    -1,   468,    43,    35,    36,
      37,    38,    39,    40,    -1,   468,    43,    35,    36,    37,
      38,    39,    40,    -1,   468,    43,    -1,   192,    -1,    -1,
      -1,    -1,    -1,   468,    -1,    35,    36,    37,    38,    39,
      40,    -1,   468,    43,    -1,    -1,   444,    -1,    -1,    -1,
      -1,   468,    -1,    -1,   443,    -1,    -1,    -1,    -1,    -1,
     468,    -1,    -1,   443,    35,    36,    37,    38,    39,    40,
      -1,   442,    43,    35,    36,    37,    38,    39,    40,    -1,
     442,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   442,    -1,   259,    -1,    -1,    -1,    -1,    -1,
     441,    -1,    35,    36,    37,    38,    39,    40,    -1,   441,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   441,    -1,    35,    36,    37,    38,    39,    40,    -1,
     441,    43,    35,    36,    37,    38,    39,    40,   440,    -1,
      43,    35,    36,    37,    38,    39,    40,   440,    -1,    43,
      35,    36,    37,    38,    39,    40,   440,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   439,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   439,    -1,    35,    36,    37,
      38,    39,    40,    -1,   439,    43,    35,    36,    37,    38,
      39,    40,    -1,   439,    43,    -1,    -1,   419,    -1,    -1,
      -1,    -1,   439,   416,    -1,    -1,    -1,   411,    -1,    35,
      36,    37,    38,    39,    40,    -1,   411,    43,    35,    36,
      37,    38,    39,    40,   410,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   409,    35,    36,    37,    38,    39,    40,    -1,
     408,    43,    35,    36,    37,    38,    39,    40,   407,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   447,    -1,    -1,    -1,    -1,   406,    35,    36,
      37,    38,    39,    40,    -1,   405,    43,    35,    36,    37,
      38,    39,    40,   404,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,   403,    43,    35,    36,    37,    38,    39,
      40,   402,    -1,    43,    -1,    -1,    -1,    -1,    -1,   401,
      35,    36,    37,    38,    39,    40,    -1,   400,    43,    35,
      36,    37,    38,    39,    40,   399,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   398,    35,    36,    37,    38,    39,    40,
      -1,   397,    43,    35,    36,    37,    38,    39,    40,   396,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   395,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,   394,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   393,    43,    35,    36,    37,    38,    39,    40,
     392,    -1,    43,    -1,    -1,    -1,    -1,    -1,   391,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   390,    35,    36,
      37,    38,    39,    40,    -1,   389,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   388,    35,    36,    37,
      38,    39,    40,    -1,   387,    43,    35,    36,    37,    38,
      39,    40,   386,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     385,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   384,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   383,    35,    36,    37,    38,
      39,    40,    -1,   382,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   381,    35,    36,    37,    38,
      39,    40,    -1,   380,    43,    -1,    -1,   369,    35,    36,
      37,    38,    39,    40,    -1,   368,    43,    35,    36,    37,
      38,    39,    40,   367,    -1,    43,    -1,    -1,    -1,    -1,
      -1,   366,    35,    36,    37,    38,    39,    40,    -1,   365,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,   363,    -1,    43,    35,
      36,    37,    38,    39,    40,   363,    -1,    43,   357,    35,
      36,    37,    38,    39,    40,    -1,   356,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     355,    35,    36,    37,    38,    39,    40,    -1,   354,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   353,    35,    36,    37,    38,    39,    40,    -1,
     352,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   351,    35,    36,    37,    38,    39,
      40,    -1,   350,    43,    35,    36,    37,    38,    39,    40,
     349,    -1,    43,    -1,    -1,    -1,    -1,    -1,   348,    -1,
      35,    36,    37,    38,    39,    40,    -1,   347,    43,    35,
      36,    37,    38,    39,    40,   346,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   345,
      35,    36,    37,    38,    39,    40,    -1,   344,    43,    35,
      36,    37,    38,    39,    40,    -1,   343,    43,    35,    36,
      37,    38,    39,    40,   342,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,   342,    43,    35,    36,    37,    38,
      39,    40,   341,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   339,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   338,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   337,    35,
      36,    37,    38,    39,    40,    -1,   336,    43,    35,    36,
      37,    38,    39,    40,   335,    -1,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   334,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   333,    35,    36,    37,
      38,    39,    40,    -1,   332,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   331,    35,
      36,    37,    38,    39,    40,    -1,   330,    43,    35,    36,
      37,    38,    39,    40,   329,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   328,    -1,    35,    36,    37,    38,    39,    40,
      -1,   327,    43,    35,    36,    37,    38,    39,    40,   326,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   325,    35,    36,    37,    38,    39,    40,    -1,   324,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   323,    35,    36,    37,    38,    39,    40,    -1,   322,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   321,    35,    36,    37,    38,    39,    40,    -1,   320,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   319,    35,    36,    37,    38,    39,
      40,    -1,   318,    43,    35,    36,    37,    38,    39,    40,
     317,    -1,    43,    -1,    -1,    -1,    -1,    -1,   313,    -1,
      35,    36,    37,    38,    39,    40,    -1,   313,    43,    35,
      36,    37,    38,    39,    40,   312,    -1,    43,    35,    36,
      37,    38,    39,    40,   312,    -1,    43,    -1,    -1,    -1,
      -1,   310,    35,    36,    37,    38,    39,    40,    -1,   309,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   308,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   307,    -1,    35,    36,    37,    38,    39,    40,    -1,
     306,    43,    35,    36,    37,    38,    39,    40,   305,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,   305,    43,
      35,    36,    37,    38,    39,    40,    -1,   305,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   305,    43,    35,
      36,    37,    38,    39,    40,   304,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   304,    -1,
      35,    36,    37,    38,    39,    40,    -1,   304,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   304,    -1,    35,    36,    37,    38,    39,
      40,    -1,   304,    43,    35,    36,    37,    38,    39,    40,
      -1,   304,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   304,    43,    35,    36,    37,    38,    39,    40,   303,
      -1,    43,    -1,    35,    36,    37,    38,    39,    40,    -1,
     303,    43,    35,    36,    37,    38,    39,    40,    -1,   303,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     303,    -1,    35,    36,    37,    38,    39,    40,    -1,   303,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,   303,    -1,    35,    36,    37,    38,    39,
      40,    -1,   303,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   303,    35,
      36,    37,    38,    39,    40,    -1,   302,    43,    35,    36,
      37,    38,    39,    40,    -1,   302,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   302,
      -1,    35,    36,    37,    38,    39,    40,    -1,   301,    43,
      35,    36,    37,    38,    39,    40,    -1,   301,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   301,
      35,    36,    37,    38,    39,    40,    -1,   300,    43,    35,
      36,    37,    38,    39,    40,    -1,   300,    43,    35,    36,
      37,    38,    39,    40,    -1,   300,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   300,    43,    35,    36,    37,
      38,    39,    40,    -1,   300,    43,    -1,    -1,    -1,    -1,
      -1,   298,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   298,    35,    36,    37,    38,    39,    40,
      -1,   297,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   297,    -1,    35,
      36,    37,    38,    39,    40,    -1,   297,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   297,    43,    35,    36,
      37,    38,    39,    40,    -1,   297,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   297,    43,    35,    36,    37,
      38,    39,    40,    -1,   297,    43,    35,    36,    37,    38,
      39,    40,    -1,   297,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   297,    -1,    35,    36,    37,    38,
      39,    40,    -1,   297,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   297,    -1,    35,
      36,    37,    38,    39,    40,    -1,   297,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   297,    35,    36,    37,    38,    39,    40,    -1,   296,
      43,    35,    36,    37,    38,    39,    40,   295,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   295,    -1,    35,    36,    37,    38,    39,    40,    -1,
     295,    43,    35,    36,    37,    38,    39,    40,    -1,   295,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     295,    -1,    35,    36,    37,    38,    39,    40,    -1,   295,
      43,    35,    36,    37,    38,    39,    40,    -1,   295,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   295,    43,
      35,    36,    37,    38,    39,    40,    -1,   295,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   295,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   295,    -1,    35,    36,    37,    38,
      39,    40,    -1,   295,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   295,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   295,
      -1,    35,    36,    37,    38,    39,    40,    -1,   295,    43,
      35,    36,    37,    38,    39,    40,    -1,   294,    43,    35,
      36,    37,    38,    39,    40,    -1,   294,    43,    35,    36,
      37,    38,    39,    40,    -1,   294,    43,    35,    36,    37,
      38,    39,    40,    -1,   294,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   294,    -1,    -1,    -1,    -1,
      -1,    -1,   269,    -1,   294,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   267,    -1,    -1,    -1,   294,    -1,
      -1,    -1,    -1,   267,    -1,    35,    36,    37,    38,    39,
      40,    -1,   267,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   267,    -1,    35,    36,    37,
      38,    39,    40,    -1,   267,    43,    -1,    -1,    -1,    -1,
      -1,   265,    -1,    -1,    -1,    -1,   250,    -1,    -1,   262,
      35,    36,    37,    38,    39,    40,   250,    -1,    43,    -1,
      -1,   246,    -1,    35,    36,    37,    38,    39,    40,    -1,
     246,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,   246,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   246,    -1,    35,
      36,    37,    38,    39,    40,    -1,   246,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   244,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
     244,    35,    36,    37,    38,    39,    40,    -1,   243,    43,
      35,    36,    37,    38,    39,    40,    -1,   243,    43,    35,
      36,    37,    38,    39,    40,   242,    -1,    43,    35,    36,
      37,    38,    39,    40,   242,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   241,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   241,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,   238,    43,
      35,    36,    37,    38,    39,    40,   237,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   237,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   237,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,   237,    35,    36,    37,    38,
      39,    40,    -1,   236,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   236,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
     236,    -1,    35,    36,    37,    38,    39,    40,    -1,   236,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     235,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   235,    -1,    35,    36,    37,    38,    39,    40,    -1,
     235,    43,    35,    36,    37,    38,    39,    40,    -1,   235,
      43,    35,    36,    37,    38,    39,    40,   234,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   234,    -1,    35,    36,    37,    38,    39,    40,
      -1,   234,    43,    -1,    35,    36,    37,    38,    39,    40,
      -1,   234,    43,    35,    36,    37,    38,    39,    40,   233,
      -1,    43,    35,    36,    37,    38,    39,    40,   233,    -1,
      43,    -1,    -1,    -1,    -1,   231,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   231,    -1,    35,    36,
      37,    38,    39,    40,    -1,   231,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   231,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   231,    -1,    35,    36,    37,    38,    39,    40,
      -1,   231,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   230,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   230,    -1,    35,
      36,    37,    38,    39,    40,    -1,   230,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   230,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   230,    -1,
      35,    36,    37,    38,    39,    40,    -1,   230,    43,    35,
      36,    37,    38,    39,    40,   229,    -1,    43,    35,    36,
      37,    38,    39,    40,   229,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   229,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   229,    -1,
      35,    36,    37,    38,    39,    40,    -1,   229,    43,    35,
      36,    37,    38,    39,    40,    -1,   229,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   228,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   228,    -1,    35,    36,    37,    38,    39,    40,    -1,
     228,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   228,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   228,    -1,    35,
      36,    37,    38,    39,    40,    -1,   228,    43,    35,    36,
      37,    38,    39,    40,    -1,   227,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,   227,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   227,    -1,    35,    36,    37,    38,    39,    40,    -1,
     227,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   227,    -1,    35,    36,    37,    38,    39,    40,
      -1,   227,    43,    35,    36,    37,    38,    39,    40,   226,
      -1,    43,    35,    36,    37,    38,    39,    40,   226,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     226,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   226,    -1,    35,    36,    37,    38,    39,    40,    -1,
     226,    43,    35,    36,    37,    38,    39,    40,    -1,   226,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     225,    -1,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   225,    -1,    35,    36,    37,    38,    39,
      40,    -1,   225,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   225,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   225,
      -1,    35,    36,    37,    38,    39,    40,    -1,   225,    43,
      35,    36,    37,    38,    39,    40,   224,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   224,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   224,    -1,    35,    36,    37,    38,    39,    40,
      -1,   224,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   224,    -1,    35,    36,    37,    38,    39,
      40,    -1,   224,    43,    35,    36,    37,    38,    39,    40,
     223,    -1,    43,    35,    36,    37,    38,    39,    40,   223,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,   223,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   223,   195,    35,    36,    37,    38,    39,    40,   189,
     223,    43,    35,    36,    37,    38,    39,    40,   189,   223,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   189,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   183,    -1,
      35,    36,    37,    38,    39,    40,    -1,   183,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   182,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   182,    35,    36,    37,    38,    39,    40,    -1,   181,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   181,    35,    36,    37,    38,    39,    40,    -1,   180,
      43,    35,    36,    37,    38,    39,    40,   170,   180,    43,
      35,    36,    37,    38,    39,    40,   170,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   170,    -1,
      35,    36,    37,    38,    39,    40,    -1,   170,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   170,    43,    35,
      36,    37,    38,    39,    40,   169,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,   169,    43,    35,    36,    37,    38,
      39,    40,    -1,   169,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   169,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   169,    35,    36,    37,
      38,    39,    40,    -1,   168,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   168,    -1,    35,    36,    37,
      38,    39,    40,    -1,   168,    43,    35,    36,    37,    38,
      39,    40,    -1,   168,    43,    35,    36,    37,    38,    39,
      40,    -1,   168,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   167,    43,    -1,    -1,    -1,    -1,   163,    -1,
      35,    36,    37,    38,    39,    40,    -1,   163,    43,    35,
      36,    37,    38,    39,    40,    -1,   163,    43,    35,    36,
      37,    38,    39,    40,    -1,   163,    43,    35,    36,    37,
      38,    39,    40,   162,    -1,    43,    -1,    -1,    -1,    -1,
     160,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     160,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   159,    -1,    35,    36,    37,    38,    39,    40,    -1,
     159,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   159,    35,    36,    37,    38,    39,    40,    -1,   158,
      43,    35,    36,    37,    38,    39,    40,    -1,   158,    43,
     150,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   150,    35,    36,    37,    38,
      39,    40,    -1,   149,    43,    35,    36,    37,    38,    39,
      40,    -1,   149,    43,    35,    36,    37,    38,    39,    40,
     148,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   147,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   147,    -1,   118,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   147,    -1,    -1,    -1,    -1,
      -1,   134,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
     134,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     134,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,   134,    -1,    35,    36,    37,
      38,    39,    40,    -1,   134,    43,    35,    36,    37,    38,
      39,    40,    -1,   134,    43,    35,    36,    37,    38,    39,
      40,   133,    -1,    43,    -1,    35,    36,    37,    38,    39,
      40,    -1,   133,    43,    -1,   124,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
     124,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     124,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   124,    35,    36,    37,
      38,    39,    40,    -1,   123,    43,    -1,    -1,   118,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   118,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   118,
      -1,    35,    36,    37,    38,    39,    40,    -1,   118,    43,
      35,    36,    37,    38,    39,    40,   117,    -1,    43,    35,
      36,    37,    38,    39,    40,   117,    -1,    43,    -1,    -1,
      -1,   114,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   114,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   114,    35,
      36,    37,    38,    39,    40,    -1,   112,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   110,    43,    -1,    -1,
      -1,    -1,    -1,   108,    35,    36,    37,    38,    39,    40,
      -1,   107,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    82,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,    -1,
     216,    -1,   218,   219,   220,   221,   222,   271,   272,   273,
     274,   275,    -1,   277,   278,   279,   280,   281,   282,   283,
      -1,    -1,    -1,   287,   288,   289,   290,   291,   292,   293
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     6,     8,     9,    10,    13,
      16,    18,    20,    27,    28,    29,    30,    31,    32,    33,
      36,    42,    44,    45,    46,    47,    50,    52,    54,    55,
      71,    78,    79,    80,    81,    96,    97,   109,   115,   459,
     462,   463,   465,   467,   473,   477,   478,   479,   482,   483,
     484,   487,   488,   489,    19,    21,    22,    23,    24,    25,
      26,    34,   467,   467,   467,   488,   488,     3,   488,   488,
      55,    56,   488,    53,    51,    55,    56,    58,    59,    61,
      62,    63,    65,    66,    67,    77,   192,   259,   447,   485,
     470,   459,    55,    55,    55,    56,    59,    61,    65,    74,
     184,    59,    82,    85,    87,    91,    94,   270,    89,   100,
     488,    72,   131,   128,    56,   459,   459,   466,   488,   473,
       0,   489,    35,    36,    37,    38,    39,    40,    43,   473,
       3,   488,   488,   488,   488,   488,   488,   488,   481,   488,
     488,   488,   474,   490,   490,   116,   116,   488,     5,   116,
     116,   486,   116,   116,   116,   116,   116,   454,   131,   116,
     116,   191,   116,   118,   488,   464,   116,   116,   116,   116,
     116,   116,   454,   127,   127,   136,    83,    86,    88,    92,
      93,    95,    89,   172,   102,   133,    73,   132,   488,   459,
     469,   488,   488,   488,   488,   488,   488,   488,   468,   469,
     469,   469,   475,   489,   491,    12,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   113,   116,   488,   488,    65,
     488,    55,   471,   129,   488,   488,   488,   488,   488,   488,
      55,    55,    56,   137,   412,   113,   113,   100,    90,   130,
     173,   176,   177,   197,    82,   100,     5,   133,   488,   491,
     475,   490,   110,   134,   134,   118,   118,   118,   118,   134,
      55,   455,   488,   134,   407,   118,   100,   116,   472,   118,
     114,   114,   116,   116,   116,   488,    34,   224,   417,    55,
      82,   117,   174,   175,   175,   178,   117,   104,    98,   101,
     105,   467,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   216,
     218,   219,   220,   221,   222,   271,   272,   273,   274,   275,
     277,   278,   279,   280,   281,   282,   283,   287,   288,   289,
     290,   291,   292,   293,    55,    56,   111,    55,   111,    55,
      56,   435,   436,   437,   116,    55,   162,   256,   257,   258,
     260,    34,    55,    58,   488,     3,   488,   111,   119,   480,
     480,   488,   488,   488,   488,    34,    34,   116,    84,    58,
     232,   184,   184,   185,   179,    58,    34,    82,    82,    82,
     488,   100,   113,   100,   113,   113,   113,   100,   113,   113,
     100,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   294,   294,   294,
     294,   294,   294,   294,   294,   364,   295,   362,   362,   294,
     294,   294,   294,   294,   294,   294,   116,   116,    57,   116,
     121,   116,   116,   113,   113,   113,   488,   116,    34,   267,
     295,   295,   266,   488,   116,   116,   448,   480,   411,   488,
     488,   488,   413,   116,   175,   186,   186,   186,   184,   116,
     488,   103,   103,   468,   488,   121,   488,   121,   121,   121,
     488,   121,   121,   488,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,   111,
     488,   488,   134,   488,   124,   488,   488,   438,   438,   438,
     124,   488,   488,    34,    34,    34,    34,   408,   488,   488,
      34,    34,   416,   124,    34,   488,   184,   117,   117,   117,
     186,   488,    34,    34,   488,   189,   467,   189,   467,   467,
     467,   189,   467,   467,   189,   467,   467,   467,   467,   467,
     467,   467,   467,   467,   467,   467,   467,   467,   467,   467,
     467,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     134,   134,   134,   144,   124,   119,    34,    34,    34,   119,
     112,   147,   488,   488,   488,   488,    34,   457,   118,   488,
     488,    34,   119,   488,   147,   186,    58,    58,    58,   117,
     147,   488,   488,   468,   188,   488,   188,   488,   488,   488,
     188,   488,   488,   188,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     295,   295,   380,   342,   396,   342,   295,   309,   365,   296,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   143,
     139,   140,   141,   143,   144,   144,   145,   117,   119,   103,
     488,   488,   488,   455,   456,    34,   117,   297,   233,   488,
     119,    67,   449,   410,   488,   137,   414,    34,   117,   116,
     116,   116,    58,    34,   107,   488,   190,   468,   190,   468,
     468,   468,   190,   468,   468,   190,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,   122,   122,   122,   117,   117,   110,    77,   103,   148,
      34,   439,   439,   439,   112,    55,   488,    58,    34,    34,
     409,    65,   116,    34,    34,   488,    34,   488,    58,   488,
     488,   488,   116,   488,    34,   469,   249,   488,   249,   488,
     488,   488,   249,   488,   488,   249,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,    34,    34,    34,    77,    77,   121,   116,    34,    34,
     488,    34,    34,    34,    55,   116,   116,   488,   488,    34,
     193,   488,   488,   488,   488,   163,   116,   168,   168,   168,
     488,   163,   488,   113,   113,   468,   113,   468,   468,   468,
     113,   468,   468,   113,   469,   469,   469,   469,   469,   468,
     468,   468,   468,   469,   469,   469,   469,   468,   468,   468,
     297,   297,   381,   339,   397,   343,   297,   310,   366,   363,
     363,   297,   297,   297,   297,   297,   297,   297,   488,   488,
     488,   116,   116,   467,   488,   488,   488,   147,   488,   488,
     488,   116,   488,   488,   267,   236,   488,   458,   262,   450,
     411,    34,   488,    34,    34,    34,   168,    34,   106,   488,
     121,   488,   121,   488,   488,   488,   121,   488,   488,   121,
     113,   196,   233,   233,   233,   488,   488,   488,   488,   117,
     240,   194,   233,   488,   488,   488,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,   103,   163,   488,   488,   488,   148,
     158,    34,   440,   441,   441,   488,   457,    34,    34,   488,
      34,    34,    34,     5,   168,   488,   488,   488,    34,     5,
      34,   114,   467,   468,   467,   468,   468,   468,   467,   468,
     468,   467,   488,    34,    34,    34,    34,   468,   468,   469,
     468,    58,    34,    34,    34,   468,   468,   468,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,    34,    34,   468,    34,
      34,   488,    34,    34,    34,   124,   480,   488,   488,   488,
     488,   488,   421,    34,   169,   169,   169,   488,   421,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     123,   488,   488,   488,   488,   488,   488,   117,   488,   116,
     488,   488,   488,   488,   488,   488,   382,   319,   398,   297,
     401,   306,   367,   439,   444,   298,   298,   300,   300,   300,
     300,   300,   488,     5,   488,   488,   488,   167,   434,   488,
     488,   119,   294,   237,   265,   451,    34,   488,    34,    34,
      34,   169,    34,   108,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   117,   269,   295,   295,   295,   469,
     469,    58,   469,   488,   241,   195,   117,   468,   468,   468,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,   163,   468,   158,   149,
      34,   440,   442,   456,    34,    34,    34,    34,   467,   169,
     488,   488,   488,    34,   467,    34,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,    67,    34,    34,    34,
      34,   117,   117,   116,   117,   233,    34,    34,    58,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,    34,   488,
      34,    34,   488,    34,    34,   488,   488,   467,   488,   488,
      34,   170,   170,   170,   488,   488,   488,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   116,   488,   488,
     488,   488,    58,    58,   488,    58,    34,   488,   488,   116,
     468,   468,   468,   383,   317,   399,   341,   402,   307,   419,
     441,   445,   305,   301,   305,   305,   305,   301,   301,     5,
     468,   488,   488,   159,   434,   488,   234,   488,   452,   468,
     488,    34,    34,    34,   170,   468,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   294,   234,   234,
     234,   116,   116,   267,   116,   488,   242,   241,   488,   488,
     488,   488,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,   421,   488,
     149,   160,    34,   440,    34,   468,    34,   488,   170,   488,
     488,   488,    34,   488,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   294,    34,    34,    34,    34,   488,
     488,    34,   488,   294,    34,    34,   468,   468,   468,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,    34,   468,   469,    34,    34,
     488,    34,   488,   488,   488,   468,    34,   488,   468,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,    34,
     488,   488,   488,   488,   267,   267,   488,   267,    34,   488,
     488,   488,   488,   488,   384,   318,   400,   344,   403,   308,
     368,   442,   446,   304,   302,   304,   304,   304,   302,   302,
     467,   488,   113,   488,   488,   434,   235,   469,   453,   488,
     488,   488,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   488,   246,   235,   235,   235,    34,    34,    34,
     488,   243,   242,   468,   468,   468,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,   488,   468,   165,   166,   160,   150,    34,    34,
     468,   468,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   238,    34,    34,    34,    34,   488,   488,   488,
      34,    34,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   468,   488,   467,   488,    34,    34,   488,   488,   488,
     488,   468,   469,   468,   468,   469,   468,   468,   468,   469,
     468,    34,   467,   488,   488,   488,   488,   488,   469,   468,
     468,   385,   320,   297,   345,   404,   369,   443,   439,   304,
     303,   303,   303,   304,   304,   488,   468,   488,   488,   488,
     468,   468,   488,   117,   488,   488,   117,   488,   488,   488,
     117,   488,   488,   488,   237,   237,   237,   244,   243,   117,
     488,   488,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,   468,   488,   468,   150,
     159,   488,   488,   468,    58,   468,   468,    58,   468,   468,
     468,    58,   468,   246,   468,    34,    34,    34,    34,    34,
      58,   468,   468,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   468,   488,
      34,    34,   468,   468,   488,   116,   488,   488,   116,   488,
     488,   488,   116,   488,    34,   488,   488,   488,   488,   488,
     488,   116,   488,   488,   386,   321,   303,   346,   405,   441,
     313,   303,   303,   468,   488,   468,   488,   488,   488,   488,
     468,   488,   468,   468,   488,   468,   468,   468,   488,   468,
     467,   468,   236,   236,   236,   250,   244,   488,   468,   468,
      34,    34,    34,    34,    34,    34,    34,    34,    34,   488,
     468,   469,   488,   159,   469,   469,   488,   488,   488,   223,
     488,   488,   488,   223,   488,   488,   488,    34,    34,    34,
      34,    34,   223,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   468,   488,   113,   468,    34,   415,
     415,   469,   468,   468,    34,   468,   468,   468,    34,   468,
     468,   469,   488,   488,   488,   467,   488,    34,   468,   468,
     387,   322,   347,   406,   442,   312,   313,   488,   469,   165,
     166,   488,   488,    34,    34,   117,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   247,   294,   294,   294,   488,
     250,   488,   488,   488,    34,    34,    34,    34,    34,    34,
      34,   468,   113,   467,   488,   469,   488,   488,    58,   468,
     468,   224,   468,   468,   468,   224,   468,   468,    34,    34,
      34,    34,   468,    34,   224,   468,   468,   488,   488,   488,
     488,   488,   488,   488,   488,   165,   166,   488,   116,   488,
     488,    34,   488,   488,   488,    34,   488,   488,   467,   488,
     488,   488,   488,   467,    34,   488,   488,   388,   323,   348,
     303,   443,   312,   469,   467,   488,   468,   488,   468,   468,
     488,   468,   468,   468,   488,   468,   469,   488,   246,   246,
     246,   468,   488,   488,   468,   468,    34,    34,    34,    34,
      34,    34,   488,   488,   488,   488,   225,   488,   488,   488,
     225,   488,   247,   468,    34,    34,    34,   488,   468,   225,
     488,   488,   488,   488,   488,   488,   488,   488,   468,   468,
     468,   468,    34,   468,   468,   468,    34,   468,    34,   488,
     467,   467,   467,   469,   488,    34,   468,   468,   389,   324,
     349,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   467,   468,   488,   488,   488,   468,   488,   488,   488,
      34,    34,    34,   468,   468,   468,   468,   226,   468,   468,
     468,   226,   468,   488,   488,   468,   468,   468,   488,   226,
     468,   468,   488,   488,   488,   488,   488,   488,   488,    34,
     488,   488,   488,    34,   488,   468,   469,   488,   488,   488,
     469,    34,   488,   488,   390,   325,   350,   468,   468,   468,
     468,   488,   468,   468,   468,   488,   468,   488,   248,   468,
     468,   468,   488,   468,   468,    34,    34,    34,   488,   488,
     488,   488,   227,   488,   488,   488,   227,   488,   468,    34,
     488,   488,   488,   227,   488,   488,   488,   488,   488,   468,
     468,   468,   468,    34,   468,   468,   468,    34,   468,   488,
     467,   469,   469,   469,    34,   468,   468,   391,   326,   351,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     469,   488,   247,   247,   247,   488,   488,   488,    34,    34,
      34,   468,   468,   468,   468,   228,   468,   468,   468,   228,
     468,   248,   468,    34,    34,    34,   228,   468,   468,   488,
     488,   488,   488,   488,   488,   488,    34,   488,   488,   488,
      34,   488,    34,   488,   467,   467,   467,    34,   488,   488,
     392,   327,   352,   468,   468,   468,   468,   488,   468,   468,
     468,   488,   468,   467,   468,   488,   488,   488,   488,   468,
     468,    34,    34,    34,   488,   488,   488,   488,   229,   488,
     488,   488,   229,   488,   488,   488,   468,   468,   468,   229,
     488,   488,   488,   488,   488,   468,   469,   468,   468,    34,
     469,   468,   469,    34,   468,   468,   469,   488,   488,   488,
      34,   469,   468,   393,   328,   353,   488,   488,   488,   488,
     117,   488,   117,   488,   488,   488,   468,   468,   468,   488,
     117,   488,    34,    34,    34,   468,   468,   468,   230,    58,
     468,    58,   230,   468,   468,   488,   488,   488,   230,    58,
     468,   488,   488,   488,   488,   488,   488,    34,   116,   488,
     116,    34,   488,   488,   469,   469,   469,    34,   116,   488,
     394,   329,   354,   469,   468,   468,   488,   488,   468,   488,
     488,   468,   469,   248,   248,   248,   488,   488,   468,    34,
      34,    34,   488,   488,   231,   223,   488,   231,   488,    34,
      34,    34,   231,   223,   488,   488,   488,   488,   468,   468,
      34,    34,   469,    34,   468,   467,   467,   467,    34,    34,
     468,   395,   330,   355,   488,   488,   488,   488,   117,   488,
     488,   488,   488,   488,   488,   488,   488,    34,    34,    34,
     468,   468,   224,    58,   468,   468,   468,   468,   224,   468,
     488,   488,   488,   488,   488,    34,   116,   488,   488,   488,
     488,    34,   488,   331,   356,   468,   468,   488,   488,   468,
     468,   468,   468,   488,   468,    34,    34,   488,   488,   225,
     488,   488,   488,   488,   225,   488,   488,   488,   468,   468,
      34,   468,   469,   469,   469,    34,   468,   332,   357,   488,
     488,   488,   488,   488,   488,    34,    34,   468,   469,   226,
     468,   226,   469,   488,   488,   488,   117,    34,   488,    34,
     117,   333,   303,   468,    58,   488,   468,   488,    58,    34,
      34,   488,   116,   227,   488,   227,   116,   488,   488,   468,
     488,    34,   468,    34,   488,   334,    82,   488,   488,   488,
     488,   223,    34,    34,   469,   228,   469,   228,    34,   488,
     488,   117,    34,   117,    34,   488,   335,   180,    58,   488,
      58,   488,   224,    34,    34,   116,   229,   116,   229,    34,
     488,   488,   488,    34,   488,    34,   488,   336,   181,   488,
     488,   225,    34,    34,   230,   230,    34,   488,   488,    34,
      34,   488,   337,   182,   488,   488,   226,    34,    34,   231,
     231,    34,   488,   488,    34,    34,   488,   338,   183,   488,
     488,   227,    34,    34,    34,   488,   488,   488,    82,   228,
      34,    34,   488,   488,   180,   229,    34,    34,   488,   488,
     181,   230,    34,    34,   488,   488,   182,   231,    34,    34,
     488,   488,   183,    34,   488
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   476,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   479,   479,   479,   479,   480,   480,   481,   481,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   483,   483,   483,   483,   483,   483,   483,   483,
     483,   483,   483,   483,   483,   483,   483,   483,   484,   484,
     484,   484,   484,   484,   484,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   487,   487,   487,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   490,   490,   491,   491
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
      32,    29,    35,    29,    35,    95,    80,    23,    11,    26,
      35,    26,    21,    14,    56,    27,    18,    33,    65,    71,
      71,    51,    57,    54,    54,    54,    78,    78,    92,    47,
      59,    59,    51,    59,    35,    38,    22,    22,    20,    22,
      21,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     3,     3,     4,     4,     1,     4,     1,     1,
       1,     1,     1,     1,     1,     6,     1,     2,     2,     3,
       5,     3,     1,     1,     2,     2,     3,     1,     2
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
  "DruckerPragerArmstrongFrederickLT",
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
  "RMC_qa", "RMC_pc", "RMC_e", "RMC_eta0", "RMC_Heta", "ax", "ay", "az",
  "verbose_level", "maximum_iterations", "tolerance",
  "yield_function_relative_tolerance", "stress_relative_tolerance",
  "verbose_output", "beta", "gamma", "kappa", "lambda", "delta", "sigma0",
  "DOMAIN_", "startTime", "endTime", "Period", "Phase", "Amplitude",
  "frequency", "MaxTime", "frequency1", "frequency2", "frequency3",
  "frequency4", "stiffness_to_use_opt", "DAMPING_RAYLEIGH",
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
    2356,  2388,  2418,  2451,  2488,  2524,  2607,  2681,  2706,  2721,
    2750,  2785,  2818,  2845,  2865,  2926,  2952,  2971,  2998,  3045,
    3093,  3144,  3184,  3225,  3272,  3319,  3357,  3411,  3465,  3527,
    3581,  3632,  3683,  3736,  3788,  3825,  3863,  3889,  3915,  3939,
    3964,  4156,  4198,  4240,  4255,  4300,  4307,  4314,  4321,  4328,
    4335,  4342,  4348,  4355,  4363,  4371,  4379,  4387,  4395,  4399,
    4405,  4410,  4416,  4422,  4428,  4434,  4440,  4448,  4453,  4459,
    4464,  4469,  4474,  4479,  4484,  4492,  4529,  4534,  4538,  4548,
    4570,  4595,  4615,  4633,  4644,  4654,  4660,  4668,  4672
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
     467,   469,    37,    35,   468,    36,   472,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   473,
       2,    34,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   470,     2,   471,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   474,     2,   475,     2,     2,     2,     2,
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
     463,   464,   465,   466
    };
    const unsigned int user_token_number_max_ = 713;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 7971 "feiparser.tab.cc" // lalr1.cc:1155
#line 4700 "feiparser.yy" // lalr1.cc:1156


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



