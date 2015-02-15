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
        SET = 318,
        ELEMENTNAME = 319,
        MATERIALNAME = 320,
        ACCELERATION_FIELD = 321,
        FIX = 322,
        FREE = 323,
        REMOVE = 324,
        DEFINE = 325,
        ALGORITHM = 326,
        ALGNAME = 327,
        CONVERGENCE_TEST = 328,
        TESTNAME = 329,
        SOLVER = 330,
        SOLVERNAME = 331,
        DYNAMICINTEGRATOR = 332,
        DYNAMICINTEGRATOR_HHT = 333,
        DYNAMICINTEGRATOR_NEWMARK = 334,
        STATICINTEGRATOR = 335,
        STATICINTEGRATOR_DISPLACEMENT = 336,
        SIMULATE = 337,
        STATIC = 338,
        DYNAMIC = 339,
        USING = 340,
        TRANSIENT = 341,
        EIGEN = 342,
        time_step = 343,
        number_of_modes = 344,
        VARIABLETRANSIENT = 345,
        maximum_time_step = 346,
        minimum_time_step = 347,
        number_of_iterations = 348,
        AT = 349,
        ALL = 350,
        AND = 351,
        WITH = 352,
        TEXTDOFS = 353,
        NEW = 354,
        TEXTNUMBER = 355,
        USE = 356,
        TO = 357,
        DOF = 358,
        TEXTWITH = 359,
        NODES = 360,
        FORCE = 361,
        INTEGRATIONPOINTS = 362,
        dof = 363,
        RESPONSE = 364,
        FILE = 365,
        FROM = 366,
        EVERY = 367,
        LEVEL = 368,
        LOADING = 369,
        STAGE = 370,
        STEPS = 371,
        TYPE = 372,
        DOFS = 373,
        FACTOR = 374,
        INCREMENT = 375,
        TH_GROUNDMOTION = 376,
        TH_LINEAR = 377,
        TH_PATH_SERIES = 378,
        TH_PATH_TIME_SERIES = 379,
        TH_CONSTANT = 380,
        TH_FROM_REACTIONS = 381,
        self_weight = 382,
        surface = 383,
        load_value = 384,
        scale_factor = 385,
        displacement_scale_unit = 386,
        velocity_scale_unit = 387,
        acceleration_scale_unit = 388,
        number_of_steps = 389,
        number_of_boundary_nodes = 390,
        number_of_exterior_nodes = 391,
        number_of_drm_elements = 392,
        element_file = 393,
        boundary_nodes_file = 394,
        exterior_nodes_file = 395,
        displacement_file = 396,
        acceleration_file = 397,
        velocity_file = 398,
        force_file = 399,
        series_file = 400,
        time_series_file = 401,
        MAGNITUDES = 402,
        MAGNITUDE = 403,
        strain_increment_size = 404,
        maximum_strain = 405,
        number_of_times_reaching_maximum_strain = 406,
        constitutive = 407,
        testing = 408,
        constant = 409,
        mean = 410,
        triaxial = 411,
        drained = 412,
        undrained = 413,
        simple = 414,
        shear = 415,
        number_of_subincrements = 416,
        maximum_number_of_iterations = 417,
        tolerance_1 = 418,
        tolerance_2 = 419,
        strain = 420,
        stress = 421,
        control = 422,
        Guass = 423,
        points = 424,
        Gauss = 425,
        each = 426,
        point = 427,
        single = 428,
        value = 429,
        EightNodeBrick = 430,
        TwentySevenNodeBrick = 431,
        EightNodeBrick_upU = 432,
        TwentyNodeBrick_uPU = 433,
        TwentyNodeBrick = 434,
        TwentyNodeBrickElastic = 435,
        EightNodeBrick_up = 436,
        variable_node_brick_8_to_27 = 437,
        EightNodeBrickElastic = 438,
        TwentySevenNodeBrickElastic = 439,
        beam_displacement_based = 440,
        beam_elastic = 441,
        beam_elastic_lumped_mass = 442,
        penalty = 443,
        penalty_for_applying_generalized_displacement = 444,
        beam_9dof_elastic = 445,
        FourNodeShellMITC4 = 446,
        FourNodeShellNewMITC4 = 447,
        ThreeNodeShellANDES = 448,
        FourNodeShellANDES = 449,
        truss = 450,
        contact = 451,
        EightNodeBrickLT = 452,
        TwentySevenNodeBrickLT = 453,
        ShearBeamLT = 454,
        porosity = 455,
        alpha = 456,
        rho_s = 457,
        rho_f = 458,
        k_x = 459,
        k_y = 460,
        k_z = 461,
        K_s = 462,
        K_f = 463,
        pressure = 464,
        cross_section = 465,
        shear_modulus = 466,
        torsion_Jx = 467,
        bending_Iz = 468,
        bending_Iy = 469,
        IntegrationRule = 470,
        number_of_integration_points = 471,
        stiffness = 472,
        normal_stiffness = 473,
        tangential_stiffness = 474,
        friction_ratio = 475,
        maximum_gap = 476,
        xz_plane_vector = 477,
        joint_1_offset = 478,
        joint_2_offset = 479,
        direction = 480,
        contact_plane_vector = 481,
        MembranePlateFiber = 482,
        ElasticMembranePlate = 483,
        thickness = 484,
        linear_elastic_isotropic_3d = 485,
        vonmises_perfectly_plastic = 486,
        vonmises_isotropic_hardening = 487,
        vonmises_linear_kinematic_hardening = 488,
        vonmises_linear_kinematic_hardening_accelerated = 489,
        vonmises_kinematic_hardening = 490,
        vonmises_perfectly_plastic_accelerated = 491,
        vonmises_isotropic_hardening_accelerated = 492,
        vonmises_kinematic_hardening_accelerated = 493,
        sanisand2008 = 494,
        camclay = 495,
        camclay_accelerated = 496,
        sanisand2004 = 497,
        druckerprager_isotropic_hardening = 498,
        druckerprager_isotropic_hardening_accelerated = 499,
        druckerprager_kinematic_hardening = 500,
        druckerprager_kinematic_hardening_accelerated = 501,
        druckerprager_perfectly_plastic = 502,
        druckerprager_perfectly_plastic_accelerated = 503,
        linear_elastic_crossanisotropic = 504,
        uniaxial_concrete02 = 505,
        uniaxial_elastic_1d = 506,
        uniaxial_steel01 = 507,
        uniaxial_steel02 = 508,
        pisano = 509,
        vonmises_perfectly_plastic_LT = 510,
        pisanoLT = 511,
        New_PisanoLT = 512,
        linear_elastic_isotropic_3d_LT = 513,
        mass_density = 514,
        elastic_modulus = 515,
        viscoelastic_modulus = 516,
        poisson_ratio = 517,
        von_mises_radius = 518,
        druckerprager_angle = 519,
        armstrong_frederick_ha = 520,
        armstrong_frederick_cr = 521,
        initial_confining_stress = 522,
        isotropic_hardening_rate = 523,
        kinematic_hardening_rate = 524,
        poisson_ratio_h_v = 525,
        poisson_ratio_h_h = 526,
        shear_modulus_h_v = 527,
        elastic_modulus_horizontal = 528,
        elastic_modulus_vertical = 529,
        pressure_reference_p0 = 530,
        initial_void_ratio = 531,
        initial_shear_modulus = 532,
        initial_bulk_modulus = 533,
        sanisand2008_Pat = 534,
        sanisand2008_k_c = 535,
        sanisand2008_K0 = 536,
        sanisand2008_alpha_cc = 537,
        sanisand2008_c = 538,
        sanisand2008_xi = 539,
        sanisand2008_lambda = 540,
        sanisand2008_ec_ref = 541,
        sanisand2008_m = 542,
        sanisand2008_h0 = 543,
        sanisand2008_ch = 544,
        sanisand2008_nb = 545,
        sanisand2008_A0 = 546,
        sanisand2008_nd = 547,
        sanisand2008_p_r = 548,
        sanisand2008_rho_c = 549,
        sanisand2008_theta_c = 550,
        sanisand2008_X = 551,
        sanisand2008_z_max = 552,
        sanisand2008_cz = 553,
        sanisand2008_p0 = 554,
        sanisand2008_p_in = 555,
        sanisand2008_G0 = 556,
        sanisand2004_K0 = 557,
        sanisand2004_Pat = 558,
        e0 = 559,
        sanisand2004_G0 = 560,
        sanisand2004_p_cut = 561,
        sanisand2004_Mc = 562,
        sanisand2004_c = 563,
        sanisand2004_lambda_c = 564,
        sanisand2004_xi = 565,
        sanisand2004_ec_ref = 566,
        sanisand2004_m = 567,
        sanisand2004_h0 = 568,
        sanisand2004_ch = 569,
        sanisand2004_nb = 570,
        sanisand2004_A0 = 571,
        sanisand2004_nd = 572,
        sanisand2004_z_max = 573,
        sanisand2004_cz = 574,
        reference_void_ratio = 575,
        critical_stress_ratio_M = 576,
        minimum_bulk_modulus = 577,
        initial_mean_pressure = 578,
        yield_strength = 579,
        strain_hardening_ratio = 580,
        compressive_strength = 581,
        strain_at_compressive_strength = 582,
        crushing_strength = 583,
        strain_at_crushing_strength = 584,
        tensile_strength = 585,
        tension_softening_stiffness = 586,
        M_in = 587,
        kd_in = 588,
        xi_in = 589,
        h_in = 590,
        m_in = 591,
        beta_min = 592,
        n_in = 593,
        a_in = 594,
        elastic_modulus_1atm = 595,
        eplcum_cr_in = 596,
        ax = 597,
        ay = 598,
        az = 599,
        verbose_level = 600,
        maximum_iterations = 601,
        tolerance = 602,
        beta = 603,
        gamma = 604,
        kappa = 605,
        lambda = 606,
        delta = 607,
        DOMAIN_ = 608,
        startTime = 609,
        endTime = 610,
        Period = 611,
        Phase = 612,
        Amplitude = 613,
        frequency = 614,
        MaxTime = 615,
        frequency1 = 616,
        frequency2 = 617,
        frequency3 = 618,
        frequency4 = 619,
        stiffness_to_use_opt = 620,
        DAMPING_RAYLEIGH = 621,
        DAMPING_CAUGHEY3 = 622,
        DAMPING_CAUGHEY4 = 623,
        a0 = 624,
        a1 = 625,
        stiffness_to_use = 626,
        a2 = 627,
        a3 = 628,
        a4 = 629,
        R0 = 630,
        cR1 = 631,
        cR2 = 632,
        MASS = 633,
        mx = 634,
        my = 635,
        mz = 636,
        Imx = 637,
        Imy = 638,
        Imz = 639,
        equaldof = 640,
        master = 641,
        slave = 642,
        dof_to_constrain = 643,
        of = 644,
        OUTPUT = 645,
        BINARY = 646,
        TEXT = 647,
        ENABLE = 648,
        DISABLE = 649,
        COMPRESSION = 650
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
      yylast_ = 10329,     ///< Last index in yytable_.
      yynnts_ = 15,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 114, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 413  ///< Number of tokens.
    };


  };



} // yy
#line 850 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
