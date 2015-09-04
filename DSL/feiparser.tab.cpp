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
#line 274 "feiparser.yy" // lalr1.cc:407

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
    #line 279 "feiparser.yy" // lalr1.cc:725
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
#line 300 "feiparser.yy" // lalr1.cc:847
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
#line 314 "feiparser.yy" // lalr1.cc:847
    {
		//theDomain.Print(cerr);
		cerr << "Not currently working. Should be implemented as API function and used here." << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 676 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 324 "feiparser.yy" // lalr1.cc:847
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
#line 347 "feiparser.yy" // lalr1.cc:847
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
#line 360 "feiparser.yy" // lalr1.cc:847
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
#line 371 "feiparser.yy" // lalr1.cc:847
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
#line 459 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new DslWhos(global_variables, locked_global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 739 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 467 "feiparser.yy" // lalr1.cc:847
    {
		cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 749 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 472 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 755 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 473 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 761 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 474 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 767 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 475 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 773 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 476 "feiparser.yy" // lalr1.cc:847
    {   (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 779 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 480 "feiparser.yy" // lalr1.cc:847
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
#line 490 "feiparser.yy" // lalr1.cc:847
    {  (yylhs.value.exp) = new DslParseError("add", yylineno); nodes.push((yylhs.value.exp)); }
#line 797 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 495 "feiparser.yy" // lalr1.cc:847
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
#line 554 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp);}
#line 845 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 559 "feiparser.yy" // lalr1.cc:847
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
#line 862 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 571 "feiparser.yy" // lalr1.cc:847
    { (yylhs.value.exp) = (yystack_[0].value.exp); }
#line 868 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 597 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 886 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 615 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();


		args.push_back((yystack_[10].value.exp));    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[6].value.exp));    signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back((yystack_[0].value.exp));   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_node, args, signature, "add_load_selfweight_to_node");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 904 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 634 "feiparser.yy" // lalr1.cc:847
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
#line 929 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 660 "feiparser.yy" // lalr1.cc:847
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
#line 957 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 689 "feiparser.yy" // lalr1.cc:847
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
#line 986 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 719 "feiparser.yy" // lalr1.cc:847
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
#line 1022 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 756 "feiparser.yy" // lalr1.cc:847
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
#line 1052 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 787 "feiparser.yy" // lalr1.cc:847
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
#line 1091 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 825 "feiparser.yy" // lalr1.cc:847
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
#line 1112 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 847 "feiparser.yy" // lalr1.cc:847
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
#line 1144 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 881 "feiparser.yy" // lalr1.cc:847
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
#line 1178 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 914 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back((yystack_[2].value.exp));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 1194 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 929 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[6].value.exp));         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back(new Number(-1, ESSIunits::unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		nodes.pop(); //pop 1 exps
		nodes.push((yylhs.value.exp));
	}
#line 1210 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 951 "feiparser.yy" // lalr1.cc:847
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
#line 1256 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 1002 "feiparser.yy" // lalr1.cc:847
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
#line 1300 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 1119 "feiparser.yy" // lalr1.cc:847
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
#line 1318 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 1137 "feiparser.yy" // lalr1.cc:847
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
#line 1337 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 1156 "feiparser.yy" // lalr1.cc:847
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
#line 1357 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 1175 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_element(int elementNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("element_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1374 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 1191 "feiparser.yy" // lalr1.cc:847
    {
		//add_damping_to_node(int nodeNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("node_number",    &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("damping_number",    &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1391 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 1207 "feiparser.yy" // lalr1.cc:847
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
#line 1410 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 1225 "feiparser.yy" // lalr1.cc:847
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
#line 1432 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 1246 "feiparser.yy" // lalr1.cc:847
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
#line 1470 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 1326 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("hdf5_file",            &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<int,string>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1486 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 1341 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("section_number",           &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("material_number",           &isAdimensional));
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("thickness",           &isLength));

		(yylhs.value.exp) = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1503 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 1361 "feiparser.yy" // lalr1.cc:847
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
#line 1522 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 1387 "feiparser.yy" // lalr1.cc:847
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
#line 1563 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 1427 "feiparser.yy" // lalr1.cc:847
    {
		args.clear();           signature.clear();
		args.push_back((yystack_[2].value.exp));     signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1578 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 1441 "feiparser.yy" // lalr1.cc:847
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
#line 1601 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 1463 "feiparser.yy" // lalr1.cc:847
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
#line 1630 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 1491 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.clear();
							dofchain_holder.push_back(dof2number(*(yystack_[0].value.ident)));
						}
#line 1639 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 1495 "feiparser.yy" // lalr1.cc:847
    {
							dofchain_holder.push_back(dof2number(*(yystack_[1].value.ident)));
						}
#line 1647 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 1505 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1657 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 1514 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
		nodes.push((yylhs.value.exp));
	}
#line 1667 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 1521 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1677 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 1528 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[1].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
		nodes.push((yylhs.value.exp));
	}
#line 1688 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 1536 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("level", &isAdimensional));
		(yylhs.value.exp) = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
		nodes.push((yylhs.value.exp));
	}
#line 1699 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 1546 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&disable_element_output,args,signature,"disable_element_output");
		nodes.push((yylhs.value.exp));
	}
#line 1709 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 1556 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("factor", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1723 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 1569 "feiparser.yy" // lalr1.cc:847
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
#line 1772 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 1617 "feiparser.yy" // lalr1.cc:847
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
#line 1810 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 1654 "feiparser.yy" // lalr1.cc:847
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
#line 1840 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 1683 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("gamma", &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("beta", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 1855 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 1697 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("alpha", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

		nodes.pop(); //pop 2 exps
		nodes.push((yylhs.value.exp));
	}
#line 1869 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 1710 "feiparser.yy" // lalr1.cc:847
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
#line 1891 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 1737 "feiparser.yy" // lalr1.cc:847
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
#line 1930 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 1775 "feiparser.yy" // lalr1.cc:847
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
#line 1959 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 1803 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[4].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1973 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 1816 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("time_step", &isTime));
		args.push_back((yystack_[7].value.exp)); signature.push_back(this_signature("nsteps", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

		nodes.pop();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 1990 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 1836 "feiparser.yy" // lalr1.cc:847
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
#line 2010 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 1855 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back
		(this_signature("number_of_modes", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2025 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 1872 "feiparser.yy" // lalr1.cc:847
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
#line 2047 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 1896 "feiparser.yy" // lalr1.cc:847
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
#line 2069 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 1920 "feiparser.yy" // lalr1.cc:847
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
#line 2091 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 1944 "feiparser.yy" // lalr1.cc:847
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
#line 2113 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 1968 "feiparser.yy" // lalr1.cc:847
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
#line 2135 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1990 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		(yylhs.value.exp) = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

	}
#line 2146 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 2006 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2160 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 2019 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2174 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 2032 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

		//pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2189 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 2046 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
			 //pop 1
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2202 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 2058 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("number", &isAdimensional));
			 (yylhs.value.exp) = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2215 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 2082 "feiparser.yy" // lalr1.cc:847
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
#line 2231 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 2107 "feiparser.yy" // lalr1.cc:847
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
#line 2256 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 2139 "feiparser.yy" // lalr1.cc:847
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
#line 2278 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 2168 "feiparser.yy" // lalr1.cc:847
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
#line 2300 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 2200 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",          &isAdimensional));
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",       &isPressure));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

		(yylhs.value.exp) = new FeiDslCaller12<int, int, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_isotropic_hardening");
		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2324 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 2232 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",            &isAdimensional));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",      &isDensity));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",  &isPressure));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("von_mises_radius",  &isPressure));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate", &isPressure));
		args.push_back((yystack_[9].value.exp)); signature.push_back(this_signature("initial_confining_stress", &isPressure));
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("tolerance_1", &isAdimensional));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("tolerance_2", &isAdimensional));


		(yylhs.value.exp) = new FeiDslCaller10<int, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening");
		for(int ii = 1;ii <=10; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2347 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 2266 "feiparser.yy" // lalr1.cc:847
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
#line 2373 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 2302 "feiparser.yy" // lalr1.cc:847
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
#line 2398 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 2336 "feiparser.yy" // lalr1.cc:847
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
#line 2422 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 2368 "feiparser.yy" // lalr1.cc:847
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
#line 2445 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 2396 "feiparser.yy" // lalr1.cc:847
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
#line 2476 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 2432 "feiparser.yy" // lalr1.cc:847
    {

		//int add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening(int MaterialNumber,
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
		args.push_back((yystack_[6].value.exp)); signature.push_back(this_signature("druckerprager_k",  &isPressure));  // 5
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7



		(yylhs.value.exp) = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening");
		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 2507 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 2492 "feiparser.yy" // lalr1.cc:847
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
#line 2561 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 2559 "feiparser.yy" // lalr1.cc:847
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
#line 2595 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 2604 "feiparser.yy" // lalr1.cc:847
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
#line 2625 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 2658 "feiparser.yy" // lalr1.cc:847
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
#line 2675 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 2718 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[35].value.exp)); signature.push_back(this_signature("number",                         &isAdimensional));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("algoritm",                      &isAdimensional));
		args.push_back((yystack_[30].value.exp)); signature.push_back(this_signature("mass_density",                   &isDensity));
		args.push_back((yystack_[27].value.exp)); signature.push_back(this_signature("elastic_modulus",               &isPressure));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("poisson_ratio",                 &isAdimensional));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("druckerprager_angle",           &isAdimensional));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",      &isPressure));
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
#line 2705 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 2756 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[29].value.exp)); signature.push_back(this_signature("number",                        &isAdimensional));
		args.push_back((yystack_[24].value.exp)); signature.push_back(this_signature("mass_density",                  &isDensity));
		args.push_back((yystack_[21].value.exp)); signature.push_back(this_signature("elastic_modulus",              &isPressure));
		args.push_back((yystack_[18].value.exp)); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
		args.push_back((yystack_[15].value.exp)); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
		args.push_back((yystack_[12].value.exp)); signature.push_back(this_signature("isotropic_hardening_rate",     &isPressure));
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
#line 2732 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 2794 "feiparser.yy" // lalr1.cc:847
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
#line 2764 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 2835 "feiparser.yy" // lalr1.cc:847
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
#line 2793 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 2874 "feiparser.yy" // lalr1.cc:847
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
#line 2822 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 2910 "feiparser.yy" // lalr1.cc:847
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
#line 2848 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 2941 "feiparser.yy" // lalr1.cc:847
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
#line 2868 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 2960 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();

		args.push_back((yystack_[8].value.exp)); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back((yystack_[3].value.exp)); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back((yystack_[0].value.exp)); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		(yylhs.value.exp) = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push((yylhs.value.exp));
	}
#line 2884 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 2982 "feiparser.yy" // lalr1.cc:847
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
#line 2907 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 3014 "feiparser.yy" // lalr1.cc:847
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
#line 2933 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 3046 "feiparser.yy" // lalr1.cc:847
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
#line 2956 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 3078 "feiparser.yy" // lalr1.cc:847
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
#line 2985 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 3116 "feiparser.yy" // lalr1.cc:847
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
#line 3014 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 3147 "feiparser.yy" // lalr1.cc:847
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
#line 3031 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 3175 "feiparser.yy" // lalr1.cc:847
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
#line 3062 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 3217 "feiparser.yy" // lalr1.cc:847
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
#line 3086 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 3243 "feiparser.yy" // lalr1.cc:847
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
#line 3109 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 3267 "feiparser.yy" // lalr1.cc:847
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
#line 3127 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 3287 "feiparser.yy" // lalr1.cc:847
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
#line 3150 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 3311 "feiparser.yy" // lalr1.cc:847
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
#line 3176 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 3340 "feiparser.yy" // lalr1.cc:847
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
#line 3220 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 3387 "feiparser.yy" // lalr1.cc:847
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
#line 3264 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 3434 "feiparser.yy" // lalr1.cc:847
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
#line 3309 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 3483 "feiparser.yy" // lalr1.cc:847
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
#line 3353 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 3530 "feiparser.yy" // lalr1.cc:847
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
#line 3400 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 3581 "feiparser.yy" // lalr1.cc:847
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
#line 3437 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 3621 "feiparser.yy" // lalr1.cc:847
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
#line 3475 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 3663 "feiparser.yy" // lalr1.cc:847
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
#line 3512 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 3711 "feiparser.yy" // lalr1.cc:847
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
#line 3548 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 3751 "feiparser.yy" // lalr1.cc:847
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
#line 3582 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 3790 "feiparser.yy" // lalr1.cc:847
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
#line 3631 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 3849 "feiparser.yy" // lalr1.cc:847
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
#line 3685 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 3913 "feiparser.yy" // lalr1.cc:847
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
#line 3739 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 3974 "feiparser.yy" // lalr1.cc:847
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
#line 3785 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 4080 "feiparser.yy" // lalr1.cc:847
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
#line 3827 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 4163 "feiparser.yy" // lalr1.cc:847
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
#line 3858 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 4196 "feiparser.yy" // lalr1.cc:847
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
#line 3882 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 4222 "feiparser.yy" // lalr1.cc:847
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
#line 3906 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 4248 "feiparser.yy" // lalr1.cc:847
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
#line 3928 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 4272 "feiparser.yy" // lalr1.cc:847
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
#line 3951 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 4298 "feiparser.yy" // lalr1.cc:847
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
#line 3971 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 4486 "feiparser.yy" // lalr1.cc:847
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
#line 4017 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 4528 "feiparser.yy" // lalr1.cc:847
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
#line 4063 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 4570 "feiparser.yy" // lalr1.cc:847
    {
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		(yylhs.value.errcode) = 0;
	}
#line 4079 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 4585 "feiparser.yy" // lalr1.cc:847
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
#line 4128 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 4630 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Plus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4139 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 4637 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ((yylhs.value.exp));
	}
#line 4150 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 4644 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Times ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4161 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 4651 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Divide ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4172 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 4658 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Modulo ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4183 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 4665 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Pow ((yystack_[2].value.exp), (yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4194 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 4672 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new LogicalNot ((yystack_[0].value.exp));
		nodes.pop ();  // The same as above.
		nodes.push ((yylhs.value.exp));
	}
#line 4204 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 4678 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Plus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4215 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 4685 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Minus (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4227 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 4693 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Times (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4239 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 4701 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Divide (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4251 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 4709 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Modulo (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4263 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 4717 "feiparser.yy" // lalr1.cc:847
    {
		Expression* varref = new VariableReference(*(yystack_[2].value.ident), global_variables);
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), new Pow (varref, (yystack_[0].value.exp)), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4275 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 4725 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4283 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 4729 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Minus (new Number(0.0, (yystack_[0].value.exp) -> value().Getunit() ), (yystack_[0].value.exp));
		nodes.pop ();
		nodes.push ((yylhs.value.exp));
	}
#line 4293 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 4735 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Number (*(yystack_[0].value.value));
		nodes.push ((yylhs.value.exp));
	}
#line 4302 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 4740 "feiparser.yy" // lalr1.cc:847
    {

		(yylhs.value.exp) = new VariableReference(*(yystack_[0].value.ident),global_variables);
		nodes.push((yylhs.value.exp));
	}
#line 4312 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 4746 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Assign(*(yystack_[2].value.ident), (yystack_[0].value.exp), global_variables, locked_global_variables);
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4322 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 4752 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new ConvertUnits(*(yystack_[2].value.ident), *(yystack_[0].value.ident), global_variables);

		nodes.push((yylhs.value.exp));
	}
#line 4332 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 4758 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiMath(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4342 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 4764 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new UnitTypeTest(*(yystack_[3].value.ident), (yystack_[1].value.exp));
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4352 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 165:
#line 4770 "feiparser.yy" // lalr1.cc:847
    {
		string temp = *(yystack_[0].value.ident);
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		(yylhs.value.exp) = new FeiString(temp);
		nodes.push((yylhs.value.exp));
	}
#line 4364 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 166:
#line 4778 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push((yylhs.value.exp));
	}
#line 4374 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 167:
#line 4784 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push((yylhs.value.exp));
	}
#line 4383 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 168:
#line 4789 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push((yylhs.value.exp));
	}
#line 4392 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 169:
#line 4794 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4401 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 170:
#line 4799 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push((yylhs.value.exp));
	}
#line 4410 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 171:
#line 4804 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push((yylhs.value.exp));
	}
#line 4419 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 172:
#line 4809 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push((yylhs.value.exp));
	}
#line 4428 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 173:
#line 4817 "feiparser.yy" // lalr1.cc:847
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
#line 4460 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 174:
#line 4848 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4469 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 175:
#line 4853 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4477 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 176:
#line 4857 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[1].value.exp);
		args.clear();
		signature.clear();
	}
#line 4487 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 177:
#line 4866 "feiparser.yy" // lalr1.cc:847
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
#line 4510 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 178:
#line 4888 "feiparser.yy" // lalr1.cc:847
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
#line 4536 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 179:
#line 4913 "feiparser.yy" // lalr1.cc:847
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
#line 4560 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 180:
#line 4933 "feiparser.yy" // lalr1.cc:847
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
#line 4582 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 181:
#line 4951 "feiparser.yy" // lalr1.cc:847
    {
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		(yylhs.value.exp) = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push((yylhs.value.exp));
	}
#line 4597 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 182:
#line 4962 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = new Empty();
		nodes.pop();
		nodes.push((yylhs.value.exp));
	}
#line 4607 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 183:
#line 4972 "feiparser.yy" // lalr1.cc:847
    {
		// empty statement generates an  empty expression
		(yylhs.value.exp) = new Empty();
		nodes.push((yylhs.value.exp));
	}
#line 4617 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 184:
#line 4978 "feiparser.yy" // lalr1.cc:847
    {
		// Used to initialize a list of statements (compound statement)
		(yylhs.value.exp) = (yystack_[1].value.exp);
	}
#line 4626 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 185:
#line 4986 "feiparser.yy" // lalr1.cc:847
    {
		(yylhs.value.exp) = (yystack_[0].value.exp);
	}
#line 4634 "feiparser.tab.cc" // lalr1.cc:847
    break;

  case 186:
#line 4990 "feiparser.yy" // lalr1.cc:847
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
#line 4650 "feiparser.tab.cc" // lalr1.cc:847
    break;


