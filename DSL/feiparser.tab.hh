// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file feiparser.tab.hh
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_FEIPARSER_TAB_HH_INCLUDED
# define YY_YY_FEIPARSER_TAB_HH_INCLUDED


# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"
# include "location.hh"


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif


namespace yy {
#line 113 "feiparser.tab.hh" // lalr1.cc:372





  /// A Bison parser.
  class feiparser
  {
  public:
#ifndef YYSTYPE
    /// Symbol semantic values.
    union semantic_type
    {
    #line 109 "feiparser.yy" // lalr1.cc:372

  Expression    *exp;           /* For the expressions. Since it is a pointer, no problem. */
  Quantity      *value;         /* For the lexical analyser. NUMBER tokens */
  string        *ident;         /* For the lexical analyser. IDENT tokens */
  int           fn;
  int           errcode;

#line 135 "feiparser.tab.hh" // lalr1.cc:372
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        IDENT = 258,
        NUMBER = 259,
        STRING = 260,
        EOL = 261,
        ISUNITTYPE = 262,
        FUNCTION = 263,
        IF = 264,
        THEN = 265,
        ELSE = 266,
        WHILE = 267,
        DO = 268,
        LET = 269,
        EXITSAFEMODE = 270,
        CONTINUE = 271,
        BYE = 272,
        IN = 273,
        REQUIRE = 274,
        PLUSEQUAL = 275,
        MINUSEQUAL = 276,
        TIMESEQUAL = 277,
        DIVIDEEQUAL = 278,
        MODULUSEQUAL = 279,
        POWEQUAL = 280,
        NUMBER_OF_NODES = 281,
        NUMBER_OF_ELEMENTS = 282,
        CURRENT_TIME = 283,
        NUMBER_OF_SP_CONSTRAINTS = 284,
        NUMBER_OF_MP_CONSTRAINTS = 285,
        NUMBER_OF_LOADS = 286,
        UMINUS = 287,
        CMP = 288,
        CMD_print = 289,
        CMD_help = 290,
        CMD_whos = 291,
        CMD_check = 292,
        CMD_save = 293,
        CMD_restore = 294,
        MODEL = 295,
        NAME = 296,
        RESTART = 297,
        MESH = 298,
        ADD = 299,
        NODE = 300,
        ELEMENT = 301,
        MATERIAL = 302,
        LOAD = 303,
        TIMEHISTORY = 304,
        IMPOSEDMOTION = 305,
        DAMPING = 306,
        DAMPINGTYPE = 307,
        CONSTRAINT = 308,
        DRM = 309,
        SAVEFORCES = 310,
        RESTOREFORCES = 311,
        SECTION = 312,
        LOADPATTERN = 313,
        PENALTYDISPLACEMENT = 314,
        LOADVALUE = 315,
        ELEMENTNAME = 316,
        MATERIALNAME = 317,
        ACCELERATION_FIELD = 318,
        FIX = 319,
        FREE = 320,
        REMOVE = 321,
        DEFINE = 322,
        ALGORITHM = 323,
        ALGNAME = 324,
        CONVERGENCE_TEST = 325,
        TESTNAME = 326,
        SOLVER = 327,
        SOLVERNAME = 328,
        DYNAMICINTEGRATOR = 329,
        DYNAMICINTEGRATOR_HHT = 330,
        DYNAMICINTEGRATOR_NEWMARK = 331,
        STATICINTEGRATOR = 332,
        STATICINTEGRATOR_DISPLACEMENT = 333,
        SIMULATE = 334,
        STATIC = 335,
        DYNAMIC = 336,
        USING = 337,
        TRANSIENT = 338,
        EIGEN = 339,
        time_step = 340,
        number_of_modes = 341,
        VARIABLETRANSIENT = 342,
        maximum_time_step = 343,
        minimum_time_step = 344,
        number_of_iterations = 345,
        AT = 346,
        ALL = 347,
        AND = 348,
        WITH = 349,
        TEXTDOFS = 350,
        NEW = 351,
        TEXTNUMBER = 352,
        USE = 353,
        TO = 354,
        DOF = 355,
        TEXTWITH = 356,
        NODES = 357,
        FORCE = 358,
        INTEGRATIONPOINTS = 359,
        dof = 360,
        RESPONSE = 361,
        LOADING = 362,
        STAGE = 363,
        STEPS = 364,
        TYPE = 365,
        DOFS = 366,
        FACTOR = 367,
        INCREMENT = 368,
        TH_GROUNDMOTION = 369,
        TH_LINEAR = 370,
        TH_PATH_SERIES = 371,
        TH_PATH_TIME_SERIES = 372,
        TH_CONSTANT = 373,
        TH_FROM_REACTIONS = 374,
        self_weight = 375,
        surface = 376,
        load_value = 377,
        scale_factor = 378,
        displacement_scale_unit = 379,
        velocity_scale_unit = 380,
        acceleration_scale_unit = 381,
        number_of_steps = 382,
        number_of_drm_nodes = 383,
        number_of_drm_elements = 384,
        x_max = 385,
        x_min = 386,
        y_max = 387,
        y_min = 388,
        z_max = 389,
        z_min = 390,
        element_file = 391,
        nodes_file = 392,
        displacement_file = 393,
        acceleration_file = 394,
        velocity_file = 395,
        force_file = 396,
        series_file = 397,
        time_series_file = 398,
        MAGNITUDES = 399,
        MAGNITUDE = 400,
        strain_increment_size = 401,
        maximum_strain = 402,
        number_of_times_reaching_maximum_strain = 403,
        constitutive = 404,
        testing = 405,
        constant = 406,
        mean = 407,
        triaxial = 408,
        drained = 409,
        undrained = 410,
        simple = 411,
        shear = 412,
        number_of_subincrements = 413,
        maximum_number_of_iterations = 414,
        tolerance_1 = 415,
        tolerance_2 = 416,
        strain = 417,
        stress = 418,
        control = 419,
        Guass = 420,
        points = 421,
        Gauss = 422,
        each = 423,
        point = 424,
        single = 425,
        value = 426,
        EightNodeBrick = 427,
        TwentySevenNodeBrick = 428,
        EightNodeBrick_upU = 429,
        TwentyNodeBrick_uPU = 430,
        TwentyNodeBrick = 431,
        TwentyNodeBrickElastic = 432,
        EightNodeBrick_up = 433,
        variable_node_brick_8_to_27 = 434,
        EightNodeBrickElastic = 435,
        TwentySevenNodeBrickElastic = 436,
        beam_displacement_based = 437,
        beam_elastic = 438,
        beam_elastic_lumped_mass = 439,
        penalty = 440,
        penalty_for_applying_generalized_displacement = 441,
        beam_9dof_elastic = 442,
        FourNodeShellMITC4 = 443,
        FourNodeShellNewMITC4 = 444,
        ThreeNodeShellANDES = 445,
        FourNodeShellANDES = 446,
        truss = 447,
        contact = 448,
        EightNodeBrickLT = 449,
        TwentySevenNodeBrickLT = 450,
        porosity = 451,
        alpha = 452,
        rho_s = 453,
        rho_f = 454,
        k_x = 455,
        k_y = 456,
        k_z = 457,
        K_s = 458,
        K_f = 459,
        pressure = 460,
        cross_section = 461,
        shear_modulus = 462,
        torsion_Jx = 463,
        bending_Iz = 464,
        bending_Iy = 465,
        IntegrationRule = 466,
        number_of_integration_points = 467,
        stiffness = 468,
        normal_stiffness = 469,
        tangential_stiffness = 470,
        friction_ratio = 471,
        maximum_gap = 472,
        xz_plane_vector = 473,
        joint_1_offset = 474,
        joint_2_offset = 475,
        direction = 476,
        contact_plane_vector = 477,
        MembranePlateFiber = 478,
        ElasticMembranePlate = 479,
        thickness = 480,
        linear_elastic_isotropic_3d = 481,
        vonmises_perfectly_plastic = 482,
        vonmises_isotropic_hardening = 483,
        vonmises_linear_kinematic_hardening = 484,
        vonmises_linear_kinematic_hardening_accelerated = 485,
        vonmises_kinematic_hardening = 486,
        vonmises_perfectly_plastic_accelerated = 487,
        vonmises_isotropic_hardening_accelerated = 488,
        vonmises_kinematic_hardening_accelerated = 489,
        sanisand2008 = 490,
        camclay = 491,
        camclay_accelerated = 492,
        sanisand2004 = 493,
        druckerprager_isotropic_hardening = 494,
        druckerprager_isotropic_hardening_accelerated = 495,
        druckerprager_kinematic_hardening = 496,
        druckerprager_kinematic_hardening_accelerated = 497,
        druckerprager_perfectly_plastic = 498,
        druckerprager_perfectly_plastic_accelerated = 499,
        linear_elastic_crossanisotropic = 500,
        uniaxial_concrete02 = 501,
        uniaxial_elastic_1d = 502,
        uniaxial_steel01 = 503,
        uniaxial_steel02 = 504,
        pisano = 505,
        vonmises_perfectly_plastic_LT = 506,
        pisanoLT = 507,
        New_PisanoLT = 508,
        linear_elastic_isotropic_3d_LT = 509,
        mass_density = 510,
        elastic_modulus = 511,
        viscoelastic_modulus = 512,
        poisson_ratio = 513,
        von_mises_radius = 514,
        druckerprager_angle = 515,
        armstrong_frederick_ha = 516,
        armstrong_frederick_cr = 517,
        initial_confining_stress = 518,
        isotropic_hardening_rate = 519,
        kinematic_hardening_rate = 520,
        poisson_ratio_h_v = 521,
        poisson_ratio_h_h = 522,
        shear_modulus_h_v = 523,
        elastic_modulus_horizontal = 524,
        elastic_modulus_vertical = 525,
        pressure_reference_p0 = 526,
        initial_void_ratio = 527,
        initial_shear_modulus = 528,
        initial_bulk_modulus = 529,
        sanisand2008_Pat = 530,
        sanisand2008_k_c = 531,
        sanisand2008_K0 = 532,
        sanisand2008_alpha_cc = 533,
        sanisand2008_c = 534,
        sanisand2008_xi = 535,
        sanisand2008_lambda = 536,
        sanisand2008_ec_ref = 537,
        sanisand2008_m = 538,
        sanisand2008_h0 = 539,
        sanisand2008_ch = 540,
        sanisand2008_nb = 541,
        sanisand2008_A0 = 542,
        sanisand2008_nd = 543,
        sanisand2008_p_r = 544,
        sanisand2008_rho_c = 545,
        sanisand2008_theta_c = 546,
        sanisand2008_X = 547,
        sanisand2008_z_max = 548,
        sanisand2008_cz = 549,
        sanisand2008_p0 = 550,
        sanisand2008_p_in = 551,
        sanisand2008_G0 = 552,
        sanisand2004_K0 = 553,
        sanisand2004_Pat = 554,
        e0 = 555,
        sanisand2004_G0 = 556,
        sanisand2004_p_cut = 557,
        sanisand2004_Mc = 558,
        sanisand2004_c = 559,
        sanisand2004_lambda_c = 560,
        sanisand2004_xi = 561,
        sanisand2004_ec_ref = 562,
        sanisand2004_m = 563,
        sanisand2004_h0 = 564,
        sanisand2004_ch = 565,
        sanisand2004_nb = 566,
        sanisand2004_A0 = 567,
        sanisand2004_nd = 568,
        sanisand2004_z_max = 569,
        sanisand2004_cz = 570,
        reference_void_ratio = 571,
        critical_stress_ratio_M = 572,
        minimum_bulk_modulus = 573,
        initial_mean_pressure = 574,
        yield_strength = 575,
        strain_hardening_ratio = 576,
        compressive_strength = 577,
        strain_at_compressive_strength = 578,
        crushing_strength = 579,
        strain_at_crushing_strength = 580,
        tensile_strength = 581,
        tension_softening_stiffness = 582,
        M_in = 583,
        kd_in = 584,
        xi_in = 585,
        h_in = 586,
        m_in = 587,
        beta_min = 588,
        ax = 589,
        ay = 590,
        az = 591,
        verbose_level = 592,
        maximum_iterations = 593,
        tolerance = 594,
        beta = 595,
        gamma = 596,
        kappa = 597,
        lambda = 598,
        delta = 599,
        DOMAIN_ = 600,
        startTime = 601,
        endTime = 602,
        Period = 603,
        Phase = 604,
        Amplitude = 605,
        frequency = 606,
        MaxTime = 607,
        frequency1 = 608,
        frequency2 = 609,
        frequency3 = 610,
        frequency4 = 611,
        stiffness_to_use_opt = 612,
        DAMPING_RAYLEIGH = 613,
        DAMPING_CAUGHEY3 = 614,
        DAMPING_CAUGHEY4 = 615,
        a0 = 616,
        a1 = 617,
        stiffness_to_use = 618,
        a2 = 619,
        a3 = 620,
        a4 = 621,
        R0 = 622,
        cR1 = 623,
        cR2 = 624,
        MASS = 625,
        mx = 626,
        my = 627,
        mz = 628,
        Imx = 629,
        Imy = 630,
        Imz = 631,
        equaldof = 632,
        master = 633,
        slave = 634,
        dof_to_constrain = 635,
        of = 636,
        OUTPUT = 637,
        BINARY = 638,
        TEXT = 639,
        ENABLE = 640,
        DISABLE = 641
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned short int token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t,
                    const location_type& l);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      ~basic_symbol ();

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;


    /// Build a parser object.
    feiparser ();
    virtual ~feiparser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    feiparser (const feiparser&);
    feiparser& operator= (const feiparser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (int t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short int yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned short int yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned short int yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 10494,     ///< Last index in yytable_.
      yynnts_ = 15,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 105, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 404  ///< Number of tokens.
    };


  };



} // yy
#line 841 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
