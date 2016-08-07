// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

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


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
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
#line 114 "feiparser.tab.hh" // lalr1.cc:377





  /// A Bison parser.
  class feiparser
  {
  public:
#ifndef YYSTYPE
    /// Symbol semantic values.
    union semantic_type
    {
    #line 143 "feiparser.yy" // lalr1.cc:377

  Expression    *exp;           /* For the expressions. Since it is a pointer, no problem. */
  Quantity      *value;         /* For the lexical analyser. NUMBER tokens */
  string        *ident;         /* For the lexical analyser. IDENT tokens */
  int           fn;
  int           errcode;

#line 136 "feiparser.tab.hh" // lalr1.cc:377
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
        LOADING = 377,
        STAGE = 378,
        STEPS = 379,
        TYPE = 380,
        DOFS = 381,
        FACTOR = 382,
        INCREMENT = 383,
        TH_GROUNDMOTION = 384,
        TH_LINEAR = 385,
        TH_PATH_SERIES = 386,
        TH_PATH_TIME_SERIES = 387,
        TH_CONSTANT = 388,
        TH_FROM_REACTIONS = 389,
        self_weight = 390,
        surface = 391,
        load_value = 392,
        scale_factor = 393,
        displacement_scale_unit = 394,
        velocity_scale_unit = 395,
        acceleration_scale_unit = 396,
        number_of_steps = 397,
        number_of_boundary_nodes = 398,
        number_of_exterior_nodes = 399,
        number_of_drm_elements = 400,
        element_file = 401,
        boundary_nodes_file = 402,
        exterior_nodes_file = 403,
        displacement_file = 404,
        acceleration_file = 405,
        velocity_file = 406,
        force_file = 407,
        hdf5_file = 408,
        series_file = 409,
        time_series_file = 410,
        MAGNITUDES = 411,
        MAGNITUDE = 412,
        initial_velocity = 413,
        strain_increment_size = 414,
        maximum_strain = 415,
        number_of_times_reaching_maximum_strain = 416,
        constitutive = 417,
        testing = 418,
        constant = 419,
        mean = 420,
        triaxial = 421,
        drained = 422,
        undrained = 423,
        simple = 424,
        shear = 425,
        number_of_subincrements = 426,
        maximum_number_of_iterations = 427,
        tolerance_1 = 428,
        tolerance_2 = 429,
        strain = 430,
        stress = 431,
        control = 432,
        Guass = 433,
        points = 434,
        Gauss = 435,
        each = 436,
        point = 437,
        single = 438,
        value = 439,
        initial_normal_stiffness = 440,
        stiffning_rate = 441,
        number_of_integration_points = 442,
        EightNodeBrick = 443,
        TwentySevenNodeBrick = 444,
        EightNodeBrick_upU = 445,
        TwentyNodeBrick_uPU = 446,
        TwentyNodeBrick = 447,
        TwentyNodeBrickElastic = 448,
        EightNodeBrick_up = 449,
        variable_node_brick_8_to_27 = 450,
        EightNodeBrickElastic = 451,
        TwentySevenNodeBrickElastic = 452,
        beam_displacement_based = 453,
        BeamColumnDispFiber3d = 454,
        beam_elastic = 455,
        beam_elastic_lumped_mass = 456,
        beam_9dof_elastic = 457,
        FourNodeShellMITC4 = 458,
        FourNodeShellNewMITC4 = 459,
        ThreeNodeShellANDES = 460,
        FourNodeShellANDES = 461,
        truss = 462,
        contact = 463,
        HardContact = 464,
        FrictionalPenaltyContact = 465,
        SoftContact = 466,
        EightNodeBrickLT = 467,
        EightNodeBrickLTNoOutput = 468,
        TwentyNodeBrickLT = 469,
        TwentySevenNodeBrickLT = 470,
        ShearBeamLT = 471,
        porosity = 472,
        alpha = 473,
        rho_s = 474,
        rho_f = 475,
        k_x = 476,
        k_y = 477,
        k_z = 478,
        K_s = 479,
        K_f = 480,
        pressure = 481,
        cross_section = 482,
        shear_modulus = 483,
        torsion_Jx = 484,
        bending_Iz = 485,
        bending_Iy = 486,
        IntegrationRule = 487,
        stiffness = 488,
        normal_stiffness = 489,
        tangential_stiffness = 490,
        normal_damping = 491,
        tangential_damping = 492,
        friction_ratio = 493,
        maximum_gap = 494,
        xz_plane_vector = 495,
        joint_1_offset = 496,
        joint_2_offset = 497,
        direction = 498,
        contact_plane_vector = 499,
        power = 500,
        linear = 501,
        exponential = 502,
        factorial = 503,
        inverse = 504,
        MembranePlateFiber = 505,
        ElasticMembranePlate = 506,
        elastic3d = 507,
        FIBER = 508,
        FiberSection = 509,
        Section = 510,
        fiber_cross_section = 511,
        fiber_location_Y = 512,
        fiber_location_Z = 513,
        fiber_location = 514,
        TorsionConstant_GJ = 515,
        thickness = 516,
        integration_rule = 517,
        section_number = 518,
        NDMaterialLT = 519,
        linear_elastic_isotropic_3d = 520,
        linear_elastic_isotropic_3d_LT = 521,
        NonlinearIsotropic3DLT = 522,
        sanisand2008 = 523,
        camclay = 524,
        camclay_accelerated = 525,
        sanisand2004 = 526,
        linear_elastic_crossanisotropic = 527,
        uniaxial_concrete02 = 528,
        uniaxial_elastic_1d = 529,
        uniaxial_steel01 = 530,
        uniaxial_steel02 = 531,
        pisano = 532,
        PisanoLT = 533,
        VonMisesLT = 534,
        DruckerPragerLT = 535,
        DruckerPragerVonMisesLT = 536,
        DruckerPragerArmstrongFrederickLT = 537,
        mass_density = 538,
        elastic_modulus = 539,
        viscoelastic_modulus = 540,
        poisson_ratio = 541,
        von_mises_radius = 542,
        druckerprager_angle = 543,
        druckerprager_k = 544,
        armstrong_frederick_ha = 545,
        armstrong_frederick_cr = 546,
        initial_confining_stress = 547,
        isotropic_hardening_rate = 548,
        kinematic_hardening_rate = 549,
        poisson_ratio_h_v = 550,
        poisson_ratio_h_h = 551,
        shear_modulus_h_v = 552,
        elastic_modulus_horizontal = 553,
        elastic_modulus_vertical = 554,
        pressure_reference_p0 = 555,
        initial_void_ratio = 556,
        initial_shear_modulus = 557,
        initial_bulk_modulus = 558,
        sanisand2008_Pat = 559,
        sanisand2008_k_c = 560,
        sanisand2008_K0 = 561,
        sanisand2008_alpha_cc = 562,
        sanisand2008_c = 563,
        sanisand2008_xi = 564,
        sanisand2008_lambda = 565,
        sanisand2008_ec_ref = 566,
        sanisand2008_m = 567,
        sanisand2008_h0 = 568,
        sanisand2008_ch = 569,
        sanisand2008_nb = 570,
        sanisand2008_A0 = 571,
        sanisand2008_nd = 572,
        sanisand2008_p_r = 573,
        sanisand2008_rho_c = 574,
        sanisand2008_theta_c = 575,
        sanisand2008_X = 576,
        sanisand2008_z_max = 577,
        sanisand2008_cz = 578,
        sanisand2008_p0 = 579,
        sanisand2008_p_in = 580,
        sanisand2008_G0 = 581,
        sanisand2004_K0 = 582,
        sanisand2004_Pat = 583,
        e0 = 584,
        sanisand2004_G0 = 585,
        sanisand2004_p_cut = 586,
        sanisand2004_Mc = 587,
        sanisand2004_c = 588,
        sanisand2004_lambda_c = 589,
        sanisand2004_xi = 590,
        sanisand2004_ec_ref = 591,
        sanisand2004_m = 592,
        sanisand2004_h0 = 593,
        sanisand2004_ch = 594,
        sanisand2004_nb = 595,
        sanisand2004_A0 = 596,
        sanisand2004_nd = 597,
        sanisand2004_z_max = 598,
        sanisand2004_cz = 599,
        reference_void_ratio = 600,
        critical_stress_ratio_M = 601,
        minimum_bulk_modulus = 602,
        initial_mean_pressure = 603,
        yield_strength = 604,
        strain_hardening_ratio = 605,
        compressive_strength = 606,
        strain_at_compressive_strength = 607,
        crushing_strength = 608,
        strain_at_crushing_strength = 609,
        tensile_strength = 610,
        tension_softening_stiffness = 611,
        M_in = 612,
        kd_in = 613,
        xi_in = 614,
        h_in = 615,
        m_in = 616,
        beta_min = 617,
        n_in = 618,
        a_in = 619,
        elastic_modulus_1atm = 620,
        eplcum_cr_in = 621,
        Niso3d_K = 622,
        Niso3d_Kur = 623,
        Niso3d_n = 624,
        Niso3d_c = 625,
        Niso3d_phi0 = 626,
        Niso3d_dphi = 627,
        Niso3d_Rf = 628,
        Niso3d_K0 = 629,
        Niso3d_Kb = 630,
        Niso3d_m = 631,
        Niso3d_pa = 632,
        Niso3d_K2 = 633,
        Niso3d_B = 634,
        Niso3d_Et = 635,
        Niso3d_Ei = 636,
        Niso3d_Er = 637,
        ax = 638,
        ay = 639,
        az = 640,
        verbose_level = 641,
        maximum_iterations = 642,
        tolerance = 643,
        yield_function_relative_tolerance = 644,
        stress_relative_tolerance = 645,
        beta = 646,
        gamma = 647,
        kappa = 648,
        lambda = 649,
        delta = 650,
        DOMAIN_ = 651,
        startTime = 652,
        endTime = 653,
        Period = 654,
        Phase = 655,
        Amplitude = 656,
        frequency = 657,
        MaxTime = 658,
        frequency1 = 659,
        frequency2 = 660,
        frequency3 = 661,
        frequency4 = 662,
        stiffness_to_use_opt = 663,
        DAMPING_RAYLEIGH = 664,
        DAMPING_CAUGHEY3 = 665,
        DAMPING_CAUGHEY4 = 666,
        a0 = 667,
        a1 = 668,
        stiffness_to_use = 669,
        a2 = 670,
        a3 = 671,
        a4 = 672,
        R0 = 673,
        cR1 = 674,
        cR2 = 675,
        MASS = 676,
        mx = 677,
        my = 678,
        mz = 679,
        Imx = 680,
        Imy = 681,
        Imz = 682,
        equaldof = 683,
        master = 684,
        slave = 685,
        dof_to_constrain = 686,
        of = 687,
        OUTPUT = 688,
        BINARY = 689,
        TEXT = 690,
        ENABLE = 691,
        DISABLE = 692,
        COMPRESSION = 693,
        SAVE = 694,
        NON_CONVERGED_SUBSTEPS = 695
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

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

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

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

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
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
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

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

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
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

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 9246,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyfinal_ = 123, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 458  ///< Number of tokens.
    };


  };



} // yy
#line 912 "feiparser.tab.hh" // lalr1.cc:377




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
