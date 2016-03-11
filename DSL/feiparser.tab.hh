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
        testing = 415,
        constant = 416,
        mean = 417,
        triaxial = 418,
        drained = 419,
        undrained = 420,
        simple = 421,
        shear = 422,
        number_of_subincrements = 423,
        maximum_number_of_iterations = 424,
        tolerance_1 = 425,
        tolerance_2 = 426,
        strain = 427,
        stress = 428,
        control = 429,
        Guass = 430,
        points = 431,
        Gauss = 432,
        each = 433,
        point = 434,
        single = 435,
        value = 436,
        EightNodeBrick = 437,
        TwentySevenNodeBrick = 438,
        EightNodeBrick_upU = 439,
        TwentyNodeBrick_uPU = 440,
        TwentyNodeBrick = 441,
        TwentyNodeBrickElastic = 442,
        EightNodeBrick_up = 443,
        variable_node_brick_8_to_27 = 444,
        EightNodeBrickElastic = 445,
        TwentySevenNodeBrickElastic = 446,
        beam_displacement_based = 447,
        BeamColumnDispFiber3d = 448,
        beam_elastic = 449,
        beam_elastic_lumped_mass = 450,
        beam_9dof_elastic = 451,
        FourNodeShellMITC4 = 452,
        FourNodeShellNewMITC4 = 453,
        ThreeNodeShellANDES = 454,
        FourNodeShellANDES = 455,
        truss = 456,
        contact = 457,
        FrictionalPenaltyContact = 458,
        EightNodeBrickLT = 459,
        EightNodeBrickLTNoOutput = 460,
        TwentySevenNodeBrickLT = 461,
        ShearBeamLT = 462,
        porosity = 463,
        alpha = 464,
        rho_s = 465,
        rho_f = 466,
        k_x = 467,
        k_y = 468,
        k_z = 469,
        K_s = 470,
        K_f = 471,
        pressure = 472,
        cross_section = 473,
        shear_modulus = 474,
        torsion_Jx = 475,
        bending_Iz = 476,
        bending_Iy = 477,
        IntegrationRule = 478,
        stiffness = 479,
        normal_stiffness = 480,
        tangential_stiffness = 481,
        normal_damping = 482,
        tangential_damping = 483,
        friction_ratio = 484,
        maximum_gap = 485,
        xz_plane_vector = 486,
        joint_1_offset = 487,
        joint_2_offset = 488,
        direction = 489,
        contact_plane_vector = 490,
        MembranePlateFiber = 491,
        ElasticMembranePlate = 492,
        elastic3d = 493,
        FIBER = 494,
        FiberSection = 495,
        Section = 496,
        fiber_cross_section = 497,
        fiber_location_Y = 498,
        fiber_location_Z = 499,
        fiber_location = 500,
        TorsionConstant_GJ = 501,
        thickness = 502,
        integration_rule = 503,
        section_number = 504,
        number_of_integration_points = 505,
        NDMaterialLT = 506,
        linear_elastic_isotropic_3d = 507,
        linear_elastic_isotropic_3d_LT = 508,
        sanisand2008 = 509,
        camclay = 510,
        camclay_accelerated = 511,
        sanisand2004 = 512,
        linear_elastic_crossanisotropic = 513,
        uniaxial_concrete02 = 514,
        uniaxial_elastic_1d = 515,
        uniaxial_steel01 = 516,
        uniaxial_steel02 = 517,
        pisano = 518,
        PisanoLT = 519,
        VonMisesLT = 520,
        DruckerPragerLT = 521,
        DruckerPragerVonMisesLT = 522,
        DruckerPragerArmstrongFrederickLT = 523,
        mass_density = 524,
        elastic_modulus = 525,
        viscoelastic_modulus = 526,
        poisson_ratio = 527,
        von_mises_radius = 528,
        druckerprager_angle = 529,
        druckerprager_k = 530,
        armstrong_frederick_ha = 531,
        armstrong_frederick_cr = 532,
        initial_confining_stress = 533,
        isotropic_hardening_rate = 534,
        kinematic_hardening_rate = 535,
        poisson_ratio_h_v = 536,
        poisson_ratio_h_h = 537,
        shear_modulus_h_v = 538,
        elastic_modulus_horizontal = 539,
        elastic_modulus_vertical = 540,
        pressure_reference_p0 = 541,
        initial_void_ratio = 542,
        initial_shear_modulus = 543,
        initial_bulk_modulus = 544,
        sanisand2008_Pat = 545,
        sanisand2008_k_c = 546,
        sanisand2008_K0 = 547,
        sanisand2008_alpha_cc = 548,
        sanisand2008_c = 549,
        sanisand2008_xi = 550,
        sanisand2008_lambda = 551,
        sanisand2008_ec_ref = 552,
        sanisand2008_m = 553,
        sanisand2008_h0 = 554,
        sanisand2008_ch = 555,
        sanisand2008_nb = 556,
        sanisand2008_A0 = 557,
        sanisand2008_nd = 558,
        sanisand2008_p_r = 559,
        sanisand2008_rho_c = 560,
        sanisand2008_theta_c = 561,
        sanisand2008_X = 562,
        sanisand2008_z_max = 563,
        sanisand2008_cz = 564,
        sanisand2008_p0 = 565,
        sanisand2008_p_in = 566,
        sanisand2008_G0 = 567,
        sanisand2004_K0 = 568,
        sanisand2004_Pat = 569,
        e0 = 570,
        sanisand2004_G0 = 571,
        sanisand2004_p_cut = 572,
        sanisand2004_Mc = 573,
        sanisand2004_c = 574,
        sanisand2004_lambda_c = 575,
        sanisand2004_xi = 576,
        sanisand2004_ec_ref = 577,
        sanisand2004_m = 578,
        sanisand2004_h0 = 579,
        sanisand2004_ch = 580,
        sanisand2004_nb = 581,
        sanisand2004_A0 = 582,
        sanisand2004_nd = 583,
        sanisand2004_z_max = 584,
        sanisand2004_cz = 585,
        reference_void_ratio = 586,
        critical_stress_ratio_M = 587,
        minimum_bulk_modulus = 588,
        initial_mean_pressure = 589,
        yield_strength = 590,
        strain_hardening_ratio = 591,
        compressive_strength = 592,
        strain_at_compressive_strength = 593,
        crushing_strength = 594,
        strain_at_crushing_strength = 595,
        tensile_strength = 596,
        tension_softening_stiffness = 597,
        M_in = 598,
        kd_in = 599,
        xi_in = 600,
        h_in = 601,
        m_in = 602,
        beta_min = 603,
        n_in = 604,
        a_in = 605,
        elastic_modulus_1atm = 606,
        eplcum_cr_in = 607,
        ax = 608,
        ay = 609,
        az = 610,
        verbose_level = 611,
        maximum_iterations = 612,
        tolerance = 613,
        yield_function_relative_tolerance = 614,
        stress_relative_tolerance = 615,
        beta = 616,
        gamma = 617,
        kappa = 618,
        lambda = 619,
        delta = 620,
        DOMAIN_ = 621,
        startTime = 622,
        endTime = 623,
        Period = 624,
        Phase = 625,
        Amplitude = 626,
        frequency = 627,
        MaxTime = 628,
        frequency1 = 629,
        frequency2 = 630,
        frequency3 = 631,
        frequency4 = 632,
        stiffness_to_use_opt = 633,
        DAMPING_RAYLEIGH = 634,
        DAMPING_CAUGHEY3 = 635,
        DAMPING_CAUGHEY4 = 636,
        a0 = 637,
        a1 = 638,
        stiffness_to_use = 639,
        a2 = 640,
        a3 = 641,
        a4 = 642,
        R0 = 643,
        cR1 = 644,
        cR2 = 645,
        MASS = 646,
        mx = 647,
        my = 648,
        mz = 649,
        Imx = 650,
        Imy = 651,
        Imz = 652,
        equaldof = 653,
        master = 654,
        slave = 655,
        dof_to_constrain = 656,
        of = 657,
        OUTPUT = 658,
        BINARY = 659,
        TEXT = 660,
        ENABLE = 661,
        DISABLE = 662,
        COMPRESSION = 663
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
      yylast_ = 8708,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 118, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 426  ///< Number of tokens.
    };


  };



} // yy
#line 863 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
