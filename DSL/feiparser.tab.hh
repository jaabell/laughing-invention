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
    #line 144 "feiparser.yy" // lalr1.cc:372

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
        SIMULATE_EXIT_FLAG = 289,
        UMINUS = 290,
        CMP = 291,
        CMD_print = 292,
        CMD_help = 293,
        CMD_whos = 294,
        CMD_check = 295,
        CMD_save = 296,
        CMD_restore = 297,
        MODEL = 298,
        NAME = 299,
        RESTART = 300,
        MESH = 301,
        ADD = 302,
        NODE = 303,
        ELEMENT = 304,
        ELEMENTS = 305,
        MATERIAL = 306,
        LOAD = 307,
        TIMEHISTORY = 308,
        IMPOSEDMOTION = 309,
        UNIFORMACCELERATION = 310,
        DAMPING = 311,
        DAMPINGTYPE = 312,
        CONSTRAINT = 313,
        DRM = 314,
        SECTION = 315,
        LOADPATTERN = 316,
        PENALTYDISPLACEMENT = 317,
        LOADVALUE = 318,
        SET = 319,
        SUPPORT = 320,
        REACTION = 321,
        REACTIONS = 322,
        FORCES = 323,
        DISPLACEMENT = 324,
        ELEMENTNAME = 325,
        MATERIALNAME = 326,
        ACCELERATION_FIELD = 327,
        FIX = 328,
        FREE = 329,
        REMOVE = 330,
        DEFINE = 331,
        ALGORITHM = 332,
        ALGNAME = 333,
        CONSTITUTIVE_ALGNAME = 334,
        CONVERGENCE_TEST = 335,
        TESTNAME = 336,
        SOLVER = 337,
        SOLVERNAME = 338,
        CONSTITUTIVE = 339,
        INTEGRATION = 340,
        DYNAMICINTEGRATOR = 341,
        DYNAMICINTEGRATOR_HHT = 342,
        DYNAMICINTEGRATOR_NEWMARK = 343,
        STATICINTEGRATOR = 344,
        STATICINTEGRATOR_DISPLACEMENT = 345,
        SIMULATE = 346,
        COMPUTE = 347,
        STATIC = 348,
        DYNAMIC = 349,
        USING = 350,
        TRANSIENT = 351,
        EIGEN = 352,
        time_step = 353,
        number_of_modes = 354,
        VARIABLETRANSIENT = 355,
        maximum_time_step = 356,
        minimum_time_step = 357,
        number_of_iterations = 358,
        RUNTEST = 359,
        AT = 360,
        ALL = 361,
        AND = 362,
        WITH = 363,
        TEXTDOFS = 364,
        NEW = 365,
        TEXTNUMBER = 366,
        USE = 367,
        TO = 368,
        DOF = 369,
        TEXTWITH = 370,
        NODES = 371,
        FORCE = 372,
        INTEGRATIONPOINTS = 373,
        dof = 374,
        RESPONSE = 375,
        FILE = 376,
        FROM = 377,
        EVERY = 378,
        LEVEL = 379,
        BARDETMETHOD = 380,
        LOADING = 381,
        STAGE = 382,
        STEPS = 383,
        TYPE = 384,
        DOFS = 385,
        FACTOR = 386,
        INCREMENT = 387,
        TH_GROUNDMOTION = 388,
        TH_LINEAR = 389,
        TH_PATH_SERIES = 390,
        TH_PATH_TIME_SERIES = 391,
        TH_CONSTANT = 392,
        TH_FROM_REACTIONS = 393,
        self_weight = 394,
        surface = 395,
        load_value = 396,
        scale_factor = 397,
        displacement_scale_unit = 398,
        velocity_scale_unit = 399,
        acceleration_scale_unit = 400,
        number_of_steps = 401,
        number_of_boundary_nodes = 402,
        number_of_exterior_nodes = 403,
        number_of_drm_elements = 404,
        element_file = 405,
        boundary_nodes_file = 406,
        exterior_nodes_file = 407,
        displacement_file = 408,
        acceleration_file = 409,
        velocity_file = 410,
        force_file = 411,
        hdf5_file = 412,
        series_file = 413,
        time_series_file = 414,
        MAGNITUDES = 415,
        MAGNITUDE = 416,
        initial_velocity = 417,
        strain_increment_size = 418,
        maximum_strain = 419,
        number_of_times_reaching_maximum_strain = 420,
        constitutive = 421,
        testing = 422,
        constant = 423,
        mean = 424,
        triaxial = 425,
        drained = 426,
        undrained = 427,
        simple = 428,
        shear = 429,
        number_of_subincrements = 430,
        maximum_number_of_iterations = 431,
        tolerance_1 = 432,
        tolerance_2 = 433,
        strain = 434,
        stress = 435,
        control = 436,
        Guass = 437,
        points = 438,
        Gauss = 439,
        each = 440,
        point = 441,
        single = 442,
        value = 443,
        initial_normal_stiffness = 444,
        stiffning_rate = 445,
        number_of_integration_points = 446,
        DIRECT_STRAIN = 447,
        EightNodeBrick = 448,
        TwentyNodeBrick = 449,
        TwentySevenNodeBrick = 450,
        VariableNodeBrick = 451,
        variable_node_brick_8_to_27 = 452,
        EightNodeBrick_up = 453,
        TwentyNodeBrick_up = 454,
        TwentySevenNodeBrick_up = 455,
        VariableNodeBrick_up = 456,
        EightNodeBrick_upU = 457,
        TwentyNodeBrick_upU = 458,
        TwentySevenNodeBrick_upU = 459,
        VariableNodeBrick_upU = 460,
        beam_displacement_based = 461,
        BeamColumnDispFiber3d = 462,
        beam_elastic = 463,
        beam_elastic_lumped_mass = 464,
        beam_9dof_elastic = 465,
        FourNodeShellMITC4 = 466,
        FourNodeShellNewMITC4 = 467,
        ThreeNodeShellANDES = 468,
        FourNodeShellANDES = 469,
        truss = 470,
        contact = 471,
        HardContact = 472,
        FrictionalPenaltyContact = 473,
        SoftContact = 474,
        ShearBeam = 475,
        porosity = 476,
        alpha = 477,
        rho_s = 478,
        rho_f = 479,
        k_x = 480,
        k_y = 481,
        k_z = 482,
        K_s = 483,
        K_f = 484,
        pressure = 485,
        cross_section = 486,
        shear_modulus = 487,
        torsion_Jx = 488,
        bending_Iz = 489,
        bending_Iy = 490,
        IntegrationRule = 491,
        stiffness = 492,
        normal_stiffness = 493,
        tangential_stiffness = 494,
        normal_damping = 495,
        tangential_damping = 496,
        friction_ratio = 497,
        maximum_gap = 498,
        xz_plane_vector = 499,
        joint_1_offset = 500,
        joint_2_offset = 501,
        direction = 502,
        contact_plane_vector = 503,
        power = 504,
        linear = 505,
        exponential = 506,
        factorial = 507,
        inverse = 508,
        MembranePlateFiber = 509,
        ElasticMembranePlate = 510,
        elastic3d = 511,
        FIBER = 512,
        FiberSection = 513,
        Section = 514,
        fiber_cross_section = 515,
        fiber_location_Y = 516,
        fiber_location_Z = 517,
        fiber_location = 518,
        TorsionConstant_GJ = 519,
        thickness = 520,
        integration_rule = 521,
        section_number = 522,
        NDMaterialLT = 523,
        linear_elastic_isotropic_3d = 524,
        linear_elastic_isotropic_3d_LT = 525,
        NonlinearIsotropic3DLT = 526,
        sanisand2008 = 527,
        CamClay = 528,
        sanisand2004 = 529,
        sanisand2004_legacy = 530,
        roundedMohrCoulomb = 531,
        linear_elastic_crossanisotropic = 532,
        uniaxial_concrete02 = 533,
        uniaxial_elastic_1d = 534,
        uniaxial_steel01 = 535,
        uniaxial_steel02 = 536,
        pisano = 537,
        PisanoLT = 538,
        CamClayLT = 539,
        VonMisesLT = 540,
        VonMisesArmstrongFrederickLT = 541,
        DruckerPragerLT = 542,
        DruckerPragerNonAssociateLinearHardeningLT = 543,
        DruckerPragerVonMisesLT = 544,
        DruckerPragerArmstrongFrederickLE = 545,
        DruckerPragerArmstrongFrederickNE = 546,
        DruckerPragerNonAssociateArmstrongFrederickLT = 547,
        mass_density = 548,
        elastic_modulus = 549,
        viscoelastic_modulus = 550,
        poisson_ratio = 551,
        von_mises_radius = 552,
        druckerprager_angle = 553,
        druckerprager_k = 554,
        armstrong_frederick_ha = 555,
        armstrong_frederick_cr = 556,
        initial_confining_stress = 557,
        isotropic_hardening_rate = 558,
        kinematic_hardening_rate = 559,
        poisson_ratio_h_v = 560,
        poisson_ratio_h_h = 561,
        shear_modulus_h_v = 562,
        elastic_modulus_horizontal = 563,
        elastic_modulus_vertical = 564,
        pressure_reference_p0 = 565,
        plastic_flow_kd = 566,
        plastic_flow_xi = 567,
        initial_void_ratio = 568,
        initial_shear_modulus = 569,
        initial_bulk_modulus = 570,
        sanisand2008_Pat = 571,
        sanisand2008_k_c = 572,
        sanisand2008_K0 = 573,
        sanisand2008_alpha_cc = 574,
        sanisand2008_c = 575,
        sanisand2008_xi = 576,
        sanisand2008_lambda = 577,
        sanisand2008_ec_ref = 578,
        sanisand2008_m = 579,
        sanisand2008_h0 = 580,
        sanisand2008_ch = 581,
        sanisand2008_nb = 582,
        sanisand2008_A0 = 583,
        sanisand2008_nd = 584,
        sanisand2008_p_r = 585,
        sanisand2008_rho_c = 586,
        sanisand2008_theta_c = 587,
        sanisand2008_X = 588,
        sanisand2008_z_max = 589,
        sanisand2008_cz = 590,
        sanisand2008_p0 = 591,
        sanisand2008_p_in = 592,
        sanisand2008_G0 = 593,
        sanisand2004_K0 = 594,
        sanisand2004_Pat = 595,
        e0 = 596,
        sanisand2004_G0 = 597,
        sanisand2004_p_cut = 598,
        sanisand2004_Mc = 599,
        sanisand2004_c = 600,
        sanisand2004_lambda_c = 601,
        sanisand2004_xi = 602,
        sanisand2004_ec_ref = 603,
        sanisand2004_m = 604,
        sanisand2004_h0 = 605,
        sanisand2004_ch = 606,
        sanisand2004_nb = 607,
        sanisand2004_A0 = 608,
        sanisand2004_nd = 609,
        sanisand2004_z_max = 610,
        sanisand2004_cz = 611,
        reference_void_ratio = 612,
        critical_stress_ratio_M = 613,
        minimum_bulk_modulus = 614,
        initial_mean_pressure = 615,
        yield_strength = 616,
        strain_hardening_ratio = 617,
        compressive_strength = 618,
        strain_at_compressive_strength = 619,
        crushing_strength = 620,
        strain_at_crushing_strength = 621,
        tensile_strength = 622,
        tension_softening_stiffness = 623,
        M_in = 624,
        kd_in = 625,
        xi_in = 626,
        h_in = 627,
        m_in = 628,
        beta_min = 629,
        n_in = 630,
        a_in = 631,
        elastic_modulus_1atm = 632,
        eplcum_cr_in = 633,
        CriticalState_M = 634,
        CriticalState_lambda = 635,
        CriticalState_kappa = 636,
        CriticalState_e0 = 637,
        CriticalState_p0 = 638,
        RMC_m = 639,
        RMC_qa = 640,
        RMC_pc = 641,
        RMC_e = 642,
        RMC_eta0 = 643,
        RMC_Heta = 644,
        DuncanCheng_K = 645,
        DuncanCheng_pa = 646,
        DuncanCheng_n = 647,
        DuncanCheng_sigma3_max = 648,
        DuncanCheng_nu = 649,
        ax = 650,
        ay = 651,
        az = 652,
        verbose_level = 653,
        maximum_iterations = 654,
        tolerance = 655,
        yield_function_relative_tolerance = 656,
        stress_relative_tolerance = 657,
        verbose_output = 658,
        beta = 659,
        gamma = 660,
        kappa = 661,
        lambda = 662,
        delta = 663,
        sigma0 = 664,
        DOMAIN_ = 665,
        startTime = 666,
        endTime = 667,
        Period = 668,
        Phase = 669,
        Amplitude = 670,
        frequency = 671,
        MaxTime = 672,
        frequency1 = 673,
        frequency2 = 674,
        frequency3 = 675,
        frequency4 = 676,
        stiffness_to_use_opt = 677,
        DAMPING_RAYLEIGH = 678,
        DAMPING_CAUGHEY3 = 679,
        DAMPING_CAUGHEY4 = 680,
        a0 = 681,
        a1 = 682,
        stiffness_to_use = 683,
        a2 = 684,
        a3 = 685,
        a4 = 686,
        R0 = 687,
        cR1 = 688,
        cR2 = 689,
        MASS = 690,
        mx = 691,
        my = 692,
        mz = 693,
        Imx = 694,
        Imy = 695,
        Imz = 696,
        equal_dof = 697,
        master = 698,
        slave = 699,
        dof_to_constrain = 700,
        of = 701,
        OUTPUT = 702,
        BINARY = 703,
        TEXT = 704,
        ENABLE = 705,
        DISABLE = 706,
        COMPRESSION = 707,
        NON_CONVERGED_ITERATIONS = 708
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
      yylast_ = 14222,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 121, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 471  ///< Number of tokens.
    };


  };



} // yy
#line 908 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
