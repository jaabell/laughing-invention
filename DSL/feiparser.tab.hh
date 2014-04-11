/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2013 Free Software Foundation, Inc.
   
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

/**
 ** \file feiparser.tab.hh
 ** Define the yy::parser class.
 */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef YY_YY_FEIPARSER_TAB_HH_INCLUDED
# define YY_YY_FEIPARSER_TAB_HH_INCLUDED



#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif


namespace yy {
/* Line 33 of lalr1.cc  */
#line 59 "feiparser.tab.hh"

  /// A Bison parser.
  class feiparser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {
/* Line 33 of lalr1.cc  */
#line 109 "feiparser.yy"

  Expression    *exp;           /* For the expressions. Since it is a pointer, no problem. */
  Quantity      *value;         /* For the lexical analyser. NUMBER tokens */
  string        *ident;         /* For the lexical analyser. IDENT tokens */
  int           fn;
  int           errcode;


/* Line 33 of lalr1.cc  */
#line 80 "feiparser.tab.hh"
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
     TH_LINEAR = 364,
     TH_PATH_SERIES = 365,
     TH_PATH_TIME_SERIES = 366,
     TH_CONSTANT = 367,
     TH_FROM_REACTIONS = 368,
     self_weight = 369,
     surface = 370,
     load_value = 371,
     scale_factor = 372,
     displacement_scale_unit = 373,
     velocity_scale_unit = 374,
     acceleration_scale_unit = 375,
     number_of_steps = 376,
     number_of_drm_nodes = 377,
     number_of_drm_elements = 378,
     x_max = 379,
     x_min = 380,
     y_max = 381,
     y_min = 382,
     z_max = 383,
     z_min = 384,
     element_file = 385,
     nodes_file = 386,
     displacement_file = 387,
     acceleration_file = 388,
     velocity_file = 389,
     force_file = 390,
     series_file = 391,
     time_series_file = 392,
     MAGNITUDES = 393,
     MAGNITUDE = 394,
     strain_increment_size = 395,
     maximum_strain = 396,
     number_of_times_reaching_maximum_strain = 397,
     constitutive = 398,
     testing = 399,
     constant = 400,
     mean = 401,
     triaxial = 402,
     drained = 403,
     undrained = 404,
     simple = 405,
     shear = 406,
     number_of_subincrements = 407,
     maximum_number_of_iterations = 408,
     tolerance_1 = 409,
     tolerance_2 = 410,
     strain = 411,
     stress = 412,
     control = 413,
     Guass = 414,
     points = 415,
     Gauss = 416,
     each = 417,
     point = 418,
     single = 419,
     value = 420,
     EightNodeBrick = 421,
     TwentySevenNodeBrick = 422,
     EightNodeBrick_upU = 423,
     TwentyNodeBrick_uPU = 424,
     TwentyNodeBrick = 425,
     TwentyNodeBrickElastic = 426,
     EightNodeBrick_up = 427,
     variable_node_brick_8_to_27 = 428,
     EightNodeBrickElastic = 429,
     TwentySevenNodeBrickElastic = 430,
     beam_displacement_based = 431,
     beam_elastic = 432,
     beam_elastic_lumped_mass = 433,
     penalty = 434,
     penalty_for_applying_generalized_displacement = 435,
     beam_9dof_elastic = 436,
     FourNodeShellMITC4 = 437,
     FourNodeShellNewMITC4 = 438,
     ThreeNodeShellANDES = 439,
     FourNodeShellANDES = 440,
     truss = 441,
     contact = 442,
     EightNodeBrickLT = 443,
     TwentySevenNodeBrickLT = 444,
     porosity = 445,
     alpha = 446,
     rho_s = 447,
     rho_f = 448,
     k_x = 449,
     k_y = 450,
     k_z = 451,
     K_s = 452,
     K_f = 453,
     pressure = 454,
     cross_section = 455,
     shear_modulus = 456,
     torsion_Jx = 457,
     bending_Iz = 458,
     bending_Iy = 459,
     IntegrationRule = 460,
     number_of_integration_points = 461,
     stiffness = 462,
     normal_stiffness = 463,
     tangential_stiffness = 464,
     friction_ratio = 465,
     maximum_gap = 466,
     xz_plane_vector = 467,
     joint_1_offset = 468,
     joint_2_offset = 469,
     direction = 470,
     contact_plane_vector = 471,
     MembranePlateFiber = 472,
     ElasticMembranePlate = 473,
     thickness = 474,
     linear_elastic_isotropic_3d = 475,
     vonmises_perfectly_plastic = 476,
     vonmises_isotropic_hardening = 477,
     vonmises_linear_kinematic_hardening = 478,
     vonmises_linear_kinematic_hardening_accelerated = 479,
     vonmises_kinematic_hardening = 480,
     vonmises_perfectly_plastic_accelerated = 481,
     vonmises_isotropic_hardening_accelerated = 482,
     vonmises_kinematic_hardening_accelerated = 483,
     sanisand2008 = 484,
     camclay = 485,
     camclay_accelerated = 486,
     sanisand2004 = 487,
     druckerprager_isotropic_hardening = 488,
     druckerprager_isotropic_hardening_accelerated = 489,
     druckerprager_kinematic_hardening = 490,
     druckerprager_kinematic_hardening_accelerated = 491,
     druckerprager_perfectly_plastic = 492,
     druckerprager_perfectly_plastic_accelerated = 493,
     linear_elastic_crossanisotropic = 494,
     uniaxial_concrete02 = 495,
     uniaxial_elastic_1d = 496,
     uniaxial_steel01 = 497,
     uniaxial_steel02 = 498,
     pisano = 499,
     vonmises_perfectly_plastic_LT = 500,
     pisanoLT = 501,
     linear_elastic_isotropic_3d_LT = 502,
     mass_density = 503,
     elastic_modulus = 504,
     viscoelastic_modulus = 505,
     poisson_ratio = 506,
     von_mises_radius = 507,
     druckerprager_angle = 508,
     armstrong_frederick_ha = 509,
     armstrong_frederick_cr = 510,
     initial_confining_stress = 511,
     isotropic_hardening_rate = 512,
     kinematic_hardening_rate = 513,
     poisson_ratio_h_v = 514,
     poisson_ratio_h_h = 515,
     shear_modulus_h_v = 516,
     elastic_modulus_horizontal = 517,
     elastic_modulus_vertical = 518,
     pressure_reference_p0 = 519,
     initial_void_ratio = 520,
     initial_shear_modulus = 521,
     initial_bulk_modulus = 522,
     sanisand2008_Pat = 523,
     sanisand2008_k_c = 524,
     sanisand2008_K0 = 525,
     sanisand2008_alpha_cc = 526,
     sanisand2008_c = 527,
     sanisand2008_xi = 528,
     sanisand2008_lambda = 529,
     sanisand2008_ec_ref = 530,
     sanisand2008_m = 531,
     sanisand2008_h0 = 532,
     sanisand2008_ch = 533,
     sanisand2008_nb = 534,
     sanisand2008_A0 = 535,
     sanisand2008_nd = 536,
     sanisand2008_p_r = 537,
     sanisand2008_rho_c = 538,
     sanisand2008_theta_c = 539,
     sanisand2008_X = 540,
     sanisand2008_z_max = 541,
     sanisand2008_cz = 542,
     sanisand2008_p0 = 543,
     sanisand2008_p_in = 544,
     sanisand2008_G0 = 545,
     sanisand2004_K0 = 546,
     sanisand2004_Pat = 547,
     e0 = 548,
     sanisand2004_G0 = 549,
     sanisand2004_p_cut = 550,
     sanisand2004_Mc = 551,
     sanisand2004_c = 552,
     sanisand2004_lambda_c = 553,
     sanisand2004_xi = 554,
     sanisand2004_ec_ref = 555,
     sanisand2004_m = 556,
     sanisand2004_h0 = 557,
     sanisand2004_ch = 558,
     sanisand2004_nb = 559,
     sanisand2004_A0 = 560,
     sanisand2004_nd = 561,
     sanisand2004_z_max = 562,
     sanisand2004_cz = 563,
     reference_void_ratio = 564,
     critical_stress_ratio_M = 565,
     minimum_bulk_modulus = 566,
     initial_mean_pressure = 567,
     yield_strength = 568,
     strain_hardening_ratio = 569,
     compressive_strength = 570,
     strain_at_compressive_strength = 571,
     crushing_strength = 572,
     strain_at_crushing_strength = 573,
     tensile_strength = 574,
     tension_softening_stiffness = 575,
     M_in = 576,
     kd_in = 577,
     xi_in = 578,
     h_in = 579,
     m_in = 580,
     beta_min = 581,
     ax = 582,
     ay = 583,
     az = 584,
     verbose_level = 585,
     maximum_iterations = 586,
     tolerance = 587,
     beta = 588,
     gamma = 589,
     kappa = 590,
     lambda = 591,
     delta = 592,
     DOMAIN_ = 593,
     startTime = 594,
     endTime = 595,
     Period = 596,
     Phase = 597,
     Amplitude = 598,
     frequency = 599,
     MaxTime = 600,
     frequency1 = 601,
     frequency2 = 602,
     frequency3 = 603,
     frequency4 = 604,
     stiffness_to_use_opt = 605,
     DAMPING_RAYLEIGH = 606,
     DAMPING_CAUGHEY3 = 607,
     DAMPING_CAUGHEY4 = 608,
     a0 = 609,
     a1 = 610,
     stiffness_to_use = 611,
     a2 = 612,
     a3 = 613,
     a4 = 614,
     R0 = 615,
     cR1 = 616,
     cR2 = 617,
     MASS = 618,
     mx = 619,
     my = 620,
     mz = 621,
     Imx = 622,
     Imy = 623,
     Imz = 624,
     equaldof = 625,
     master = 626,
     slave = 627,
     dof_to_constrain = 628,
     of = 629,
     OUTPUT = 630,
     BINARY = 631,
     TEXT = 632
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
    /// This class is not copyable.
    feiparser (const feiparser&);
    feiparser& operator= (const feiparser&);

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

#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

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
    ///                     If null, do not display the symbol, just free it.
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
/* Line 33 of lalr1.cc  */
#line 643 "feiparser.tab.hh"



#endif /* !YY_YY_FEIPARSER_TAB_HH_INCLUDED  */