#line 4654 "feiparser.tab.cc" // lalr1.cc:847
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


  const short int feiparser::yypact_ninf_ = -398;

  const signed char feiparser::yytable_ninf_ = -1;

  const short int
  feiparser::yypact_[] =
  {
     249,  -398,   648,  -398,  -398,  -397,  -328,   577,   577,  -398,
    -398,    86,  -398,  -398,  -398,  -398,  -398,  -398,  -398,   577,
     577,    31,  -398,  -398,    48,    41,  -398,  5902,  -310,  -284,
      91,    94,   136,   161,   356,    85,    33,    37,   -53,   -52,
     577,  -398,  -259,  -398,  -398,  -398,  -398,  -398,   157,    39,
    -398,   160,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   -18,   -18,  -398,   125,   125,  -104,    61,    62,  -398,
   10188,   577,   163,    64,    66,    68,    69,    70,    71,  -220,
      74,    73,    84,    22,    96,  -398,   577,  -206,   109,   116,
     119,   120,   121,   122,  -164,   111,   165,   169,   175,   -44,
     171,   166,   100,  9879,   196,   145,   577,  -136,  -398,  -135,
    -398,   270,  -398,  -398,  -398,   577,   577,   577,   577,   577,
     577,   577,  -398,  -398, 10188, 10188, 10188, 10188, 10188, 10188,
   10188,   399,   473,  2740,   261,  -398,   577,   577,   577, 10188,
    -398,   577,   577,  -398,   577,   577,   577,   577,   167,   173,
     577,   577,   211,   229,    81,   164,   577,   577,   577,   577,
     577,   577,   231,   158,  -398,   -74,  -398,   182,   190,   197,
     218,  -138,   205,  -398,   296,  9889,  -398,  -398,  -398,   185,
     185,    29,    29,     2,   125,   268,  -398,  -398,  -398,  2793,
    -107,  -105, 10188, 10188, 10188, 10023,  9831,  9840,  9934,  9943,
    9819,   -87,   577,  9849,  6053,   206,   207,   -98,   209,  9973,
    9989, 10188, 10188, 10188, 10188,   212,   577,   290,   117,   -37,
     276,   233,   172,   176,   -45,   -12,  -398,  -398,  -398,  -398,
    -398,   -80, 10105, 10016,   -23,   284,     1,  -374,   287,  9691,
    -171,   310,   291,   577,   341,   577,   -22,   235,   577, 10188,
     577,   316,   317,   242,   322,   138,   187,   -50,   201,   293,
     295,   297,   577,   -83,   -69,   269,   271,   -66,   272,   273,
     286,   275,   283,   302,   303,   307,   308,   309,   318,   319,
     321,   323,   324,   333,   334,   336,   118,   128,   154,   170,
     174,   177,   179,   180,   181,   215,   216,   217,   219,   220,
     222,   224,   225,   228,   232,   248,    51,   115,   106,   107,
     241,   252,   267,   277,   285,   288,   294,   340,   343,   383,
     394,   434,   444,   454,   458,   456,   537,   329,   298,   577,
     463,  1751,  -398, 10188,  -398,   235,  -398,  -398, 10188,  6010,
     577,   577,   577,   577,   408,   409,   411,   412,   401,  -398,
     493,   496,   612,   577,   480,   577,   481,   483,   484,   577,
     485,   486,   487,   577,   488,   497,   499,   500,   501,   502,
     503,   511,   512,   514,   515,   516,   518,   520,   521,   600,
     610,   611,   619,   620,   623,   624,   627,   629,   630,   631,
     632,   642,   646,   647,   649,   655,   659,   706,   707,   708,
     709,   710,   720,   725,   726,   727,   728,   731,   732,   733,
     391,   577,   577,   577,   577,   251,   382,   390,   577,   577,
     743,   745,  6044,   577,   753,  -398,   754, 10188,  6001,  9904,
   10188,   435,   669,   685,   686,   621,   766,   767,   577,  8771,
     389,  8780,   392,   393,   400,  8789,   403,   404,   405,  8798,
     406,   407,   413,   414,   415,   423,   425,   426,   427,   429,
     430,   431,   432,   433,   437,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   676,  9860,  9869,  9913,
   10188, 10188,   787,   788,   814, 10013, 10188,   577,   577,   820,
     735,   577,   577,   824,   748,   684,   805,   806,   811,   759,
     577,   577,  1794,   699,   577,   700,   577,   577,   577,   717,
     577,   577,   577,   718,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,  7822,
    7834,  7847,  7861,  7870,  7879,  7891,  7900,  7909,  6580,  6404,
    6413,  6589,  7923,  7934,  7943,  7952,  7961,  7970,  6892,  6325,
    7813,  7981,  7996,  7538,  8008,  7547,  6071,  8017,  8028,  8037,
     764,   -29,    -9,   776,   577,   577,   577,   498,  9954,  7559,
     577,   790,  1857,  6019,   577,   771,   793,   795,   796,   807,
     858, 10188, 10047,   577,   738,  1888,   739,  1917,  1926,  1948,
     744,  2004,  2032,  2046,   747,  2094,  2103,  2115,  2147,  2159,
    2201,  2224,  2260,  2272,  2306,  2340,  2386,  2467,  2495,  2520,
     893,   894,   896,   897,   898,   899,   908,   916,   917,   919,
     957,   959,   960,   962,   964,   965,   967,   970,   990,  1003,
    1004,  1006,  1007,  1008,  1009,  1010,  1011,  1019,  1020,  1022,
    1023,  -398,   930,   942,   943,  -398,   951,   955,   956,   -75,
    5873,  5882,  5891,  1014,  1013,  1036,  6034,  1015,  1046,  1054,
   10188,   577,  1031,   577,   577,   577,   981,  1064,   510,   868,
     577,   869,   577,   577,   577,   877,   577,   577,   577,   878,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,  1092,  1101,  1102,  1044,
    1063,  1024,  1107,  1108,  1112,  1113,  1128,  1053,  1055,   577,
    1130,   982,   577,   577, 10188,  1056,  9623,  9632,  9641,   577,
     577,  1061,  1062,  2554,  1095,  2573,  2606,  2815,  1097,  2839,
    2851,  2860,  1098,  2875,  2884,   519,   585,   603,   712,  2893,
    2902,  2911,  2920,   746,   755,  2934,  2943,   838,  7568,  7577,
    7590,  7603,  7616,  7625,  7634,  7646,  7655,  6608,  6386,  6395,
    6571,  7664,  7679,  7689,  7698,  7707,  7716,  6877,  6313, 10188,
    6339,  6348,  6255,  7725,  6264,  7737,  7752,  7763,  7772,   577,
     577,   577,  1096,  1100,   794,   577,   577,   577,   577,   577,
     577,   577,  8165,   577,   816,    59, 10188,   577,  1177,  1178,
    1179,  9652, 10056,   577,  1110,   577,  1111,   577,   577,   577,
    1114,   577,   577,   577,  1118,   577,   577,  1116,  1005,  1026,
    1027,   577,   577,   577,   577,  1117,  1016,   577,   577,  1028,
    1200,  1202,  1203,  1211,  1217,  1218,  1227,  1230,  1231,  1232,
    1233,  1234,  1235,  1236,  1240,  1242,  1243,  1245,  1247,  1248,
    1249,  1270,  1276,  1277,  1278,  1279,  1280,  1289,  1298,  1299,
   10188, 10065,  9671,   577,   577,   577,  9804,  9739,  5845,  5803,
    5812,   875, 10188,  1301, 10188,   577,  1302,  9661,   577,   577,
     577,  1303,  1304,  9998,   841,  2953,   903,  2962,  2971,  2986,
     927,  2995,  3004,  3013,   928,  3022,  3031,   577,  1308,  1309,
    1310,  3040,  3049,   847,  3058,  1287,  1321,  3067,  3076,  1322,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
    1331,  1335, 10188, 10188,  3085,  1337,  1338,  1339,  1340,  1341,
     235,   577, 10188,   577,  1370,  9572,  9583,  9592,   577,   577,
    -398,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,  9925,   577,   577,   577,   577,   577,
    1294,   577,  1296,   577,   577,   577,   577, 10188,  7433,  7442,
    7452,  7461,  7470,  7479,  7492,  7507,  6820,  5949,  5960,  7783,
    7369,  7378,  7387,  7399,  7408,  7417,  6920,  6304,  5900,  5743,
    6228,  7517,  6237,  6273, 10188,  7526,  7356,   577,  1401,   577,
     577,   577,  1030,   577,   577,  -398,  8076,  1590,   577,  1375,
    1376,  1397,  9602, 10032,  3094,  3103,  3127,  3136,  3145,  3154,
    3178,  3187,  3196,  3205,  3219,  3232,  3241,  1324,  8048,  8057,
    8066,   856,   900,  1374,   909,   577,  8300,  3256,  3265,  9963,
    1400,  1411,  1412,  1414,  1415,  1416,  1417,  1418,  1427,  1430,
    1431,  1432,  1434,  1437,  1448,  1450,  1451,  1452,  1453,  1455,
    1457,  1458,  1459,  1460,  1461,  1463,  1464,  1465,  9680,  -398,
    3274,  9750,  9779,  -398,  5855,  5770,  1467,  1468,  9611,   577,
     577,   577,  1469,  1470,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,  1388,  1471,  1472,
    1473,  1363,  1405,  1398,  1406,  8472,  1491,   577,   577,  1474,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,  1492,   577,
    1494,  1495,  1496,  1497,   577,   577,  1499,  9523,  9532,  9541,
     577,   577,  3283,  3292,  3301,  3315,  3325,  3334,  3343,  3352,
    3367,  3376,  3385,  3394,  3403,  1424,   577,   577,   577,  1490,
    1493,   577,  1498,  1515,   577,  3412,  3421,  1445,  7037,  6967,
    6929,  6938,  7310,  7046,  6976,  7319,  6844,  5978,  5992,  6598,
    6991,  7005,  7328,  7342,  7059,  7068,  6910,  5969,  5821,  5734,
    6199,  7077,  6210,  6246,  6947,  6956,  1544,  3430,   577,   577,
    1206,   577, 10188,  1314,   577,  1556,  1557,  1559,  9551, 10188,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,  8440,  8454,  8463,  1485,  1500,  8177,
    1501,   577,  8291,   577,   577,   577,  1560,  1569,  1570,  1572,
    1578,  1580,  1581,  1582,  1583,  1584,  1586,  1587,  1588,  1589,
    1597,  1598,  1600,  1608,  1609,  1612,  1615,  1617,  1620,  1625,
    1627,  1630,  1631,  1632,  -398,   577,  9794,  9700,  -398,  5864,
    1633,  9563,   577,   577,   577,  1634,  3439,  3448,  3457,  3466,
    3475,  3484,  3493,  3508,  3517,  3526,  3535,  3544,  3559,  8089,
    1643,  1652,  1653,   577,   577,  1655,   577,  8104,  1656,  3568,
    3577, 10188,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     178,  1657,  1658,  1659,   577,  1660, 10188, 10188, 10188,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,  1662,   577,   577,   577,  8186,  8195,   577,
    8204,  1671,   577,   577,   577, 10074,  7092,  7106,  7118,  7266,
    9234,  7127,  7276,  6829,  7792,  7804,  6555,  7136,  7148,  7288,
    7297, 10085,  9243,  6901,  6295,  5779,  1323,  6168,  9252,  6177,
    6219,  7014,  7025,   577,  1517,   577,   577,  1288,   772,   577,
   10188,  3586,  3599,  3613,  3622,  3637,  3646,  3655,  3664,  3673,
    3682,  3695,  3706,  3715,   577,  8396,  8405,  8425,  1681,  1684,
   10188,  1710,   577,  8280,  3724,  3733,  1712,  1713,  1715,  1718,
    1723,  1725,  1727,  1732,  1740,  1741,  1749,  1750,  1758,  1763,
    1765,  1766,  1768,  1769,  1770,  1771,  1773,  1782,  1784,  1792,
    1793,  1802,  1804,  1805,  3748,   -25,  9711,  9759,  -398,  1806,
   10188,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,  8309,  1808,  1809,  1811,   577,   577,
     577, 10188,  1814,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,  1285,   577,  1819,  1824,   577,  3757,
     949,  3766,  3775,   996,  3784,  3793,  3802,  3811,  1012,  3820,
    1038,  3829,  1825,   577,   577,   577, 10188, 10188, 10188,   577,
    1047,  3838,  9424, 10094, 10103,  9261,  7157,  9039,  9273,  7166,
    6810,  6362,  6371,  6546, 10132,  9284,  7182,  7191,  9433,  9052,
   10188,  6283,  5752,  5909,  6137,  9061,  6150,  6186, 10188, 10188,
    3847,   577, 10188,   577,   577, 10188,   577,  1777,   577,   577,
    1778,   577,   577,   577,   577,  1780,   577,  1787,   577,   577,
    8318,  8330,  8346,  8271,  1790,   577,  1826,  1827,  1828,  1868,
    1872,  1873,  1874,  1875,  1876,  1877,  1878,  1886,  1887,  1895,
    1896,  1898,  1901,  1913,  1914,  1915,  1924,  1925,  1933,  1934,
    1936,   577,  3856,  9768,  9721,  3865,  1855,  3874,  3889,  1918,
    3898,  3907,  3916,  3925,  1920,  3940,  1921,  3949,  8223,  1937,
    1938,  1939,  1941,  1922,  3958,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
    3967,   577,  1947,  1956,   577,  1882,   577,   577,  1883,   577,
     577,   577,   577,  1904,   577,  1907,   577,  1984,   577,   577,
     577,   577,  1911,   577,  9293,  9444,  9453,  9071, 10141,  8811,
    9080,  9304,  6801,  6859,  6868,  6537,  9462,  9093, 10150,  9313,
    9322,  8826, 10188, 10188,  5830,  8113,  8835,  8122,  6159,   577,
    3979,   577,   577,  3994,   577,  4003,  4018,   577,  4027,  4036,
    4045,  4054,   577,  4063,   577,  4075,  1613,  8355,  8364,  8376,
    8213,   577,  4087,  2001,  2002,  2003,  2011,  2012,  2014,  2015,
    2016,  2018,  2019,  2020,  2021,  2023,  2025,  2028,  2029,  2030,
    2031,  2039,  2040,  2042,  2043,  2044,   331,   577,  9730, 10188,
     577, 10188,   577,   577,  8741,   577,   577,   577,   577,  8751,
     577, 10188,   577,   577,  2053,  2056,  2057,  2059, 10188,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,  1931,  4096,  2061,  1057,  4105,  4114,
    2064,  4129,  4138,  4147,  4156,  2069,  4165,  4174,  4183,   577,
     577,   577,  1667,  4192,  9102,  9333,  9344,  8849,  9472, 10188,
    8858,  9111,  6792,  7200,  7210,  6528,  9353,  8869,  9484,  9120,
    9133, 10188,  5794,  7219, 10188,  7228,  7242,  1066,   -16,   577,
     577,  1871,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,  8134,  8143,  8152,   577,   577,  2072,  2073,
    2074,  2075,  2101,  2102,  2110,  2111,  2122,  2125,  2126,  2127,
    2128,  2129,  2142,  2146,  2154,  2157,  2158,  2167,  2086,  1736,
     577,  1075, 10188,  2145,  4201,  4210,  8708,  4219,  4228,  4237,
    4246,  8717,  4255,  4270,  4279,  2171,  2172,  2173,  4288,  4297,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     -11,   577, 10188,  -398,  2113,   577,   577,  2194,   577,   577,
     577,   577,  2197,   577,   577,   577,   577,   577,   577,   577,
     577,  8879,  9143,  9152, 10188,  9362, 10188,  8888,  6777, 10159,
    9374,  6516,  9163, 10188,  9383,  8897,  8911,  5761,  6119,  6128,
    6110,  1829,   577,  4306,   577,  4321,  4330,   577,  4339,  4348,
    4359,  4375,   577,  4384,  4399,  1085,  8238,  8251,  8260,  4408,
    4417,  2209,  2211,  2212,  2213,  2214,  2215,  2217,  2218,  2219,
    2222,  2223,  2231,  2232,  2234,  2235,  2240,  2245,   577, 10188,
     577, 10188,   577,   577,  8677,   577,   577,   577,   577,  8688,
     577,   577,  2045,  2267,  2270,  2271,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,  4426,  4435,  4444,  4455,  2279,
    4468,  4477,  4486,  4495,  2280,  4510,  4519,  2282,  1919,  1923,
    1935,  1094,  4528, 10188,  8938,  8952,  9172, 10188,  6768,  9493,
    9181,  6506,  8968,  9193, 10188, 10188, 10188, 10188, 10188,  6080,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,  1940,   577,   577,   577,  -398,   577,  2283,  2296,
    2299,  2301,  2303,  2304,  2305,  2313,  2316,  2317,  4537,  4546,
    4555,  4564,  8644,  4573,  4582,  4591,  4600,  8653,  4609,  4618,
     577,  4627,  4636,  4651,  4660,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,  2318,
     577,   577,   577,   577,  2320,   577,   577,  4669,   577,   577,
     577,   577, 10188, 10188,  8977,  6755,  9392,  8991,  6497, 10188,
    9000,  6062,  4678,  4687,  4702,  4711,   577,  4720,  4729,  4739,
    4748,   577,  4757,  4766,   577,  4780,  4789,  4798,  4807,  2333,
    2336,  2337,  2338,  2339,  2347,  2350,   577,   577,   577,   577,
    8605,   577,   577,   577,   577,  8615,   577,   577,  4816,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
    4825,  4835,  4849,  4858,  2351,  4867,  4876,  4891,  4900,  2352,
    4909,  4918,   577,  1181,  1204,  1220,  4927, 10188,  6742,  9203,
   10188,  6488, 10188, 10188,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,  1263,  2151,  2152,  2153,
     577,  2360,  2361,  2362,  4936,  4945,  4954,  4963,  8576,  4972,
    4981,  4990,  4999,  8587,  5008,  5017,  2164,  2363,  2373,  2377,
    5032,   577,   577,   577,   577,   577,   577,   577,  2378,   577,
     577,   577,   577,  2379,   577,   577,  2385,  2007,  2017,  2022,
     577,  6733,  9010,  6479,  5041,  5050,  5059,  5068,   577,  5083,
    5092,  5101,  5111,   577,  5120,  5129,  2024,   577,   577,   577,
    5138,  2393,  2394,  2400,   577,   577,   577,   577,  8547,   577,
     577,   577,   577,  8556,   577,   577,   577,  5147,  5161,  5170,
     577,   577,   577,   577,  5179,  1282,  5188,  5197,  2402,  1291,
    5207,  1404,  1420,  2405,  5216,  5230,  5239,   577,   577,   577,
    5248,  6718, 10188,  6470,   577,  -398,   577,   577,   577,  2332,
     577,  2334,  2341,   577,   577,   577,   577,  5257,  5272,  5281,
     577,  2417,  2419,  5290,  5299,  5308,  8511,  2397,  5317,  2398,
    2399,  8522,  5326,  5335,  5344,   577,   577,   577,  5353,   577,
     577,   577,   577,   577,  2424,  2349,   577,  2353,  2354,  2427,
     577,   577,   577,  1440,  1475,  1484,   577,  6709,  6456,  1562,
    5362,  5371,   577,   577,  5380,   577,   577,   577,  5389,  5398,
    1601,  2227,  2228,  2229,  5413,  2428,  2434,  -398,   577,   577,
    8481,  8760,   577, 10188, 10188,  8490,   577,   577,  -398,  2438,
    2444,  2445,   577,   577,   577,  5422,  5431,  2447,  2448,  1636,
    2449,  5440,  5449,  2076,  2078,  2079,  5464,  6700,  6441,   577,
     577,   577,   577,  2376,   577,   577,   577,   577,   577,   577,
     577,  2454,  2459,  5473,  5483, 10188,  8732,  2436, 10188,  5492,
    5501,  5510,  5519,  5528,  5542,   577,   577,   577,   577,  2461,
    2388,   577,   577,   577,   577,   577,   577,  6691,  6432,  5551,
    5560,   577,   577,  5569,  5579,  5588,  5597,  5611,  5620,  2475,
    2477,   577,   577,  8697, 10188,   577,   577,   577,   577,   577,
     577,   577,   577,  5629,  5638,  2480,  5653,  5662,  1645,  1664,
    1673,  5671,  6677,  6423,   577,   577,   577,   577,   577,  -398,
    -398,  -398,   577,  2481,  2484,  5680,  1733,  8668,  5689,  1742,
    1774,   577,   577,   577,  2408,  2488,   577,  2412,  2413,  6668,
    7257,  5698,  2478,   577,  5707,  2479,  2482,  2491,  2505,   577,
    2431,  8629,   577,  2432,  2437,   577,   577,  5716,   577,  2528,
    5725,   577,   577,  6657, 10168,   577, 10188,   577,   577, 10188,
   10188,  2530,  2531,  1785,  8596,  1879,   577,   577,  2455,  2533,
    2457,  6648,  9502,  2516,   577,  2523,  2548,  2549,  2474,  8567,
    2497,   577,   577,   577,  2553,   577,  6636,  9401, 10188,   577,
   10188,  2564,  2580,  8531,   577,   577,  2583,  6627,  9212,   577,
    2589,  2590,  8502,   577,   577,  2591,  6617,  9021,   577,  2593,
    2594, 10188,   577,   577, 10179, 10188,  2595,   577,  9514,  2596,
     577,  9412,  2597,   577,  9221,  2598,   577,  9030,  2599,   577,
   10188
  };

  const unsigned char
  feiparser::yydefact_[] =
  {
       0,   142,   160,   159,   165,     0,     0,     0,     0,    15,
     180,     0,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     9,     8,     0,     0,   181,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   174,     0,    10,    12,    13,    11,    14,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   158,   150,     0,     0,     0,     3,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
      54,     0,   176,     1,   141,     0,     0,     0,     0,     0,
       0,     0,   175,   162,   151,   152,   153,   154,   155,   156,
     161,     0,     0,     0,   177,   179,     0,     0,     0,     7,
      66,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,     0,     0,
       0,     0,     0,    77,     0,     0,    55,    58,   157,   144,
     145,   146,   147,   148,   149,   143,   164,   163,   183,   185,
       0,     0,     5,     4,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    78,    81,    80,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    56,   186,   184,
     178,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,    57,    48,    51,    47,    50,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,    64,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    39,     0,     0,     0,     0,    44,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    45,     0,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
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
       0,     0,    46,     0,     0,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    76,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   139,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   135,   136,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,    93,    94,
       0,     0,    23,     0,     0,    42,     0,     0,     0,     0,
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
       0,     0,   109,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
       0,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   117,     0,     0,     0,     0,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,    88,     0,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,   102,   103,   108,   110,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    90,     0,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      97,     0,   101,   113,     0,     0,     0,     0,     0,     0,
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
       0,     0,    96,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,     0,     0,
       0,     0,     0,   124,   126,     0,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,     0,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   130,
     131,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,     0,     0,   122,
     120,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95
  };

  const short int
  feiparser::yypgoto_[] =
  {
    -398,  -398,  -398,  -398,  -243,  -398,  -398,  -398,  -398,  -398,
    -398,    -7,     6,   -55,  2458
  };

  const short int
  feiparser::yydefgoto_[] =
  {
      -1,    42,    43,    44,   336,    45,    46,    47,    85,   143,
      48,    49,   189,   134,   190
  };

  const short int
  feiparser::yytable_[] =
  {
      61,    62,   107,   109,   337,   136,    50,   135,   322,   323,
     324,   353,    64,    65,    70,    59,   115,   116,   117,   118,
     119,   120,   752,   354,   121,   355,   222,   103,   359,   223,
     224,   317,   318,   111,     2,     3,     4,   356,     5,     6,
     360,   120,   167,   168,   121,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   114,   320,   321,    12,    13,    14,
      15,    16,    17,    18,   139,   753,    19,   119,   120,   327,
     328,   121,    20,   115,   116,   117,   118,   119,   120,   154,
     259,   121,   334,   260,    60,    66,    67,   261,    68,    63,
     335,    72,   425,   115,   116,   117,   118,   119,   120,   175,
      71,   121,   662,   663,   664,    86,   665,   666,   179,   180,
     181,   182,   183,   184,   185,   115,   116,   117,   118,   119,
     120,   257,    87,   121,   258,   346,   347,   667,   668,   192,
     193,   194,  1594,  1595,   195,   196,   230,   197,   198,   199,
     200,  1979,  1980,   203,   204,    88,  2061,  2062,    89,   209,
     210,   211,   212,   213,   214,   104,   105,   113,   106,   112,
       2,     3,     4,   123,     5,     6,     7,   121,   140,     8,
     137,   138,     9,   141,    10,   142,    11,   144,   145,   146,
     147,   148,   150,    12,    13,    14,    15,    16,    17,    18,
      90,    91,    19,   151,    92,   239,    93,   149,    20,    94,
      21,    22,    23,    24,   152,   155,    25,   153,    26,   249,
      27,    28,   115,   116,   117,   118,   119,   120,   156,    95,
     121,   117,   118,   119,   120,   157,    29,   121,   158,   159,
     160,   161,    30,    31,    32,    33,   331,   162,   333,   163,
      96,   338,    97,   339,    98,   164,    99,    34,    35,   100,
       1,   165,     2,     3,     4,   352,     5,     6,     7,   166,
     169,     8,   170,   171,     9,    36,    10,   173,    11,   174,
     176,   177,   191,   201,   205,    12,    13,    14,    15,    16,
      17,    18,   202,   206,    19,   215,   208,   216,   218,   217,
      20,   220,    21,    22,    23,    24,   219,   221,    25,   225,
      26,   226,    27,    28,   115,   116,   117,   118,   119,   120,
      -1,    86,   121,   229,   133,   238,   243,   242,    29,   244,
     245,   248,   422,   250,    30,    31,    32,    33,   252,   251,
     253,   254,   262,   427,   428,   429,   430,   255,   319,    34,
      35,   325,   256,   329,   332,   330,   439,   335,   441,   340,
     341,   342,   445,   108,   110,   343,   449,    36,   344,     2,
       3,     4,   345,     5,     6,   115,   116,   117,   118,   119,
     120,   348,   349,   121,   350,   357,   351,   358,   361,   362,
     363,   364,    12,    13,    14,    15,    16,    17,    18,   365,
     400,    19,   379,   399,     2,     3,     4,    20,     5,     6,
      69,   133,   380,   497,   498,   499,   500,   501,   366,   367,
      28,   505,   506,   368,   369,   370,   510,    12,    13,    14,
      15,    16,    17,    18,   371,   372,    19,   373,   381,   374,
     375,   522,    20,   115,   116,   117,   118,   119,   120,   376,
     377,   121,   378,    40,   382,    28,   401,   402,   383,   410,
     101,   384,   411,   385,   386,   387,   926,   122,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   388,
     389,   390,   412,   391,   392,   496,   393,   207,   394,   395,
     588,   589,   396,   413,   592,   593,   397,   115,   116,   117,
     118,   119,   120,   601,   602,   121,   403,   605,   102,   607,
     608,   609,   398,   611,   612,   613,   404,   615,   616,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   405,   414,   115,   116,   117,   118,   119,   120,
     415,   406,   121,   115,   116,   117,   118,   119,   120,   407,
     416,   121,   408,    37,   417,   418,    38,    39,   409,    40,
     419,   420,   423,   421,   431,    41,   435,   670,   671,   672,
       2,     3,     4,   676,     5,     6,   432,   680,   433,   434,
     436,  1473,  1474,   437,   440,   442,   688,   443,   444,   446,
     447,   448,   450,    12,    13,    14,    15,    16,    17,    18,
     515,   451,    19,   452,   453,   454,   455,   456,    20,   115,
     116,   117,   118,   119,   120,   457,   458,   121,   459,   460,
     461,    28,   462,   465,   463,   464,   502,   115,   116,   117,
     118,   119,   120,   466,   467,   121,   115,   116,   117,   118,
     119,   120,   468,   469,   121,    37,   470,   471,    38,    39,
     472,    40,   473,   474,   475,   476,    51,    41,    52,    53,
      54,    55,    56,    57,   764,   477,   766,   767,   768,   478,
     479,    58,   480,   773,   178,   775,   776,   777,   481,   779,
     780,   781,   482,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   483,
     484,   485,   486,   487,  1893,  1894,   115,   116,   117,   118,
     119,   120,   842,   488,   121,   845,   846,  1075,   489,   490,
     491,   492,   851,   852,   493,   494,   495,   503,    40,   115,
     116,   117,   118,   119,   120,   504,   507,   121,   508,   516,
     115,   116,   117,   118,   119,   120,   511,   512,   121,   115,
     116,   117,   118,   119,   120,   517,   518,   121,   519,   520,
     521,   524,   580,    40,   526,   527,   115,   116,   117,   118,
     119,   120,   528,   186,   121,   530,   531,   532,   534,   535,
     584,   585,   910,   911,   912,   536,   537,   538,   916,   917,
     918,   919,   920,   921,   922,   539,   924,   540,   541,   542,
     927,   543,   544,   545,   546,   547,   933,   586,   935,   548,
     937,   938,   939,   590,   941,   942,   943,   594,   945,   946,
     595,   596,   597,   598,   951,   952,   953,   954,   599,   600,
     957,   958,   115,   116,   117,   118,   119,   120,   604,   606,
     121,   115,   116,   117,   118,   119,   120,   187,   669,   121,
     115,   116,   117,   118,   119,   120,   610,   614,   121,   661,
     681,   673,   677,   682,   683,   684,   992,   993,   994,   115,
     116,   117,   118,   119,   120,   686,   685,   121,  1002,   689,
     691,  1005,  1006,  1007,   771,   695,   715,   716,   699,   717,
     718,   719,   720,   867,   115,   116,   117,   118,   119,   120,
    1024,   721,   121,   115,   116,   117,   118,   119,   120,   722,
     723,   121,   724,  1037,  1038,  1039,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,   115,   116,   117,   118,   119,   120,    40,
     725,   121,   726,   727,  1076,   728,  1077,   729,   730,   868,
     731,  1082,  1083,   732,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,   869,  1098,  1099,
    1100,  1101,  1102,   733,  1104,   438,  1106,  1107,  1108,  1109,
     115,   116,   117,   118,   119,   120,   734,   735,   121,   736,
     737,   738,   739,   740,   741,   746,   115,   116,   117,   118,
     119,   120,   742,   743,   121,   744,   745,   747,   748,   751,
    1138,   749,  1140,  1141,  1142,   750,  1144,  1145,   757,   759,
     758,  1148,   115,   116,   117,   118,   119,   120,   761,   762,
     121,   115,   116,   117,   118,   119,   120,   763,   765,   121,
     769,   115,   116,   117,   118,   119,   120,   770,  1175,   121,
     115,   116,   117,   118,   119,   120,   772,   774,   121,   115,
     116,   117,   118,   119,   120,   778,   782,   121,   832,   115,
     116,   117,   118,   119,   120,   829,   870,   121,   115,   116,
     117,   118,   119,   120,   830,   831,   121,   833,   834,   591,
     835,   836,  1217,  1218,  1219,   837,   838,  1222,  1223,  1224,
    1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,
     875,   839,   840,   843,   841,   847,   844,   853,   854,   876,
    1245,  1246,  1539,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,
    1275,   856,  1277,   860,   864,   913,   915,  1282,  1283,   914,
     928,   929,   930,  1288,  1289,   115,   116,   117,   118,   119,
     120,   925,   947,   121,   934,   936,   948,   955,   940,  1304,
    1305,  1306,   944,   960,  1309,   961,   962,  1312,   115,   116,
     117,   118,   119,   120,   963,   956,   121,   949,   950,   959,
     964,   965,   879,  1011,   115,   116,   117,   118,   119,   120,
     966,  1030,   121,   967,   968,   969,   970,   971,   972,   973,
    1171,  1346,  1347,   974,  1349,   975,   976,  1351,   977,  1000,
     978,   979,   980,  1356,  1357,  1358,  1359,  1360,  1361,  1362,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,   115,   116,   117,
     118,   119,   120,   981,  1377,   121,  1379,  1380,  1381,   982,
     983,   984,   985,   986,  1172,  1013,   115,   116,   117,   118,
     119,   120,   987,  1174,   121,   115,   116,   117,   118,   119,
     120,   988,   989,   121,  1001,  1003,  1008,  1009,  1410,  1017,
    1021,  1025,  1026,  1027,  1032,  1416,  1417,  1418,   115,   116,
     117,   118,   119,   120,  1033,  1036,   121,   115,   116,   117,
     118,   119,   120,  1657,  1067,   121,  1437,  1438,  1068,  1440,
    1070,  1071,  1072,  1073,  1074,  1445,  1446,  1447,  1448,  1449,
    1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,
    1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,
    1470,  1471,  1472,  1078,  1103,  1105,  1139,  1478,  1149,  1150,
    1660,  1143,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1665,  1495,  1496,  1497,
    1151,  1173,  1500,  1180,  1167,  1503,  1504,  1505,   115,   116,
     117,   118,   119,   120,  1181,  1182,   121,  1183,  1184,  1185,
    1186,  1187,  1667,  1235,   115,   116,   117,   118,   119,   120,
    1188,  1674,   121,  1189,  1190,  1191,  1534,  1192,  1536,  1537,
    1193,  1941,  1540,  1239,   115,   116,   117,   118,   119,   120,
    1978,  1194,   121,  1195,  1196,  1197,  1198,  1554,  1199,  2023,
    1200,  1201,  1202,  1203,  1204,  1561,  1205,  1206,  1207,  2112,
    1214,  1215,  1220,  1221,  1236,  1237,  1238,  1241,  2186,   115,
     116,   117,   118,   119,   120,  1240,  1242,   121,   115,   116,
     117,   118,   119,   120,  1244,  1276,   121,  1278,  1279,  1280,
    1281,  1247,  1284,  1303,  1599,  1600,  1601,  1602,  1603,  1604,
    1605,  1606,  1607,  1608,  1609,  1610,  1611,  1307,  1311,  1344,
    1308,  1616,  1617,  1618,  1315,  1310,  1620,  1621,  1622,  1623,
    1624,  1625,  1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,
    1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,  1643,
    1644,  1645,  1646,  1647,  1648,  1649,  1650,  1348,  1652,  1352,
    1353,  1655,  1354,  1382,  1373,  2337,   115,   116,   117,   118,
     119,   120,  1383,  1384,   121,  1385,  1670,  1671,  1672,  1374,
    1376,  1386,  1673,  1387,  1388,  1389,  1390,  1391,  2338,  1392,
    1393,  1394,  1395,  1535,   115,   116,   117,   118,   119,   120,
    1396,  1397,   121,  1398,  2339,   115,   116,   117,   118,   119,
     120,  1399,  1400,   121,  1702,  1401,  1703,  1704,  1402,  1705,
    1403,  1707,  1708,  1404,  1710,  1711,  1712,  1713,  1405,  1715,
    1406,  1717,  1718,  1407,  1408,  1409,  1414,  1419,  1724,  1538,
     115,   116,   117,   118,   119,   120,  1434,  2356,   121,   115,
     116,   117,   118,   119,   120,  1435,  1436,   121,  1439,  1442,
    1475,  1476,  1477,  1479,  1750,  1494,  2445,  1651,   115,   116,
     117,   118,   119,   120,  1502,  2449,   121,   115,   116,   117,
     118,   119,   120,  1350,  1558,   121,  1527,  1559,  1774,  1775,
    1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,
    1786,  1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,
    1796,  1797,  1798,  1560,  1800,  1565,  1566,  1803,  1567,  1805,
    1806,  1568,  1808,  1809,  1810,  1811,  1569,  1813,  1570,  1815,
    1571,  1817,  1818,  1819,  1820,  1572,  1822,   115,   116,   117,
     118,   119,   120,  1573,  1574,   121,   115,   116,   117,   118,
     119,   120,  1575,  1576,   121,   115,   116,   117,   118,   119,
     120,  1577,  1846,   121,  1848,  1849,  1578,  1851,  1579,  1580,
    1854,  1581,  1582,  1583,  1584,  1859,  1585,  1861,   115,   116,
     117,   118,   119,   120,  1868,  1586,   121,  1587,  2451,   115,
     116,   117,   118,   119,   120,  1588,  1589,   121,   115,   116,
     117,   118,   119,   120,  2452,  1590,   121,  1591,  1592,  1598,
    1895,  1613,  1614,  1897,  1615,  1898,  1899,  1619,  1901,  1902,
    1903,  1904,  1653,  1906,  2511,  1907,  1908,  1654,  1669,  1725,
    1726,  1727,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,
    1931,  1932,  1933,  1934,  1935,  1936,  1937,  1706,  1709,  2512,
    1714,   115,   116,   117,   118,   119,   120,  1716,  2513,   121,
    1723,  1728,  1953,  1954,  1955,  1729,  1730,  1731,  1732,  1733,
    1734,  1735,  1755,   115,   116,   117,   118,   119,   120,  1736,
    1737,   121,   115,   116,   117,   118,   119,   120,  1738,  1739,
     121,  1740,  1981,  1982,  1741,  1984,  1985,  1986,  1987,  1988,
    1989,  1990,  1991,  1992,  1993,  1994,  1742,  1743,  1744,  1998,
    1999,   115,   116,   117,   118,   119,   120,  1745,  1746,   121,
     115,   116,   117,   118,   119,   120,  1747,  1748,   121,  1749,
    1768,  1769,  1770,  2022,  1771,  1758,  2517,  1763,  1765,  1772,
    1801,  1983,   115,   116,   117,   118,   119,   120,  1147,  1802,
     121,  1804,  1807,  2041,  2042,  2043,  2044,  2045,  2046,  2047,
    2048,  2049,  2050,  2051,  2052,  2053,  2054,  2055,  2056,  2057,
    2058,  2059,  2060,  1812,  2063,  2528,  1814,  1816,  2065,  2066,
    1821,  2068,  2069,  2070,  2071,  1863,  2073,  2074,  2075,  2076,
    2077,  2078,  2079,  2080,  1870,  1871,  1872,  1938,   115,   116,
     117,   118,   119,   120,  1873,  1874,   121,  1875,  1876,  1877,
    2553,  1878,  1879,  1880,  1881,  2099,  1882,  2101,  1883,  2629,
    2104,  1884,  1885,  1886,  1887,  2109,   115,   116,   117,   118,
     119,   120,  1888,  1889,   121,  1890,  1891,  1892,  2630,  1956,
     115,   116,   117,   118,   119,   120,  1909,  2631,   121,  1910,
    1911,  2135,  1912,  2136,  1940,  2137,  2138,  1944,  2140,  2141,
    2142,  2143,  1949,  2145,  2146,  2000,  2001,  2002,  2003,  2151,
    2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,
    2162,  2163,  2164,  2165,  2166,  2167,  2168,  2169,   115,   116,
     117,   118,   119,   120,  2004,  2005,   121,   115,   116,   117,
     118,   119,   120,  2006,  2007,   121,   424,  2644,  2021,   115,
     116,   117,   118,   119,   120,  2008,  2647,   121,  2009,  2010,
    2011,  2012,  2013,  2198,  2199,  2200,  2201,  2202,  2203,  2204,
    2205,  2206,  2207,  2208,  2209,  2014,  2211,  2212,  2213,  2015,
    2214,   115,   116,   117,   118,   119,   120,  2016,  2648,   121,
    2017,  2018,  2020,   115,   116,   117,   118,   119,   120,  2688,
    2019,   121,  2024,  2237,  2036,  2037,  2038,   603,  2242,  2243,
    2244,  2245,  2246,  2247,  2248,  2249,  2250,  2251,  2252,  2253,
    2254,  2255,  2064,  2257,  2258,  2259,  2260,  2067,  2262,  2263,
    2072,  2265,  2266,  2267,  2268,   115,   116,   117,   118,   119,
     120,  2098,  2118,   121,  2119,  2120,  2121,  2122,  2123,  2280,
    2124,  2125,  2126,   678,  2285,  2127,  2128,  2288,   115,   116,
     117,   118,   119,   120,  2129,  2130,   121,  2131,  2132,  2300,
    2301,  2302,  2303,  2133,  2305,  2306,  2307,  2308,  2134,  2310,
    2311,  2147,  2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,
    2321,  2322,  2323,  2690,   115,   116,   117,   118,   119,   120,
    2148,   690,   121,  2149,  2150,  2336,   115,   116,   117,   118,
     119,   120,  2174,  2179,   121,  2182,  2215,  2344,  2345,  2346,
    2347,  2348,  2349,  2350,  2351,  2352,  2353,  2354,  2355,  2216,
     692,  2183,  2217,  2360,  2218,  2184,  2219,  2220,  2221,   693,
     115,   116,   117,   118,   119,   120,  2222,  2185,   121,  2223,
    2224,  2256,  2210,  2261,  2381,  2382,  2383,  2384,  2385,  2386,
    2387,   694,  2389,  2390,  2391,  2392,  2293,  2394,  2395,  2294,
    2295,  2296,  2297,  2400,   115,   116,   117,   118,   119,   120,
    2298,  2408,   121,  2299,  2328,  2333,  2413,  2357,  2358,  2359,
    2417,  2418,  2419,  2361,  2362,  2363,  2377,  2424,  2425,  2426,
    2427,  2376,  2429,  2430,  2431,  2432,  2378,  2434,  2435,  2436,
    2379,  2388,  2393,  2440,  2441,  2442,  2443,   696,  2396,  2397,
     115,   116,   117,   118,   119,   120,  2421,  2422,   121,  2398,
    2457,  2458,  2459,  2423,  2399,  2448,  2416,  2463,  2453,  2464,
    2465,  2466,  2467,  2468,  2469,   697,  2471,  2472,  2473,  2474,
    2479,  2470,  2480,  2478,  2485,  2487,  2488,  2502,  2503,   698,
    2507,  2533,  2505,  2506,  2529,  2530,  2531,  2534,  2493,  2494,
    2495,  2543,  2497,  2498,  2499,  2500,  2501,  2544,  2545,  2504,
    2551,  2552,  2554,  2508,  2509,  2510,  2567,  2575,  2557,  2514,
    2558,  2559,  2576,  2580,  2591,  2520,  2521,  2592,  2523,  2524,
    2525,   115,   116,   117,   118,   119,   120,   700,  2611,   121,
    2612,  2535,  2536,  2626,  2641,  2539,   701,  2642,  2652,  2541,
    2542,  2653,  2655,  2656,  2665,  2546,  2547,  2548,   702,   115,
     116,   117,   118,   119,   120,  2660,  2663,   121,  2666,  2664,
    2668,  2671,  2563,  2564,  2565,  2566,  2672,  2568,  2569,  2570,
    2571,  2572,  2573,  2574,   115,   116,   117,   118,   119,   120,
     703,  2677,   121,  2686,  2687,  2693,  2694,  2695,  2587,  2588,
    2589,  2590,   704,  2698,  2593,  2594,  2595,  2596,  2597,  2598,
    2700,  2701,  2702,  2703,  2603,  2604,  2709,     0,   115,   116,
     117,   118,   119,   120,  2613,  2614,   121,  2714,  2616,  2617,
    2618,  2619,  2620,  2621,  2622,  2623,  2705,   115,   116,   117,
     118,   119,   120,  2715,   705,   121,  2719,  2635,  2636,  2637,
    2638,  2639,  2723,  2724,  2728,  2640,  2732,  2733,  2737,  2740,
    2743,  2746,  2749,     0,  2649,  2650,  2651,   706,     0,  2654,
     115,   116,   117,   118,   119,   120,  2661,   228,   121,     0,
       0,     0,  2667,     0,     0,  2670,     0,     0,  2673,  2674,
       0,  2676,     0,     0,  2679,  2680,     0,     0,  2683,     0,
    2684,  2685,     0,   707,     0,     0,     0,     0,     0,  2691,
    2692,     0,     0,     0,     0,   708,     0,  2699,     0,     0,
       0,     0,     0,     0,  2706,  2707,  2708,     0,  2710,     0,
       0,     0,  2713,     0,     0,     0,     0,  2717,  2718,     0,
       0,     0,  2722,     0,     0,     0,  2726,  2727,     0,   709,
       0,  2731,     0,     0,     0,  2734,  2735,     0,     0,     0,
    2738,     0,     0,  2741,     0,     0,  2744,     0,     0,  2747,
       0,     0,  2750,     2,     3,     4,     0,     5,     6,     7,
       0,     0,     8,   710,     0,     9,     0,    10,     0,    11,
       0,     0,     0,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,     0,     0,    19,     0,     0,     0,     0,
       0,    20,     0,    21,    22,    23,    24,     0,     0,    25,
       0,    26,     0,    27,    28,     0,     2,     3,     4,   711,
       5,     6,     7,     0,     0,     8,     0,     0,     9,    29,
      10,     0,    11,     0,     0,    30,    31,    32,    33,    12,
      13,    14,    15,    16,    17,    18,     0,     0,    19,     0,
      34,    35,     0,     0,    20,     0,    21,    22,    23,    24,
       0,     0,    25,     0,    26,     0,    27,    28,    36,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,    29,     0,     0,     0,     0,     0,    30,    31,
      32,    33,     0,   115,   116,   117,   118,   119,   120,     0,
     712,   121,     0,    34,    35,   115,   116,   117,   118,   119,
     120,     0,     0,   121,   115,   116,   117,   118,   119,   120,
       0,    36,   121,     0,     0,     0,     0,     0,   713,   115,
     116,   117,   118,   119,   120,     0,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,   714,     0,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,     0,   855,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   857,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,     0,     0,   858,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,    37,     0,     0,    38,
      39,     0,    40,     0,     0,     0,     0,     0,    41,     0,
     188,   115,   116,   117,   118,   119,   120,     0,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,    37,
       0,     0,    38,    39,     0,    40,     0,     0,     0,     0,
       0,    41,   115,   116,   117,   118,   119,   120,     0,     0,
     121,   115,   116,   117,   118,   119,   120,     0,   859,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,   861,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,   862,     0,   115,   116,   117,   118,
     119,   120,     0,   863,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,     0,   865,     0,
     115,   116,   117,   118,   119,   120,     0,   866,   121,   115,
     116,   117,   118,   119,   120,     0,   871,   121,   115,   116,
     117,   118,   119,   120,     0,   872,   121,   115,   116,   117,
     118,   119,   120,     0,   873,   121,   115,   116,   117,   118,
     119,   120,     0,   874,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,   877,     0,   115,
     116,   117,   118,   119,   120,     0,   878,   121,     0,   115,
     116,   117,   118,   119,   120,     0,  1012,   121,   115,   116,
     117,   118,   119,   120,     0,  1014,   121,   115,   116,   117,
     118,   119,   120,     0,  1015,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,  1016,
       0,   115,   116,   117,   118,   119,   120,     0,  1018,   121,
     115,   116,   117,   118,   119,   120,     0,  1019,   121,   115,
     116,   117,   118,   119,   120,     0,  1020,   121,   115,   116,
     117,   118,   119,   120,     0,  1022,   121,   115,   116,   117,
     118,   119,   120,     0,  1023,   121,   115,   116,   117,   118,
     119,   120,     0,  1028,   121,   115,   116,   117,   118,   119,
     120,     0,  1029,   121,   115,   116,   117,   118,   119,   120,
       0,  1031,   121,   115,   116,   117,   118,   119,   120,     0,
    1034,   121,   115,   116,   117,   118,   119,   120,     0,  1035,
     121,   115,   116,   117,   118,   119,   120,     0,  1069,   121,
     115,   116,   117,   118,   119,   120,     0,  1154,   121,   115,
     116,   117,   118,   119,   120,     0,  1155,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,     0,     0,
    1156,     0,   115,   116,   117,   118,   119,   120,     0,  1157,
     121,   115,   116,   117,   118,   119,   120,     0,  1158,   121,
     115,   116,   117,   118,   119,   120,     0,  1159,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,  1160,     0,   115,   116,   117,   118,   119,   120,     0,
    1161,   121,   115,   116,   117,   118,   119,   120,     0,  1162,
     121,   115,   116,   117,   118,   119,   120,     0,  1163,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,  1164,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,  1165,     0,   115,   116,   117,
     118,   119,   120,     0,  1166,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,  1177,
       0,   115,   116,   117,   118,   119,   120,     0,  1178,   121,
     115,   116,   117,   118,   119,   120,     0,  1209,   121,   115,
     116,   117,   118,   119,   120,     0,  1290,   121,   115,   116,
     117,   118,   119,   120,     0,  1291,   121,   115,   116,   117,
     118,   119,   120,     0,  1292,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,  1293,   115,
     116,   117,   118,   119,   120,     0,     0,   121,  1294,     0,
     115,   116,   117,   118,   119,   120,     0,  1295,   121,   115,
     116,   117,   118,   119,   120,     0,  1296,   121,   115,   116,
     117,   118,   119,   120,     0,  1297,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,     0,     0,
    1298,     0,   115,   116,   117,   118,   119,   120,     0,  1299,
     121,   115,   116,   117,   118,   119,   120,     0,  1300,   121,
     115,   116,   117,   118,   119,   120,     0,  1301,   121,   115,
     116,   117,   118,   119,   120,     0,  1302,   121,   115,   116,
     117,   118,   119,   120,     0,  1313,   121,   115,   116,   117,
     118,   119,   120,     0,  1314,   121,   115,   116,   117,   118,
     119,   120,     0,  1345,   121,   115,   116,   117,   118,   119,
     120,     0,  1420,   121,   115,   116,   117,   118,   119,   120,
       0,  1421,   121,   115,   116,   117,   118,   119,   120,     0,
    1422,   121,   115,   116,   117,   118,   119,   120,     0,  1423,
     121,   115,   116,   117,   118,   119,   120,     0,  1424,   121,
     115,   116,   117,   118,   119,   120,     0,  1425,   121,   115,
     116,   117,   118,   119,   120,     0,  1426,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,  1427,     0,   115,   116,   117,   118,   119,   120,     0,
    1428,   121,   115,   116,   117,   118,   119,   120,     0,  1429,
     121,   115,   116,   117,   118,   119,   120,     0,  1430,   121,
     115,   116,   117,   118,   119,   120,     0,  1431,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,  1432,     0,   115,   116,   117,   118,   119,   120,
       0,  1443,   121,   115,   116,   117,   118,   119,   120,     0,
    1444,   121,   115,   116,   117,   118,   119,   120,     0,  1541,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,  1542,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,  1543,     0,   115,   116,
     117,   118,   119,   120,     0,  1544,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,     0,     0,
    1545,     0,   115,   116,   117,   118,   119,   120,     0,  1546,
     121,   115,   116,   117,   118,   119,   120,     0,  1547,   121,
     115,   116,   117,   118,   119,   120,     0,  1548,   121,   115,
     116,   117,   118,   119,   120,     0,  1549,   121,   115,   116,
     117,   118,   119,   120,     0,  1550,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,  1551,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,  1552,
       0,   115,   116,   117,   118,   119,   120,     0,  1553,   121,
     115,   116,   117,   118,   119,   120,     0,  1563,   121,   115,
     116,   117,   118,   119,   120,     0,  1564,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,  1593,     0,   115,   116,   117,   118,   119,   120,     0,
    1656,   121,   115,   116,   117,   118,   119,   120,     0,  1658,
     121,   115,   116,   117,   118,   119,   120,     0,  1659,   121,
     115,   116,   117,   118,   119,   120,     0,  1661,   121,   115,
     116,   117,   118,   119,   120,     0,  1662,   121,   115,   116,
     117,   118,   119,   120,     0,  1663,   121,   115,   116,   117,
     118,   119,   120,     0,  1664,   121,   115,   116,   117,   118,
     119,   120,     0,  1666,   121,   115,   116,   117,   118,   119,
     120,     0,  1668,   121,   115,   116,   117,   118,   119,   120,
       0,  1675,   121,   115,   116,   117,   118,   119,   120,     0,
    1701,   121,   115,   116,   117,   118,   119,   120,     0,  1751,
     121,   115,   116,   117,   118,   119,   120,     0,  1754,   121,
     115,   116,   117,   118,   119,   120,     0,  1756,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,  1757,     0,   115,   116,   117,   118,   119,   120,
       0,  1759,   121,   115,   116,   117,   118,   119,   120,     0,
    1760,   121,   115,   116,   117,   118,   119,   120,     0,  1761,
     121,   115,   116,   117,   118,   119,   120,     0,  1762,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,     0,  1764,     0,   115,   116,   117,   118,   119,
     120,     0,  1766,   121,   115,   116,   117,   118,   119,   120,
       0,  1773,   121,   115,   116,   117,   118,   119,   120,     0,
    1799,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,  1847,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,     0,  1850,     0,   115,
     116,   117,   118,   119,   120,     0,  1852,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,  1853,     0,   115,   116,   117,   118,   119,   120,     0,
    1855,   121,   115,   116,   117,   118,   119,   120,     0,  1856,
     121,   115,   116,   117,   118,   119,   120,     0,  1857,   121,
     115,   116,   117,   118,   119,   120,     0,  1858,   121,   115,
     116,   117,   118,   119,   120,     0,  1860,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,  1862,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
    1869,     0,   115,   116,   117,   118,   119,   120,     0,  1939,
     121,   115,   116,   117,   118,   119,   120,     0,  1942,   121,
     115,   116,   117,   118,   119,   120,     0,  1943,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,  1945,     0,   115,   116,   117,   118,   119,   120,
       0,  1946,   121,   115,   116,   117,   118,   119,   120,     0,
    1947,   121,   115,   116,   117,   118,   119,   120,     0,  1948,
     121,   115,   116,   117,   118,   119,   120,     0,  1950,   121,
     115,   116,   117,   118,   119,   120,     0,  1951,   121,   115,
     116,   117,   118,   119,   120,     0,  1952,   121,   115,   116,
     117,   118,   119,   120,     0,  1957,   121,   115,   116,   117,
     118,   119,   120,     0,  2025,   121,   115,   116,   117,   118,
     119,   120,     0,  2026,   121,   115,   116,   117,   118,   119,
     120,     0,  2028,   121,   115,   116,   117,   118,   119,   120,
       0,  2029,   121,   115,   116,   117,   118,   119,   120,     0,
    2030,   121,   115,   116,   117,   118,   119,   120,     0,  2031,
     121,   115,   116,   117,   118,   119,   120,     0,  2033,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,     0,  2034,     0,   115,   116,   117,   118,   119,
     120,     0,  2035,   121,   115,   116,   117,   118,   119,   120,
       0,  2039,   121,   115,   116,   117,   118,   119,   120,     0,
    2040,   121,   115,   116,   117,   118,   119,   120,     0,  2100,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,  2102,     0,   115,   116,   117,   118,
     119,   120,     0,  2103,   121,   115,   116,   117,   118,   119,
     120,     0,  2105,   121,   115,   116,   117,   118,   119,   120,
       0,  2106,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,  2107,   115,   116,   117,   118,   119,   120,     0,
       0,   121,   115,   116,   117,   118,   119,   120,  2108,     0,
     121,   115,   116,   117,   118,   119,   120,  2110,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,  2111,     0,   115,   116,   117,   118,   119,   120,
       0,  2116,   121,   115,   116,   117,   118,   119,   120,     0,
    2117,   121,   115,   116,   117,   118,   119,   120,     0,  2170,
     121,   115,   116,   117,   118,   119,   120,     0,  2171,   121,
     115,   116,   117,   118,   119,   120,     0,  2172,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,  2173,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,  2175,     0,   115,   116,   117,   118,   119,   120,     0,
    2176,   121,   115,   116,   117,   118,   119,   120,     0,  2177,
     121,   115,   116,   117,   118,   119,   120,     0,  2178,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,     0,  2180,     0,   115,   116,   117,   118,   119,
     120,     0,  2181,   121,   115,   116,   117,   118,   119,   120,
       0,  2187,   121,   115,   116,   117,   118,   119,   120,     0,
    2225,   121,   115,   116,   117,   118,   119,   120,     0,  2226,
     121,   115,   116,   117,   118,   119,   120,     0,  2227,   121,
     115,   116,   117,   118,   119,   120,     0,  2228,   121,   115,
     116,   117,   118,   119,   120,     0,  2230,   121,   115,   116,
     117,   118,   119,   120,     0,  2231,   121,   115,   116,   117,
     118,   119,   120,     0,  2232,   121,   115,   116,   117,   118,
     119,   120,     0,  2233,   121,   115,   116,   117,   118,   119,
     120,     0,  2235,   121,   115,   116,   117,   118,   119,   120,
       0,  2236,   121,   115,   116,   117,   118,   119,   120,     0,
    2238,   121,   115,   116,   117,   118,   119,   120,     0,  2239,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,  2240,     0,   115,   116,   117,   118,
     119,   120,     0,  2241,   121,   115,   116,   117,   118,   119,
     120,     0,  2264,   121,   115,   116,   117,   118,   119,   120,
       0,  2276,   121,   115,   116,   117,   118,   119,   120,     0,
    2277,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,     0,  2278,     0,   115,   116,   117,
     118,   119,   120,     0,  2279,   121,   115,   116,   117,   118,
     119,   120,     0,  2281,   121,   115,   116,   117,   118,   119,
     120,     0,  2282,   121,     0,   115,   116,   117,   118,   119,
     120,     0,  2283,   121,   115,   116,   117,   118,   119,   120,
       0,  2284,   121,   115,   116,   117,   118,   119,   120,     0,
    2286,   121,   115,   116,   117,   118,   119,   120,     0,  2287,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,  2289,     0,   115,   116,   117,   118,   119,
     120,     0,  2290,   121,   115,   116,   117,   118,   119,   120,
       0,  2291,   121,   115,   116,   117,   118,   119,   120,     0,
    2292,   121,   115,   116,   117,   118,   119,   120,     0,  2312,
     121,   115,   116,   117,   118,   119,   120,     0,  2324,   121,
       0,   115,   116,   117,   118,   119,   120,     0,  2325,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,  2326,     0,   115,   116,   117,   118,   119,   120,
       0,  2327,   121,   115,   116,   117,   118,   119,   120,     0,
    2329,   121,   115,   116,   117,   118,   119,   120,     0,  2330,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,  2331,     0,   115,   116,   117,   118,
     119,   120,     0,  2332,   121,   115,   116,   117,   118,   119,
     120,     0,  2334,   121,   115,   116,   117,   118,   119,   120,
       0,  2335,   121,   115,   116,   117,   118,   119,   120,     0,
    2340,   121,   115,   116,   117,   118,   119,   120,     0,  2364,
     121,   115,   116,   117,   118,   119,   120,     0,  2365,   121,
     115,   116,   117,   118,   119,   120,     0,  2366,   121,   115,
     116,   117,   118,   119,   120,     0,  2367,   121,   115,   116,
     117,   118,   119,   120,     0,  2369,   121,   115,   116,   117,
     118,   119,   120,     0,  2370,   121,   115,   116,   117,   118,
     119,   120,     0,  2371,   121,   115,   116,   117,   118,   119,
     120,     0,  2372,   121,   115,   116,   117,   118,   119,   120,
       0,  2374,   121,   115,   116,   117,   118,   119,   120,     0,
    2375,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,     0,  2380,     0,   115,   116,   117,
     118,   119,   120,     0,  2404,   121,   115,   116,   117,   118,
     119,   120,     0,  2405,   121,   115,   116,   117,   118,   119,
     120,     0,  2406,   121,   115,   116,   117,   118,   119,   120,
       0,  2407,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,  2409,     0,   115,   116,
     117,   118,   119,   120,     0,  2410,   121,   115,   116,   117,
     118,   119,   120,     0,  2411,   121,     0,   115,   116,   117,
     118,   119,   120,     0,  2412,   121,   115,   116,   117,   118,
     119,   120,     0,  2414,   121,   115,   116,   117,   118,   119,
     120,     0,  2415,   121,   115,   116,   117,   118,   119,   120,
       0,  2420,   121,   115,   116,   117,   118,   119,   120,     0,
    2437,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,  2438,     0,   115,   116,   117,   118,
     119,   120,     0,  2439,   121,   115,   116,   117,   118,   119,
     120,     0,  2444,   121,   115,   116,   117,   118,   119,   120,
       0,  2446,   121,   115,   116,   117,   118,   119,   120,     0,
    2447,   121,     0,   115,   116,   117,   118,   119,   120,     0,
    2450,   121,   115,   116,   117,   118,   119,   120,     0,  2454,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,  2455,     0,   115,   116,   117,   118,   119,
     120,     0,  2456,   121,   115,   116,   117,   118,   119,   120,
       0,  2460,   121,   115,   116,   117,   118,   119,   120,     0,
    2475,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,     0,  2476,     0,   115,   116,   117,
     118,   119,   120,     0,  2477,   121,   115,   116,   117,   118,
     119,   120,     0,  2481,   121,   115,   116,   117,   118,   119,
     120,     0,  2482,   121,   115,   116,   117,   118,   119,   120,
       0,  2483,   121,   115,   116,   117,   118,   119,   120,     0,
    2486,   121,   115,   116,   117,   118,   119,   120,     0,  2490,
     121,   115,   116,   117,   118,   119,   120,     0,  2491,   121,
     115,   116,   117,   118,   119,   120,     0,  2492,   121,   115,
     116,   117,   118,   119,   120,     0,  2496,   121,   115,   116,
     117,   118,   119,   120,     0,  2518,   121,   115,   116,   117,
     118,   119,   120,     0,  2519,   121,   115,   116,   117,   118,
     119,   120,     0,  2522,   121,   115,   116,   117,   118,   119,
     120,     0,  2526,   121,   115,   116,   117,   118,   119,   120,
       0,  2527,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,  2532,     0,   115,   116,
     117,   118,   119,   120,     0,  2549,   121,   115,   116,   117,
     118,   119,   120,     0,  2550,   121,   115,   116,   117,   118,
     119,   120,     0,  2555,   121,   115,   116,   117,   118,   119,
     120,     0,  2556,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,     0,  2560,     0,   115,
     116,   117,   118,   119,   120,     0,  2577,   121,     0,   115,
     116,   117,   118,   119,   120,     0,  2578,   121,   115,   116,
     117,   118,   119,   120,     0,  2581,   121,   115,   116,   117,
     118,   119,   120,     0,  2582,   121,   115,   116,   117,   118,
     119,   120,     0,  2583,   121,   115,   116,   117,   118,   119,
     120,     0,  2584,   121,   115,   116,   117,   118,   119,   120,
       0,  2585,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,  2586,    73,    74,     0,    75,
      76,     0,    77,    78,  2601,    79,    80,    81,     0,     0,
       0,     0,     0,  2602,     0,     0,    82,     0,     0,     0,
       0,     0,  2605,   115,   116,   117,   118,   119,   120,     0,
       0,   121,  2606,     0,   115,   116,   117,   118,   119,   120,
       0,  2607,   121,   115,   116,   117,   118,   119,   120,     0,
    2608,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,  2609,     0,   115,   116,   117,   118,
     119,   120,     0,  2610,   121,   115,   116,   117,   118,   119,
     120,     0,  2624,   121,   115,   116,   117,   118,   119,   120,
       0,  2625,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,  2627,     0,   115,   116,
     117,   118,   119,   120,     0,  2628,   121,     0,   115,   116,
     117,   118,   119,   120,  2632,    83,   121,   115,   116,   117,
     118,   119,   120,  2643,     0,   121,   115,   116,   117,   118,
     119,   120,  2646,     0,   121,   115,   116,   117,   118,   119,
     120,  2659,     0,   121,   115,   116,   117,   118,   119,   120,
    2662,     0,   121,     0,     0,     0,  1337,     0,     0,  2675,
       0,     0,     0,     0,  1131,     0,     0,     0,  2678,     0,
       0,     0,  1695,     0,   115,   116,   117,   118,   119,   120,
       0,  2094,   121,   115,   116,   117,   118,   119,   120,  1213,
       0,   121,   115,   116,   117,   118,   119,   120,  1526,     0,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,  1974,   115,   116,   117,   118,   119,   120,
       0,   998,   121,   115,   116,   117,   118,   119,   120,     0,
     999,   121,   115,   116,   117,   118,   119,   120,     0,  1336,
     121,   115,   116,   117,   118,   119,   120,     0,  1841,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,   997,   115,   116,   117,   118,   119,   120,     0,
       0,   121,  1212,     0,   115,   116,   117,   118,   119,   120,
       0,  1413,   121,   115,   116,   117,   118,   119,   120,   754,
       0,   121,   115,   116,   117,   118,   119,   120,   755,     0,
     121,   115,   116,   117,   118,   119,   120,   756,     0,   121,
     115,   116,   117,   118,   119,   120,  1130,     0,   121,   115,
     116,   117,   118,   119,   120,  1696,    84,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,  1119,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,  1120,     0,   115,
     116,   117,   118,   119,   120,     0,  1335,   121,   115,   116,
     117,   118,   119,   120,  1325,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,  1326,   115,
     116,   117,   118,   119,   120,   513,     0,   121,     0,     0,
       0,     0,   426,   115,   116,   117,   118,   119,   120,     0,
     679,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,   760,     0,   115,   116,   117,   118,
     119,   120,     0,   509,   121,   115,   116,   117,   118,   119,
     120,   241,     0,   121,     0,     0,     0,     0,     0,  2275,
     115,   116,   117,   118,   119,   120,     0,   657,   121,   115,
     116,   117,   118,   119,   120,  2197,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,   115,   116,   117,
     118,   119,   120,     0,  2097,   121,   115,   116,   117,   118,
     119,   120,  2095,     0,   121,   115,   116,   117,   118,   119,
     120,  2096,     0,   121,     0,     0,     0,     0,     0,  1697,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,  1699,     0,   115,   116,   117,   118,   119,   120,
       0,  1845,   121,   115,   116,   117,   118,   119,   120,  1528,
       0,   121,   115,   116,   117,   118,   119,   120,  1530,     0,
     121,   115,   116,   117,   118,   119,   120,  1700,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,  1338,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    1340,     0,   115,   116,   117,   118,   119,   120,     0,  1531,
     121,   115,   116,   117,   118,   119,   120,  1132,     0,   121,
     115,   116,   117,   118,   119,   120,  1134,     0,   121,   115,
     116,   117,   118,   119,   120,  1341,     0,   121,     0,     0,
       0,     0,     0,   903,     0,   115,   116,   117,   118,   119,
     120,     0,   905,   121,   115,   116,   117,   118,   119,   120,
       0,  1135,   121,   115,   116,   117,   118,   119,   120,     0,
    1694,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,  1525,   115,   116,   117,   118,   119,   120,     0,  1129,
     121,   115,   116,   117,   118,   119,   120,   900,     0,   121,
       0,   115,   116,   117,   118,   119,   120,     0,   650,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
     901,     0,   115,   116,   117,   118,   119,   120,     0,   902,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
    1685,     0,   115,   116,   117,   118,   119,   120,     0,  1686,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,   890,     0,   115,   116,   117,   118,   119,
     120,     0,   891,   121,   115,   116,   117,   118,   119,   120,
     640,     0,   121,   115,   116,   117,   118,   119,   120,   641,
       0,   121,   115,   116,   117,   118,   119,   120,  2634,     0,
     121,     0,     0,     0,     0,     0,  2600,   115,   116,   117,
     118,   119,   120,     0,  2562,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,  2516,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,  2462,   115,   116,   117,   118,   119,   120,     0,  2403,
     121,   115,   116,   117,   118,   119,   120,  2343,     0,   121,
       0,     0,     0,     0,     0,  2273,   115,   116,   117,   118,
     119,   120,     0,  2194,   121,   115,   116,   117,   118,   119,
     120,     0,  2089,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  1968,   115,   116,   117,   118,   119,   120,
       0,  1834,   121,   115,   116,   117,   118,   119,   120,  1687,
       0,   121,     0,     0,     0,     0,     0,  1517,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,     0,   892,   115,   116,   117,   118,   119,   120,     0,
     639,   121,   115,   116,   117,   118,   119,   120,     0,   642,
     121,   115,   116,   117,   118,   119,   120,  1327,     0,   121,
       0,     0,     0,     0,     0,   889,   115,   116,   117,   118,
     119,   120,     0,  2729,   121,   115,   116,   117,   118,   119,
     120,     0,  2720,   121,   115,   116,   117,   118,   119,   120,
    2711,     0,   121,     0,   115,   116,   117,   118,   119,   120,
       0,  2696,   121,   115,   116,   117,   118,   119,   120,  2681,
       0,   121,   115,   116,   117,   118,   119,   120,     0,  2657,
     121,   115,   116,   117,   118,   119,   120,  2633,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    2599,   115,   116,   117,   118,   119,   120,     0,  2561,   121,
     115,   116,   117,   118,   119,   120,  2515,     0,   121,     0,
       0,     0,     0,     0,  2461,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,     0,  2401,   115,
     116,   117,   118,   119,   120,     0,  2341,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,  2270,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
    2191,   115,   116,   117,   118,   119,   120,     0,  2086,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,  1965,   115,   116,   117,   118,   119,   120,     0,
    1831,   121,   115,   116,   117,   118,   119,   120,  1684,     0,
     121,   115,   116,   117,   118,   119,   120,  1118,     0,   121,
       0,     0,     0,     0,     0,  1514,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,  1324,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    1832,     0,   115,   116,   117,   118,   119,   120,     0,  1833,
     121,   115,   116,   117,   118,   119,   120,   899,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,   649,   115,   116,   117,   118,   119,   120,     0,  1524,
     121,   115,   116,   117,   118,   119,   120,  1334,     0,   121,
     115,   116,   117,   118,   119,   120,  1128,     0,   121,     0,
       0,     0,     0,     0,  1318,     0,   115,   116,   117,   118,
     119,   120,     0,  1319,   121,   115,   116,   117,   118,   119,
     120,     0,  1342,   121,   115,   116,   117,   118,   119,   120,
       0,  1343,   121,     0,   115,   116,   117,   118,   119,   120,
       0,  1317,   121,   115,   116,   117,   118,   119,   120,     0,
    1322,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,     0,  1328,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,  1329,
       0,   115,   116,   117,   118,   119,   120,     0,  1532,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,  1533,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    1316,     0,   115,   116,   117,   118,   119,   120,     0,  1321,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,  1332,     0,   115,   116,   117,   118,   119,   120,
       0,  1333,   121,   115,   116,   117,   118,   119,   120,     0,
    1339,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,     0,  1507,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,  1508,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,  1509,     0,   115,   116,   117,   118,   119,   120,     0,
    1512,   121,   115,   116,   117,   118,   119,   120,     0,  1518,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,  1519,     0,   115,   116,   117,   118,   119,   120,     0,
    1680,   121,   115,   116,   117,   118,   119,   120,     0,  1683,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,     0,  1690,     0,   115,   116,   117,
     118,   119,   120,     0,  1691,   121,   115,   116,   117,   118,
     119,   120,     0,  1966,   121,     0,   115,   116,   117,   118,
     119,   120,     0,  1967,   121,   115,   116,   117,   118,   119,
     120,     0,  1975,   121,   115,   116,   117,   118,   119,   120,
       0,  1976,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,  1977,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,     0,
    2658,   115,   116,   117,   118,   119,   120,     0,  1510,   121,
       0,   115,   116,   117,   118,   119,   120,     0,  1513,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    1520,     0,   115,   116,   117,   118,   119,   120,     0,  1521,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,  1320,     0,   115,   116,   117,   118,   119,   120,     0,
    1323,   121,   115,   116,   117,   118,   119,   120,     0,  1330,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,  1331,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,  1137,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,  1122,     0,
     115,   116,   117,   118,   119,   120,     0,  1123,   121,   115,
     116,   117,   118,   119,   120,     0,  1124,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,  1125,     0,
     115,   116,   117,   118,   119,   120,     0,  1126,   121,   115,
     116,   117,   118,   119,   120,     0,  1127,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
       0,  1110,     0,   115,   116,   117,   118,   119,   120,     0,
    1111,   121,     0,   115,   116,   117,   118,   119,   120,     0,
    1112,   121,   115,   116,   117,   118,   119,   120,     0,  1113,
     121,   115,   116,   117,   118,   119,   120,     0,  1114,   121,
     115,   116,   117,   118,   119,   120,     0,  1115,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
    1116,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,     0,  1117,   115,   116,   117,   118,
     119,   120,     0,     0,   121,  1133,     0,   115,   116,   117,
     118,   119,   120,     0,  1136,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   654,     0,   115,   116,   117,
     118,   119,   120,     0,   656,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,   675,     0,   115,   116,
     117,   118,   119,   120,     0,   880,   121,   115,   116,   117,
     118,   119,   120,     0,   881,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,   882,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,     0,     0,
     883,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,   884,     0,   115,   116,   117,   118,   119,
     120,     0,   885,   121,   115,   116,   117,   118,   119,   120,
       0,   886,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,   887,     0,   115,   116,   117,   118,   119,
     120,     0,   888,   121,   115,   116,   117,   118,   119,   120,
       0,   893,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,   894,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   895,     0,   115,   116,
     117,   118,   119,   120,     0,   896,   121,   115,   116,   117,
     118,   119,   120,     0,   897,   121,   115,   116,   117,   118,
     119,   120,     0,   898,   121,   115,   116,   117,   118,   119,
     120,     0,   904,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,   906,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,     0,     0,   907,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
     908,     0,   115,   116,   117,   118,   119,   120,     0,   909,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
    1121,     0,   115,   116,   117,   118,   119,   120,     0,  1515,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,  1516,   115,   116,   117,   118,   119,   120,     0,   651,
     121,   115,   116,   117,   118,   119,   120,   630,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   631,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
       0,     0,   632,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,   633,     0,   115,   116,
     117,   118,   119,   120,     0,   634,   121,   115,   116,   117,
     118,   119,   120,     0,   635,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,   636,     0,   115,   116,
     117,   118,   119,   120,     0,   637,   121,   115,   116,   117,
     118,   119,   120,     0,   638,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,   643,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,   644,
       0,   115,   116,   117,   118,   119,   120,     0,   645,   121,
     115,   116,   117,   118,   119,   120,     0,   646,   121,   115,
     116,   117,   118,   119,   120,     0,   647,   121,   115,   116,
     117,   118,   119,   120,     0,   648,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   652,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,     0,     0,
       0,   653,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,   655,     0,   115,   116,   117,   118,   119,
     120,     0,   658,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,   659,     0,   115,   116,   117,   118,   119,
     120,     0,   660,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  1168,     0,   115,   116,   117,   118,   119,
     120,     0,  1169,   121,   115,   116,   117,   118,   119,   120,
       0,  1170,   121,   115,   116,   117,   118,   119,   120,     0,
    1146,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,  1433,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,     0,     0,  1441,     0,
     115,   116,   117,   118,   119,   120,     0,  1842,   121,   115,
     116,   117,   118,   119,   120,     0,  1844,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,   923,  1995,     0,
     115,   116,   117,   118,   119,   120,     0,  1996,   121,  1375,
       0,     0,     0,     0,     0,     0,  1997,     0,  1498,     0,
     115,   116,   117,   118,   119,   120,     0,  1499,   121,   115,
     116,   117,   118,   119,   120,     0,  1501,   121,     0,     0,
       0,     0,  1867,     0,     0,     0,     0,     0,  1767,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,     0,  2113,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,  2114,     0,   115,   116,
     117,   118,   119,   120,     0,  2115,   121,   115,   116,   117,
     118,   119,   120,     0,  1722,   121,   115,   116,   117,   118,
     119,   120,  1562,     0,   121,   115,   116,   117,   118,   119,
     120,     0,  1378,   121,   115,   116,   117,   118,   119,   120,
    1176,     0,   121,     0,     0,  1612,   115,   116,   117,   118,
     119,   120,     0,  1719,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,  1720,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   115,   116,   117,   118,   119,
     120,  1721,     0,   121,     0,     0,     0,     0,     0,  1864,
       0,   115,   116,   117,   118,   119,   120,     0,  1865,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,     0,
    1866,   115,   116,   117,   118,   119,   120,     0,     0,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,  1555,
       0,   115,   116,   117,   118,   119,   120,     0,  1556,   121,
     115,   116,   117,   118,   119,   120,     0,     0,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,  1557,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,  1370,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,     0,  1371,     0,   115,   116,
     117,   118,   119,   120,     0,  1372,   121,   115,   116,   117,
     118,   119,   120,  1243,     0,   121,     0,     0,     0,     0,
    2537,     0,   115,   116,   117,   118,   119,   120,     0,  2540,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,  2725,   115,   116,   117,   118,   119,   120,     0,  2484,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
    2489,     0,   115,   116,   117,   118,   119,   120,     0,  2716,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,  2428,     0,   115,   116,   117,   118,
     119,   120,     0,  2433,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,  2704,   115,   116,   117,   118,   119,
     120,     0,  2368,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  2373,     0,   115,   116,   117,   118,   119,
     120,     0,  2689,   121,   115,   116,   117,   118,   119,   120,
    2304,     0,   121,   115,   116,   117,   118,   119,   120,     0,
    2309,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,     0,     0,     0,  2669,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,     0,     0,  2229,     0,
     115,   116,   117,   118,   119,   120,     0,  2234,   121,   115,
     116,   117,   118,   119,   120,     0,     0,   121,     0,     0,
       0,     0,  2645,   115,   116,   117,   118,   119,   120,     0,
    2139,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,  2144,     0,   115,   116,   117,   118,   119,   120,     0,
    2615,   121,     0,   115,   116,   117,   118,   119,   120,     0,
    2027,   121,   115,   116,   117,   118,   119,   120,     0,  2032,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,     0,  2579,   115,   116,   117,   118,   119,
     120,   523,  1900,   121,     0,     0,     0,     0,     0,     0,
     525,     0,  1905,     0,     0,     0,     0,     0,     0,   529,
       0,  2538,   115,   116,   117,   118,   119,   120,   533,     0,
     121,     0,     0,     0,     0,  1828,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,     0,     0,     0,     0,
    1840,     0,   115,   116,   117,   118,   119,   120,     0,  1843,
     121,   115,   116,   117,   118,   119,   120,     0,     0,   121,
       0,     0,     0,  1961,     0,   115,   116,   117,   118,   119,
     120,     0,  1963,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  1970,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  2081,     0,   115,   116,   117,   118,   119,
     120,     0,  2085,   121,   115,   116,   117,   118,   119,   120,
       0,  2092,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,     0,     0,  2093,   115,   116,   117,   118,
     119,   120,     0,     0,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,   115,   116,   117,   118,   119,
     120,     0,  2188,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,     0,  2189,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   115,   116,   117,   118,
     119,   120,  2195,     0,   121,   115,   116,   117,   118,   119,
     120,  2269,     0,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,     0,  2272,     0,   115,   116,   117,
     118,   119,   120,     0,  2274,   121,     0,   115,   116,   117,
     118,   119,   120,     0,  2402,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,  2730,     0,   115,   116,   117,
     118,   119,   120,     0,  2748,   121,   115,   116,   117,   118,
     119,   120,  1681,     0,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,  1693,     0,   115,   116,   117,
     118,   119,   120,     0,  1698,   121,     0,   115,   116,   117,
     118,   119,   120,     0,  1826,   121,   115,   116,   117,   118,
     119,   120,     0,  1829,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,  1836,     0,   115,   116,
     117,   118,   119,   120,     0,  1958,   121,   115,   116,   117,
     118,   119,   120,     0,  1964,   121,   115,   116,   117,   118,
     119,   120,     0,  1972,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,     0,  1973,   115,   116,   117,
     118,   119,   120,     0,     0,   121,  2082,     0,   115,   116,
     117,   118,   119,   120,     0,  2083,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,  2090,     0,   115,   116,
     117,   118,   119,   120,     0,  2190,   121,   115,   116,   117,
     118,   119,   120,     0,  2193,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,  2196,   115,   116,   117,
     118,   119,   120,     0,     0,   121,  2342,     0,   115,   116,
     117,   118,   119,   120,     0,  2721,   121,   115,   116,   117,
     118,   119,   120,     0,  2745,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,  1511,     0,   115,   116,
     117,   118,   119,   120,     0,  1523,   121,   115,   116,   117,
     118,   119,   120,     0,  1529,   121,   115,   116,   117,   118,
     119,   120,     0,  1679,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,  1682,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,  1689,     0,   115,   116,
     117,   118,   119,   120,     0,  1823,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,  1830,     0,   115,   116,
     117,   118,   119,   120,     0,  1838,   121,   115,   116,   117,
     118,   119,   120,     0,  1839,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,  1959,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,  1960,     0,   115,   116,
     117,   118,   119,   120,     0,  1969,   121,   115,   116,   117,
     118,   119,   120,     0,  2084,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,     0,  2088,     0,   115,   116,
     117,   118,   119,   120,     0,  2091,   121,   115,   116,   117,
     118,   119,   120,     0,  2271,   121,   115,   116,   117,   118,
     119,   120,     0,  2712,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,  2742,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,  1676,     0,   115,   116,   117,
     118,   119,   120,     0,  1692,   121,   115,   116,   117,   118,
     119,   120,     0,     0,   121,  1824,     0,   115,   116,   117,
     118,   119,   120,     0,  1825,   121,   115,   116,   117,   118,
     119,   120,     0,  1835,   121,     0,   115,   116,   117,   118,
     119,   120,     0,  1962,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,     0,  1971,     0,   115,   116,   117,
     118,   119,   120,     0,  2192,   121,   115,   116,   117,   118,
     119,   120,     0,  2697,   121,   115,   116,   117,   118,   119,
     120,     0,     0,   121,  1285,  2739,   115,   116,   117,   118,
     119,   120,     0,  1286,   121,   115,   116,   117,   118,   119,
     120,     0,  1287,   121,     0,   115,   116,   117,   118,   119,
     120,     0,  1355,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,     0,  1415,   115,   116,   117,   118,   119,
     120,     0,  1079,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,  1080,     0,   115,   116,   117,   118,   119,
     120,     0,  1081,   121,     0,   115,   116,   117,   118,   119,
     120,     0,  1152,   121,   115,   116,   117,   118,   119,   120,
       0,  1216,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,   848,     0,   115,   116,   117,   118,   119,   120,
       0,   849,   121,   115,   116,   117,   118,   119,   120,     0,
     850,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,   931,     0,   115,   116,   117,   118,   119,   120,     0,
    1004,   121,     0,     0,     0,     0,   991,     0,   115,   116,
     117,   118,   119,   120,     0,  1208,   121,     0,   115,   116,
     117,   118,   119,   120,     0,   326,   121,     0,     0,     0,
       0,     0,  1412,   115,   116,   117,   118,   119,   120,     0,
       0,   121,     0,  1596,     0,   115,   116,   117,   118,   119,
     120,     0,  1753,   121,   115,   116,   117,   118,   119,   120,
       0,  1896,   121,   115,   116,   117,   118,   119,   120,   996,
       0,   121,     0,     0,   115,   116,   117,   118,   119,   120,
    1210,  1597,   121,   115,   116,   117,   118,   119,   120,     0,
    1752,   121,     0,   115,   116,   117,   118,   119,   120,     0,
    1211,   121,     0,   115,   116,   117,   118,   119,   120,     0,
     236,   121,     0,     0,     0,  1411,     0,     0,   115,   116,
     117,   118,   119,   120,   995,   237,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,   232,     0,   115,
     116,   117,   118,   119,   120,     0,   233,   121,   115,   116,
     117,   118,   119,   120,     0,   240,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,   581,     0,   115,   116,
     117,   118,   119,   120,     0,   582,   121,   115,   116,   117,
     118,   119,   120,     0,   172,   121,     0,   115,   116,   117,
     118,   119,   120,     0,   227,   121,     0,     0,     0,     0,
       0,   514,     0,   115,   116,   117,   118,   119,   120,     0,
     583,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,  1097,     0,     0,     0,   234,     0,   115,   116,   117,
     118,   119,   120,     0,   235,   121,     0,   115,   116,   117,
     118,   119,   120,     0,   674,   121,   115,   116,   117,   118,
     119,   120,     0,  1179,   121,     0,     0,     0,     0,     0,
     246,   115,   116,   117,   118,   119,   120,     0,     0,   121,
     115,   116,   117,   118,   119,   120,   247,     0,   121,   115,
     116,   117,   118,   119,   120,  1010,     0,   121,   115,   116,
     117,   118,   119,   120,     0,     0,   121,     0,   587,   115,
     116,   117,   118,   119,   120,     0,   231,   121,   115,   116,
     117,   118,   119,   120,  1153,     0,   121,   115,   116,   117,
     118,   119,   120,     0,     0,   121,     0,     0,   687,     0,
       0,     0,     0,  1506,     0,     0,   932,     0,     0,     0,
       0,     0,   990,     0,  1522,     0,   115,   116,   117,   118,
     119,   120,     0,  1677,   121,   115,   116,   117,   118,   119,
     120,     0,  1678,   121,   115,   116,   117,   118,   119,   120,
       0,     0,   121,   115,   116,   117,   118,   119,   120,     0,
       0,   121,   115,   116,   117,   118,   119,   120,     0,     0,
     121,  1688,     0,   115,   116,   117,   118,   119,   120,     0,
    1827,   121,   115,   116,   117,   118,   119,   120,     0,  1837,
     121,     0,     0,     0,     0,     0,     0,     0,  2087,     0,
       0,     0,     0,     0,     0,     0,     0,  2682,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2736,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,     0,     0,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   285
  };

  const short int
  feiparser::yycheck_[] =
  {
       7,     8,    55,    55,   247,   109,     0,    62,   382,   383,
     384,    94,    19,    20,    21,   412,    34,    35,    36,    37,
      38,    39,    97,   106,    42,    94,   164,    34,    94,   167,
     168,    54,    55,    40,     3,     4,     5,   106,     7,     8,
     106,    39,    86,    87,    42,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    48,    54,    55,    26,    27,    28,
      29,    30,    31,    32,    71,   140,    35,    38,    39,   240,
     241,    42,    41,    34,    35,    36,    37,    38,    39,    86,
      92,    42,   104,    95,   412,    54,    55,    99,    57,     3,
     112,    50,   335,    34,    35,    36,    37,    38,    39,   106,
      52,    42,   131,   132,   133,   415,   135,   136,   115,   116,
     117,   118,   119,   120,   121,    34,    35,    36,    37,    38,
      39,   166,   406,    42,   169,   175,   176,   136,   137,   136,
     137,   138,   157,   158,   141,   142,   191,   144,   145,   146,
     147,   157,   158,   150,   151,    54,   157,   158,    54,   156,
     157,   158,   159,   160,   161,    70,   123,     0,   121,   418,
       3,     4,     5,     3,     7,     8,     9,    42,     5,    12,
     109,   109,    15,   109,    17,   109,    19,   109,   109,   109,
     109,   401,   109,    26,    27,    28,    29,    30,    31,    32,
      54,    55,    35,   109,    58,   202,    60,   123,    41,    63,
      43,    44,    45,    46,   182,   411,    49,   111,    51,   216,
      53,    54,    34,    35,    36,    37,    38,    39,   109,    58,
      42,    36,    37,    38,    39,   109,    69,    42,   109,   109,
     109,   109,    75,    76,    77,    78,   243,   401,   245,   128,
      79,   248,    81,   250,    83,    80,    85,    90,    91,    88,
       1,    82,     3,     4,     5,   262,     7,     8,     9,    84,
      89,    12,    96,   163,    15,   108,    17,    71,    19,   124,
     406,   406,    11,   106,    63,    26,    27,    28,    29,    30,
      31,    32,   109,    54,    35,    54,   122,   129,   106,   363,
      41,    94,    43,    44,    45,    46,   106,    79,    49,    94,
      51,     5,    53,    54,    34,    35,    36,    37,    38,    39,
      42,   415,    42,   420,   419,   402,   109,   111,    69,   417,
     111,   109,   329,    33,    75,    76,    77,    78,   365,   212,
      54,    98,   412,   340,   341,   342,   343,   165,    54,    90,
      91,    54,   166,    33,     3,    54,   353,   112,   355,    33,
      33,   109,   359,   406,   406,    33,   363,   108,   220,     3,
       4,     5,   175,     7,     8,    34,    35,    36,    37,    38,
      39,   170,    79,    42,    79,   106,    79,   106,   106,   106,
      94,   106,    26,    27,    28,    29,    30,    31,    32,   106,
     275,    35,   274,   342,     3,     4,     5,    41,     7,     8,
     369,   419,   274,   410,   411,   412,   413,   414,   106,   106,
      54,   418,   419,   106,   106,   106,   423,    26,    27,    28,
      29,    30,    31,    32,   106,   106,    35,   106,   274,   106,
     106,   438,    41,    34,    35,    36,    37,    38,    39,   106,
     106,    42,   106,   412,   274,    54,   340,   340,   274,   109,
      94,   274,   109,   274,   274,   274,   397,   418,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   274,
     274,   274,   109,   274,   274,   104,   274,   416,   274,   274,
     507,   508,   274,   109,   511,   512,   274,    34,    35,    36,
      37,    38,    39,   520,   521,    42,   275,   524,   162,   526,
     527,   528,   274,   530,   531,   532,   274,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   275,   109,    34,    35,    36,    37,    38,    39,
     106,   274,    42,    34,    35,    36,    37,    38,    39,   274,
     106,    42,   274,   406,   106,   109,   409,   410,   274,   412,
      33,   242,   109,   275,   166,   418,   175,   584,   585,   586,
       3,     4,     5,   590,     7,     8,   177,   594,   177,   177,
      97,   413,   414,    97,   114,   114,   603,   114,   114,   114,
     114,   114,   114,    26,    27,    28,    29,    30,    31,    32,
     175,   114,    35,   114,   114,   114,   114,   114,    41,    34,
      35,    36,    37,    38,    39,   114,   114,    42,   114,   114,
     114,    54,   114,    33,   114,   114,   385,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    34,    35,    36,    37,
      38,    39,    33,    33,    42,   406,    33,    33,   409,   410,
      33,   412,    33,    33,    33,    33,    18,   418,    20,    21,
      22,    23,    24,    25,   681,    33,   683,   684,   685,    33,
      33,    33,    33,   690,   414,   692,   693,   694,    33,   696,
     697,   698,    33,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,    33,
      33,    33,    33,    33,   413,   414,    34,    35,    36,    37,
      38,    39,   759,    33,    42,   762,   763,  1000,    33,    33,
      33,    33,   769,   770,    33,    33,    33,   385,   412,    34,
      35,    36,    37,    38,    39,   385,    33,    42,    33,   110,
      34,    35,    36,    37,    38,    39,    33,    33,    42,    34,
      35,    36,    37,    38,    39,   110,   110,    42,   177,    33,
      33,   412,   126,   412,   412,   412,    34,    35,    36,    37,
      38,    39,   412,   414,    42,   412,   412,   412,   412,   412,
      33,    33,   829,   830,   831,   412,   412,   412,   835,   836,
     837,   838,   839,   840,   841,   412,   843,   412,   412,   412,
     847,   412,   412,   412,   412,   412,   853,    33,   855,   412,
     857,   858,   859,    33,   861,   862,   863,    33,   865,   866,
     112,   177,    57,    57,   871,   872,   873,   874,    57,   110,
     877,   878,    34,    35,    36,    37,    38,    39,   179,   179,
      42,    34,    35,    36,    37,    38,    39,   414,   112,    42,
      34,    35,    36,    37,    38,    39,   179,   179,    42,   135,
     129,   403,   112,   110,   109,   109,   913,   914,   915,    34,
      35,    36,    37,    38,    39,    57,   109,    42,   925,   181,
     181,   928,   929,   930,   414,   181,    33,    33,   181,    33,
      33,    33,    33,   414,    34,    35,    36,    37,    38,    39,
     947,    33,    42,    34,    35,    36,    37,    38,    39,    33,
      33,    42,    33,   960,   961,   962,   963,   964,   965,   966,
     967,   968,   969,   970,   971,   972,   973,   974,   975,   976,
     977,   978,   979,   980,   981,   982,   983,   984,   985,   986,
     987,   988,   989,    34,    35,    36,    37,    38,    39,   412,
      33,    42,    33,    33,  1001,    33,  1003,    33,    33,   414,
      33,  1008,  1009,    33,  1011,  1012,  1013,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,   414,  1025,  1026,
    1027,  1028,  1029,    33,  1031,   413,  1033,  1034,  1035,  1036,
      34,    35,    36,    37,    38,    39,    33,    33,    42,    33,
      33,    33,    33,    33,    33,   115,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,   115,   115,   103,
    1067,   110,  1069,  1070,  1071,   110,  1073,  1074,    54,    33,
      57,  1078,    34,    35,    36,    37,    38,    39,    63,    33,
      42,    34,    35,    36,    37,    38,    39,    33,    57,    42,
     109,    34,    35,    36,    37,    38,    39,    33,  1105,    42,
      34,    35,    36,    37,    38,    39,   238,   238,    42,    34,
      35,    36,    37,    38,    39,   238,   238,    42,    74,    34,
      35,    36,    37,    38,    39,    33,   414,    42,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    74,   114,   404,
      33,    33,  1149,  1150,  1151,    33,    33,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,
     414,    33,   109,    33,   109,   109,   184,   106,   106,   414,
    1177,  1178,   400,  1180,  1181,  1182,  1183,  1184,  1185,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,
    1207,   106,  1209,   106,   106,   109,   412,  1214,  1215,   109,
      33,    33,    33,  1220,  1221,    34,    35,    36,    37,    38,
      39,   405,   106,    42,   114,   114,   221,   110,   114,  1236,
    1237,  1238,   114,    33,  1241,    33,    33,  1244,    34,    35,
      36,    37,    38,    39,    33,   229,    42,   221,   221,   221,
      33,    33,   414,   412,    34,    35,    36,    37,    38,    39,
      33,   414,    42,    33,    33,    33,    33,    33,    33,    33,
     414,  1278,  1279,    33,  1281,    33,    33,  1284,    33,   404,
      33,    33,    33,  1290,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1300,  1301,  1302,  1303,    34,    35,    36,
      37,    38,    39,    33,  1311,    42,  1313,  1314,  1315,    33,
      33,    33,    33,    33,   414,   412,    34,    35,    36,    37,
      38,    39,    33,   414,    42,    34,    35,    36,    37,    38,
      39,    33,    33,    42,    33,    33,    33,    33,  1345,   412,
     412,    33,    33,    33,    57,  1352,  1353,  1354,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    34,    35,    36,
      37,    38,    39,   414,    33,    42,  1373,  1374,    33,  1376,
      33,    33,    33,    33,    33,  1382,  1383,  1384,  1385,  1386,
    1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,  1396,
    1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,
    1407,  1408,  1409,    33,   110,   109,     5,  1414,    33,    33,
     414,   381,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,
    1427,  1428,  1429,  1430,  1431,  1432,   414,  1434,  1435,  1436,
      33,    57,  1439,    33,   110,  1442,  1443,  1444,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,    33,
      33,    33,   414,    65,    34,    35,    36,    37,    38,    39,
      33,   414,    42,    33,    33,    33,  1473,    33,  1475,  1476,
      33,   414,  1479,   110,    34,    35,    36,    37,    38,    39,
     414,    33,    42,    33,    33,    33,    33,  1494,    33,   414,
      33,    33,    33,    33,    33,  1502,    33,    33,    33,   414,
      33,    33,    33,    33,    33,    33,    33,   109,   414,    34,
      35,    36,    37,    38,    39,   110,   110,    42,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,    33,
      33,    57,    33,   109,  1541,  1542,  1543,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,  1552,  1553,    57,    33,     5,
      57,  1558,  1559,  1560,   109,    57,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,   381,  1595,    33,
      33,  1598,    33,    33,   109,   414,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,  1613,  1614,  1615,   109,
     109,    33,  1619,    33,    33,    33,    33,    33,   414,    33,
      33,    33,    33,   106,    34,    35,    36,    37,    38,    39,
      33,    33,    42,    33,   414,    34,    35,    36,    37,    38,
      39,    33,    33,    42,  1651,    33,  1653,  1654,    33,  1656,
      33,  1658,  1659,    33,  1661,  1662,  1663,  1664,    33,  1666,
      33,  1668,  1669,    33,    33,    33,    33,    33,  1675,   381,
      34,    35,    36,    37,    38,    39,    33,   414,    42,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    33,    33,
      33,    33,    33,    33,  1701,    33,   414,   412,    34,    35,
      36,    37,    38,    39,    33,   414,    42,    34,    35,    36,
      37,    38,    39,   399,    33,    42,   393,    33,  1725,  1726,
    1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,
    1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,  1745,  1746,
    1747,  1748,  1749,    33,  1751,    33,    33,  1754,    33,  1756,
    1757,    33,  1759,  1760,  1761,  1762,    33,  1764,    33,  1766,
      33,  1768,  1769,  1770,  1771,    33,  1773,    34,    35,    36,
      37,    38,    39,    33,    33,    42,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    34,    35,    36,    37,    38,
      39,    33,  1799,    42,  1801,  1802,    33,  1804,    33,    33,
    1807,    33,    33,    33,    33,  1812,    33,  1814,    34,    35,
      36,    37,    38,    39,  1821,    33,    42,    33,   414,    34,
      35,    36,    37,    38,    39,    33,    33,    42,    34,    35,
      36,    37,    38,    39,   414,    33,    42,    33,    33,    33,
    1847,    33,    33,  1850,    33,  1852,  1853,    33,  1855,  1856,
    1857,  1858,    33,  1860,   414,  1862,  1863,    33,    33,    33,
      33,    33,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,
    1877,  1878,  1879,  1880,  1881,  1882,  1883,  1884,  1885,  1886,
    1887,  1888,  1889,  1890,  1891,  1892,  1893,   110,   110,   414,
     110,    34,    35,    36,    37,    38,    39,   110,   414,    42,
     110,    33,  1909,  1910,  1911,    33,    33,    33,    33,    33,
      33,    33,    57,    34,    35,    36,    37,    38,    39,    33,
      33,    42,    34,    35,    36,    37,    38,    39,    33,    33,
      42,    33,  1939,  1940,    33,  1942,  1943,  1944,  1945,  1946,
    1947,  1948,  1949,  1950,  1951,  1952,    33,    33,    33,  1956,
    1957,    34,    35,    36,    37,    38,    39,    33,    33,    42,
      34,    35,    36,    37,    38,    39,    33,    33,    42,    33,
      33,    33,    33,  1980,    33,    57,   414,    57,    57,    57,
      33,   110,    34,    35,    36,    37,    38,    39,   398,    33,
      42,   109,   109,  2000,  2001,  2002,  2003,  2004,  2005,  2006,
    2007,  2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,  2016,
    2017,  2018,  2019,   109,  2021,   414,   109,    33,  2025,  2026,
     109,  2028,  2029,  2030,  2031,   412,  2033,  2034,  2035,  2036,
    2037,  2038,  2039,  2040,    33,    33,    33,   106,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,    33,
     414,    33,    33,    33,    33,  2062,    33,  2064,    33,   414,
    2067,    33,    33,    33,    33,  2072,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,    33,   414,   412,
      34,    35,    36,    37,    38,    39,    33,   414,    42,    33,
      33,  2098,    33,  2100,    33,  2102,  2103,    33,  2105,  2106,
    2107,  2108,    33,  2110,  2111,    33,    33,    33,    33,  2116,
    2117,  2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,
    2127,  2128,  2129,  2130,  2131,  2132,  2133,  2134,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    34,    35,    36,
      37,    38,    39,    33,    33,    42,   395,   414,   412,    34,
      35,    36,    37,    38,    39,    33,   414,    42,    33,    33,
      33,    33,    33,  2170,  2171,  2172,  2173,  2174,  2175,  2176,
    2177,  2178,  2179,  2180,  2181,    33,  2183,  2184,  2185,    33,
    2187,    34,    35,    36,    37,    38,    39,    33,   414,    42,
      33,    33,   106,    34,    35,    36,    37,    38,    39,   414,
      33,    42,    57,  2210,    33,    33,    33,   413,  2215,  2216,
    2217,  2218,  2219,  2220,  2221,  2222,  2223,  2224,  2225,  2226,
    2227,  2228,   109,  2230,  2231,  2232,  2233,    33,  2235,  2236,
      33,  2238,  2239,  2240,  2241,    34,    35,    36,    37,    38,
      39,   412,    33,    42,    33,    33,    33,    33,    33,  2256,
      33,    33,    33,   396,  2261,    33,    33,  2264,    34,    35,
      36,    37,    38,    39,    33,    33,    42,    33,    33,  2276,
    2277,  2278,  2279,    33,  2281,  2282,  2283,  2284,    33,  2286,
    2287,   236,  2289,  2290,  2291,  2292,  2293,  2294,  2295,  2296,
    2297,  2298,  2299,   414,    34,    35,    36,    37,    38,    39,
      33,   413,    42,    33,    33,  2312,    34,    35,    36,    37,
      38,    39,    33,    33,    42,    33,    33,  2324,  2325,  2326,
    2327,  2328,  2329,  2330,  2331,  2332,  2333,  2334,  2335,    33,
     413,   412,    33,  2340,    33,   412,    33,    33,    33,   413,
      34,    35,    36,    37,    38,    39,    33,   412,    42,    33,
      33,    33,   412,    33,  2361,  2362,  2363,  2364,  2365,  2366,
    2367,   413,  2369,  2370,  2371,  2372,    33,  2374,  2375,    33,
      33,    33,    33,  2380,    34,    35,    36,    37,    38,    39,
      33,  2388,    42,    33,    33,    33,  2393,   236,   236,   236,
    2397,  2398,  2399,    33,    33,    33,    33,  2404,  2405,  2406,
    2407,   237,  2409,  2410,  2411,  2412,    33,  2414,  2415,  2416,
      33,    33,    33,  2420,  2421,  2422,  2423,   413,    33,   412,
      34,    35,    36,    37,    38,    39,    33,    33,    42,   412,
    2437,  2438,  2439,    33,   412,    33,   412,  2444,    33,  2446,
    2447,  2448,   110,  2450,   110,   413,  2453,  2454,  2455,  2456,
      33,   110,    33,  2460,    57,    57,    57,    33,   109,   413,
      33,    33,   109,   109,   237,   237,   237,    33,  2475,  2476,
    2477,    33,  2479,  2480,  2481,  2482,  2483,    33,    33,  2486,
      33,    33,    33,  2490,  2491,  2492,   110,    33,   412,  2496,
     412,   412,    33,    57,    33,  2502,  2503,   109,  2505,  2506,
    2507,    34,    35,    36,    37,    38,    39,   413,    33,    42,
      33,  2518,  2519,    33,    33,  2522,   413,    33,   110,  2526,
    2527,    33,   110,   110,    33,  2532,  2533,  2534,   413,    34,
      35,    36,    37,    38,    39,    57,    57,    42,    33,    57,
     109,   109,  2549,  2550,  2551,  2552,   109,  2554,  2555,  2556,
    2557,  2558,  2559,  2560,    34,    35,    36,    37,    38,    39,
     413,    33,    42,    33,    33,   110,    33,   110,  2575,  2576,
    2577,  2578,   413,    57,  2581,  2582,  2583,  2584,  2585,  2586,
      57,    33,    33,   109,  2591,  2592,    33,    -1,    34,    35,
      36,    37,    38,    39,  2601,  2602,    42,    33,  2605,  2606,
    2607,  2608,  2609,  2610,  2611,  2612,   109,    34,    35,    36,
      37,    38,    39,    33,   413,    42,    33,  2624,  2625,  2626,
    2627,  2628,    33,    33,    33,  2632,    33,    33,    33,    33,
      33,    33,    33,    -1,  2641,  2642,  2643,   413,    -1,  2646,
      34,    35,    36,    37,    38,    39,  2653,   189,    42,    -1,
      -1,    -1,  2659,    -1,    -1,  2662,    -1,    -1,  2665,  2666,
      -1,  2668,    -1,    -1,  2671,  2672,    -1,    -1,  2675,    -1,
    2677,  2678,    -1,   413,    -1,    -1,    -1,    -1,    -1,  2686,
    2687,    -1,    -1,    -1,    -1,   413,    -1,  2694,    -1,    -1,
      -1,    -1,    -1,    -1,  2701,  2702,  2703,    -1,  2705,    -1,
      -1,    -1,  2709,    -1,    -1,    -1,    -1,  2714,  2715,    -1,
      -1,    -1,  2719,    -1,    -1,    -1,  2723,  2724,    -1,   413,
      -1,  2728,    -1,    -1,    -1,  2732,  2733,    -1,    -1,    -1,
    2737,    -1,    -1,  2740,    -1,    -1,  2743,    -1,    -1,  2746,
      -1,    -1,  2749,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    12,   413,    -1,    15,    -1,    17,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    43,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    53,    54,    -1,     3,     4,     5,   413,
       7,     8,     9,    -1,    -1,    12,    -1,    -1,    15,    69,
      17,    -1,    19,    -1,    -1,    75,    76,    77,    78,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    -1,
      90,    91,    -1,    -1,    41,    -1,    43,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    53,    54,   108,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    78,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    -1,    90,    91,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,   108,    42,    -1,    -1,    -1,    -1,    -1,   413,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,   413,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   413,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   413,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   413,
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
      37,    38,    39,    -1,    -1,    42,   406,    -1,    -1,   409,
     410,    -1,   412,    -1,    -1,    -1,    -1,    -1,   418,    -1,
     420,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,   406,
      -1,    -1,   409,   410,    -1,   412,    -1,    -1,    -1,    -1,
      -1,   418,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   413,    -1,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,   413,    -1,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   413,
      -1,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     413,    -1,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   413,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   413,    -1,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   413,
      -1,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   413,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   413,    -1,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     413,    -1,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   413,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
     413,    -1,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   413,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   413,
      -1,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   413,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   413,    -1,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   413,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   413,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     413,    -1,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,   413,    -1,
      42,    34,    35,    36,    37,    38,    39,   413,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   413,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   413,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,   413,
      42,    34,    35,    36,    37,    38,    39,    -1,   413,    42,
      34,    35,    36,    37,    38,    39,    -1,   413,    42,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,   413,    -1,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    -1,    34,
      35,    36,    37,    38,    39,    -1,   413,    42,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    34,    35,    36,
      37,    38,    39,    -1,   413,    42,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   413,    54,    55,    -1,    57,
      58,    -1,    60,    61,   413,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,   413,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,   413,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   413,    -1,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
     413,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   413,    -1,    34,    35,    36,    37,
      38,    39,    -1,   413,    42,    34,    35,    36,    37,    38,
      39,    -1,   413,    42,    34,    35,    36,    37,    38,    39,
      -1,   413,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   413,    -1,    34,    35,
      36,    37,    38,    39,    -1,   413,    42,    -1,    34,    35,
      36,    37,    38,    39,   413,   183,    42,    34,    35,    36,
      37,    38,    39,   413,    -1,    42,    34,    35,    36,    37,
      38,    39,   413,    -1,    42,    34,    35,    36,    37,    38,
      39,   413,    -1,    42,    34,    35,    36,    37,    38,    39,
     413,    -1,    42,    -1,    -1,    -1,   392,    -1,    -1,   413,
      -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   413,    -1,
      -1,    -1,   390,    -1,    34,    35,    36,    37,    38,    39,
      -1,   390,    42,    34,    35,    36,    37,    38,    39,   389,
      -1,    42,    34,    35,    36,    37,    38,    39,   389,    -1,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   389,    34,    35,    36,    37,    38,    39,
      -1,   388,    42,    34,    35,    36,    37,    38,    39,    -1,
     388,    42,    34,    35,    36,    37,    38,    39,    -1,   388,
      42,    34,    35,    36,    37,    38,    39,    -1,   388,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   387,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   387,    -1,    34,    35,    36,    37,    38,    39,
      -1,   387,    42,    34,    35,    36,    37,    38,    39,   386,
      -1,    42,    34,    35,    36,    37,    38,    39,   386,    -1,
      42,    34,    35,    36,    37,    38,    39,   386,    -1,    42,
      34,    35,    36,    37,    38,    39,   386,    -1,    42,    34,
      35,    36,    37,    38,    39,   386,   394,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   367,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   367,    -1,    34,
      35,    36,    37,    38,    39,    -1,   367,    42,    34,    35,
      36,    37,    38,    39,   366,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   366,    34,
      35,    36,    37,    38,    39,   364,    -1,    42,    -1,    -1,
      -1,    -1,   362,    34,    35,    36,    37,    38,    39,    -1,
     361,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   360,    -1,    34,    35,    36,    37,
      38,    39,    -1,   359,    42,    34,    35,    36,    37,    38,
      39,   358,    -1,    42,    -1,    -1,    -1,    -1,    -1,   357,
      34,    35,    36,    37,    38,    39,    -1,   356,    42,    34,
      35,    36,    37,    38,    39,   355,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   354,    42,    34,    35,    36,    37,
      38,    39,   353,    -1,    42,    34,    35,    36,    37,    38,
      39,   353,    -1,    42,    -1,    -1,    -1,    -1,    -1,   352,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   352,    -1,    34,    35,    36,    37,    38,    39,
      -1,   352,    42,    34,    35,    36,    37,    38,    39,   351,
      -1,    42,    34,    35,    36,    37,    38,    39,   351,    -1,
      42,    34,    35,    36,    37,    38,    39,   351,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   350,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     350,    -1,    34,    35,    36,    37,    38,    39,    -1,   350,
      42,    34,    35,    36,    37,    38,    39,   349,    -1,    42,
      34,    35,    36,    37,    38,    39,   349,    -1,    42,    34,
      35,    36,    37,    38,    39,   349,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   348,    -1,    34,    35,    36,    37,    38,
      39,    -1,   348,    42,    34,    35,    36,    37,    38,    39,
      -1,   348,    42,    34,    35,    36,    37,    38,    39,    -1,
     347,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   346,    34,    35,    36,    37,    38,    39,    -1,   345,
      42,    34,    35,    36,    37,    38,    39,   344,    -1,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   343,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     341,    -1,    34,    35,    36,    37,    38,    39,    -1,   341,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     338,    -1,    34,    35,    36,    37,    38,    39,    -1,   338,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   337,    -1,    34,    35,    36,    37,    38,
      39,    -1,   337,    42,    34,    35,    36,    37,    38,    39,
     336,    -1,    42,    34,    35,    36,    37,    38,    39,   336,
      -1,    42,    34,    35,    36,    37,    38,    39,   335,    -1,
      42,    -1,    -1,    -1,    -1,    -1,   334,    34,    35,    36,
      37,    38,    39,    -1,   333,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   332,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,   331,    34,    35,    36,    37,    38,    39,    -1,   330,
      42,    34,    35,    36,    37,    38,    39,   329,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   328,    34,    35,    36,    37,
      38,    39,    -1,   327,    42,    34,    35,    36,    37,    38,
      39,    -1,   326,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   325,    34,    35,    36,    37,    38,    39,
      -1,   324,    42,    34,    35,    36,    37,    38,    39,   323,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   322,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,   321,    34,    35,    36,    37,    38,    39,    -1,
     320,    42,    34,    35,    36,    37,    38,    39,    -1,   320,
      42,    34,    35,    36,    37,    38,    39,   319,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   317,    34,    35,    36,    37,
      38,    39,    -1,   316,    42,    34,    35,    36,    37,    38,
      39,    -1,   315,    42,    34,    35,    36,    37,    38,    39,
     314,    -1,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   313,    42,    34,    35,    36,    37,    38,    39,   312,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,   311,
      42,    34,    35,    36,    37,    38,    39,   310,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     309,    34,    35,    36,    37,    38,    39,    -1,   308,    42,
      34,    35,    36,    37,    38,    39,   307,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   306,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   305,    34,
      35,    36,    37,    38,    39,    -1,   304,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   303,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     302,    34,    35,    36,    37,    38,    39,    -1,   301,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   300,    34,    35,    36,    37,    38,    39,    -1,
     299,    42,    34,    35,    36,    37,    38,    39,   298,    -1,
      42,    34,    35,    36,    37,    38,    39,   297,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   296,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   295,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     291,    -1,    34,    35,    36,    37,    38,    39,    -1,   291,
      42,    34,    35,    36,    37,    38,    39,   290,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   289,    34,    35,    36,    37,    38,    39,    -1,   288,
      42,    34,    35,    36,    37,    38,    39,   287,    -1,    42,
      34,    35,    36,    37,    38,    39,   286,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   285,    -1,    34,    35,    36,    37,
      38,    39,    -1,   285,    42,    34,    35,    36,    37,    38,
      39,    -1,   285,    42,    34,    35,    36,    37,    38,    39,
      -1,   285,    42,    -1,    34,    35,    36,    37,    38,    39,
      -1,   284,    42,    34,    35,    36,    37,    38,    39,    -1,
     284,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   284,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   284,
      -1,    34,    35,    36,    37,    38,    39,    -1,   284,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   284,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     283,    -1,    34,    35,    36,    37,    38,    39,    -1,   283,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,   283,    -1,    34,    35,    36,    37,    38,    39,
      -1,   283,    42,    34,    35,    36,    37,    38,    39,    -1,
     283,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   283,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   283,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,   283,    -1,    34,    35,    36,    37,    38,    39,    -1,
     283,    42,    34,    35,    36,    37,    38,    39,    -1,   283,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   283,    -1,    34,    35,    36,    37,    38,    39,    -1,
     283,    42,    34,    35,    36,    37,    38,    39,    -1,   283,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   283,    -1,    34,    35,    36,
      37,    38,    39,    -1,   283,    42,    34,    35,    36,    37,
      38,    39,    -1,   283,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   283,    42,    34,    35,    36,    37,    38,
      39,    -1,   283,    42,    34,    35,    36,    37,    38,    39,
      -1,   283,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   283,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     283,    34,    35,    36,    37,    38,    39,    -1,   282,    42,
      -1,    34,    35,    36,    37,    38,    39,    -1,   282,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     282,    -1,    34,    35,    36,    37,    38,    39,    -1,   282,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   281,    -1,    34,    35,    36,    37,    38,    39,    -1,
     281,    42,    34,    35,    36,    37,    38,    39,    -1,   281,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   281,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   280,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   279,    -1,
      34,    35,    36,    37,    38,    39,    -1,   279,    42,    34,
      35,    36,    37,    38,    39,    -1,   279,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   279,    -1,
      34,    35,    36,    37,    38,    39,    -1,   279,    42,    34,
      35,    36,    37,    38,    39,    -1,   279,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
      -1,   278,    -1,    34,    35,    36,    37,    38,    39,    -1,
     278,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     278,    42,    34,    35,    36,    37,    38,    39,    -1,   278,
      42,    34,    35,    36,    37,    38,    39,    -1,   278,    42,
      34,    35,    36,    37,    38,    39,    -1,   278,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
     278,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,   278,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   278,    -1,    34,    35,    36,
      37,    38,    39,    -1,   278,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   277,    -1,    34,    35,    36,
      37,    38,    39,    -1,   277,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   277,    -1,    34,    35,
      36,    37,    38,    39,    -1,   277,    42,    34,    35,    36,
      37,    38,    39,    -1,   277,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,   277,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,
     277,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   277,    -1,    34,    35,    36,    37,    38,
      39,    -1,   277,    42,    34,    35,    36,    37,    38,    39,
      -1,   277,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   277,    -1,    34,    35,    36,    37,    38,
      39,    -1,   277,    42,    34,    35,    36,    37,    38,    39,
      -1,   277,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   277,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   277,    -1,    34,    35,
      36,    37,    38,    39,    -1,   277,    42,    34,    35,    36,
      37,    38,    39,    -1,   277,    42,    34,    35,    36,    37,
      38,    39,    -1,   277,    42,    34,    35,    36,    37,    38,
      39,    -1,   277,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,   277,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   277,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     277,    -1,    34,    35,    36,    37,    38,    39,    -1,   277,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     277,    -1,    34,    35,    36,    37,    38,    39,    -1,   277,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   277,    34,    35,    36,    37,    38,    39,    -1,   276,
      42,    34,    35,    36,    37,    38,    39,   275,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   275,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,   275,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   275,    -1,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   275,    -1,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,   275,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,   275,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,   275,
      -1,    34,    35,    36,    37,    38,    39,    -1,   275,    42,
      34,    35,    36,    37,    38,    39,    -1,   275,    42,    34,
      35,    36,    37,    38,    39,    -1,   275,    42,    34,    35,
      36,    37,    38,    39,    -1,   275,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   275,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,   275,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   275,    -1,    34,    35,    36,    37,    38,
      39,    -1,   275,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   275,    -1,    34,    35,    36,    37,    38,
      39,    -1,   275,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   275,    -1,    34,    35,    36,    37,    38,
      39,    -1,   275,    42,    34,    35,    36,    37,    38,    39,
      -1,   275,    42,    34,    35,    36,    37,    38,    39,    -1,
     274,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,   274,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   274,    -1,
      34,    35,    36,    37,    38,    39,    -1,   274,    42,    34,
      35,    36,    37,    38,    39,    -1,   274,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,   242,   274,    -1,
      34,    35,    36,    37,    38,    39,    -1,   274,    42,   242,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,   242,    -1,
      34,    35,    36,    37,    38,    39,    -1,   242,    42,    34,
      35,    36,    37,    38,    39,    -1,   242,    42,    -1,    -1,
      -1,    -1,   239,    -1,    -1,    -1,    -1,    -1,   235,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,   235,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   235,    -1,    34,    35,
      36,    37,    38,    39,    -1,   235,    42,    34,    35,    36,
      37,    38,    39,    -1,   233,    42,    34,    35,    36,    37,
      38,    39,   232,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,   231,    42,    34,    35,    36,    37,    38,    39,
     230,    -1,    42,    -1,    -1,   226,    34,    35,    36,    37,
      38,    39,    -1,   225,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   225,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,   225,    -1,    42,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    34,    35,    36,    37,    38,    39,    -1,   224,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    -1,
     224,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,   223,
      -1,    34,    35,    36,    37,    38,    39,    -1,   223,    42,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,   223,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   222,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,   222,    -1,    34,    35,
      36,    37,    38,    39,    -1,   222,    42,    34,    35,    36,
      37,    38,    39,   221,    -1,    42,    -1,    -1,    -1,    -1,
     219,    -1,    34,    35,    36,    37,    38,    39,    -1,   219,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,   219,    34,    35,    36,    37,    38,    39,    -1,   218,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
     218,    -1,    34,    35,    36,    37,    38,    39,    -1,   218,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   217,    -1,    34,    35,    36,    37,
      38,    39,    -1,   217,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,   217,    34,    35,    36,    37,    38,
      39,    -1,   216,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   216,    -1,    34,    35,    36,    37,    38,
      39,    -1,   216,    42,    34,    35,    36,    37,    38,    39,
     215,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
     215,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    -1,    -1,    -1,   215,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,   214,    -1,
      34,    35,    36,    37,    38,    39,    -1,   214,    42,    34,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   214,    34,    35,    36,    37,    38,    39,    -1,
     213,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   213,    -1,    34,    35,    36,    37,    38,    39,    -1,
     213,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     212,    42,    34,    35,    36,    37,    38,    39,    -1,   212,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   212,    34,    35,    36,    37,    38,
      39,   180,   211,    42,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   211,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   211,    34,    35,    36,    37,    38,    39,   180,    -1,
      42,    -1,    -1,    -1,    -1,   174,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    34,    35,    36,    37,    38,    39,    -1,   174,
      42,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,   174,    -1,    34,    35,    36,    37,    38,
      39,    -1,   174,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   174,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   174,    -1,    34,    35,    36,    37,    38,
      39,    -1,   174,    42,    34,    35,    36,    37,    38,    39,
      -1,   174,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,   174,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   174,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,   174,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    34,    35,    36,    37,
      38,    39,   174,    -1,    42,    34,    35,    36,    37,    38,
      39,   174,    -1,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,    -1,   174,    -1,    34,    35,    36,
      37,    38,    39,    -1,   174,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   174,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   174,    -1,    34,    35,    36,
      37,    38,    39,    -1,   174,    42,    34,    35,    36,    37,
      38,    39,   173,    -1,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   173,    -1,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    34,    35,    36,    37,
      38,    39,    -1,   173,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,   173,    -1,    34,    35,
      36,    37,    38,    39,    -1,   173,    42,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    34,    35,    36,    37,
      38,    39,    -1,   173,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,   173,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   173,    -1,    34,    35,
      36,    37,    38,    39,    -1,   173,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   173,    -1,    34,    35,
      36,    37,    38,    39,    -1,   173,    42,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   173,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   173,    -1,    34,    35,
      36,    37,    38,    39,    -1,   173,    42,    34,    35,    36,
      37,    38,    39,    -1,   173,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   172,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,   172,    42,    34,    35,    36,    37,
      38,    39,    -1,   172,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   172,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   172,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   172,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,   172,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   172,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   172,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,   172,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    -1,   172,    -1,    34,    35,
      36,    37,    38,    39,    -1,   172,    42,    34,    35,    36,
      37,    38,    39,    -1,   172,    42,    34,    35,    36,    37,
      38,    39,    -1,   172,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,   172,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   171,    -1,    34,    35,    36,
      37,    38,    39,    -1,   171,    42,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,   171,    -1,    34,    35,    36,
      37,    38,    39,    -1,   171,    42,    34,    35,    36,    37,
      38,    39,    -1,   171,    42,    -1,    34,    35,    36,    37,
      38,    39,    -1,   171,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,   171,    -1,    34,    35,    36,
      37,    38,    39,    -1,   171,    42,    34,    35,    36,    37,
      38,    39,    -1,   171,    42,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,   161,   171,    34,    35,    36,    37,
      38,    39,    -1,   161,    42,    34,    35,    36,    37,    38,
      39,    -1,   161,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   161,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    -1,   161,    34,    35,    36,    37,    38,
      39,    -1,   160,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,   160,    -1,    34,    35,    36,    37,    38,
      39,    -1,   160,    42,    -1,    34,    35,    36,    37,    38,
      39,    -1,   160,    42,    34,    35,    36,    37,    38,    39,
      -1,   160,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,   159,    -1,    34,    35,    36,    37,    38,    39,
      -1,   159,    42,    34,    35,    36,    37,    38,    39,    -1,
     159,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   159,    -1,    34,    35,    36,    37,    38,    39,    -1,
     159,    42,    -1,    -1,    -1,    -1,   155,    -1,    34,    35,
      36,    37,    38,    39,    -1,   155,    42,    -1,    34,    35,
      36,    37,    38,    39,    -1,   154,    42,    -1,    -1,    -1,
      -1,    -1,   152,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,   152,    -1,    34,    35,    36,    37,    38,
      39,    -1,   151,    42,    34,    35,    36,    37,    38,    39,
      -1,   151,    42,    34,    35,    36,    37,    38,    39,   150,
      -1,    42,    -1,    -1,    34,    35,    36,    37,    38,    39,
     150,   142,    42,    34,    35,    36,    37,    38,    39,    -1,
     142,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     141,    42,    -1,    34,    35,    36,    37,    38,    39,    -1,
     111,    42,    -1,    -1,    -1,   141,    -1,    -1,    34,    35,
      36,    37,    38,    39,   140,   126,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,   126,    -1,    34,
      35,    36,    37,    38,    39,    -1,   126,    42,    34,    35,
      36,    37,    38,    39,    -1,   126,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,   126,    -1,    34,    35,
      36,    37,    38,    39,    -1,   126,    42,    34,    35,    36,
      37,    38,    39,    -1,   125,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   125,    42,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    34,    35,    36,    37,    38,    39,    -1,
     117,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,   116,    -1,    -1,    -1,   111,    -1,    34,    35,    36,
      37,    38,    39,    -1,   111,    42,    -1,    34,    35,    36,
      37,    38,    39,    -1,   110,    42,    34,    35,    36,    37,
      38,    39,    -1,   110,    42,    -1,    -1,    -1,    -1,    -1,
     107,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      34,    35,    36,    37,    38,    39,   107,    -1,    42,    34,
      35,    36,    37,    38,    39,   107,    -1,    42,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    -1,   105,    34,
      35,    36,    37,    38,    39,    -1,   103,    42,    34,    35,
      36,    37,    38,    39,   102,    -1,    42,    34,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    79,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    79,    -1,    34,    35,    36,    37,
      38,    39,    -1,    79,    42,    34,    35,    36,    37,    38,
      39,    -1,    79,    42,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    79,    -1,    34,    35,    36,    37,    38,    39,    -1,
      79,    42,    34,    35,    36,    37,    38,    39,    -1,    79,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,    -1,    -1,   200,   201,   202,   203,   204,
     205,    -1,   207,   208,   209,   210
  };

  const unsigned short int
  feiparser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,     9,    12,    15,
      17,    19,    26,    27,    28,    29,    30,    31,    32,    35,
      41,    43,    44,    45,    46,    49,    51,    53,    54,    69,
      75,    76,    77,    78,    90,    91,   108,   406,   409,   410,
     412,   418,   422,   423,   424,   426,   427,   428,   431,   432,
     433,    18,    20,    21,    22,    23,    24,    25,    33,   412,
     412,   432,   432,     3,   432,   432,    54,    55,    57,   369,
     432,    52,    50,    54,    55,    57,    58,    60,    61,    63,
      64,    65,    74,   183,   394,   429,   415,   406,    54,    54,
      54,    55,    58,    60,    63,    58,    79,    81,    83,    85,
      88,    94,   162,   432,    70,   123,   121,    55,   406,    55,
     406,   432,   418,     0,   433,    34,    35,    36,    37,    38,
      39,    42,   418,     3,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   419,   434,   434,   109,   109,   109,   432,
       5,   109,   109,   430,   109,   109,   109,   109,   401,   123,
     109,   109,   182,   111,   432,   411,   109,   109,   109,   109,
     109,   109,   401,   128,    80,    82,    84,    86,    87,    89,
      96,   163,   125,    71,   124,   432,   406,   406,   414,   432,
     432,   432,   432,   432,   432,   432,   414,   414,   420,   433,
     435,    11,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   106,   109,   432,   432,    63,    54,   416,   122,   432,
     432,   432,   432,   432,   432,    54,   129,   363,   106,   106,
      94,    79,   164,   167,   168,    94,     5,   125,   435,   420,
     434,   103,   126,   126,   111,   111,   111,   126,   402,   432,
     126,   358,   111,   109,   417,   111,   107,   107,   109,   432,
      33,   212,   365,    54,    98,   165,   166,   166,   169,    92,
      95,    99,   412,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   200,   201,   202,   203,
     204,   205,   207,   208,   209,   210,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,    54,    55,    54,
      54,    55,   382,   383,   384,    54,   154,   240,   241,    33,
      54,   432,     3,   432,   104,   112,   425,   425,   432,   432,
      33,    33,   109,    33,   220,   175,   175,   176,   170,    79,
      79,    79,   432,    94,   106,    94,   106,   106,   106,    94,
     106,   106,   106,    94,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   342,
     275,   340,   340,   275,   274,   275,   274,   274,   274,   274,
     109,   109,   109,   109,   109,   106,   106,   106,   109,    33,
     242,   275,   432,   109,   395,   425,   362,   432,   432,   432,
     432,   166,   177,   177,   177,   175,    97,    97,   413,   432,
     114,   432,   114,   114,   114,   432,   114,   114,   114,   432,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,   104,   432,   432,   432,
     432,   432,   385,   385,   385,   432,   432,    33,    33,   359,
     432,    33,    33,   364,   117,   175,   110,   110,   110,   177,
      33,    33,   432,   180,   412,   180,   412,   412,   412,   180,
     412,   412,   412,   180,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     126,   126,   126,   117,    33,    33,    33,   105,   432,   432,
      33,   404,   432,   432,    33,   112,   177,    57,    57,    57,
     110,   432,   432,   413,   179,   432,   179,   432,   432,   432,
     179,   432,   432,   432,   179,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   320,
     336,   336,   320,   275,   275,   275,   275,   275,   275,   289,
     343,   276,   275,   275,   277,   275,   277,   356,   275,   275,
     275,   135,   131,   132,   133,   135,   136,   136,   137,   112,
     432,   432,   432,   403,   110,   277,   432,   112,   396,   361,
     432,   129,   110,   109,   109,   109,    57,   101,   432,   181,
     413,   181,   413,   413,   413,   181,   413,   413,   413,   181,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,   115,   115,   115,   110,
     110,   103,    97,   140,   386,   386,   386,    54,    57,    33,
     360,    63,    33,    33,   432,    57,   432,   432,   432,   109,
      33,   414,   238,   432,   238,   432,   432,   432,   238,   432,
     432,   432,   238,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,    33,
      33,    33,    74,    74,   114,    33,    33,    33,    33,    33,
     109,   109,   432,    33,   184,   432,   432,   109,   159,   159,
     159,   432,   432,   106,   106,   413,   106,   413,   413,   413,
     106,   413,   413,   413,   106,   413,   413,   414,   414,   414,
     414,   413,   413,   413,   413,   414,   414,   413,   413,   414,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   317,
     337,   337,   321,   277,   277,   277,   277,   277,   277,   290,
     344,   341,   341,   348,   277,   348,   277,   277,   277,   277,
     432,   432,   432,   109,   109,   412,   432,   432,   432,   432,
     432,   432,   432,   242,   432,   405,   397,   432,    33,    33,
      33,   159,   100,   432,   114,   432,   114,   432,   432,   432,
     114,   432,   432,   432,   114,   432,   432,   106,   221,   221,
     221,   432,   432,   432,   432,   110,   229,   432,   432,   221,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      97,   155,   432,   432,   432,   140,   150,   387,   388,   388,
     404,    33,   432,    33,   159,   432,   432,   432,    33,    33,
     107,   412,   413,   412,   413,   413,   413,   412,   413,   413,
     413,   412,   413,   413,   432,    33,    33,    33,   413,   413,
     414,   413,    57,    33,   413,   413,    33,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,    33,    33,   413,
      33,    33,    33,    33,    33,   425,   432,   432,    33,   160,
     160,   160,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   116,   432,   432,
     432,   432,   432,   110,   432,   109,   432,   432,   432,   432,
     278,   278,   278,   278,   278,   278,   278,   278,   297,   367,
     367,   277,   279,   279,   279,   279,   279,   279,   286,   345,
     386,   391,   349,   278,   349,   348,   278,   280,   432,     5,
     432,   432,   432,   381,   432,   432,   274,   398,   432,    33,
      33,    33,   160,   102,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   110,   275,   275,
     275,   414,   414,    57,   414,   432,   230,   413,   413,   110,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,   155,   413,
     150,   141,   387,   389,    33,    33,   160,   432,   432,   432,
      33,    33,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,    65,    33,    33,    33,   110,
     110,   109,   110,   221,    33,   432,   432,    57,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,    33,   432,    33,    33,
      33,    33,   432,   432,    33,   161,   161,   161,   432,   432,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   109,   432,   432,   432,    57,    57,   432,
      57,    33,   432,   413,   413,   109,   283,   284,   285,   285,
     281,   283,   284,   281,   295,   366,   366,   319,   284,   284,
     281,   281,   283,   283,   287,   367,   388,   392,   350,   283,
     350,   349,   285,   285,     5,   413,   432,   432,   381,   432,
     399,   432,    33,    33,    33,   161,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     222,   222,   222,   109,   109,   242,   109,   432,   231,   432,
     432,   432,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     432,   141,   152,   387,    33,   161,   432,   432,   432,    33,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   274,    33,    33,    33,   432,   432,    33,
     432,   274,    33,   413,   413,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   413,   414,    33,    33,    33,   432,    33,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,    33,   432,   432,   432,   242,   242,
     432,   242,    33,   432,   432,   432,    79,   283,   283,   283,
     282,   172,   283,   282,   296,   277,   277,   322,   283,   283,
     282,   282,    79,   172,   288,   346,   389,   393,   351,   172,
     351,   350,   284,   284,   432,   106,   432,   432,   381,   400,
     432,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   432,   223,   223,   223,    33,    33,
      33,   432,   232,   413,   413,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   413,   157,   158,   152,   142,    33,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   226,    33,    33,    33,   432,   432,   432,    33,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   412,   432,    33,    33,   432,   413,   414,   413,   413,
     414,   413,   413,   413,   413,   414,   413,   414,   413,    33,
     432,   432,   432,   432,   414,   413,   171,    79,    79,   172,
     283,   173,   172,   283,   298,   338,   338,   323,    79,   172,
     283,   283,   171,   173,   347,   390,   386,   352,   173,   352,
     351,   413,   432,   432,   432,   432,   110,   432,   432,   110,
     432,   432,   432,   432,   110,   432,   110,   432,   432,   225,
     225,   225,   233,   110,   432,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     432,   413,   142,   151,   413,    57,   413,   413,    57,   413,
     413,   413,   413,    57,   413,    57,   413,   235,    33,    33,
      33,    33,    57,   413,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   413,
     432,    33,    33,   432,   109,   432,   432,   109,   432,   432,
     432,   432,   109,   432,   109,   432,    33,   432,   432,   432,
     432,   109,   432,   172,   171,   171,   173,    79,   174,   173,
     172,   299,   291,   291,   324,   171,   173,    79,   172,   172,
     174,   388,   274,   174,   274,   352,   432,   413,   432,   432,
     413,   432,   413,   413,   432,   413,   413,   413,   413,   432,
     413,   432,   413,   412,   224,   224,   224,   239,   432,   413,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,   413,   414,   432,   151,   432,   432,   432,
     211,   432,   432,   432,   432,   211,   432,   432,   432,    33,
      33,    33,    33,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   106,   413,
      33,   414,   413,   413,    33,   413,   413,   413,   413,    33,
     413,   413,   413,   432,   432,   432,   412,   413,   173,   172,
     172,   174,   171,   174,   173,   300,   283,   283,   325,   172,
     174,   171,   173,   173,   389,   283,   283,   283,   414,   157,
     158,   432,   432,   110,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   274,   274,   274,   432,   432,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
     106,   412,   432,   414,    57,   413,   413,   212,   413,   413,
     413,   413,   212,   413,   413,   413,    33,    33,    33,   413,
     413,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   157,   158,   432,   109,   432,   432,    33,   432,   432,
     432,   432,    33,   432,   432,   432,   432,   432,   432,   432,
     432,   174,   173,   173,   172,   174,   301,    79,   172,   326,
     173,   172,   174,   174,   390,   353,   353,   354,   412,   432,
     413,   432,   413,   413,   432,   413,   413,   413,   413,   432,
     413,   413,   414,   235,   235,   235,   413,   413,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,   432,   432,   432,   432,   213,
     432,   432,   432,   432,   213,   432,   432,   236,    33,    33,
      33,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     413,   413,   413,   413,    33,   413,   413,   413,   413,    33,
     413,   413,    33,   412,   412,   412,   414,   413,   174,   174,
     173,   302,   171,   173,   327,   174,   173,   355,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     412,   432,   432,   432,   432,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,   413,   413,   413,   413,   214,
     413,   413,   413,   413,   214,   413,   413,   432,   413,   413,
     413,   413,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,    33,   432,   432,   432,
     432,    33,   432,   432,   413,   432,   432,   432,   432,   174,
     303,   172,   174,   328,   174,   357,   413,   413,   413,   413,
     432,   413,   413,   413,   413,   432,   413,   413,   432,   413,
     413,   413,   413,    33,    33,    33,    33,    33,    33,    33,
     432,   432,   432,   432,   215,   432,   432,   432,   432,   215,
     432,   432,   413,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   413,   413,   413,   413,    33,   413,
     413,   413,   413,    33,   413,   413,   432,   414,   414,   414,
     413,   304,   173,   329,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   414,   236,   236,   236,
     432,    33,    33,    33,   413,   413,   413,   413,   216,   413,
     413,   413,   413,   216,   413,   413,   237,    33,    33,    33,
     413,   432,   432,   432,   432,   432,   432,   432,    33,   432,
     432,   432,   432,    33,   432,   432,    33,   412,   412,   412,
     432,   305,   174,   330,   413,   413,   413,   413,   432,   413,
     413,   413,   413,   432,   413,   413,   412,   432,   432,   432,
     413,    33,    33,    33,   432,   432,   432,   432,   217,   432,
     432,   432,   432,   217,   432,   432,   432,   413,   413,   413,
     432,   432,   432,   432,   413,   414,   413,   413,    33,   414,
     413,   414,   414,    33,   413,   413,   413,   432,   432,   432,
     413,   306,   331,   432,   432,   432,   432,   110,   432,   110,
     110,   432,   432,   432,   432,   413,   413,   413,   432,    33,
      33,   413,   413,   413,   218,    57,   413,    57,    57,   218,
     413,   413,   413,   432,   432,   432,   413,   432,   432,   432,
     432,   432,    33,   109,   432,   109,   109,    33,   432,   432,
     432,   414,   414,   414,   432,   307,   332,   414,   413,   413,
     432,   432,   413,   432,   432,   432,   413,   413,   414,   237,
     237,   237,   413,    33,    33,   432,   432,   219,   211,   432,
     219,   432,   432,    33,    33,    33,   432,   432,   432,   413,
     413,    33,    33,   414,    33,   413,   413,   412,   412,   412,
     413,   308,   333,   432,   432,   432,   432,   110,   432,   432,
     432,   432,   432,   432,   432,    33,    33,   413,   413,   212,
      57,   413,   413,   413,   413,   413,   413,   432,   432,   432,
     432,    33,   109,   432,   432,   432,   432,   432,   432,   309,
     334,   413,   413,   432,   432,   413,   413,   413,   413,   413,
     413,    33,    33,   432,   432,   213,   432,   432,   432,   432,
     432,   432,   432,   432,   413,   413,    33,   413,   413,   414,
     414,   414,   413,   310,   335,   432,   432,   432,   432,   432,
     432,    33,    33,   413,   414,   214,   413,   414,   414,   432,
     432,   432,   110,    33,   432,   110,   110,   311,   283,   413,
      57,   432,   413,    57,    57,    33,    33,   432,   109,   215,
     432,   109,   109,   432,   432,   413,   432,    33,   413,   432,
     432,   312,    79,   432,   432,   432,    33,    33,   414,   216,
     414,   432,   432,   110,    33,   110,   313,   171,    57,   432,
      57,    33,    33,   109,   217,   109,   432,   432,   432,    33,
     432,   314,   172,   432,    33,    33,   218,   432,   432,    33,
     315,   173,   432,    33,    33,   219,   432,   432,    33,   316,
     174,   432,    33,    33,   432,   432,    79,    33,   432,   171,
      33,   432,   172,    33,   432,   173,    33,   432,   174,    33,
     432
  };

  const unsigned short int
  feiparser::yyr1_[] =
  {
       0,   421,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   424,   424,   424,
     424,   425,   425,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   427,   427,   427,   427,
     427,   427,   427,   427,   427,   427,   427,   427,   428,   428,
     428,   428,   428,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     431,   431,   431,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   432,   432,   432,   432,   432,   432,
     432,   432,   432,   432,   433,   433,   433,   433,   433,   433,
     433,   433,   433,   434,   434,   435,   435
  };

  const unsigned char
  feiparser::yyr2_[] =
  {
       0,     2,     2,     2,     4,     4,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,    15,     2,    13,
       3,    14,    14,    24,    32,    32,    48,    34,    52,    13,
      16,    19,    10,    10,    31,    28,    16,    19,    22,     8,
       8,    15,    24,    15,     8,    13,    18,     6,     6,    14,
       6,     1,     2,     2,     2,     3,     4,     6,     3,     5,
       3,    12,     3,    10,     7,    11,     3,     4,     6,     9,
      18,     7,    22,    20,    20,    21,    20,     3,     4,     4,
       4,     4,     6,    14,    35,    29,    29,    38,    32,    41,
      38,    35,    32,    23,    23,    95,    47,    41,    80,    38,
      32,    41,    35,    35,    29,    23,    11,    26,    35,    26,
      35,    35,    14,    41,    27,    27,    18,    27,    33,    65,
      65,    71,    65,    71,    51,    57,    51,    54,    54,    78,
      59,    59,    51,    59,    35,    22,    22,    20,    22,    21,
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
  "FourNodeShellANDES", "truss", "contact", "FrictionalPenaltyContact",
  "EightNodeBrickLT", "TwentySevenNodeBrickLT", "ShearBeamLT", "porosity",
  "alpha", "rho_s", "rho_f", "k_x", "k_y", "k_z", "K_s", "K_f", "pressure",
  "cross_section", "shear_modulus", "torsion_Jx", "bending_Iz",
  "bending_Iy", "IntegrationRule", "number_of_integration_points",
  "stiffness", "normal_stiffness", "tangential_stiffness",
  "normal_damping", "tangential_damping", "friction_ratio", "maximum_gap",
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
  "linear_elastic_isotropic_3d_LT", "VonMisesLT", "DruckerPragerLT",
  "mass_density", "elastic_modulus", "viscoelastic_modulus",
  "poisson_ratio", "von_mises_radius", "druckerprager_angle",
  "druckerprager_k", "armstrong_frederick_ha", "armstrong_frederick_cr",
  "initial_confining_stress", "isotropic_hardening_rate",
  "kinematic_hardening_rate", "poisson_ratio_h_v", "poisson_ratio_h_h",
  "shear_modulus_h_v", "elastic_modulus_horizontal",
  "elastic_modulus_vertical", "pressure_reference_p0",
  "initial_void_ratio", "initial_shear_modulus", "initial_bulk_modulus",
  "sanisand2008_Pat", "sanisand2008_k_c", "sanisand2008_K0",
  "sanisand2008_alpha_cc", "sanisand2008_c", "sanisand2008_xi",
  "sanisand2008_lambda", "sanisand2008_ec_ref", "sanisand2008_m",
  "sanisand2008_h0", "sanisand2008_ch", "sanisand2008_nb",
  "sanisand2008_A0", "sanisand2008_nd", "sanisand2008_p_r",
  "sanisand2008_rho_c", "sanisand2008_theta_c", "sanisand2008_X",
  "sanisand2008_z_max", "sanisand2008_cz", "sanisand2008_p0",
  "sanisand2008_p_in", "sanisand2008_G0", "sanisand2004_K0",
  "sanisand2004_Pat", "e0", "sanisand2004_G0", "sanisand2004_p_cut",
  "sanisand2004_Mc", "sanisand2004_c", "sanisand2004_lambda_c",
  "sanisand2004_xi", "sanisand2004_ec_ref", "sanisand2004_m",
  "sanisand2004_h0", "sanisand2004_ch", "sanisand2004_nb",
  "sanisand2004_A0", "sanisand2004_nd", "sanisand2004_z_max",
  "sanisand2004_cz", "reference_void_ratio", "critical_stress_ratio_M",
  "minimum_bulk_modulus", "initial_mean_pressure", "yield_strength",
  "strain_hardening_ratio", "compressive_strength",
  "strain_at_compressive_strength", "crushing_strength",
  "strain_at_crushing_strength", "tensile_strength",
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
       0,   299,   299,   313,   323,   346,   359,   370,   458,   466,
     472,   473,   474,   475,   476,   480,   490,   494,   554,   558,
     571,   595,   613,   631,   657,   686,   716,   753,   784,   824,
     844,   877,   913,   928,   943,   995,  1118,  1136,  1155,  1174,
    1190,  1206,  1224,  1245,  1323,  1340,  1356,  1386,  1426,  1440,
    1462,  1491,  1495,  1504,  1513,  1520,  1527,  1535,  1545,  1555,
    1568,  1616,  1653,  1682,  1696,  1709,  1736,  1774,  1802,  1815,
    1831,  1854,  1868,  1892,  1916,  1940,  1964,  1989,  2005,  2018,
    2031,  2045,  2057,  2078,  2096,  2130,  2159,  2188,  2222,  2253,
    2290,  2325,  2358,  2389,  2425,  2461,  2544,  2591,  2632,  2706,
    2746,  2781,  2824,  2863,  2901,  2934,  2959,  2974,  3003,  3038,
    3067,  3105,  3143,  3162,  3213,  3239,  3264,  3283,  3308,  3335,
    3382,  3429,  3478,  3525,  3576,  3616,  3658,  3698,  3745,  3783,
    3837,  3901,  3965,  4068,  4155,  4192,  4218,  4244,  4268,  4293,
    4485,  4527,  4569,  4584,  4629,  4636,  4643,  4650,  4657,  4664,
    4671,  4677,  4684,  4692,  4700,  4708,  4716,  4724,  4728,  4734,
    4739,  4745,  4751,  4757,  4763,  4769,  4777,  4783,  4788,  4793,
    4798,  4803,  4808,  4816,  4847,  4852,  4856,  4865,  4887,  4912,
    4932,  4950,  4961,  4971,  4977,  4985,  4989
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
     412,   414,    36,    34,   413,    35,   417,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   418,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   415,     2,   416,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   419,     2,   420,     2,     2,     2,     2,
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
     403,   404,   405,   406,   407,   408,   409,   410,   411
    };
    const unsigned int user_token_number_max_ = 658;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 8068 "feiparser.tab.cc" // lalr1.cc:1155
#line 5017 "feiparser.yy" // lalr1.cc:1156


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



