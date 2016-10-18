/*
///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Jan 2012
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////




 FEI PARSER! v0.9
*/

%language "C++"
%defines
%locations

%define parser_class_name "feiparser"

%{
	// Basic
	#include <iostream>
	#include <iomanip>

	// STL
	#include <vector>
	#include <set>
	#include <string>
	#include <algorithm>
	#include <stack>
	#include <map>
	#include <utility>

	// C library
	#include <cctype>
	#include <cstring>
	#include <cstdio>

	// Boost
	#include <boost/lexical_cast.hpp>

	// Own
	#include "feiast.h"
	#include "feivector.h"
	#include "dsl_actions.h"
	#include "siunit.h"
	#include "quantity.h"
	#include "unitdefinitions.h"
	#include "feidslcaller.h"
	#include "feimath.h"

/*     #include "command_suggestion.h" */

	// CompGeoMech API
	typedef std::vector<Number*> VectorNumber;          // This hack gets rid of a collision between std::vector and Vector classes used within FEI
	#include "../API/ESSI_API.h"

	using namespace std;

	// Prototypes to keep the compiler happy (these are defined on lexer source feiparser.l feiparser.lex.cpp)
	void yyerror (const char *error);
	void clear_stack ();

	//These are auxilliary functions to help in parsing
	Number* dof2number(string dof);
	UnitCheckerFunctionPointerType dof2signature(string dof);
	Number* force2number(string dof);
	UnitCheckerFunctionPointerType force2signature(string dof);
	UnitCheckerFunctionPointerType dof2stiffnesschecker(string dof);
	int bardet_type_parse(string type);

	// Variables
	std::map<string,Quantity> global_variables;
	std::set<string> locked_global_variables;

	//To vectorize commands which apply to several dofs.
	VectorNumber dofchain_holder;
	VectorNumber::const_iterator dofchain_holder_it;

	// Arguments to pass to functions
	ArgumentType args;              //ArgumentType defined in feidslcaller_typedefs.h (std::vector<Expression*>)
	SignatureType signature;        //signatureType defined in feidslcaller_typedefs.h (std::vector<SignatureElementType>)

	// stack class that takes care of all the AST nodes that were allocated (nothing to do with FEM nodes)
	stack <Expression *> nodes;

	// Some useful quantities,
	Quantity q0(0.0, ESSIunits::unitless);

	extern int yylineno;
	extern char* curfilename;
	extern const string thePrompt = "ESSI> ";
	extern int newfile(char* fn);
	extern int newstdin(void);
	extern int popfile(void);
	extern int error_behavior(void);

	void set_outcppfile_name(string newfilename);
	void set_profiling_results_filename(string filename);
	void sync_global_timers();

	//Command line options flags
	extern int FLAG_interactive_mode;
	extern int FLAG_generate_cpp_output;
	extern int FLAG_dry_run;
	extern int FLAG_execute_apis;
	extern int FLAG_inclusion_mode;
	extern int FLAG_binary_output;
	extern int FLAG_exodusii_output;

	string out_cpp_filename("");
	string prompt = thePrompt;

	// Some parse mode flags
	int recovery_mode = 0;

	// Command advisor helps with useful suggestions (hopefully) when parse errors are encountered.
	//    CommandSuggestion cmd_advisor;

%}

// This union defines the values that each rule can parse into.
// %type operators set each token to the corresponding output type.
%union {
  Expression    *exp;           /* For the expressions. Since it is a pointer, no problem. */
  Quantity      *value;         /* For the lexical analyser. NUMBER tokens */
  string        *ident;         /* For the lexical analyser. IDENT tokens */
  int           fn;
  int           errcode;
}

// Basic tokens
%token IDENT NUMBER STRING VECTOR EOL ISUNITTYPE FUNCTION
%token IF THEN ELSE WHILE DO LET EXITSAFEMODE CONTINUE BYE IN REQUIRE
%token PLUSEQUAL MINUSEQUAL TIMESEQUAL DIVIDEEQUAL MODULUSEQUAL POWEQUAL
%token NUMBER_OF_NODES NUMBER_OF_ELEMENTS CURRENT_TIME NUMBER_OF_SP_CONSTRAINTS NUMBER_OF_MP_CONSTRAINTS 
%token NUMBER_OF_LOADS IS_PARALLEL SIMULATE_EXIT_FLAG

// Inform Bison about the type of each terminal and non-terminal (rule or token)
%type <exp>     exp stmt list cpd_stmt dsl CMD_add CMD_fix CMD_misc CMD_define CMD_remove
%type <exp>     ADD_material ADD_element explist
%type <errcode> prompt
%type <value>   NUMBER
%type <ident>   IDENT STRING ISUNITTYPE FUNCTION BYE
//%type <fn>      CMP

/* Precedence information to resolve ambiguity of operations */
%right '=' PLUSEQUAL MINUSEQUAL TIMESEQUAL DIVIDEEQUAL MODULUSEQUAL POWEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left '^'
//%left '&' '|' '!'
%nonassoc UMINUS '!'
%nonassoc <fn> CMP

/* ===============================================================================================*/
/* DSL COMMAND TOKEN DEFINITIONS */
/* ===============================================================================================*/

/* Language commands */
%token CMD_print CMD_help CMD_whos CMD_check CMD_save CMD_restore

%token MODEL NAME RESTART MESH
%token ADD NODE ELEMENT ELEMENTS MATERIAL LOAD TIMEHISTORY IMPOSEDMOTION UNIFORMACCELERATION DAMPING DAMPINGTYPE CONSTRAINT DRM  SECTION LOADPATTERN PENALTYDISPLACEMENT LOADVALUE SET SUPPORT REACTION REACTIONS FORCES DISPLACEMENT
%token ELEMENTNAME MATERIALNAME
%token ACCELERATION_FIELD
%token FIX FREE REMOVE
%token DEFINE ALGORITHM ALGNAME CONSTITUTIVE_ALGNAME CONVERGENCE_TEST TESTNAME SOLVER SOLVERNAME CONSTITUTIVE INTEGRATION 
%token DYNAMICINTEGRATOR DYNAMICINTEGRATOR_HHT DYNAMICINTEGRATOR_NEWMARK STATICINTEGRATOR STATICINTEGRATOR_DISPLACEMENT
%token SIMULATE COMPUTE STATIC DYNAMIC USING TRANSIENT EIGEN time_step number_of_modes VARIABLETRANSIENT maximum_time_step minimum_time_step number_of_iterations RUNTEST
%token AT ALL AND WITH TEXTDOFS NEW TEXTNUMBER USE TO DOF TEXTWITH NODES FORCE INTEGRATIONPOINTS dof RESPONSE FILE FROM EVERY LEVEL BARDETMETHOD
%token LOADING STAGE STEPS TYPE DOFS FACTOR INCREMENT 
%token TH_GROUNDMOTION TH_LINEAR TH_PATH_SERIES TH_PATH_TIME_SERIES TH_CONSTANT TH_FROM_REACTIONS
%token self_weight surface load_value
%token scale_factor displacement_scale_unit velocity_scale_unit acceleration_scale_unit 
%token number_of_steps number_of_boundary_nodes number_of_exterior_nodes number_of_drm_elements 
%token element_file boundary_nodes_file exterior_nodes_file displacement_file acceleration_file velocity_file force_file hdf5_file series_file time_series_file MAGNITUDES MAGNITUDE initial_velocity
%token strain_increment_size maximum_strain  number_of_times_reaching_maximum_strain constitutive testing constant mean triaxial drained undrained simple shear
%token number_of_subincrements maximum_number_of_iterations tolerance_1 tolerance_2 strain stress control Guass points Gauss each point single value
%token initial_normal_stiffness stiffning_rate number_of_integration_points 
%token DIRECT_STRAIN
// Additionally these tokens carry a string type (the above carry no type)
// This is becuase there are several options to what this token may be
// and the program branches depending on these.
// ie. DOF may take the values {ux, uy, uz, Ux, Uy, Uz, rx, ry, rz, p} as
// defined in the lexer file (feiparser.l).
%type <ident> DOF ELEMENTNAME MATERIALNAME ALGNAME CONSTITUTIVE_ALGNAME TESTNAME SOLVERNAME FORCE BARDETMETHOD 
%type <ident> DAMPINGTYPE 



// Tokens for elements
%token EightNodeBrick      TwentyNodeBrick     TwentySevenNodeBrick      VariableNodeBrick           variable_node_brick_8_to_27
%token EightNodeBrick_up   TwentyNodeBrick_up  TwentySevenNodeBrick_up   VariableNodeBrick_up 
%token EightNodeBrick_upU  TwentyNodeBrick_upU TwentySevenNodeBrick_upU  VariableNodeBrick_upU 
%token beam_displacement_based BeamColumnDispFiber3d beam_elastic beam_elastic_lumped_mass beam_9dof_elastic
%token FourNodeShellMITC4 FourNodeShellNewMITC4 ThreeNodeShellANDES FourNodeShellANDES truss contact HardContact FrictionalPenaltyContact SoftContact
%token ShearBeam 

// Element options tokens
%token porosity  alpha rho_s rho_f k_x k_y k_z K_s K_f pressure cross_section shear_modulus torsion_Jx bending_Iz bending_Iy IntegrationRule stiffness normal_stiffness tangential_stiffness normal_damping tangential_damping
%token friction_ratio maximum_gap
%token xz_plane_vector joint_1_offset joint_2_offset direction contact_plane_vector power linear exponential factorial inverse

// Tokens for sections
%token MembranePlateFiber ElasticMembranePlate elastic3d FIBER FiberSection Section fiber_cross_section fiber_location_Y fiber_location_Z fiber_location TorsionConstant_GJ 

// Section options tokens
%token thickness integration_rule section_number 

// Tokens for materials
%token NDMaterialLT linear_elastic_isotropic_3d linear_elastic_isotropic_3d_LT NonlinearIsotropic3DLT
%token sanisand2008 CamClay  sanisand2004 sanisand2004_legacy roundedMohrCoulomb
%token linear_elastic_crossanisotropic uniaxial_concrete02 uniaxial_elastic_1d uniaxial_steel01 uniaxial_steel02 pisano 
%token PisanoLT CamClayLT
%token VonMisesLT VonMisesArmstrongFrederickLT DruckerPragerLT DruckerPragerNonAssociateLinearHardeningLT DruckerPragerVonMisesLT DruckerPragerArmstrongFrederickLE DruckerPragerArmstrongFrederickNE DruckerPragerNonAssociateArmstrongFrederickLT

// Material options tokens
%token mass_density elastic_modulus viscoelastic_modulus poisson_ratio von_mises_radius druckerprager_angle druckerprager_k
%token armstrong_frederick_ha armstrong_frederick_cr initial_confining_stress isotropic_hardening_rate kinematic_hardening_rate poisson_ratio_h_v poisson_ratio_h_h shear_modulus_h_v elastic_modulus_horizontal elastic_modulus_vertical pressure_reference_p0 plastic_flow_kd plastic_flow_xi
%token initial_void_ratio initial_shear_modulus initial_bulk_modulus sanisand2008_Pat sanisand2008_k_c sanisand2008_K0 sanisand2008_alpha_cc sanisand2008_c sanisand2008_xi sanisand2008_lambda sanisand2008_ec_ref sanisand2008_m sanisand2008_h0 sanisand2008_ch sanisand2008_nb sanisand2008_A0 sanisand2008_nd sanisand2008_p_r sanisand2008_rho_c sanisand2008_theta_c sanisand2008_X sanisand2008_z_max sanisand2008_cz sanisand2008_p0 sanisand2008_p_in sanisand2008_G0 sanisand2004_K0
%token sanisand2004_Pat e0 sanisand2004_G0 sanisand2004_p_cut sanisand2004_Mc sanisand2004_c sanisand2004_lambda_c sanisand2004_xi sanisand2004_ec_ref sanisand2004_m sanisand2004_h0 sanisand2004_ch sanisand2004_nb sanisand2004_A0 sanisand2004_nd sanisand2004_z_max sanisand2004_cz
%token reference_void_ratio critical_stress_ratio_M minimum_bulk_modulus initial_mean_pressure yield_strength strain_hardening_ratio compressive_strength strain_at_compressive_strength
%token crushing_strength strain_at_crushing_strength tensile_strength tension_softening_stiffness
%token M_in kd_in xi_in h_in m_in beta_min n_in a_in elastic_modulus_1atm eplcum_cr_in
%token CriticalState_M CriticalState_lambda CriticalState_kappa CriticalState_e0 CriticalState_p0
%token RMC_m RMC_qa RMC_pc RMC_e RMC_eta0 RMC_Heta
%token DuncanCheng_K DuncanCheng_pa DuncanCheng_n DuncanCheng_sigma3_max DuncanCheng_nu

// For acceleration field
%token ax ay az

// Convergence test option tokens
%token verbose_level maximum_iterations tolerance yield_function_relative_tolerance stress_relative_tolerance verbose_output


// Greek
%token beta gamma kappa lambda delta sigma0

%token DOMAIN_ startTime endTime Period Phase Amplitude frequency MaxTime frequency1 frequency2 frequency3 frequency4

// For damping
%token stiffness_to_use_opt DAMPING_RAYLEIGH DAMPING_CAUGHEY3 DAMPING_CAUGHEY4 a0 a1 stiffness_to_use a2 a3 a4 R0 cR1 cR2
%type <ident> stiffness_to_use_opt

// For mass
%token MASS mx my mz Imx Imy Imz

// for constraints
%token equal_dof master slave dof_to_constrain of

// for output control
%token OUTPUT BINARY TEXT ENABLE DISABLE COMPRESSION NON_CONVERGED_ITERATIONS 

// for mysql
//%token mysql databasename host password username port socket STATE


/* ===============================================================================================*/
/* END DSL COMMAND TOKEN DEFINITIONS */



%{
extern int yylex(yy::feiparser::semantic_type *yylval,
				 yy::feiparser::location_type *yylloc);
%}

%initial-action{
	@$.begin.filename = @$.end.filename = new std::string(curfilename);
}

/*  Start not at the topmost rule (dsl) but at one way at the bottom... prompt. */
%start prompt
%%

/* ========================================================================================================= */
/*  Bind DSL actions here */
/* ========================================================================================================= */

// "$1", "$2", etc. symbols are placeholders for the values parsed by tokens which return
// something when matched.

