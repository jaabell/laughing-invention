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
#line 281 "feiparser.yy" // lalr1.cc:407

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
    #line 286 "feiparser.yy" // lalr1.cc:725
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
#line 306 "feiparser.yy" // lalr1.cc:847
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
#line 320 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "poutput_of_element_to_screen");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 682 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 333 "feiparser.yy" // lalr1.cc:847
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
#line 346 "feiparser.yy" // lalr1.cc:847
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
#line 359 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 719 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 367 "feiparser.yy" // lalr1.cc:847
    {
		cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 729 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 372 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 735 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 373 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 374 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 747 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 375 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 753 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 376 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 759 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 380 "feiparser.yy" // lalr1.cc:847
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
#line 390 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 777 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 395 "feiparser.yy" // lalr1.cc:847
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
#line 432 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 825 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 437 "feiparser.yy" // lalr1.cc:847
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
#line 449 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 848 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 455 "feiparser.yy" // lalr1.cc:847
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
#line 472 "feiparser.yy" // lalr1.cc:847
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
#line 488 "feiparser.yy" // lalr1.cc:847
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
#line 507 "feiparser.yy" // lalr1.cc:847
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
#line 533 "feiparser.yy" // lalr1.cc:847
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
#line 562 "feiparser.yy" // lalr1.cc:847
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
#line 592 "feiparser.yy" // lalr1.cc:847
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
#line 629 "feiparser.yy" // lalr1.cc:847
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
#line 660 "feiparser.yy" // lalr1.cc:847
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
#line 698 "feiparser.yy" // lalr1.cc:847
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
#line 720 "feiparser.yy" // lalr1.cc:847
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
#line 755 "feiparser.yy" // lalr1.cc:847
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
#line 788 "feiparser.yy" // lalr1.cc:847
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
#line 803 "feiparser.yy" // lalr1.cc:847
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
#line 825 "feiparser.yy" // lalr1.cc:847
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
#line 876 "feiparser.yy" // lalr1.cc:847
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
#line 923 "feiparser.yy" // lalr1.cc:847
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
#line 961 "feiparser.yy" // lalr1.cc:847
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
#line 979 "feiparser.yy" // lalr1.cc:847
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
#line 998 "feiparser.yy" // lalr1.cc:847
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
#line 1017 "feiparser.yy" // lalr1.cc:847
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
#line 1033 "feiparser.yy" // lalr1.cc:847
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
#line 1049 "feiparser.yy" // lalr1.cc:847
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
#line 1067 "feiparser.yy" // lalr1.cc:847
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
#line 1088 "feiparser.yy" // lalr1.cc:847
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
#line 1125 "feiparser.yy" // lalr1.cc:847
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
#line 1144 "feiparser.yy" // lalr1.cc:847
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
#line 1162 "feiparser.yy" // lalr1.cc:847
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
#line 1178 "feiparser.yy" // lalr1.cc:847
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
#line 1198 "feiparser.yy" // lalr1.cc:847
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
#line 1222 "feiparser.yy" // lalr1.cc:847
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
#line 1241 "feiparser.yy" // lalr1.cc:847
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
#line 1263 "feiparser.yy" // lalr1.cc:847
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
#line 1303 "feiparser.yy" // lalr1.cc:847
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
#line 1317 "feiparser.yy" // lalr1.cc:847
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
#line 1339 "feiparser.yy" // lalr1.cc:847
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
#line 1367 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.clear();
							dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
						}
#line 1741 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1371 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
						}
#line 1749 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1376 "feiparser.yy" // lalr1.cc:847
    {
							(yylhs.value.exp) = new FeiVector();
							dynamic_cast<FeiVector*>((yylhs.value.exp))->add_component((yystack_[0].value.exp)->value());
							nodes.pop();
							nodes.push((yylhs.value.exp));
						}
#line 1760 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1382 "feiparser.yy" // lalr1.cc:847
    {
							dynamic_cast<FeiVector*>((yystack_[2].value.exp))->add_component((yystack_[0].value.exp)->value());
							(yylhs.value.exp) = (yystack_[2].value.exp);
							nodes.pop();
						}
#line 1770 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1394 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1780 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1403 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1790 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1410 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1800 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1417 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
		nodes.push((yylhs.value.exp));
	}
#line 1811 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1425 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("level", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
		nodes.push((yylhs.value.exp));
	}
#line 1822 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1435 "feiparser.yy" // lalr1.cc:847
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
#line 1451 "feiparser.yy" // lalr1.cc:847
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
#line 1532 "feiparser.yy" // lalr1.cc:847
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
#line 1588 "feiparser.yy" // lalr1.cc:847
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
#line 1663 "feiparser.yy" // lalr1.cc:847
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
#line 1698 "feiparser.yy" // lalr1.cc:847
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
#line 1712 "feiparser.yy" // lalr1.cc:847
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
#line 1725 "feiparser.yy" // lalr1.cc:847
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
#line 1752 "feiparser.yy" // lalr1.cc:847
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
#line 1790 "feiparser.yy" // lalr1.cc:847
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
#line 1818 "feiparser.yy" // lalr1.cc:847
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
#line 1831 "feiparser.yy" // lalr1.cc:847
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
#line 1851 "feiparser.yy" // lalr1.cc:847
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
#line 1870 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(this_signature("non_converged_iterations", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller0<>(&save_non_converged_iterations, args, signature, "save_non_converged_iterations");
	}
#line 2268 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1880 "feiparser.yy" // lalr1.cc:847
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
#line 1897 "feiparser.yy" // lalr1.cc:847
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
#line 1921 "feiparser.yy" // lalr1.cc:847
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
#line 1945 "feiparser.yy" // lalr1.cc:847
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
#line 1969 "feiparser.yy" // lalr1.cc:847
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
#line 1993 "feiparser.yy" // lalr1.cc:847
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
#line 2017 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		//int simulate_constitutive_testing_BardetLT_path(int MaterialNumber, int type, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, )

		//Get the string from the parsed string
		string filename = *(yystack_[15].value.ident);
		//Remove quotes
		filename.erase(0, 1);
		filename.erase(filename.length()-1, filename.length());

		//Parse BARDETMETHOD
		int btype = bardet_type_parse(*(yystack_[25].value.ident));
		Expression* type = new Number( btype, ESSIunits::unitless);

		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("MaterialNumber", &isAdimensional));
		args.push_back(type); signature.push_back(this_signature("type", &isAdimensional));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("scale_factor", &isAdimensional));
		args.push_back( new FeiString(filename)); signature.push_back(this_signature("series_file", &isAdimensional));
		args.push_back((yystack_[11].value.exp)); signature.push_back(this_signature("sxx0", &isPressure));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("syy0", &isPressure));
		args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("szz0", &isPressure));
		args.push_back((yystack_[5].value.exp)); signature.push_back(this_signature("sxy0", &isPressure));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("sxz0", &isPressure));
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("syz0", &isPressure));


		(yylhs.value.exp) = new FeiDslCaller10<int, int, double, std::string, 
		   	double, double, double, double, double, double>(&simulate_constitutive_testing_BardetLT_path, args, signature, "simulate_constitutive_testing_BardetLT_path");

		for(int ii = 1;ii <=8; ii++) nodes.pop();

		nodes.push((yylhs.value.exp));
	}
#line 2431 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 2054 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&run_test, args, signature, "run_test");
		nodes.push((yylhs.value.exp));
	}
#line 2441 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 2064 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		(yylhs.value.exp) = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

	}
#line 2452 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 2080 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2466 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2093 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2480 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2106 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

		//pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2495 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2120 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
			 //pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2508 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2132 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2521 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2144 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_displacement_from_node, args, signature, "remove_displacement_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2534 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2156 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_strain_from_element, args, signature, "remove_strain_from_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2547 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2180 "feiparser.yy" // lalr1.cc:847
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
#line 2563 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2201 "feiparser.yy" // lalr1.cc:847
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
#line 2594 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2238 "feiparser.yy" // lalr1.cc:847
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
#line 2616 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2266 "feiparser.yy" // lalr1.cc:847
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
#line 2638 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2294 "feiparser.yy" // lalr1.cc:847
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
#line 2660 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2324 "feiparser.yy" // lalr1.cc:847
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
#line 2684 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2355 "feiparser.yy" // lalr1.cc:847
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
#line 2707 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2387 "feiparser.yy" // lalr1.cc:847
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
#line 2731 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2418 "feiparser.yy" // lalr1.cc:847
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
#line 2761 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2477 "feiparser.yy" // lalr1.cc:847
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
#line 2815 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2555 "feiparser.yy" // lalr1.cc:847
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
#line 2865 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2610 "feiparser.yy" // lalr1.cc:847
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
#line 2885 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2629 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		(yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push((yylhs.value.exp));
	}
#line 2901 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2651 "feiparser.yy" // lalr1.cc:847
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
#line 2924 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 2683 "feiparser.yy" // lalr1.cc:847
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
#line 2950 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 2715 "feiparser.yy" // lalr1.cc:847
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
#line 2973 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 2743 "feiparser.yy" // lalr1.cc:847
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
#line 2994 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 2771 "feiparser.yy" // lalr1.cc:847
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
#line 3012 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 2805 "feiparser.yy" // lalr1.cc:847
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
#line 3048 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 2852 "feiparser.yy" // lalr1.cc:847
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
#line 3072 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 2877 "feiparser.yy" // lalr1.cc:847
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
#line 3090 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 2896 "feiparser.yy" // lalr1.cc:847
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
#line 3116 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 2925 "feiparser.yy" // lalr1.cc:847
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
#line 3160 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 2972 "feiparser.yy" // lalr1.cc:847
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
#line 3205 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3020 "feiparser.yy" // lalr1.cc:847
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
#line 3252 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3071 "feiparser.yy" // lalr1.cc:847
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
#line 3289 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3111 "feiparser.yy" // lalr1.cc:847
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
#line 3327 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3160 "feiparser.yy" // lalr1.cc:847
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
#line 3363 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3207 "feiparser.yy" // lalr1.cc:847
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
#line 3399 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3247 "feiparser.yy" // lalr1.cc:847
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
#line 3433 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3286 "feiparser.yy" // lalr1.cc:847
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
#line 3482 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3340 "feiparser.yy" // lalr1.cc:847
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
#line 3531 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3394 "feiparser.yy" // lalr1.cc:847
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
#line 3588 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3457 "feiparser.yy" // lalr1.cc:847
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
#line 3624 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3515 "feiparser.yy" // lalr1.cc:847
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
#line 3665 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3566 "feiparser.yy" // lalr1.cc:847
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
#line 3706 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3614 "feiparser.yy" // lalr1.cc:847
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
#line 3752 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3670 "feiparser.yy" // lalr1.cc:847
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
#line 3794 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3718 "feiparser.yy" // lalr1.cc:847
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
#line 3825 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 3755 "feiparser.yy" // lalr1.cc:847
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
#line 3857 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 3789 "feiparser.yy" // lalr1.cc:847
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
#line 3881 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 3815 "feiparser.yy" // lalr1.cc:847
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
#line 3905 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 3841 "feiparser.yy" // lalr1.cc:847
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
#line 3927 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 3865 "feiparser.yy" // lalr1.cc:847
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
#line 3950 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 3891 "feiparser.yy" // lalr1.cc:847
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
#line 3970 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 4079 "feiparser.yy" // lalr1.cc:847
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
#line 4016 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 4121 "feiparser.yy" // lalr1.cc:847
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
#line 4062 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4163 "feiparser.yy" // lalr1.cc:847
    {
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		(yylhs.value.errcode) = 0;
	}
#line 4078 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4178 "feiparser.yy" // lalr1.cc:847
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
#line 4127 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4223 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4138 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4230 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4149 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4237 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4160 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4244 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4171 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4251 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4182 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4258 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4193 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4265 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.push ((yylhs.value.exp));
	}
#line 4203 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4271 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4214 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4278 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4226 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4286 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4238 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4294 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4250 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4302 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4262 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4310 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4274 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4318 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4282 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4322 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4292 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4328 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Number (*(yystack_[0].value.value));
		nodes.push ((yylhs.value.exp));
	}
#line 4301 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4333 "feiparser.yy" // lalr1.cc:847
    {

		(yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 4311 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4339 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4321 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4345 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

		nodes.push((yylhs.value.exp));
	}
#line 4331 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4351 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4341 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4357 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4351 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4363 "feiparser.yy" // lalr1.cc:847
    {
		string temp = *(yystack_[0].value.ident);
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		(yylhs.value.exp) = new FeiString(temp);
		nodes.push((yylhs.value.exp));
	}
#line 4363 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4371 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);//new FeiVector($3);
		//nodes.push($$);
	}
#line 4372 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4376 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push((yylhs.value.exp));
	}
#line 4382 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4382 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push((yylhs.value.exp));
	}
#line 4391 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4387 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push((yylhs.value.exp));
	}
#line 4400 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4392 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4409 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4397 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4418 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4402 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push((yylhs.value.exp));
	}
#line 4427 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4407 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push((yylhs.value.exp));
	}
#line 4436 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4415 "feiparser.yy" // lalr1.cc:847
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
#line 4468 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4446 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4477 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4451 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4485 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4455 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
		args.clear();
		signature.clear();
	}
#line 4495 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4464 "feiparser.yy" // lalr1.cc:847
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
#line 4518 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4486 "feiparser.yy" // lalr1.cc:847
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
#line 4544 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4511 "feiparser.yy" // lalr1.cc:847
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
#line 4568 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4531 "feiparser.yy" // lalr1.cc:847
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
#line 4590 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4549 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		(yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push((yylhs.value.exp));
	}
#line 4605 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4560 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4615 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4570 "feiparser.yy" // lalr1.cc:847
    {
		// empty statement generates an  empty expression
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4625 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4576 "feiparser.yy" // lalr1.cc:847
    {
		// Used to initialize a list of statements (compound statement)
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4634 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4584 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[0].value.exp);
	}
