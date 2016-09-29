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
        REACTION = 319,
        FORCES = 320,
        DISPLACEMENT = 321,
        ELEMENTNAME = 322,
        MATERIALNAME = 323,
        ACCELERATION_FIELD = 324,
        FIX = 325,
        FREE = 326,
        REMOVE = 327,
        DEFINE = 328,
        ALGORITHM = 329,
        ALGNAME = 330,
        CONSTITUTIVE_ALGNAME = 331,
        CONVERGENCE_TEST = 332,
        TESTNAME = 333,
        SOLVER = 334,
        SOLVERNAME = 335,
        CONSTITUTIVE = 336,
        INTEGRATION = 337,
        DYNAMICINTEGRATOR = 338,
        DYNAMICINTEGRATOR_HHT = 339,
        DYNAMICINTEGRATOR_NEWMARK = 340,
        STATICINTEGRATOR = 341,
        STATICINTEGRATOR_DISPLACEMENT = 342,
        SIMULATE = 343,
        COMPUTE = 344,
        STATIC = 345,
        DYNAMIC = 346,
        USING = 347,
        TRANSIENT = 348,
        EIGEN = 349,
        time_step = 350,
        number_of_modes = 351,
        VARIABLETRANSIENT = 352,
        maximum_time_step = 353,
        minimum_time_step = 354,
        number_of_iterations = 355,
        RUNTEST = 356,
        AT = 357,
        ALL = 358,
        AND = 359,
        WITH = 360,
        TEXTDOFS = 361,
        NEW = 362,
        TEXTNUMBER = 363,
        USE = 364,
        TO = 365,
        DOF = 366,
        TEXTWITH = 367,
        NODES = 368,
        FORCE = 369,
        INTEGRATIONPOINTS = 370,
        dof = 371,
        RESPONSE = 372,
        FILE = 373,
        FROM = 374,
        EVERY = 375,
        LEVEL = 376,
        BARDETMETHOD = 377,
        LOADING = 378,
        STAGE = 379,
        STEPS = 380,
        TYPE = 381,
        DOFS = 382,
        FACTOR = 383,
        INCREMENT = 384,
        TH_GROUNDMOTION = 385,
        TH_LINEAR = 386,
        TH_PATH_SERIES = 387,
        TH_PATH_TIME_SERIES = 388,
        TH_CONSTANT = 389,
        TH_FROM_REACTIONS = 390,
        self_weight = 391,
        surface = 392,
        load_value = 393,
        scale_factor = 394,
        displacement_scale_unit = 395,
        velocity_scale_unit = 396,
        acceleration_scale_unit = 397,
        number_of_steps = 398,
        number_of_boundary_nodes = 399,
        number_of_exterior_nodes = 400,
        number_of_drm_elements = 401,
        element_file = 402,
        boundary_nodes_file = 403,
        exterior_nodes_file = 404,
        displacement_file = 405,
        acceleration_file = 406,
        velocity_file = 407,
        force_file = 408,
        hdf5_file = 409,
        series_file = 410,
        time_series_file = 411,
        MAGNITUDES = 412,
        MAGNITUDE = 413,
        initial_velocity = 414,
        strain_increment_size = 415,
        maximum_strain = 416,
        number_of_times_reaching_maximum_strain = 417,
        constitutive = 418,
        testing = 419,
        constant = 420,
        mean = 421,
        triaxial = 422,
        drained = 423,
        undrained = 424,
        simple = 425,
        shear = 426,
        number_of_subincrements = 427,
        maximum_number_of_iterations = 428,
        tolerance_1 = 429,
        tolerance_2 = 430,
        strain = 431,
        stress = 432,
        control = 433,
        Guass = 434,
        points = 435,
        Gauss = 436,
        each = 437,
        point = 438,
        single = 439,
        value = 440,
        initial_normal_stiffness = 441,
        stiffning_rate = 442,
        number_of_integration_points = 443,
        DIRECT_STRAIN = 444,
        EightNodeBrick = 445,
        TwentySevenNodeBrick = 446,
        EightNodeBrick_upU = 447,
        TwentyNodeBrick_uPU = 448,
        TwentyNodeBrick = 449,
        EightNodeBrick_up = 450,
        variable_node_brick_8_to_27 = 451,
        beam_displacement_based = 452,
        BeamColumnDispFiber3d = 453,
        beam_elastic = 454,
        beam_elastic_lumped_mass = 455,
        beam_9dof_elastic = 456,
        FourNodeShellMITC4 = 457,
        FourNodeShellNewMITC4 = 458,
        ThreeNodeShellANDES = 459,
        FourNodeShellANDES = 460,
        truss = 461,
        contact = 462,
        HardContact = 463,
        FrictionalPenaltyContact = 464,
        SoftContact = 465,
        ShearBeam = 466,
        EightNodeBrick_upULT = 467,
        TwentyNodeBrick_uPULT = 468,
        TwentySevenNodeBrick_uPULT = 469,
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
        NDMaterialLT = 517,
        linear_elastic_isotropic_3d = 518,
        linear_elastic_isotropic_3d_LT = 519,
        NonlinearIsotropic3DLT = 520,
        sanisand2008 = 521,
        CamClay = 522,
        sanisand2004 = 523,
        sanisand2004_legacy = 524,
        roundedMohrCoulomb = 525,
        linear_elastic_crossanisotropic = 526,
        uniaxial_concrete02 = 527,
        uniaxial_elastic_1d = 528,
        uniaxial_steel01 = 529,
        uniaxial_steel02 = 530,
        pisano = 531,
        PisanoLT = 532,
        CamClayLT = 533,
        VonMisesLT = 534,
        VonMisesArmstrongFrederickLT = 535,
        DruckerPragerLT = 536,
        DruckerPragerNonAssociateLinearHardeningLT = 537,
        DruckerPragerVonMisesLT = 538,
        DruckerPragerArmstrongFrederickLE = 539,
        DruckerPragerArmstrongFrederickNE = 540,
        DruckerPragerNonAssociateArmstrongFrederickLT = 541,
        mass_density = 542,
        elastic_modulus = 543,
        viscoelastic_modulus = 544,
        poisson_ratio = 545,
        von_mises_radius = 546,
        druckerprager_angle = 547,
        druckerprager_k = 548,
        armstrong_frederick_ha = 549,
        armstrong_frederick_cr = 550,
        initial_confining_stress = 551,
        isotropic_hardening_rate = 552,
        kinematic_hardening_rate = 553,
        poisson_ratio_h_v = 554,
        poisson_ratio_h_h = 555,
        shear_modulus_h_v = 556,
        elastic_modulus_horizontal = 557,
        elastic_modulus_vertical = 558,
        pressure_reference_p0 = 559,
        plastic_flow_kd = 560,
        plastic_flow_xi = 561,
        initial_void_ratio = 562,
        initial_shear_modulus = 563,
        initial_bulk_modulus = 564,
        sanisand2008_Pat = 565,
        sanisand2008_k_c = 566,
        sanisand2008_K0 = 567,
        sanisand2008_alpha_cc = 568,
        sanisand2008_c = 569,
        sanisand2008_xi = 570,
        sanisand2008_lambda = 571,
        sanisand2008_ec_ref = 572,
        sanisand2008_m = 573,
        sanisand2008_h0 = 574,
        sanisand2008_ch = 575,
        sanisand2008_nb = 576,
        sanisand2008_A0 = 577,
        sanisand2008_nd = 578,
        sanisand2008_p_r = 579,
        sanisand2008_rho_c = 580,
        sanisand2008_theta_c = 581,
        sanisand2008_X = 582,
        sanisand2008_z_max = 583,
        sanisand2008_cz = 584,
        sanisand2008_p0 = 585,
        sanisand2008_p_in = 586,
        sanisand2008_G0 = 587,
        sanisand2004_K0 = 588,
        sanisand2004_Pat = 589,
        e0 = 590,
        sanisand2004_G0 = 591,
        sanisand2004_p_cut = 592,
        sanisand2004_Mc = 593,
        sanisand2004_c = 594,
        sanisand2004_lambda_c = 595,
        sanisand2004_xi = 596,
        sanisand2004_ec_ref = 597,
        sanisand2004_m = 598,
        sanisand2004_h0 = 599,
        sanisand2004_ch = 600,
        sanisand2004_nb = 601,
        sanisand2004_A0 = 602,
        sanisand2004_nd = 603,
        sanisand2004_z_max = 604,
        sanisand2004_cz = 605,
        reference_void_ratio = 606,
        critical_stress_ratio_M = 607,
        minimum_bulk_modulus = 608,
        initial_mean_pressure = 609,
        yield_strength = 610,
        strain_hardening_ratio = 611,
        compressive_strength = 612,
        strain_at_compressive_strength = 613,
        crushing_strength = 614,
        strain_at_crushing_strength = 615,
        tensile_strength = 616,
        tension_softening_stiffness = 617,
        M_in = 618,
        kd_in = 619,
        xi_in = 620,
        h_in = 621,
        m_in = 622,
        beta_min = 623,
        n_in = 624,
        a_in = 625,
        elastic_modulus_1atm = 626,
        eplcum_cr_in = 627,
        Niso3d_K = 628,
        Niso3d_Kur = 629,
        Niso3d_n = 630,
        Niso3d_c = 631,
        Niso3d_phi0 = 632,
        Niso3d_dphi = 633,
        Niso3d_Rf = 634,
        Niso3d_K0 = 635,
        Niso3d_Kb = 636,
        Niso3d_m = 637,
        Niso3d_pa = 638,
        Niso3d_K2 = 639,
        Niso3d_B = 640,
        Niso3d_Et = 641,
        Niso3d_Ei = 642,
        Niso3d_Er = 643,
        CriticalState_M = 644,
        CriticalState_lambda = 645,
        CriticalState_kappa = 646,
        CriticalState_e0 = 647,
        CriticalState_p0 = 648,
        RMC_m = 649,
        RMC_qa = 650,
        RMC_pc = 651,
        RMC_e = 652,
        RMC_eta0 = 653,
        RMC_Heta = 654,
        DuncanCheng_K = 655,
        DuncanCheng_pa = 656,
        DuncanCheng_n = 657,
        DuncanCheng_sigma3_max = 658,
        DuncanCheng_nu = 659,
        ax = 660,
        ay = 661,
        az = 662,
        verbose_level = 663,
        maximum_iterations = 664,
        tolerance = 665,
        yield_function_relative_tolerance = 666,
        stress_relative_tolerance = 667,
        verbose_output = 668,
        beta = 669,
        gamma = 670,
        kappa = 671,
        lambda = 672,
        delta = 673,
        sigma0 = 674,
        DOMAIN_ = 675,
        startTime = 676,
        endTime = 677,
        Period = 678,
        Phase = 679,
        Amplitude = 680,
        frequency = 681,
        MaxTime = 682,
        frequency1 = 683,
        frequency2 = 684,
        frequency3 = 685,
        frequency4 = 686,
        stiffness_to_use_opt = 687,
        DAMPING_RAYLEIGH = 688,
        DAMPING_CAUGHEY3 = 689,
        DAMPING_CAUGHEY4 = 690,
        a0 = 691,
        a1 = 692,
        stiffness_to_use = 693,
        a2 = 694,
        a3 = 695,
        a4 = 696,
        R0 = 697,
        cR1 = 698,
        cR2 = 699,
        MASS = 700,
        mx = 701,
        my = 702,
        mz = 703,
        Imx = 704,
        Imy = 705,
        Imz = 706,
        equal_dof = 707,
        master = 708,
        slave = 709,
        dof_to_constrain = 710,
        of = 711,
        OUTPUT = 712,
        BINARY = 713,
        TEXT = 714,
        ENABLE = 715,
        DISABLE = 716,
        COMPRESSION = 717,
        SAVE = 718,
        NON_CONVERGED_ITERATIONS = 719
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
      yylast_ = 9761,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 120, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 482  ///< Number of tokens.
    };


  };



} // yy
#line 919 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
