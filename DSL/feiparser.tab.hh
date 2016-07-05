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
    #line 143 "feiparser.yy" // lalr1.cc:372

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
        VECTOR = 261,
        EOL = 262,
        ISUNITTYPE = 263,
        FUNCTION = 264,
        IF = 265,
        THEN = 266,
        ELSE = 267,
        WHILE = 268,
        DO = 269,
        LET = 270,
        EXITSAFEMODE = 271,
        CONTINUE = 272,
        BYE = 273,
        IN = 274,
        REQUIRE = 275,
        PLUSEQUAL = 276,
        MINUSEQUAL = 277,
        TIMESEQUAL = 278,
        DIVIDEEQUAL = 279,
        MODULUSEQUAL = 280,
        POWEQUAL = 281,
        NUMBER_OF_NODES = 282,
        NUMBER_OF_ELEMENTS = 283,
        CURRENT_TIME = 284,
        NUMBER_OF_SP_CONSTRAINTS = 285,
        NUMBER_OF_MP_CONSTRAINTS = 286,
        NUMBER_OF_LOADS = 287,
        IS_PARALLEL = 288,
        UMINUS = 289,
        CMP = 290,
        CMD_print = 291,
        CMD_help = 292,
        CMD_whos = 293,
        CMD_check = 294,
        CMD_save = 295,
        CMD_restore = 296,
        MODEL = 297,
        NAME = 298,
        RESTART = 299,
        MESH = 300,
        ADD = 301,
        NODE = 302,
        ELEMENT = 303,
        ELEMENTS = 304,
        MATERIAL = 305,
        LOAD = 306,
        TIMEHISTORY = 307,
        IMPOSEDMOTION = 308,
        UNIFORMACCELERATION = 309,
        DAMPING = 310,
        DAMPINGTYPE = 311,
        CONSTRAINT = 312,
        DRM = 313,
        SECTION = 314,
        LOADPATTERN = 315,
        PENALTYDISPLACEMENT = 316,
        LOADVALUE = 317,
        SET = 318,
        REACTION = 319,
        FORCES = 320,
        ELEMENTNAME = 321,
        MATERIALNAME = 322,
        ACCELERATION_FIELD = 323,
        FIX = 324,
        FREE = 325,
        REMOVE = 326,
        DEFINE = 327,
        ALGORITHM = 328,
        ALGNAME = 329,
        CONSTITUTIVE_ALGNAME = 330,
        CONVERGENCE_TEST = 331,
        TESTNAME = 332,
        SOLVER = 333,
        SOLVERNAME = 334,
        CONSTITUTIVE = 335,
        INTEGRATION = 336,
        DYNAMICINTEGRATOR = 337,
        DYNAMICINTEGRATOR_HHT = 338,
        DYNAMICINTEGRATOR_NEWMARK = 339,
        STATICINTEGRATOR = 340,
        STATICINTEGRATOR_DISPLACEMENT = 341,
        SIMULATE = 342,
        COMPUTE = 343,
        STATIC = 344,
        DYNAMIC = 345,
        USING = 346,
        TRANSIENT = 347,
        EIGEN = 348,
        time_step = 349,
        number_of_modes = 350,
        VARIABLETRANSIENT = 351,
        maximum_time_step = 352,
        minimum_time_step = 353,
        number_of_iterations = 354,
        RUNTEST = 355,
        AT = 356,
        ALL = 357,
        AND = 358,
        WITH = 359,
        TEXTDOFS = 360,
        NEW = 361,
        TEXTNUMBER = 362,
        USE = 363,
        TO = 364,
        DOF = 365,
        TEXTWITH = 366,
        NODES = 367,
        FORCE = 368,
        INTEGRATIONPOINTS = 369,
        dof = 370,
        RESPONSE = 371,
        FILE = 372,
        FROM = 373,
        EVERY = 374,
        LEVEL = 375,
        LOADING = 376,
        STAGE = 377,
        STEPS = 378,
        TYPE = 379,
        DOFS = 380,
        FACTOR = 381,
        INCREMENT = 382,
        TH_GROUNDMOTION = 383,
        TH_LINEAR = 384,
        TH_PATH_SERIES = 385,
        TH_PATH_TIME_SERIES = 386,
        TH_CONSTANT = 387,
        TH_FROM_REACTIONS = 388,
        self_weight = 389,
        surface = 390,
        load_value = 391,
        scale_factor = 392,
        displacement_scale_unit = 393,
        velocity_scale_unit = 394,
        acceleration_scale_unit = 395,
        number_of_steps = 396,
        number_of_boundary_nodes = 397,
        number_of_exterior_nodes = 398,
        number_of_drm_elements = 399,
        element_file = 400,
        boundary_nodes_file = 401,
        exterior_nodes_file = 402,
        displacement_file = 403,
        acceleration_file = 404,
        velocity_file = 405,
        force_file = 406,
        hdf5_file = 407,
        series_file = 408,
        time_series_file = 409,
        MAGNITUDES = 410,
        MAGNITUDE = 411,
        initial_velocity = 412,
        strain_increment_size = 413,
        maximum_strain = 414,
        number_of_times_reaching_maximum_strain = 415,
        constitutive = 416,
        testing = 417,
        constant = 418,
        mean = 419,
        triaxial = 420,
        drained = 421,
        undrained = 422,
        simple = 423,
        shear = 424,
        number_of_subincrements = 425,
        maximum_number_of_iterations = 426,
        tolerance_1 = 427,
        tolerance_2 = 428,
        strain = 429,
        stress = 430,
        control = 431,
        Guass = 432,
        points = 433,
        Gauss = 434,
        each = 435,
        point = 436,
        single = 437,
        value = 438,
        initial_normal_stiffness = 439,
        stiffning_rate = 440,
        EightNodeBrick = 441,
        TwentySevenNodeBrick = 442,
        EightNodeBrick_upU = 443,
        TwentyNodeBrick_uPU = 444,
        TwentyNodeBrick = 445,
        TwentyNodeBrickElastic = 446,
        EightNodeBrick_up = 447,
        variable_node_brick_8_to_27 = 448,
        EightNodeBrickElastic = 449,
        TwentySevenNodeBrickElastic = 450,
        beam_displacement_based = 451,
        BeamColumnDispFiber3d = 452,
        beam_elastic = 453,
        beam_elastic_lumped_mass = 454,
        beam_9dof_elastic = 455,
        FourNodeShellMITC4 = 456,
        FourNodeShellNewMITC4 = 457,
        ThreeNodeShellANDES = 458,
        FourNodeShellANDES = 459,
        truss = 460,
        contact = 461,
        HardContact = 462,
        FrictionalPenaltyContact = 463,
        SoftContact = 464,
        EightNodeBrickLT = 465,
        EightNodeBrickLTNoOutput = 466,
        TwentyNodeBrickLT = 467,
        TwentySevenNodeBrickLT = 468,
        ShearBeamLT = 469,
        porosity = 470,
        alpha = 471,
        rho_s = 472,
        rho_f = 473,
        k_x = 474,
        k_y = 475,
        k_z = 476,
        K_s = 477,
        K_f = 478,
        pressure = 479,
        cross_section = 480,
        shear_modulus = 481,
        torsion_Jx = 482,
        bending_Iz = 483,
        bending_Iy = 484,
        IntegrationRule = 485,
        stiffness = 486,
        normal_stiffness = 487,
        tangential_stiffness = 488,
        normal_damping = 489,
        tangential_damping = 490,
        friction_ratio = 491,
        maximum_gap = 492,
        xz_plane_vector = 493,
        joint_1_offset = 494,
        joint_2_offset = 495,
        direction = 496,
        contact_plane_vector = 497,
        power = 498,
        linear = 499,
        exponential = 500,
        factorial = 501,
        inverse = 502,
        MembranePlateFiber = 503,
        ElasticMembranePlate = 504,
        elastic3d = 505,
        FIBER = 506,
        FiberSection = 507,
        Section = 508,
        fiber_cross_section = 509,
        fiber_location_Y = 510,
        fiber_location_Z = 511,
        fiber_location = 512,
        TorsionConstant_GJ = 513,
        thickness = 514,
        integration_rule = 515,
        section_number = 516,
        number_of_integration_points = 517,
        NDMaterialLT = 518,
        linear_elastic_isotropic_3d = 519,
        linear_elastic_isotropic_3d_LT = 520,
        sanisand2008 = 521,
        camclay = 522,
        camclay_accelerated = 523,
        sanisand2004 = 524,
        linear_elastic_crossanisotropic = 525,
        uniaxial_concrete02 = 526,
        uniaxial_elastic_1d = 527,
        uniaxial_steel01 = 528,
        uniaxial_steel02 = 529,
        pisano = 530,
        PisanoLT = 531,
        VonMisesLT = 532,
        DruckerPragerLT = 533,
        DruckerPragerVonMisesLT = 534,
        DruckerPragerArmstrongFrederickLT = 535,
        mass_density = 536,
        elastic_modulus = 537,
        viscoelastic_modulus = 538,
        poisson_ratio = 539,
        von_mises_radius = 540,
        druckerprager_angle = 541,
        druckerprager_k = 542,
        armstrong_frederick_ha = 543,
        armstrong_frederick_cr = 544,
        initial_confining_stress = 545,
        isotropic_hardening_rate = 546,
        kinematic_hardening_rate = 547,
        poisson_ratio_h_v = 548,
        poisson_ratio_h_h = 549,
        shear_modulus_h_v = 550,
        elastic_modulus_horizontal = 551,
        elastic_modulus_vertical = 552,
        pressure_reference_p0 = 553,
        initial_void_ratio = 554,
        initial_shear_modulus = 555,
        initial_bulk_modulus = 556,
        sanisand2008_Pat = 557,
        sanisand2008_k_c = 558,
        sanisand2008_K0 = 559,
        sanisand2008_alpha_cc = 560,
        sanisand2008_c = 561,
        sanisand2008_xi = 562,
        sanisand2008_lambda = 563,
        sanisand2008_ec_ref = 564,
        sanisand2008_m = 565,
        sanisand2008_h0 = 566,
        sanisand2008_ch = 567,
        sanisand2008_nb = 568,
        sanisand2008_A0 = 569,
        sanisand2008_nd = 570,
        sanisand2008_p_r = 571,
        sanisand2008_rho_c = 572,
        sanisand2008_theta_c = 573,
        sanisand2008_X = 574,
        sanisand2008_z_max = 575,
        sanisand2008_cz = 576,
        sanisand2008_p0 = 577,
        sanisand2008_p_in = 578,
        sanisand2008_G0 = 579,
        sanisand2004_K0 = 580,
        sanisand2004_Pat = 581,
        e0 = 582,
        sanisand2004_G0 = 583,
        sanisand2004_p_cut = 584,
        sanisand2004_Mc = 585,
        sanisand2004_c = 586,
        sanisand2004_lambda_c = 587,
        sanisand2004_xi = 588,
        sanisand2004_ec_ref = 589,
        sanisand2004_m = 590,
        sanisand2004_h0 = 591,
        sanisand2004_ch = 592,
        sanisand2004_nb = 593,
        sanisand2004_A0 = 594,
        sanisand2004_nd = 595,
        sanisand2004_z_max = 596,
        sanisand2004_cz = 597,
        reference_void_ratio = 598,
        critical_stress_ratio_M = 599,
        minimum_bulk_modulus = 600,
        initial_mean_pressure = 601,
        yield_strength = 602,
        strain_hardening_ratio = 603,
        compressive_strength = 604,
        strain_at_compressive_strength = 605,
        crushing_strength = 606,
        strain_at_crushing_strength = 607,
        tensile_strength = 608,
        tension_softening_stiffness = 609,
        M_in = 610,
        kd_in = 611,
        xi_in = 612,
        h_in = 613,
        m_in = 614,
        beta_min = 615,
        n_in = 616,
        a_in = 617,
        elastic_modulus_1atm = 618,
        eplcum_cr_in = 619,
        ax = 620,
        ay = 621,
        az = 622,
        verbose_level = 623,
        maximum_iterations = 624,
        tolerance = 625,
        yield_function_relative_tolerance = 626,
        stress_relative_tolerance = 627,
        beta = 628,
        gamma = 629,
        kappa = 630,
        lambda = 631,
        delta = 632,
        DOMAIN_ = 633,
        startTime = 634,
        endTime = 635,
        Period = 636,
        Phase = 637,
        Amplitude = 638,
        frequency = 639,
        MaxTime = 640,
        frequency1 = 641,
        frequency2 = 642,
        frequency3 = 643,
        frequency4 = 644,
        stiffness_to_use_opt = 645,
        DAMPING_RAYLEIGH = 646,
        DAMPING_CAUGHEY3 = 647,
        DAMPING_CAUGHEY4 = 648,
        a0 = 649,
        a1 = 650,
        stiffness_to_use = 651,
        a2 = 652,
        a3 = 653,
        a4 = 654,
        R0 = 655,
        cR1 = 656,
        cR2 = 657,
        MASS = 658,
        mx = 659,
        my = 660,
        mz = 661,
        Imx = 662,
        Imy = 663,
        Imz = 664,
        equaldof = 665,
        master = 666,
        slave = 667,
        dof_to_constrain = 668,
        of = 669,
        OUTPUT = 670,
        BINARY = 671,
        TEXT = 672,
        ENABLE = 673,
        DISABLE = 674,
        COMPRESSION = 675
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
      yylast_ = 9150,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 119, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 438  ///< Number of tokens.
    };


  };



} // yy
#line 875 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
