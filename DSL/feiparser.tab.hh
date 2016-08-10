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
        EightNodeBrick = 440,
        TwentySevenNodeBrick = 441,
        EightNodeBrick_upU = 442,
        TwentyNodeBrick_uPU = 443,
        TwentyNodeBrick = 444,
        TwentyNodeBrickElastic = 445,
        EightNodeBrick_up = 446,
        variable_node_brick_8_to_27 = 447,
        EightNodeBrickElastic = 448,
        TwentySevenNodeBrickElastic = 449,
        beam_displacement_based = 450,
        BeamColumnDispFiber3d = 451,
        beam_elastic = 452,
        beam_elastic_lumped_mass = 453,
        beam_9dof_elastic = 454,
        FourNodeShellMITC4 = 455,
        FourNodeShellNewMITC4 = 456,
        ThreeNodeShellANDES = 457,
        FourNodeShellANDES = 458,
        truss = 459,
        contact = 460,
        FrictionalPenaltyContact = 461,
        EightNodeBrickLT = 462,
        EightNodeBrickLTNoOutput = 463,
        TwentyNodeBrickLT = 464,
        TwentySevenNodeBrickLT = 465,
        ShearBeamLT = 466,
        porosity = 467,
        alpha = 468,
        rho_s = 469,
        rho_f = 470,
        k_x = 471,
        k_y = 472,
        k_z = 473,
        K_s = 474,
        K_f = 475,
        pressure = 476,
        cross_section = 477,
        shear_modulus = 478,
        torsion_Jx = 479,
        bending_Iz = 480,
        bending_Iy = 481,
        IntegrationRule = 482,
        number_of_integration_points = 483,
        stiffness = 484,
        normal_stiffness = 485,
        tangential_stiffness = 486,
        normal_damping = 487,
        tangential_damping = 488,
        friction_ratio = 489,
        maximum_gap = 490,
        xz_plane_vector = 491,
        joint_1_offset = 492,
        joint_2_offset = 493,
        direction = 494,
        contact_plane_vector = 495,
        MembranePlateFiber = 496,
        ElasticMembranePlate = 497,
        elastic3d = 498,
        FIBER = 499,
        FiberSection = 500,
        Section = 501,
        fiber_cross_section = 502,
        fiber_location_Y = 503,
        fiber_location_Z = 504,
        fiber_location = 505,
        TorsionConstant_GJ = 506,
        thickness = 507,
        integration_rule = 508,
        section_number = 509,
        NDMaterialLT = 510,
        linear_elastic_isotropic_3d = 511,
        linear_elastic_isotropic_3d_LT = 512,
        NonlinearIsotropic3DLT = 513,
        sanisand2008 = 514,
        camclay = 515,
        camclay_accelerated = 516,
        sanisand2004 = 517,
        linear_elastic_crossanisotropic = 518,
        uniaxial_concrete02 = 519,
        uniaxial_elastic_1d = 520,
        uniaxial_steel01 = 521,
        uniaxial_steel02 = 522,
        pisano = 523,
        PisanoLT = 524,
        VonMisesLT = 525,
        VonMisesArmstrongFrederickLT = 526,
        DruckerPragerLT = 527,
        DruckerPragerNonAssociateLinearHardeningLT = 528,
        DruckerPragerVonMisesLT = 529,
        DruckerPragerArmstrongFrederickLT = 530,
        DruckerPragerNonAssociateArmstrongFrederickLT = 531,
        mass_density = 532,
        elastic_modulus = 533,
        viscoelastic_modulus = 534,
        poisson_ratio = 535,
        von_mises_radius = 536,
        druckerprager_angle = 537,
        druckerprager_k = 538,
        armstrong_frederick_ha = 539,
        armstrong_frederick_cr = 540,
        initial_confining_stress = 541,
        isotropic_hardening_rate = 542,
        kinematic_hardening_rate = 543,
        poisson_ratio_h_v = 544,
        poisson_ratio_h_h = 545,
        shear_modulus_h_v = 546,
        elastic_modulus_horizontal = 547,
        elastic_modulus_vertical = 548,
        pressure_reference_p0 = 549,
        plastic_flow_kd = 550,
        plastic_flow_xi = 551,
        initial_void_ratio = 552,
        initial_shear_modulus = 553,
        initial_bulk_modulus = 554,
        sanisand2008_Pat = 555,
        sanisand2008_k_c = 556,
        sanisand2008_K0 = 557,
        sanisand2008_alpha_cc = 558,
        sanisand2008_c = 559,
        sanisand2008_xi = 560,
        sanisand2008_lambda = 561,
        sanisand2008_ec_ref = 562,
        sanisand2008_m = 563,
        sanisand2008_h0 = 564,
        sanisand2008_ch = 565,
        sanisand2008_nb = 566,
        sanisand2008_A0 = 567,
        sanisand2008_nd = 568,
        sanisand2008_p_r = 569,
        sanisand2008_rho_c = 570,
        sanisand2008_theta_c = 571,
        sanisand2008_X = 572,
        sanisand2008_z_max = 573,
        sanisand2008_cz = 574,
        sanisand2008_p0 = 575,
        sanisand2008_p_in = 576,
        sanisand2008_G0 = 577,
        sanisand2004_K0 = 578,
        sanisand2004_Pat = 579,
        e0 = 580,
        sanisand2004_G0 = 581,
        sanisand2004_p_cut = 582,
        sanisand2004_Mc = 583,
        sanisand2004_c = 584,
        sanisand2004_lambda_c = 585,
        sanisand2004_xi = 586,
        sanisand2004_ec_ref = 587,
        sanisand2004_m = 588,
        sanisand2004_h0 = 589,
        sanisand2004_ch = 590,
        sanisand2004_nb = 591,
        sanisand2004_A0 = 592,
        sanisand2004_nd = 593,
        sanisand2004_z_max = 594,
        sanisand2004_cz = 595,
        reference_void_ratio = 596,
        critical_stress_ratio_M = 597,
        minimum_bulk_modulus = 598,
        initial_mean_pressure = 599,
        yield_strength = 600,
        strain_hardening_ratio = 601,
        compressive_strength = 602,
        strain_at_compressive_strength = 603,
        crushing_strength = 604,
        strain_at_crushing_strength = 605,
        tensile_strength = 606,
        tension_softening_stiffness = 607,
        M_in = 608,
        kd_in = 609,
        xi_in = 610,
        h_in = 611,
        m_in = 612,
        beta_min = 613,
        n_in = 614,
        a_in = 615,
        elastic_modulus_1atm = 616,
        eplcum_cr_in = 617,
        Niso3d_K = 618,
        Niso3d_Kur = 619,
        Niso3d_n = 620,
        Niso3d_c = 621,
        Niso3d_phi0 = 622,
        Niso3d_dphi = 623,
        Niso3d_Rf = 624,
        Niso3d_K0 = 625,
        Niso3d_Kb = 626,
        Niso3d_m = 627,
        Niso3d_pa = 628,
        Niso3d_K2 = 629,
        Niso3d_B = 630,
        Niso3d_Et = 631,
        Niso3d_Ei = 632,
        Niso3d_Er = 633,
        ax = 634,
        ay = 635,
        az = 636,
        verbose_level = 637,
        maximum_iterations = 638,
        tolerance = 639,
        yield_function_relative_tolerance = 640,
        stress_relative_tolerance = 641,
        beta = 642,
        gamma = 643,
        kappa = 644,
        lambda = 645,
        delta = 646,
        DOMAIN_ = 647,
        startTime = 648,
        endTime = 649,
        Period = 650,
        Phase = 651,
        Amplitude = 652,
        frequency = 653,
        MaxTime = 654,
        frequency1 = 655,
        frequency2 = 656,
        frequency3 = 657,
        frequency4 = 658,
        stiffness_to_use_opt = 659,
        DAMPING_RAYLEIGH = 660,
        DAMPING_CAUGHEY3 = 661,
        DAMPING_CAUGHEY4 = 662,
        a0 = 663,
        a1 = 664,
        stiffness_to_use = 665,
        a2 = 666,
        a3 = 667,
        a4 = 668,
        R0 = 669,
        cR1 = 670,
        cR2 = 671,
        MASS = 672,
        mx = 673,
        my = 674,
        mz = 675,
        Imx = 676,
        Imy = 677,
        Imz = 678,
        equaldof = 679,
        master = 680,
        slave = 681,
        dof_to_constrain = 682,
        of = 683,
        OUTPUT = 684,
        BINARY = 685,
        TEXT = 686,
        ENABLE = 687,
        DISABLE = 688,
        COMPRESSION = 689
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
      yylast_ = 9437,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 121, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 452  ///< Number of tokens.
    };


  };



} // yy
#line 889 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
