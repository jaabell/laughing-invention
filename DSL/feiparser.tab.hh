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
    #line 110 "feiparser.yy" // lalr1.cc:372

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
        IS_PARALLEL = 287,
        UMINUS = 288,
        CMP = 289,
        CMD_print = 290,
        CMD_help = 291,
        CMD_whos = 292,
        CMD_check = 293,
        CMD_save = 294,
        CMD_restore = 295,
        MODEL = 296,
        NAME = 297,
        RESTART = 298,
        MESH = 299,
        ADD = 300,
        NODE = 301,
        ELEMENT = 302,
        ELEMENTS = 303,
        MATERIAL = 304,
        LOAD = 305,
        TIMEHISTORY = 306,
        IMPOSEDMOTION = 307,
        DAMPING = 308,
        DAMPINGTYPE = 309,
        CONSTRAINT = 310,
        DRM = 311,
        SAVEFORCES = 312,
        RESTOREFORCES = 313,
        SECTION = 314,
        LOADPATTERN = 315,
        PENALTYDISPLACEMENT = 316,
        LOADVALUE = 317,
        ELEMENTNAME = 318,
        MATERIALNAME = 319,
        ACCELERATION_FIELD = 320,
        FIX = 321,
        FREE = 322,
        REMOVE = 323,
        DEFINE = 324,
        ALGORITHM = 325,
        ALGNAME = 326,
        CONVERGENCE_TEST = 327,
        TESTNAME = 328,
        SOLVER = 329,
        SOLVERNAME = 330,
        DYNAMICINTEGRATOR = 331,
        DYNAMICINTEGRATOR_HHT = 332,
        DYNAMICINTEGRATOR_NEWMARK = 333,
        STATICINTEGRATOR = 334,
        STATICINTEGRATOR_DISPLACEMENT = 335,
        SIMULATE = 336,
        STATIC = 337,
        DYNAMIC = 338,
        USING = 339,
        TRANSIENT = 340,
        EIGEN = 341,
        time_step = 342,
        number_of_modes = 343,
        VARIABLETRANSIENT = 344,
        maximum_time_step = 345,
        minimum_time_step = 346,
        number_of_iterations = 347,
        AT = 348,
        ALL = 349,
        AND = 350,
        WITH = 351,
        TEXTDOFS = 352,
        NEW = 353,
        TEXTNUMBER = 354,
        USE = 355,
        TO = 356,
        DOF = 357,
        TEXTWITH = 358,
        NODES = 359,
        FORCE = 360,
        INTEGRATIONPOINTS = 361,
        dof = 362,
        RESPONSE = 363,
        FILE = 364,
        FROM = 365,
        LOADING = 366,
        STAGE = 367,
        STEPS = 368,
        TYPE = 369,
        DOFS = 370,
        FACTOR = 371,
        INCREMENT = 372,
        TH_GROUNDMOTION = 373,
        TH_LINEAR = 374,
        TH_PATH_SERIES = 375,
        TH_PATH_TIME_SERIES = 376,
        TH_CONSTANT = 377,
        TH_FROM_REACTIONS = 378,
        self_weight = 379,
        surface = 380,
        load_value = 381,
        scale_factor = 382,
        displacement_scale_unit = 383,
        velocity_scale_unit = 384,
        acceleration_scale_unit = 385,
        number_of_steps = 386,
        number_of_drm_nodes = 387,
        number_of_drm_elements = 388,
        x_max = 389,
        x_min = 390,
        y_max = 391,
        y_min = 392,
        z_max = 393,
        z_min = 394,
        element_file = 395,
        nodes_file = 396,
        displacement_file = 397,
        acceleration_file = 398,
        velocity_file = 399,
        force_file = 400,
        series_file = 401,
        time_series_file = 402,
        MAGNITUDES = 403,
        MAGNITUDE = 404,
        strain_increment_size = 405,
        maximum_strain = 406,
        number_of_times_reaching_maximum_strain = 407,
        constitutive = 408,
        testing = 409,
        constant = 410,
        mean = 411,
        triaxial = 412,
        drained = 413,
        undrained = 414,
        simple = 415,
        shear = 416,
        number_of_subincrements = 417,
        maximum_number_of_iterations = 418,
        tolerance_1 = 419,
        tolerance_2 = 420,
        strain = 421,
        stress = 422,
        control = 423,
        Guass = 424,
        points = 425,
        Gauss = 426,
        each = 427,
        point = 428,
        single = 429,
        value = 430,
        EightNodeBrick = 431,
        TwentySevenNodeBrick = 432,
        EightNodeBrick_upU = 433,
        TwentyNodeBrick_uPU = 434,
        TwentyNodeBrick = 435,
        TwentyNodeBrickElastic = 436,
        EightNodeBrick_up = 437,
        variable_node_brick_8_to_27 = 438,
        EightNodeBrickElastic = 439,
        TwentySevenNodeBrickElastic = 440,
        beam_displacement_based = 441,
        beam_elastic = 442,
        beam_elastic_lumped_mass = 443,
        penalty = 444,
        penalty_for_applying_generalized_displacement = 445,
        beam_9dof_elastic = 446,
        FourNodeShellMITC4 = 447,
        FourNodeShellNewMITC4 = 448,
        ThreeNodeShellANDES = 449,
        FourNodeShellANDES = 450,
        truss = 451,
        contact = 452,
        EightNodeBrickLT = 453,
        TwentySevenNodeBrickLT = 454,
        ShearBeamLT = 455,
        porosity = 456,
        alpha = 457,
        rho_s = 458,
        rho_f = 459,
        k_x = 460,
        k_y = 461,
        k_z = 462,
        K_s = 463,
        K_f = 464,
        pressure = 465,
        cross_section = 466,
        shear_modulus = 467,
        torsion_Jx = 468,
        bending_Iz = 469,
        bending_Iy = 470,
        IntegrationRule = 471,
        number_of_integration_points = 472,
        stiffness = 473,
        normal_stiffness = 474,
        tangential_stiffness = 475,
        friction_ratio = 476,
        maximum_gap = 477,
        xz_plane_vector = 478,
        joint_1_offset = 479,
        joint_2_offset = 480,
        direction = 481,
        contact_plane_vector = 482,
        MembranePlateFiber = 483,
        ElasticMembranePlate = 484,
        thickness = 485,
        linear_elastic_isotropic_3d = 486,
        vonmises_perfectly_plastic = 487,
        vonmises_isotropic_hardening = 488,
        vonmises_linear_kinematic_hardening = 489,
        vonmises_linear_kinematic_hardening_accelerated = 490,
        vonmises_kinematic_hardening = 491,
        vonmises_perfectly_plastic_accelerated = 492,
        vonmises_isotropic_hardening_accelerated = 493,
        vonmises_kinematic_hardening_accelerated = 494,
        sanisand2008 = 495,
        camclay = 496,
        camclay_accelerated = 497,
        sanisand2004 = 498,
        druckerprager_isotropic_hardening = 499,
        druckerprager_isotropic_hardening_accelerated = 500,
        druckerprager_kinematic_hardening = 501,
        druckerprager_kinematic_hardening_accelerated = 502,
        druckerprager_perfectly_plastic = 503,
        druckerprager_perfectly_plastic_accelerated = 504,
        linear_elastic_crossanisotropic = 505,
        uniaxial_concrete02 = 506,
        uniaxial_elastic_1d = 507,
        uniaxial_steel01 = 508,
        uniaxial_steel02 = 509,
        pisano = 510,
        vonmises_perfectly_plastic_LT = 511,
        pisanoLT = 512,
        New_PisanoLT = 513,
        linear_elastic_isotropic_3d_LT = 514,
        mass_density = 515,
        elastic_modulus = 516,
        viscoelastic_modulus = 517,
        poisson_ratio = 518,
        von_mises_radius = 519,
        druckerprager_angle = 520,
        armstrong_frederick_ha = 521,
        armstrong_frederick_cr = 522,
        initial_confining_stress = 523,
        isotropic_hardening_rate = 524,
        kinematic_hardening_rate = 525,
        poisson_ratio_h_v = 526,
        poisson_ratio_h_h = 527,
        shear_modulus_h_v = 528,
        elastic_modulus_horizontal = 529,
        elastic_modulus_vertical = 530,
        pressure_reference_p0 = 531,
        initial_void_ratio = 532,
        initial_shear_modulus = 533,
        initial_bulk_modulus = 534,
        sanisand2008_Pat = 535,
        sanisand2008_k_c = 536,
        sanisand2008_K0 = 537,
        sanisand2008_alpha_cc = 538,
        sanisand2008_c = 539,
        sanisand2008_xi = 540,
        sanisand2008_lambda = 541,
        sanisand2008_ec_ref = 542,
        sanisand2008_m = 543,
        sanisand2008_h0 = 544,
        sanisand2008_ch = 545,
        sanisand2008_nb = 546,
        sanisand2008_A0 = 547,
        sanisand2008_nd = 548,
        sanisand2008_p_r = 549,
        sanisand2008_rho_c = 550,
        sanisand2008_theta_c = 551,
        sanisand2008_X = 552,
        sanisand2008_z_max = 553,
        sanisand2008_cz = 554,
        sanisand2008_p0 = 555,
        sanisand2008_p_in = 556,
        sanisand2008_G0 = 557,
        sanisand2004_K0 = 558,
        sanisand2004_Pat = 559,
        e0 = 560,
        sanisand2004_G0 = 561,
        sanisand2004_p_cut = 562,
        sanisand2004_Mc = 563,
        sanisand2004_c = 564,
        sanisand2004_lambda_c = 565,
        sanisand2004_xi = 566,
        sanisand2004_ec_ref = 567,
        sanisand2004_m = 568,
        sanisand2004_h0 = 569,
        sanisand2004_ch = 570,
        sanisand2004_nb = 571,
        sanisand2004_A0 = 572,
        sanisand2004_nd = 573,
        sanisand2004_z_max = 574,
        sanisand2004_cz = 575,
        reference_void_ratio = 576,
        critical_stress_ratio_M = 577,
        minimum_bulk_modulus = 578,
        initial_mean_pressure = 579,
        yield_strength = 580,
        strain_hardening_ratio = 581,
        compressive_strength = 582,
        strain_at_compressive_strength = 583,
        crushing_strength = 584,
        strain_at_crushing_strength = 585,
        tensile_strength = 586,
        tension_softening_stiffness = 587,
        M_in = 588,
        kd_in = 589,
        xi_in = 590,
        h_in = 591,
        m_in = 592,
        beta_min = 593,
        n_in = 594,
        a_in = 595,
        elastic_modulus_1atm = 596,
        eplcum_cr_in = 597,
        ax = 598,
        ay = 599,
        az = 600,
        verbose_level = 601,
        maximum_iterations = 602,
        tolerance = 603,
        beta = 604,
        gamma = 605,
        kappa = 606,
        lambda = 607,
        delta = 608,
        DOMAIN_ = 609,
        startTime = 610,
        endTime = 611,
        Period = 612,
        Phase = 613,
        Amplitude = 614,
        frequency = 615,
        MaxTime = 616,
        frequency1 = 617,
        frequency2 = 618,
        frequency3 = 619,
        frequency4 = 620,
        stiffness_to_use_opt = 621,
        DAMPING_RAYLEIGH = 622,
        DAMPING_CAUGHEY3 = 623,
        DAMPING_CAUGHEY4 = 624,
        a0 = 625,
        a1 = 626,
        stiffness_to_use = 627,
        a2 = 628,
        a3 = 629,
        a4 = 630,
        R0 = 631,
        cR1 = 632,
        cR2 = 633,
        MASS = 634,
        mx = 635,
        my = 636,
        mz = 637,
        Imx = 638,
        Imy = 639,
        Imz = 640,
        equaldof = 641,
        master = 642,
        slave = 643,
        dof_to_constrain = 644,
        of = 645,
        OUTPUT = 646,
        BINARY = 647,
        TEXT = 648,
        ENABLE = 649,
        DISABLE = 650
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
      yylast_ = 10696,     ///< Last index in yytable_.
      yynnts_ = 15,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 110, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 413  ///< Number of tokens.
    };


  };



} // yy
#line 850 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