dsl
	//!=========================================================================================================
	//!
	//!FEIDOC print <.>;
	: CMD_print exp
	{
		if ( $2 != 0 ) // ... and deliver us from the null pointer
		{
			$$ = new DslPrint($2);
		} else {
			$$ = new Empty();
		}
		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC print element # <.>;
	| CMD_print ELEMENT TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&output_of_element_to_screen, args, signature, "output_of_element_to_screen");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC print node # <.>;
	| CMD_print NODE TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&output_of_node_to_screen, args, signature, "output_of_node_to_screen");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC check mesh filename;
	| CMD_check MESH exp
	{
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("filename", &isAdimensional));

		$$ = new FeiDslCaller1<string>(&check_mesh, args, signature,"check_mesh");

		nodes.pop();
		nodes.push($$);
		}
	//!=========================================================================================================
	//!
	//!FEIDOC whos;
	| CMD_whos
	{
		$$ = new DslWhos(global_variables, locked_global_variables);
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC help;
	| CMD_help
	{
		cout << "\n Please refer to ESSI Notes for available syntax and examples. \n\n" << endl;
		$$ = new Empty();
		nodes.push($$);
	}
	| CMD_add                   {   $$ = $1; }
	| CMD_misc                  {   $$ = $1; }
	| CMD_fix                   {   $$ = $1; }
	| CMD_define                {   $$ = $1; }
	| CMD_remove                {   $$ = $1; }
	//!=========================================================================================================
	//!
	//! ~FEIDOC;
	| EXITSAFEMODE              {
									cout << "    *** Commands will now be execued. " << endl << endl;
									recovery_mode = 0;
									prompt = thePrompt;
									$$ = new Empty();
									nodes.push($$);
								}
	;

CMD_add
	: ADD                       {  $$ = new DslParseError("add", yylineno); nodes.push($$); }
	//!=========================================================================================================
	//!
	//!FEIDOC add node # <.> at (<length>,<length>,<length>)  with <.> dofs;
	| ADD NODE TEXTNUMBER exp AT '(' exp ',' exp ',' exp ')'  WITH exp TEXTDOFS
	{

		//Always clear the args and signature vectors ... errors ensue if not... like segfaults
		args.clear();
		signature.clear();

		// Add parsed 'exp's to the argument list. Make sure these are of type Expression*.
		// The only reason to not use exp type tokens is to parse strings in which case
		// one matches a STRING token and creates an Expression* by using FeiString (which
		// inherits from Expression) to hold the string.
		args.push_back($4);     //Number
		args.push_back($14);    //ndofs
		args.push_back($7);     //x
		args.push_back($9);     //y
		args.push_back($11);    //z

		// Create the command signature for input verification
		signature.push_back(this_signature("number", &isAdimensional));
		signature.push_back(this_signature("dofs", &isAdimensional));
		signature.push_back(this_signature("x", &isLength));
		signature.push_back(this_signature("y", &isLength));
		signature.push_back(this_signature("z", &isLength));

		// Create the DSL caller node.
		$$ = new FeiDslCaller5<int, int, double, double, double>(&add_node, args, signature, "add_node");

		// Remove the parsed exps from the nodes (free up the memory)
		// There must be as many 'pop's as there are 'exp's.
		nodes.pop();
		nodes.pop();
		nodes.pop();
		nodes.pop();
		nodes.pop();

		// Then add the new caller to the nodes!
		nodes.push($$);
	}
	| ADD ADD_material          { $$ = $2;} // Look in ADD_material rule to see the material commands
	//!=========================================================================================================
	//!
	//!FEIDOC add acceleration field # <.> ax = <accel> ay = <accel> az = <aaccel> ;
	| ADD ACCELERATION_FIELD TEXTNUMBER exp ax '=' exp ay '=' exp az '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($7); signature.push_back(this_signature("ax", &isAcceleration));
		args.push_back($10); signature.push_back(this_signature("ay", &isAcceleration));
		args.push_back($13); signature.push_back(this_signature("az", &isAcceleration));

		$$ = new FeiDslCaller4<int, double, double, double>(&add_acceleration_field, args, signature, "add_acceleration_field");

		for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
		nodes.push($$);
	}
	| ADD ELEMENT ADD_element   { $$ = $3; }
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type self_weight use acceleration field # <.>;
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE self_weight
	  USE ACCELERATION_FIELD TEXTNUMBER exp
	{
		args.clear(); signature.clear();


		args.push_back($4);    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);    signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back($14);   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		$$ = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_element, args, signature, "add_load_selfweight_to_element");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to all elements type self_weight use acceleration field # <.>;
	| ADD LOAD TEXTNUMBER exp TO ALL ELEMENTS TYPE self_weight USE ACCELERATION_FIELD TEXTNUMBER exp
	{
		args.clear(); signature.clear();

		args.push_back($4);    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($13);   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		$$ = new FeiDslCaller2<int, int>(&add_load_selfweight_to_all_elements, args, signature, "add_load_selfweight_to_all_elements");

		for(int ii = 1;ii <=2; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type self_weight use acceleration field # <.>;
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE self_weight
	  USE ACCELERATION_FIELD TEXTNUMBER exp
	{
		args.clear(); signature.clear();


		args.push_back($4);    signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);    signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back($14);   signature.push_back(this_signature("accelerationfieldnumber", &isAdimensional));

		$$ = new FeiDslCaller3<int, int, int>(&add_load_selfweight_to_node, args, signature, "add_load_selfweight_to_node");

		for(int ii = 1;ii <=3; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>) with magnitude <.>;
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ')'
	  WITH MAGNITUDE exp
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",     &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",      &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",      &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",      &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",      &isAdimensional));
		args.push_back($24);       signature.push_back(this_signature("magnitude",  &isPressure));


		$$ = new FeiDslCaller7<int, int,
							   int, int, int, int,
							   double>(&add_load_constant_normal_pressure_to_8node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_8node_brick_surface");

		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>) with magnitudes (<.> , <.> , <.> , <.>);
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ')'
	  WITH MAGNITUDES '(' exp ','  exp ','  exp ',' exp ')'
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",     &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",      &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",      &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",      &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",      &isAdimensional));
		args.push_back($25);       signature.push_back(this_signature("magnitude1", &isPressure));
		args.push_back($27);       signature.push_back(this_signature("magnitude2", &isPressure));
		args.push_back($29);       signature.push_back(this_signature("magnitude3", &isPressure));
		args.push_back($31);       signature.push_back(this_signature("magnitude4", &isPressure));


		$$ = new FeiDslCaller10<int, int,
								int, int, int, int,
								double, double, double, double>(&add_load_different_normal_pressure_to_8node_brick_surface, args, signature, "add_load_different_normal_pressure_to_8node_brick_surface");

		for(int ii = 1;ii <=10; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>) with magnitude <.>;
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp  ')'
	  WITH MAGNITUDE exp
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",     &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",      &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",      &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",      &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",      &isAdimensional));
		args.push_back($22);       signature.push_back(this_signature("node5",      &isAdimensional));
		args.push_back($24);       signature.push_back(this_signature("node6",      &isAdimensional));
		args.push_back($26);       signature.push_back(this_signature("node7",      &isAdimensional));
		args.push_back($28);       signature.push_back(this_signature("node8",      &isAdimensional));
		args.push_back($32);       signature.push_back(this_signature("magnitude",  &isPressure));


		$$ = new FeiDslCaller11<int, int,
								int, int, int, int, int, int, int, int,
								double>(&add_load_constant_normal_pressure_to_20node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_20node_brick_surface");

		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>) with magnitudes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>);
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp  ')'
	  WITH MAGNITUDES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp  ')'
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",      &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element",  &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",       &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",       &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",       &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",       &isAdimensional));
		args.push_back($22);       signature.push_back(this_signature("node5",       &isAdimensional));
		args.push_back($24);       signature.push_back(this_signature("node6",       &isAdimensional));
		args.push_back($26);       signature.push_back(this_signature("node7",       &isAdimensional));
		args.push_back($28);       signature.push_back(this_signature("node8",       &isAdimensional));
		args.push_back($33);       signature.push_back(this_signature("magnitude1",  &isPressure));
		args.push_back($35);       signature.push_back(this_signature("magnitude2",  &isPressure));
		args.push_back($37);       signature.push_back(this_signature("magnitude3",  &isPressure));
		args.push_back($39);       signature.push_back(this_signature("magnitude4",  &isPressure));
		args.push_back($41);       signature.push_back(this_signature("magnitude5",  &isPressure));
		args.push_back($43);       signature.push_back(this_signature("magnitude6",  &isPressure));
		args.push_back($45);       signature.push_back(this_signature("magnitude7",  &isPressure));
		args.push_back($47);       signature.push_back(this_signature("magnitude8",  &isPressure));


		$$ = new FeiDslCaller18<int, int,
								int, int, int, int, int, int, int, int,
								double, double, double, double, double, double, double, double>(&add_load_different_normal_pressure_to_20node_brick_surface, args, signature, "add_load_different_normal_pressure_to_20node_brick_surface");

		for(int ii = 1;ii <=18; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>) with magnitude <.>;
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp ',' exp  ')'
	  WITH MAGNITUDE exp
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",     &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element", &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",      &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",      &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",      &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",      &isAdimensional));
		args.push_back($22);       signature.push_back(this_signature("node5",      &isAdimensional));
		args.push_back($24);       signature.push_back(this_signature("node6",      &isAdimensional));
		args.push_back($26);       signature.push_back(this_signature("node7",      &isAdimensional));
		args.push_back($28);       signature.push_back(this_signature("node8",      &isAdimensional));
		args.push_back($30);       signature.push_back(this_signature("node9",      &isAdimensional));
		args.push_back($34);       signature.push_back(this_signature("magnitude",  &isPressure));


		$$ = new FeiDslCaller12<int, int,
								int, int, int, int, int, int, int, int, int,
								double>(&add_load_constant_normal_pressure_to_27node_brick_surface, args, signature, "add_load_constant_normal_pressure_to_27node_brick_surface");

		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>) with magnitudes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>);
	| ADD LOAD TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp TYPE surface
	  AT NODES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp ',' exp  ')'
	  WITH MAGNITUDES '(' exp ','  exp ','  exp ',' exp ','  exp  ','  exp  ','  exp  ','  exp ',' exp  ')'
	{
		args.clear(); signature.clear();


		args.push_back($4);        signature.push_back(this_signature("number",      &isAdimensional));
		args.push_back($8);        signature.push_back(this_signature("to_element",  &isAdimensional));
		args.push_back($14);       signature.push_back(this_signature("node1",       &isAdimensional));
		args.push_back($16);       signature.push_back(this_signature("node2",       &isAdimensional));
		args.push_back($18);       signature.push_back(this_signature("node3",       &isAdimensional));
		args.push_back($20);       signature.push_back(this_signature("node4",       &isAdimensional));
		args.push_back($22);       signature.push_back(this_signature("node5",       &isAdimensional));
		args.push_back($24);       signature.push_back(this_signature("node6",       &isAdimensional));
		args.push_back($26);       signature.push_back(this_signature("node7",       &isAdimensional));
		args.push_back($28);       signature.push_back(this_signature("node8",       &isAdimensional));
		args.push_back($30);       signature.push_back(this_signature("node9",       &isAdimensional));
		args.push_back($35);       signature.push_back(this_signature("magnitude1",  &isPressure));
		args.push_back($37);       signature.push_back(this_signature("magnitude2",  &isPressure));
		args.push_back($39);       signature.push_back(this_signature("magnitude3",  &isPressure));
		args.push_back($41);       signature.push_back(this_signature("magnitude4",  &isPressure));
		args.push_back($43);       signature.push_back(this_signature("magnitude5",  &isPressure));
		args.push_back($45);       signature.push_back(this_signature("magnitude6",  &isPressure));
		args.push_back($47);       signature.push_back(this_signature("magnitude7",  &isPressure));
		args.push_back($49);       signature.push_back(this_signature("magnitude8",  &isPressure));
		args.push_back($51);       signature.push_back(this_signature("magnitude9",  &isPressure));


		$$ = new FeiDslCaller20<int, int,
								int, int, int, int, int, int, int, int, int,
								double, double, double, double, double,
								double, double, double, double>(&add_load_different_normal_pressure_to_27node_brick_surface, args, signature, "add_load_different_normal_pressure_to_27node_brick_surface");

		for(int ii = 1;ii <=20; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type [linear] [FORCETYPE] = <force or moment>; //[FORCETYPE] = [Fx] [Fy] [Fz] [Mx] [My] [Mz] [F_fluid_x] [F_fluid_y] [F_fluid_z]
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE TH_LINEAR FORCE '=' exp
	{
		args.clear(); signature.clear();

		Expression* dof_number                         = force2number(*$11);
		UnitCheckerFunctionPointerType function_ptr    = force2signature(*$11);

		args.push_back($4);         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);         signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
		args.push_back($13);        signature.push_back(this_signature(*$11, function_ptr));

		$$ = new FeiDslCaller4<int, int, int, double>(&add_force_time_history_linear, args, signature, "add_force_time_history_linear");

		for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 5 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type [path_time_series] [FORCETYPE] = <force or moment> series_file = "filename";
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE TH_PATH_TIME_SERIES
			FORCE '=' exp
			series_file '=' STRING
	{
		args.clear(); signature.clear();

		Expression* dof_number                         = force2number(*$11);
		UnitCheckerFunctionPointerType function_ptr    = force2signature(*$11);

		args.push_back($4);         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);         signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
		args.push_back($13);        signature.push_back(this_signature(*$11, function_ptr));

		//Get the string from the parsed string
		string modelname = *$16;
		//Remove quotes
		modelname.erase(0, 1);
		modelname.erase(modelname.length()-1, modelname.length());

		//Put the string in the argument list
		args.push_back( new FeiString(modelname));
		signature.push_back(this_signature("series_file", &isAdimensional));


		$$ = new FeiDslCaller5<int, int, int, double, string>(&add_force_time_history_path_time_series, args, signature, "add_force_time_history_path_time_series");

		for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 5 exps
			nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type [path_series] [FORCETYPE] = <force or moment> time_step = <time> series_file = "filename";
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE TH_PATH_SERIES
				FORCE '=' exp
				time_step '=' exp
				series_file '=' STRING
				sigma0 '=' '(' exp ',' exp ',' exp ',' exp ',' exp ','exp ')'
	{
		args.clear(); signature.clear();

		Expression* dof_number                         = force2number(*$11);
		UnitCheckerFunctionPointerType function_ptr    = force2signature(*$11);

		args.push_back($4);         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);         signature.push_back(this_signature("to_node", &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
		args.push_back($16);        signature.push_back(this_signature("time_step", &isTime));
		args.push_back($13);        signature.push_back(this_signature(*$11, function_ptr));

		//Get the string from the parsed string
		string modelname = *$19;
		//Remove quotes
		modelname.erase(0, 1);
		modelname.erase(modelname.length()-1, modelname.length());

		//Put the string in the argument list
		args.push_back( new FeiString(modelname));
		signature.push_back(this_signature("series_file", &isAdimensional));



		$$ = new FeiDslCaller6<int, int, int, double, double, string>(&add_force_time_history_path_series, args, signature, "add_force_time_history_path_series");

		for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 5 exps
			nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type [from_reactions];
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE TH_FROM_REACTIONS
	{
		args.clear(); signature.clear();

		args.push_back($4);         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);         signature.push_back(this_signature("to_node", &isAdimensional));

		$$ = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add load # <.> to node # <.> type [from_reactions];
	| ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER ALL TYPE TH_FROM_REACTIONS
	{
		args.clear(); signature.clear();

		args.push_back($4);         signature.push_back(this_signature("number", &isAdimensional));
		args.push_back(new Number(-1, ESSIunits::unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

		$$ = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

		nodes.pop(); //pop 1 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add imposed motion # <.> to node # <.> dof [DOFTYPE] time_step = <t> displacement_scale_unit = <length> displacement_file = "disp_filename" velocity_scale_unit = <velocity> velocity_file = "vel_filename" acceleration_scale_unit = <acceleration> acceleration_file = "acc_filename";
	| ADD IMPOSEDMOTION TEXTNUMBER exp TO NODE TEXTNUMBER exp dof DOF
			time_step '=' exp
			displacement_scale_unit '=' exp
			displacement_file '=' exp
			velocity_scale_unit '=' exp
			velocity_file '=' exp
			acceleration_scale_unit '=' exp
			acceleration_file '=' exp
	{

		args.clear(); signature.clear();

		// Required units are deduced from the passed dof.
		Expression* dof_number = dof2number(*$10);

		args.push_back($4); signature.push_back(this_signature("number",    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node",    &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));
		args.push_back($13); signature.push_back(this_signature("time_step",    &isTime));

		args.push_back($16); signature.push_back(this_signature("displacement_scale_unit",    &isLength));


		//Add displacements file.
		args.push_back( $19);                                          // Add to arguments of call signature
		signature.push_back(this_signature("displacement_file", &isAdimensional));                            // Specify signature


		args.push_back($22); signature.push_back(this_signature("velocity_scale_unit",    &isVelocity));

		//Add velocities file.
		args.push_back( $25);                                          // Add to arguments of call signature
		signature.push_back(this_signature("velocity_file", &isAdimensional));                            // Specify signature


		args.push_back($28); signature.push_back(this_signature("acceleration_scale_unit",    &isAcceleration));

		//Add accelerations file.
		args.push_back( $31);                                          // Add to arguments of call signature
		signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature

		// Generate one command
	   $$ = new FeiDslCaller10<int, int, int, double,
							  double, string, double, string, double, string>(&add_imposed_motion, args,signature, "add_imposed_motion");


		for(int i = 1; i <= 9; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add imposed motion # <.> to node # <.> dof [DOFTYPE] displacement_scale_unit = <displacement> displacement_file = "disp_filename" velocity_scale_unit = <velocity> velocity_file = "vel_filename" acceleration_scale_unit = <acceleration> acceleration_file = "acc_filename";
	| ADD IMPOSEDMOTION TEXTNUMBER exp TO NODE TEXTNUMBER exp dof DOF
			displacement_scale_unit '=' exp
			displacement_file '=' exp
			velocity_scale_unit '=' exp
			velocity_file '=' exp
			acceleration_scale_unit '=' exp
			acceleration_file '=' exp
	{

		args.clear(); signature.clear();

		// Required units are deduced from the passed dof.
		Expression* dof_number = dof2number(*$10);

		args.push_back($4); signature.push_back(this_signature("number",    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node",    &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));

		args.push_back($13); signature.push_back(this_signature("displacement_scale_unit",    &isLength));


		//Add displacements file.
		args.push_back( $16);                                          // Add to arguments of call signature
		signature.push_back(this_signature("displacement_file", &isAdimensional));                            // Specify signature

		args.push_back($19); signature.push_back(this_signature("velocity_scale_unit",    &isVelocity));

		//Add velocities file.
		args.push_back( $22);                                          // Add to arguments of call signature
		signature.push_back(this_signature("velocity_file", &isAdimensional));                            // Specify signature


		args.push_back($25); signature.push_back(this_signature("acceleration_scale_unit",    &isAcceleration));

		//Add accelerations file.
		args.push_back( $28);                                          // Add to arguments of call signature
		signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature

		// Generate one command
	   $$ = new FeiDslCaller9<int, int, int,
							  double, string, double, string, double, string>(&add_imposed_motion, args,signature, "add_imposed_motion");


		for(int i = 1; i <= 8; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add uniform acceleration # <.> to all nodes dof <.> time_step = <T> scale_factor = <.> initial_velocity = <L/S> acceleration_file = <string>;
	| ADD UNIFORMACCELERATION TEXTNUMBER exp TO ALL NODES dof DOF
			time_step '=' exp
			scale_factor '=' exp
			initial_velocity '=' exp
			acceleration_file '=' exp
	{

		args.clear(); signature.clear();


		args.push_back($4); signature.push_back(this_signature("number",    &isAdimensional));


		args.push_back($12); signature.push_back(this_signature("time_step",    &isTime));
		args.push_back($15); signature.push_back(this_signature("scale_factor",    &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("initial_velocity",    &isVelocity));


		//Add acceleration_file file.
		args.push_back( $21);                                          // Add to arguments of call signature
		signature.push_back(this_signature("acceleration_file", &isAdimensional));                            // Specify signature
		Expression* dof_number = dof2number(*$9);
		args.push_back(dof_number); signature.push_back(this_signature("dof",    &isAdimensional));


		//	add_time_history_of_ground_motion_for_uniform_excitation(
		//        int ExcitationNumber,
		//        double timestep,
		//        double scale,
		//        double vel0,
		//        string inputfilename_string,
		//        int direction)
		// Generate one command
	   $$ = new FeiDslCaller6<int, double,  double, double, string, int>(&add_time_history_of_ground_motion_for_uniform_excitation, args,signature, "add_time_history_of_ground_motion_for_uniform_excitation");


		for(int i = 1; i <= 5; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add damping # <.> type [Rayleigh] with a0 = <1/time> a1 = <time> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
	| ADD DAMPING TEXTNUMBER exp TYPE DAMPING_RAYLEIGH WITH a0 '=' exp a1 '=' exp stiffness_to_use '=' stiffness_to_use_opt
	{
		args.clear(); signature.clear();

		args.push_back($4); signature.push_back(this_signature("number",    &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("a0",    &isFrequency));
		args.push_back($13); signature.push_back(this_signature("a1",    &isTime));
		args.push_back(new FeiString(*$16)); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

		$$ = new FeiDslCaller4<int,double,double,string>(&add_damping_rayleigh, args, signature, "add_damping_rayleigh");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add damping # <.> type [Caughey3rd] with a0 = <1/time> a1 = <time> a2 = <time^3> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
	//WARNING: unit for a2 should be implemented
	| ADD DAMPING TEXTNUMBER exp TYPE DAMPING_CAUGHEY3 WITH a0 '=' exp a1 '=' exp a2 '=' exp stiffness_to_use '=' stiffness_to_use_opt
	{
		args.clear(); signature.clear();

		args.push_back($4); signature.push_back(this_signature("number",   &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("a0",      &isFrequency));
		args.push_back($13); signature.push_back(this_signature("a1",      &isTime));
		args.push_back($16); signature.push_back(this_signature("a2",      &isThisUnit< 0, 0, 3>));
		args.push_back(new FeiString(*$19)); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

		$$ = new FeiDslCaller5<int,double,double,double,string>(&add_damping_caughey3rd, args, signature, "add_damping_caughey3rd");

		for(int i = 1; i <= 4; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add damping # <.> type [Caughey4th] with a0 = <1/time> a1 = <time> a2 = <time^3> a3 = <time^5> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
	//WARNING: unit for a2 and a3 should be implemented
	| ADD DAMPING TEXTNUMBER exp TYPE DAMPING_CAUGHEY4 WITH a0 '=' exp a1 '=' exp a2 '=' exp a3 '=' exp stiffness_to_use '=' stiffness_to_use_opt
	{
		args.clear(); signature.clear();

		args.push_back($4); signature.push_back(this_signature("number",   &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("a0",      &isFrequency));
		args.push_back($13); signature.push_back(this_signature("a1",      &isTime));
		args.push_back($16); signature.push_back(this_signature("a2",      &isThisUnit< 0, 0, 3>));
		args.push_back($19); signature.push_back(this_signature("a3",      &isThisUnit< 0, 0, 5>));
		args.push_back(new FeiString(*$22)); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

		$$ = new FeiDslCaller6<int,double,double,double,double,string>(&add_damping_caughey4th, args, signature, "add_damping_caughey4th");

		for(int i = 1; i <= 5; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add damping # <.> to element # <.>;
	| ADD DAMPING TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp
	{
		//add_damping_to_element(int elementNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back($8); signature.push_back(this_signature("element_number",    &isAdimensional));
		args.push_back($4); signature.push_back(this_signature("damping_number",    &isAdimensional));

		$$ = new FeiDslCaller2<int,int>(&add_damping_to_element, args, signature, "add_damping_to_element");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add damping # <.> to node # <.>;
	| ADD DAMPING TEXTNUMBER exp TO NODE TEXTNUMBER exp
	{
		//add_damping_to_node(int nodeNumber, int dampingNumber)
		args.clear(); signature.clear();

		args.push_back($8); signature.push_back(this_signature("node_number",    &isAdimensional));
		args.push_back($4); signature.push_back(this_signature("damping_number",    &isAdimensional));

		$$ = new FeiDslCaller2<int,int>(&add_damping_to_node, args, signature, "add_damping_to_node");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add mass to node # <.> mx = <mass> my = <mass> mz = <mass>;
	| ADD MASS TO NODE TEXTNUMBER exp mx '=' exp my '=' exp mz '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($6); signature.push_back(this_signature("number",    &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("mx",    &isMass));
		args.push_back($12); signature.push_back(this_signature("my",    &isMass));
		args.push_back($15); signature.push_back(this_signature("mz",    &isMass));

		//add_mass_to_node(int, double, double, double, double, double, double, double);
		$$ = new FeiDslCaller4<int,double,double,double>(&add_mass_to_node, args, signature, "add_mass_to_node");

		for(int i = 1; i <= 4; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add mass to node # <.> mx = <mass> my = <mass> mz = <mass> Imx = <mass*length^2> Imy = <mass*length^2> Imz = <mass*length^2>;
	| ADD MASS TO NODE TEXTNUMBER exp mx '=' exp my '=' exp mz '=' exp Imx '=' exp Imy '=' exp Imz '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($6); signature.push_back(this_signature("number",    &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("mx",    &isMass));
		args.push_back($12); signature.push_back(this_signature("my",    &isMass));
		args.push_back($15); signature.push_back(this_signature("mz",    &isMass));
		args.push_back($18); signature.push_back(this_signature("Imx",    &isMassMomentOfInertia));
		args.push_back($21); signature.push_back(this_signature("Imy",    &isMassMomentOfInertia));
		args.push_back($24); signature.push_back(this_signature("Imz",    &isMassMomentOfInertia));

		//add_mass_to_node(int, double, double, double, double, double, double, double);
		$$ = new FeiDslCaller7<int,double,double,double,double,double,double>(&add_mass_to_node, args, signature, "add_mass_to_node");

		for(int i = 1; i <= 7; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add constraint equal_dof with master node # <.> and slave node # <.> dof to constrain <.>;
	| ADD CONSTRAINT equal_dof WITH master NODE TEXTNUMBER exp AND slave NODE TEXTNUMBER exp dof_to_constrain dofchain
	{
		args.clear(); signature.clear();

		args.push_back($8); signature.push_back(this_signature("master",           &isAdimensional));
		args.push_back($13); signature.push_back(this_signature("slave",           &isAdimensional));

		int dofnum = 1;
		for (dofchain_holder_it = dofchain_holder.begin();
			dofchain_holder_it != dofchain_holder.end();
			dofchain_holder_it++)
		{
			args.push_back(*dofchain_holder_it);    signature.push_back(this_signature("dof"+boost::lexical_cast<string>(dofnum++), &isAdimensional)); // lexical_cast converts number to string in this case (www.boost.org) :)
		}

		//add_equaldof_to_two_nodes(int, int, int);
		if (dofchain_holder.size() == 1)
			$$ = new FeiDslCaller3<int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 2)
			$$ = new FeiDslCaller4<int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 3)
			$$ = new FeiDslCaller5<int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 4)
			$$ = new FeiDslCaller6<int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 5)
			$$ = new FeiDslCaller7<int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 6)
			$$ = new FeiDslCaller8<int, int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");
		else if (dofchain_holder.size() == 7)
			$$ = new FeiDslCaller9<int, int, int, int, int, int, int, int, int>(&add_equaldof_to_two_nodes, args, signature, "add_equaldof_to_two_nodes");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add constraint equal_dof with node # <.> dof <.> master and node # <.> dof <.> slave;
	| ADD CONSTRAINT equal_dof WITH NODE TEXTNUMBER exp dof DOF master AND NODE TEXTNUMBER exp dof DOF slave
	{
		args.clear(); signature.clear();

		args.push_back($7); signature.push_back(this_signature("master",           &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("slave",           &isAdimensional));
		args.push_back(dof2number(*$9)); signature.push_back(this_signature("masterdof",           &isAdimensional));
		args.push_back(dof2number(*$16)); signature.push_back(this_signature("slavedof",           &isAdimensional));

		$$ = new FeiDslCaller4<int, int, int, int>(&add_equaldof_to_two_nodes_two_dofs, args, signature, "add_equaldof_to_two_nodes_two_dofs");
		
		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add domain reduction method loading # <.> hdf5_file = <string>;
	| ADD DRM LOADING TEXTNUMBER 
					  exp
					  hdf5_file '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($5); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("hdf5_file",            &isAdimensional));

		$$ = new FeiDslCaller2<int,string>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add domain reduction method loading # <.> hdf5_file = <string> scale_factor = <.>;
	| ADD DRM LOADING TEXTNUMBER 
					  exp
					  hdf5_file '=' exp
					  scale_factor '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($5); signature.push_back(this_signature("pattern_number",        &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("hdf5_file",            &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("scale_factor",            &isAdimensional));

		$$ = new FeiDslCaller3<int, string, double>(&add_load_pattern_domain_reduction_method_hdf5, args, signature, "add_load_pattern_domain_reduction_method_hdf5");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add section # <.> type Membrane_Plate_Fiber thickness = <length> use material # <.>;
	| ADD SECTION TEXTNUMBER exp TYPE MembranePlateFiber thickness '=' exp USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();

		args.push_back($4); signature.push_back(this_signature("section_number",           &isAdimensional));
		args.push_back($13); signature.push_back(this_signature("material_number",           &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("thickness",           &isLength));

		$$ = new FeiDslCaller3<int, int, double>(&add_section_membrane_plate_fiber_to_shell, args, signature, "add_section_membrane_plate_fiber_to_shell");

		for(int i = 1; i <= 3; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add section # <.> type Elastic_Membrane_Plate elastic_modulus = <F/L^2> poisson_ratio = <.> thickness = <length> mass_density = <M/L^3>;
	| ADD SECTION TEXTNUMBER exp TYPE ElasticMembranePlate
	elastic_modulus '=' exp
	poisson_ratio '=' exp
	thickness '=' exp
	mass_density '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($4); signature.push_back(this_signature("section_number",      &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("elastic_modulus",    &isPressure));
		args.push_back($12); signature.push_back(this_signature("poisson_ratio",       &isAdimensional));
		args.push_back($15); signature.push_back(this_signature("thickness",           &isLength));
		args.push_back($18); signature.push_back(this_signature("mass_density",        &isDensity));

		$$ = new FeiDslCaller5<int, double, double, double, double>(&add_section_elastic_membrane_plate_to_shell, args, signature, "add_section_elastic_membrane_plate_to_shell");

		for(int i = 1; i <= 5; i++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//! adding elastic section3d -- Yuan
	//!FEIDOC add section # <.> type elastic3d elastic_modulus = <F/L^2> cross_section = <L^2> bending_Iz = <L^4> bending_Iy=<L^4> torsion_Jx=<L^4> ;
	| ADD SECTION TEXTNUMBER exp TYPE elastic3d 
		elastic_modulus '=' exp 
		cross_section '=' exp 
		bending_Iz '=' exp 
		bending_Iy '='exp 
		shear_modulus '='exp 
		torsion_Jx '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("section_number",          &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("elastic_modulus",         &isPressure));
		args.push_back($12); signature.push_back(this_signature("cross_section",           &isArea));
		args.push_back($15); signature.push_back(this_signature("bending_Iz",           &isAreaMomentOfInertia));
		args.push_back($18); signature.push_back(this_signature("bending_Iy",           &isAreaMomentOfInertia));
		args.push_back($21); signature.push_back(this_signature("shear_modulus",           &isPressure));
		args.push_back($24); signature.push_back(this_signature("torsion_Jx",           &isAreaMomentOfInertia));
		$$ = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_section_elastic_to_beam, args, signature, "add_section_elastic_to_beam");
		for(int i = 1; i <= 7; i++) nodes.pop();
		nodes.push($$);
	}

	//!=========================================================================================================
	//! adding fiber section3d -- Yuan
	//!FEIDOC add section # <.> type FiberSection TorsionConstant_GJ = <F*L^2>
	| ADD SECTION TEXTNUMBER exp TYPE FiberSection
		TorsionConstant_GJ '=' exp 
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("section_number",          &isAdimensional));
		args.push_back($9); signature.push_back(this_signature("TorsionConstant_GJ",           &isThisUnit< 1, 3,-2>));

		$$ = new FeiDslCaller2<int, double>(&add_section_to_model, args, signature, "add_section_to_model");
		for(int i = 1; i <= 2; i++) nodes.pop();
		nodes.push($$);
	}

	;



CMD_fix
	//!=========================================================================================================
	//!
	//!FEIDOC fix node # <.> dofs <.>;
	//!=========================================================================================================
	//!
	//!FEIDOC ux uy uz Ux Uy Uz rx ry rz;
	: FIX NODE TEXTNUMBER exp TEXTDOFS dofchain
	{
		// This implementation is a bit uncommon. The need arises due to the unknown
		// number of fixed dofs the user will specify. Also since the add_support_to_node
		// and related DSLs take only two parameters, one command will generate
		// multiple calls to the DSL. The way to do this is by using ExpressionList
		// which is a linked list of expressions.
		Expression* cmd;
		Expression* command_list = 0;
		int count = 0;

		//dofchain is just a std::vector<Number*> to hold the parsed DOFS to fix.
		//dofchain_it is its const iterator to access each component.
		for (dofchain_holder_it = dofchain_holder.begin();
			dofchain_holder_it != dofchain_holder.end();
			dofchain_holder_it++)
		{
			//Since we're creating one command for each element in dofchain, clear the args and signature.
			args.clear(); signature.clear();

			args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));
			args.push_back(*dofchain_holder_it); signature.push_back(this_signature("dof", &isAdimensional));

			// Generate one command
			cmd = new FeiDslCaller2<int, int>(&add_support_to_node, args,signature,"add_support_to_node");

			// Add to the linked list
			if (count == 0)
				command_list = cmd;
			else
				command_list = new ExpressionList(cmd, command_list);
			count++;
		}
		$$ = command_list;
		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC fix node # <.> dofs all;
	| FIX NODE TEXTNUMBER exp TEXTDOFS ALL
	{
		args.clear();           signature.clear();
		args.push_back($4);     signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&add_support_to_all_dofs_of_node,args,signature, "add_support_to_all_dofs_of_node");


		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add single point constraint to node # <.> dof to constrain <dof_type> constraint value of <corresponding unit>;
	| ADD single point CONSTRAINT TO NODE TEXTNUMBER exp dof_to_constrain DOF CONSTRAINT value of exp
	{
		args.clear();           signature.clear();

		Expression* dof_number = dof2number(*$10);
		UnitCheckerFunctionPointerType function_ptr = dof2signature(*$10);


		args.push_back($8); signature.push_back(this_signature("node", &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("constraint_value", function_ptr));


		$$ = new FeiDslCaller3<int, int, double>(&add_single_point_constraint_to_node,args,signature, "add_single_point_constraint_to_node");


		for(int ii = 1;ii <=2; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC free node # <.> dofs <.>;
	| FREE NODE TEXTNUMBER exp TEXTDOFS dofchain
	{
				Expression* cmd;
		Expression* command_list = 0;
		int count = 0;
		for (dofchain_holder_it = dofchain_holder.begin();
			dofchain_holder_it != dofchain_holder.end();
			dofchain_holder_it++)
		{
			args.clear(); signature.clear();
			args.push_back($4);                     signature.push_back(this_signature("number", &isAdimensional));
			args.push_back(*dofchain_holder_it);    signature.push_back(this_signature("dof", &isAdimensional));

			cmd = new FeiDslCaller2<int, int>(&remove_support_from_node, args,signature ,"remove_support_from_node");

			if (count == 0)
				command_list = cmd;
			else
				command_list = new ExpressionList(cmd, command_list);
			count++;
		}
		$$ = command_list;
		nodes.pop();
		nodes.push($$);
	}
	;

 // This is a neat trick!
dofchain
	: DOF               {
							dofchain_holder.clear();
							dofchain_holder.push_back(dof2number(*$1));
						}
	| DOF dofchain      {
							dofchain_holder.push_back(dof2number(*$1));
						}

explist
	: exp 				{
							$$ = new FeiVector();
							dynamic_cast<FeiVector*>($$)->add_component($1->value());
							nodes.pop();
							nodes.push($$);
						}
	| explist ',' exp   {
							dynamic_cast<FeiVector*>($1)->add_component($3->value());
							$$ = $1;
							nodes.pop();
						}


CMD_define
	//!=========================================================================================================
	//!
	//!FEIDOC enable output;
	: ENABLE OUTPUT 
	{
		args.clear(); signature.clear();
		$$ = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC disable output;
	| DISABLE OUTPUT 
	{
		args.clear(); signature.clear();
		$$ = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
		nodes.push($$);
	}
	//!FEIDOC enable element output;
	| ENABLE ELEMENT OUTPUT 
	{
		args.clear(); signature.clear();
		$$ = new FeiDslCaller0<>(&enable_element_output,args,signature,"enable_element_output");
		nodes.push($$);
	}
	//!FEIDOC output every <.> steps;
	| OUTPUT  EVERY exp STEPS
	{
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("nsteps", &isAdimensional));
		$$ = new FeiDslCaller1<int>(&output_every_nsteps,args,signature,"output_every_nsteps");
		nodes.push($$);
	}
	//!FEIDOC set output compression level to <.>;
	| SET OUTPUT COMPRESSION LEVEL TO exp
	{
		args.clear(); signature.clear();
		args.push_back($6); signature.push_back(this_signature("level", &isAdimensional));
		$$ = new FeiDslCaller1<int>(&set_output_compression_level,args,signature,"set_output_compression_level");
		nodes.push($$);
	}
	////!=========================================================================================================
	////!
	////!FEIDOC define load factor increment <.>;
	| DEFINE LOAD FACTOR INCREMENT exp
	{
		args.clear(); signature.clear();
		args.push_back($5); signature.push_back(this_signature("factor", &isAdimensional));

		$$ = new FeiDslCaller1<double>(&define_static_solution_advancement_integrator_load_control,args,signature,"define_static_solution_advancement_integrator_load_control");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define NDMaterialLT constitutive integration algorithm Forward_Euler;
	| DEFINE NDMaterialLT CONSTITUTIVE INTEGRATION ALGORITHM CONSTITUTIVE_ALGNAME 
	{

		string fname;       // name of the DSL called to report
		args.clear(); signature.clear();
		
		//Read the string and turn into lower-case
		string algname(*$6);
		//std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		int method = -1;
		bool good = false;

		if( algname.compare("Forward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
			good = true;
		}
		
		else
		{
			$$ = new Number(-1, ESSIunits::unitless);
		}

		if(good)
		{
			args.push_back(new Number(method, ESSIunits::unitless)); signature.push_back(this_signature("method", &isAdimensional));
			$$ = new FeiDslCaller1<int>(&define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler,args, signature, "define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler");
		}
		
		for(int i = 0; i < 0; i++)
			nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define NDMaterialLT constitutive integration algorithm Forward_Euler_Subincrement number_of_subincrements =<.>;
	| DEFINE NDMaterialLT CONSTITUTIVE INTEGRATION ALGORITHM CONSTITUTIVE_ALGNAME 
		number_of_subincrements  '=' exp
	{

		string fname;       // name of the DSL called to report
		args.clear(); signature.clear();
		
		//Read the string and turn into lower-case
		string algname(*$6);
		//std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		int method = -1;
		bool good = false;

		if( algname.compare("Forward_Euler_Subincrement") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler_Subincrement;
			good = true;
		}
		else
		{
			$$ = new Number(-1, ESSIunits::unitless);
		}

		if(good)
		{
			args.push_back(new Number(method, ESSIunits::unitless)); signature.push_back(this_signature("method", &isAdimensional));
			args.push_back($9); signature.push_back(this_signature("number_of_subincrements", &isAdimensional));
			$$ = new FeiDslCaller2<int, int>(&define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler_Subincrement,args, signature, "define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler_Subincrement");
		}
		
		for(int i = 0; i < 1; i++)
			nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define NDMaterialLT constitutive integration algorithm [Forward_Euler|Forward_Euler_Subincrement|Backward_Euler|Backward_Euler_ddlambda|Backward_Euler_ddlambda_Subincrement] yield_function_relative_tolerance  = <.> stress_relative_tolerance = <.> maximum_iterations = <.>;
	| DEFINE NDMaterialLT CONSTITUTIVE INTEGRATION ALGORITHM CONSTITUTIVE_ALGNAME 
		yield_function_relative_tolerance  '=' exp
		stress_relative_tolerance '=' exp
		maximum_iterations '=' exp
	{

		string fname;       // name of the DSL called to report
		args.clear(); signature.clear();
		
		//Read the string and turn into lower-case
		string algname(*$6);
		//std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		int method = -1;
		bool good = false;

		if( algname.compare("Forward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
			good = true;
		}
		// make essi back compatible. Keep the old DSLs
		if( algname.compare("Euler_One_Step") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
			good = true;
		}
		if( algname.compare("Forward_Euler_Crisfield") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler_Crisfield;
			good = true;
		}
		if( algname.compare("Multistep_Forward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler;
			good = true;
		}
		if( algname.compare("Forward_Euler_Subincrement") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler_Subincrement;
			good = true;
		}
		if( algname.compare("Multistep_Forward_Euler_Crisfield") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler_Crisfield;
			good = true;
		}
		if( algname.compare("Modified_Euler_Error_Control") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Modified_Euler_Error_Control;
			good = true;
		}
		if( algname.compare("Runge_Kutta_45_Error_Control") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Runge_Kutta_45_Error_Control;
			good = true;
		}
		if( algname.compare("Backward_Euler") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler;
			good = true;
		}
		if( algname.compare("Backward_Euler_ddlambda") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler_ddlambda;
			good = true;
		}
		if( algname.compare("Backward_Euler_ddlambda_Subincrement") == 0)
		{
			method = (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler_ddlambda_Subincrement;
			good = true;
		}
		else
		{
			$$ = new Number(-1, ESSIunits::unitless);
		}

		if(good)
		{
			args.push_back(new Number(method, ESSIunits::unitless)); signature.push_back(this_signature("method", &isAdimensional));
			args.push_back($9); signature.push_back(this_signature("yield_function_relative_tolerance", &isAdimensional));
			args.push_back($12); signature.push_back(this_signature("stress_relative_tolerance", &isAdimensional));
			args.push_back($15); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
			$$ = new FeiDslCaller4<int, double, double, int>(&define_NDMaterialLT_constitutive_integration_algorithm,args, signature, "define_NDMaterialLT_constitutive_integration_algorithm");
		}
		
		for(int i = 0; i < 3; i++)
			nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define algorithm [With_no_convergence_check] / [Newton] / [Modified_Newton];
	| DEFINE ALGORITHM ALGNAME
	{
		// This case is a bit different. ALGNAME contains a string to
		// indicate which algorithm to choose. Instead of having different
		// tokens for each algorithm, just have one and decide in the
		// semantic action which DSL to call.
		// Usually different DSLs have different arguments, but in this
		// case all three take no arguments, which makes this approach easier.

		int (*f)() = NULL;         // function poiner to the algorithm DSL
		string fname;       // name of the DSL called to report

		//Read the string and turn into lower-case
		string algname(*$3);
		std::transform(algname.begin(), algname.end(), algname.begin(), ::tolower);

		args.clear(); signature.clear();
		if( algname.compare("with_no_convergence_check") == 0)
		{
			f = &define_algorithm_with_no_convergence_check_for_analysis;
			fname = "define_algorithm_with_no_convergence_check_for_analysis";
		}
		else if( algname.compare("modified_newton") == 0)
		{
			f = &define_algorithm_modifiednewton_for_analysis;
			fname = "define_algorithm_modifiednewton_for_analysis";
		}
		else if( algname.compare("newton") == 0)
		{
			f = &define_algorithm_newton_for_analysis;
			fname = "define_algorithm_newton_for_analysis";
		}
		else if( algname.compare("newtonlinesearch") == 0)
		{
			f = &define_algorithm_newtonlinesearch_for_analysis;
			fname = "define_algorithm_newtonlinesearch_for_analysis";
		}
		else
		{
			cerr << "Algorithm " << *$3 << " not recognized.\n\n";
		}

		if(f == NULL)
		{
			$$ = new Empty();
		}
		else
		{
			$$ = new FeiDslCaller0<>(f,args, signature, fname);
		}
		
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define convergence test [Norm_Displacement_Increment] / [Energy_Increment] / [Norm_Unbalance] / [Relative_Norm_Displacement_Increment] / [Relative_Energy_Increment] / [Relative_Norm_Unbalance] tolerance = <.> maximum_iterations = <.> verbose_level = <0>|<1>|<2>;
	| DEFINE CONVERGENCE_TEST TESTNAME tolerance '=' exp maximum_iterations '=' exp verbose_level '=' exp
	{
		args.clear(); signature.clear();

		int (*f)(double, int, int) = NULL;         // function poiner to the test DSL
		string fname;       // name of the DSL called to report

		args.clear(); signature.clear();
		if($3->compare("Norm_Displacement_Increment") == 0)
		{
			f = &define_convergence_test_normdisplacementincrement_for_analysis;
			fname = "define_convergence_test_normdisplacementincrement_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if($3->compare("Energy_Increment") == 0)
		{
			f = &define_convergence_test_energyincrement_for_analysis;
			fname = "define_convergence_test_energyincrement_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if($3->compare("Norm_Unbalance") == 0)
		{
			f = &define_convergence_test_normunbalance_for_analysis;
			fname = "define_convergence_test_normunbalance_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}


		if($3->compare("Relative_Norm_Displacement_Increment") == 0)
		{
			f = &define_convergence_test_RelativeNormdisplacementincrement_for_analysis;
			fname = "define_convergence_test_RelativeNormdisplacementincrement_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if($3->compare("Relative_Energy_Increment") == 0)
		{
			f = &define_convergence_test_RelativeEnergyincrement_for_analysis;
			fname = "define_convergence_test_RelativeEnergyincrement_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}
		if($3->compare("Relative_Norm_Unbalance") == 0)
		{
			f = &define_convergence_test_RelativeNormunbalance_for_analysis;
			fname = "define_convergence_test_RelativeNormunbalance_for_analysis";
			args.push_back($6); signature.push_back(this_signature("tolerance", &isAdimensional));
		}












		args.push_back($9); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("verbose_level", &isAdimensional));

		if(f == NULL)
		{
			$$ = new Empty();
		}
		else
		{
			$$ = new FeiDslCaller3<double, int, int> (f, args, signature, fname);
		}
		for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 3 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define solver [ProfileSPD] / [UMFPack];
	| DEFINE SOLVER SOLVERNAME
	{
		int (*f)() = NULL;         // function poiner to the algorithm DSL
		string fname;       // name of the DSL called to report

		args.clear(); signature.clear();
		if($3->compare("ProfileSPD") == 0 || $3->compare("profilespd") == 0)
		{
			f = &define_solver_profilespd_for_analysis;
			fname = "define_solver_profilespd_for_analysis";
		}
		if($3->compare("UMFPack") == 0 || $3->compare("umfpack") == 0)
		{
			f = &define_solver_umfpack_for_analysis;
			fname = "define_solver_umfpack_for_analysis";
		}
		if(
			($3->compare("parallel") == 0) ||
			($3->compare("Parallel") == 0) ||
			($3->compare("PetSc") == 0) ||
			($3->compare("petsc") == 0) ||
			)
		{
			 f = &define_solver_parallel_for_analysis; 
			 fname = "define_solver_parallel_for_analysis"; 
		}

		if(f == NULL)
		{
			$$ = new Empty();
		}
		else
		{
			$$ = new FeiDslCaller0<>(f, args, signature, fname);
		}
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define dynamic integrator [Newmark] with gamma = <.> beta = <.>;
	| DEFINE DYNAMICINTEGRATOR DYNAMICINTEGRATOR_NEWMARK WITH gamma '=' exp beta '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($7); signature.push_back(this_signature("gamma", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("beta", &isAdimensional));

		$$ = new FeiDslCaller2<double,double>(&define_dynamic_solution_advancement_integrator_newmark, args, signature, "define_dynamic_solution_advancement_integrator_newmark");

		for(int ii = 1;ii <=2; ii++) nodes.pop(); //pop 2 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define dynamic integrator [Hilber_Hughes_Taylor] with alpha = <.>;
	| DEFINE DYNAMICINTEGRATOR DYNAMICINTEGRATOR_HHT WITH alpha '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($7); signature.push_back(this_signature("alpha", &isAdimensional));

		$$ = new FeiDslCaller1<double>(&define_dynamic_solution_advancement_integrator_hilber_hughes_taylor, args, signature, "define_dynamic_solution_advancement_integrator_hilber_hughes_taylor");

		nodes.pop(); //pop 2 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC define static integrator displacement_control using node # <.> dof [DOFTYPE] increment <length>;
	| DEFINE STATICINTEGRATOR STATICINTEGRATOR_DISPLACEMENT USING NODE TEXTNUMBER exp dof DOF INCREMENT exp
	{
		args.clear(); signature.clear();

		Expression* dof_number = dof2number(*$9);


		args.push_back($7); signature.push_back(this_signature("node",         &isAdimensional));
		args.push_back(dof_number); signature.push_back(this_signature("dof",  &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("increment",   &isLength));

		$$ = new FeiDslCaller3<int, int, double>(&define_static_solution_advancement_integrator_displacement_control, args, signature, "define_static_solution_advancement_integrator_displacement_control");



		for(int ii = 1;ii <=2; ii++) nodes.pop();
		nodes.push($$);
	}





CMD_misc
	//!=========================================================================================================
	//!
	//!FEIDOC model name "name_string";
	: MODEL NAME STRING
	{
		args.clear(); signature.clear();
		//cout << "  --> Starting new model named " << *$3 << endl;
		//$$ = new FeiDslCaller0<>(&wipe_analysis, args, signature, "wipe_analysis");
		//$$ = new ExpressionList(new Empty(), $$);

		//Get the string from the parsed string
		string modelname = *$3;
		//Remove quotes
		modelname.erase(0, 1);
		modelname.erase(modelname.length()-1, modelname.length());

		//Put the string in the argument list
		args.push_back( new FeiString(modelname));
		signature.push_back(this_signature("name", &isAdimensional));

		//Push into the expression list
		$$ = new FeiDslCaller1<string>(&define_model_name, args, signature, "define_model_name");
		nodes.push($$);

		// Define output file name with the string
		// Remove quotes
		if (out_cpp_filename.length() == 0)
		{
			string newfilename = *$3;
			newfilename.erase(0, 1);
			newfilename.erase(newfilename.length()-1, newfilename.length());

			set_outcppfile_name(newfilename);
		}



	}
	//!=========================================================================================================
	//!
	//!FEIDOC new loading stage "name_string";
	| NEW LOADING STAGE STRING
	{
		Quantity returnQuantity;
		string temp = *$4;
		temp.erase(0, 1);
		temp.erase(temp.length()-1, temp.length());

		//Repace white spaces with underscore "_".
		size_t pos = temp.find(" ");
		while( pos != string::npos)
		{
			temp.replace(pos,1,"_");
			pos = temp.find(" ");
		}

		// Now generate the command
		args.clear();
		signature.clear();
		args.push_back( new FeiString(temp));
		signature.push_back(this_signature("name", &isAdimensional));

		$$ = new FeiDslCaller1<string>(&start_new_stage, args, signature,"start_new_stage");

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate <.> steps using [static] algorithm;
	| SIMULATE exp STEPS USING STATIC ALGORITHM
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("nsteps", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&simulate_using_static_multistep, args, signature, "simulate_using_static_multistep");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate <.> steps using [transient] algorithm time_step = <time>;
	| SIMULATE exp STEPS USING TRANSIENT ALGORITHM time_step '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($9); signature.push_back(this_signature("time_step", &isTime));
		args.push_back($2); signature.push_back(this_signature("nsteps", &isAdimensional));


		$$ = new FeiDslCaller2<double, int>(&simulate_using_transient_multistep, args, signature, "simulate_using_transient_multistep");

		nodes.pop();
		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate <.> steps using [variable transient] algorithm time_step = <time> minimum_time_step = <time> maximum_time_step = <time> number_of_iterations = <.>;
	| SIMULATE exp STEPS USING VARIABLETRANSIENT ALGORITHM
	  time_step '=' exp
	  minimum_time_step '=' exp
	  maximum_time_step '=' exp
	  number_of_iterations '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($9); signature.push_back(this_signature("time_step", &isTime));
		args.push_back($2); signature.push_back(this_signature("nsteps", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("minimum_time_step", &isTime));
		args.push_back($15); signature.push_back(this_signature("maximum_time_step", &isTime));
		args.push_back($18); signature.push_back(this_signature("number_of_iterations", &isAdimensional));


		$$ = new FeiDslCaller5<double, int, double, double, int>(&simulate_using_transient_variable_multistep, args, signature, "simulate_using_transient_variable_multistep");

		for(int ii = 1;ii <=5; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC output non_converged_iterations;
	| OUTPUT NON_CONVERGED_ITERATIONS
	{
		args.clear(); signature.clear();
		(this_signature("non_converged_iterations", &isAdimensional));

		$$ = new FeiDslCaller0<>(&output_non_converged_iterations, args, signature, "output_non_converged_iterations");
	}
	//!=========================================================================================================
	//!
	//!FEIDOC output support reactions;
	| OUTPUT SUPPORT REACTIONS
	{
		args.clear(); signature.clear();
		(this_signature("output_support_reactions", &isAdimensional));

		$$ = new FeiDslCaller0<>(&output_support_reactions, args, signature, "output_support_reactions");
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate using eigen algorithm number_of_modes = <.>;
	| SIMULATE USING EIGEN ALGORITHM number_of_modes '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($7); signature.push_back
		(this_signature("number_of_modes", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&simulate_using_eigen_analysis, args, signature, "simulate_using_eigen_analysis");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing constant mean pressure triaxial strain control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;
	| SIMULATE CONSTITUTIVE testing constant mean pressure triaxial strain control USE MATERIAL TEXTNUMBER exp
	  strain_increment_size '=' exp
	  maximum_strain '=' exp
	  number_of_times_reaching_maximum_strain '=' exp
	{
		args.clear(); signature.clear();


		args.push_back($16); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
		args.push_back($19); signature.push_back(this_signature("maximum_strain", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
		args.push_back($13); signature.push_back(this_signature("material", &isAdimensional));


		$$ = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_constant_p_triaxial, args, signature, "simulate_constitutive_testing_for_constant_p_triaxial");


		for(int ii = 1;ii <=4; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing drained triaxial strain control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;
	| SIMULATE CONSTITUTIVE testing drained triaxial strain control USE MATERIAL TEXTNUMBER exp
	  strain_increment_size '=' exp
	  maximum_strain '=' exp
	  number_of_times_reaching_maximum_strain '=' exp
	{
		args.clear(); signature.clear();


		args.push_back($14); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("maximum_strain", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("material", &isAdimensional));


		$$ = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_drained_triaxial, args, signature, "simulate_constitutive_testing_for_drained_triaxial");


		for(int ii = 1;ii <=4; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing undrained triaxial stress control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;
	| SIMULATE CONSTITUTIVE testing undrained triaxial stress control USE MATERIAL TEXTNUMBER exp
	  strain_increment_size '=' exp
	  maximum_strain '=' exp
	  number_of_times_reaching_maximum_strain '=' exp
	{
		args.clear(); signature.clear();


		args.push_back($14); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("maximum_strain", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("material", &isAdimensional));


		$$ = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_triaxial_stress_control, args, signature, "simulate_constitutive_testing_for_undrained_triaxial_stress_control");


		for(int ii = 1;ii <=4; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing undrained simple shear use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;
	| SIMULATE CONSTITUTIVE testing undrained simple shear strain control USE MATERIAL TEXTNUMBER exp
	  strain_increment_size '=' exp
	  maximum_strain '=' exp
	  number_of_times_reaching_maximum_strain '=' exp
	{
		args.clear(); signature.clear();


		args.push_back($15); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("maximum_strain", &isAdimensional));
		args.push_back($21); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("material", &isAdimensional));


		$$ = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_simple_shear, args, signature, "simulate_constitutive_testing_for_undrained_simple_shear");


		for(int ii = 1;ii <=4; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing undrained triaxial use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;
	| SIMULATE CONSTITUTIVE testing undrained triaxial strain control USE MATERIAL TEXTNUMBER exp
	  strain_increment_size '=' exp
	  maximum_strain '=' exp
	  number_of_times_reaching_maximum_strain '=' exp
	{
		args.clear(); signature.clear();


		args.push_back($14); signature.push_back(this_signature("strain_increment_size", &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("maximum_strain", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("number_of_times_reaching_maximum_strain", &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("material", &isAdimensional));


		$$ = new FeiDslCaller4<double, double, int, int>(&simulate_constitutive_testing_for_undrained_triaxial, args, signature, "simulate_constitutive_testing_for_undrained_triaxial");


		for(int ii = 1;ii <=4; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing BARDETMETHOD use material # <.> scale_factor = <.> series_file = <string>  sigma0 = ( <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> )  verbose_output = <.>
	| SIMULATE CONSTITUTIVE testing BARDETMETHOD USE MATERIAL TEXTNUMBER exp
	  scale_factor '=' exp
	  series_file '=' STRING
	  sigma0 '=' '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
	  verbose_output '=' exp
	{
		args.clear(); signature.clear();
		//int simulate_constitutive_testing_BardetLT_path(int MaterialNumber, int type, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, )

		//Get the string from the parsed string
		string filename = *$14;
		//Remove quotes
		filename.erase(0, 1);
		filename.erase(filename.length()-1, filename.length());

		//Parse BARDETMETHOD
		int btype = bardet_type_parse(*$4);
		Expression* type = new Number( btype, ESSIunits::unitless);

		args.push_back($8); signature.push_back(this_signature("MaterialNumber", &isAdimensional));
		args.push_back(type); signature.push_back(this_signature("type", &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("scale_factor", &isAdimensional));
		args.push_back( new FeiString(filename)); signature.push_back(this_signature("series_file", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("sxx0", &isPressure));
		args.push_back($20); signature.push_back(this_signature("syy0", &isPressure));
		args.push_back($22); signature.push_back(this_signature("szz0", &isPressure));
		args.push_back($24); signature.push_back(this_signature("sxy0", &isPressure));
		args.push_back($26); signature.push_back(this_signature("sxz0", &isPressure));
		args.push_back($28); signature.push_back(this_signature("syz0", &isPressure));
		args.push_back($32); signature.push_back(this_signature("verbose_out", &isAdimensional));


		$$ = new FeiDslCaller11<int, int, double, std::string, 
		   	double, double, double, double, double, double, int>(&simulate_constitutive_testing_BardetLT_path, args, signature, "simulate_constitutive_testing_BardetLT_path");

		for(int ii = 1;ii <=9; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC simulate constitutive testing DIRECT_STRAIN use material # <.> scale_factor = <.> series_file = <string>  sigma0 = ( <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> , <F/L^2> ) verbose_output = <.>
	| SIMULATE CONSTITUTIVE testing DIRECT_STRAIN USE MATERIAL TEXTNUMBER exp
	  scale_factor '=' exp
	  series_file '=' STRING
	  sigma0 '=' '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
	  verbose_output '=' exp
	{
		args.clear(); signature.clear();
		//int simulate_constitutive_testing_DirectStrain_path(int MaterialNumber, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, int verbose_output)

		//Get the string from the parsed string
		string filename = *$14;
		//Remove quotes
		filename.erase(0, 1);
		filename.erase(filename.length()-1, filename.length());


		args.push_back($8); signature.push_back(this_signature("MaterialNumber", &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("scale_factor", &isAdimensional));
		args.push_back( new FeiString(filename)); signature.push_back(this_signature("series_file", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("sxx0", &isPressure));
		args.push_back($20); signature.push_back(this_signature("syy0", &isPressure));
		args.push_back($22); signature.push_back(this_signature("szz0", &isPressure));
		args.push_back($24); signature.push_back(this_signature("sxy0", &isPressure));
		args.push_back($26); signature.push_back(this_signature("sxz0", &isPressure));
		args.push_back($28); signature.push_back(this_signature("syz0", &isPressure));
		args.push_back($32); signature.push_back(this_signature("verbose_out", &isAdimensional));


		$$ = new FeiDslCaller10<int,  double, std::string, 
		   	double, double, double, double, double, double, int>(&	simulate_constitutive_testing_DirectStrain_path, args, signature, "	simulate_constitutive_testing_DirectStrain_path");

		for(int ii = 1;ii <=9; ii++) nodes.pop();

		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC runTest;
	| RUNTEST
	{
		args.clear(); signature.clear();
		$$ = new FeiDslCaller0<>(&run_test, args, signature, "run_test");
		nodes.push($$);
	}
	//!=========================================================================================================
	//!=========================================================================================================
	//!
	//!FEIDOC compute reaction forces;
	| COMPUTE REACTION FORCES
	{
		args.clear(); signature.clear();

		$$ = new FeiDslCaller0<>(&compute_reaction_forces, args, signature, "compute_reaction_forces");

	}
	//!=========================================================================================================
	
	;


CMD_remove
	//!=========================================================================================================
	//!
	//!FEIDOC remove element # <.>;
	: REMOVE ELEMENT TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&remove_element, args, signature, "remove_element");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove node # <.>;
	| REMOVE NODE TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&remove_node, args, signature, "remove_node");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove imposed motion # <.>;
	| REMOVE IMPOSEDMOTION TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));

		$$ = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");

		//pop 1
		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove load # <.>;
	| REMOVE LOAD TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($4); signature.push_back(this_signature("number", &isAdimensional));
			 $$ = new FeiDslCaller1<int>(&remove_load, args, signature, "remove_load");
			 //pop 1
		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove constraint equal_dof node # <.>;
	| REMOVE CONSTRAINT equal_dof NODE TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($6); signature.push_back(this_signature("number", &isAdimensional));
			 $$ = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove displacement from  node # <.>;
	| REMOVE DISPLACEMENT FROM NODE TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($6); signature.push_back(this_signature("number", &isAdimensional));
		$$ = new FeiDslCaller1<int>(&remove_displacement_from_node, args, signature, "remove_displacement_from_node");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC remove strain from element # <.>;
	| REMOVE strain FROM ELEMENT TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($6); signature.push_back(this_signature("number", &isAdimensional));
		$$ = new FeiDslCaller1<int>(&remove_strain_from_element, args, signature, "remove_strain_from_element");

		nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	;


// =======================================================================================================
// Material Models
// =======================================================================================================

ADD_material
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [linear_elastic_isotropic_3d] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.>;
	: MATERIAL TEXTNUMBER exp TYPE linear_elastic_isotropic_3d
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));

		$$ = new FeiDslCaller4<int, double, double, double>(&add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d, args, signature, "add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d");
		for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [VonMisesLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> ;
	| MATERIAL TEXTNUMBER exp TYPE VonMisesLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		von_mises_radius '=' exp
		kinematic_hardening_rate '=' exp
		isotropic_hardening_rate '=' exp
	{

		//int add_constitutive_model_NDMaterialLT_vonmises_linear_hardening(int MaterialNumber,
        //double rho,
        //double E,
        //double nu,
        //double k,
        //double H_alpha,
        //double H_k)


		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));  // 5
		args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7



		$$ = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_vonmises_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_vonmises_linear_hardening");
		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [VonMisesArmstrongFrederickLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> isotropic_hardening_rate = <F/L^2> ;
	| MATERIAL TEXTNUMBER exp TYPE VonMisesArmstrongFrederickLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		von_mises_radius '=' exp
		armstrong_frederick_ha '=' exp
		armstrong_frederick_cr '=' exp
		isotropic_hardening_rate '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));  // 5
		args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 8


		$$ = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_vonMises_ArmstrongFrederick, args, signature, "add_constitutive_model_NDMaterialLT_vonMises_ArmstrongFrederick");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = exp;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		druckerprager_k '=' exp
		kinematic_hardening_rate '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		$$ = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerVonMisesLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = exp;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerVonMisesLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		druckerprager_k '=' exp
		kinematic_hardening_rate '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		$$ = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerNonAssociateLinearHardeningLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2> plastic_flow_xi = <> plastic_flow_kd = <> ;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerNonAssociateLinearHardeningLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		druckerprager_k '=' exp
		kinematic_hardening_rate '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
		plastic_flow_xi '=' exp
		plastic_flow_kd '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional)); // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 8
		args.push_back($29); signature.push_back(this_signature("plastic_flow_xi",   &isAdimensional));  // 9
		args.push_back($32); signature.push_back(this_signature("plastic_flow_kd",   &isAdimensional));  // 10


		$$ = new FeiDslCaller10<int, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_non_associate_linear_hardening, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_non_associate_linear_hardening");
		for(int ii = 1;ii <=10; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerArmstrongFrederickNE] mass_density = <M/L^3> DuncanCheng_K = <.> DuncanCheng_pa = <F/L^2> DuncanCheng_n = <.> DuncanCheng_sigma3_max = <F/L^2> DuncanCheng_nu = <.> druckerprager_k = <> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2>;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerArmstrongFrederickNE
		mass_density '=' exp
		DuncanCheng_K '=' exp
		DuncanCheng_pa '=' exp
		DuncanCheng_n '=' exp
		DuncanCheng_sigma3_max '=' exp
		DuncanCheng_nu '=' exp
		druckerprager_k '=' exp
		armstrong_frederick_ha '=' exp
		armstrong_frederick_cr '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
	{

		args.clear(); signature.clear();
//		add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne(tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double K_in, double pa_in, double n_in, double sigma3_max_in, double nu_in, double rho_, double p0)
		args.push_back($3); signature.push_back(this_signature("number"                    , &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density"              , &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("DuncanCheng_K"            , &isAdimensional));  // 3
		args.push_back($14); signature.push_back(this_signature("DuncanCheng_pa"           , &isPressure));  // 3
		args.push_back($17); signature.push_back(this_signature("DuncanCheng_n"            , &isAdimensional));  // 3
		args.push_back($20); signature.push_back(this_signature("DuncanCheng_sigma3_max"   , &isPressure));  // 3
		args.push_back($23); signature.push_back(this_signature("DuncanCheng_nu"           , &isAdimensional));  // 3
		args.push_back($26); signature.push_back(this_signature("druckerprager_k"          , &isAdimensional));  // 5
		args.push_back($29); signature.push_back(this_signature("armstrong_frederick_ha"   , &isPressure));  // 6
		args.push_back($32); signature.push_back(this_signature("armstrong_frederick_cr"   , &isPressure));  // 6
		args.push_back($35); signature.push_back(this_signature("isotropic_hardening_rate" , &isPressure));  // 7
		args.push_back($38); signature.push_back(this_signature("initial_confining_stress" , &isPressure));  // 7


		$$ = new FeiDslCaller12<int, double, double, double, double, 
		double, double, double, double, 
		double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne");
		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerArmstrongFrederickLE] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2>;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerArmstrongFrederickLE
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		druckerprager_k '=' exp
		armstrong_frederick_ha '=' exp
		armstrong_frederick_cr '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 6
		args.push_back($26); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 7
		args.push_back($29); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 7


		$$ = new FeiDslCaller9<int, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_le, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_le");
		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [DruckerPragerNonAssociateArmstrongFrederickLT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2> plastic_flow_xi = <> plastic_flow_kd = <> ;
	| MATERIAL TEXTNUMBER exp TYPE DruckerPragerNonAssociateArmstrongFrederickLT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		druckerprager_k '=' exp
		armstrong_frederick_ha '=' exp
		armstrong_frederick_cr '=' exp
		isotropic_hardening_rate '=' exp
		initial_confining_stress '=' exp
		plastic_flow_xi '=' exp
		plastic_flow_kd '=' exp
	{

		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  // 3
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("druckerprager_k",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",   &isPressure));  // 6
		args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("isotropic_hardening_rate",   &isPressure));  // 8
		args.push_back($29); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 9
		args.push_back($32); signature.push_back(this_signature("plastic_flow_xi",   &isAdimensional));  // 10
		args.push_back($35); signature.push_back(this_signature("plastic_flow_kd",   &isAdimensional));  // 11

		$$ = new FeiDslCaller11<int, double, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_druckerprager_non_associate_armstrong_frederick, args, signature, "add_constitutive_model_NDMaterialLT_druckerprager_non_associate_armstrong_frederick");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [CamClay] mass_density = <M/L^3> CriticalState_M = <.> CriticalState_lambda = <.> CriticalState_kappa = <.> CriticalState_e0 = <.> CriticalState_p0 = <F/L^2> poisson_ratio = <.> initial_confining_stress = <F/L^2>
	| MATERIAL TEXTNUMBER exp TYPE CamClay
		mass_density '=' exp
		CriticalState_M '=' exp
		CriticalState_lambda '=' exp
		CriticalState_kappa '=' exp
		CriticalState_e0 '=' exp
		CriticalState_p0 '=' exp
		poisson_ratio '=' exp
		initial_confining_stress '=' exp
	{
	//add_constitutive_model_NDMaterialLT_camclay(int MaterialNumber,
	//        double rho_,
	//        double M_,
	//        double lambda_,
	//        double kappa_,
	//        double e0_,
	//        double p0_,
	//        double nu_,
	//        double initial_confinement)
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    // 1
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  // 2
		args.push_back($11); signature.push_back(this_signature("CriticalState_M",  &isAdimensional));  // 3
		args.push_back($14); signature.push_back(this_signature("CriticalState_lambda",    &isAdimensional));  // 4
		args.push_back($17); signature.push_back(this_signature("CriticalState_kappa",  &isAdimensional));  // 5
		args.push_back($20); signature.push_back(this_signature("CriticalState_e0",   &isAdimensional));  // 6
		args.push_back($23); signature.push_back(this_signature("CriticalState_p0",   &isPressure));  // 7
		args.push_back($26); signature.push_back(this_signature("poisson_ratio",   &isAdimensional));  // 8
		args.push_back($29); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  // 9

		$$ = new FeiDslCaller9<int, double, double, double, double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_camclay, args, signature, "add_constitutive_model_NDMaterialLT_camclay");
		for(int ii = 1;ii <=9; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [roundedMohrCoulomb] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> RMC_m = <.> RMC_qa = <F/L^2> RMC_pc = <F/L^2> RMC_e = <.> RMC_eta0 = <.> RMC_Heta = <F/L^2> initial_confining_stress = <F/L^2>
	| MATERIAL TEXTNUMBER exp TYPE roundedMohrCoulomb
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
		RMC_m '=' exp
		RMC_qa '=' exp
		RMC_pc '=' exp
		RMC_e '=' exp
		RMC_Heta '=' exp
		RMC_eta0 '=' exp
		initial_confining_stress '=' exp
	{
		//add_constitutive_model_NDMaterialLT_roundedMohrCoulomb(int tag_in, 
		//double rho_in, double E_in, double nu_in, double m_in, double qa_in, 
		//double pc_in, double e_in, double H_k, double eta0_in, double p0)
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));    
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));  
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));  
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));  
		args.push_back($17); signature.push_back(this_signature("RMC_m",  			&isAdimensional));  
		args.push_back($20); signature.push_back(this_signature("RMC_qa",  			&isPressure));  
		args.push_back($23); signature.push_back(this_signature("RMC_pc",  			&isPressure));  
		args.push_back($26); signature.push_back(this_signature("RMC_e",   			&isAdimensional));  
		args.push_back($29); signature.push_back(this_signature("RMC_Heta",   		&isPressure));  
		args.push_back($32); signature.push_back(this_signature("RMC_eta0",   		&isAdimensional));  
		args.push_back($35); signature.push_back(this_signature("initial_confining_stress",   &isPressure));  

		$$ = new FeiDslCaller11<int, double, double, double, double, double, 
									 double, double, double, double, double>(&add_constitutive_model_NDMaterialLT_roundedMohrCoulomb, args, signature, "add_constitutive_model_NDMaterialLT_roundedMohrCoulomb");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [sanisand2008] mass_density = <M/L^3>  e0 = <.>  sanisand2008_G0 = <.>  sanisand2008_K0 = <.> sanisand2008_Pat = <stress> sanisand2008_k_c = <.>  sanisand2008_alpha_cc = <.> sanisand2008_c = <.>  sanisand2008_xi = <.>  sanisand2008_lambda = <.>  sanisand2008_ec_ref = <.>  sanisand2008_m = <.>  sanisand2008_h0 = <.>  sanisand2008_ch = <.>  sanisand2008_nb = <.>  sanisand2008_A0 = <.> sanisand2008_nd = <.>  sanisand2008_p_r = <.>  sanisand2008_rho_c = <.>  sanisand2008_theta_c = <.>  sanisand2008_X = <.>  sanisand2008_z_max = <.>  sanisand2008_cz = <.>  sanisand2008_p0 = <stress>  sanisand2008_p_in = <.>  algorithm = <explicit|implicit>  number_of_subincrements = <.>  maximum_number_of_iterations = <.>  tolerance_1 = <.>  tolerance_2 = <.>;
	| MATERIAL TEXTNUMBER exp TYPE sanisand2008
		mass_density '=' exp
		e0 '=' exp
		sanisand2008_G0 '=' exp
		sanisand2008_K0 '=' exp
		sanisand2008_Pat '=' exp
		sanisand2008_k_c '=' exp
		sanisand2008_alpha_cc '=' exp
		sanisand2008_c '=' exp
		sanisand2008_xi '=' exp
		sanisand2008_lambda '=' exp
		sanisand2008_ec_ref '=' exp
		sanisand2008_m '=' exp
		sanisand2008_h0 '=' exp
		sanisand2008_ch '=' exp
		sanisand2008_nb '=' exp
		sanisand2008_A0 '=' exp
		sanisand2008_nd '=' exp
		sanisand2008_p_r '=' exp
		sanisand2008_rho_c '=' exp
		sanisand2008_theta_c '=' exp
		sanisand2008_X '=' exp
		sanisand2008_z_max '=' exp
		sanisand2008_cz '=' exp
		sanisand2008_p0 '=' exp
		sanisand2008_p_in '=' exp
		ALGORITHM '=' exp
		number_of_subincrements '=' exp
		maximum_number_of_iterations '=' exp
		tolerance_1 '=' exp
		tolerance_2 '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($3);  signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($83); signature.push_back(this_signature("algoritm",                 &isAdimensional));
		args.push_back($8);  signature.push_back(this_signature("mass_density",              &isDensity));
		args.push_back($11); signature.push_back(this_signature("initial_void_ratio",       &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("initial_shear_modulus",    &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("initial_bulk_modulus",     &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("sanisand2008_Pat",             &isPressure));
		args.push_back($23); signature.push_back(this_signature("sanisand2008_k_c",             &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("sanisand2008_alpha_cc",        &isAdimensional));
		args.push_back($29); signature.push_back(this_signature("sanisand2008_c",               &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("sanisand2008_xi",              &isAdimensional));
		args.push_back($35); signature.push_back(this_signature("sanisand2008_lambda",          &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("sanisand2008_ec_ref",          &isAdimensional));
		args.push_back($41); signature.push_back(this_signature("sanisand2008_m",               &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("sanisand2008_h0",              &isAdimensional));
		args.push_back($47); signature.push_back(this_signature("sanisand2008_ch",              &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("sanisand2008_nb",              &isAdimensional));
		args.push_back($53); signature.push_back(this_signature("sanisand2008_A0",              &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("sanisand2008_nd",              &isAdimensional));
		args.push_back($59); signature.push_back(this_signature("sanisand2008_p_r",             &isPressure));
		args.push_back($62); signature.push_back(this_signature("sanisand2008_rho_c",           &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("sanisand2008_theta_c",         &isAdimensional));
		args.push_back($68); signature.push_back(this_signature("sanisand2008_X",               &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("sanisand2008_z_max",           &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("sanisand2008_cz",              &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("sanisand2008_p0",              &isPressure));
		args.push_back($80); signature.push_back(this_signature("sanisand2008_p_in",             &isPressure));
		args.push_back($86); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
		args.push_back($89); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
		args.push_back($92); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
		args.push_back($95); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


/*         cout << "\n\n\n ******************************************** \n\n" ; */
/*         cout << "WARNING ON SANISAND2008!!! Names of parameters are not definitive! Also the unit checking is not correct! :)"; */
/*         cout << "\n\n\n ******************************************** \n\n" ; */

		$$ = new FeiDslCaller31<int, int,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								int, int, double, double>(&add_constitutive_model_NDMaterial_sanisand_2008, args, signature, "add_constitutive_model_NDMaterial_sanisand_2008");
		for(int ii = 1;ii <= 31; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [sanisand2004] mass_density = <M/L^3> e0 = <.> sanisand2004_G0 = <.> poisson_ratio = <.> sanisand2004_Pat = <stress>  sanisand2004_p_cut = <.>  sanisand2004_Mc = <.>  sanisand2004_c = <.> sanisand2004_lambda_c = <.> sanisand2004_xi = <.>  sanisand2004_ec_ref = <.>  sanisand2004_m = <.>  sanisand2004_h0 = <.> sanisand2004_ch = <.>  sanisand2004_nb = <.> sanisand2004_A0 = <.> sanisand2004_nd = <.> sanisand2004_z_max = <.>  sanisand2004_cz = <.> initial_confining_stress = <stress> ;
	| MATERIAL TEXTNUMBER exp TYPE sanisand2004
		mass_density '=' exp
		e0 '=' exp
		sanisand2004_G0 '=' exp
		poisson_ratio '=' exp
		sanisand2004_Pat '=' exp
		sanisand2004_p_cut '=' exp
		sanisand2004_Mc '=' exp
		sanisand2004_c '=' exp
		sanisand2004_lambda_c '=' exp
		sanisand2004_xi '=' exp
		sanisand2004_ec_ref '=' exp
		sanisand2004_m '=' exp
		sanisand2004_h0 '=' exp
		sanisand2004_ch '=' exp
		sanisand2004_nb '=' exp
		sanisand2004_A0 '=' exp
		sanisand2004_nd '=' exp
		sanisand2004_z_max '=' exp
		sanisand2004_cz '=' exp
		initial_confining_stress '=' exp
	{
		//add_constitutive_model_NDMaterialLT_sanisand_2004(int MaterialNumber, double rho_, double initial_confinement_p0,
        //			double G0_in, double patm_in, double nu_in,
        //			double h0_in, double ch_in,
        //			double c_in, double Mc_in, double ec0_in, double lambda_c_in,
        //			double xi_in,
        //			double nb_in,
        //			double A0_in, double cz_in, double zmax_in, double nd_in,
        //			double m_in, double e_in)
		args.clear(); signature.clear();

		args.push_back($3); signature.push_back(this_signature("number",                        & isAdimensional));
		args.push_back($8); signature.push_back(this_signature("mass_density",                  & isDensity));
		args.push_back($65); signature.push_back(this_signature("initial_confining_stress",     & isPressure));
		args.push_back($14); signature.push_back(this_signature("sanisand2004_G0",              & isAdimensional));
		args.push_back($20); signature.push_back(this_signature("sanisand2004_Pat",             & isPressure));
		args.push_back($17); signature.push_back(this_signature("poisson_ratio",                & isAdimensional));
		args.push_back($44); signature.push_back(this_signature("sanisand2004_h0",              & isAdimensional));
		args.push_back($47); signature.push_back(this_signature("sanisand2004_ch",              & isAdimensional));
		args.push_back($29); signature.push_back(this_signature("sanisand2004_c",               & isAdimensional));
		args.push_back($26); signature.push_back(this_signature("sanisand2004_Mc",              & isAdimensional));
		args.push_back($38); signature.push_back(this_signature("sanisand2004_ec_ref",          & isAdimensional));
		args.push_back($32); signature.push_back(this_signature("sanisand2004_lambda_c",        & isAdimensional));
		args.push_back($35); signature.push_back(this_signature("sanisand2004_xi",              & isAdimensional));
		args.push_back($50); signature.push_back(this_signature("sanisand2004_nb",              & isAdimensional));
		args.push_back($53); signature.push_back(this_signature("sanisand2004_A0",              & isAdimensional));
		args.push_back($62); signature.push_back(this_signature("sanisand2004_cz",              & isAdimensional));
		args.push_back($59); signature.push_back(this_signature("sanisand2004_z_max",           & isAdimensional));
		args.push_back($56); signature.push_back(this_signature("sanisand2004_nd",              & isAdimensional));
		args.push_back($41); signature.push_back(this_signature("sanisand2004_m",               & isAdimensional));
		args.push_back($11); signature.push_back(this_signature("e0",       				    & isAdimensional));
		//args.push_back($23); signature.push_back(this_signature("sanisand2004_p_cut",           & isAdimensional));

/*         cout << "\n\n\n ******************************************** \n\n" ; */
/*         cout << "WARNING ON Dafalias-Manzari!!! Names of parameters are not definitive! Also the unit checking is not correct! :)"; */
/*         cout << "\n\n\n ******************************************** \n\n" ; */

		$$ = new FeiDslCaller20<int,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double>(&add_constitutive_model_NDMaterialLT_sanisand_2004, args, signature, "add_constitutive_model_NDMaterialLT_sanisand_2004");

		for(int ii = 1;ii <= 21; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [sanisand2004_legacy] mass_density = <M/L^3> e0 = <.> sanisand2004_G0 = <.> poisson_ratio = <.> sanisand2004_Pat = <stress>  sanisand2004_p_cut = <.>  sanisand2004_Mc = <.>  sanisand2004_c = <.> sanisand2004_lambda_c = <.> sanisand2004_xi = <.>  sanisand2004_ec_ref = <.>  sanisand2004_m = <.>  sanisand2004_h0 = <.> sanisand2004_ch = <.>  sanisand2004_nb = <.> sanisand2004_A0 = <.> sanisand2004_nd = <.> sanisand2004_z_max = <.>  sanisand2004_cz = <.> initial_confining_stress = <stress>  algorithm = <explicit|implicit>  number_of_subincrements = <.>  maximum_number_of_iterations = <.>  tolerance_1 = <.>  tolerance_2 = <.>;
	| MATERIAL TEXTNUMBER exp TYPE sanisand2004_legacy
		mass_density '=' exp
		e0 '=' exp
		sanisand2004_G0 '=' exp
		poisson_ratio '=' exp
		sanisand2004_Pat '=' exp
		sanisand2004_p_cut '=' exp
		sanisand2004_Mc '=' exp
		sanisand2004_c '=' exp
		sanisand2004_lambda_c '=' exp
		sanisand2004_xi '=' exp
		sanisand2004_ec_ref '=' exp
		sanisand2004_m '=' exp
		sanisand2004_h0 '=' exp
		sanisand2004_ch '=' exp
		sanisand2004_nb '=' exp
		sanisand2004_A0 '=' exp
		sanisand2004_nd '=' exp
		sanisand2004_z_max '=' exp
		sanisand2004_cz '=' exp
		initial_confining_stress '=' exp
		ALGORITHM '=' exp
		number_of_subincrements '=' exp
		maximum_number_of_iterations '=' exp
		tolerance_1 '=' exp
		tolerance_2 '=' exp
	{

		args.clear(); signature.clear();

		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($68); signature.push_back(this_signature("algoritm",                 &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("mass_density",              &isDensity));
		args.push_back($11); signature.push_back(this_signature("e0",       &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("sanisand2004_G0",    &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("sanisand2004_Pat",                   &isPressure));
		args.push_back($23); signature.push_back(this_signature("sanisand2004_p_cut",                 &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("sanisand2004_Mc",                    &isAdimensional));
		args.push_back($29); signature.push_back(this_signature("sanisand2004_c",                     &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("sanisand2004_lambda_c",              &isAdimensional));
		args.push_back($35); signature.push_back(this_signature("sanisand2004_xi",                    &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("sanisand2004_ec_ref",                &isAdimensional));
		args.push_back($41); signature.push_back(this_signature("sanisand2004_m",                     &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("sanisand2004_h0",                    &isAdimensional));
		args.push_back($47); signature.push_back(this_signature("sanisand2004_ch",                    &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("sanisand2004_nb",                    &isAdimensional));
		args.push_back($53); signature.push_back(this_signature("sanisand2004_A0",                    &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("sanisand2004_nd",                    &isAdimensional));
		args.push_back($59); signature.push_back(this_signature("sanisand2004_z_max",                 &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("sanisand2004_cz",                    &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("initial_confining_stress", &isPressure));
		args.push_back($71); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
		args.push_back($80); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

/*         cout << "\n\n\n ******************************************** \n\n" ; */
/*         cout << "WARNING ON Dafalias-Manzari!!! Names of parameters are not definitive! Also the unit checking is not correct! :)"; */
/*         cout << "\n\n\n ******************************************** \n\n" ; */

		$$ = new FeiDslCaller26<int, int,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								double, double, double, double, double,
								int, int, double, double>(&add_constitutive_model_NDMaterial_sanisand_2004, args, signature, "add_constitutive_model_NDMaterial_sanisand_2004");

		for(int ii = 1;ii <= 26; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [linear_elastic_crossanisotropic] mass_density = <mass_density> elastic_modulus_horizontal = <F/L^2> elastic_modulus_vertical = <F/L^2> poisson_ratio_h_v = <.> poisson_ratio_h_h = <.> shear_modulus_h_v = <F/L^2>;
	| MATERIAL TEXTNUMBER exp TYPE linear_elastic_crossanisotropic
	mass_density '=' exp
	elastic_modulus_horizontal '=' exp
	elastic_modulus_vertical '=' exp
	poisson_ratio_h_v '=' exp
	poisson_ratio_h_h '=' exp
	shear_modulus_h_v '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($3);  signature.push_back(this_signature("number",                       &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("elastic_modulus_horizontal",   &isPressure));
		args.push_back($14); signature.push_back(this_signature("elastic_modulus_vertical",     &isPressure));
		args.push_back($17); signature.push_back(this_signature("poisson_ratio_h_v",            &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("poisson_ratio_h_h",            &isAdimensional));
		args.push_back($23); signature.push_back(this_signature("shear_modulus_h_v",            &isPressure));
		args.push_back($8);  signature.push_back(this_signature("mass_density",                 &isDensity));

		$$ = new FeiDslCaller7<int, double, double, double, double, double, double>(&add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic, args, signature, "add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic");
		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [uniaxial_elastic] elastic_modulus = <F/L^2> viscoelastic_modulus = <mass / length / time> ;
	| MATERIAL TEXTNUMBER exp TYPE uniaxial_elastic_1d elastic_modulus '=' exp viscoelastic_modulus '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("elastic_modulus",           &isPressure));
		args.push_back($11); signature.push_back(this_signature("viscolestic_modulus",      &isThisUnit< 1, -1,-1>));
		$$ = new FeiDslCaller3<int, double, double>(&add_constitutive_model_uniaxial_elastic, args, signature, "add_constitutive_model_uniaxial_elastic");

		for(int ii = 1;ii <= 2; ii++) nodes.pop(); //pop 2 exps
			nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [uniaxial_steel01] yield_strength = <F/L^2> elastic_modulus = <F/L^2> strain_hardening_ratio = <.>  a1 = <.>  a2 = <.>  a3 = <>  a4 = <.> ;
	| MATERIAL TEXTNUMBER exp TYPE uniaxial_steel01
	  yield_strength '=' exp
	  elastic_modulus '=' exp
	  strain_hardening_ratio '=' exp
	  a1 '=' exp
	  a2 '=' exp
	  a3 '=' exp
	  a4 '=' exp
	  {
		args.clear(); signature.clear();


		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("yield_strength",            &isPressure));
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",          &isPressure));
		args.push_back($14); signature.push_back(this_signature("strain_hardening_ratio",   &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("a1",                       &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("a2",                       &isAdimensional));
		args.push_back($23); signature.push_back(this_signature("a3",                       &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("a4",                       &isAdimensional));


		$$ = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_steel01, args, signature, "add_constitutive_model_uniaxial_steel01");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push($$);
	  }
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [uniaxial_steel02] yield_strength = <F/L^2> elastic_modulus = <F/L^2> strain_hardening_ratio = <.> R0 = <.> cR1 = <.> cR2 = <.>  a1 = <.>  a2 = <.>  a3 = <>  a4 = <.> ;
	| MATERIAL TEXTNUMBER exp TYPE uniaxial_steel02
	  yield_strength '=' exp
	  elastic_modulus '=' exp
	  strain_hardening_ratio '=' exp
	  R0 '=' exp
	  cR1 '=' exp
	  cR2 '=' exp
	  a1 '=' exp
	  a2 '=' exp
	  a3 '=' exp
	  a4 '=' exp
	  {
		args.clear(); signature.clear();


		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("yield_strength",            &isPressure));
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",          &isPressure));
		args.push_back($14); signature.push_back(this_signature("strain_hardening_ratio",   &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("R0",                       &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("cR1",                      &isAdimensional));
		args.push_back($23); signature.push_back(this_signature("cR2",                      &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("a1",                       &isAdimensional));
		args.push_back($29); signature.push_back(this_signature("a2",                       &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("a3",                       &isAdimensional));
		args.push_back($35); signature.push_back(this_signature("a4",                       &isAdimensional));


		$$ = new FeiDslCaller11<int, double, double, double, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_steel02, args, signature, "add_constitutive_model_uniaxial_steel02");
		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [uniaxial_concrete02] compressive_strength = <F/L^2> strain_at_compressive_strength = <.> crushing_strength = <F/L^2>  strain_at_crushing_strength = <.> lambda = <.> tensile_strength = <F/L^2> tension_softening_stiffness = <F/L^2>;
	| MATERIAL TEXTNUMBER exp TYPE uniaxial_concrete02
	  compressive_strength '=' exp
	  strain_at_compressive_strength '=' exp
	  crushing_strength '=' exp
	  strain_at_crushing_strength '=' exp
	  lambda '=' exp
	  tensile_strength '=' exp
	  tension_softening_stiffness '=' exp
	  {
		args.clear(); signature.clear();


		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("compressive_strength",            &isPressure));
		args.push_back($11); signature.push_back(this_signature("strain_at_compressive_strength",          &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("crushing_strength",   &isPressure));
		args.push_back($17); signature.push_back(this_signature("strain_at_crushing_strength",                       &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("lambda",                      &isAdimensional));
		args.push_back($23); signature.push_back(this_signature("tensile_strength",                      &isPressure));
		args.push_back($26); signature.push_back(this_signature("tension_softening_stiffness",                       &isPressure));


		$$ = new FeiDslCaller8<int, double, double, double, double, double, double, double>(&add_constitutive_model_uniaxial_concrete02, args, signature, "add_constitutive_model_uniaxial_concrete02");
		for(int ii = 1;ii <=8; ii++) nodes.pop();
		nodes.push($$);
	}




	//!=========================================================================================================
	//! adding uniaxial_fiber3d  -Yuan
	//!FEIDOC add fiber # <.> using material # <.> to section # <.>  fiber_cross_section = <area> fiber_location = (<L>,<L>);
	| FIBER TEXTNUMBER exp USING MATERIAL TEXTNUMBER exp TO SECTION TEXTNUMBER exp
		fiber_cross_section '=' exp
		fiber_location '=' '(' exp ',' exp ')'
	  {
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));

		// change the argument order for API
		args.push_back($11); signature.push_back(this_signature("material_number",            &isAdimensional));
		args.push_back($7); signature.push_back(this_signature("section_number",            &isAdimensional));
		
		args.push_back($14); signature.push_back(this_signature("fiber_cross_section",          &isArea));
		args.push_back($18); signature.push_back(this_signature("fiber_location_Y",   &isLength));
		args.push_back($20); signature.push_back(this_signature("fiber_location_Z",   &isLength));

		$$ = new FeiDslCaller6<int, int, int, double, double, double>(&add_single_fiber_to_section, args, signature, "add_single_fiber_to_section");
		for(int ii = 1;ii <=6; ii++) nodes.pop();
		nodes.push($$);
	}





	//!=========================================================================================================
	//!
	//!FEIDOC add material # <.> type [linear_elastic_isotropic_3d_LT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.>;
	| MATERIAL TEXTNUMBER exp TYPE linear_elastic_isotropic_3d_LT
		mass_density '=' exp
		elastic_modulus '=' exp
		poisson_ratio '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
		args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
		args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));


		$$ = new FeiDslCaller4<int, double, double, double>(&add_constitutive_model_NDMaterialLT_linear_elastic_isotropic_3d, args,
			signature,"add_constitutive_model_NDMaterialLT_linear_elastic_isotropic_3d");
		for(int ii = 1;ii <=4; ii++) nodes.pop(); //pop 4 exps
		nodes.push($$);
	}
	;



// =======================================================================================================
// Elements
// =======================================================================================================

ADD_element:
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	TEXTNUMBER exp TYPE EightNodeBrick WITH NODES
		'(' exp ',' exp ',' exp ',' exp ','
			exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($27); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node, args, signature, "add_element_brick_8node");


		for(int ii = 1;ii <=10; ii++) nodes.pop(); //pop 11 exps
		nodes.push($$);
	}  
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [ShearBeam] with nodes (<.>, <.>) cross_section = <l^2> use material # <.>;
	| TEXTNUMBER exp TYPE ShearBeam WITH NODES
		'(' exp ',' exp  ')' cross_section '=' exp
			USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back($18); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller5<int,int,int,double,int>(&add_element_shear_beam, args, signature, "add_element_shear_beam");

		for(int ii = 1;ii <=5; ii++) nodes.pop(); //pop 5 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE EightNodeBrick USING exp Gauss points each direction
		WITH NODES '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGPs", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($33); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller11<int,int,int,int,
								int,int,int,int,
								int,int,int>(&add_element_brick_8node_variable_number_of_gauss_points, args, signature, "add_element_brick_8node_variable_number_of_gauss_points");

		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller29<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int>(&add_element_brick_27node, args, signature, "add_element_brick_27node");

		for(int ii = 1;ii <=29; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE VariableNodeBrick WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller29<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int>(&add_element_brick_variable_node, args, signature, "add_element_brick_27node");

		for(int ii = 1;ii <=29; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller30<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_variable_number_of_gauss_points, args, signature, "add_element_brick_27node_variable_number_of_gauss_points");

		for(int ii = 1;ii <=30; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [variable_node_brick_8_to_27] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE variable_node_brick_8_to_27 USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller30<int,int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,
								int>(&add_element_brick_variable_node_8_to_27, args, signature, "add_element_brick_variable_node_8_to_27");


		for(int ii = 1;ii <=30; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE VariableNodeBrick USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller30<int,int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,
								int>(&add_element_brick_variable_node_variable_number_of_gauss_points, args, signature, "add_element_brick_variable_node_variable_number_of_gauss_points");


		for(int ii = 1;ii <=30; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2);  signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8);  signature.push_back(this_signature("node1",  &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2",  &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3",  &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4",  &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5",  &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6",  &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7",  &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8",  &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($51); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller22<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int>(&add_element_brick_20node, args, signature, "add_element_brick_20node");

		for(int ii = 1;ii <=22; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
	{
		args.clear(); signature.clear();
		args.push_back($2);  signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6);  signature.push_back(this_signature("noGP", &isAdimensional));
		args.push_back($14);  signature.push_back(this_signature("node1",  &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2",  &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3",  &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4",  &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5",  &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7",  &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8",  &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($57); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller23<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int>(&add_element_brick_20node_variable_number_of_gauss_points, args, signature, "add_element_brick_20node_variable_number_of_gauss_points");

		for(int ii = 1;ii <=23; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick_upU] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE EightNodeBrick_upU WITH NODES
		'(' exp ',' exp ',' exp ',' exp ','
			exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp
			alpha '=' exp
			rho_s '=' exp
			rho_f '=' exp
			k_x '=' exp
			k_y '=' exp
			k_z '=' exp
			K_s '=' exp
			K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($27); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($30); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($33); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($39); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($42); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));  //L^3*T/M
		args.push_back($45); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($48); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($51); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($54); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller19<int,int,int,int,int,int,
								int,int,int,int,
								double,double,double,double,
								double,double,double,double,double>(&add_element_brick_8node_upU, args, signature, "add_element_brick_8node_upU");

		for(int ii = 1;ii <=19; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick_upU] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE EightNodeBrick_upU USING exp Gauss points each direction WITH NODES 
		'(' exp ',' exp ',' exp ',' exp ','
			exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp
			alpha '=' exp
			rho_s '=' exp
			rho_f '=' exp
			k_x '=' exp
			k_y '=' exp
			k_z '=' exp
			K_s '=' exp
			K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGPs", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($33); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($36); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($39); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($45); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($48); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));  //L^3*T/M
		args.push_back($51); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($54); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($57); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($60); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller20<int,int,int,int,int,int,
								int,int,int,int,int,
								double,double,double,double,
								double,double,double,double,double>(&add_element_brick_8node_upU_variable_number_of_gauss_points, args, signature, "add_element_brick_8node_upU_variable_number_of_gauss_points");

		for(int ii = 1;ii <=20; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick_up] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE EightNodeBrick_up WITH NODES
		'(' exp ',' exp ',' exp ',' exp ','
			exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($27); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($30); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($33); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($39); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($42); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($45); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($48); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($51); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($54); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller19<int,int,int,int,int,int,int,int,int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_8node_up, args, signature, "add_element_brick_8node_up");

		for(int ii = 1;ii <=19; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8NodeBrick_up] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE EightNodeBrick_up USING exp Gauss points each direction WITH NODES 
		'(' exp ',' exp ',' exp ',' exp ','
			exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp
			alpha '=' exp
			rho_s '=' exp
			rho_f '=' exp
			k_x '=' exp
			k_y '=' exp
			k_z '=' exp
			K_s '=' exp
			K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGPs", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($33); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($36); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($39); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($45); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($48); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));  //L^3*T/M
		args.push_back($51); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($54); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($57); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($60); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller20<int,int,int,int,int,int,
								int,int,int,int,int,
								double,double,double,double,
								double,double,double,double,double>(&add_element_brick_8node_up_variable_number_of_gauss_points, args, signature, "add_element_brick_8node_up_variable_number_of_gauss_points");

		for(int ii = 1;ii <=20; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick_upU] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick_upU WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));

		args.push_back($51); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($54); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($57); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($63); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($66); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($69); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($72); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($75); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($78); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller31<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_upU, args, signature, "add_element_brick_20node_upU");

		for(int ii = 1;ii <=31; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick_upU] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>)  use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick_upU USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2);  signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6);  signature.push_back(this_signature("noGP", &isAdimensional));
		args.push_back($14);  signature.push_back(this_signature("node1",  &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2",  &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3",  &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4",  &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5",  &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7",  &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8",  &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($57); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($63); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($69); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($72); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($75); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($78); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($81); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($84); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller32<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_upU_variable_number_of_gauss_points, args, signature, "add_element_brick_20node_upU_variable_number_of_gauss_points");

		for(int ii = 1;ii <=32; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick_up] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick_up WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($51); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($57); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($63); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($66); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($69); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($72); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($75); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($78); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller31<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_up, args, signature, "add_element_brick_20node_up");

		for(int ii = 1;ii <=31; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [20NodeBrick_up] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentyNodeBrick_up USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2);  signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6);  signature.push_back(this_signature("noGP", &isAdimensional));
		args.push_back($14);  signature.push_back(this_signature("node1",  &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2",  &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3",  &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4",  &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5",  &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7",  &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8",  &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($57); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($63); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($69); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($72); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($75); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($78); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($81); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($84); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller32<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_20node_up_variable_number_of_gauss_points, args, signature, "add_element_brick_20node_up_variable_number_of_gauss_points");

		for(int ii = 1;ii <=32; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick_upU] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick_upU WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));

		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($68); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($77); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($80); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($83); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($86); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($92); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller38<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_27node_upU, args, signature, "add_element_brick_27node_upU");

		for(int ii = 1;ii <=38; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick_upU] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE VariableNodeBrick_upU WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));

		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));

		args.push_back($68); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($77); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($80); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($83); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($86); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($92); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller38<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_variable_node_upU, args, signature, "add_element_brick_variable_node_upU");

		for(int ii = 1;ii <=38; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick_upU] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick_upU USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($80); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($83); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($86); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($92); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($95); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($98); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller39<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_27node_upU_variable_number_of_gauss_points, args, signature, "add_element_brick_27node_upU_variable_number_of_gauss_points");


		for(int ii = 1;ii <=39; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick_upU] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE VariableNodeBrick_upU USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($80); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($83); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($86); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($92); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($95); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($98); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller39<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_variable_node_upU_variable_number_of_gauss_points, args, signature, "add_element_brick_variable_node_upU_variable_number_of_gauss_points");


		for(int ii = 1;ii <=39; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick_up] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick_up WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($68); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($77); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($80); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($83); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($86); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($92); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller38<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_27node_up, args, signature, "add_element_brick_27node_up");

		for(int ii = 1;ii <=38; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick_up] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE VariableNodeBrick_up WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
			USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node9",  &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($65); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($68); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($77); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($80); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($83); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($86); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($92); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller38<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_variable_node_up, args, signature, "add_element_brick_variable_node_up");

		for(int ii = 1;ii <=38; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [27NodeBrick_up] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE TwentySevenNodeBrick_up USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($80); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($83); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($86); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($92); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($95); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($98); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller39<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_27node_up_variable_number_of_gauss_points, args, signature, "add_element_brick_27node_up_variable_number_of_gauss_points");


		for(int ii = 1;ii <=39; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [8_27_NodeBrick_up] using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
	| TEXTNUMBER exp TYPE VariableNodeBrick_up USING exp Gauss points each direction WITH NODES
			'(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ','
				exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')'
				USE MATERIAL TEXTNUMBER exp
			porosity '=' exp alpha '=' exp  rho_s '=' exp  rho_f '=' exp
			k_x '=' exp  k_y '=' exp  k_z '=' exp  K_s '=' exp  K_f '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($6); signature.push_back(this_signature("noGP",   &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($16); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($20); signature.push_back(this_signature("node4", &isAdimensional));
		args.push_back($22); signature.push_back(this_signature("node5", &isAdimensional));
		args.push_back($24); signature.push_back(this_signature("node6", &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("node7", &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("node8", &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("node9", &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("node10", &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("node11", &isAdimensional));
		args.push_back($36); signature.push_back(this_signature("node12", &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("node13", &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("node14", &isAdimensional));
		args.push_back($42); signature.push_back(this_signature("node15", &isAdimensional));
		args.push_back($44); signature.push_back(this_signature("node16", &isAdimensional));
		args.push_back($46); signature.push_back(this_signature("node17", &isAdimensional));
		args.push_back($48); signature.push_back(this_signature("node18", &isAdimensional));
		args.push_back($50); signature.push_back(this_signature("node19", &isAdimensional));
		args.push_back($52); signature.push_back(this_signature("node20", &isAdimensional));
		args.push_back($54); signature.push_back(this_signature("node21", &isAdimensional));
		args.push_back($56); signature.push_back(this_signature("node22", &isAdimensional));
		args.push_back($58); signature.push_back(this_signature("node23", &isAdimensional));
		args.push_back($60); signature.push_back(this_signature("node24", &isAdimensional));
		args.push_back($62); signature.push_back(this_signature("node25", &isAdimensional));
		args.push_back($64); signature.push_back(this_signature("node26", &isAdimensional));
		args.push_back($66); signature.push_back(this_signature("node27", &isAdimensional));
		args.push_back($71); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($74); signature.push_back(this_signature("porosity", &isAdimensional));
		args.push_back($77); signature.push_back(this_signature("alpha", &isAdimensional));
		args.push_back($80); signature.push_back(this_signature("rho_s", &isDensity));
		args.push_back($83); signature.push_back(this_signature("rho_f", &isDensity));
		args.push_back($86); signature.push_back(this_signature("k_x", &isThisUnit<-1,3,1>));
		args.push_back($89); signature.push_back(this_signature("k_y", &isThisUnit<-1,3,1>));
		args.push_back($92); signature.push_back(this_signature("k_z", &isThisUnit<-1,3,1>));
		args.push_back($95); signature.push_back(this_signature("K_s", &isPressure));
		args.push_back($98); signature.push_back(this_signature("K_f", &isPressure));

		$$ = new FeiDslCaller39<int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int,int,int,
								int,int,int,int,int,int,int,int, int,int,
								double,double,double,double,double,double,double,double,double>(&add_element_brick_variable_node_up_variable_number_of_gauss_points, args, signature, "add_element_brick_variable_node_up_variable_number_of_gauss_points");


		for(int ii = 1;ii <=39; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//! adding element DispBeamColumn3d -- Yuan
	//!FEIDOC add element # <.> type [BeamColumnDispFiber3d] with nodes (<.>, <.>) number_of_integration_points = <.> section_number = <.> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );
	| TEXTNUMBER exp TYPE BeamColumnDispFiber3d WITH NODES
		'(' exp ',' exp ')'
		number_of_integration_points '=' exp
		section_number  '=' exp 
		mass_density '=' exp
		xz_plane_vector '=' '(' exp ','  exp ','  exp ')'
		joint_1_offset '=' '(' exp ','  exp ','  exp ')'
		joint_2_offset '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));

		args.push_back($14); signature.push_back(this_signature("number_of_integration_points", &isAdimensional));
		args.push_back($17); signature.push_back(this_signature("section_number", &isAdimensional));
		
		args.push_back($20); signature.push_back(this_signature("mass_density", &isDensity));

		args.push_back($24); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
		args.push_back($26); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
		args.push_back($28); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));

		args.push_back($33); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
		args.push_back($35); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
		args.push_back($37); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));

		args.push_back($42); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
		args.push_back($44); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
		args.push_back($46); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));
		
		$$ = new FeiDslCaller15<int, int, int, int, int, 
								double, double, double, double,
								double, double, double, double, double, double>(&add_element_DispBeamColumn3d, args, signature, "add_element_DispBeamColumn3d");

		for(int ii = 1;ii <=15; ii++) nodes.pop(); 
		nodes.push($$);
	}








// DispBeamColumn3d_lumped_mass



	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [beam_elastic] with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );
	| TEXTNUMBER exp TYPE beam_elastic WITH NODES
		'(' exp ',' exp ')'
		cross_section '=' exp
		elastic_modulus '=' exp
		shear_modulus '=' exp
		torsion_Jx '=' exp
		bending_Iy '=' exp
		bending_Iz '=' exp
		mass_density '=' exp
		xz_plane_vector '=' '(' exp ','  exp ','  exp ')'
		joint_1_offset '=' '(' exp ','  exp ','  exp ')'
		joint_2_offset '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($14); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back($17); signature.push_back(this_signature("elastic_modulus", &isPressure));
		args.push_back($20); signature.push_back(this_signature("shear_modulus", &isPressure));
		args.push_back($23); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
		args.push_back($26); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
		args.push_back($29); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));

		args.push_back($32); signature.push_back(this_signature("mass_density", &isDensity));

		args.push_back($36); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
		args.push_back($45); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
		args.push_back($47); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
		args.push_back($49); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
		args.push_back($54); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
		args.push_back($56); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
		args.push_back($58); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));
		
		$$ = new FeiDslCaller19<int,
								double, double, double, double, double, double,
								int, int, double,
								double, double, double,
								double, double, double,
								double, double, double>(&add_element_beam_elastic, args, signature, "add_element_beam_elastic");

		for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop        19 exps
		nodes.push($$);
	}
		//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [beam_elastic_lumped_mass] with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );
	| TEXTNUMBER exp TYPE beam_elastic_lumped_mass WITH NODES
		'(' exp ',' exp ')'
		cross_section '=' exp
		elastic_modulus '=' exp
		shear_modulus '=' exp
		torsion_Jx '=' exp
		bending_Iy '=' exp
		bending_Iz '=' exp
		mass_density '=' exp
		xz_plane_vector '=' '(' exp ','  exp ','  exp ')'
		joint_1_offset '=' '(' exp ','  exp ','  exp ')'
		joint_2_offset '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($14); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back($17); signature.push_back(this_signature("elastic_modulus", &isPressure));
		args.push_back($20); signature.push_back(this_signature("shear_modulus", &isPressure));
		args.push_back($23); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
		args.push_back($26); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
		args.push_back($29); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));

		args.push_back($32); signature.push_back(this_signature("mass_density", &isDensity));

		args.push_back($36); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
		args.push_back($45); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
		args.push_back($47); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
		args.push_back($49); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
		args.push_back($54); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
		args.push_back($56); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
		args.push_back($58); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));
	
		$$ = new FeiDslCaller19<int,
								double, double, double, double, double, double,
								int, int, double,
								double, double, double,
								double, double, double,
								double, double, double>(&add_element_beam_elastic_lumped_mass, args, signature, "beam_elastic_lumped_mass");

		for(int ii = 1;ii <=19; ii++) nodes.pop(); //pop 19 exps
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [beam_displacement_based] with nodes (<.>, <.>) with # <.> integration_points use section # <.> mass_density = <M/L^3> IntegrationRule = "" xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );
	| TEXTNUMBER exp TYPE beam_displacement_based WITH NODES
		'(' exp ',' exp ')'
		WITH exp INTEGRATIONPOINTS
		USE SECTION TEXTNUMBER exp
		mass_density '=' exp
		IntegrationRule '=' exp
		xz_plane_vector '=' '(' exp ','  exp ','  exp ')'
		joint_1_offset  '=' '(' exp ','  exp ','  exp ')'
		joint_2_offset  '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();

		args.push_back($2);  signature.push_back(this_signature("number",                        &isAdimensional));
		args.push_back($8);  signature.push_back(this_signature("node1",                         &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2",                         &isAdimensional));
		args.push_back($13); signature.push_back(this_signature("number_of_integration_points",  &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("SectionNumber",                 &isAdimensional));
		args.push_back($21); signature.push_back(this_signature("mass_density",                  &isDensity));




/*        string modelname = *$24;
		modelname.erase(0, 1);
		modelname.erase(modelname.length()-1, modelname.length());
		args.push_back( new FeiString(modelname));
		signature.push_back(this_signature("IntegrationRule", &isAdimensional));*/


		args.push_back($24); signature.push_back(this_signature("IntegrationRule",               &isAdimensional));


		args.push_back($28); signature.push_back(this_signature("vecxzPlane_X",    &isAdimensional));
		args.push_back($30); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
		args.push_back($37); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
		args.push_back($39); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
		args.push_back($41); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
		args.push_back($46); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
		args.push_back($48); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
		args.push_back($50); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));

		$$ = new FeiDslCaller16<int, int, int, int, int, double, string,
								double, double, double,
								double, double, double,
								double, double, double>(&add_element_beam_displacement_based, args, signature, "add_element_beam_displacement_based");

		for(int ii = 1;ii <=16; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [beam_9dof_elastic] with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );
	| TEXTNUMBER exp TYPE beam_9dof_elastic WITH NODES
		'(' exp ',' exp ')'
		cross_section '=' exp
		elastic_modulus '=' exp
		shear_modulus '=' exp
		torsion_Jx '=' exp
		bending_Iy '=' exp
		bending_Iz '=' exp
		mass_density '=' exp
		xz_plane_vector '=' '(' exp ','  exp ','  exp ')'
		joint_1_offset '=' '(' exp ','  exp ','  exp ')'
		joint_2_offset '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($14); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back($17); signature.push_back(this_signature("elastic_modulus", &isPressure));
		args.push_back($20); signature.push_back(this_signature("shear_modulus", &isPressure));
		args.push_back($23); signature.push_back(this_signature("torsion_Jx", &isAreaMomentOfInertia));
		args.push_back($26); signature.push_back(this_signature("bending_Iy", &isAreaMomentOfInertia));
		args.push_back($29); signature.push_back(this_signature("bending_Iz", &isAreaMomentOfInertia));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));

		args.push_back($32); signature.push_back(this_signature("mass_density", &isDensity));

		args.push_back($36); signature.push_back(this_signature("vecxzPlane_X",     &isAdimensional));
		args.push_back($38); signature.push_back(this_signature("vecxzPlane_Y",    &isAdimensional));
		args.push_back($40); signature.push_back(this_signature("vecxzPlane_Z",    &isAdimensional));
		args.push_back($45); signature.push_back(this_signature("jntOffsetI_X",    &isLength));
		args.push_back($47); signature.push_back(this_signature("jntOffsetI_Y",    &isLength));
		args.push_back($49); signature.push_back(this_signature("jntOffsetI_Z",    &isLength));
		args.push_back($54); signature.push_back(this_signature("jntOffsetJ_X",    &isLength));
		args.push_back($56); signature.push_back(this_signature("jntOffsetJ_Y",    &isLength));
		args.push_back($58); signature.push_back(this_signature("jntOffsetJ_Z",    &isLength));

		$$ = new FeiDslCaller19<int,
								double, double, double, double, double, double,
								int, int, double,
								double, double, double,
								double, double, double,
								double, double, double>(&add_element_rank_one_deficient_elastic_pinned_fixed_beam, args, signature, "add_element_rank_one_deficient_elastic_pinned_fixed_beam");


		for(int ii = 1;ii <=19; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [HardContact] with nodes (<.>, <.>) normal_stiffness = <F/L> tangential_stiffness = <F/L> normal_damping = <F/L> tangential_damping = <F/L>  friction_ratio = <.>  contact_plane_vector = (<.>, <.>, <.> );
	| TEXTNUMBER exp TYPE HardContact WITH NODES
		'(' exp ',' exp ')'
		normal_stiffness '=' exp
		tangential_stiffness '=' exp
		normal_damping '=' exp
		tangential_damping '=' exp
		friction_ratio '=' exp
		contact_plane_vector '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();

		args.push_back($2); signature.push_back(this_signature("number",                  &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1",                   &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2",                  &isAdimensional));

		args.push_back($14); signature.push_back(this_signature("normal_stiffness",       &isThisUnit<1, 0, -2>));
		args.push_back($17); signature.push_back(this_signature("tangential_stiffness",   &isThisUnit<1, 0, -2>));
		args.push_back($20); signature.push_back(this_signature("normal_damping",         &isThisUnit<1, 0, -1>));
		args.push_back($23); signature.push_back(this_signature("tangential_damping",     &isThisUnit<1, 0, -1>));
		args.push_back($26); signature.push_back(this_signature("friction_ratio",         &isAdimensional));

		args.push_back($30); signature.push_back(this_signature("x_local_1",              &isAdimensional));
		args.push_back($32); signature.push_back(this_signature("x_local_2",              &isAdimensional));
		args.push_back($34); signature.push_back(this_signature("x_local_3",              &isAdimensional));


		$$ = new FeiDslCaller11<int, int, int,
							   double, double, double, double, double,
							   double, double, double>(&add_element_hard_contact, args, signature, "add_element_hard_contact");

		for(int ii = 1;ii <=11; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [SoftContact] with nodes (<.>, <.>) initial_normal_stiffness = <F/L> stiffning_rate = <m^-1> tangential_stiffness = <F/L> normal_damping = <F/L> tangential_damping = <F/L>  friction_ratio = <.>  contact_plane_vector = (<.>, <.>, <.> );
	| TEXTNUMBER exp TYPE SoftContact WITH NODES '(' exp ',' exp ')'
		initial_normal_stiffness '=' exp
		stiffning_rate '=' exp
		tangential_stiffness '=' exp
		normal_damping '=' exp
		tangential_damping '=' exp
		friction_ratio '=' exp
		contact_plane_vector '=' '(' exp ','  exp ','  exp ')'
	{
		args.clear(); signature.clear();


		args.push_back($2); signature.push_back(this_signature("number",                    &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1",                     &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2",                    &isAdimensional));
 
 		args.push_back($14); signature.push_back(this_signature("initial_normal_stiffness", &isThisUnit<1, 0, -2>));
		args.push_back($17); signature.push_back(this_signature("stiffning_rate",           &isThisUnit<0, -1, 0>));
		args.push_back($20); signature.push_back(this_signature("tangential_stiffness",     &isThisUnit<1, 0, -2>));
		args.push_back($23); signature.push_back(this_signature("normal_damping",           &isThisUnit<1, 0, -1>));
		args.push_back($26); signature.push_back(this_signature("tangential_damping",       &isThisUnit<1, 0, -1>));
		args.push_back($29); signature.push_back(this_signature("friction_ratio",           &isAdimensional));
  
		args.push_back($33); signature.push_back(this_signature("x_local_1",                &isAdimensional));
		args.push_back($35); signature.push_back(this_signature("x_local_2",                &isAdimensional));
		args.push_back($37); signature.push_back(this_signature("x_local_3",                &isAdimensional));


		$$ = new FeiDslCaller12<int, int, int, double, double, double, double, double, double,
				   double, double, double>(&add_element_soft_contact, args, signature, "add_element_soft_contact");

		for(int ii = 1;ii <=12; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [4NodeShell_MITC4] with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <L>;
	| TEXTNUMBER exp TYPE FourNodeShellMITC4 WITH NODES
		'(' exp ',' exp ','  exp ','  exp')'
		USE MATERIAL TEXTNUMBER exp
		thickness '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));

		args.push_back($22); signature.push_back(this_signature("thickness", &isLength));

		args.push_back($19); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller7<int, int, int, int, int,
							   double, int>(&add_element_shell_MITC4, args, signature, "add_element_shell_MITC4");

		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [4NodeShell_NewMITC4] with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <L>;
	| TEXTNUMBER exp TYPE FourNodeShellNewMITC4 WITH NODES
		'(' exp ',' exp ','  exp ','  exp')'
		USE MATERIAL TEXTNUMBER exp
		thickness '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));

		args.push_back($22); signature.push_back(this_signature("thickness", &isLength));

		args.push_back($19); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller7<int, int, int, int, int,
							   double, int>(&add_element_shell_NewMITC4, args, signature, "add_element_shell_NewMITC4");

		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
//!FEIDOC add element # <.> type [3NodeShell_ANDES] with nodes (<.>, <.>, <.>) use material # <.> thickness = <l> ;
	| TEXTNUMBER exp TYPE ThreeNodeShellANDES WITH NODES
		'(' exp ',' exp ','  exp ')'
		USE MATERIAL TEXTNUMBER exp
		thickness '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));

		args.push_back($20); signature.push_back(this_signature("thickness", &isLength));

		args.push_back($17); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller6<int, int, int, int, double, int>(&add_element_shell_andes_3node, args, signature, "add_element_shell_andes_3node");

		for(int ii = 1;ii <=6; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [4NodeShell_ANDES] with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <l> ;
	| TEXTNUMBER exp TYPE FourNodeShellANDES WITH NODES
		'(' exp ',' exp ','  exp ','  exp')'
		USE MATERIAL TEXTNUMBER exp
		thickness '=' exp
	{
		args.clear(); signature.clear();
		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));

		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($12); signature.push_back(this_signature("node3", &isAdimensional));
		args.push_back($14); signature.push_back(this_signature("node4", &isAdimensional));

		args.push_back($22); signature.push_back(this_signature("thickness", &isLength));

		args.push_back($19); signature.push_back(this_signature("material", &isAdimensional));

		$$ = new FeiDslCaller7<int, int, int, int, int, double, int>(&add_element_shell_andes_4node, args, signature, "add_element_shell_andes_4node");

		for(int ii = 1;ii <=7; ii++) nodes.pop();
		nodes.push($$);
	}
	//!=========================================================================================================
	//!
	//!FEIDOC add element # <.> type [truss] with nodes (<.>, <.>) use material # <.> cross_section = <length^2> mass_density = <M/L^3> ;
	| TEXTNUMBER exp TYPE truss WITH NODES
		'(' exp ',' exp ')'
		USE MATERIAL TEXTNUMBER exp
		cross_section '=' exp
		mass_density '=' exp
	{
		args.clear(); signature.clear();

		args.push_back($2); signature.push_back(this_signature("number", &isAdimensional));
		args.push_back($8); signature.push_back(this_signature("node1", &isAdimensional));
		args.push_back($10); signature.push_back(this_signature("node2", &isAdimensional));
		args.push_back($15); signature.push_back(this_signature("material", &isAdimensional));
		args.push_back($18); signature.push_back(this_signature("cross_section", &isArea));
		args.push_back($21); signature.push_back(this_signature("mass_density", &isDensity));

		$$ = new FeiDslCaller6<int, int, int, int, double, double>(&add_element_truss, args, signature, "add_element_truss");

		for(int ii = 1;ii <=6; ii++) nodes.pop(); //pop 6 exps
		nodes.push($$);
	}

	//!=========================================================================================================

	;









































 // No need for you to be down here.





















































 //  .... seriously






























































// =====================================================================================
// This section defines the rest of the language, handling variables, control statements
// and so on. In general it should not be touched.
// =====================================================================================

// For each 'exp' there should be a nodes.pop() call and a null pointer guard.
// Segfault ensues if this is not done. 


prompt
	: stmt
	{
		if ($1) // Null pointer safety
		{
			/* Check if the statement is a DslAction in which case we may
			want to report the error code.
			*/
			DslAction *actionpointer = dynamic_cast<DslAction*>($1);
			if (actionpointer != 0)
			{
				try
				{
					actionpointer->value();   /* this evaluates the AST */
				}
				catch (exception& e)
				{
					cout << e.what() << endl;
					cout << "Returning control to user!" << endl << endl;
					newstdin();
				}            }
			else // If Statement is just an expression evaluate it.
			{
				try
				{
					$1->value();   /* this evaluates the AST */
				}
				catch (exception& e)
				{
					cout << e.what() << endl;
					cout << "Returning control to user!" << endl << endl;
					newstdin();
				}
			}
			clear_stack ();

		}
		if (FLAG_interactive_mode == 1)
		{
			cout << prompt;
		}
		$$ = 0;
	}
	| prompt stmt
	{
		if ($2) // Null pointer safety
		{
			/* Check if the statement is a DslAction in which case we may
			want to report the error code.
			*/
			DslAction *actionpointer = dynamic_cast<DslAction*>($2);
			if (actionpointer != 0)
			{
				try
				{
					actionpointer->value();   /* this evaluates the AST */
				}
				catch (exception& e)
				{
					cout << e.what() << endl;
					cout << "Returning control to user!" << endl << endl;
					newstdin();
				}
			}
			else
			{  // If Statement is just an expression evaluate it.
				try
				{
					$2->value();   /* this evaluates the AST */
				}
				catch (exception& e)
				{
					cout << e.what() << endl;
					cout << "Returning control to user!" << endl << endl;
					newstdin();
				}
			}
			clear_stack ();
		}
		if (FLAG_interactive_mode == 1)
		{
			cout << prompt;
		}
		$$ = 0;
	}
	| error
	{
		clear_stack ();

		if ( recovery_mode == 0)
		{
			//Tell lexer to discard every file in stack and go into interactive safe mode
			error_behavior();
		}

		$$ = 0;
	}
	;

exp
	: exp CMP exp
	{   /*  Just a reminder...
		">"  = 1
		"<"  = 2
		"<>" = 3
		"==" = 4
		">=" = 5
		"<=" = 6
		"&" = 7
		"|" = 8
		*/
		switch ($2)
		{
			case 1: // >
				$$ = new GreaterThan($1,$3);
				break;
			case 2: // <
				$$ = new LessThan($1,$3);
				break;
			case 3: // <>
				$$ = new NotEqualTo($1,$3);
				break;
			case 4: // ==
				$$ = new EqualTo($1,$3);
				break;
			case 5: // >=
				$$ = new GreaterThanOrEqualTo($1,$3);
				break;
			case 6: // <=
				$$ = new LessThanOrEqualTo($1,$3);
				break;
			case 7: // &
				$$ = new LogicalAnd($1,$3);
				break;
			case 8: // |
				$$ = new LogicalOr($1,$3);
				break;
			default:
				$$ = new Empty() ;
				cout << "Comparison operator not defined!" << endl;
		}
		nodes.pop();
		nodes.pop();
		nodes.push($$);
	}
	| exp '+' exp
	{
		$$ = new Plus ($1, $3);
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ($$);
	}
	| exp '-' exp
	{
		$$ = new Minus ($1, $3);
		nodes.pop ();  //  The children are handled by Plus so we
		nodes.pop ();  // take them of the allocated nodes stack.
		nodes.push ($$);
	}
	| exp '*' exp
	{
		$$ = new Times ($1, $3);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| exp '/' exp
	{
		$$ = new Divide ($1, $3);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| exp '%' exp
	{
		$$ = new Modulo ($1, $3);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| exp '^' exp
	{
		$$ = new Pow ($1, $3);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| '!' exp
	{
		$$ = new LogicalNot ($2);
		nodes.pop ();  // The same as above.
		nodes.push ($$);
	}
	| IDENT PLUSEQUAL exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Plus (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.push ($$);
	}
	| IDENT MINUSEQUAL  exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Minus (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();
		nodes.pop ();
		nodes.push ($$);
	}
	| IDENT TIMESEQUAL  exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Times (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| IDENT DIVIDEEQUAL exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Divide (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| IDENT MODULUSEQUAL  exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Modulo (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| IDENT POWEQUAL  exp
	{
		Expression* varref = new VariableReference(*$1, global_variables);
		$$ = new Assign(*$1, new Pow (varref, $3), global_variables, locked_global_variables);
		nodes.pop ();  // The same as above.
		nodes.pop ();
		nodes.push ($$);
	}
	| '(' exp ')'
	{
		$$ = $2;
	}
	| '-' exp %prec UMINUS
	{
		$$ = new Minus (new Number(0.0, $2 -> value().Getunit() ), $2);
		nodes.pop ();
		nodes.push ($$);
	}
	| NUMBER
	{
		$$ = new Number (*$1);
		nodes.push ($$);
	}
	| IDENT
	{

		$$ = new VariableReference(*$1,global_variables);
		nodes.push($$);
	}
	| IDENT '=' exp
	{
		$$ = new Assign(*$1, $3, global_variables, locked_global_variables);
		nodes.pop();
		nodes.push($$);
	}
	| IDENT IN IDENT
	{
		$$ = new ConvertUnits(*$1, *$3, global_variables);

		nodes.push($$);
	}
	| FUNCTION '(' exp ')'
	{
		$$ = new FeiMath(*$1, $3);
		nodes.pop();
		nodes.push($$);
	}
	| ISUNITTYPE '(' exp ')'
	{
		$$ = new UnitTypeTest(*$1, $3);
		nodes.pop();
		nodes.push($$);
	}
	| STRING
	{
		string temp = *$1;
		temp.erase(0, 1);                               //remove quotes
		temp.erase(temp.length()-1, temp.length());     //remove quotes
		$$ = new FeiString(temp);
		nodes.push($$);
	}
	| VECTOR '(' explist ')'
	{
		$$ = $3;//new FeiVector($3);
		//nodes.push($$);
	}
	| NUMBER_OF_NODES
	{
		args.clear(); signature.clear();
		$$ = new FeiDslCaller0<>(&query_domain_number_of_nodes, args, signature, "query_domain_number_of_nodes");
		nodes.push($$);
	}
	| NUMBER_OF_ELEMENTS
	{
		$$ = new FeiDslCaller0<>(&query_domain_number_of_elements, args, signature, "query_domain_number_of_elements");
		nodes.push($$);
	}
	| CURRENT_TIME
	{
		$$ = new FeiDslCaller0<double>(&query_domain_current_time, args, signature, "query_domain_current_time");
		nodes.push($$);
	}
	| NUMBER_OF_SP_CONSTRAINTS
	{
		$$ = new FeiDslCaller0<>(&query_domain_number_of_sp_constraints, args, signature, "query_domain_number_of_sp_constraints");
		nodes.push($$);
	}
	| NUMBER_OF_MP_CONSTRAINTS
	{
		$$ = new FeiDslCaller0<>(&query_domain_number_of_mp_constraints, args, signature, "query_domain_number_of_mp_constraints");
		nodes.push($$);
	}
	| NUMBER_OF_LOADS
	{
		$$ = new FeiDslCaller0<>(&query_domain_number_of_loads, args, signature, "query_domain_number_of_loads");
		nodes.push($$);
	}
	| IS_PARALLEL
	{
		$$ = new FeiDslCaller0<>(&query_domain_is_parallel, args, signature, "query_domain_is_parallel");
		nodes.push($$);
	}
	| SIMULATE_EXIT_FLAG
	{
		$$ = new FeiDslCaller0<>(&query_simulate_flag, args, signature, "query_simulate_flag");
		nodes.push($$);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Model Query
	/////////////////////////////////////////////////////////////////////////////////////////////
	| NODE '[' exp ']' '.' IDENT
	{
		args.clear();
		signature.clear();

		Expression* which_coordinate;
		which_coordinate = dof2number(*$6);
		if (which_coordinate->value() == -100)
		{
			string msg = "Unknown node data field request -> ";
			msg += *$6;
			throw RunTimeException( msg);
			$$ = new Empty();
		}
		else
		{
			nodes.push(which_coordinate);

			args.push_back($3); signature.push_back(this_signature("node_number", &isAdimensional));
			args.push_back(which_coordinate); signature.push_back(this_signature("coordinate", &isAdimensional));

			$$ = new FeiDslCaller2<int,int,Quantity>(&get_nodal_coordinate,args,signature,"get_nodal_coordinate");
		}

		nodes.pop();
		nodes.pop();
		nodes.push($$);
	}
//	| dsl
//	{
//		$$ = $1;
//		args.clear();
//		signature.clear();
//	}
	;

stmt
	: ';'
	{
		$$ = new Empty();
		nodes.push($$);
	}
	| exp ';'
	{
		$$ = $1;
	}
	| dsl ';'
	{
		$$ = $1;
		args.clear();
		signature.clear();
	}
	
	//!=========================================================================================================
	//!
	//!FEIDOC if (.) { };
	| IF exp cpd_stmt
	{
		Comparison* ptr_comp = dynamic_cast<Comparison*>($2);
		if (!ptr_comp) // Check whether ptr_comp is a valid Comparison* pointer
		{
			cout << "If statement requieres a comparison." << endl;
			$$ = new Empty();
			nodes.pop();
			nodes.pop();
			nodes.push($$);
		}
		else
		{
			$$ = new IfStatement(ptr_comp, $3, NULL);
			nodes.pop();
			nodes.pop();
			nodes.push($$);
		}
	}
	//!=========================================================================================================
	//!
	//!FEIDOC if (.) { } else {};
   | IF exp cpd_stmt ELSE cpd_stmt
   {
		Comparison* ptr_comp = dynamic_cast<Comparison*>($2);
		if (!ptr_comp) // Check whether ptr_comp is a valid Comparison* pointer
		{
			cout << "If statement requires a comparison." << endl;
			$$ = new Empty();
			nodes.pop();
			nodes.pop();
			nodes.pop();
			nodes.push($$);

		}
		else
		{
			$$ = new IfStatement(ptr_comp, $3, $5);
			nodes.pop();
			nodes.pop();
			nodes.pop();
			nodes.push($$);
		}
	}
	//!=========================================================================================================
	//!
	//!FEIDOC while (.) { };
	| WHILE exp cpd_stmt
	{
		Comparison* ptr_comp = dynamic_cast<Comparison*>($2);
		if (!ptr_comp)  // Check whether ptr_comp is a valid Comparison* pointer
		{
			cout << "While statement requires a comparison." << endl;
			$$ = new Empty();
			nodes.pop();
			nodes.pop();
			nodes.push($$);

		}
		else
		{
			$$ = new WhileStatement(ptr_comp, $3);
			nodes.pop();
			nodes.pop();
			nodes.push($$);
		}
	}
	| BYE
	{
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}
		if($1->compare("quit") == 0) cout << "quit: \n/kwit/ \nverb \n   1. leave (a place), usually permanently. \n " << endl ;
		if($1->compare("exit") == 0) cout << "All exits are marked by the \"exit\" word..."<< endl ;
		if($1->compare("bye") == 0) cout << "How polite! Bye, have a nice day!" << endl ;

		wipe_model();

		$$ = new Empty();
		nodes.push($$);

		//exit(0);
		return(0);
	}
	| RESTART
	{
		args.clear(); signature.clear();
		if (FLAG_generate_cpp_output == 1)
		{
			*FeiDslCallerBase::cppfile << "}" << endl;
		}

		$$ = new FeiDslCaller0<>(&wipe_model, args, signature, "wipe_model");
		nodes.push($$);
	}
	| REQUIRE IDENT
	{
		$$ = new Empty();
		nodes.pop();
		nodes.push($$);
	}
	;


cpd_stmt
	: '{' '}'
	{
		// empty statement generates an  empty expression
		$$ = new Empty();
		nodes.push($$);
	}
	| '{' list '}'
	{
		// Used to initialize a list of statements (compound statement)
		$$ = $2;
	}
	;

list
	: stmt
	{
		$$ = $1;
	}
	| stmt list
	{
		if ($2 == NULL)
			$$ = $1;
		else
		{
			$$ = new ExpressionList($1, $2);
			nodes.pop();
			nodes.pop();
			nodes.push($$);
		}
	}
	;




/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */
/* ========================================================================================================= */

%%

// Deletes all the nodes that were allocated
void clear_stack ()
{
  while (!nodes.empty ())
  {
	delete nodes.top ();
	nodes.pop ();
  }
}

//Define what happens when an error is encountered.
//FIXME: More verbose error reporting... line number, column, etc. (jaabell)
//FIXME: command suggestions are not working properly.
//NOTE: This doesnt work on windows
namespace yy {
	void feiparser::error(location const& loc, const string& s)
	{
		cerr << " <!!!>  Error at ---> " << loc << ": " << s << endl << endl;
		// printf("\033[6;3HHello\n");
		ifstream file_for_error_reporting(loc.begin.filename->c_str());

		unsigned int begin_line = loc.begin.line;
		unsigned int end_line = loc.end.line;
		unsigned int begin_column = loc.begin.column;
		unsigned int end_column = loc.end.column;

		string line;
		string fullcommand;
		unsigned int linenumber = 1;
		unsigned int lastprintedline = 1;
		while(linenumber <= end_line)
		{
			getline(file_for_error_reporting, line);

			if (linenumber >= begin_line)
			{
				if(linenumber == begin_line)
				{
					if( line.size() <= size_t(begin_column) )
						line = "";
					if (line.size() > size_t(begin_column) )
						line = line.substr(begin_column);
				}

				fullcommand += line;

				if(line.size() > 0)
				{
					if(linenumber > lastprintedline + 1)
						cerr << ">skip " << setw(3) << linenumber - lastprintedline - 1<< endl;
					cerr << "line " << setw(4) << linenumber << ": " << line << endl;
					lastprintedline = linenumber;
				}
				if (linenumber == end_line)
				{
					unsigned int columnnumber = 1;

					while(columnnumber <= end_column+10)
					{
						cerr << " ";
						columnnumber++;
					}
					cerr << "^";
					//cerr << endl;
				}
			}
			linenumber ++;
		}

		file_for_error_reporting.close();
		//cerr << endl;
		//cerr << "Full command: " << fullcommand << endl << endl;
//        cmd_advisor.Suggest(fullcommand);

		cout << endl;
	}
}

// Transforms a dofstring into a dof number according to conventions described in the manual... and
// evident from reading below.
Number* dof2number(string dof)
{
	if (dof.compare("ux") == 0) return new Number(1, ESSIunits::unitless);
	if (dof.compare("uy") == 0) return new Number(2, ESSIunits::unitless);
	if (dof.compare("uz") == 0) return new Number(3, ESSIunits::unitless);
	if (dof.compare("rx") == 0) return new Number(4, ESSIunits::unitless);
	if (dof.compare("ry") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("rz") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("Ux") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("Uy") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("Uz") == 0) return new Number(7, ESSIunits::unitless);
	if (dof.compare("p")  == 0) return new Number(4, ESSIunits::unitless);

	return new Number(-100, ESSIunits::unitless);
}


// Returns the appropiate unit checking function pointer according to the
// type of DOF specified in dof string.
UnitCheckerFunctionPointerType dof2signature(string dof)
{
	if (dof.compare("ux") == 0) return &isLength;
	if (dof.compare("uy") == 0) return &isLength;
	if (dof.compare("uz") == 0) return &isLength;
	if (dof.compare("rx") == 0) return &isAdimensional;
	if (dof.compare("ry") == 0) return &isAdimensional;
	if (dof.compare("rz") == 0) return &isAdimensional;
	if (dof.compare("Ux") == 0) return &isLength;
	if (dof.compare("Uy") == 0) return &isLength;
	if (dof.compare("Uz") == 0) return &isLength;
	if (dof.compare("p") == 0)  return &isPressure;

	return &isAdimensional;
}

// Transforms a force string into a dof number.
Number* force2number(string dof)
{
	if (dof.compare("Fx") == 0) return new Number(1, ESSIunits::unitless);
	if (dof.compare("Fy") == 0) return new Number(2, ESSIunits::unitless);
	if (dof.compare("Fz") == 0) return new Number(3, ESSIunits::unitless);
	if (dof.compare("Mx") == 0) return new Number(4, ESSIunits::unitless);
	if (dof.compare("My") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("Mz") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("F_fluid_x") == 0) return new Number(5, ESSIunits::unitless);
	if (dof.compare("F_fluid_y") == 0) return new Number(6, ESSIunits::unitless);
	if (dof.compare("F_fluid_z") == 0) return new Number(7, ESSIunits::unitless);

	return new Number(-100, ESSIunits::unitless);
}

// Returns the appropiate unit checking function pointer according to the
// type of DOF specified in dof string.
UnitCheckerFunctionPointerType force2signature(string dof)
{
	if (dof.compare("Fx") == 0) return &isForce;
	if (dof.compare("Fy") == 0) return &isForce;
	if (dof.compare("Fz") == 0) return &isForce;
	if (dof.compare("Mx") == 0) return &isTorque;
	if (dof.compare("My") == 0) return &isTorque;
	if (dof.compare("Mz") == 0) return &isTorque;
	if (dof.compare("F_fluid_x") == 0) return &isForce;
	if (dof.compare("F_fluid_y") == 0) return &isForce;
	if (dof.compare("F_fluid_z") == 0) return &isForce;

	return &isAdimensional;
}




// Returns appropriate stiffness which corresponds to a certain dof name.
UnitCheckerFunctionPointerType dof2stiffnesschecker(string dof)
{
	if (dof.compare("ux") == 0) return &isThisUnit<1, 0, -2>;   //Check for units of Force / Length = kg/s^2;
	if (dof.compare("uy") == 0) return &isThisUnit<1, 0, -2>;
	if (dof.compare("uz") == 0) return &isThisUnit<1, 0, -2>;
	if (dof.compare("rx") == 0) return &isTorque;
	if (dof.compare("ry") == 0) return &isTorque;
	if (dof.compare("rz") == 0) return &isTorque;
	if (dof.compare("Ux") == 0) return &isThisUnit<1, 0, -2>;
	if (dof.compare("Uy") == 0) return &isThisUnit<1, 0, -2>;
	if (dof.compare("Uz") == 0) return &isThisUnit<1, 0, -2>;
	if (dof.compare("p") == 0)  return &isPressure;

	return &isAdimensional;
}

// Returns appropriate bardet-type code
int bardet_type_parse(string type)
{
	if (type.compare("CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 0;
	if (type.compare("DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL") == 0) return 1;
	if (type.compare("DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 2;
	if (type.compare("UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL") == 0) return 3;
	if (type.compare("UNDRAINED_TRIAXIAL_LOADING_STRESS_CONTROL") == 0) return 4;
	if (type.compare("UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL") == 0) return 5;

	return -1;
}


void set_model_name(char* cstring)
{
	string modelNameStringFromInput(cstring);
	//ModelName = modelNameStringFromInput;
	define_model_name(modelNameStringFromInput);
}


void set_outcppfile_name(string newfilename)
{
	out_cpp_filename = newfilename;
	if (out_cpp_filename.find(".cpp") == string::npos )
	{
		out_cpp_filename += ".cpp";
	}

	if(FLAG_generate_cpp_output)
	{
		// Point output to this new file.
		fstream *outputcppfile = new fstream(out_cpp_filename.c_str(), ios::out);
		FeiDslCallerBase::cppfile = outputcppfile;

		*outputcppfile << "#include \"ESSI_API.h\"" << endl ;
		*outputcppfile << endl;
		*outputcppfile << endl;
		*outputcppfile << "int main()" << endl;
		*outputcppfile << "{" << endl;
	}
}

void set_profiling_results_filename(char* cstring)
{
	string filename(cstring);
	cout << "Setting timing report file to : " << filename << endl;
	profiling_results_filename = filename;
}


void set_dry_run_mode(int flag)
{
	if( flag <= 0)
	{
		FeiDslCallerBase::dry_run = false;
	}
	else
	{
		FeiDslCallerBase::dry_run = true;
		cout << "\n\n\n";
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		cout << "!! NOTE: DRY RUN. No actual calls to ESSI API are being made. !!\n";
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		cout << "\n\n\n";
	}
}























// That's all folks.



