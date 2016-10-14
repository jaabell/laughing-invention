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
        SUPPORT = 319,
        REACTION = 320,
        REACTIONS = 321,
        FORCES = 322,
        DISPLACEMENT = 323,
        ELEMENTNAME = 324,
        MATERIALNAME = 325,
        ACCELERATION_FIELD = 326,
        FIX = 327,
        FREE = 328,
        REMOVE = 329,
        DEFINE = 330,
        ALGORITHM = 331,
        ALGNAME = 332,
        CONSTITUTIVE_ALGNAME = 333,
        CONVERGENCE_TEST = 334,
        TESTNAME = 335,
        SOLVER = 336,
        SOLVERNAME = 337,
        CONSTITUTIVE = 338,
        INTEGRATION = 339,
        DYNAMICINTEGRATOR = 340,
        DYNAMICINTEGRATOR_HHT = 341,
        DYNAMICINTEGRATOR_NEWMARK = 342,
        STATICINTEGRATOR = 343,
        STATICINTEGRATOR_DISPLACEMENT = 344,
        SIMULATE = 345,
        COMPUTE = 346,
        STATIC = 347,
        DYNAMIC = 348,
        USING = 349,
        TRANSIENT = 350,
        EIGEN = 351,
        time_step = 352,
        number_of_modes = 353,
        VARIABLETRANSIENT = 354,
        maximum_time_step = 355,
        minimum_time_step = 356,
        number_of_iterations = 357,
        RUNTEST = 358,
        AT = 359,
        ALL = 360,
        AND = 361,
        WITH = 362,
        TEXTDOFS = 363,
        NEW = 364,
        TEXTNUMBER = 365,
        USE = 366,
        TO = 367,
        DOF = 368,
        TEXTWITH = 369,
        NODES = 370,
        FORCE = 371,
        INTEGRATIONPOINTS = 372,
        dof = 373,
        RESPONSE = 374,
        FILE = 375,
        FROM = 376,
        EVERY = 377,
        LEVEL = 378,
        BARDETMETHOD = 379,
        LOADING = 380,
        STAGE = 381,
        STEPS = 382,
        TYPE = 383,
        DOFS = 384,
        FACTOR = 385,
        INCREMENT = 386,
        TH_GROUNDMOTION = 387,
        TH_LINEAR = 388,
        TH_PATH_SERIES = 389,
        TH_PATH_TIME_SERIES = 390,
        TH_CONSTANT = 391,
        TH_FROM_REACTIONS = 392,
        self_weight = 393,
        surface = 394,
        load_value = 395,
        scale_factor = 396,
        displacement_scale_unit = 397,
        velocity_scale_unit = 398,
        acceleration_scale_unit = 399,
        number_of_steps = 400,
        number_of_boundary_nodes = 401,
        number_of_exterior_nodes = 402,
        number_of_drm_elements = 403,
        element_file = 404,
        boundary_nodes_file = 405,
        exterior_nodes_file = 406,
        displacement_file = 407,
        acceleration_file = 408,
        velocity_file = 409,
        force_file = 410,
        hdf5_file = 411,
        series_file = 412,
        time_series_file = 413,
        MAGNITUDES = 414,
        MAGNITUDE = 415,
        initial_velocity = 416,
        strain_increment_size = 417,
        maximum_strain = 418,
        number_of_times_reaching_maximum_strain = 419,
        constitutive = 420,
        testing = 421,
        constant = 422,
        mean = 423,
        triaxial = 424,
        drained = 425,
        undrained = 426,
        simple = 427,
        shear = 428,
        number_of_subincrements = 429,
        maximum_number_of_iterations = 430,
        tolerance_1 = 431,
        tolerance_2 = 432,
        strain = 433,
        stress = 434,
        control = 435,
        Guass = 436,
        points = 437,
        Gauss = 438,
        each = 439,
        point = 440,
        single = 441,
        value = 442,
        initial_normal_stiffness = 443,
        stiffning_rate = 444,
        number_of_integration_points = 445,
        DIRECT_STRAIN = 446,
        EightNodeBrick = 447,
        TwentyNodeBrick = 448,
        TwentySevenNodeBrick = 449,
        VariableNodeBrick = 450,
        variable_node_brick_8_to_27 = 451,
        EightNodeBrick_up = 452,
        TwentyNodeBrick_up = 453,
        TwentySevenNodeBrick_up = 454,
        VariableNodeBrick_up = 455,
        EightNodeBrick_upU = 456,
        TwentyNodeBrick_upU = 457,
        TwentySevenNodeBrick_upU = 458,
        VariableNodeBrick_upU = 459,
        beam_displacement_based = 460,
        BeamColumnDispFiber3d = 461,
        beam_elastic = 462,
        beam_elastic_lumped_mass = 463,
        beam_9dof_elastic = 464,
        FourNodeShellMITC4 = 465,
        FourNodeShellNewMITC4 = 466,
        ThreeNodeShellANDES = 467,
        FourNodeShellANDES = 468,
        truss = 469,
        contact = 470,
        HardContact = 471,
        FrictionalPenaltyContact = 472,
        SoftContact = 473,
        ShearBeam = 474,
        porosity = 475,
        alpha = 476,
        rho_s = 477,
        rho_f = 478,
        k_x = 479,
        k_y = 480,
        k_z = 481,
        K_s = 482,
        K_f = 483,
        pressure = 484,
        cross_section = 485,
        shear_modulus = 486,
        torsion_Jx = 487,
        bending_Iz = 488,
        bending_Iy = 489,
        IntegrationRule = 490,
        stiffness = 491,
        normal_stiffness = 492,
        tangential_stiffness = 493,
        normal_damping = 494,
        tangential_damping = 495,
        friction_ratio = 496,
        maximum_gap = 497,
        xz_plane_vector = 498,
        joint_1_offset = 499,
        joint_2_offset = 500,
        direction = 501,
        contact_plane_vector = 502,
        power = 503,
        linear = 504,
        exponential = 505,
        factorial = 506,
        inverse = 507,
        MembranePlateFiber = 508,
        ElasticMembranePlate = 509,
        elastic3d = 510,
        FIBER = 511,
        FiberSection = 512,
        Section = 513,
        fiber_cross_section = 514,
        fiber_location_Y = 515,
        fiber_location_Z = 516,
        fiber_location = 517,
        TorsionConstant_GJ = 518,
        thickness = 519,
        integration_rule = 520,
        section_number = 521,
        NDMaterialLT = 522,
        linear_elastic_isotropic_3d = 523,
        linear_elastic_isotropic_3d_LT = 524,
        NonlinearIsotropic3DLT = 525,
        sanisand2008 = 526,
        CamClay = 527,
        sanisand2004 = 528,
        sanisand2004_legacy = 529,
        roundedMohrCoulomb = 530,
        linear_elastic_crossanisotropic = 531,
        uniaxial_concrete02 = 532,
        uniaxial_elastic_1d = 533,
        uniaxial_steel01 = 534,
        uniaxial_steel02 = 535,
        pisano = 536,
        PisanoLT = 537,
        CamClayLT = 538,
        VonMisesLT = 539,
        VonMisesArmstrongFrederickLT = 540,
        DruckerPragerLT = 541,
        DruckerPragerNonAssociateLinearHardeningLT = 542,
        DruckerPragerVonMisesLT = 543,
        DruckerPragerArmstrongFrederickLE = 544,
        DruckerPragerArmstrongFrederickNE = 545,
        DruckerPragerNonAssociateArmstrongFrederickLT = 546,
        mass_density = 547,
        elastic_modulus = 548,
        viscoelastic_modulus = 549,
        poisson_ratio = 550,
        von_mises_radius = 551,
        druckerprager_angle = 552,
        druckerprager_k = 553,
        armstrong_frederick_ha = 554,
        armstrong_frederick_cr = 555,
        initial_confining_stress = 556,
        isotropic_hardening_rate = 557,
        kinematic_hardening_rate = 558,
        poisson_ratio_h_v = 559,
        poisson_ratio_h_h = 560,
        shear_modulus_h_v = 561,
        elastic_modulus_horizontal = 562,
        elastic_modulus_vertical = 563,
        pressure_reference_p0 = 564,
        plastic_flow_kd = 565,
        plastic_flow_xi = 566,
        initial_void_ratio = 567,
        initial_shear_modulus = 568,
        initial_bulk_modulus = 569,
        sanisand2008_Pat = 570,
        sanisand2008_k_c = 571,
        sanisand2008_K0 = 572,
        sanisand2008_alpha_cc = 573,
        sanisand2008_c = 574,
        sanisand2008_xi = 575,
        sanisand2008_lambda = 576,
        sanisand2008_ec_ref = 577,
        sanisand2008_m = 578,
        sanisand2008_h0 = 579,
        sanisand2008_ch = 580,
        sanisand2008_nb = 581,
        sanisand2008_A0 = 582,
        sanisand2008_nd = 583,
        sanisand2008_p_r = 584,
        sanisand2008_rho_c = 585,
        sanisand2008_theta_c = 586,
        sanisand2008_X = 587,
        sanisand2008_z_max = 588,
        sanisand2008_cz = 589,
        sanisand2008_p0 = 590,
        sanisand2008_p_in = 591,
        sanisand2008_G0 = 592,
        sanisand2004_K0 = 593,
        sanisand2004_Pat = 594,
        e0 = 595,
        sanisand2004_G0 = 596,
        sanisand2004_p_cut = 597,
        sanisand2004_Mc = 598,
        sanisand2004_c = 599,
        sanisand2004_lambda_c = 600,
        sanisand2004_xi = 601,
        sanisand2004_ec_ref = 602,
        sanisand2004_m = 603,
        sanisand2004_h0 = 604,
        sanisand2004_ch = 605,
        sanisand2004_nb = 606,
        sanisand2004_A0 = 607,
        sanisand2004_nd = 608,
        sanisand2004_z_max = 609,
        sanisand2004_cz = 610,
        reference_void_ratio = 611,
        critical_stress_ratio_M = 612,
        minimum_bulk_modulus = 613,
        initial_mean_pressure = 614,
        yield_strength = 615,
        strain_hardening_ratio = 616,
        compressive_strength = 617,
        strain_at_compressive_strength = 618,
        crushing_strength = 619,
        strain_at_crushing_strength = 620,
        tensile_strength = 621,
        tension_softening_stiffness = 622,
        M_in = 623,
        kd_in = 624,
        xi_in = 625,
        h_in = 626,
        m_in = 627,
        beta_min = 628,
        n_in = 629,
        a_in = 630,
        elastic_modulus_1atm = 631,
        eplcum_cr_in = 632,
        Niso3d_K = 633,
        Niso3d_Kur = 634,
        Niso3d_n = 635,
        Niso3d_c = 636,
        Niso3d_phi0 = 637,
        Niso3d_dphi = 638,
        Niso3d_Rf = 639,
        Niso3d_K0 = 640,
        Niso3d_Kb = 641,
        Niso3d_m = 642,
        Niso3d_pa = 643,
        Niso3d_K2 = 644,
        Niso3d_B = 645,
        Niso3d_Et = 646,
        Niso3d_Ei = 647,
        Niso3d_Er = 648,
        CriticalState_M = 649,
        CriticalState_lambda = 650,
        CriticalState_kappa = 651,
        CriticalState_e0 = 652,
        CriticalState_p0 = 653,
        RMC_m = 654,
        RMC_qa = 655,
        RMC_pc = 656,
        RMC_e = 657,
        RMC_eta0 = 658,
        RMC_Heta = 659,
        DuncanCheng_K = 660,
        DuncanCheng_pa = 661,
        DuncanCheng_n = 662,
        DuncanCheng_sigma3_max = 663,
        DuncanCheng_nu = 664,
        ax = 665,
        ay = 666,
        az = 667,
        verbose_level = 668,
        maximum_iterations = 669,
        tolerance = 670,
        yield_function_relative_tolerance = 671,
        stress_relative_tolerance = 672,
        verbose_output = 673,
        beta = 674,
        gamma = 675,
        kappa = 676,
        lambda = 677,
        delta = 678,
        sigma0 = 679,
        DOMAIN_ = 680,
        startTime = 681,
        endTime = 682,
        Period = 683,
        Phase = 684,
        Amplitude = 685,
        frequency = 686,
        MaxTime = 687,
        frequency1 = 688,
        frequency2 = 689,
        frequency3 = 690,
        frequency4 = 691,
        stiffness_to_use_opt = 692,
        DAMPING_RAYLEIGH = 693,
        DAMPING_CAUGHEY3 = 694,
        DAMPING_CAUGHEY4 = 695,
        a0 = 696,
        a1 = 697,
        stiffness_to_use = 698,
        a2 = 699,
        a3 = 700,
        a4 = 701,
        R0 = 702,
        cR1 = 703,
        cR2 = 704,
        MASS = 705,
        mx = 706,
        my = 707,
        mz = 708,
        Imx = 709,
        Imy = 710,
        Imz = 711,
        equal_dof = 712,
        master = 713,
        slave = 714,
        dof_to_constrain = 715,
        of = 716,
        OUTPUT = 717,
        BINARY = 718,
        TEXT = 719,
        ENABLE = 720,
        DISABLE = 721,
        COMPRESSION = 722,
        NON_CONVERGED_ITERATIONS = 723
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
      yylast_ = 14015,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 120, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 486  ///< Number of tokens.
    };


  };



} // yy
#line 923 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
