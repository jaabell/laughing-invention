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
     UMINUS = 281,
     CMP = 282,
     CMD_print = 283,
     CMD_help = 284,
     CMD_whos = 285,
     CMD_check = 286,
     CMD_save = 287,
     CMD_restore = 288,
     MODEL = 289,
     NAME = 290,
     RESTART = 291,
     MESH = 292,
     ADD = 293,
     NODE = 294,
     ELEMENT = 295,
     MATERIAL = 296,
     LOAD = 297,
     TIMEHISTORY = 298,
     IMPOSEDMOTION = 299,
     DAMPING = 300,
     DAMPINGTYPE = 301,
     CONSTRAINT = 302,
     DRM = 303,
     SAVEFORCES = 304,
     RESTOREFORCES = 305,
     SECTION = 306,
     LOADPATTERN = 307,
     PENALTYDISPLACEMENT = 308,
     LOADVALUE = 309,
     ELEMENTNAME = 310,
     MATERIALNAME = 311,
     ACCELERATION_FIELD = 312,
     FIX = 313,
     FREE = 314,
     REMOVE = 315,
     DEFINE = 316,
     ALGORITHM = 317,
     ALGNAME = 318,
     CONVERGENCE_TEST = 319,
     TESTNAME = 320,
     SOLVER = 321,
     SOLVERNAME = 322,
     DYNAMICINTEGRATOR = 323,
     DYNAMICINTEGRATOR_HHT = 324,
     DYNAMICINTEGRATOR_NEWMARK = 325,
     STATICINTEGRATOR = 326,
     STATICINTEGRATOR_DISPLACEMENT = 327,
     SIMULATE = 328,
     STATIC = 329,
     DYNAMIC = 330,
     USING = 331,
     TRANSIENT = 332,
     EIGEN = 333,
     time_step = 334,
     number_of_modes = 335,
     VARIABLETRANSIENT = 336,
     maximum_time_step = 337,
     minimum_time_step = 338,
     number_of_iterations = 339,
     AT = 340,
     ALL = 341,
     AND = 342,
     WITH = 343,
     TEXTDOFS = 344,
     NEW = 345,
     TEXTNUMBER = 346,
     USE = 347,
     TO = 348,
     DOF = 349,
     TEXTWITH = 350,
     NODES = 351,
     FORCE = 352,
     INTEGRATIONPOINTS = 353,
     dof = 354,
     RESPONSE = 355,
     LOADING = 356,
     STAGE = 357,
     STEPS = 358,
     TYPE = 359,
     DOFS = 360,
     FACTOR = 361,
     INCREMENT = 362,
     TH_GROUNDMOTION = 363,
     TH_RICKER2ND = 364,
     TH_ORMSBY = 365,
     TH_RICKER1ST = 366,
     TH_SINE = 367,
     TH_LINEAR = 368,
     TH_PATH_SERIES = 369,
     TH_PATH_TIME_SERIES = 370,
     self_weight = 371,
     surface = 372,
     load_value = 373,
     scale_factor = 374,
     displacement_scale_unit = 375,
     velocity_scale_unit = 376,
     acceleration_scale_unit = 377,
     number_of_steps = 378,
     number_of_drm_nodes = 379,
     number_of_drm_elements = 380,
     x_max = 381,
     x_min = 382,
     y_max = 383,
     y_min = 384,
     z_max = 385,
     z_min = 386,
     element_file = 387,
     nodes_file = 388,
     displacement_file = 389,
     acceleration_file = 390,
     velocity_file = 391,
     force_file = 392,
     series_file = 393,
     time_series_file = 394,
     MAGNITUDES = 395,
     MAGNITUDE = 396,
     strain_increment_size = 397,
     maximum_strain = 398,
     number_of_times_reaching_maximum_strain = 399,
     constitutive = 400,
     testing = 401,
     constant = 402,
     mean = 403,
     triaxial = 404,
     drained = 405,
     undrained = 406,
     simple = 407,
     shear = 408,
     number_of_subincrements = 409,
     maximum_number_of_iterations = 410,
     tolerance_1 = 411,
     tolerance_2 = 412,
     strain = 413,
     stress = 414,
     control = 415,
     Guass = 416,
     points = 417,
     Gauss = 418,
     each = 419,
     point = 420,
     single = 421,
     value = 422,
     EightNodeBrick = 423,
     TwentySevenNodeBrick = 424,
     EightNodeBrick_upU = 425,
     TwentyNodeBrick_uPU = 426,
     TwentyNodeBrick = 427,
     TwentyNodeBrickElastic = 428,
     EightNodeBrick_up = 429,
     variable_node_brick_8_to_27 = 430,
     EightNodeBrickElastic = 431,
     TwentySevenNodeBrickElastic = 432,
     beam_displacement_based = 433,
     beam_elastic = 434,
     beam_elastic_lumped_mass = 435,
     penalty = 436,
     penalty_for_applying_generalized_displacement = 437,
     beam_9dof_elastic = 438,
     FourNodeShellMITC4 = 439,
     FourNodeShellNewMITC4 = 440,
     ThreeNodeShellANDES = 441,
     FourNodeShellANDES = 442,
     truss = 443,
     contact = 444,
     EightNodeBrickLT = 445,
     TwentySevenNodeBrickLT = 446,
     porosity = 447,
     alpha = 448,
     rho_s = 449,
     rho_f = 450,
     k_x = 451,
     k_y = 452,
     k_z = 453,
     K_s = 454,
     K_f = 455,
     pressure = 456,
     cross_section = 457,
     shear_modulus = 458,
     torsion_Jx = 459,
     bending_Iz = 460,
     bending_Iy = 461,
     IntegrationRule = 462,
     number_of_integration_points = 463,
     stiffness = 464,
     normal_stiffness = 465,
     tangential_stiffness = 466,
     friction_ratio = 467,
     maximum_gap = 468,
     xz_plane_vector = 469,
     joint_1_offset = 470,
     joint_2_offset = 471,
     direction = 472,
     contact_plane_vector = 473,
     MembranePlateFiber = 474,
     ElasticMembranePlate = 475,
     thickness = 476,
     linear_elastic_isotropic_3d = 477,
     vonmises_perfectly_plastic = 478,
     vonmises_isotropic_hardening = 479,
     vonmises_linear_kinematic_hardening = 480,
     vonmises_linear_kinematic_hardening_accelerated = 481,
     vonmises_kinematic_hardening = 482,
     vonmises_perfectly_plastic_accelerated = 483,
     vonmises_isotropic_hardening_accelerated = 484,
     vonmises_kinematic_hardening_accelerated = 485,
     sanisand2008 = 486,
     camclay = 487,
     camclay_accelerated = 488,
     sanisand2004 = 489,
     druckerprager_isotropic_hardening = 490,
     druckerprager_isotropic_hardening_accelerated = 491,
     druckerprager_kinematic_hardening = 492,
     druckerprager_kinematic_hardening_accelerated = 493,
     druckerprager_perfectly_plastic = 494,
     druckerprager_perfectly_plastic_accelerated = 495,
     linear_elastic_crossanisotropic = 496,
     uniaxial_concrete02 = 497,
     uniaxial_elastic_1d = 498,
     uniaxial_steel01 = 499,
     uniaxial_steel02 = 500,
     pisano = 501,
     vonmises_perfectly_plastic_LT = 502,
     pisanoLT = 503,
     linear_elastic_isotropic_3d_LT = 504,
     mass_density = 505,
     elastic_modulus = 506,
     viscoelastic_modulus = 507,
     poisson_ratio = 508,
     von_mises_radius = 509,
     druckerprager_angle = 510,
     armstrong_frederick_ha = 511,
     armstrong_frederick_cr = 512,
     initial_confining_stress = 513,
     isotropic_hardening_rate = 514,
     kinematic_hardening_rate = 515,
     poisson_ratio_h_v = 516,
     poisson_ratio_h_h = 517,
     shear_modulus_h_v = 518,
     elastic_modulus_horizontal = 519,
     elastic_modulus_vertical = 520,
     pressure_reference_p0 = 521,
     initial_void_ratio = 522,
     initial_shear_modulus = 523,
     initial_bulk_modulus = 524,
     sanisand2008_Pat = 525,
     sanisand2008_k_c = 526,
     sanisand2008_K0 = 527,
     sanisand2008_alpha_cc = 528,
     sanisand2008_c = 529,
     sanisand2008_xi = 530,
     sanisand2008_lambda = 531,
     sanisand2008_ec_ref = 532,
     sanisand2008_m = 533,
     sanisand2008_h0 = 534,
     sanisand2008_ch = 535,
     sanisand2008_nb = 536,
     sanisand2008_A0 = 537,
     sanisand2008_nd = 538,
     sanisand2008_p_r = 539,
     sanisand2008_rho_c = 540,
     sanisand2008_theta_c = 541,
     sanisand2008_X = 542,
     sanisand2008_z_max = 543,
     sanisand2008_cz = 544,
     sanisand2008_p0 = 545,
     sanisand2008_p_in = 546,
     sanisand2008_G0 = 547,
     sanisand2004_K0 = 548,
     sanisand2004_Pat = 549,
     e0 = 550,
     sanisand2004_G0 = 551,
     sanisand2004_p_cut = 552,
     sanisand2004_Mc = 553,
     sanisand2004_c = 554,
     sanisand2004_lambda_c = 555,
     sanisand2004_xi = 556,
     sanisand2004_ec_ref = 557,
     sanisand2004_m = 558,
     sanisand2004_h0 = 559,
     sanisand2004_ch = 560,
     sanisand2004_nb = 561,
     sanisand2004_A0 = 562,
     sanisand2004_nd = 563,
     sanisand2004_z_max = 564,
     sanisand2004_cz = 565,
     reference_void_ratio = 566,
     critical_stress_ratio_M = 567,
     minimum_bulk_modulus = 568,
     initial_mean_pressure = 569,
     yield_strength = 570,
     strain_hardening_ratio = 571,
     compressive_strength = 572,
     strain_at_compressive_strength = 573,
     crushing_strength = 574,
     strain_at_crushing_strength = 575,
     tensile_strength = 576,
     tension_softening_stiffness = 577,
     M_in = 578,
     kd_in = 579,
     xi_in = 580,
     h_in = 581,
     m_in = 582,
     beta_min = 583,
     ax = 584,
     ay = 585,
     az = 586,
     verbose_level = 587,
     maximum_iterations = 588,
     tolerance = 589,
     beta = 590,
     gamma = 591,
     kappa = 592,
     lambda = 593,
     delta = 594,
     DOMAIN_ = 595,
     startTime = 596,
     endTime = 597,
     Period = 598,
     Phase = 599,
     Amplitude = 600,
     frequency = 601,
     MaxTime = 602,
     frequency1 = 603,
     frequency2 = 604,
     frequency3 = 605,
     frequency4 = 606,
     stiffness_to_use_opt = 607,
     DAMPING_RAYLEIGH = 608,
     DAMPING_CAUGHEY3 = 609,
     DAMPING_CAUGHEY4 = 610,
     a0 = 611,
     a1 = 612,
     stiffness_to_use = 613,
     a2 = 614,
     a3 = 615,
     a4 = 616,
     R0 = 617,
     cR1 = 618,
     cR2 = 619,
     MASS = 620,
     mx = 621,
     my = 622,
     mz = 623,
     Imx = 624,
     Imy = 625,
     Imz = 626,
     equaldof = 627,
     master = 628,
     slave = 629,
     dof_to_constrain = 630,
     of = 631,
     OUTPUT = 632,
     BINARY = 633,
     TEXT = 634
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
#line 654 "feiparser.tab.hh"



#endif /* ! defined PARSER_HEADER_H */
