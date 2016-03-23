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
        AT = 355,
        ALL = 356,
        AND = 357,
        WITH = 358,
        TEXTDOFS = 359,
        NEW = 360,
        TEXTNUMBER = 361,
        USE = 362,
        TO = 363,
        DOF = 364,
        TEXTWITH = 365,
        NODES = 366,
        FORCE = 367,
        INTEGRATIONPOINTS = 368,
        dof = 369,
        RESPONSE = 370,
        FILE = 371,
        FROM = 372,
        EVERY = 373,
        LEVEL = 374,
        LOADING = 375,
        STAGE = 376,
        STEPS = 377,
        TYPE = 378,
        DOFS = 379,
        FACTOR = 380,
        INCREMENT = 381,
        TH_GROUNDMOTION = 382,
        TH_LINEAR = 383,
        TH_PATH_SERIES = 384,
        TH_PATH_TIME_SERIES = 385,
        TH_CONSTANT = 386,
        TH_FROM_REACTIONS = 387,
        self_weight = 388,
        surface = 389,
        load_value = 390,
        scale_factor = 391,
        displacement_scale_unit = 392,
        velocity_scale_unit = 393,
        acceleration_scale_unit = 394,
        number_of_steps = 395,
        number_of_boundary_nodes = 396,
        number_of_exterior_nodes = 397,
        number_of_drm_elements = 398,
        element_file = 399,
        boundary_nodes_file = 400,
        exterior_nodes_file = 401,
        displacement_file = 402,
        acceleration_file = 403,
        velocity_file = 404,
        force_file = 405,
        hdf5_file = 406,
        series_file = 407,
        time_series_file = 408,
        MAGNITUDES = 409,
        MAGNITUDE = 410,
        initial_velocity = 411,
        strain_increment_size = 412,
        maximum_strain = 413,
        number_of_times_reaching_maximum_strain = 414,
        constitutive = 415,
        testing = 416,
        constant = 417,
        mean = 418,
        triaxial = 419,
        drained = 420,
        undrained = 421,
        simple = 422,
        shear = 423,
        number_of_subincrements = 424,
        maximum_number_of_iterations = 425,
        tolerance_1 = 426,
        tolerance_2 = 427,
        strain = 428,
        stress = 429,
        control = 430,
        Guass = 431,
        points = 432,
        Gauss = 433,
        each = 434,
        point = 435,
        single = 436,
        value = 437,
        EightNodeBrick = 438,
        TwentySevenNodeBrick = 439,
        EightNodeBrick_upU = 440,
        TwentyNodeBrick_uPU = 441,
        TwentyNodeBrick = 442,
        TwentyNodeBrickElastic = 443,
        EightNodeBrick_up = 444,
        variable_node_brick_8_to_27 = 445,
        EightNodeBrickElastic = 446,
        TwentySevenNodeBrickElastic = 447,
        beam_displacement_based = 448,
        BeamColumnDispFiber3d = 449,
        beam_elastic = 450,
        beam_elastic_lumped_mass = 451,
        beam_9dof_elastic = 452,
        FourNodeShellMITC4 = 453,
        FourNodeShellNewMITC4 = 454,
        ThreeNodeShellANDES = 455,
        FourNodeShellANDES = 456,
        truss = 457,
        contact = 458,
        FrictionalPenaltyContact = 459,
        EightNodeBrickLT = 460,
        EightNodeBrickLTNoOutput = 461,
        TwentyNodeBrickLT = 462,
        TwentySevenNodeBrickLT = 463,
        ShearBeamLT = 464,
        porosity = 465,
        alpha = 466,
        rho_s = 467,
        rho_f = 468,
        k_x = 469,
        k_y = 470,
        k_z = 471,
        K_s = 472,
        K_f = 473,
        pressure = 474,
        cross_section = 475,
        shear_modulus = 476,
        torsion_Jx = 477,
        bending_Iz = 478,
        bending_Iy = 479,
        IntegrationRule = 480,
        number_of_integration_points = 481,
        stiffness = 482,
        normal_stiffness = 483,
        tangential_stiffness = 484,
        normal_damping = 485,
        tangential_damping = 486,
        friction_ratio = 487,
        maximum_gap = 488,
        xz_plane_vector = 489,
        joint_1_offset = 490,
        joint_2_offset = 491,
        direction = 492,
        contact_plane_vector = 493,
        MembranePlateFiber = 494,
        ElasticMembranePlate = 495,
        elastic3d = 496,
        FIBER = 497,
        FiberSection = 498,
        Section = 499,
        fiber_cross_section = 500,
        fiber_location_Y = 501,
        fiber_location_Z = 502,
        fiber_location = 503,
        TorsionConstant_GJ = 504,
        thickness = 505,
        integration_rule = 506,
        section_number = 507,
        NDMaterialLT = 508,
        linear_elastic_isotropic_3d = 509,
        linear_elastic_isotropic_3d_LT = 510,
        sanisand2008 = 511,
        camclay = 512,
        camclay_accelerated = 513,
        sanisand2004 = 514,
        linear_elastic_crossanisotropic = 515,
        uniaxial_concrete02 = 516,
        uniaxial_elastic_1d = 517,
        uniaxial_steel01 = 518,
        uniaxial_steel02 = 519,
        pisano = 520,
        PisanoLT = 521,
        VonMisesLT = 522,
        DruckerPragerLT = 523,
        DruckerPragerVonMisesLT = 524,
        DruckerPragerArmstrongFrederickLT = 525,
        mass_density = 526,
        elastic_modulus = 527,
        viscoelastic_modulus = 528,
        poisson_ratio = 529,
        von_mises_radius = 530,
        druckerprager_angle = 531,
        druckerprager_k = 532,
        armstrong_frederick_ha = 533,
        armstrong_frederick_cr = 534,
        initial_confining_stress = 535,
        isotropic_hardening_rate = 536,
        kinematic_hardening_rate = 537,
        poisson_ratio_h_v = 538,
        poisson_ratio_h_h = 539,
        shear_modulus_h_v = 540,
        elastic_modulus_horizontal = 541,
        elastic_modulus_vertical = 542,
        pressure_reference_p0 = 543,
        initial_void_ratio = 544,
        initial_shear_modulus = 545,
        initial_bulk_modulus = 546,
        sanisand2008_Pat = 547,
        sanisand2008_k_c = 548,
        sanisand2008_K0 = 549,
        sanisand2008_alpha_cc = 550,
        sanisand2008_c = 551,
        sanisand2008_xi = 552,
        sanisand2008_lambda = 553,
        sanisand2008_ec_ref = 554,
        sanisand2008_m = 555,
        sanisand2008_h0 = 556,
        sanisand2008_ch = 557,
        sanisand2008_nb = 558,
        sanisand2008_A0 = 559,
        sanisand2008_nd = 560,
        sanisand2008_p_r = 561,
        sanisand2008_rho_c = 562,
        sanisand2008_theta_c = 563,
        sanisand2008_X = 564,
        sanisand2008_z_max = 565,
        sanisand2008_cz = 566,
        sanisand2008_p0 = 567,
        sanisand2008_p_in = 568,
        sanisand2008_G0 = 569,
        sanisand2004_K0 = 570,
        sanisand2004_Pat = 571,
        e0 = 572,
        sanisand2004_G0 = 573,
        sanisand2004_p_cut = 574,
        sanisand2004_Mc = 575,
        sanisand2004_c = 576,
        sanisand2004_lambda_c = 577,
        sanisand2004_xi = 578,
        sanisand2004_ec_ref = 579,
        sanisand2004_m = 580,
        sanisand2004_h0 = 581,
        sanisand2004_ch = 582,
        sanisand2004_nb = 583,
        sanisand2004_A0 = 584,
        sanisand2004_nd = 585,
        sanisand2004_z_max = 586,
        sanisand2004_cz = 587,
        reference_void_ratio = 588,
        critical_stress_ratio_M = 589,
        minimum_bulk_modulus = 590,
        initial_mean_pressure = 591,
        yield_strength = 592,
        strain_hardening_ratio = 593,
        compressive_strength = 594,
        strain_at_compressive_strength = 595,
        crushing_strength = 596,
        strain_at_crushing_strength = 597,
        tensile_strength = 598,
        tension_softening_stiffness = 599,
        M_in = 600,
        kd_in = 601,
        xi_in = 602,
        h_in = 603,
        m_in = 604,
        beta_min = 605,
        n_in = 606,
        a_in = 607,
        elastic_modulus_1atm = 608,
        eplcum_cr_in = 609,
        ax = 610,
        ay = 611,
        az = 612,
        verbose_level = 613,
        maximum_iterations = 614,
        tolerance = 615,
        yield_function_relative_tolerance = 616,
        stress_relative_tolerance = 617,
        beta = 618,
        gamma = 619,
        kappa = 620,
        lambda = 621,
        delta = 622,
        DOMAIN_ = 623,
        startTime = 624,
        endTime = 625,
        Period = 626,
        Phase = 627,
        Amplitude = 628,
        frequency = 629,
        MaxTime = 630,
        frequency1 = 631,
        frequency2 = 632,
        frequency3 = 633,
        frequency4 = 634,
        stiffness_to_use_opt = 635,
        DAMPING_RAYLEIGH = 636,
        DAMPING_CAUGHEY3 = 637,
        DAMPING_CAUGHEY4 = 638,
        a0 = 639,
        a1 = 640,
        stiffness_to_use = 641,
        a2 = 642,
        a3 = 643,
        a4 = 644,
        R0 = 645,
        cR1 = 646,
        cR2 = 647,
        MASS = 648,
        mx = 649,
        my = 650,
        mz = 651,
        Imx = 652,
        Imy = 653,
        Imz = 654,
        equaldof = 655,
        master = 656,
        slave = 657,
        dof_to_constrain = 658,
        of = 659,
        OUTPUT = 660,
        BINARY = 661,
        TEXT = 662,
        ENABLE = 663,
        DISABLE = 664,
        COMPRESSION = 665
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
      yylast_ = 8909,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 118, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 428  ///< Number of tokens.
    };


  };



} // yy
#line 865 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
