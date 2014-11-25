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
        EVERY = 366,
        LOADING = 367,
        STAGE = 368,
        STEPS = 369,
        TYPE = 370,
        DOFS = 371,
        FACTOR = 372,
        INCREMENT = 373,
        TH_GROUNDMOTION = 374,
        TH_LINEAR = 375,
        TH_PATH_SERIES = 376,
        TH_PATH_TIME_SERIES = 377,
        TH_CONSTANT = 378,
        TH_FROM_REACTIONS = 379,
        self_weight = 380,
        surface = 381,
        load_value = 382,
        scale_factor = 383,
        displacement_scale_unit = 384,
        velocity_scale_unit = 385,
        acceleration_scale_unit = 386,
        number_of_steps = 387,
        number_of_drm_nodes = 388,
        number_of_drm_elements = 389,
        x_max = 390,
        x_min = 391,
        y_max = 392,
        y_min = 393,
        z_max = 394,
        z_min = 395,
        element_file = 396,
        nodes_file = 397,
        displacement_file = 398,
        acceleration_file = 399,
        velocity_file = 400,
        force_file = 401,
        series_file = 402,
        time_series_file = 403,
        MAGNITUDES = 404,
        MAGNITUDE = 405,
        strain_increment_size = 406,
        maximum_strain = 407,
        number_of_times_reaching_maximum_strain = 408,
        constitutive = 409,
        testing = 410,
        constant = 411,
        mean = 412,
        triaxial = 413,
        drained = 414,
        undrained = 415,
        simple = 416,
        shear = 417,
        number_of_subincrements = 418,
        maximum_number_of_iterations = 419,
        tolerance_1 = 420,
        tolerance_2 = 421,
        strain = 422,
        stress = 423,
        control = 424,
        Guass = 425,
        points = 426,
        Gauss = 427,
        each = 428,
        point = 429,
        single = 430,
        value = 431,
        EightNodeBrick = 432,
        TwentySevenNodeBrick = 433,
        EightNodeBrick_upU = 434,
        TwentyNodeBrick_uPU = 435,
        TwentyNodeBrick = 436,
        TwentyNodeBrickElastic = 437,
        EightNodeBrick_up = 438,
        variable_node_brick_8_to_27 = 439,
        EightNodeBrickElastic = 440,
        TwentySevenNodeBrickElastic = 441,
        beam_displacement_based = 442,
        beam_elastic = 443,
        beam_elastic_lumped_mass = 444,
        penalty = 445,
        penalty_for_applying_generalized_displacement = 446,
        beam_9dof_elastic = 447,
        FourNodeShellMITC4 = 448,
        FourNodeShellNewMITC4 = 449,
        ThreeNodeShellANDES = 450,
        FourNodeShellANDES = 451,
        truss = 452,
        contact = 453,
        EightNodeBrickLT = 454,
        TwentySevenNodeBrickLT = 455,
        ShearBeamLT = 456,
        porosity = 457,
        alpha = 458,
        rho_s = 459,
        rho_f = 460,
        k_x = 461,
        k_y = 462,
        k_z = 463,
        K_s = 464,
        K_f = 465,
        pressure = 466,
        cross_section = 467,
        shear_modulus = 468,
        torsion_Jx = 469,
        bending_Iz = 470,
        bending_Iy = 471,
        IntegrationRule = 472,
        number_of_integration_points = 473,
        stiffness = 474,
        normal_stiffness = 475,
        tangential_stiffness = 476,
        friction_ratio = 477,
        maximum_gap = 478,
        xz_plane_vector = 479,
        joint_1_offset = 480,
        joint_2_offset = 481,
        direction = 482,
        contact_plane_vector = 483,
        MembranePlateFiber = 484,
        ElasticMembranePlate = 485,
        thickness = 486,
        linear_elastic_isotropic_3d = 487,
        vonmises_perfectly_plastic = 488,
        vonmises_isotropic_hardening = 489,
        vonmises_linear_kinematic_hardening = 490,
        vonmises_linear_kinematic_hardening_accelerated = 491,
        vonmises_kinematic_hardening = 492,
        vonmises_perfectly_plastic_accelerated = 493,
        vonmises_isotropic_hardening_accelerated = 494,
        vonmises_kinematic_hardening_accelerated = 495,
        sanisand2008 = 496,
        camclay = 497,
        camclay_accelerated = 498,
        sanisand2004 = 499,
        druckerprager_isotropic_hardening = 500,
        druckerprager_isotropic_hardening_accelerated = 501,
        druckerprager_kinematic_hardening = 502,
        druckerprager_kinematic_hardening_accelerated = 503,
        druckerprager_perfectly_plastic = 504,
        druckerprager_perfectly_plastic_accelerated = 505,
        linear_elastic_crossanisotropic = 506,
        uniaxial_concrete02 = 507,
        uniaxial_elastic_1d = 508,
        uniaxial_steel01 = 509,
        uniaxial_steel02 = 510,
        pisano = 511,
        vonmises_perfectly_plastic_LT = 512,
        pisanoLT = 513,
        New_PisanoLT = 514,
        linear_elastic_isotropic_3d_LT = 515,
        mass_density = 516,
        elastic_modulus = 517,
        viscoelastic_modulus = 518,
        poisson_ratio = 519,
        von_mises_radius = 520,
        druckerprager_angle = 521,
        armstrong_frederick_ha = 522,
        armstrong_frederick_cr = 523,
        initial_confining_stress = 524,
        isotropic_hardening_rate = 525,
        kinematic_hardening_rate = 526,
        poisson_ratio_h_v = 527,
        poisson_ratio_h_h = 528,
        shear_modulus_h_v = 529,
        elastic_modulus_horizontal = 530,
        elastic_modulus_vertical = 531,
        pressure_reference_p0 = 532,
        initial_void_ratio = 533,
        initial_shear_modulus = 534,
        initial_bulk_modulus = 535,
        sanisand2008_Pat = 536,
        sanisand2008_k_c = 537,
        sanisand2008_K0 = 538,
        sanisand2008_alpha_cc = 539,
        sanisand2008_c = 540,
        sanisand2008_xi = 541,
        sanisand2008_lambda = 542,
        sanisand2008_ec_ref = 543,
        sanisand2008_m = 544,
        sanisand2008_h0 = 545,
        sanisand2008_ch = 546,
        sanisand2008_nb = 547,
        sanisand2008_A0 = 548,
        sanisand2008_nd = 549,
        sanisand2008_p_r = 550,
        sanisand2008_rho_c = 551,
        sanisand2008_theta_c = 552,
        sanisand2008_X = 553,
        sanisand2008_z_max = 554,
        sanisand2008_cz = 555,
        sanisand2008_p0 = 556,
        sanisand2008_p_in = 557,
        sanisand2008_G0 = 558,
        sanisand2004_K0 = 559,
        sanisand2004_Pat = 560,
        e0 = 561,
        sanisand2004_G0 = 562,
        sanisand2004_p_cut = 563,
        sanisand2004_Mc = 564,
        sanisand2004_c = 565,
        sanisand2004_lambda_c = 566,
        sanisand2004_xi = 567,
        sanisand2004_ec_ref = 568,
        sanisand2004_m = 569,
        sanisand2004_h0 = 570,
        sanisand2004_ch = 571,
        sanisand2004_nb = 572,
        sanisand2004_A0 = 573,
        sanisand2004_nd = 574,
        sanisand2004_z_max = 575,
        sanisand2004_cz = 576,
        reference_void_ratio = 577,
        critical_stress_ratio_M = 578,
        minimum_bulk_modulus = 579,
        initial_mean_pressure = 580,
        yield_strength = 581,
        strain_hardening_ratio = 582,
        compressive_strength = 583,
        strain_at_compressive_strength = 584,
        crushing_strength = 585,
        strain_at_crushing_strength = 586,
        tensile_strength = 587,
        tension_softening_stiffness = 588,
        M_in = 589,
        kd_in = 590,
        xi_in = 591,
        h_in = 592,
        m_in = 593,
        beta_min = 594,
        n_in = 595,
        a_in = 596,
        elastic_modulus_1atm = 597,
        eplcum_cr_in = 598,
        ax = 599,
        ay = 600,
        az = 601,
        verbose_level = 602,
        maximum_iterations = 603,
        tolerance = 604,
        beta = 605,
        gamma = 606,
        kappa = 607,
        lambda = 608,
        delta = 609,
        DOMAIN_ = 610,
        startTime = 611,
        endTime = 612,
        Period = 613,
        Phase = 614,
        Amplitude = 615,
        frequency = 616,
        MaxTime = 617,
        frequency1 = 618,
        frequency2 = 619,
        frequency3 = 620,
        frequency4 = 621,
        stiffness_to_use_opt = 622,
        DAMPING_RAYLEIGH = 623,
        DAMPING_CAUGHEY3 = 624,
        DAMPING_CAUGHEY4 = 625,
        a0 = 626,
        a1 = 627,
        stiffness_to_use = 628,
        a2 = 629,
        a3 = 630,
        a4 = 631,
        R0 = 632,
        cR1 = 633,
        cR2 = 634,
        MASS = 635,
        mx = 636,
        my = 637,
        mz = 638,
        Imx = 639,
        Imy = 640,
        Imz = 641,
        equaldof = 642,
        master = 643,
        slave = 644,
        dof_to_constrain = 645,
        of = 646,
        OUTPUT = 647,
        BINARY = 648,
        TEXT = 649,
        ENABLE = 650,
        DISABLE = 651
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
      yylast_ = 10743,     ///< Last index in yytable_.
      yynnts_ = 15,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 112, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 414  ///< Number of tokens.
    };


  };



} // yy
#line 851 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
