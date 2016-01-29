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
        DAMPING = 309,
        DAMPINGTYPE = 310,
        CONSTRAINT = 311,
        DRM = 312,
        SECTION = 313,
        LOADPATTERN = 314,
        PENALTYDISPLACEMENT = 315,
        LOADVALUE = 316,
        SET = 317,
        REACTION = 318,
        FORCES = 319,
        ELEMENTNAME = 320,
        MATERIALNAME = 321,
        ACCELERATION_FIELD = 322,
        FIX = 323,
        FREE = 324,
        REMOVE = 325,
        DEFINE = 326,
        ALGORITHM = 327,
        ALGNAME = 328,
        CONSTITUTIVE_ALGNAME = 329,
        CONVERGENCE_TEST = 330,
        TESTNAME = 331,
        SOLVER = 332,
        SOLVERNAME = 333,
        CONSTITUTIVE = 334,
        INTEGRATION = 335,
        DYNAMICINTEGRATOR = 336,
        DYNAMICINTEGRATOR_HHT = 337,
        DYNAMICINTEGRATOR_NEWMARK = 338,
        STATICINTEGRATOR = 339,
        STATICINTEGRATOR_DISPLACEMENT = 340,
        SIMULATE = 341,
        COMPUTE = 342,
        STATIC = 343,
        DYNAMIC = 344,
        USING = 345,
        TRANSIENT = 346,
        EIGEN = 347,
        time_step = 348,
        number_of_modes = 349,
        VARIABLETRANSIENT = 350,
        maximum_time_step = 351,
        minimum_time_step = 352,
        number_of_iterations = 353,
        AT = 354,
        ALL = 355,
        AND = 356,
        WITH = 357,
        TEXTDOFS = 358,
        NEW = 359,
        TEXTNUMBER = 360,
        USE = 361,
        TO = 362,
        DOF = 363,
        TEXTWITH = 364,
        NODES = 365,
        FORCE = 366,
        INTEGRATIONPOINTS = 367,
        dof = 368,
        RESPONSE = 369,
        FILE = 370,
        FROM = 371,
        EVERY = 372,
        LEVEL = 373,
        LOADING = 374,
        STAGE = 375,
        STEPS = 376,
        TYPE = 377,
        DOFS = 378,
        FACTOR = 379,
        INCREMENT = 380,
        TH_GROUNDMOTION = 381,
        TH_LINEAR = 382,
        TH_PATH_SERIES = 383,
        TH_PATH_TIME_SERIES = 384,
        TH_CONSTANT = 385,
        TH_FROM_REACTIONS = 386,
        self_weight = 387,
        surface = 388,
        load_value = 389,
        scale_factor = 390,
        displacement_scale_unit = 391,
        velocity_scale_unit = 392,
        acceleration_scale_unit = 393,
        number_of_steps = 394,
        number_of_boundary_nodes = 395,
        number_of_exterior_nodes = 396,
        number_of_drm_elements = 397,
        element_file = 398,
        boundary_nodes_file = 399,
        exterior_nodes_file = 400,
        displacement_file = 401,
        acceleration_file = 402,
        velocity_file = 403,
        force_file = 404,
        hdf5_file = 405,
        series_file = 406,
        time_series_file = 407,
        MAGNITUDES = 408,
        MAGNITUDE = 409,
        strain_increment_size = 410,
        maximum_strain = 411,
        number_of_times_reaching_maximum_strain = 412,
        testing = 413,
        constant = 414,
        mean = 415,
        triaxial = 416,
        drained = 417,
        undrained = 418,
        simple = 419,
        shear = 420,
        number_of_subincrements = 421,
        maximum_number_of_iterations = 422,
        tolerance_1 = 423,
        tolerance_2 = 424,
        strain = 425,
        stress = 426,
        control = 427,
        Guass = 428,
        points = 429,
        Gauss = 430,
        each = 431,
        point = 432,
        single = 433,
        value = 434,
        EightNodeBrick = 435,
        TwentySevenNodeBrick = 436,
        EightNodeBrick_upU = 437,
        TwentyNodeBrick_uPU = 438,
        TwentyNodeBrick = 439,
        TwentyNodeBrickElastic = 440,
        EightNodeBrick_up = 441,
        variable_node_brick_8_to_27 = 442,
        EightNodeBrickElastic = 443,
        TwentySevenNodeBrickElastic = 444,
        beam_displacement_based = 445,
        beam_elastic = 446,
        beam_elastic_lumped_mass = 447,
        beam_9dof_elastic = 448,
        FourNodeShellMITC4 = 449,
        FourNodeShellNewMITC4 = 450,
        ThreeNodeShellANDES = 451,
        FourNodeShellANDES = 452,
        truss = 453,
        contact = 454,
        FrictionalPenaltyContact = 455,
        EightNodeBrickLT = 456,
        EightNodeBrickLTNoOutput = 457,
        TwentySevenNodeBrickLT = 458,
        ShearBeamLT = 459,
        porosity = 460,
        alpha = 461,
        rho_s = 462,
        rho_f = 463,
        k_x = 464,
        k_y = 465,
        k_z = 466,
        K_s = 467,
        K_f = 468,
        pressure = 469,
        cross_section = 470,
        shear_modulus = 471,
        torsion_Jx = 472,
        bending_Iz = 473,
        bending_Iy = 474,
        IntegrationRule = 475,
        number_of_integration_points = 476,
        stiffness = 477,
        normal_stiffness = 478,
        tangential_stiffness = 479,
        normal_damping = 480,
        tangential_damping = 481,
        friction_ratio = 482,
        maximum_gap = 483,
        xz_plane_vector = 484,
        joint_1_offset = 485,
        joint_2_offset = 486,
        direction = 487,
        contact_plane_vector = 488,
        MembranePlateFiber = 489,
        ElasticMembranePlate = 490,
        thickness = 491,
        NDMaterialLT = 492,
        linear_elastic_isotropic_3d = 493,
        linear_elastic_isotropic_3d_LT = 494,
        sanisand2008 = 495,
        camclay = 496,
        camclay_accelerated = 497,
        sanisand2004 = 498,
        linear_elastic_crossanisotropic = 499,
        uniaxial_concrete02 = 500,
        uniaxial_elastic_1d = 501,
        uniaxial_steel01 = 502,
        uniaxial_steel02 = 503,
        pisano = 504,
        PisanoLT = 505,
        VonMisesLT = 506,
        DruckerPragerLT = 507,
        DruckerPragerVonMisesLT = 508,
        mass_density = 509,
        elastic_modulus = 510,
        viscoelastic_modulus = 511,
        poisson_ratio = 512,
        von_mises_radius = 513,
        druckerprager_angle = 514,
        druckerprager_k = 515,
        armstrong_frederick_ha = 516,
        armstrong_frederick_cr = 517,
        initial_confining_stress = 518,
        isotropic_hardening_rate = 519,
        kinematic_hardening_rate = 520,
        poisson_ratio_h_v = 521,
        poisson_ratio_h_h = 522,
        shear_modulus_h_v = 523,
        elastic_modulus_horizontal = 524,
        elastic_modulus_vertical = 525,
        pressure_reference_p0 = 526,
        initial_void_ratio = 527,
        initial_shear_modulus = 528,
        initial_bulk_modulus = 529,
        sanisand2008_Pat = 530,
        sanisand2008_k_c = 531,
        sanisand2008_K0 = 532,
        sanisand2008_alpha_cc = 533,
        sanisand2008_c = 534,
        sanisand2008_xi = 535,
        sanisand2008_lambda = 536,
        sanisand2008_ec_ref = 537,
        sanisand2008_m = 538,
        sanisand2008_h0 = 539,
        sanisand2008_ch = 540,
        sanisand2008_nb = 541,
        sanisand2008_A0 = 542,
        sanisand2008_nd = 543,
        sanisand2008_p_r = 544,
        sanisand2008_rho_c = 545,
        sanisand2008_theta_c = 546,
        sanisand2008_X = 547,
        sanisand2008_z_max = 548,
        sanisand2008_cz = 549,
        sanisand2008_p0 = 550,
        sanisand2008_p_in = 551,
        sanisand2008_G0 = 552,
        sanisand2004_K0 = 553,
        sanisand2004_Pat = 554,
        e0 = 555,
        sanisand2004_G0 = 556,
        sanisand2004_p_cut = 557,
        sanisand2004_Mc = 558,
        sanisand2004_c = 559,
        sanisand2004_lambda_c = 560,
        sanisand2004_xi = 561,
        sanisand2004_ec_ref = 562,
        sanisand2004_m = 563,
        sanisand2004_h0 = 564,
        sanisand2004_ch = 565,
        sanisand2004_nb = 566,
        sanisand2004_A0 = 567,
        sanisand2004_nd = 568,
        sanisand2004_z_max = 569,
        sanisand2004_cz = 570,
        reference_void_ratio = 571,
        critical_stress_ratio_M = 572,
        minimum_bulk_modulus = 573,
        initial_mean_pressure = 574,
        yield_strength = 575,
        strain_hardening_ratio = 576,
        compressive_strength = 577,
        strain_at_compressive_strength = 578,
        crushing_strength = 579,
        strain_at_crushing_strength = 580,
        tensile_strength = 581,
        tension_softening_stiffness = 582,
        M_in = 583,
        kd_in = 584,
        xi_in = 585,
        h_in = 586,
        m_in = 587,
        beta_min = 588,
        n_in = 589,
        a_in = 590,
        elastic_modulus_1atm = 591,
        eplcum_cr_in = 592,
        ax = 593,
        ay = 594,
        az = 595,
        verbose_level = 596,
        maximum_iterations = 597,
        tolerance = 598,
        yield_function_relative_tolerance = 599,
        stress_relative_tolerance = 600,
        beta = 601,
        gamma = 602,
        kappa = 603,
        lambda = 604,
        delta = 605,
        DOMAIN_ = 606,
        startTime = 607,
        endTime = 608,
        Period = 609,
        Phase = 610,
        Amplitude = 611,
        frequency = 612,
        MaxTime = 613,
        frequency1 = 614,
        frequency2 = 615,
        frequency3 = 616,
        frequency4 = 617,
        stiffness_to_use_opt = 618,
        DAMPING_RAYLEIGH = 619,
        DAMPING_CAUGHEY3 = 620,
        DAMPING_CAUGHEY4 = 621,
        a0 = 622,
        a1 = 623,
        stiffness_to_use = 624,
        a2 = 625,
        a3 = 626,
        a4 = 627,
        R0 = 628,
        cR1 = 629,
        cR2 = 630,
        MASS = 631,
        mx = 632,
        my = 633,
        mz = 634,
        Imx = 635,
        Imy = 636,
        Imz = 637,
        equaldof = 638,
        master = 639,
        slave = 640,
        dof_to_constrain = 641,
        of = 642,
        OUTPUT = 643,
        BINARY = 644,
        TEXT = 645,
        ENABLE = 646,
        DISABLE = 647,
        COMPRESSION = 648
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
      yylast_ = 8165,     ///< Last index in yytable_.
      yynnts_ = 16,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 116, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 411  ///< Number of tokens.
    };


  };



} // yy
#line 848 "feiparser.tab.hh" // lalr1.cc:372




#endif // !YY_YY_FEIPARSER_TAB_HH_INCLUDED
