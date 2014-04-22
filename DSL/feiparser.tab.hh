/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H



#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


namespace yy {

/* Line 35 of lalr1.cc  */
#line 68 "feiparser.tab.hh"

  /// A Bison parser.
  class feiparser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 109 "feiparser.yy"

  Expression    *exp;           /* For the expressions. Since it is a pointer, no problem. */
  Quantity      *value;         /* For the lexical analyser. NUMBER tokens */
  string        *ident;         /* For the lexical analyser. IDENT tokens */
  int           fn;
  int           errcode;



/* Line 35 of lalr1.cc  */
#line 91 "feiparser.tab.hh"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
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
     UMINUS = 287,
     CMP = 288,
     CMD_print = 289,
     CMD_help = 290,
     CMD_whos = 291,
     CMD_check = 292,
     CMD_save = 293,
     CMD_restore = 294,
     MODEL = 295,
     NAME = 296,
     RESTART = 297,
     MESH = 298,
     ADD = 299,
     NODE = 300,
     ELEMENT = 301,
     MATERIAL = 302,
     LOAD = 303,
     TIMEHISTORY = 304,
     IMPOSEDMOTION = 305,
     DAMPING = 306,
     DAMPINGTYPE = 307,
     CONSTRAINT = 308,
     DRM = 309,
     SAVEFORCES = 310,
     RESTOREFORCES = 311,
     SECTION = 312,
     LOADPATTERN = 313,
     PENALTYDISPLACEMENT = 314,
     LOADVALUE = 315,
     ELEMENTNAME = 316,
     MATERIALNAME = 317,
     ACCELERATION_FIELD = 318,
     FIX = 319,
     FREE = 320,
     REMOVE = 321,
     DEFINE = 322,
     ALGORITHM = 323,
     ALGNAME = 324,
     CONVERGENCE_TEST = 325,
     TESTNAME = 326,
     SOLVER = 327,
     SOLVERNAME = 328,
     DYNAMICINTEGRATOR = 329,
     DYNAMICINTEGRATOR_HHT = 330,
     DYNAMICINTEGRATOR_NEWMARK = 331,
     STATICINTEGRATOR = 332,
     STATICINTEGRATOR_DISPLACEMENT = 333,
     SIMULATE = 334,
     STATIC = 335,
     DYNAMIC = 336,
     USING = 337,
     TRANSIENT = 338,
     EIGEN = 339,
     time_step = 340,
     number_of_modes = 341,
     VARIABLETRANSIENT = 342,
     maximum_time_step = 343,
     minimum_time_step = 344,
     number_of_iterations = 345,
     AT = 346,
     ALL = 347,
     AND = 348,
     WITH = 349,
     TEXTDOFS = 350,
     NEW = 351,
     TEXTNUMBER = 352,
     USE = 353,
     TO = 354,
     DOF = 355,
     TEXTWITH = 356,
     NODES = 357,
     FORCE = 358,
     INTEGRATIONPOINTS = 359,
     dof = 360,
     RESPONSE = 361,
     LOADING = 362,
     STAGE = 363,
     STEPS = 364,
     TYPE = 365,
     DOFS = 366,
     FACTOR = 367,
     INCREMENT = 368,
     TH_GROUNDMOTION = 369,
     TH_LINEAR = 370,
     TH_PATH_SERIES = 371,
     TH_PATH_TIME_SERIES = 372,
     TH_CONSTANT = 373,
     TH_FROM_REACTIONS = 374,
     self_weight = 375,
     surface = 376,
     load_value = 377,
     scale_factor = 378,
     displacement_scale_unit = 379,
     velocity_scale_unit = 380,
     acceleration_scale_unit = 381,
     number_of_steps = 382,
     number_of_drm_nodes = 383,
     number_of_drm_elements = 384,
     x_max = 385,
     x_min = 386,
     y_max = 387,
     y_min = 388,
     z_max = 389,
     z_min = 390,
     element_file = 391,
     nodes_file = 392,
     displacement_file = 393,
     acceleration_file = 394,
     velocity_file = 395,
     force_file = 396,
     series_file = 397,
     time_series_file = 398,
     MAGNITUDES = 399,
     MAGNITUDE = 400,
     strain_increment_size = 401,
     maximum_strain = 402,
     number_of_times_reaching_maximum_strain = 403,
     constitutive = 404,
     testing = 405,
     constant = 406,
     mean = 407,
     triaxial = 408,
     drained = 409,
     undrained = 410,
     simple = 411,
     shear = 412,
     number_of_subincrements = 413,
     maximum_number_of_iterations = 414,
     tolerance_1 = 415,
     tolerance_2 = 416,
     strain = 417,
     stress = 418,
     control = 419,
     Guass = 420,
     points = 421,
     Gauss = 422,
     each = 423,
     point = 424,
     single = 425,
     value = 426,
     EightNodeBrick = 427,
     TwentySevenNodeBrick = 428,
     EightNodeBrick_upU = 429,
     TwentyNodeBrick_uPU = 430,
     TwentyNodeBrick = 431,
     TwentyNodeBrickElastic = 432,
     EightNodeBrick_up = 433,
     variable_node_brick_8_to_27 = 434,
     EightNodeBrickElastic = 435,
     TwentySevenNodeBrickElastic = 436,
     beam_displacement_based = 437,
     beam_elastic = 438,
     beam_elastic_lumped_mass = 439,
     penalty = 440,
     penalty_for_applying_generalized_displacement = 441,
     beam_9dof_elastic = 442,
     FourNodeShellMITC4 = 443,
     FourNodeShellNewMITC4 = 444,
     ThreeNodeShellANDES = 445,
     FourNodeShellANDES = 446,
     truss = 447,
     contact = 448,
     EightNodeBrickLT = 449,
     TwentySevenNodeBrickLT = 450,
     porosity = 451,
     alpha = 452,
     rho_s = 453,
     rho_f = 454,
     k_x = 455,
     k_y = 456,
     k_z = 457,
     K_s = 458,
     K_f = 459,
     pressure = 460,
     cross_section = 461,
     shear_modulus = 462,
     torsion_Jx = 463,
     bending_Iz = 464,
     bending_Iy = 465,
     IntegrationRule = 466,
     number_of_integration_points = 467,
     stiffness = 468,
     normal_stiffness = 469,
     tangential_stiffness = 470,
     friction_ratio = 471,
     maximum_gap = 472,
     xz_plane_vector = 473,
     joint_1_offset = 474,
     joint_2_offset = 475,
     direction = 476,
     contact_plane_vector = 477,
     MembranePlateFiber = 478,
     ElasticMembranePlate = 479,
     thickness = 480,
     linear_elastic_isotropic_3d = 481,
     vonmises_perfectly_plastic = 482,
     vonmises_isotropic_hardening = 483,
     vonmises_linear_kinematic_hardening = 484,
     vonmises_linear_kinematic_hardening_accelerated = 485,
     vonmises_kinematic_hardening = 486,
     vonmises_perfectly_plastic_accelerated = 487,
     vonmises_isotropic_hardening_accelerated = 488,
     vonmises_kinematic_hardening_accelerated = 489,
     sanisand2008 = 490,
     camclay = 491,
     camclay_accelerated = 492,
     sanisand2004 = 493,
     druckerprager_isotropic_hardening = 494,
     druckerprager_isotropic_hardening_accelerated = 495,
     druckerprager_kinematic_hardening = 496,
     druckerprager_kinematic_hardening_accelerated = 497,
     druckerprager_perfectly_plastic = 498,
     druckerprager_perfectly_plastic_accelerated = 499,
     linear_elastic_crossanisotropic = 500,
     uniaxial_concrete02 = 501,
     uniaxial_elastic_1d = 502,
     uniaxial_steel01 = 503,
     uniaxial_steel02 = 504,
     pisano = 505,
     vonmises_perfectly_plastic_LT = 506,
     pisanoLT = 507,
     New_PisanoLT = 508,
     linear_elastic_isotropic_3d_LT = 509,
     mass_density = 510,
     elastic_modulus = 511,
     viscoelastic_modulus = 512,
     poisson_ratio = 513,
     von_mises_radius = 514,
     druckerprager_angle = 515,
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
     ax = 589,
     ay = 590,
     az = 591,
     verbose_level = 592,
     maximum_iterations = 593,
     tolerance = 594,
     beta = 595,
     gamma = 596,
     kappa = 597,
     lambda = 598,
     delta = 599,
     DOMAIN_ = 600,
     startTime = 601,
     endTime = 602,
     Period = 603,
     Phase = 604,
     Amplitude = 605,
     frequency = 606,
     MaxTime = 607,
     frequency1 = 608,
     frequency2 = 609,
     frequency3 = 610,
     frequency4 = 611,
     stiffness_to_use_opt = 612,
     DAMPING_RAYLEIGH = 613,
     DAMPING_CAUGHEY3 = 614,
     DAMPING_CAUGHEY4 = 615,
     a0 = 616,
     a1 = 617,
     stiffness_to_use = 618,
     a2 = 619,
     a3 = 620,
     a4 = 621,
     R0 = 622,
     cR1 = 623,
     cR2 = 624,
     MASS = 625,
     mx = 626,
     my = 627,
     mz = 628,
     Imx = 629,
     Imy = 630,
     Imz = 631,
     equaldof = 632,
     master = 633,
     slave = 634,
     dof_to_constrain = 635,
     of = 636,
     OUTPUT = 637,
     BINARY = 638,
     TEXT = 639
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    feiparser ();
    virtual ~feiparser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned short int token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned char yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned short int yystos_[];

    /// For a rule, its LHS.
    static const unsigned short int yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

  };

} // yy

/* Line 35 of lalr1.cc  */
#line 659 "feiparser.tab.hh"



#endif /* ! defined PARSER_HEADER_H */