#line 4642 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4588 "feiparser.yy" // lalr1.cc:847
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
#line 4658 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4662 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -450;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
    2540,  -450,  1389,  -450,  -450,  -449,  -443,  -433,   354,   354,
    -450,  -450,    54,  -450,  -450,  -450,  -450,  -450,  -450,  -450,
     354,   354,   279,  -450,  -450,    16,    27,  -450,  5662,  -397,
    -360,    47,    51,  2346,   577,    67,    38,  -450,   -10,   -16,
     -30,  -320,  -317,   354,  -450,  -321,  -450,  -450,  -450,  -450,
    -450,   165,   -19,  -450,   152,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   -32,   -32,  -450,   121,   121,
     -94,    50,  9192,   354,   167,    63,    66,    68,    70,    90,
     100,   102,  -213,   104,   117,   124,    75,   143,   157,  -450,
     354,  -178,   162,   163,   170,   179,   180,   182,  -142,   186,
     187,   181,   235,   230,   231,   107,   225,   241,   150,   229,
    8922,   263,   205,   354,  -111,  -450,  -450,  -450,   191,  -450,
    -450,  -450,   354,   354,   354,   354,   354,   354,   354,  -450,
    -450,  9192,  9192,  9192,  9192,  9192,  9192,  9192,  -257,  9192,
     212,   306,  2596,   335,  -450,   354,   354,  9192,  -450,   354,
     354,  -450,   354,   354,   354,   354,   354,   237,   232,   354,
     354,   287,   354,   298,    -7,   236,   354,   354,   354,   354,
     354,   354,   311,   313,   305,   239,  -450,   -34,  -450,   259,
     260,   274,   289,   -96,   295,   280,  -450,   383,  8932,  -450,
    -450,   413,   413,   137,   137,     3,   121,   346,   354,  -450,
    -450,  -450,  -450,  2694,   -75,   -73,  9192,  9192,  9109,  8874,
    8883,  9003,  9013,  9024,  8865,   -53,   354,  8892,  5776,   276,
    9156,   281,   -65,   277,  9066,  9075,  9192,  9192,  9192,  9192,
     283,   290,   294,   354,   377,   189,     7,   362,   339,   309,
     248,   252,   -71,   319,    52,  -450,  -450,  9192,  -450,  -450,
    -450,   -29,  9063,  9015,    12,   373,   330,   149,  -299,   314,
     388,  8690,   -68,   410,   391,   389,   354,   445,   354,   -18,
     338,   354,   354,   354,  9192,   354,   420,   424,   343,   376,
     405,   242,   286,    29,   321,   440,   393,   419,   421,   354,
     -21,    11,   392,   400,    13,   401,   404,   407,   408,   409,
     411,   415,   416,   418,   422,   425,   444,   451,   452,   454,
     455,   462,   463,   214,   220,   226,   299,   300,   301,   155,
     285,   228,   244,   310,   315,   322,   325,   328,   332,   333,
     334,   485,   490,   466,   506,   507,   511,   521,   532,   533,
     535,   354,   542,   615,   384,   368,   370,   402,   354,   549,
     553,  2083,  -450,  9192,  -450,   338,  -450,  -450,  9192,  9192,
    9192,  5731,   354,   354,   354,   265,   557,   499,   489,   491,
     493,   500,   354,  -450,   580,   582,    48,   354,   565,   354,
     567,   610,   611,   354,   612,   613,   354,   614,   619,   629,
     635,   636,   637,   638,   639,   642,   643,   644,   646,   647,
     654,   655,   663,   702,   704,   751,   759,   760,   768,   769,
     771,   772,   773,   777,   779,   783,   787,   790,   800,   802,
     803,   296,   354,   708,   354,   714,   354,   354,   423,   431,
     432,  8942,   354,   354,   806,   815,   816,   817,  5767,   354,
     354,   819,  -450,   820,  9192,  5707,  8955,   828,   354,   688,
     756,   764,   765,   698,  9192,   851,   863,   354,  8350,   441,
    8361,   442,   443,   446,  8370,   447,   448,  8381,   449,   453,
     464,   465,   467,   468,   469,   470,   477,   478,   480,   481,
     505,   508,   509,   510,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   778,  8902,  8911,   776,  8975,   784,  9192,  9192,
     935,   938,   942,   858,  9097,  8827,   354,   354,   354,   354,
     944,    23,  9033,   354,   354,   945,   882,   354,  8842,   830,
     959,   961,   963,   886,   354,   354,   233,   834,   354,   835,
     354,   354,   354,   836,   354,   354,   837,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,  7092,  7104,  6001,  6253,  6262,  6592,  6048,
    7083,  7114,  7123,  5834,  7132,  7142,  7151,  7162,  7178,  7187,
    7199,   883,    98,    77,   911,   913,   933,   354,   354,   354,
     597,   605,  1019,  9045,  6964,  7734,  9192,   354,   937,   990,
    1240,  5749,   354,   928,  5722,  1033,   951,   954,   962,   970,
    1029,  9192,  9129,   354,   899,  1208,   900,  1556,  1872,  1957,
     907,  2002,  2014,   908,  2042,  2051,  2171,  2205,  2247,  2275,
    2333,  2400,  2440,  2455,  2622,  2643,  2715,  2724,  2741,  2758,
    1066,  1067,  1068,  1069,  1070,  1071,  1072,  1076,  1077,  1094,
    1095,  1097,  1098,  1099,  1100,  1119,  1121,  1122,  -450,  1037,
    1038,  1041,  -450,  1050,  1051,  1048,  1092,   -59,  1144,  5633,
    5642,  5651,  1075,  1124,   354,  1123,  1148,  1154,  5758,  1125,
    1079,  1157,  1170,  9192,   354,  1179,   354,  1138,   354,   354,
     354,  1101,  1184,   365,   972,   354,   976,   354,   354,   354,
     978,   354,   354,   979,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,  1189,  1194,
    1195,  1153,  1165,  1128,  1147,  1230,  1238,   354,  1247,  1253,
    1254,  1234,  1174,  9192,  1176,   354,   354,  1268,  1112,   354,
     354,   354,  9192,   354,  8656,  1191,  8595,  8604,  8613,   354,
     354,  1196,  1197,  2775,  1198,  2784,  2793,  2802,  1199,  2811,
    2825,  1200,   399,   429,   459,   523,   534,  2834,  2843,  2852,
    2865,   560,   573,   595,   604,  2880,  2889,  2898,  6973,  6982,
    5992,  6280,  6238,  6583,  6039,  9192,  6058,  6067,  5825,  6992,
    7001,  7010,  7021,  7030,  7039,  7051,   354,   354,   354,  1192,
    1213,   872,   354,   354,   354,  8856,   354,   354,   354,  1215,
     354,   354,  7312,  7601,   354,   885,  7374,   254,  9192,  5740,
    1298,   354,  1305,  1306,  1307,  8622,  9138,   354,  1221,   354,
    1239,   354,   354,   354,  1241,   354,   354,  1242,  1246,  1180,
    1143,  1146,  1149,   354,   354,   354,   354,  1262,  1141,  1190,
    1151,   354,   354,   354,  1352,  1355,  1356,  1357,  1358,  1359,
    1368,  1369,  1371,  1372,  1375,  1390,  1398,  1399,  1402,  1410,
    1411,  9192,  9147,  8665,   354,   354,   354,  9192,  8818,  8748,
    1413,  5603,  5564,  5573,   354,  1011,  9192,  1424,  1426,  9192,
     354,  1427,  1433,  1434,  1484,  8631,   354,   354,   354,  1453,
    1457,  9087,  1034,  2907,  1039,  2916,  2925,  2934,  1046,  2943,
    2970,  1049,   354,  1477,  1478,  1479,  1489,  2979,  2988,   617,
    2997,  1466,  1499,  1503,  1506,  3028,  3037,  3047,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,  1507,  1508,  9192,  9192,  3056,
    1510,  1517,   354,  1530,  1538,  1539,  8984,   338,   354,   354,
    9192,   354,   354,   354,  1114,  1541,  8543,  8554,  8563,   354,
     354,  -450,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,  8994,   354,   354,   354,   354,   354,   354,  1448,
     354,  1460,   354,   354,   354,   354,   354,   354,  9192,  9192,
    5982,  6508,  7061,  6628,  6030,  5660,  5527,  5807,  6944,  6954,
    6886,  6898,  6913,  6922,  6935,   354,  1573,   354,   354,   354,
    8646,  1155,   354,   354,  1469,  -450,  7235,  7556,  7363,  5518,
    9192,  1545,   354,  1555,  1564,  1566,  8572,  9118,  3068,  3077,
    3086,  3095,  3124,  3133,  3142,  3151,  3166,  3175,  1501,  7301,
    7208,  7217,  7226,   706,   734,  1543,   743,   354,  7504,  8341,
    9054,  3184,  3193,  3202,  1582,  1585,  1586,  1591,  1595,  1604,
    1607,  1615,  1616,  1618,  1619,  1620,  1622,  1626,  1628,  8678,
    -450,  3211,  8757,  8795,  1629,  -450,  5612,   547,  1218,  1632,
    1642,  1645,  1647,  1211,  8581,   354,   354,   354,  1649,  1650,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
    1575,  1651,  1652,  1653,  1655,  1576,  1581,  1590,  1592,  7743,
    1658,  1668,  1664,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,  1673,   354,  1674,  1676,   354,  1677,  1697,  -450,   354,
     354,  1266,   354,   354,  1698,  8475,  8484,  8509,   354,   354,
    3220,  3229,  3238,  3251,  3260,  3269,  3278,  3290,  3306,  3315,
    1621,   354,   354,   354,   354,  1678,  1680,   354,  1684,  1700,
     354,   354,  1634,  3324,  3333,  3342,  5973,  6529,  6271,  6614,
    5698,  5582,  2128,  5794,  6643,  6857,  6652,  6665,  6674,  6866,
    6877,  1730,  3351,   354,   354,  8717,  1326,   354,  9192,  7686,
     354,  1023,  3360,   354,  1719,  1720,  1722,  8518,  9192,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
    7245,  7695,  7711,  7725,  1641,  1644,  7323,  1654,   354,  7486,
    7513,   354,   354,   354,   354,  1737,  1738,  1739,  1741,  1751,
    1752,  1755,  1756,  1757,  1758,  1759,  1767,  1768,  1770,  1771,
    1394,   354,  8807,  8699,  1773,  -450,  5624,  1774,  3369,  1775,
     354,  8528,   354,   354,   354,  1783,  3378,  3387,  3396,  3405,
    3414,  3423,  3432,  3441,  3454,  3464,  7254,  1785,  1786,  1790,
    1794,   354,   354,  1797,   354,  7263,  1802,  1803,  9192,  3473,
    3482,  3494,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,  1806,     2,  1807,
    1809,   354,  1810,   354,   354,   354,  3503,  1811,  9192,  9192,
    9192,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,  1812,   354,   354,   354,   354,  7332,  7343,   354,
    7352,  1813,   354,   354,   354,   354,   354,  5964,  6517,  6229,
    6605,  6021,  5545,  1258,  5785,  6683,  6825,  6694,  6704,  6713,
    6837,  6846,  1396,   354,  1736,   354,   354,  9192,  1430,  7642,
     752,   855,   354,   354,  9192,  3512,  3521,  3530,  3539,  3550,
    3559,  3568,  3577,  3592,  3601,   354,  7402,  7657,  7668,  7677,
    1818,  1822,  9192,  1824,   354,  7468,  7495,  3610,  3619,  3628,
    1825,  1827,  1828,  1830,  1840,  1844,  1847,  1851,  1853,  1854,
    1855,  1856,  1857,  1859,  1860,   354,  3637,    58,  8708,  8766,
    -450,  1868,  -450,  1869,  3646,  9192,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,  7522,  1870,  1871,  1880,
    1882,   354,   354,   354,  9192,  1883,  1885,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,  3655,   354,  1409,   354,  1886,
    1887,   354,   354,   354,  3664,   761,  3677,  3686,   792,  3695,
    3704,  3715,   831,  3724,  1888,  1467,   354,   354,   354,  9192,
    9192,  9192,   354,   354,   840,  3733,  3742,  5955,  6499,  6220,
    9192,  6010,  1919,  5669,  7070,  9192,  6722,  6753,  6762,  6771,
    6732,  6741,   354,  3751,   354,  9192,   354,   354,  9192,  9192,
    3760,   354,  1789,   354,   354,  1820,   354,   354,   354,  1821,
     354,   354,   354,  7565,  7576,  7585,  7448,  7477,  1826,   354,
     354,  1890,  1892,  1913,  1914,  1915,  1916,  1917,  1929,  1930,
    1932,  1933,  1934,  1942,  3769,   354,  3778,  8786,  8730,   354,
    3787,  1895,  3796,  3805,  1924,  3814,  3823,  3832,  1925,  3841,
    7411,  3850,  1953,  1954,  1965,  1967,  1969,  1947,  3859,  3868,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,  3879,   354,  1972,  1974,  3890,   354,
    1893,   354,   354,  1896,   354,   354,   354,  1897,   354,  1977,
     354,   354,   354,   354,   354,   354,  1898,   354,   354,  5946,
    6484,  6205,  9192,  9192,  5594,  6783,  9192,  9192,  6538,  9192,
    6792,  6802,  3899,   354,  3908,   354,   354,   354,  3920,   354,
    3929,  3938,   354,  3947,  3956,  3965,   354,  3976,  1562,  3985,
    7614,  7624,  7633,  7383,  7459,   354,  3994,  4003,  1990,  1992,
    1993,  1997,  2009,  2010,  2012,  2021,   354,    96,   354,  8739,
    9192,   878,   354,  9192,   354,   354,  8276,   354,   354,   354,
    8285,   354,   354,   354,  2022,  2036,  2037,  2038,  2049,  8295,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
    4018,   354,  1935,  4027,  2050,  -450,   894,  4036,  4045,  2058,
    4054,  4063,  4072,  2059,  4081,  4090,   922,   354,   354,   354,
    1638,   354,  2070,  4103,  4112,  5936,  6475,  6196,  5555,  9192,
    6560,  9192,  6547,   354,   975,    88,   354,   354,  1959,   354,
     354,   354,   354,   354,   354,   354,   354,   354,  1862,  7272,
    7283,  7292,   354,  7393,   354,   354,   354,  2076,  2078,  2079,
    2080,  2081,  2090,  4121,  2015,  1669,   354,  1002,  9192,  2072,
    4130,  4141,  8215,  4150,  4159,  4168,  8224,  4177,  4186,  2097,
    2098,  2103,  2104,  4195,  2105,  8233,  4204,  4213,   354,   354,
     354,   354,   354,   354,   354,    91,   354,  9192,  -450,  2024,
     354,   354,  2107,   354,   354,   354,  2112,   354,   354,  1696,
     354,   354,   354,   354,  1699,  2121,   354,   354,  5918,  6465,
    6187,  5536,  9192,  6569,  1036,  1712,   354,  4222,   354,  4231,
    4240,   354,  4249,  4258,  4267,   354,  4276,  1045,   354,  7421,
    7430,  7439,  4285,   354,   354,  4294,  4304,  2122,  2138,  2139,
    2140,  2144,  -450,   354,  9192,   354,  9192,   354,   354,  8150,
     354,   354,   354,  8159,   354,  1936,  4316,  2145,  2146,  2147,
     354,  4325,  8170,   354,   354,   354,   354,   354,   354,   354,
    4334,  4346,  4355,  4364,  2149,  4373,  4382,  4391,  2151,  4402,
    2155,   354,  1733,  1740,  1742,  1056,   354,  2161,  4411,  4420,
    5908,  6452,  6178,  9192,  9192,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,  1745,  4429,   354,   354,   354,
    -450,  4444,   354,   354,   354,  2162,  2163,  2168,  4453,  4462,
    4471,  4480,  8083,  4489,  4498,  4507,  8092,  4516,   354,   354,
    4529,  4538,  4547,   354,  8104,  4556,  4567,   354,   354,   354,
     354,   354,   354,   354,  2178,   354,   354,   354,  2179,   354,
    4576,  1087,   354,   354,   354,  1137,  2184,   354,   354,  5899,
    6443,  6169,  4585,  4594,  4603,  4612,   354,  4621,  4630,  4639,
     354,  4648,   354,  1958,  4657,  4666,  4675,  -450,   354,  4684,
    4693,  2185,  2193,  2194,   354,   354,   354,   354,  8022,   354,
     354,   354,  8031,   354,  4702,  2196,   354,   354,   354,  8040,
     354,   354,   354,   354,   354,  4711,  4721,  4730,  4742,  2197,
    4751,  4760,  4772,  2200,  4781,   354,  1777,  1162,  1171,  1222,
    2202,  4790,  4799,  5890,  6430,  6158,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,  1231,   354,  1991,  2005,
    2006,   354,   354,   354,  2204,  2219,  2220,  4808,  4817,  4828,
    4837,  7956,  4846,  4855,  4870,  7965,  4879,  2008,  4888,  2224,
    2225,  2226,  7976,  4897,  4906,   354,   354,   354,   354,   354,
     354,   354,  2227,   354,   354,   354,  2228,   354,  2231,   354,
    1808,  1814,  1815,  2234,   354,   354,  5872,  6421,  6149,  4915,
    4924,  4933,  4942,   354,  4955,  4964,  4973,   354,  4982,  1816,
    4993,   354,   354,   354,   354,  5002,  5011,  2235,  2237,  2241,
     354,   354,   354,   354,  7887,   354,   354,   354,  7896,   354,
     354,   354,  5020,  5029,  5038,  7910,   354,   354,   354,   354,
     354,  5047,  1334,  5056,  5065,  2242,  1361,  5074,  1382,  2254,
    5083,  5092,  1391,   354,   354,   354,  2255,  1403,  5101,  5863,
    6410,  6134,   354,  -450,   354,   354,   354,  2164,   354,  2174,
     354,   354,   354,  -450,  5110,  5119,  5128,   354,  2175,   354,
    2260,  2261,  2262,  5138,  5147,  5156,  7826,  2243,  5168,  2258,
    7835,  5177,  5186,   354,   354,   354,  7844,  2259,  5198,   354,
     354,   354,   354,   354,   354,  2285,  2206,   354,  2207,  2286,
     354,   354,  1463,  1482,  1492,  2287,  2208,   354,  5854,  6401,
    6125,  1520,  5207,  5216,   354,   354,  5225,   354,   354,  5234,
    1531,  2085,  2092,  2093,   354,   354,  5243,  2291,  2303,  2307,
    -450,   354,   354,  7759,  8311,   354,  9192,  7768,   354,  -450,
    2308,  2309,  2312,  7779,  8320,   354,   354,   354,   354,  5254,
    5263,  2313,  2314,  1547,  2315,  5272,  1894,  1899,  1904,  2317,
    2322,  5281,  5845,  6387,  6110,   354,   354,   354,   354,  2249,
     354,   354,   354,   354,   354,   354,   354,   354,  2340,  2341,
    2347,  5296,  5305,  9192,  8244,  2324,  9192,  5314,  5323,  5332,
    5341,  9192,  8258,  5350,   354,   354,   354,   354,   354,  2349,
    2251,   354,   354,   354,   354,  2352,   354,  9192,  6378,  6101,
    5359,  5368,   354,   354,  5381,  5390,  5399,  5408,   354,  5419,
    2353,  2354,   354,   354,  8179,  9192,   354,   354,   354,   354,
    8191,   354,   354,   354,  5428,  5437,  2355,  5446,  1608,  1635,
    1690,  2356,  5455,  6369,  6080,   354,   354,   354,   354,  -450,
    -450,  -450,   354,   354,  2357,  2358,  5464,  1709,  8119,  5473,
    8128,  1744,   354,   354,   354,  2276,  2363,   354,  2364,  2283,
    6354,  6816,  5482,  2345,   354,  5491,   354,  2351,  2370,  2376,
     354,  2297,  8052,   354,  8065,  2298,   354,   354,  5500,   354,
    2383,  5509,  2388,   354,  6345,  9167,   354,  9192,   354,   354,
     354,  8329,  2389,  2390,  1760,  7985,  1833,  7998,  2392,   354,
     354,  2310,  2395,  2316,  2410,   354,  6329,  8457,  2387,   354,
    2391,   354,  8267,  2412,  2414,  2318,  7924,  2334,  7933,  2418,
     354,   354,   354,  2420,   354,  2421,   354,  6313,  8433,  9192,
     354,  9192,   354,  8204,  2422,  2423,  7857,  7869,  2424,   354,
     354,  2425,  2426,   354,  6304,  8414,   354,   354,  8137,  2431,
    2433,  7788,  7802,  2435,   354,   354,  2436,  2437,   354,  6289,
    8394,   354,   354,  8074,  2450,  2452,  9192,  9192,  2453,   354,
     354,   354,  9177,  9192,  8011,  2465,  2468,   354,   354,  8466,
    7942,  2471,  2472,   354,   354,  8448,  7878,  2473,  2474,   354,
     354,  8423,  7815,  2475,  2480,   354,   354,  8405,  9192,  2481,
     354,  9192
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   141,   159,   158,   164,     0,     0,     0,     0,     0,
      13,   180,     0,   166,   167,   168,   169,   170,   171,   172,
       0,     0,     0,     7,     6,     0,     0,   181,    14,     0,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,   174,     0,     8,    10,    11,     9,
      12,     0,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   157,   149,
       0,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    77,     0,   176,
       1,   140,     0,     0,     0,     0,     0,     0,     0,   175,
     161,   150,   151,   152,   153,   154,   155,   160,     0,    57,
       0,     0,     0,   177,   179,     0,     0,     5,    72,     0,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    68,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,    61,
     156,   143,   144,   145,   146,   147,   148,   142,     0,   165,
     163,   162,   183,   185,     0,     0,     4,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    87,    90,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    62,    58,   186,   184,
     178,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,    63,    52,    55,    51,    54,    91,    92,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,     0,
       0,     0,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,     0,     0,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    47,     0,     0,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,    41,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    80,    83,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,   110,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   137,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,    49,    42,
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
       0,     0,   109,   107,     0,     0,    96,    97,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,    26,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   133,     0,     0,
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
       0,     0,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,   119,     0,     0,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,     0,     0,   123,     0,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   128,
     129,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   117,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   124,   125,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   126,     0,
       0,   103
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -450,  -450,  -450,  -450,  -247,  -450,  -450,  -450,  -450,  -450,
    -450,  -450,    -8,    74,   -56,  2320
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    45,    46,    47,   356,   138,    48,    49,    50,    89,
     151,    51,    52,   203,   143,   204
  };

  const short int
  feiparser::yytable_[] =
  {
      65,    66,   259,   122,   123,   124,   125,   126,   127,    62,
     144,   128,    68,    69,    72,    63,   122,   123,   124,   125,
     126,   127,   145,   357,   128,    64,   114,   110,   122,   123,
     124,   125,   126,   127,   239,   118,   128,   122,   123,   124,
     125,   126,   127,   127,   745,   128,   128,   131,   132,   133,
     134,   135,   136,   137,   139,   140,   141,    67,   122,   123,
     124,   125,   126,   127,    90,   147,   128,   331,   332,    73,
       2,     3,     4,     5,    53,     6,     7,   240,    74,   377,
     241,   242,   164,   122,   123,   124,   125,   126,   127,   746,
      91,   128,   378,   354,    13,    14,    15,    16,    17,    18,
      19,   355,    92,    20,   283,   188,    93,   284,   442,    21,
     111,   379,   113,   383,   191,   192,   193,   194,   195,   196,
     197,   112,    29,   333,   380,   121,   384,   338,   339,   340,
     116,   122,   123,   124,   125,   126,   127,   206,   207,   128,
     117,   208,   209,   119,   210,   211,   212,   213,   214,   250,
     286,   217,   218,   287,   220,   130,   108,   288,   224,   225,
     226,   227,   228,   229,   128,   120,   146,   109,     2,     3,
       4,     5,   148,     6,     7,     8,   126,   127,     9,   149,
     128,    10,   150,    11,   152,    12,   153,   344,   345,   346,
     247,   347,    13,    14,    15,    16,    17,    18,    19,   179,
     180,    20,   198,   199,   336,   337,   154,    21,   261,    22,
      23,    24,    25,   369,   370,    26,   155,    27,   156,    28,
      29,   664,   665,  1497,  1498,   274,   122,   123,   124,   125,
     126,   127,   157,   159,   128,   158,    30,   659,   660,   661,
     160,   662,   663,    31,    32,    33,    34,   122,   123,   124,
     125,   126,   127,  1795,  1796,   128,  1855,  1856,   351,   162,
     353,    35,    36,   358,   359,   360,   161,   361,   122,   123,
     124,   125,   126,   127,    37,   163,   128,   165,   166,   167,
      38,   376,     2,     3,     4,     5,   168,     6,     7,   122,
     123,   124,   125,   126,   127,   169,   170,   128,   171,     2,
       3,     4,     5,   172,     6,     7,    13,    14,    15,    16,
      17,    18,    19,   173,   174,    20,   177,   175,   176,   178,
     181,    21,   183,    13,    14,    15,    16,    17,    18,    19,
     182,   184,    20,   431,    70,    71,   186,   187,    21,   189,
     438,   122,   123,   124,   125,   126,   127,   205,   216,   128,
     215,    29,   219,   221,   444,   445,   446,     2,     3,     4,
       5,   232,     6,     7,   454,   223,   230,    90,   231,   458,
     234,   460,   235,   236,   237,   464,   233,   243,   467,   238,
     244,    13,    14,    15,    16,    17,    18,    19,   245,    -1,
      20,   249,   142,   260,   264,   268,    21,   266,   267,   271,
     122,   123,   124,   125,   126,   127,   272,   502,   128,    29,
     273,   275,   276,   503,   504,   277,   506,   278,   508,   509,
     115,   279,   281,   285,   514,   515,   280,   282,   334,   289,
     341,   521,   522,   142,   122,   123,   124,   125,   126,   127,
     528,   335,   128,   342,   348,   129,   349,   350,   352,   536,
     124,   125,   126,   127,   362,   222,   128,   355,   363,   364,
     365,  1393,  1394,   366,   122,   123,   124,   125,   126,   127,
     368,   598,   128,   367,   372,   373,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   122,   123,   124,   125,   126,   127,
     371,   374,   128,   375,   386,   381,   403,   457,   593,   594,
     595,   596,   404,   382,   385,   600,   601,   409,   405,   604,
     387,   388,   389,   423,   390,    43,   611,   612,   391,   392,
     615,   393,   617,   618,   619,   394,   621,   622,   395,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,  1731,  1732,   396,   122,   123,
     124,   125,   126,   127,   397,   398,   128,   399,   400,   122,
     123,   124,   125,   126,   127,   401,   402,   128,   410,   669,
     670,   671,   122,   123,   124,   125,   126,   127,   411,   678,
     128,   406,   407,   408,   683,   122,   123,   124,   125,   126,
     127,   421,   413,   128,   412,   693,   422,   414,   122,   123,
     124,   125,   126,   127,   415,    39,   128,   416,    40,    41,
     417,    42,   424,    43,   418,   419,   420,   426,   425,    44,
     122,   123,   124,   125,   126,   127,   101,   427,   128,   122,
     123,   124,   125,   126,   127,   428,   429,   128,   430,   433,
     434,   190,   122,   123,   124,   125,   126,   127,   432,   102,
     128,   435,   103,   436,   104,   439,   753,   437,   105,   440,
     447,   106,   200,   448,   449,   450,   762,   451,   764,   452,
     766,   767,   768,   455,   453,   456,   459,   773,   461,   775,
     776,   777,   613,   779,   780,   912,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   815,
    1045,   462,   463,   465,   466,   468,   484,    43,   485,   825,
     469,   122,   123,   124,   125,   126,   127,   832,   833,   128,
     470,   836,   837,   838,    43,   839,   471,   472,   473,   474,
     475,   845,   846,   476,   477,   478,   201,   479,   480,   122,
     123,   124,   125,   126,   127,   481,   482,   128,   122,   123,
     124,   125,   126,   127,   483,   486,   128,   122,   123,   124,
     125,   126,   127,   487,   488,   128,   122,   123,   124,   125,
     126,   127,   489,   490,   128,   491,   492,   493,   891,   892,
     893,   494,    43,   495,   897,   898,   899,   496,   901,   902,
     903,   497,   905,   906,   498,   771,   909,   122,   123,   124,
     125,   126,   127,   915,   499,   128,   500,   501,   507,   921,
     516,   923,   505,   925,   926,   927,   107,   929,   930,   517,
     518,   519,   510,   523,   524,   937,   938,   939,   940,   858,
     511,   512,   527,   945,   946,   947,   122,   123,   124,   125,
     126,   127,   529,   530,   128,   122,   123,   124,   125,   126,
     127,   531,   532,   128,   533,   534,   967,   968,   969,   859,
     122,   123,   124,   125,   126,   127,   976,   535,   128,   538,
     540,   541,   980,   586,   542,   544,   545,   547,   986,   987,
     988,   548,   581,   122,   123,   124,   125,   126,   127,   860,
     584,   128,   549,   550,  1002,   551,   552,   553,   554,   122,
     123,   124,   125,   126,   127,   555,   556,   128,   557,   558,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,  1032,  1033,  1034,   122,   123,   124,
     125,   126,   127,   559,  1040,   128,   560,   561,   562,   587,
    1046,  1047,   588,  1048,  1049,  1050,   589,   590,   597,   602,
    1167,  1056,  1057,   861,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,   862,  1069,  1070,  1071,  1072,  1073,
    1074,   603,  1076,   610,  1078,  1079,  1080,  1081,  1082,  1083,
     122,   123,   124,   125,   126,   127,   606,   607,   128,   608,
     867,   609,   614,   616,   620,   623,   658,  1099,   666,  1101,
    1102,  1103,   667,   868,  1106,  1107,   668,   122,   123,   124,
     125,   126,   127,   672,  1114,   128,   122,   123,   124,   125,
     126,   127,   673,   674,   128,   869,   679,   680,   122,   123,
     124,   125,   126,   127,   870,   684,   128,   686,   687,  1139,
     688,   122,   123,   124,   125,   126,   127,  1009,   689,   128,
     122,   123,   124,   125,   126,   127,   690,   691,   128,   694,
     696,   122,   123,   124,   125,   126,   127,   700,   703,   128,
     720,   721,   722,   723,   724,   725,   726,  1175,  1176,  1177,
     727,   728,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,
    1188,  1189,   122,   123,   124,   125,   126,   127,   729,   730,
     128,   731,   732,   733,   734,  1203,  1204,  1205,  1206,  1207,
    1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,   735,  1222,   736,   737,  1225,   743,   738,
     739,  1228,  1229,   740,  1231,  1232,  1135,   741,   742,   744,
    1237,  1238,   122,   123,   124,   125,   126,   127,   747,   752,
     128,   754,   755,  1250,  1251,  1252,  1253,   751,   756,  1256,
     758,   760,  1259,  1260,  1136,   759,   765,   122,   123,   124,
     125,   126,   127,  1138,   761,   128,   122,   123,   124,   125,
     126,   127,  1452,   763,   128,  1282,  1283,   769,   770,  1286,
     772,  1552,  1288,   816,   774,  1291,   778,   781,   817,   818,
     819,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,
    1305,  1306,   820,   122,   123,   124,   125,   126,   127,   821,
    1315,   128,  1555,  1318,  1319,  1320,  1321,   122,   123,   124,
     125,   126,   127,   822,   823,   128,   122,   123,   124,   125,
     126,   127,   824,  1338,   128,   122,   123,   124,   125,   126,
     127,   826,  1346,   128,  1348,  1349,  1350,   827,   828,   829,
     830,  1559,   831,   122,   123,   124,   125,   126,   127,  1453,
    1568,   128,   834,  1367,  1368,   835,  1370,   841,   894,   847,
     848,   850,   854,   857,  1377,  1378,  1379,  1380,  1381,  1382,
    1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,   895,
     896,   904,   914,  1397,   910,  1399,  1400,  1401,  1735,   916,
     917,   918,   922,  1404,  1405,  1406,  1407,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1768,  1416,  1417,  1418,  1419,   932,
     924,  1422,   928,   931,  1425,  1426,  1427,  1428,  1429,   122,
     123,   124,   125,   126,   127,   934,   933,   128,   935,   941,
     942,   936,  1778,   944,   943,  1446,   948,  1448,  1449,   949,
     950,   951,   952,   953,  1454,  1455,   122,   123,   124,   125,
     126,   127,   954,   955,   128,   956,   957,  1466,    54,   958,
      55,    56,    57,    58,    59,    60,  1474,   122,   123,   124,
     125,   126,   127,    61,   959,   128,   122,   123,   124,   125,
     126,   127,   960,   961,   128,  1794,   962,  1495,   122,   123,
     124,   125,   126,   127,   963,   964,   128,   972,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,   978,   977,
     979,   981,  1828,  1519,  1520,  1521,  1289,   982,   983,  1524,
    1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,
    1535,  1536,  1537,  1538,  1539,  1540,  1541,   989,  1543,   984,
    1545,   990,   992,  1548,  1549,  1550,  1882,   994,   122,   123,
     124,   125,   126,   127,   998,  1895,   128,  1001,  1563,  1564,
    1565,  1003,  1004,  1005,  1566,  1567,  1950,   122,   123,   124,
     125,   126,   127,  1006,  1011,   128,  1051,   122,   123,   124,
     125,   126,   127,  1012,  1584,   128,  1586,  1013,  1587,  1588,
    1014,  1035,  1036,  1590,  1038,  1592,  1593,  2013,  1595,  1596,
    1597,  1039,  1599,  1600,  1601,   122,   123,   124,   125,   126,
     127,  1608,  1609,   128,  1041,  1075,   122,   123,   124,   125,
     126,   127,  1042,  1043,   128,  1052,  1077,  1624,  1100,  1113,
    1105,  1628,   122,   123,   124,   125,   126,   127,  1108,  1115,
     128,   122,   123,   124,   125,   126,   127,  2017,  1116,   128,
    1117,  1137,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,
    1657,  1658,  1659,  1660,  1661,  1662,  1146,  1664,  1130,  1147,
    1148,  1668,  2078,  1670,  1671,  1149,  1673,  1674,  1675,  1150,
    1677,  2079,  1679,  1680,  1681,  1682,  1683,  1684,  1151,  1686,
    1687,  1152,  1190,   122,   123,   124,   125,   126,   127,  1153,
    1154,   128,  1155,  1156,  1157,  1697,  1158,  1699,  1700,  1701,
    1159,  1703,  1160,  1165,  1706,  1168,  1169,   695,  1710,  1173,
     122,   123,   124,   125,   126,   127,  1170,  1719,   128,  1171,
     681,  1172,  2080,  1178,  1179,  1191,  1192,  1193,  1730,  1194,
    1733,  2097,  1200,  1195,  1736,  1436,  1737,  1738,  1196,  1740,
    1741,  1742,  1201,  1744,  1745,  1746,  1197,  1221,  1223,  1198,
    1224,  1226,  1753,  1754,  1755,  1756,  1757,  1758,  1759,  1760,
    1761,  1762,  1202,  1764,  1230,   122,   123,   124,   125,   126,
     127,  1227,  1233,   128,  1258,  1280,  1254,  1249,  1255,  1779,
    1780,  1781,  1257,  1783,   122,   123,   124,   125,   126,   127,
    1261,  1285,   128,  1292,  1293,  1793,  1294,  1311,  1797,  1798,
    1312,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,  1808,
    1314,  1322,  1323,  1324,  1813,  1325,  1815,  1816,  1817,   122,
     123,   124,   125,   126,   127,  1326,  1327,   128,  1827,  1328,
    1329,  1330,  1331,  1332,  2193,   122,   123,   124,   125,   126,
     127,  1333,  1334,   128,  1335,  1336,  1337,  1341,  1343,  1345,
    1848,  1849,  1850,  1851,  1852,  1853,  1854,  1351,  1857,  1363,
    1364,  2197,  1859,  1860,  1365,  1862,  1863,  1864,  1366,  1866,
    1867,  1369,  1869,  1870,  1871,  1872,  1372,  1373,  1875,  1876,
    1392,  1395,  2199,  1396,  1398,  1403,  1415,  1424,  1884,  1447,
    1886,  2203,  1471,  1889,  1445,  1450,  1472,  1893,  1473,  1480,
    1896,  1481,  1482,  2208,  1483,  1901,  1902,  1544,   122,   123,
     124,   125,   126,   127,  1484,  1910,   128,  1911,  1485,  1912,
    1913,  1486,  1915,  1916,  1917,  1487,  1919,  1488,  1489,  1490,
    1491,  1492,  1925,  1493,  1494,  1928,  1929,  1930,  1931,  1932,
    1933,  1934,  1501,  1502,  1515,  1516,  1591,   122,   123,   124,
     125,   126,   127,  1946,  1517,   128,  1518,  1522,  1951,  1523,
    1546,  1547,  1561,  2261,  1610,  1562,  1611,  1958,  1959,  1960,
    1961,  1962,  1963,  1964,  1965,  1966,  1967,  1594,  1598,  1970,
    1971,  1972,  2262,  1607,  1974,  1975,  1976,  1612,  1613,  1614,
    1615,  1616,  2263,  1630,   122,   123,   124,   125,   126,   127,
    1990,  1991,   128,  1617,  1618,  1995,  1619,  1620,  1621,  1999,
    2000,  2001,  2002,  2003,  2004,  2005,  1622,  2007,  2008,  2009,
    2270,  2011,  1633,  1637,  2014,  2015,  2016,  1641,  1642,  2019,
    2020,  2279,   122,   123,   124,   125,   126,   127,  2028,  1643,
     128,  1644,  2032,  1645,  2034,  1646,  1665,  2309,  1666,  1669,
    2039,  1678,  1672,  1676,  1685,   697,  2045,  2046,  2047,  2048,
    1712,  2050,  2051,  2052,  1722,  2054,  1723,  1724,  2057,  2058,
    2059,  1725,  2061,  2062,  2063,  2064,  2065,   122,   123,   124,
     125,   126,   127,  1726,  1727,   128,  1728,  2076,  1765,   122,
     123,   124,   125,   126,   127,  1729,  1747,   128,  2087,  2088,
    2089,  2090,  2091,  2092,  2093,  2094,  2095,  2096,  2389,  2098,
    1748,  1749,  1750,  2102,  2103,  2104,  1799,   122,   123,   124,
     125,   126,   127,  1751,  1767,   128,   122,   123,   124,   125,
     126,   127,  1771,  1775,   128,  2390,  1782,  2126,  2127,  2128,
    2129,  2130,  2131,  2132,  1784,  2134,  2135,  2136,  1809,  2138,
    1818,  2140,  1819,  1820,  1821,  1822,  2145,  2146,   122,   123,
     124,   125,   126,   127,  1823,  2154,   128,  1826,  1825,  2158,
    1829,  1839,  1840,  2162,  2163,  2164,  2165,  1841,  1842,  1844,
    1858,  1861,  2171,  2172,  2173,  2174,  1865,  2176,  2177,  2178,
    2391,  2180,  2181,  2182,  1868,  1874,  1905,  1873,  2187,  2188,
    2189,  2190,  2191,   122,   123,   124,   125,   126,   127,  2405,
    1883,   128,  1906,  1907,  1908,  2204,  2205,  2206,  1909,  1922,
    1923,  1924,  1920,  1939,  2213,  1943,  2214,  2215,  2216,  1945,
    2218,  1947,  2220,  2221,  2222,  1952,  1977,  1978,  1948,  2226,
    1949,  2228,  1979,  1968,  2409,  2035,   122,   123,   124,   125,
     126,   127,  2006,  2010,   128,  2242,  2243,  2244,  2018,  2042,
    2451,  2248,  2249,  2250,  2251,  2252,  2253,  2043,  2044,  2256,
    2056,  2070,  2259,  2260,  2074,  2077,  2081,  2099,  2105,  2266,
     122,   123,   124,   125,   126,   127,  2273,  2274,   128,  2276,
    2277,  2100,  2101,  2106,  2107,  2118,  2283,  2284,  2120,  2121,
    2122,  2133,  2137,  2289,  2290,  2139,  2141,  2293,  2144,  2168,
    2295,  2169,  2142,  2143,  2160,  2170,  2196,  2301,  2302,  2303,
    2304,  2217,   122,   123,   124,   125,   126,   127,  2200,  2207,
     128,  2219,  2227,  2453,  2229,  2230,  2231,  2321,  2322,  2323,
    2324,  2236,  2326,  2327,  2328,  2329,  2330,  2331,  2332,  2333,
     122,   123,   124,   125,   126,   127,  2238,  2246,   128,  2254,
    2258,  2264,  2255,  2257,  2265,  2286,  2347,  2348,  2349,  2350,
    2351,   698,  2280,  2354,  2355,  2356,  2357,  2287,  2359,  2281,
    2282,  2288,  2296,  2297,  2364,  2365,  2298,  2307,  2308,  2310,
    2370,  2315,  2312,  1575,  2374,  2375,  2316,  2313,  2377,  2378,
    2379,  2380,  2314,  2382,  2383,  2384,  2325,  2353,   122,   123,
     124,   125,   126,   127,  2334,  2335,   128,  2396,  2397,  2398,
    2399,  2336,  2340,  2352,  2400,  2401,  2358,  2372,  2373,  2387,
    2392,  2402,  2403,  2413,  2410,  2411,  2412,  2414,  2416,  2415,
    2417,    94,    95,  2421,  2426,    96,  2422,    97,  2424,  2425,
    2427,    98,  2428,  2429,  2433,  2431,   699,  2438,  2434,  2435,
      99,  2437,  2440,  2449,  2450,  2441,  2455,  2458,  2444,  2459,
    2445,  2446,  2447,  2460,  2472,   122,   123,   124,   125,   126,
     127,  2456,  2457,   128,  2461,  2465,  2470,  2462,  2471,  2467,
    2474,  2466,  2476,  2468,  2480,  2482,  2489,  2490,  2493,  2496,
    2497,   701,  2477,  2478,  2479,  2504,  2481,  2505,  2483,  2508,
    2511,  2512,  2486,   702,  2487,   122,   123,   124,   125,   126,
     127,  2494,  2495,   128,  2519,  2498,  2520,  2521,  2501,  2502,
     122,   123,   124,   125,   126,   127,  2509,  2510,   128,  2527,
    2513,   704,  2528,  2516,  2517,  2533,  2534,  2539,  2540,  2545,
     705,  2522,  2523,  2524,  2546,  2550,     0,     0,     0,  2529,
    2530,     0,   441,   248,     0,  2535,  2536,     0,     0,     0,
     100,  2541,  2542,     0,     0,     0,     0,  2547,  2548,     0,
       0,     1,  2551,     2,     3,     4,     5,     0,     6,     7,
       8,     0,     0,     9,     0,     0,    10,     0,    11,     0,
      12,     0,     0,     0,  1271,     0,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,     0,     0,     0,
       0,     0,    21,     0,    22,    23,    24,    25,     0,     0,
      26,     0,    27,     0,    28,    29,     0,     0,     0,     2,
       3,     4,     5,     0,     6,     7,     8,     0,     0,     9,
       0,    30,    10,     0,    11,     0,    12,     0,    31,    32,
      33,    34,     0,    13,    14,    15,    16,    17,    18,    19,
     706,     0,    20,     0,     0,     0,    35,    36,    21,     0,
      22,    23,    24,    25,     0,     0,    26,     0,    27,    37,
      28,    29,     0,     0,     0,    38,     0,   122,   123,   124,
     125,   126,   127,     0,   707,   128,     0,    30,     0,     0,
       0,     0,     0,     0,    31,    32,    33,    34,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,     0,    35,    36,     0,     0,     0,     2,     3,     4,
       5,     0,     6,     7,     8,    37,   708,     9,     0,     0,
      10,    38,    11,     0,    12,     0,     0,     0,     0,     0,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      20,     0,     0,     0,   709,     0,    21,     0,    22,    23,
      24,    25,     0,     0,    26,     0,    27,     0,    28,    29,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,    30,     0,   128,     0,     0,
       0,     0,    31,    32,    33,    34,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,     0,
      35,    36,   710,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,    37,     0,     0,     0,     0,     0,    38,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,   711,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,   712,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,     0,   713,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
      39,     0,     0,    40,    41,     0,    42,     0,    43,     0,
       0,     0,     0,     0,    44,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,     0,    39,     0,     0,    40,
      41,     0,    42,     0,    43,     0,     0,     0,     0,     0,
      44,     0,   202,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   714,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,   715,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,     0,    39,     0,     0,    40,    41,     0,
      42,     0,    43,     0,     0,     0,     0,     0,    44,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,   716,     0,   128,   122,   123,   124,
     125,   126,   127,   717,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,     0,
     718,   122,   123,   124,   125,   126,   127,     0,     0,   128,
     122,   123,   124,   125,   126,   127,     0,   719,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,   849,     0,   128,   122,   123,   124,
     125,   126,   127,   851,     0,   128,   122,   123,   124,   125,
     126,   127,   852,     0,   128,   122,   123,   124,   125,   126,
     127,   853,     0,   128,   122,   123,   124,   125,   126,   127,
     855,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,   856,     0,   122,   123,   124,   125,
     126,   127,     0,   863,   128,   122,   123,   124,   125,   126,
     127,     0,   864,   128,   122,   123,   124,   125,   126,   127,
       0,   865,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,   866,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,     0,   871,
       0,   122,   123,   124,   125,   126,   127,     0,   872,   128,
     122,   123,   124,   125,   126,   127,     0,   873,   128,   122,
     123,   124,   125,   126,   127,     0,   993,   128,   122,   123,
     124,   125,   126,   127,     0,   995,   128,   122,   123,   124,
     125,   126,   127,     0,   996,   128,   122,   123,   124,   125,
     126,   127,     0,   997,   128,   122,   123,   124,   125,   126,
     127,     0,   999,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   122,   123,   124,   125,   126,   127,     0,  1000,
     128,   122,   123,   124,   125,   126,   127,     0,  1007,   128,
     122,   123,   124,   125,   126,   127,     0,  1008,   128,   122,
     123,   124,   125,   126,   127,     0,  1010,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,  1015,     0,   122,
     123,   124,   125,   126,   127,     0,  1016,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1017,   128,   122,   123,
     124,   125,   126,   127,     0,  1037,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1120,     0,   122,
     123,   124,   125,   126,   127,     0,  1121,   128,   122,   123,
     124,   125,   126,   127,     0,  1122,   128,   122,   123,   124,
     125,   126,   127,     0,  1123,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  1124,     0,   122,   123,   124,   125,   126,
     127,     0,  1125,   128,   122,   123,   124,   125,   126,   127,
       0,  1126,   128,   122,   123,   124,   125,   126,   127,     0,
    1127,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,  1128,     0,   122,   123,   124,
     125,   126,   127,     0,  1129,   128,   122,   123,   124,   125,
     126,   127,     0,  1143,   128,   122,   123,   124,   125,   126,
     127,     0,  1144,   128,   122,   123,   124,   125,   126,   127,
       0,  1145,   128,   122,   123,   124,   125,   126,   127,     0,
    1162,   128,   122,   123,   124,   125,   126,   127,     0,  1239,
     128,   122,   123,   124,   125,   126,   127,     0,  1240,   128,
     122,   123,   124,   125,   126,   127,     0,  1241,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    1242,     0,   122,   123,   124,   125,   126,   127,     0,  1243,
     128,   122,   123,   124,   125,   126,   127,     0,  1244,   128,
     122,   123,   124,   125,   126,   127,     0,  1245,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1246,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,  1247,     0,   128,   122,   123,
     124,   125,   126,   127,  1248,     0,   128,   122,   123,   124,
     125,   126,   127,  1262,     0,   128,   122,   123,   124,   125,
     126,   127,  1263,     0,   128,   122,   123,   124,   125,   126,
     127,  1264,     0,   128,   122,   123,   124,   125,   126,   127,
    1281,     0,   128,   122,   123,   124,   125,   126,   127,  1290,
       0,   128,   122,   123,   124,   125,   126,   127,  1344,     0,
     128,   122,   123,   124,   125,   126,   127,  1352,     0,   128,
     122,   123,   124,   125,   126,   127,  1353,     0,   128,   122,
     123,   124,   125,   126,   127,  1354,     0,   128,   122,   123,
     124,   125,   126,   127,  1355,     0,   128,   122,   123,   124,
     125,   126,   127,  1356,     0,   128,   122,   123,   124,   125,
     126,   127,  1357,     0,   128,   122,   123,   124,   125,   126,
     127,  1358,     0,   128,   122,   123,   124,   125,   126,   127,
    1359,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1360,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  1361,     0,   122,   123,   124,   125,   126,
     127,     0,  1374,   128,   122,   123,   124,   125,   126,   127,
       0,  1375,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1376,     0,   122,   123,   124,   125,   126,
     127,     0,  1402,   128,   122,   123,   124,   125,   126,   127,
       0,  1456,   128,   122,   123,   124,   125,   126,   127,     0,
    1457,   128,   122,   123,   124,   125,   126,   127,     0,  1458,
     128,   122,   123,   124,   125,   126,   127,     0,  1459,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1460,
       0,   122,   123,   124,   125,   126,   127,     0,  1461,   128,
     122,   123,   124,   125,   126,   127,     0,  1462,   128,   122,
     123,   124,   125,   126,   127,     0,  1463,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  1464,     0,   122,   123,   124,   125,   126,   127,     0,
    1465,   128,   122,   123,   124,   125,   126,   127,     0,  1477,
     128,   122,   123,   124,   125,   126,   127,     0,  1478,   128,
     122,   123,   124,   125,   126,   127,     0,  1479,   128,   122,
     123,   124,   125,   126,   127,     0,  1496,   128,   122,   123,
     124,   125,   126,   127,     0,  1503,   128,   122,   123,   124,
     125,   126,   127,     0,  1542,   128,   122,   123,   124,   125,
     126,   127,     0,  1551,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  1553,     0,   122,   123,
     124,   125,   126,   127,     0,  1554,   128,   122,   123,   124,
     125,   126,   127,     0,  1556,   128,   122,   123,   124,   125,
     126,   127,     0,  1557,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1558,     0,   122,   123,   124,   125,
     126,   127,     0,  1560,   128,   122,   123,   124,   125,   126,
     127,     0,  1569,   128,   122,   123,   124,   125,   126,   127,
       0,  1570,   128,   122,   123,   124,   125,   126,   127,     0,
    1585,   128,   122,   123,   124,   125,   126,   127,     0,  1589,
     128,   122,   123,   124,   125,   126,   127,     0,  1623,   128,
     122,   123,   124,   125,   126,   127,     0,  1625,   128,   122,
     123,   124,   125,   126,   127,     0,  1629,   128,   122,   123,
     124,   125,   126,   127,     0,  1631,   128,   122,   123,   124,
     125,   126,   127,     0,  1632,   128,   122,   123,   124,   125,
     126,   127,     0,  1634,   128,   122,   123,   124,   125,   126,
     127,     0,  1635,   128,   122,   123,   124,   125,   126,   127,
       0,  1636,   128,   122,   123,   124,   125,   126,   127,     0,
    1638,   128,   122,   123,   124,   125,   126,   127,     0,  1640,
     128,   122,   123,   124,   125,   126,   127,     0,  1647,   128,
     122,   123,   124,   125,   126,   127,     0,  1648,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  1663,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1667,
       0,   122,   123,   124,   125,   126,   127,     0,  1696,   128,
     122,   123,   124,   125,   126,   127,     0,  1698,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,  1702,
       0,   122,   123,   124,   125,   126,   127,     0,  1704,   128,
     122,   123,   124,   125,   126,   127,     0,  1705,   128,   122,
     123,   124,   125,   126,   127,     0,  1707,   128,   122,   123,
     124,   125,   126,   127,     0,  1708,   128,   122,   123,   124,
     125,   126,   127,     0,  1709,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  1711,     0,   122,   123,   124,
     125,   126,   127,     0,  1713,   128,   122,   123,   124,   125,
     126,   127,     0,  1720,   128,   122,   123,   124,   125,   126,
     127,     0,  1721,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,     0,     0,  1763,     0,   122,
     123,   124,   125,   126,   127,     0,  1766,   128,   122,   123,
     124,   125,   126,   127,     0,  1769,   128,   122,   123,   124,
     125,   126,   127,     0,  1770,   128,   122,   123,   124,   125,
     126,   127,     0,  1772,   128,   122,   123,   124,   125,   126,
     127,     0,  1773,   128,   122,   123,   124,   125,   126,   127,
       0,  1774,   128,   122,   123,   124,   125,   126,   127,     0,
    1776,   128,   122,   123,   124,   125,   126,   127,     0,  1777,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,  1785,     0,   122,   123,   124,   125,   126,   127,
       0,  1786,   128,   122,   123,   124,   125,   126,   127,     0,
    1824,   128,   122,   123,   124,   125,   126,   127,     0,  1830,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
    1831,     0,   122,   123,   124,   125,   126,   127,     0,  1833,
     128,   122,   123,   124,   125,   126,   127,     0,  1834,   128,
     122,   123,   124,   125,   126,   127,     0,  1835,   128,   122,
     123,   124,   125,   126,   127,     0,  1837,   128,   122,   123,
     124,   125,   126,   127,     0,  1838,   128,   122,   123,   124,
     125,   126,   127,     0,  1843,   128,   122,   123,   124,   125,
     126,   127,     0,  1846,   128,   122,   123,   124,   125,   126,
     127,     0,  1847,   128,   122,   123,   124,   125,   126,   127,
       0,  1885,   128,   122,   123,   124,   125,   126,   127,     0,
    1887,   128,   122,   123,   124,   125,   126,   127,     0,  1888,
     128,   122,   123,   124,   125,   126,   127,     0,  1890,   128,
     122,   123,   124,   125,   126,   127,     0,  1891,   128,   122,
     123,   124,   125,   126,   127,     0,  1892,   128,   122,   123,
     124,   125,   126,   127,     0,  1894,   128,   122,   123,   124,
     125,   126,   127,     0,  1900,   128,   122,   123,   124,   125,
     126,   127,     0,  1903,   128,     0,   122,   123,   124,   125,
     126,   127,     0,  1904,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  1921,     0,   122,   123,   124,
     125,   126,   127,     0,  1926,   128,   122,   123,   124,   125,
     126,   127,     0,  1935,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  1936,     0,   122,   123,   124,
     125,   126,   127,     0,  1937,   128,   122,   123,   124,   125,
     126,   127,     0,  1938,   128,   122,   123,   124,   125,   126,
     127,     0,  1940,   128,   122,   123,   124,   125,   126,   127,
       0,  1941,   128,   122,   123,   124,   125,   126,   127,     0,
    1942,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,  1944,     0,   122,   123,   124,   125,   126,   127,     0,
    1953,   128,   122,   123,   124,   125,   126,   127,     0,  1954,
     128,   122,   123,   124,   125,   126,   127,     0,  1969,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,  1973,     0,   122,   123,   124,   125,   126,
     127,     0,  1980,   128,   122,   123,   124,   125,   126,   127,
       0,  1981,   128,   122,   123,   124,   125,   126,   127,     0,
    1982,   128,   122,   123,   124,   125,   126,   127,     0,  1983,
     128,   122,   123,   124,   125,   126,   127,     0,  1985,   128,
     122,   123,   124,   125,   126,   127,     0,  1986,   128,   122,
     123,   124,   125,   126,   127,     0,  1987,   128,   122,   123,
     124,   125,   126,   127,     0,  1989,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  1992,     0,
     122,   123,   124,   125,   126,   127,     0,  1993,   128,   122,
     123,   124,   125,   126,   127,     0,  1994,   128,   122,   123,
     124,   125,   126,   127,     0,  1997,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1998,     0,   122,   123,
     124,   125,   126,   127,     0,  2012,   128,   122,   123,   124,
     125,   126,   127,     0,  2024,   128,   122,   123,   124,   125,
     126,   127,     0,  2025,   128,   122,   123,   124,   125,   126,
     127,     0,  2026,   128,   122,   123,   124,   125,   126,   127,
       0,  2027,   128,   122,   123,   124,   125,   126,   127,     0,
    2029,   128,   122,   123,   124,   125,   126,   127,     0,  2030,
     128,   122,   123,   124,   125,   126,   127,     0,  2031,   128,
     122,   123,   124,   125,   126,   127,     0,  2033,   128,   122,
     123,   124,   125,   126,   127,     0,  2036,   128,   122,   123,
     124,   125,   126,   127,     0,  2037,   128,   122,   123,   124,
     125,   126,   127,     0,  2038,   128,   122,   123,   124,   125,
     126,   127,     0,  2040,   128,   122,   123,   124,   125,   126,
     127,     0,  2041,   128,   122,   123,   124,   125,   126,   127,
       0,  2055,   128,   122,   123,   124,   125,   126,   127,     0,
    2066,   128,     0,   122,   123,   124,   125,   126,   127,     0,
    2067,   128,   122,   123,   124,   125,   126,   127,     0,  2068,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2069,     0,   122,   123,   124,   125,   126,   127,     0,
    2071,   128,   122,   123,   124,   125,   126,   127,     0,  2072,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  2073,     0,   122,   123,   124,   125,   126,   127,     0,
    2075,   128,   122,   123,   124,   125,   126,   127,     0,  2082,
     128,   122,   123,   124,   125,   126,   127,     0,  2083,   128,
     122,   123,   124,   125,   126,   127,     0,  2108,   128,   122,
     123,   124,   125,   126,   127,     0,  2109,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,  2110,     0,   122,
     123,   124,   125,   126,   127,     0,  2111,   128,   122,   123,
     124,   125,   126,   127,     0,  2113,   128,   122,   123,   124,
     125,   126,   127,     0,  2114,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,  2115,
       0,   122,   123,   124,   125,   126,   127,     0,  2117,   128,
     122,   123,   124,   125,   126,   127,     0,  2119,   128,   122,
     123,   124,   125,   126,   127,     0,  2124,   128,   122,   123,
     124,   125,   126,   127,     0,  2125,   128,   122,   123,   124,
     125,   126,   127,     0,  2150,   128,   122,   123,   124,   125,
     126,   127,     0,  2151,   128,   122,   123,   124,   125,   126,
     127,     0,  2152,   128,   122,   123,   124,   125,   126,   127,
       0,  2153,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,  2155,     0,   122,   123,   124,   125,
     126,   127,     0,  2156,   128,   122,   123,   124,   125,   126,
     127,     0,  2157,   128,   122,   123,   124,   125,   126,   127,
       0,  2159,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,  2161,     0,   122,   123,   124,   125,   126,   127,
       0,  2166,   128,   122,   123,   124,   125,   126,   127,     0,
    2167,   128,   122,   123,   124,   125,   126,   127,     0,  2183,
     128,   122,   123,   124,   125,   126,   127,     0,  2184,   128,
     122,   123,   124,   125,   126,   127,     0,  2185,   128,   122,
     123,   124,   125,   126,   127,     0,  2192,   128,   122,   123,
     124,   125,   126,   127,     0,  2194,   128,   122,   123,   124,
     125,   126,   127,     0,  2195,   128,   122,   123,   124,   125,
     126,   127,     0,  2198,   128,   122,   123,   124,   125,   126,
     127,     0,  2201,   128,   122,   123,   124,   125,   126,   127,
       0,  2202,   128,   122,   123,   124,   125,   126,   127,     0,
    2209,   128,   122,   123,   124,   125,   126,   127,     0,  2223,
     128,   122,   123,   124,   125,   126,   127,     0,  2224,   128,
     122,   123,   124,   125,   126,   127,     0,  2225,   128,     0,
     122,   123,   124,   125,   126,   127,     0,  2232,   128,   122,
     123,   124,   125,   126,   127,     0,  2233,   128,   122,   123,
     124,   125,   126,   127,     0,  2234,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  2237,     0,   122,
     123,   124,   125,   126,   127,     0,  2240,   128,   122,   123,
     124,   125,   126,   127,     0,  2241,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  2247,     0,   122,
     123,   124,   125,   126,   127,     0,  2271,   128,   122,   123,
     124,   125,   126,   127,     0,  2272,   128,   122,   123,   124,
     125,   126,   127,     0,  2275,   128,   122,   123,   124,   125,
     126,   127,     0,  2278,   128,   122,   123,   124,   125,   126,
     127,     0,  2285,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  2305,     0,     0,     0,    75,    76,     0,
      77,    78,  2306,    79,    80,    81,     0,    82,    83,    84,
       0,  2311,     0,   122,   123,   124,   125,   126,   127,    85,
    2317,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,     0,     0,  2337,     0,   122,   123,   124,
     125,   126,   127,     0,  2338,   128,   122,   123,   124,   125,
     126,   127,     0,  2341,   128,   122,   123,   124,   125,   126,
     127,     0,  2342,   128,   122,   123,   124,   125,   126,   127,
       0,  2343,   128,   122,   123,   124,   125,   126,   127,     0,
    2344,   128,   122,   123,   124,   125,   126,   127,     0,  2346,
     128,   122,   123,   124,   125,   126,   127,     0,  2362,   128,
     122,   123,   124,   125,   126,   127,     0,  2363,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
    2366,     0,   122,   123,   124,   125,   126,   127,     0,  2367,
     128,     0,     0,     0,    86,     0,     0,     0,  2368,     0,
     122,   123,   124,   125,   126,   127,     0,  2369,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  2371,     0,
     122,   123,   124,   125,   126,   127,     0,  2385,   128,   122,
     123,   124,   125,   126,   127,     0,  2386,   128,   122,   123,
     124,   125,   126,   127,     0,  2388,   128,   122,   123,   124,
     125,   126,   127,     0,  2393,   128,     0,     0,     0,     0,
      87,     0,     0,  2404,     0,   122,   123,   124,   125,   126,
     127,     0,  2407,   128,   122,   123,   124,   125,   126,   127,
       0,  2420,   128,   122,   123,   124,   125,   126,   127,     0,
    2423,   128,     0,   122,   123,   124,   125,   126,   127,  2436,
    1112,   128,  1090,     0,     0,     0,     0,     0,  2439,     0,
    1880,   122,   123,   124,   125,   126,   127,     0,  1435,   128,
       0,   122,   123,   124,   125,   126,   127,     0,  1790,   128,
     122,   123,   124,   125,   126,   127,   974,     0,   128,   122,
     123,   124,   125,   126,   127,   975,     0,   128,   122,   123,
     124,   125,   126,   127,  1270,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1691,   122,   123,   124,
     125,   126,   127,     0,   973,   128,   122,   123,   124,   125,
     126,   127,     0,  1166,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  1342,   122,   123,   124,   125,
     126,   127,     0,   748,   128,   122,   123,   124,   125,   126,
     127,     0,   749,   128,   122,   123,   124,   125,   126,   127,
       0,   750,   128,   122,   123,   124,   125,   126,   127,     0,
    1089,   128,     0,   122,   123,   124,   125,   126,   127,  1576,
      88,   128,   122,   123,   124,   125,   126,   127,  1269,     0,
     128,     0,     0,     0,   525,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,   685,     0,
       0,     0,     0,     0,   443,     0,   122,   123,   124,   125,
     126,   127,     0,   913,   128,   122,   123,   124,   125,   126,
     127,   682,     0,   128,     0,     0,     0,     0,     0,   757,
     122,   123,   124,   125,   126,   127,     0,   520,   128,   122,
     123,   124,   125,   126,   127,   263,     0,   128,     0,     0,
       0,     0,     0,  1437,   122,   123,   124,   125,   126,   127,
       0,  1272,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1091,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,     0,
     883,   128,   122,   123,   124,   125,   126,   127,   650,     0,
     128,   122,   123,   124,   125,   126,   127,     0,  2318,   128,
     122,   123,   124,   125,   126,   127,  2267,     0,   128,     0,
       0,     0,     0,     0,  2210,   122,   123,   124,   125,   126,
     127,     0,  2147,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,   122,   123,   124,   125,   126,   127,  2084,
       0,   128,     0,     0,     0,     0,     0,  2021,   122,   123,
     124,   125,   126,   127,     0,  1955,   128,   122,   123,   124,
     125,   126,   127,     0,  1877,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,  1787,     0,   128,   122,   123,   124,   125,   126,   127,
    1688,     0,   128,     0,     0,     0,     0,     0,  1571,   122,
     123,   124,   125,   126,   127,     0,  1430,   128,   122,   123,
     124,   125,   126,   127,  1265,     0,   128,     0,     0,     0,
       0,     0,  1084,     0,   122,   123,   124,   125,   126,   127,
       0,   876,   128,     0,     0,     0,     0,  1574,     0,   642,
     122,   123,   124,   125,   126,   127,     0,  1434,   128,   122,
     123,   124,   125,   126,   127,  1088,     0,   128,     0,     0,
       0,     0,     0,   880,   122,   123,   124,   125,   126,   127,
       0,   646,   128,   122,   123,   124,   125,   126,   127,   881,
       0,   128,   122,   123,   124,   125,   126,   127,   882,     0,
     128,     0,     0,     0,     0,  2395,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  2361,   122,   123,   124,   125,
     126,   127,     0,  2320,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  2269,   122,   123,
     124,   125,   126,   127,     0,  2212,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2149,
     122,   123,   124,   125,   126,   127,     0,  2086,   128,     0,
     122,   123,   124,   125,   126,   127,     0,  2023,   128,   122,
     123,   124,   125,   126,   127,  1957,     0,   128,     0,     0,
       0,     0,     0,  1879,   122,   123,   124,   125,   126,   127,
       0,  1789,   128,   122,   123,   124,   125,   126,   127,  1690,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  1573,   122,   123,   124,   125,   126,   127,
       0,  1432,   128,   122,   123,   124,   125,   126,   127,   878,
       0,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,   643,     0,   122,   123,   124,   125,   126,
     127,     0,   644,   128,   122,   123,   124,   125,   126,   127,
    1267,     0,   128,     0,     0,     0,     0,   877,   122,   123,
     124,   125,   126,   127,     0,  2514,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2499,
     122,   123,   124,   125,   126,   127,     0,  2484,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,  2463,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,     0,  2442,   122,   123,
     124,   125,   126,   127,     0,  2418,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2394,
     122,   123,   124,   125,   126,   127,     0,  2360,   128,   122,
     123,   124,   125,   126,   127,  2319,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2268,   122,
     123,   124,   125,   126,   127,     0,  2211,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  2148,   128,   122,   123,
     124,   125,   126,   127,  2085,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  2022,   122,   123,   124,
     125,   126,   127,     0,  1956,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  1878,     0,   122,   123,
     124,   125,   126,   127,     0,  1788,   128,   122,   123,   124,
     125,   126,   127,  1689,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,  1572,   122,   123,
     124,   125,   126,   127,     0,  1085,   128,   122,   123,   124,
     125,   126,   127,  1431,     0,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  1266,   128,     0,     0,     0,  1693,
       0,   122,   123,   124,   125,   126,   127,     0,  1792,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
    1791,     0,   122,   123,   124,   125,   126,   127,     0,  1881,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,   879,   122,   123,   124,   125,   126,   127,     0,   645,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,  1433,   122,   123,   124,   125,   126,   127,     0,  1268,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,  1087,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,  1273,     0,   122,   123,
     124,   125,   126,   127,     0,  1275,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  1276,     0,
     122,   123,   124,   125,   126,   127,     0,  1277,   128,   122,
     123,   124,   125,   126,   127,  1438,     0,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1440,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1441,   128,   122,   123,
     124,   125,   126,   127,     0,  1442,   128,   122,   123,   124,
     125,   126,   127,     0,  1578,   128,     0,   122,   123,   124,
     125,   126,   127,     0,  1582,   128,   122,   123,   124,   125,
     126,   127,     0,  1583,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1579,     0,   122,   123,   124,   125,
     126,   127,     0,  1580,   128,   122,   123,   124,   125,   126,
     127,     0,  1581,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  1692,     0,   122,   123,   124,   125,
     126,   127,     0,  1694,   128,     0,   122,   123,   124,   125,
     126,   127,     0,  1695,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,  2419,   122,   123,
     124,   125,   126,   127,     0,  1439,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,  1443,     0,   122,
     123,   124,   125,   126,   127,     0,  1444,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1274,   128,   122,   123,
     124,   125,   126,   127,     0,  1278,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1279,   122,   123,   124,
     125,   126,   127,     0,  1094,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  1095,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,     0,
       0,  1096,     0,   122,   123,   124,   125,   126,   127,     0,
    1097,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  1098,   122,   123,   124,   125,   126,   127,
    1092,     0,   128,   122,   123,   124,   125,   126,   127,     0,
    1093,   128,   122,   123,   124,   125,   126,   127,     0,   676,
     128,   122,   123,   124,   125,   126,   127,     0,   874,   128,
     122,   123,   124,   125,   126,   127,     0,   875,   128,     0,
     122,   123,   124,   125,   126,   127,     0,   884,   128,   122,
     123,   124,   125,   126,   127,     0,   885,   128,   122,   123,
     124,   125,   126,   127,     0,   886,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   887,     0,   122,   123,
     124,   125,   126,   127,     0,   888,   128,   122,   123,   124,
     125,   126,   127,     0,   889,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,   890,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1086,     0,   122,   123,
     124,   125,   126,   127,     0,  1577,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,   647,   122,   123,
     124,   125,   126,   127,     0,   640,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,   641,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   648,     0,   122,
     123,   124,   125,   126,   127,     0,   649,   128,   122,   123,
     124,   125,   126,   127,     0,   651,   128,     0,   122,   123,
     124,   125,   126,   127,     0,   652,   128,   122,   123,   124,
     125,   126,   127,     0,   653,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   654,   122,   123,   124,   125,
     126,   127,     0,     0,   128,   122,   123,   124,   125,   126,
     127,   655,     0,   128,   122,   123,   124,   125,   126,   127,
     656,     0,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   657,     0,   122,   123,   124,   125,   126,   127,
       0,  1132,   128,   122,   123,   124,   125,   126,   127,     0,
    1133,   128,   122,   123,   124,   125,   126,   127,     0,  1134,
     128,   122,   123,   124,   125,   126,   127,  1109,     0,   128,
     122,   123,   124,   125,   126,   127,     0,  1307,   128,   122,
     123,   124,   125,   126,   127,     0,  1362,   128,   122,   123,
     124,   125,   126,   127,     0,  1371,   128,   122,   123,   124,
     125,   126,   127,     0,  1810,   128,     0,     0,     0,  1131,
       0,     0,     0,     0,     0,  1811,     0,     0,   907,     0,
       0,     0,     0,     0,  1812,     0,     0,     0,     0,  1313,
       0,   122,   123,   124,   125,   126,   127,     0,  1420,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1421,
       0,   122,   123,   124,   125,   126,   127,     0,  1423,   128,
     122,   123,   124,   125,   126,   127,     0,  1111,   128,     0,
       0,     0,  1717,     0,     0,   911,   122,   123,   124,   125,
     126,   127,  1814,     0,   128,     0,     0,  1467,     0,   122,
     123,   124,   125,   126,   127,     0,  1639,   128,     0,   122,
     123,   124,   125,   126,   127,     0,  1897,   128,   122,   123,
     124,   125,   126,   127,     0,  1898,   128,   122,   123,   124,
     125,   126,   127,     0,  1899,   128,     0,     0,     0,     0,
       0,  1605,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  1718,   122,   123,   124,   125,   126,   127,     0,
    1475,   128,   122,   123,   124,   125,   126,   127,     0,  1606,
     128,   122,   123,   124,   125,   126,   127,  1316,     0,   128,
     122,   123,   124,   125,   126,   127,  1476,     0,   128,     0,
       0,     0,     0,     0,  1140,     0,   122,   123,   124,   125,
     126,   127,     0,  1317,   128,     0,     0,     0,     0,  1514,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,     0,  1110,     0,   122,   123,   124,   125,   126,   127,
       0,  1602,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,  1603,     0,   122,   123,   124,   125,   126,   127,
       0,  1604,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,     0,   908,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1714,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1715,
       0,   122,   123,   124,   125,   126,   127,     0,  1716,   128,
     122,   123,   124,   125,   126,   127,  1451,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  1468,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,  1469,     0,   122,   123,   124,   125,   126,   127,
       0,  1470,   128,   122,   123,   124,   125,   126,   127,  1287,
       0,   128,   122,   123,   124,   125,   126,   127,  1308,     0,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,     0,  1309,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,     0,     0,  1310,   122,
     123,   124,   125,   126,   127,     0,   677,   128,   122,   123,
     124,   125,   126,   127,     0,  1199,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  2291,
       0,   122,   123,   124,   125,   126,   127,     0,  2294,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  2299,
       0,   122,   123,   124,   125,   126,   127,     0,  2506,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  2507,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,     0,     0,  2544,   122,   123,   124,   125,
     126,   127,     0,     0,   128,  2235,     0,   122,   123,   124,
     125,   126,   127,     0,  2239,   128,   122,   123,   124,   125,
     126,   127,     0,  2245,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,     0,  2491,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  2492,     0,
     122,   123,   124,   125,   126,   127,     0,  2538,   128,   122,
     123,   124,   125,   126,   127,  2175,     0,   128,   122,   123,
     124,   125,   126,   127,  2179,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,  2186,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,  2473,     0,   122,   123,   124,   125,   126,   127,
       0,  2475,   128,   122,   123,   124,   125,   126,   127,     0,
    2532,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,  2112,     0,   122,   123,   124,   125,   126,
     127,     0,  2116,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,  2123,     0,   122,   123,   124,   125,   126,
     127,     0,  2452,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,     0,  2454,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,  2526,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  2049,     0,
     122,   123,   124,   125,   126,   127,     0,  2053,   128,   122,
     123,   124,   125,   126,   127,     0,  2060,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2430,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,  2432,     0,   122,   123,   124,   125,   126,   127,     0,
    2518,   128,   122,   123,   124,   125,   126,   127,  1984,     0,
     128,   122,   123,   124,   125,   126,   127,  1988,     0,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,  1996,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,     0,     0,  2406,     0,   122,   123,   124,   125,
     126,   127,     0,  2408,   128,   122,   123,   124,   125,   126,
     127,     0,  2503,   128,   122,   123,   124,   125,   126,   127,
       0,     0,   128,     0,  1914,     0,   122,   123,   124,   125,
     126,   127,     0,  1918,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,  1927,     0,   122,   123,   124,   125,
     126,   127,     0,  2376,   128,   122,   123,   124,   125,   126,
     127,     0,     0,   128,     0,  2381,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,  2488,   122,
     123,   124,   125,   126,   127,     0,     0,   128,  1832,     0,
     122,   123,   124,   125,   126,   127,     0,  1836,   128,   122,
     123,   124,   125,   126,   127,     0,  1845,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,  2339,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,     0,
       0,  2345,     0,   122,   123,   124,   125,   126,   127,     0,
    2469,   128,   122,   123,   124,   125,   126,   127,  1739,     0,
     128,   122,   123,   124,   125,   126,   127,  1743,     0,   128,
     122,   123,   124,   125,   126,   127,     0,  1752,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,     0,
       0,     0,     0,  2292,     0,     0,  1141,     0,     0,   537,
       0,     0,  2300,     0,   122,   123,   124,   125,   126,   127,
     539,  2448,   128,   122,   123,   124,   125,   126,   127,   543,
       0,   128,     0,   122,   123,   124,   125,   126,   127,     0,
     546,   128,     0,     0,     0,     0,     0,  2515,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,  2549,   122,
     123,   124,   125,   126,   127,     0,  2500,   128,   122,   123,
     124,   125,   126,   127,     0,  2543,   128,   122,   123,   124,
     125,   126,   127,     0,  2485,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,  2537,
     122,   123,   124,   125,   126,   127,     0,  2464,   128,   122,
     123,   124,   125,   126,   127,  1234,  2531,   128,   122,   123,
     124,   125,   126,   127,  1235,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,     0,     0,     0,  1236,
       0,   122,   123,   124,   125,   126,   127,     0,  1295,   128,
       0,   122,   123,   124,   125,   126,   127,     0,  1347,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,     0,  1053,   122,   123,   124,   125,   126,   127,     0,
       0,   128,     0,  1054,     0,   122,   123,   124,   125,   126,
     127,     0,  1055,   128,   122,   123,   124,   125,   126,   127,
       0,  1118,   128,   122,   123,   124,   125,   126,   127,     0,
    1174,   128,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,     0,   842,     0,   122,   123,   124,   125,   126,
     127,     0,   843,   128,   122,   123,   124,   125,   126,   127,
       0,   844,   128,   122,   123,   124,   125,   126,   127,     0,
     919,   128,   122,   123,   124,   125,   126,   127,     0,   985,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,     0,     0,  1104,     0,     0,     0,     0,     0,   840,
       0,   122,   123,   124,   125,   126,   127,     0,   966,   128,
     122,   123,   124,   125,   126,   127,     0,     0,   128,     0,
       0,  1161,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,   343,   122,   123,   124,   125,   126,   127,  1340,
       0,   128,   122,   123,   124,   125,   126,   127,  1499,     0,
     128,     0,     0,     0,     0,     0,  1284,   122,   123,   124,
     125,   126,   127,     0,     0,   128,     0,     0,     0,  1627,
       0,   122,   123,   124,   125,   126,   127,     0,  1734,   128,
     122,   123,   124,   125,   126,   127,   971,     0,   128,   122,
     123,   124,   125,   126,   127,  1163,  1500,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,   122,   123,   124,
     125,   126,   127,     0,     0,   128,  1626,   122,   123,   124,
     125,   126,   127,     0,  1164,   128,   122,   123,   124,   125,
     126,   127,     0,     0,   128,     0,  1339,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   970,   122,   123,   124,
     125,   126,   127,     0,   592,   128,     0,   122,   123,   124,
     125,   126,   127,   257,     0,   128,     0,     0,     0,   605,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   258,
       0,     0,     0,   900,     0,     0,     0,     0,   252,     0,
     122,   123,   124,   125,   126,   127,     0,   253,   128,   122,
     123,   124,   125,   126,   127,     0,   262,   128,     0,   122,
     123,   124,   125,   126,   127,     0,   582,   128,   122,   123,
     124,   125,   126,   127,     0,   583,   128,     0,   122,   123,
     124,   125,   126,   127,     0,   185,   128,     0,     0,   122,
     123,   124,   125,   126,   127,   246,   513,   128,   122,   123,
     124,   125,   126,   127,     0,     0,   128,     0,     0,   526,
     122,   123,   124,   125,   126,   127,     0,     0,   128,   122,
     123,   124,   125,   126,   127,     0,     0,   128,     0,   585,
       0,   122,   123,   124,   125,   126,   127,     0,  1044,   128,
     122,   123,   124,   125,   126,   127,     0,  1068,   128,     0,
       0,   254,   122,   123,   124,   125,   126,   127,     0,     0,
     128,   255,   122,   123,   124,   125,   126,   127,     0,     0,
     128,     0,   256,     0,   122,   123,   124,   125,   126,   127,
       0,   599,   128,   122,   123,   124,   125,   126,   127,     0,
       0,   128,   675,     0,   122,   123,   124,   125,   126,   127,
       0,  1142,   128,   122,   123,   124,   125,   126,   127,     0,
     269,   128,   122,   123,   124,   125,   126,   127,     0,   270,
     128,   122,   123,   124,   125,   126,   127,     0,     0,   128,
       0,   991,   122,   123,   124,   125,   126,   127,     0,   591,
     128,     0,   122,   123,   124,   125,   126,   127,     0,   251,
     128,     0,     0,     0,     0,     0,  1119,   122,   123,   124,
     125,   126,   127,     0,     0,   128,   692,     0,     0,     0,
       0,     0,     0,     0,   920,     0,     0,     0,     0,  2443,
     965,     0,     0,     0,     0,     0,   265,     0,     0,  2525,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,     0,   307,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,     0,
       0,   317,   318,   319,   320,   321,   322,     0,     0,   323,
     324,   325,   326,   327,   328,   329,   330
  };

  const short int
  feiparser::yycheck_[] =
  {
       8,     9,    55,    35,    36,    37,    38,    39,    40,   458,
      66,    43,    20,    21,    22,   458,    35,    36,    37,    38,
      39,    40,   116,   270,    43,   458,    56,    35,    35,    36,
      37,    38,    39,    40,   130,    43,    43,    35,    36,    37,
      38,    39,    40,    40,   103,    43,    43,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     3,    35,    36,
      37,    38,    39,    40,   461,    73,    43,    55,    56,    53,
       3,     4,     5,     6,     0,     8,     9,   173,    51,   100,
     176,   177,    90,    35,    36,    37,    38,    39,    40,   148,
     450,    43,   113,   111,    27,    28,    29,    30,    31,    32,
      33,   119,    55,    36,   175,   113,    55,   178,   355,    42,
      72,   100,   128,   100,   122,   123,   124,   125,   126,   127,
     128,   131,    55,   111,   113,    51,   113,   426,   427,   428,
     450,    35,    36,    37,    38,    39,    40,   145,   146,    43,
     457,   149,   150,   464,   152,   153,   154,   155,   156,   205,
      98,   159,   160,   101,   162,     3,    89,   105,   166,   167,
     168,   169,   170,   171,    43,     0,   116,   100,     3,     4,
       5,     6,     5,     8,     9,    10,    39,    40,    13,   116,
      43,    16,   116,    18,   116,    20,   116,   255,   256,   257,
     198,   259,    27,    28,    29,    30,    31,    32,    33,    92,
      93,    36,   459,   460,    55,    56,   116,    42,   216,    44,
      45,    46,    47,   184,   185,    50,   116,    52,   116,    54,
      55,   144,   145,   165,   166,   233,    35,    36,    37,    38,
      39,    40,   445,   116,    43,   131,    71,   139,   140,   141,
     116,   143,   144,    78,    79,    80,    81,    35,    36,    37,
      38,    39,    40,   165,   166,    43,   165,   166,   266,   116,
     268,    96,    97,   271,   272,   273,   191,   275,    35,    36,
      37,    38,    39,    40,   109,   118,    43,   455,   116,   116,
     115,   289,     3,     4,     5,     6,   116,     8,     9,    35,
      36,    37,    38,    39,    40,   116,   116,    43,   116,     3,
       4,     5,     6,   445,     8,     9,    27,    28,    29,    30,
      31,    32,    33,   127,   127,    36,    86,   136,    83,    88,
      95,    42,   172,    27,    28,    29,    30,    31,    32,    33,
      89,   102,    36,   341,    55,    56,    73,   132,    42,   450,
     348,    35,    36,    37,    38,    39,    40,    12,   116,    43,
     113,    55,    65,    55,   362,   363,   364,     3,     4,     5,
       6,    56,     8,     9,   372,   129,    55,   461,    55,   377,
     404,   379,   113,   113,   100,   383,   137,    82,   386,    90,
     100,    27,    28,    29,    30,    31,    32,    33,     5,    43,
      36,   466,   465,   446,   118,   118,    42,   116,   463,   116,
      35,    36,    37,    38,    39,    40,   116,   111,    43,    55,
     116,    34,   223,   421,   422,   408,   424,    55,   426,   427,
     450,    82,   174,   104,   432,   433,   117,   175,    55,   458,
     116,   439,   440,   465,    35,    36,    37,    38,    39,    40,
     448,   111,    43,    55,    34,   464,    55,    58,     3,   457,
      37,    38,    39,    40,    34,   462,    43,   119,    34,   116,
      84,   459,   460,    58,    35,    36,    37,    38,    39,    40,
     184,   448,    43,   231,    34,    82,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,    35,    36,    37,    38,    39,    40,
     179,    82,    43,    82,   100,   113,   292,   459,   516,   517,
     518,   519,   292,   113,   113,   523,   524,   362,   292,   527,
     113,   113,   113,    57,   113,   458,   534,   535,   113,   113,
     538,   113,   540,   541,   542,   113,   544,   545,   113,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   459,   460,   113,    35,    36,
      37,    38,    39,    40,   113,   113,    43,   113,   113,    35,
      36,    37,    38,    39,    40,   113,   113,    43,   293,   587,
     588,   589,    35,    36,    37,    38,    39,    40,   360,   597,
      43,   292,   292,   292,   602,    35,    36,    37,    38,    39,
      40,   116,   292,    43,   360,   613,   116,   292,    35,    36,
      37,    38,    39,    40,   292,   450,    43,   292,   453,   454,
     292,   456,   116,   458,   292,   292,   292,   116,   121,   464,
      35,    36,    37,    38,    39,    40,    59,   116,    43,    35,
      36,    37,    38,    39,    40,   113,   113,    43,   113,    34,
     266,   460,    35,    36,    37,    38,    39,    40,   116,    82,
      43,   293,    85,   293,    87,   116,   674,   265,    91,   116,
     405,    94,   460,   116,   175,   186,   684,   186,   686,   186,
     688,   689,   690,   103,   184,   103,   121,   695,   121,   697,
     698,   699,   459,   701,   702,   441,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     977,   121,   121,   121,   121,   121,    34,   458,    34,   747,
     121,    35,    36,    37,    38,    39,    40,   755,   756,    43,
     121,   759,   760,   761,   458,   763,   121,   121,   121,   121,
     121,   769,   770,   121,   121,   121,   460,   121,   121,    35,
      36,    37,    38,    39,    40,   121,   121,    43,    35,    36,
      37,    38,    39,    40,   121,    34,    43,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,    34,    34,    43,    34,    34,    34,   816,   817,
     818,    34,   458,    34,   822,   823,   824,    34,   826,   827,
     828,    34,   830,   831,    34,   460,   834,    35,    36,    37,
      38,    39,    40,   841,    34,    43,    34,    34,   124,   847,
      34,   849,   134,   851,   852,   853,   269,   855,   856,    34,
      34,    34,   429,    34,    34,   863,   864,   865,   866,   460,
     429,   429,    34,   871,   872,   873,    35,    36,    37,    38,
      39,    40,   184,   117,    43,    35,    36,    37,    38,    39,
      40,   117,   117,    43,   186,    34,   894,   895,   896,   460,
      35,    36,    37,    38,    39,    40,   904,    34,    43,   458,
     458,   458,   910,   119,   458,   458,   458,   458,   916,   917,
     918,   458,   134,    35,    36,    37,    38,    39,    40,   460,
     144,    43,   458,   458,   932,   458,   458,   458,   458,    35,
      36,    37,    38,    39,    40,   458,   458,    43,   458,   458,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,    35,    36,    37,
      38,    39,    40,   458,   972,    43,   458,   458,   458,    34,
     978,   979,    34,   981,   982,   983,    34,   119,    34,    34,
     433,   989,   990,   460,   992,   993,   994,   995,   996,   997,
     998,   999,  1000,  1001,   460,  1003,  1004,  1005,  1006,  1007,
    1008,   119,  1010,   117,  1012,  1013,  1014,  1015,  1016,  1017,
      35,    36,    37,    38,    39,    40,   186,    58,    43,    58,
     460,    58,   188,   188,   188,   188,   143,  1035,   117,  1037,
    1038,  1039,   119,   460,  1042,  1043,   103,    35,    36,    37,
      38,    39,    40,   446,  1052,    43,    35,    36,    37,    38,
      39,    40,   447,    34,    43,   460,   119,    67,    35,    36,
      37,    38,    39,    40,   460,   137,    43,    34,   117,  1077,
     116,    35,    36,    37,    38,    39,    40,   460,   116,    43,
      35,    36,    37,    38,    39,    40,   116,    58,    43,   190,
     190,    35,    36,    37,    38,    39,    40,   190,   190,    43,
      34,    34,    34,    34,    34,    34,    34,  1115,  1116,  1117,
      34,    34,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,    35,    36,    37,    38,    39,    40,    34,    34,
      43,    34,    34,    34,    34,  1143,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,
    1158,  1159,  1160,    34,  1162,    34,    34,  1165,   110,   122,
     122,  1169,  1170,   122,  1172,  1173,   460,   117,   117,    77,
    1178,  1179,    35,    36,    37,    38,    39,    40,    34,    55,
      43,    58,    34,  1191,  1192,  1193,  1194,   112,    34,  1197,
      65,    34,  1200,  1201,   460,   116,    58,    35,    36,    37,
      38,    39,    40,   460,    34,    43,    35,    36,    37,    38,
      39,    40,   460,    34,    43,  1223,  1224,   116,    34,  1227,
     248,   460,  1230,    34,   248,  1233,   248,   248,    34,    34,
      77,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,
    1248,  1249,    77,    35,    36,    37,    38,    39,    40,   121,
    1258,    43,   460,  1261,  1262,  1263,  1264,    35,    36,    37,
      38,    39,    40,   116,    34,    43,    35,    36,    37,    38,
      39,    40,    34,  1281,    43,    35,    36,    37,    38,    39,
      40,    34,  1290,    43,  1292,  1293,  1294,    34,    34,    55,
     116,   460,   116,    35,    36,    37,    38,    39,    40,   444,
     460,    43,    34,  1311,  1312,   193,  1314,   116,   116,   113,
     113,   113,   113,   113,  1322,  1323,  1324,  1325,  1326,  1327,
    1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,   116,
     458,   116,    34,  1341,   449,  1343,  1344,  1345,   460,    34,
      34,    34,   121,  1351,  1352,  1353,  1354,  1355,  1356,  1357,
    1358,  1359,  1360,  1361,   460,  1363,  1364,  1365,  1366,   113,
     121,  1369,   121,   121,  1372,  1373,  1374,  1375,  1376,    35,
      36,    37,    38,    39,    40,   232,   196,    43,   232,   117,
     239,   232,   460,   232,   194,  1393,    34,  1395,  1396,    34,
      34,    34,    34,    34,  1402,  1403,    35,    36,    37,    38,
      39,    40,    34,    34,    43,    34,    34,  1415,    19,    34,
      21,    22,    23,    24,    25,    26,  1424,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,    34,    34,    43,   460,    34,  1445,    35,    36,
      37,    38,    39,    40,    34,    34,    43,    34,  1456,  1457,
    1458,  1459,  1460,  1461,  1462,  1463,  1464,  1465,    34,   448,
      34,    34,   460,  1471,  1472,  1473,   443,    34,    34,  1477,
    1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,    34,  1496,     5,
    1498,    34,   458,  1501,  1502,  1503,   460,   458,    35,    36,
      37,    38,    39,    40,   458,   460,    43,   458,  1516,  1517,
    1518,    34,    34,    34,  1522,  1523,   460,    35,    36,    37,
      38,    39,    40,    34,    58,    43,   412,    35,    36,    37,
      38,    39,    40,    34,  1542,    43,  1544,    34,  1546,  1547,
      34,    34,    34,  1551,    34,  1553,  1554,   460,  1556,  1557,
    1558,    34,  1560,  1561,  1562,    35,    36,    37,    38,    39,
      40,  1569,  1570,    43,    34,   117,    35,    36,    37,    38,
      39,    40,    34,    34,    43,    34,   116,  1585,     5,    34,
     425,  1589,    35,    36,    37,    38,    39,    40,   119,    34,
      43,    35,    36,    37,    38,    39,    40,   460,    34,    43,
      34,    58,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,
    1618,  1619,  1620,  1621,  1622,  1623,    34,  1625,   117,    34,
      34,  1629,   460,  1631,  1632,    34,  1634,  1635,  1636,    34,
    1638,   460,  1640,  1641,  1642,  1643,  1644,  1645,    34,  1647,
    1648,    34,    67,    35,    36,    37,    38,    39,    40,    34,
      34,    43,    34,    34,    34,  1663,    34,  1665,  1666,  1667,
      34,  1669,    34,    34,  1672,   447,    34,   459,  1676,   458,
      35,    36,    37,    38,    39,    40,    34,  1685,    43,    34,
     440,    34,   460,    34,    34,    34,    34,    34,  1696,    34,
    1698,   460,    34,   117,  1702,   437,  1704,  1705,   117,  1707,
    1708,  1709,    34,  1711,  1712,  1713,   116,    34,    34,   117,
      34,    34,  1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,
    1728,  1729,    58,  1731,   458,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,     5,    58,   116,    58,  1747,
    1748,  1749,    58,  1751,    35,    36,    37,    38,    39,    40,
     116,   425,    43,    34,    34,  1763,    34,   116,  1766,  1767,
     116,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,
     116,    34,    34,    34,  1782,    34,  1784,  1785,  1786,    35,
      36,    37,    38,    39,    40,    34,    34,    43,  1796,    34,
      34,    34,    34,    34,   460,    35,    36,    37,    38,    39,
      40,    34,    34,    43,    34,    34,   412,    34,    34,    34,
    1818,  1819,  1820,  1821,  1822,  1823,  1824,    34,  1826,    34,
      34,   460,  1830,  1831,    34,  1833,  1834,  1835,    34,  1837,
    1838,    34,  1840,  1841,  1842,  1843,    34,    34,  1846,  1847,
      34,    34,   460,    34,    34,    34,    34,    34,  1856,   113,
    1858,   460,    34,  1861,   458,   425,    34,  1865,    34,    34,
    1868,    34,    34,   460,    34,  1873,  1874,   458,    35,    36,
      37,    38,    39,    40,    34,  1883,    43,  1885,    34,  1887,
    1888,    34,  1890,  1891,  1892,    34,  1894,    34,    34,    34,
      34,    34,  1900,    34,    34,  1903,  1904,  1905,  1906,  1907,
    1908,  1909,    34,    34,    34,    34,   117,    35,    36,    37,
      38,    39,    40,  1921,    34,    43,    34,    34,  1926,    34,
      34,    34,    34,   460,    34,   458,    34,  1935,  1936,  1937,
    1938,  1939,  1940,  1941,  1942,  1943,  1944,   117,   117,  1947,
    1948,  1949,   460,   117,  1952,  1953,  1954,    34,    34,    34,
      34,    34,   460,    58,    35,    36,    37,    38,    39,    40,
    1968,  1969,    43,    34,    34,  1973,    34,    34,    34,  1977,
    1978,  1979,  1980,  1981,  1982,  1983,    34,  1985,  1986,  1987,
     460,  1989,    58,    58,  1992,  1993,  1994,    34,    34,  1997,
    1998,   460,    35,    36,    37,    38,    39,    40,  2006,    34,
      43,    34,  2010,    34,  2012,    58,    34,   460,    34,   116,
    2018,    34,   116,   116,   116,   459,  2024,  2025,  2026,  2027,
     458,  2029,  2030,  2031,    34,  2033,    34,    34,  2036,  2037,
    2038,    34,  2040,  2041,  2042,  2043,  2044,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    34,  2055,   113,    35,
      36,    37,    38,    39,    40,    34,    34,    43,  2066,  2067,
    2068,  2069,  2070,  2071,  2072,  2073,  2074,  2075,   460,  2077,
      34,    34,    34,  2081,  2082,  2083,   117,    35,    36,    37,
      38,    39,    40,    34,    34,    43,    35,    36,    37,    38,
      39,    40,    34,    34,    43,   460,   458,  2105,  2106,  2107,
    2108,  2109,  2110,  2111,    34,  2113,  2114,  2115,   246,  2117,
      34,  2119,    34,    34,    34,    34,  2124,  2125,    35,    36,
      37,    38,    39,    40,    34,  2133,    43,   458,   113,  2137,
      58,    34,    34,  2141,  2142,  2143,  2144,    34,    34,    34,
     116,    34,  2150,  2151,  2152,  2153,    34,  2155,  2156,  2157,
     460,  2159,  2160,  2161,   458,    34,    34,   458,  2166,  2167,
    2168,  2169,  2170,    35,    36,    37,    38,    39,    40,   460,
     458,    43,    34,    34,    34,  2183,  2184,  2185,    34,    34,
      34,    34,   246,    34,  2192,    34,  2194,  2195,  2196,    34,
    2198,   458,  2200,  2201,  2202,    34,    34,    34,   458,  2207,
     458,  2209,    34,   458,   460,   247,    35,    36,    37,    38,
      39,    40,    34,    34,    43,  2223,  2224,  2225,    34,    34,
     460,  2229,  2230,  2231,  2232,  2233,  2234,    34,    34,  2237,
      34,    34,  2240,  2241,    34,   458,    34,   246,    34,  2247,
      35,    36,    37,    38,    39,    40,  2254,  2255,    43,  2257,
    2258,   246,   246,    34,    34,   247,  2264,  2265,    34,    34,
      34,    34,    34,  2271,  2272,    34,   458,  2275,    34,    34,
    2278,    34,   458,   458,   458,    34,    34,  2285,  2286,  2287,
    2288,   117,    35,    36,    37,    38,    39,    40,    34,    34,
      43,   117,   117,   460,    34,    34,    34,  2305,  2306,  2307,
    2308,    58,  2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,
      35,    36,    37,    38,    39,    40,    58,    58,    43,    34,
      34,    34,   116,   116,   116,    34,  2334,  2335,  2336,  2337,
    2338,   459,   247,  2341,  2342,  2343,  2344,    34,  2346,   247,
     247,    34,    34,    34,  2352,  2353,    34,    34,    34,    34,
    2358,    34,   458,   434,  2362,  2363,    34,   458,  2366,  2367,
    2368,  2369,   458,  2371,  2372,  2373,   117,   116,    35,    36,
      37,    38,    39,    40,    34,    34,    43,  2385,  2386,  2387,
    2388,    34,    58,    34,  2392,  2393,    34,    34,    34,    34,
      34,    34,    34,   117,  2402,  2403,  2404,    34,    34,  2407,
     117,    55,    56,    58,    34,    59,  2414,    61,  2416,    58,
      34,    65,  2420,   116,   116,  2423,   459,    34,  2426,  2427,
      74,  2429,    34,    34,    34,  2433,    34,   117,  2436,    34,
    2438,  2439,  2440,   117,   116,    35,    36,    37,    38,    39,
      40,  2449,  2450,    43,    34,    58,    34,  2455,    34,    58,
     116,  2459,    34,  2461,    34,    34,    34,    34,    34,    34,
      34,   459,  2470,  2471,  2472,    34,  2474,    34,  2476,    34,
      34,    34,  2480,   459,  2482,    35,    36,    37,    38,    39,
      40,  2489,  2490,    43,    34,  2493,    34,    34,  2496,  2497,
      35,    36,    37,    38,    39,    40,  2504,  2505,    43,    34,
    2508,   459,    34,  2511,  2512,    34,    34,    34,    34,    34,
     459,  2519,  2520,  2521,    34,    34,    -1,    -1,    -1,  2527,
    2528,    -1,   439,   203,    -1,  2533,  2534,    -1,    -1,    -1,
     184,  2539,  2540,    -1,    -1,    -1,    -1,  2545,  2546,    -1,
      -1,     1,  2550,     3,     4,     5,     6,    -1,     8,     9,
      10,    -1,    -1,    13,    -1,    -1,    16,    -1,    18,    -1,
      20,    -1,    -1,    -1,   436,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    44,    45,    46,    47,    -1,    -1,
      50,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    10,    -1,    -1,    13,
      -1,    71,    16,    -1,    18,    -1,    20,    -1,    78,    79,
      80,    81,    -1,    27,    28,    29,    30,    31,    32,    33,
     459,    -1,    36,    -1,    -1,    -1,    96,    97,    42,    -1,
      44,    45,    46,    47,    -1,    -1,    50,    -1,    52,   109,
      54,    55,    -1,    -1,    -1,   115,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,     8,     9,    10,   109,   459,    13,    -1,    -1,
      16,   115,    18,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    30,    31,    32,    33,    -1,    -1,
      36,    -1,    -1,    -1,   459,    -1,    42,    -1,    44,    45,
      46,    47,    -1,    -1,    50,    -1,    52,    -1,    54,    55,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    71,    -1,    43,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      96,    97,   459,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   459,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   459,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   459,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
     450,    -1,    -1,   453,   454,    -1,   456,    -1,   458,    -1,
      -1,    -1,    -1,    -1,   464,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   450,    -1,    -1,   453,
     454,    -1,   456,    -1,   458,    -1,    -1,    -1,    -1,    -1,
     464,    -1,   466,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   459,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   459,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   450,    -1,    -1,   453,   454,    -1,
     456,    -1,   458,    -1,    -1,    -1,    -1,    -1,   464,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,   459,    -1,    43,    35,    36,    37,
      38,    39,    40,   459,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
     459,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,   459,    -1,    43,    35,    36,    37,
      38,    39,    40,   459,    -1,    43,    35,    36,    37,    38,
      39,    40,   459,    -1,    43,    35,    36,    37,    38,    39,
      40,   459,    -1,    43,    35,    36,    37,    38,    39,    40,
     459,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   459,    -1,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   459,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,   459,
      -1,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   459,    -1,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,   459,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     459,    -1,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   459,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,   459,    -1,    43,    35,    36,
      37,    38,    39,    40,   459,    -1,    43,    35,    36,    37,
      38,    39,    40,   459,    -1,    43,    35,    36,    37,    38,
      39,    40,   459,    -1,    43,    35,    36,    37,    38,    39,
      40,   459,    -1,    43,    35,    36,    37,    38,    39,    40,
     459,    -1,    43,    35,    36,    37,    38,    39,    40,   459,
      -1,    43,    35,    36,    37,    38,    39,    40,   459,    -1,
      43,    35,    36,    37,    38,    39,    40,   459,    -1,    43,
      35,    36,    37,    38,    39,    40,   459,    -1,    43,    35,
      36,    37,    38,    39,    40,   459,    -1,    43,    35,    36,
      37,    38,    39,    40,   459,    -1,    43,    35,    36,    37,
      38,    39,    40,   459,    -1,    43,    35,    36,    37,    38,
      39,    40,   459,    -1,    43,    35,    36,    37,    38,    39,
      40,   459,    -1,    43,    35,    36,    37,    38,    39,    40,
     459,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   459,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   459,    -1,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   459,    -1,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   459,
      -1,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   459,    -1,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   459,    -1,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   459,    -1,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   459,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   459,
      -1,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   459,
      -1,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   459,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   459,    -1,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
     459,    -1,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    -1,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   459,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   459,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   459,    -1,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,   459,    -1,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   459,    -1,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   459,    -1,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    -1,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   459,    -1,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   459,    -1,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   459,
      -1,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,   459,    -1,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   459,    -1,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   459,    -1,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   459,    -1,    -1,    -1,    55,    56,    -1,
      58,    59,   459,    61,    62,    63,    -1,    65,    66,    67,
      -1,   459,    -1,    35,    36,    37,    38,    39,    40,    77,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,   459,    -1,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    35,    36,    37,    38,
      39,    40,    -1,   459,    43,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    35,    36,    37,    38,    39,    40,    -1,   459,    43,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
     459,    -1,    35,    36,    37,    38,    39,    40,    -1,   459,
      43,    -1,    -1,    -1,   192,    -1,    -1,    -1,   459,    -1,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   459,    -1,
      35,    36,    37,    38,    39,    40,    -1,   459,    43,    35,
      36,    37,    38,    39,    40,    -1,   459,    43,    35,    36,
      37,    38,    39,    40,    -1,   459,    43,    35,    36,    37,
      38,    39,    40,    -1,   459,    43,    -1,    -1,    -1,    -1,
     258,    -1,    -1,   459,    -1,    35,    36,    37,    38,    39,
      40,    -1,   459,    43,    35,    36,    37,    38,    39,    40,
      -1,   459,    43,    35,    36,    37,    38,    39,    40,    -1,
     459,    43,    -1,    35,    36,    37,    38,    39,    40,   459,
     442,    43,   435,    -1,    -1,    -1,    -1,    -1,   459,    -1,
     434,    35,    36,    37,    38,    39,    40,    -1,   433,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   433,    43,
      35,    36,    37,    38,    39,    40,   432,    -1,    43,    35,
      36,    37,    38,    39,    40,   432,    -1,    43,    35,    36,
      37,    38,    39,    40,   432,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   432,    35,    36,    37,
      38,    39,    40,    -1,   431,    43,    35,    36,    37,    38,
      39,    40,    -1,   431,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   431,    35,    36,    37,    38,
      39,    40,    -1,   430,    43,    35,    36,    37,    38,    39,
      40,    -1,   430,    43,    35,    36,    37,    38,    39,    40,
      -1,   430,    43,    35,    36,    37,    38,    39,    40,    -1,
     430,    43,    -1,    35,    36,    37,    38,    39,    40,   430,
     438,    43,    35,    36,    37,    38,    39,    40,   410,    -1,
      43,    -1,    -1,    -1,   407,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   406,    -1,
      -1,    -1,    -1,    -1,   403,    -1,    35,    36,    37,    38,
      39,    40,    -1,   403,    43,    35,    36,    37,    38,    39,
      40,   402,    -1,    43,    -1,    -1,    -1,    -1,    -1,   401,
      35,    36,    37,    38,    39,    40,    -1,   400,    43,    35,
      36,    37,    38,    39,    40,   399,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   398,    35,    36,    37,    38,    39,    40,
      -1,   397,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   396,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     395,    43,    35,    36,    37,    38,    39,    40,   394,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,   393,    43,
      35,    36,    37,    38,    39,    40,   392,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   391,    35,    36,    37,    38,    39,
      40,    -1,   390,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    35,    36,    37,    38,    39,    40,   389,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   388,    35,    36,
      37,    38,    39,    40,    -1,   387,    43,    35,    36,    37,
      38,    39,    40,    -1,   386,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,   385,    -1,    43,    35,    36,    37,    38,    39,    40,
     384,    -1,    43,    -1,    -1,    -1,    -1,    -1,   383,    35,
      36,    37,    38,    39,    40,    -1,   382,    43,    35,    36,
      37,    38,    39,    40,   381,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   380,    -1,    35,    36,    37,    38,    39,    40,
      -1,   379,    43,    -1,    -1,    -1,    -1,   367,    -1,   378,
      35,    36,    37,    38,    39,    40,    -1,   366,    43,    35,
      36,    37,    38,    39,    40,   365,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   364,    35,    36,    37,    38,    39,    40,
      -1,   363,    43,    35,    36,    37,    38,    39,    40,   361,
      -1,    43,    35,    36,    37,    38,    39,    40,   361,    -1,
      43,    -1,    -1,    -1,    -1,   355,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   354,    35,    36,    37,    38,
      39,    40,    -1,   353,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   352,    35,    36,
      37,    38,    39,    40,    -1,   351,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   350,
      35,    36,    37,    38,    39,    40,    -1,   349,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   348,    43,    35,
      36,    37,    38,    39,    40,   347,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   346,    35,    36,    37,    38,    39,    40,
      -1,   345,    43,    35,    36,    37,    38,    39,    40,   344,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   343,    35,    36,    37,    38,    39,    40,
      -1,   342,    43,    35,    36,    37,    38,    39,    40,   341,
      -1,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   340,    -1,    35,    36,    37,    38,    39,
      40,    -1,   340,    43,    35,    36,    37,    38,    39,    40,
     339,    -1,    43,    -1,    -1,    -1,    -1,   337,    35,    36,
      37,    38,    39,    40,    -1,   336,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   335,
      35,    36,    37,    38,    39,    40,    -1,   334,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   333,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   332,    35,    36,
      37,    38,    39,    40,    -1,   331,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   330,
      35,    36,    37,    38,    39,    40,    -1,   329,    43,    35,
      36,    37,    38,    39,    40,   328,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   327,    35,
      36,    37,    38,    39,    40,    -1,   326,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   325,    43,    35,    36,
      37,    38,    39,    40,   324,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   323,    35,    36,    37,
      38,    39,    40,    -1,   322,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   321,    -1,    35,    36,
      37,    38,    39,    40,    -1,   320,    43,    35,    36,    37,
      38,    39,    40,   319,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,   318,    35,    36,
      37,    38,    39,    40,    -1,   317,    43,    35,    36,    37,
      38,    39,    40,   316,    -1,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   315,    43,    -1,    -1,    -1,   311,
      -1,    35,    36,    37,    38,    39,    40,    -1,   311,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
     310,    -1,    35,    36,    37,    38,    39,    40,    -1,   310,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   308,    35,    36,    37,    38,    39,    40,    -1,   307,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   306,    35,    36,    37,    38,    39,    40,    -1,   305,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,   304,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   303,    -1,    35,    36,
      37,    38,    39,    40,    -1,   303,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   303,    -1,
      35,    36,    37,    38,    39,    40,    -1,   303,    43,    35,
      36,    37,    38,    39,    40,   302,    -1,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   302,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   302,    43,    35,    36,
      37,    38,    39,    40,    -1,   302,    43,    35,    36,    37,
      38,    39,    40,    -1,   302,    43,    -1,    35,    36,    37,
      38,    39,    40,    -1,   302,    43,    35,    36,    37,    38,
      39,    40,    -1,   302,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   301,    -1,    35,    36,    37,    38,
      39,    40,    -1,   301,    43,    35,    36,    37,    38,    39,
      40,    -1,   301,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   301,    -1,    35,    36,    37,    38,
      39,    40,    -1,   301,    43,    -1,    35,    36,    37,    38,
      39,    40,    -1,   301,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,   301,    35,    36,
      37,    38,    39,    40,    -1,   300,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   300,    -1,    35,
      36,    37,    38,    39,    40,    -1,   300,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   299,    43,    35,    36,
      37,    38,    39,    40,    -1,   299,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   299,    35,    36,    37,
      38,    39,    40,    -1,   298,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   298,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,   298,    -1,    35,    36,    37,    38,    39,    40,    -1,
     298,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   298,    35,    36,    37,    38,    39,    40,
     296,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
     296,    43,    35,    36,    37,    38,    39,    40,    -1,   295,
      43,    35,    36,    37,    38,    39,    40,    -1,   295,    43,
      35,    36,    37,    38,    39,    40,    -1,   295,    43,    -1,
      35,    36,    37,    38,    39,    40,    -1,   295,    43,    35,
      36,    37,    38,    39,    40,    -1,   295,    43,    35,    36,
      37,    38,    39,    40,    -1,   295,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   295,    -1,    35,    36,
      37,    38,    39,    40,    -1,   295,    43,    35,    36,    37,
      38,    39,    40,    -1,   295,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   295,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   295,    -1,    35,    36,
      37,    38,    39,    40,    -1,   295,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   294,    35,    36,
      37,    38,    39,    40,    -1,   293,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,   293,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   293,    -1,    35,
      36,    37,    38,    39,    40,    -1,   293,    43,    35,    36,
      37,    38,    39,    40,    -1,   293,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   293,    43,    35,    36,    37,
      38,    39,    40,    -1,   293,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   293,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    35,    36,    37,    38,    39,
      40,   293,    -1,    43,    35,    36,    37,    38,    39,    40,
     293,    -1,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   293,    -1,    35,    36,    37,    38,    39,    40,
      -1,   293,    43,    35,    36,    37,    38,    39,    40,    -1,
     293,    43,    35,    36,    37,    38,    39,    40,    -1,   293,
      43,    35,    36,    37,    38,    39,    40,   292,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,   292,    43,    35,
      36,    37,    38,    39,    40,    -1,   292,    43,    35,    36,
      37,    38,    39,    40,    -1,   292,    43,    35,    36,    37,
      38,    39,    40,    -1,   292,    43,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,   292,    -1,    -1,   266,    -1,
      -1,    -1,    -1,    -1,   292,    -1,    -1,    -1,    -1,   266,
      -1,    35,    36,    37,    38,    39,    40,    -1,   266,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   266,
      -1,    35,    36,    37,    38,    39,    40,    -1,   266,    43,
      35,    36,    37,    38,    39,    40,    -1,   264,    43,    -1,
      -1,    -1,   249,    -1,    -1,   261,    35,    36,    37,    38,
      39,    40,   249,    -1,    43,    -1,    -1,   245,    -1,    35,
      36,    37,    38,    39,    40,    -1,   245,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   245,    43,    35,    36,
      37,    38,    39,    40,    -1,   245,    43,    35,    36,    37,
      38,    39,    40,    -1,   245,    43,    -1,    -1,    -1,    -1,
      -1,   243,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   243,    35,    36,    37,    38,    39,    40,    -1,
     242,    43,    35,    36,    37,    38,    39,    40,    -1,   242,
      43,    35,    36,    37,    38,    39,    40,   241,    -1,    43,
      35,    36,    37,    38,    39,    40,   241,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   240,    -1,    35,    36,    37,    38,
      39,    40,    -1,   240,    43,    -1,    -1,    -1,    -1,   237,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   236,    -1,    35,    36,    37,    38,    39,    40,
      -1,   236,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   236,    -1,    35,    36,    37,    38,    39,    40,
      -1,   236,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,   235,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   235,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   235,
      -1,    35,    36,    37,    38,    39,    40,    -1,   235,    43,
      35,    36,    37,    38,    39,    40,   234,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   234,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   234,    -1,    35,    36,    37,    38,    39,    40,
      -1,   234,    43,    35,    36,    37,    38,    39,    40,   233,
      -1,    43,    35,    36,    37,    38,    39,    40,   233,    -1,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,   233,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   233,    35,
      36,    37,    38,    39,    40,    -1,   232,    43,    35,    36,
      37,    38,    39,    40,    -1,   232,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   230,
      -1,    35,    36,    37,    38,    39,    40,    -1,   230,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   230,
      -1,    35,    36,    37,    38,    39,    40,    -1,   230,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   230,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,   230,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,   229,    -1,    35,    36,    37,
      38,    39,    40,    -1,   229,    43,    35,    36,    37,    38,
      39,    40,    -1,   229,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,   229,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   229,    -1,
      35,    36,    37,    38,    39,    40,    -1,   229,    43,    35,
      36,    37,    38,    39,    40,   228,    -1,    43,    35,    36,
      37,    38,    39,    40,   228,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,   228,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   228,    -1,    35,    36,    37,    38,    39,    40,
      -1,   228,    43,    35,    36,    37,    38,    39,    40,    -1,
     228,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   227,    -1,    35,    36,    37,    38,    39,
      40,    -1,   227,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,   227,    -1,    35,    36,    37,    38,    39,
      40,    -1,   227,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,   227,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   227,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   226,    -1,
      35,    36,    37,    38,    39,    40,    -1,   226,    43,    35,
      36,    37,    38,    39,    40,    -1,   226,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   226,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,   226,    -1,    35,    36,    37,    38,    39,    40,    -1,
     226,    43,    35,    36,    37,    38,    39,    40,   225,    -1,
      43,    35,    36,    37,    38,    39,    40,   225,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   225,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,   225,    -1,    35,    36,    37,    38,
      39,    40,    -1,   225,    43,    35,    36,    37,    38,    39,
      40,    -1,   225,    43,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,   224,    -1,    35,    36,    37,    38,
      39,    40,    -1,   224,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,   224,    -1,    35,    36,    37,    38,
      39,    40,    -1,   224,    43,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,   224,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,   224,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,   223,    -1,
      35,    36,    37,    38,    39,    40,    -1,   223,    43,    35,
      36,    37,    38,    39,    40,    -1,   223,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,   223,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,   223,    -1,    35,    36,    37,    38,    39,    40,    -1,
     223,    43,    35,    36,    37,    38,    39,    40,   222,    -1,
      43,    35,    36,    37,    38,    39,    40,   222,    -1,    43,
      35,    36,    37,    38,    39,    40,    -1,   222,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   222,    -1,    -1,   195,    -1,    -1,   189,
      -1,    -1,   222,    -1,    35,    36,    37,    38,    39,    40,
     189,   222,    43,    35,    36,    37,    38,    39,    40,   189,
      -1,    43,    -1,    35,    36,    37,    38,    39,    40,    -1,
     189,    43,    -1,    -1,    -1,    -1,    -1,   183,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,   183,    35,
      36,    37,    38,    39,    40,    -1,   182,    43,    35,    36,
      37,    38,    39,    40,    -1,   182,    43,    35,    36,    37,
      38,    39,    40,    -1,   181,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   181,
      35,    36,    37,    38,    39,    40,    -1,   180,    43,    35,
      36,    37,    38,    39,    40,   170,   180,    43,    35,    36,
      37,    38,    39,    40,   170,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,   170,
      -1,    35,    36,    37,    38,    39,    40,    -1,   170,    43,
      -1,    35,    36,    37,    38,    39,    40,    -1,   170,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,   169,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,   169,    -1,    35,    36,    37,    38,    39,
      40,    -1,   169,    43,    35,    36,    37,    38,    39,    40,
      -1,   169,    43,    35,    36,    37,    38,    39,    40,    -1,
     169,    43,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,   168,    -1,    35,    36,    37,    38,    39,
      40,    -1,   168,    43,    35,    36,    37,    38,    39,    40,
      -1,   168,    43,    35,    36,    37,    38,    39,    40,    -1,
     168,    43,    35,    36,    37,    38,    39,    40,    -1,   168,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    35,    36,    37,    38,    39,    40,    -1,   163,    43,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,   163,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   162,    35,    36,    37,    38,    39,    40,   160,
      -1,    43,    35,    36,    37,    38,    39,    40,   160,    -1,
      43,    -1,    -1,    -1,    -1,    -1,   159,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,   159,
      -1,    35,    36,    37,    38,    39,    40,    -1,   159,    43,
      35,    36,    37,    38,    39,    40,   158,    -1,    43,    35,
      36,    37,    38,    39,    40,   158,   150,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   150,    35,    36,    37,
      38,    39,    40,    -1,   149,    43,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,   149,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   148,    35,    36,    37,
      38,    39,    40,    -1,   147,    43,    -1,    35,    36,    37,
      38,    39,    40,   118,    -1,    43,    -1,    -1,    -1,   147,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,   134,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,   134,    -1,
      35,    36,    37,    38,    39,    40,    -1,   134,    43,    35,
      36,    37,    38,    39,    40,    -1,   134,    43,    -1,    35,
      36,    37,    38,    39,    40,    -1,   134,    43,    35,    36,
      37,    38,    39,    40,    -1,   134,    43,    -1,    35,    36,
      37,    38,    39,    40,    -1,   133,    43,    -1,    -1,    35,
      36,    37,    38,    39,    40,   133,   124,    43,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,   124,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,   124,
      -1,    35,    36,    37,    38,    39,    40,    -1,   124,    43,
      35,    36,    37,    38,    39,    40,    -1,   123,    43,    -1,
      -1,   118,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,   118,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,   118,    -1,    35,    36,    37,    38,    39,    40,
      -1,   118,    43,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,   117,    -1,    35,    36,    37,    38,    39,    40,
      -1,   117,    43,    35,    36,    37,    38,    39,    40,    -1,
     114,    43,    35,    36,    37,    38,    39,    40,    -1,   114,
      43,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,   114,    35,    36,    37,    38,    39,    40,    -1,   112,
      43,    -1,    35,    36,    37,    38,    39,    40,    -1,   110,
      43,    -1,    -1,    -1,    -1,    -1,   108,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    82,
     103,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    82,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
     217,   218,   219,   220,   221,   270,   271,   272,   273,    -1,
      -1,   276,   277,   278,   279,   280,   281,    -1,    -1,   284,
     285,   286,   287,   288,   289,   290,   291
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     6,     8,     9,    10,    13,
      16,    18,    20,    27,    28,    29,    30,    31,    32,    33,
      36,    42,    44,    45,    46,    47,    50,    52,    54,    55,
      71,    78,    79,    80,    81,    96,    97,   109,   115,   450,
     453,   454,   456,   458,   464,   468,   469,   470,   473,   474,
     475,   478,   479,   480,    19,    21,    22,    23,    24,    25,
      26,    34,   458,   458,   458,   479,   479,     3,   479,   479,
      55,    56,   479,    53,    51,    55,    56,    58,    59,    61,
      62,    63,    65,    66,    67,    77,   192,   258,   438,   476,
     461,   450,    55,    55,    55,    56,    59,    61,    65,    74,
     184,    59,    82,    85,    87,    91,    94,   269,    89,   100,
     479,    72,   131,   128,    56,   450,   450,   457,   479,   464,
       0,   480,    35,    36,    37,    38,    39,    40,    43,   464,
       3,   479,   479,   479,   479,   479,   479,   479,   472,   479,
     479,   479,   465,   481,   481,   116,   116,   479,     5,   116,
     116,   477,   116,   116,   116,   116,   116,   445,   131,   116,
     116,   191,   116,   118,   479,   455,   116,   116,   116,   116,
     116,   116,   445,   127,   127,   136,    83,    86,    88,    92,
      93,    95,    89,   172,   102,   133,    73,   132,   479,   450,
     460,   479,   479,   479,   479,   479,   479,   479,   459,   460,
     460,   460,   466,   480,   482,    12,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   113,   116,   479,   479,    65,
     479,    55,   462,   129,   479,   479,   479,   479,   479,   479,
      55,    55,    56,   137,   404,   113,   113,   100,    90,   130,
     173,   176,   177,    82,   100,     5,   133,   479,   482,   466,
     481,   110,   134,   134,   118,   118,   118,   118,   134,    55,
     446,   479,   134,   399,   118,   100,   116,   463,   118,   114,
     114,   116,   116,   116,   479,    34,   223,   408,    55,    82,
     117,   174,   175,   175,   178,   104,    98,   101,   105,   458,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   215,   217,   218,
     219,   220,   221,   270,   271,   272,   273,   276,   277,   278,
     279,   280,   281,   284,   285,   286,   287,   288,   289,   290,
     291,    55,    56,   111,    55,   111,    55,    56,   426,   427,
     428,   116,    55,   162,   255,   256,   257,   259,    34,    55,
      58,   479,     3,   479,   111,   119,   471,   471,   479,   479,
     479,   479,    34,    34,   116,    84,    58,   231,   184,   184,
     185,   179,    34,    82,    82,    82,   479,   100,   113,   100,
     113,   113,   113,   100,   113,   113,   100,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   292,   292,   292,   292,   292,   292,   362,
     293,   360,   360,   292,   292,   292,   292,   292,   292,   292,
     292,   116,   116,    57,   116,   121,   116,   116,   113,   113,
     113,   479,   116,    34,   266,   293,   293,   265,   479,   116,
     116,   439,   471,   403,   479,   479,   479,   405,   116,   175,
     186,   186,   186,   184,   479,   103,   103,   459,   479,   121,
     479,   121,   121,   121,   479,   121,   121,   479,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,   111,   479,   479,   134,   479,   124,   479,   479,
     429,   429,   429,   124,   479,   479,    34,    34,    34,    34,
     400,   479,   479,    34,    34,   407,   124,    34,   479,   184,
     117,   117,   117,   186,    34,    34,   479,   189,   458,   189,
     458,   458,   458,   189,   458,   458,   189,   458,   458,   458,
     458,   458,   458,   458,   458,   458,   458,   458,   458,   458,
     458,   458,   458,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   134,   134,   134,   144,   124,   119,    34,    34,    34,
     119,   112,   147,   479,   479,   479,   479,    34,   448,   118,
     479,   479,    34,   119,   479,   147,   186,    58,    58,    58,
     117,   479,   479,   459,   188,   479,   188,   479,   479,   479,
     188,   479,   479,   188,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     293,   293,   378,   340,   340,   307,   363,   294,   293,   293,
     394,   293,   293,   293,   293,   293,   293,   293,   143,   139,
     140,   141,   143,   144,   144,   145,   117,   119,   103,   479,
     479,   479,   446,   447,    34,   117,   295,   232,   479,   119,
      67,   440,   402,   479,   137,   406,    34,   117,   116,   116,
     116,    58,   107,   479,   190,   459,   190,   459,   459,   459,
     190,   459,   459,   190,   459,   459,   459,   459,   459,   459,
     459,   459,   459,   459,   459,   459,   459,   459,   459,   459,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,   122,   122,
     122,   117,   117,   110,    77,   103,   148,    34,   430,   430,
     430,   112,    55,   479,    58,    34,    34,   401,    65,   116,
      34,    34,   479,    34,   479,    58,   479,   479,   479,   116,
      34,   460,   248,   479,   248,   479,   479,   479,   248,   479,
     479,   248,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,    34,    34,    34,    77,
      77,   121,   116,    34,    34,   479,    34,    34,    34,    55,
     116,   116,   479,   479,    34,   193,   479,   479,   479,   479,
     163,   116,   168,   168,   168,   479,   479,   113,   113,   459,
     113,   459,   459,   459,   113,   459,   459,   113,   460,   460,
     460,   460,   460,   459,   459,   459,   459,   460,   460,   460,
     460,   459,   459,   459,   295,   295,   379,   337,   341,   308,
     364,   361,   361,   395,   295,   295,   295,   295,   295,   295,
     295,   479,   479,   479,   116,   116,   458,   479,   479,   479,
     147,   479,   479,   479,   116,   479,   479,   266,   235,   479,
     449,   261,   441,   403,    34,   479,    34,    34,    34,   168,
     106,   479,   121,   479,   121,   479,   479,   479,   121,   479,
     479,   121,   113,   196,   232,   232,   232,   479,   479,   479,
     479,   117,   239,   194,   232,   479,   479,   479,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,   103,   163,   479,   479,   479,
     148,   158,    34,   431,   432,   432,   479,   448,    34,    34,
     479,    34,    34,    34,     5,   168,   479,   479,   479,    34,
      34,   114,   458,   459,   458,   459,   459,   459,   458,   459,
     459,   458,   479,    34,    34,    34,    34,   459,   459,   460,
     459,    58,    34,    34,    34,   459,   459,   459,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,    34,    34,   459,    34,    34,
     479,    34,    34,    34,   124,   471,   479,   479,   479,   479,
     479,   412,    34,   169,   169,   169,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   123,   479,
     479,   479,   479,   479,   479,   117,   479,   116,   479,   479,
     479,   479,   479,   479,   380,   317,   295,   304,   365,   430,
     435,   396,   296,   296,   298,   298,   298,   298,   298,   479,
       5,   479,   479,   479,   167,   425,   479,   479,   119,   292,
     236,   264,   442,    34,   479,    34,    34,    34,   169,   108,
     459,   459,   459,   459,   459,   459,   459,   459,   459,   459,
     117,   268,   293,   293,   293,   460,   460,    58,   460,   479,
     240,   195,   117,   459,   459,   459,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,   163,   459,   158,   149,    34,   431,   433,   447,    34,
      34,    34,    34,   458,   169,   479,   479,   479,    34,    34,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
      67,    34,    34,    34,    34,   117,   117,   116,   117,   232,
      34,    34,    58,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,    34,   479,    34,    34,   479,    34,    34,   479,   479,
     458,   479,   479,    34,   170,   170,   170,   479,   479,   459,
     459,   459,   459,   459,   459,   459,   459,   459,   459,   116,
     479,   479,   479,   479,    58,    58,   479,    58,    34,   479,
     479,   116,   459,   459,   459,   381,   315,   339,   305,   410,
     432,   436,   397,   303,   299,   303,   303,   303,   299,   299,
       5,   459,   479,   479,   159,   425,   479,   233,   479,   443,
     459,   479,    34,    34,    34,   170,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   292,   233,   233,
     233,   116,   116,   266,   116,   479,   241,   240,   479,   479,
     479,   479,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,   412,   479,   149,
     160,    34,   431,    34,   459,    34,   479,   170,   479,   479,
     479,    34,   459,   459,   459,   459,   459,   459,   459,   459,
     459,   459,   292,    34,    34,    34,    34,   479,   479,    34,
     479,   292,    34,    34,   459,   459,   459,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,    34,   459,   460,    34,    34,   479,    34,   479,
     479,   479,   459,    34,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,    34,   479,   479,   479,   479,
     266,   266,   479,   266,    34,   479,   479,   479,   479,   479,
     382,   316,   342,   306,   366,   433,   437,   398,   302,   300,
     302,   302,   302,   300,   300,   458,   479,   113,   479,   479,
     425,   234,   460,   444,   479,   479,   459,   459,   459,   459,
     459,   459,   459,   459,   459,   459,   479,   245,   234,   234,
     234,    34,    34,    34,   479,   242,   241,   459,   459,   459,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,   479,   459,   165,   166,   160,
     150,    34,    34,   459,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   237,    34,    34,    34,    34,   479,
     479,   479,    34,    34,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   459,   479,   458,   479,    34,    34,   479,   479,
     479,   459,   460,   459,   459,   460,   459,   459,   459,   460,
     459,    34,   458,   479,   479,   479,   479,   479,   460,   459,
     459,   383,   318,   343,   367,   434,   430,   295,   302,   301,
     301,   301,   302,   302,   479,   459,   479,   479,   479,   459,
     479,   117,   479,   479,   117,   479,   479,   479,   117,   479,
     479,   479,   236,   236,   236,   243,   242,   117,   479,   479,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,   459,   479,   459,   150,   159,   479,   459,
      58,   459,   459,    58,   459,   459,   459,    58,   459,   245,
     459,    34,    34,    34,    34,    34,    58,   459,   459,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   459,   479,    34,    34,   459,   479,   116,
     479,   479,   116,   479,   479,   479,   116,   479,    34,   479,
     479,   479,   479,   479,   479,   116,   479,   479,   384,   319,
     344,   432,   301,   311,   301,   301,   459,   479,   459,   479,
     479,   479,   459,   479,   459,   459,   479,   459,   459,   459,
     479,   459,   458,   459,   235,   235,   235,   249,   243,   479,
     459,   459,    34,    34,    34,    34,    34,    34,    34,    34,
     479,   459,   460,   479,   159,   460,   479,   479,   479,   222,
     479,   479,   479,   222,   479,   479,   479,    34,    34,    34,
      34,    34,   222,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   459,   479,   113,   459,    34,   460,   459,
     459,    34,   459,   459,   459,    34,   459,   459,   460,   479,
     479,   479,   458,   479,    34,   459,   459,   385,   320,   345,
     433,   310,   311,   479,   460,   165,   166,   479,   479,   117,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   246,
     292,   292,   292,   479,   249,   479,   479,   479,    34,    34,
      34,    34,    34,    34,   459,   113,   458,   479,   460,    58,
     459,   459,   223,   459,   459,   459,   223,   459,   459,    34,
      34,    34,    34,   459,    34,   223,   459,   459,   479,   479,
     479,   479,   479,   479,   479,   165,   166,   479,   116,   479,
     479,    34,   479,   479,   479,    34,   479,   479,   458,   479,
     479,   479,   479,   458,    34,   479,   479,   386,   321,   346,
     434,   310,   460,   458,   479,   459,   479,   459,   459,   479,
     459,   459,   459,   479,   459,   460,   479,   245,   245,   245,
     459,   479,   479,   459,   459,    34,    34,    34,    34,    34,
     479,   479,   479,   479,   224,   479,   479,   479,   224,   479,
     246,   459,    34,    34,    34,   479,   459,   224,   479,   479,
     479,   479,   479,   479,   479,   459,   459,   459,   459,    34,
     459,   459,   459,    34,   459,    34,   479,   458,   458,   458,
     460,   479,    34,   459,   459,   387,   322,   347,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   458,   459,
     479,   479,   479,   459,   479,   479,   479,    34,    34,    34,
     459,   459,   459,   459,   225,   459,   459,   459,   225,   459,
     479,   479,   459,   459,   459,   479,   225,   459,   459,   479,
     479,   479,   479,   479,   479,   479,    34,   479,   479,   479,
      34,   479,   459,   460,   479,   479,   479,   460,    34,   479,
     479,   388,   323,   348,   459,   459,   459,   459,   479,   459,
     459,   459,   479,   459,   479,   247,   459,   459,   459,   479,
     459,   459,    34,    34,    34,   479,   479,   479,   479,   226,
     479,   479,   479,   226,   479,   459,    34,   479,   479,   479,
     226,   479,   479,   479,   479,   479,   459,   459,   459,   459,
      34,   459,   459,   459,    34,   459,   479,   458,   460,   460,
     460,    34,   459,   459,   389,   324,   349,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   460,   479,   246,
     246,   246,   479,   479,   479,    34,    34,    34,   459,   459,
     459,   459,   227,   459,   459,   459,   227,   459,   247,   459,
      34,    34,    34,   227,   459,   459,   479,   479,   479,   479,
     479,   479,   479,    34,   479,   479,   479,    34,   479,    34,
     479,   458,   458,   458,    34,   479,   479,   390,   325,   350,
     459,   459,   459,   459,   479,   459,   459,   459,   479,   459,
     458,   459,   479,   479,   479,   479,   459,   459,    34,    34,
      34,   479,   479,   479,   479,   228,   479,   479,   479,   228,
     479,   479,   479,   459,   459,   459,   228,   479,   479,   479,
     479,   479,   459,   460,   459,   459,    34,   460,   459,   460,
      34,   459,   459,   460,   479,   479,   479,    34,   460,   459,
     391,   326,   351,   479,   479,   479,   479,   117,   479,   117,
     479,   479,   479,   459,   459,   459,   479,   117,   479,    34,
      34,    34,   459,   459,   459,   229,    58,   459,    58,   229,
     459,   459,   479,   479,   479,   229,    58,   459,   479,   479,
     479,   479,   479,   479,    34,   116,   479,   116,    34,   479,
     479,   460,   460,   460,    34,   116,   479,   392,   327,   352,
     460,   459,   459,   479,   479,   459,   479,   479,   459,   460,
     247,   247,   247,   479,   479,   459,    34,    34,    34,   479,
     479,   230,   222,   479,   230,   479,    34,    34,    34,   230,
     222,   479,   479,   479,   479,   459,   459,    34,    34,   460,
      34,   459,   458,   458,   458,    34,    34,   459,   393,   328,
     353,   479,   479,   479,   479,   117,   479,   479,   479,   479,
     479,   479,   479,   479,    34,    34,    34,   459,   459,   223,
      58,   459,   459,   459,   459,   223,   459,   479,   479,   479,
     479,   479,    34,   116,   479,   479,   479,   479,    34,   479,
     329,   354,   459,   459,   479,   479,   459,   459,   459,   459,
     479,   459,    34,    34,   479,   479,   224,   479,   479,   479,
     479,   224,   479,   479,   479,   459,   459,    34,   459,   460,
     460,   460,    34,   459,   330,   355,   479,   479,   479,   479,
     479,   479,    34,    34,   459,   460,   225,   459,   225,   460,
     479,   479,   479,   117,    34,   479,    34,   117,   331,   301,
     459,    58,   479,   459,   479,    58,    34,    34,   479,   116,
     226,   479,   226,   116,   479,   479,   459,   479,    34,   459,
      34,   479,   332,    82,   479,   479,   479,   479,   222,    34,
      34,   460,   227,   460,   227,    34,   479,   479,   117,    34,
     117,    34,   479,   333,   180,    58,   479,    58,   479,   223,
      34,    34,   116,   228,   116,   228,    34,   479,   479,   479,
      34,   479,    34,   479,   334,   181,   479,   479,   224,    34,
      34,   229,   229,    34,   479,   479,    34,    34,   479,   335,
     182,   479,   479,   225,    34,    34,   230,   230,    34,   479,
     479,    34,    34,   479,   336,   183,   479,   479,   226,    34,
      34,    34,   479,   479,   479,    82,   227,    34,    34,   479,
     479,   180,   228,    34,    34,   479,   479,   181,   229,    34,
      34,   479,   479,   182,   230,    34,    34,   479,   479,   183,
      34,   479
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   467,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   470,   470,   470,   470,   471,   471,   472,   472,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   475,   475,   475,
     475,   475,   475,   475,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   478,
     478,   478,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   480,   480,   480,   480,   480,   480,
     480,   480,   480,   481,   481,   482,   482
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
      20,    20,    21,    20,    29,     1,     3,     4,     4,     4,
       4,     6,     6,     6,    14,    23,    26,    26,    26,    32,
      29,    35,    29,    95,    80,    23,    11,    26,    35,    26,
      21,    14,    56,    27,    18,    33,    65,    71,    71,    51,
      57,    54,    54,    54,    78,    78,    92,    47,    59,    59,
      51,    59,    35,    38,    22,    22,    20,    22,    21,     1,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       3,     3,     4,     4,     1,     4,     1,     1,     1,     1,
       1,     1,     1,     6,     1,     2,     2,     3,     5,     3,
       1,     1,     2,     2,     3,     1,     2
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
  "number_of_integration_points", "EightNodeBrick", "TwentySevenNodeBrick",
  "EightNodeBrick_upU", "TwentyNodeBrick_uPU", "TwentyNodeBrick",
  "EightNodeBrick_up", "variable_node_brick_8_to_27",
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
  "sanisand2008", "camclay", "camclay_accelerated", "sanisand2004",
  "linear_elastic_crossanisotropic", "uniaxial_concrete02",
  "uniaxial_elastic_1d", "uniaxial_steel01", "uniaxial_steel02", "pisano",
  "PisanoLT", "CamClayLT", "VonMisesLT", "VonMisesArmstrongFrederickLT",
  "DruckerPragerLT", "DruckerPragerNonAssociateLinearHardeningLT",
  "DruckerPragerVonMisesLT", "DruckerPragerArmstrongFrederickLT",
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
  "CriticalState_kappa", "CriticalState_e0", "CriticalState_p0", "ax",
  "ay", "az", "verbose_level", "maximum_iterations", "tolerance",
  "yield_function_relative_tolerance", "stress_relative_tolerance", "beta",
  "gamma", "kappa", "lambda", "delta", "sigma0", "DOMAIN_", "startTime",
  "endTime", "Period", "Phase", "Amplitude", "frequency", "MaxTime",
  "frequency1", "frequency2", "frequency3", "frequency4",
  "stiffness_to_use_opt", "DAMPING_RAYLEIGH", "DAMPING_CAUGHEY3",
  "DAMPING_CAUGHEY4", "a0", "a1", "stiffness_to_use", "a2", "a3", "a4",
  "R0", "cR1", "cR2", "MASS", "mx", "my", "mz", "Imx", "Imy", "Imz",
  "equal_dof", "master", "slave", "dof_to_constrain", "of", "OUTPUT",
  "BINARY", "TEXT", "ENABLE", "DISABLE", "COMPRESSION", "SAVE",
  "NON_CONVERGED_ITERATIONS", "'('", "','", "')'", "'['", "']'", "'.'",
  "';'", "'{'", "'}'", "$accept", "dsl", "CMD_add", "CMD_fix", "dofchain",
  "explist", "CMD_define", "CMD_misc", "CMD_remove", "ADD_material",
  "ADD_element", "prompt", "exp", "stmt", "cpd_stmt", "list", YY_NULLPTR
  };


  const unsigned short int
  feiparser::yyrline_[] =
  {
       0,   305,   305,   319,   332,   345,   358,   366,   372,   373,
     374,   375,   376,   380,   390,   394,   432,   436,   449,   453,
     471,   486,   504,   530,   559,   589,   626,   657,   697,   717,
     750,   787,   802,   817,   869,   918,   960,   978,   997,  1016,
    1032,  1048,  1066,  1087,  1124,  1141,  1158,  1177,  1193,  1215,
    1239,  1262,  1302,  1316,  1338,  1367,  1371,  1376,  1382,  1393,
    1402,  1409,  1416,  1424,  1434,  1447,  1531,  1587,  1662,  1697,
    1711,  1724,  1751,  1789,  1817,  1830,  1846,  1869,  1879,  1893,
    1917,  1941,  1965,  1989,  2013,  2053,  2063,  2079,  2092,  2105,
    2119,  2131,  2143,  2155,  2176,  2194,  2230,  2258,  2286,  2314,
    2346,  2376,  2409,  2446,  2529,  2603,  2628,  2643,  2672,  2707,
    2740,  2767,  2787,  2848,  2874,  2893,  2920,  2967,  3015,  3066,
    3106,  3147,  3194,  3241,  3279,  3333,  3387,  3449,  3503,  3554,
    3605,  3658,  3710,  3747,  3785,  3811,  3837,  3861,  3886,  4078,
    4120,  4162,  4177,  4222,  4229,  4236,  4243,  4250,  4257,  4264,
    4270,  4277,  4285,  4293,  4301,  4309,  4317,  4321,  4327,  4332,
    4338,  4344,  4350,  4356,  4362,  4370,  4375,  4381,  4386,  4391,
    4396,  4401,  4406,  4414,  4445,  4450,  4454,  4463,  4485,  4510,
    4530,  4548,  4559,  4569,  4575,  4583,  4587
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
     458,   460,    37,    35,   459,    36,   463,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   464,
       2,    34,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   461,     2,   462,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   465,     2,   466,     2,     2,     2,     2,
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
     453,   454,   455,   456,   457
    };
    const unsigned int user_token_number_max_ = 704;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 7823 "feiparser.tab.cc" // lalr1.cc:1155
#line 4615 "feiparser.yy" // lalr1.cc:1156


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
	if (type.compare("UNDRAINED_CYCLIC_TRIAXIAL_LOADING_STRESS_CONTROL") == 0) return 4;
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



