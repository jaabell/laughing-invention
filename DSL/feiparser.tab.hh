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
    #line 112 "feiparser.yy" // lalr1.cc:372

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
        SECTION = 312,
        LOADPATTERN = 313,
        PENALTYDISPLACEMENT = 314,
        LOADVALUE = 315,
        SET = 316,
        REACTION = 317,
        FORCES = 318,
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
        COMPUTE = 338,
        STATIC = 339,
        DYNAMIC = 340,
        USING = 341,
        TRANSIENT = 342,
        EIGEN = 343,
        time_step = 344,
        number_of_modes = 345,
        VARIABLETRANSIENT = 346,
        maximum_time_step = 347,
        minimum_time_step = 348,
        number_of_iterations = 349,
        AT = 350,
        ALL = 351,
        AND = 352,
        WITH = 353,
        TEXTDOFS = 354,
        NEW = 355,
        TEXTNUMBER = 356,
        USE = 357,
        TO = 358,
        DOF = 359,
        TEXTWITH = 360,
        NODES = 361,
        FORCE = 362,
        INTEGRATIONPOINTS = 363,
        dof = 364,
        RESPONSE = 365,
        FILE = 366,
        FROM = 367,
        EVERY = 368,
        LEVEL = 369,
        LOADING = 370,
        STAGE = 371,
        STEPS = 372,
        TYPE = 373,
        DOFS = 374,
        FACTOR = 375,
        INCREMENT = 376,
        TH_GROUNDMOTION = 377,
        TH_LINEAR = 378,
        TH_PATH_SERIES = 379,
        TH_PATH_TIME_SERIES = 380,
        TH_CONSTANT = 381,
        TH_FROM_REACTIONS = 382,
        self_weight = 383,
        surface = 384,
        load_value = 385,
        scale_factor = 386,
        displacement_scale_unit = 387,
        velocity_scale_unit = 388,
        acceleration_scale_unit = 389,
        number_of_steps = 390,
        number_of_boundary_nodes = 391,
        number_of_exterior_nodes = 392,
        number_of_drm_elements = 393,
        element_file = 394,
        boundary_nodes_file = 395,
        exterior_nodes_file = 396,
        displacement_file = 397,
        acceleration_file = 398,
        velocity_file = 399,
        force_file = 400,
        hdf5_file = 401,
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
        FrictionalPenaltyContact = 454,
        EightNodeBrickLT = 455,
        TwentySevenNodeBrickLT = 456,
        ShearBeamLT = 457,
        porosity = 458,
        alpha = 459,
        rho_s = 460,
        rho_f = 461,
        k_x = 462,
        k_y = 463,
        k_z = 464,
        K_s = 465,
        K_f = 466,
        pressure = 467,
        cross_section = 468,
        shear_modulus = 469,
        torsion_Jx = 470,
        bending_Iz = 471,
        bending_Iy = 472,
        IntegrationRule = 473,
        number_of_integration_points = 474,
        stiffness = 475,
        normal_stiffness = 476,
        tangential_stiffness = 477,
        normal_damping = 478,
        tangential_damping = 479,
        friction_ratio = 480,
        maximum_gap = 481,
        xz_plane_vector = 482,
        joint_1_offset = 483,
        joint_2_offset = 484,
        direction = 485,
        contact_plane_vector = 486,
        MembranePlateFiber = 487,
        ElasticMembranePlate = 488,
        thickness = 489,
        linear_elastic_isotropic_3d = 490,
        vonmises_perfectly_plastic = 491,
        vonmises_isotropic_hardening = 492,
        vonmises_linear_kinematic_hardening = 493,
        vonmises_linear_kinematic_hardening_accelerated = 494,
        vonmises_kinematic_hardening = 495,
        vonmises_perfectly_plastic_accelerated = 496,
        vonmises_isotropic_hardening_accelerated = 497,
        vonmises_kinematic_hardening_accelerated = 498,
        sanisand2008 = 499,
        camclay = 500,
        camclay_accelerated = 501,
        sanisand2004 = 502,
        druckerprager_isotropic_hardening = 503,
        druckerprager_isotropic_hardening_accelerated = 504,
        druckerprager_kinematic_hardening = 505,
        druckerprager_kinematic_hardening_accelerated = 506,
        druckerprager_perfectly_plastic = 507,
        druckerprager_perfectly_plastic_accelerated = 508,
        linear_elastic_crossanisotropic = 509,
        uniaxial_concrete02 = 510,
        uniaxial_elastic_1d = 511,
        uniaxial_steel01 = 512,
        uniaxial_steel02 = 513,
        pisano = 514,
        vonmises_perfectly_plastic_LT = 515,
        pisanoLT = 516,
        New_PisanoLT = 517,
        linear_elastic_isotropic_3d_LT = 518,
        VonMisesLT = 519,
        DruckerPragerLT = 520,
        mass_density = 521,
        elastic_modulus = 522,
        viscoelastic_modulus = 523,
        poisson_ratio = 524,
        von_mises_radius = 525,
        druckerprager_angle = 526,
        armstrong_frederick_ha = 527,
        armstrong_frederick_cr = 528,
        initial_confining_stress = 529,
        isotropic_hardening_rate = 530,
        kinematic_hardening_rate = 531,
        poisson_ratio_h_v = 532,
        poisson_ratio_h_h = 533,
        shear_modulus_h_v = 534,
        elastic_modulus_horizontal = 535,
        elastic_modulus_vertical = 536,
        pressure_reference_p0 = 537,
        initial_void_ratio = 538,
        initial_shear_modulus = 539,
        initial_bulk_modulus = 540,
        sanisand2008_Pat = 541,
        sanisand2008_k_c = 542,
        sanisand2008_K0 = 543,
        sanisand2008_alpha_cc = 544,
        sanisand2008_c = 545,
        sanisand2008_xi = 546,
        sanisand2008_lambda = 547,
        sanisand2008_ec_ref = 548,
        sanisand2008_m = 549,
        sanisand2008_h0 = 550,
        sanisand2008_ch = 551,
        sanisand2008_nb = 552,
        sanisand2008_A0 = 553,
        sanisand2008_nd = 554,
        sanisand2008_p_r = 555,
        sanisand2008_rho_c = 556,
        sanisand2008_theta_c = 557,
        sanisand2008_X = 558,
        sanisand2008_z_max = 559,
        sanisand2008_cz = 560,
        sanisand2008_p0 = 561,
        sanisand2008_p_in = 562,
        sanisand2008_G0 = 563,
        sanisand2004_K0 = 564,
        sanisand2004_Pat = 565,
        e0 = 566,
        sanisand2004_G0 = 567,
        sanisand2004_p_cut = 568,
        sanisand2004_Mc = 569,
        sanisand2004_c = 570,
        sanisand2004_lambda_c = 571,
        sanisand2004_xi = 572,
        sanisand2004_ec_ref = 573,
        sanisand2004_m = 574,
        sanisand2004_h0 = 575,
        sanisand2004_ch = 576,
        sanisand2004_nb = 577,
        sanisand2004_A0 = 578,
        sanisand2004_nd = 579,
        sanisand2004_z_max = 580,
        sanisand2004_cz = 581,
        reference_void_ratio = 582,
        critical_stress_ratio_M = 583,
        minimum_bulk_modulus = 584,
        initial_mean_pressure = 585,
        yield_strength = 586,
        strain_hardening_ratio = 587,
        compressive_strength = 588,
        strain_at_compressive_strength = 589,
        crushing_strength = 590,
        strain_at_crushing_strength = 591,
        tensile_strength = 592,
        tension_softening_stiffness = 593,
        M_in = 594,
        kd_in = 595,
        xi_in = 596,
        h_in = 597,
        m_in = 598,
        beta_min = 599,
        n_in = 600,
        a_in = 601,
        elastic_modulus_1atm = 602,
        eplcum_cr_in = 603,
        ax = 604,
        ay = 605,
        az = 606,
        verbose_level = 607,
        maximum_iterations = 608,
        tolerance = 609,
        beta = 610,
        gamma = 611,
        kappa = 612,
        lambda = 613,
        delta = 614,
        DOMAIN_ = 615,
        startTime = 616,
        endTime = 617,
        Period = 618,
        Phase = 619,
        Amplitude = 620,
        frequency = 621,
        MaxTime = 622,
        frequency1 = 623,
        frequency2 = 624,
        frequency3 = 625,
        frequency4 = 626,
        stiffness_to_use_opt = 627,
        DAMPING_RAYLEIGH = 628,
        DAMPING_CAUGHEY3 = 629,
        DAMPING_CAUGHEY4 = 630,
        a0 = 631,
        a1 = 632,
        stiffness_to_use = 633,
        a2 = 634,
        a3 = 635,
        a4 = 636,
        R0 = 637,
        cR1 = 638,
        cR2 = 639,
        MASS = 640,
        mx = 641,
        my = 642,
        mz = 643,
        Imx = 644,
        Imy = 645,
        Imz = 646,
        equaldof = 647,
        master = 648,
        slave = 649,
        dof_to_constrain = 650,
        of = 651,
        OUTPUT = 652,
        BINARY = 653,
        TEXT = 654,
        ENABLE = 655,
        DISABLE = 656,
        COMPRESSION = 657
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
      yylast_ = 10478,     ///< Last index in yytable_.
      yynnts_ = 15,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 115, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 420  ///< Number of tokens.
    };


  };



} // yy
#line 857 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
