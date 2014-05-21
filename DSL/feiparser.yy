/*
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
    #include "dsl_actions.h"
    #include "siunit.h"
    #include "quantity.h"
    #include "unitdefinitions.h"
    #include "feidslcaller.h"
    #include "feimath.h"

/*     #include "command_suggestion.h" */

    // CompGeoMech API
    typedef std::vector<Number*> VectorNumber;          // This hack gets rid of a collision between std::vector and Vector classes used within FEI
    #include "../API/CPPIncludes.h"

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
    Quantity q0(0.0, unitless);

    extern int yylineno;
    extern char* curfilename;
    extern const string thePrompt = "ESSI> ";
    extern int newfile(char* fn);
    extern int newstdin(void);
    extern int popfile(void);
    extern int error_behavior(void);

    void set_outcppfile_name(string newfilename);

    //Command line options flags
    extern int FLAG_interactive_mode;
    extern int FLAG_generate_cpp_output;
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
%token IDENT NUMBER STRING EOL ISUNITTYPE FUNCTION
%token IF THEN ELSE WHILE DO LET EXITSAFEMODE CONTINUE BYE IN REQUIRE
%token PLUSEQUAL MINUSEQUAL TIMESEQUAL DIVIDEEQUAL MODULUSEQUAL POWEQUAL
%token NUMBER_OF_NODES NUMBER_OF_ELEMENTS CURRENT_TIME NUMBER_OF_SP_CONSTRAINTS NUMBER_OF_MP_CONSTRAINTS NUMBER_OF_LOADS

// Inform Bison about the type of each terminal and non-terminal (rule or token)
%type <exp>     exp stmt list cpd_stmt dsl CMD_add CMD_fix CMD_misc CMD_define CMD_remove
%type <exp>     ADD_material ADD_element
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
%token ADD NODE ELEMENT MATERIAL LOAD TIMEHISTORY IMPOSEDMOTION DAMPING DAMPINGTYPE CONSTRAINT DRM SAVEFORCES RESTOREFORCES SECTION LOADPATTERN PENALTYDISPLACEMENT LOADVALUE
%token ELEMENTNAME MATERIALNAME
%token ACCELERATION_FIELD
%token FIX FREE REMOVE
%token DEFINE ALGORITHM ALGNAME CONVERGENCE_TEST TESTNAME SOLVER SOLVERNAME
%token DYNAMICINTEGRATOR DYNAMICINTEGRATOR_HHT DYNAMICINTEGRATOR_NEWMARK STATICINTEGRATOR STATICINTEGRATOR_DISPLACEMENT
%token SIMULATE STATIC DYNAMIC USING TRANSIENT EIGEN time_step number_of_modes VARIABLETRANSIENT maximum_time_step minimum_time_step number_of_iterations
%token AT ALL AND WITH TEXTDOFS NEW TEXTNUMBER USE TO DOF TEXTWITH NODES FORCE INTEGRATIONPOINTS dof RESPONSE
%token LOADING STAGE STEPS TYPE DOFS FACTOR INCREMENT
%token TH_GROUNDMOTION TH_LINEAR TH_PATH_SERIES TH_PATH_TIME_SERIES TH_CONSTANT TH_FROM_REACTIONS
%token self_weight surface load_value
%token scale_factor displacement_scale_unit velocity_scale_unit acceleration_scale_unit number_of_steps number_of_drm_nodes number_of_drm_elements x_max x_min y_max y_min z_max z_min element_file nodes_file displacement_file acceleration_file velocity_file force_file series_file time_series_file MAGNITUDES MAGNITUDE
%token strain_increment_size maximum_strain  number_of_times_reaching_maximum_strain constitutive testing constant mean triaxial drained undrained simple shear
%token number_of_subincrements maximum_number_of_iterations tolerance_1 tolerance_2 strain stress control Guass points Gauss each point single value
// Additionally these tokens carry a string type (the above carry no type)
// This is becuase there are several options to what this token may be
// and the program branches depending on these.
// ie. DOF may take the values {ux, uy, uz, Ux, Uy, Uz, rx, ry, rz, p} as
// defined in the lexer file (feiparser.l).
%type <ident> DOF ELEMENTNAME MATERIALNAME ALGNAME TESTNAME SOLVERNAME FORCE
%type <ident> DAMPINGTYPE



// Tokens for elements
%token EightNodeBrick TwentySevenNodeBrick EightNodeBrick_upU TwentyNodeBrick_uPU TwentyNodeBrick TwentyNodeBrickElastic EightNodeBrick_up variable_node_brick_8_to_27
%token EightNodeBrickElastic TwentySevenNodeBrickElastic beam_displacement_based beam_elastic beam_elastic_lumped_mass penalty penalty_for_applying_generalized_displacement beam_9dof_elastic
%token FourNodeShellMITC4 FourNodeShellNewMITC4 ThreeNodeShellANDES FourNodeShellANDES truss contact
%token EightNodeBrickLT TwentySevenNodeBrickLT

// Element options tokens
%token porosity  alpha rho_s rho_f k_x k_y k_z K_s K_f pressure cross_section shear_modulus torsion_Jx bending_Iz bending_Iy IntegrationRule number_of_integration_points stiffness normal_stiffness tangential_stiffness
%token friction_ratio maximum_gap
%token xz_plane_vector joint_1_offset joint_2_offset direction contact_plane_vector

// Tokens for sections
%token MembranePlateFiber ElasticMembranePlate

// Section options tokens
%token thickness

// Tokens for materials
%token linear_elastic_isotropic_3d vonmises_perfectly_plastic vonmises_isotropic_hardening vonmises_linear_kinematic_hardening vonmises_linear_kinematic_hardening_accelerated vonmises_kinematic_hardening vonmises_perfectly_plastic_accelerated vonmises_isotropic_hardening_accelerated vonmises_kinematic_hardening_accelerated
%token sanisand2008 camclay camclay_accelerated sanisand2004 druckerprager_isotropic_hardening druckerprager_isotropic_hardening_accelerated druckerprager_kinematic_hardening druckerprager_kinematic_hardening_accelerated
%token druckerprager_perfectly_plastic druckerprager_perfectly_plastic_accelerated linear_elastic_crossanisotropic uniaxial_concrete02 uniaxial_elastic_1d uniaxial_steel01 uniaxial_steel02 pisano 
%token vonmises_perfectly_plastic_LT pisanoLT New_PisanoLT linear_elastic_isotropic_3d_LT
// Material options tokens
%token mass_density elastic_modulus viscoelastic_modulus poisson_ratio von_mises_radius druckerprager_angle
%token armstrong_frederick_ha armstrong_frederick_cr initial_confining_stress isotropic_hardening_rate kinematic_hardening_rate poisson_ratio_h_v poisson_ratio_h_h shear_modulus_h_v elastic_modulus_horizontal elastic_modulus_vertical pressure_reference_p0
%token initial_void_ratio initial_shear_modulus initial_bulk_modulus sanisand2008_Pat sanisand2008_k_c sanisand2008_K0 sanisand2008_alpha_cc sanisand2008_c sanisand2008_xi sanisand2008_lambda sanisand2008_ec_ref sanisand2008_m sanisand2008_h0 sanisand2008_ch sanisand2008_nb sanisand2008_A0 sanisand2008_nd sanisand2008_p_r sanisand2008_rho_c sanisand2008_theta_c sanisand2008_X sanisand2008_z_max sanisand2008_cz sanisand2008_p0 sanisand2008_p_in sanisand2008_G0 sanisand2004_K0
%token sanisand2004_Pat e0 sanisand2004_G0 sanisand2004_p_cut sanisand2004_Mc sanisand2004_c sanisand2004_lambda_c sanisand2004_xi sanisand2004_ec_ref sanisand2004_m sanisand2004_h0 sanisand2004_ch sanisand2004_nb sanisand2004_A0 sanisand2004_nd sanisand2004_z_max sanisand2004_cz
%token reference_void_ratio critical_stress_ratio_M minimum_bulk_modulus initial_mean_pressure yield_strength strain_hardening_ratio compressive_strength strain_at_compressive_strength
%token crushing_strength strain_at_crushing_strength tensile_strength tension_softening_stiffness
%token M_in kd_in xi_in h_in m_in beta_min



// For acceleration field
%token ax ay az

// Convergence test option tokens
%token verbose_level maximum_iterations tolerance

// Greek
%token beta gamma kappa lambda delta

%token DOMAIN_ startTime endTime Period Phase Amplitude frequency MaxTime frequency1 frequency2 frequency3 frequency4

// For damping
%token stiffness_to_use_opt DAMPING_RAYLEIGH DAMPING_CAUGHEY3 DAMPING_CAUGHEY4 a0 a1 stiffness_to_use a2 a3 a4 R0 cR1 cR2
%type <ident> stiffness_to_use_opt

// For mass
%token MASS mx my mz Imx Imy Imz

// for constraints
%token equaldof master slave dof_to_constrain of

// for output control
%token OUTPUT BINARY TEXT ENABLE DISABLE

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
    //!FEIDOC print <.> in <.>;
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
    //!FEIDOC print domain;
    | CMD_print DOMAIN_
    {
        //theDomain.Print(cerr);
        cerr << "Not currently working. Should be implemented as API function and used here." << endl;
        $$ = new Empty();
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
    //!FEIDOC check element # <.>;
/*    | CMD_check ELEMENT TEXTNUMBER exp
    {
        //theDomain.getElement( $4->value().Getvalue() ) -> report("Element report:\n");
        $$ = new Empty();
        nodes.pop();
        nodes.push($$);
    }*/
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
    //!FEIDOC print material # <.>;
    | CMD_print MATERIAL TEXTNUMBER exp
    {
        cerr << "Not currently working. Should be implemented a API function and used here." << endl;
        //theDomain.getNDMaterial( $4->value().Getvalue() ) -> Print(cerr,0);
        $$ = new Empty();
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
    /*  MySQL Stuff out by Jose on Feb 5, 2014



    //!=========================================================================================================
    //!
    //!FEIDOC save response of model type [mysql] databasename <.> host <.> username <.> password <.> port <.> socket <.>;
    | CMD_save RESPONSE of MODEL TYPE mysql databasename exp host exp username exp password exp port exp socket exp
    {
        args.clear(); signature.clear();

        args.push_back($8); signature.push_back(this_signature("databaseName", &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("host", &isAdimensional));
        args.push_back($12); signature.push_back(this_signature("username", &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("password", &isAdimensional));
        args.push_back($16); signature.push_back(this_signature("port", &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("socket", &isAdimensional));


        $$ = new FeiDslCaller6<string, string,
                               string, string, unsigned int,
                               string>(&save_response_of_model_mysql_format, args, signature,"save_response_of_model_mysql_format");

        for(int ii = 1;ii <=6; ii++) nodes.pop();
        nodes.push($$);
        }
    //!=========================================================================================================
    //!
    //!FEIDOC save state of model type [mysql] databasename <.> host <.> username <.> password <.> port <.> socket <.>;
    | CMD_save STATE of MODEL TYPE mysql databasename exp
    host exp username exp password exp port exp socket exp
    {
        args.clear(); signature.clear();

        args.push_back($8); signature.push_back(this_signature("databaseName", &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("host", &isAdimensional));
        args.push_back($12); signature.push_back(this_signature("username", &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("password", &isAdimensional));
        args.push_back($16); signature.push_back(this_signature("port", &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("socket", &isAdimensional));


        $$ = new FeiDslCaller6<string, string, string,
                               string, unsigned int,
                               string>(&save_state_of_model_mysql_format, args, signature,"save_state_of_model_mysql_format");

        for(int ii = 1;ii <=6; ii++) nodes.pop();
        nodes.push($$);
        }
    //!=========================================================================================================
    //!
    //!FEIDOC restore state of model type [mysql] databasename <.> host <.> username <.> password <.> port <.> socket <.>;
    | CMD_restore STATE of MODEL TYPE mysql databasename exp host exp username exp password exp port exp socket exp
    {
        args.clear(); signature.clear();

        args.push_back($8); signature.push_back(this_signature("databaseName", &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("host", &isAdimensional));
        args.push_back($12); signature.push_back(this_signature("username", &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("password", &isAdimensional));
        args.push_back($16); signature.push_back(this_signature("port", &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("socket", &isAdimensional));


        $$ = new FeiDslCaller6<string, string, string,
                               string, unsigned int,
                               string>(&restore_state_of_model_mysql_format, args, signature,"restore_state_of_model_mysql_format");

        for(int ii = 1;ii <=6; ii++) nodes.pop();
        nodes.push($$);
        }

            End of MySQL stuff

        */
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
    //!FEIDOC add load # <.> to node # <.> type [path_time_series] [FORCETYPE] = <force or moment> series_file = "STRING";
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
    //!FEIDOC add load # <.> to node # <.> type [path_series] [FORCETYPE] = <force or moment> time_step = <time> series_file = "STRING";
    | ADD LOAD TEXTNUMBER exp TO NODE TEXTNUMBER exp TYPE TH_PATH_SERIES
                FORCE '=' exp
                time_step '=' exp
                series_file '=' STRING
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
        args.push_back(new Number(-1, unitless));         signature.push_back(this_signature("to_node", &isAdimensional));

        $$ = new FeiDslCaller2<int, int>(&add_load_to_node_from_reaction, args, signature, "add_load_to_node_from_reaction");

        nodes.pop(); //pop 1 exps
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add imposed motion # <.> to node # <.> dof [DOFTYPE] time_step = <t> displacement_scale_unit = <length> displacement_file = "filename" velocity_scale_unit = <velocity> velocity_file = "filename" acceleration_scale_unit = <acceleration> acceleration_file = "filename";
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
    //!FEIDOC add imposed motion # <.> to node # <.> dof [DOFTYPE] displacement_scale_unit = <displacement> displacement_file = "filename" velocity_scale_unit = <velocity> velocity_file = "filename" acceleration_scale_unit = <acceleration> acceleration_file = "filename";
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
    //!FEIDOC add penalty displacement # <.> to element # <.>  [DOFTYPE] = <length> type linear;
    | ADD PENALTYDISPLACEMENT TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp DOF '=' exp TYPE TH_LINEAR
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*$9);

        args.push_back($4); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back($11); signature.push_back(this_signature(*$9,   &isLength));


       $$ = new FeiDslCaller4<int, int, int, double>(&add_penalty_displacement_time_history_linear, args,signature, "add_penalty_displacement_time_history_linear");


        for(int i = 1; i <= 3; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add penalty displacement # <.> to element # <.> dof [DOFTYPE] type path_series time_step = <time> scale_factor = <.> series_file = "STRING";
    | ADD PENALTYDISPLACEMENT TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp dof DOF TYPE TH_PATH_SERIES
      time_step '=' exp
      scale_factor '=' exp
      series_file '=' exp
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*$10);

        args.push_back($4); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back($15); signature.push_back(this_signature("time_step",   &isTime));
        args.push_back($18); signature.push_back(this_signature("scale_factor",   &isAdimensional));
        args.push_back($21); signature.push_back(this_signature("series_file",   &isAdimensional));


       $$ = new FeiDslCaller6<int, int, int, double, double, string>(&add_penalty_displacement_time_history_path_series, args,signature, "add_penalty_displacement_time_history_path_series");


        for(int i = 1; i <= 5; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add penalty displacement # <.> to element # <.> dof [DOFTYPE] type path_time_series scale_factor = <.> series_file = "STRING";
    | ADD PENALTYDISPLACEMENT TEXTNUMBER exp TO ELEMENT TEXTNUMBER exp dof DOF TYPE TH_PATH_TIME_SERIES
      scale_factor '=' exp
      series_file '=' exp
    {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*$10);

        args.push_back($4); signature.push_back(this_signature("number",        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("node",          &isAdimensional));
        args.push_back(dof_number); signature.push_back(this_signature("dof",   &isAdimensional));
        args.push_back($15); signature.push_back(this_signature("scale_factor",   &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("series_file",   &isAdimensional));


       $$ = new FeiDslCaller5<int, int, int, double, string>(&add_penalty_displacement_time_history_path_time_series, args,signature, "add_penalty_displacement_time_history_path_time_series");


        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add damping # <.> type [Rayleigh] with a0 = <time> a1 = <1/time> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
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
    //!FEIDOC add damping # <.> type [Caughey3rd] with a0 = <time> a1 = <1/time> a2 = <.> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
    //WARNING: unit for a2 should be implemented
    | ADD DAMPING TEXTNUMBER exp TYPE DAMPING_CAUGHEY3 WITH a0 '=' exp a1 '=' exp a2 '=' exp stiffness_to_use '=' stiffness_to_use_opt
    {
        args.clear(); signature.clear();

        args.push_back($4); signature.push_back(this_signature("number",   &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("a0",      &isTime));
        args.push_back($13); signature.push_back(this_signature("a1",      &isFrequency));
        args.push_back($16); signature.push_back(this_signature("a2",      &isAdimensional));
        args.push_back(new FeiString(*$19)); signature.push_back(this_signature("stiffness_to_use",    &isAdimensional));

        $$ = new FeiDslCaller5<int,double,double,double,string>(&add_damping_caughey3rd, args, signature, "add_damping_caughey3rd");

        for(int i = 1; i <= 4; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add damping # <.> type [Caughey4th] with a0 = <time> a1 = <1/time> a2 = <.> a3 = <.> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;
    //WARNING: unit for a2 and a3 should be implemented
    | ADD DAMPING TEXTNUMBER exp TYPE DAMPING_CAUGHEY4 WITH a0 '=' exp a1 '=' exp a2 '=' exp a3 '=' exp stiffness_to_use '=' stiffness_to_use_opt
    {
        args.clear(); signature.clear();

        args.push_back($4); signature.push_back(this_signature("number",   &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("a0",      &isTime));
        args.push_back($13); signature.push_back(this_signature("a1",      &isFrequency));
        args.push_back($16); signature.push_back(this_signature("a2",      &isAdimensional));
        args.push_back($19); signature.push_back(this_signature("a3",      &isAdimensional));
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
    //!FEIDOC add constraint equal dof with master node # <.> and slave node # <.> dof to constrain <.>;
    | ADD CONSTRAINT equaldof WITH master NODE TEXTNUMBER exp AND slave NODE TEXTNUMBER exp dof_to_constrain dofchain
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
    //!FEIDOC add domain reduction method loading time_step = <time> scale_factor = <.> number_of_steps = <.> number_of_drm_nodes = <.> number_of_drm_elements = <.> x_max = <length> x_min = <length> y_max = <length> y_min = <length> z_max = <length> z_min = <length> element_file = "filename" nodes_file = "filename" displacement_file = "filename" acceleration_file = "filename";
    | ADD DRM LOADING TEXTNUMBER exp
                      time_step '=' exp
                      scale_factor '=' exp
                      number_of_steps '=' exp
                      number_of_drm_nodes '=' exp
                      number_of_drm_elements '=' exp
                      x_max '=' exp
                      x_min '=' exp
                      y_max '=' exp
                      y_min '=' exp
                      z_max '=' exp
                      z_min '=' exp
                      element_file '=' exp
                      nodes_file '=' exp
                      displacement_file '=' exp
                      acceleration_file '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($5); signature.push_back(this_signature("pattern_number",           &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("time_step",           &isTime));
        args.push_back($11); signature.push_back(this_signature("scale_factor",           &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("number_of_steps",           &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("number_of_drm_nodes",           &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("number_of_drm_elements",           &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back($26); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back($29); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back($32); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back($35); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back($38); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back($41); signature.push_back(this_signature("element_file",           &isAdimensional));
        args.push_back($44); signature.push_back(this_signature("nodes_file",           &isAdimensional));
        args.push_back($47); signature.push_back(this_signature("displacement_file",           &isAdimensional));
        args.push_back($50); signature.push_back(this_signature("acceleration_file",           &isAdimensional));

        $$ = new FeiDslCaller16<int, double, double, int, int, int, double, double, double, double, double, double, string, string, string, string>(&add_load_pattern_domain_reduction_method, args, signature, "add_load_pattern_domain_reduction_method");

        for(int i = 1; i <= 16; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add domain reduction method loading save forces time_step = <time> scale_factor = <.> number_of_steps = <.> number_of_drm_nodes = <.> number_of_drm_elements = <.> x_max = <length> x_min = <length> y_max = <length> y_min = <length> z_max = <length> z_min = <length> element_file = "filename" nodes_file = "filename" displacement_file = "filename" acceleration_file = "filename" force_file = "filename";
    | ADD DRM LOADING SAVEFORCES TEXTNUMBER exp
                      time_step '=' exp
                      scale_factor '=' exp
                      number_of_steps '=' exp
                      number_of_drm_nodes '=' exp
                      number_of_drm_elements '=' exp
                      x_max '=' exp
                      x_min '=' exp
                      y_max '=' exp
                      y_min '=' exp
                      z_max '=' exp
                      z_min '=' exp
                      element_file '=' exp
                      nodes_file '=' exp
                      displacement_file '=' exp
                      acceleration_file '=' exp
                      force_file '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($6); signature.push_back(this_signature("pattern_number",           &isAdimensional));
        args.push_back($9); signature.push_back(this_signature("time_step",           &isTime));
        args.push_back($12); signature.push_back(this_signature("scale_factor",           &isAdimensional));
        args.push_back($15); signature.push_back(this_signature("number_of_steps",           &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("number_of_drm_nodes",           &isAdimensional));
        args.push_back($21); signature.push_back(this_signature("number_of_drm_elements",           &isAdimensional));
        args.push_back($24); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back($27); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back($30); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back($33); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back($36); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back($39); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back($42); signature.push_back(this_signature("element_file",       &isAdimensional));
        args.push_back($45); signature.push_back(this_signature("nodes_file",         &isAdimensional));
        args.push_back($48); signature.push_back(this_signature("displacement_file",  &isAdimensional));
        args.push_back($51); signature.push_back(this_signature("acceleration_file",  &isAdimensional));
        args.push_back($54); signature.push_back(this_signature("force_file",         &isAdimensional));

        $$ = new FeiDslCaller17<int, double, double, int, int, int, double, double, double, double, double, double, string, string, string, string, string>(&add_load_pattern_domain_reduction_method_save_forces, args, signature, "add_load_pattern_domain_reduction_method_save_forces");

        for(int i = 1; i <= 17; i++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add domain reduction method loading restore forces time_step = <time> scale_factor = <.> number_of_steps = <.> number_of_drm_nodes = <.> number_of_drm_elements = <.> x_max = <length> x_min = <length> y_max = <length> y_min = <length> z_max = <length> z_min = <length> element_file = "filename" nodes_file = "filename" force_file = "filename";
    | ADD DRM LOADING RESTOREFORCES TEXTNUMBER exp
                      time_step '=' exp
                      scale_factor '=' exp
                      number_of_steps '=' exp
                      number_of_drm_nodes '=' exp
                      number_of_drm_elements '=' exp
                      x_max '=' exp
                      x_min '=' exp
                      y_max '=' exp
                      y_min '=' exp
                      z_max '=' exp
                      z_min '=' exp
                      element_file '=' exp
                      nodes_file '=' exp
                      force_file '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($6); signature.push_back(this_signature("pattern_number",           &isAdimensional));
        args.push_back($9); signature.push_back(this_signature("time_step",           &isTime));
        args.push_back($12); signature.push_back(this_signature("scale_factor",           &isAdimensional));
        args.push_back($15); signature.push_back(this_signature("number_of_steps",           &isAdimensional));
        args.push_back($18); signature.push_back(this_signature("number_of_drm_nodes",           &isAdimensional));
        args.push_back($21); signature.push_back(this_signature("number_of_drm_elements",           &isAdimensional));
        args.push_back($24); signature.push_back(this_signature("x_max",           &isLength));
        args.push_back($27); signature.push_back(this_signature("x_min",           &isLength));
        args.push_back($30); signature.push_back(this_signature("y_max",           &isLength));
        args.push_back($33); signature.push_back(this_signature("y_min",           &isLength));
        args.push_back($36); signature.push_back(this_signature("z_max",           &isLength));
        args.push_back($39); signature.push_back(this_signature("z_min",           &isLength));


        args.push_back($42); signature.push_back(this_signature("element_file",           &isAdimensional));
        args.push_back($45); signature.push_back(this_signature("nodes_file",           &isAdimensional));
        args.push_back($48); signature.push_back(this_signature("force_file",           &isAdimensional));

        $$ = new FeiDslCaller15<int, double, double, int, int, int, double, double, double, double, double, double, string, string, string>(&add_load_pattern_domain_reduction_method_restore_forces, args, signature, "add_load_pattern_domain_reduction_method_restore_forces");

        for(int i = 1; i <= 15; i++) nodes.pop();
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
        Expression* command_list;
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


CMD_define
    //!=========================================================================================================
    //!
    //!FEIDOC define output to binary;
    : ENABLE OUTPUT 
    {
        args.clear(); signature.clear();
        $$ = new FeiDslCaller0<>(&enable_output,args,signature,"enable_output");
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC define output to binary;
    | DISABLE OUTPUT 
    {
        args.clear(); signature.clear();
        $$ = new FeiDslCaller0<>(&disable_output,args,signature,"disable_output");
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
    //!FEIDOC define algorithm [With_no_convergence_check] / [Newton] / [Modified_Newton];
    | DEFINE ALGORITHM ALGNAME
    {
        // This case is a bit different. ALGNAME contains a string to
        // indicate which algorithm to choose. Instead of having different
        // tokens for each algorithm, just have one and decide in the
        // semantic action which DSL to call.
        // Usually different DSLs have different arguments, but in this
        // case all three take no arguments, which makes this approach easier.

        int (*f)();         // function poiner to the algorithm DSL
        string fname;       // name of the DSL called to report

        args.clear(); signature.clear();
        if((*$3).compare("With_no_convergence_check") == 0 || (*$3).compare("with_no_convergence_check") == 0)
        {
            f = &define_algorithm_with_no_convergence_check_for_analysis;
            fname = "define_algorithm_with_no_convergence_check_for_analysis";
        }
        if((*$3).compare("Modified_Newton") == 0 || (*$3).compare("modified_newton") == 0)
        {
            f = &define_algorithm_modifiednewton_for_analysis;
            fname = "define_algorithm_modifiednewton_for_analysis";
        }
        if((*$3).compare("Newton") == 0 || (*$3).compare("newton") == 0)
        {
            f = &define_algorithm_newton_for_analysis;
            fname = "define_algorithm_newton_for_analysis";
        }

        $$ = new FeiDslCaller0<>(f,args, signature, fname);


        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC define convergence test [Norm_Displacement_Increment] / [Energy_Increment] / [Norm_Unbalance] tolerance = <length> maximum_iterations = <.> verbose_level = <0>|<1>|<2>;
    | DEFINE CONVERGENCE_TEST TESTNAME tolerance '=' exp maximum_iterations '=' exp verbose_level '=' exp
    {
        args.clear(); signature.clear();

        int (*f)(double, int, int);         // function poiner to the test DSL
        string fname;       // name of the DSL called to report

        args.clear(); signature.clear();
        if($3->compare("Norm_Displacement_Increment") == 0)
        {
            f = &define_convergence_test_normdisplacementincrement_for_analysis;
            fname = "define_convergence_test_normdisplacementincrement_for_analysis";
            args.push_back($6); signature.push_back(this_signature("tolerance", &isLength));
        }
        if($3->compare("Energy_Increment") == 0)
        {
            f = &define_convergence_test_energyincrement_for_analysis;
            fname = "define_convergence_test_energyincrement_for_analysis";
            args.push_back($6); signature.push_back(this_signature("tolerance", &isEnergy));
        }
        if($3->compare("Norm_Unbalance") == 0)
        {
            f = &define_convergence_test_normunbalance_for_analysis;
            fname = "define_convergence_test_normunbalance_for_analysis";
            args.push_back($6); signature.push_back(this_signature("tolerance", &isForce));
        }
        args.push_back($9); signature.push_back(this_signature("maximum_iterations", &isAdimensional));
        args.push_back($12); signature.push_back(this_signature("verbose_level", &isAdimensional));

        $$ = new FeiDslCaller3<double, int, int> (f, args, signature, fname);

        for(int ii = 1;ii <=3; ii++) nodes.pop(); //pop 3 exps
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC define solver [ProfileSPD] / [UMFPack];
    | DEFINE SOLVER SOLVERNAME
    {
        int (*f)() = &define_solver_umfpack_for_analysis;         // function poiner to the algorithm DSL
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
        if($3->compare("parallel") == 0)
        {
             f = &define_solver_parallel_for_analysis; 
             fname = "define_solver_parallel_for_analysis"; 
        }

        $$ = new FeiDslCaller0<>(f, args, signature, fname);

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
    | SIMULATE constitutive testing constant mean pressure triaxial strain control USE MATERIAL TEXTNUMBER exp
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
    | SIMULATE constitutive testing drained triaxial strain control USE MATERIAL TEXTNUMBER exp
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
    | SIMULATE constitutive testing undrained triaxial stress control USE MATERIAL TEXTNUMBER exp
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
    | SIMULATE constitutive testing undrained simple shear strain control USE MATERIAL TEXTNUMBER exp
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
    | SIMULATE constitutive testing undrained triaxial strain control USE MATERIAL TEXTNUMBER exp
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
    //!FEIDOC remove constraint equaldof node # <.>;
    | REMOVE CONSTRAINT equaldof NODE TEXTNUMBER exp
    {
        args.clear(); signature.clear();
        args.push_back($6); signature.push_back(this_signature("number", &isAdimensional));
             $$ = new FeiDslCaller1<int>(&remove_equaldof_from_node, args, signature, "remove_equaldof_from_node");

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
    //!FEIDOC add material # <.> type [vonmises_perfectly_plastic] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit> number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_perfectly_plastic
      mass_density '=' exp
      elastic_modulus '=' exp
      poisson_ratio '=' exp
      von_mises_radius '=' exp
      initial_confining_stress '=' exp
      ALGORITHM '=' exp
      number_of_subincrements '=' exp
      maximum_number_of_iterations '=' exp
      tolerance_1 '=' exp
      tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("algoritm",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back($26); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

        $$ = new FeiDslCaller11<int, int, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_perfectly_plastic_accelerated] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> initial_confining_stress = <F/L^2>  maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_perfectly_plastic_accelerated
                              mass_density '=' exp
                              elastic_modulus '=' exp
                              poisson_ratio '=' exp
                              von_mises_radius '=' exp
                              initial_confining_stress '=' exp
                              maximum_number_of_iterations '=' exp
                              tolerance_1 '=' exp
                              tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back($23); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_2", &isAdimensional));

        $$ = new FeiDslCaller9<int, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_perfectly_plastic_LT] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> initial_confining_stress = <F/L^2>  maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_perfectly_plastic_LT
                              mass_density '=' exp
                              elastic_modulus '=' exp
                              poisson_ratio '=' exp
                              von_mises_radius '=' exp
                              initial_confining_stress '=' exp
                              maximum_number_of_iterations '=' exp
                              tolerance_1 '=' exp
                              tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back($23); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_2", &isAdimensional));

        $$ = new FeiDslCaller9<int, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterialLT_vonmises_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterialLT_vonmises_perfectly_plastic");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_isotropic_hardening] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit> number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_isotropic_hardening
      mass_density '=' exp
      elastic_modulus '=' exp
      poisson_ratio '=' exp
      von_mises_radius '=' exp
      isotropic_hardening_rate '=' exp
      initial_confining_stress '=' exp
      ALGORITHM '=' exp
      number_of_subincrements '=' exp
      maximum_number_of_iterations '=' exp
      tolerance_1 '=' exp
      tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("isotropic_hardening_rate",       &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back($29); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_2",      &isAdimensional));

        $$ = new FeiDslCaller12<int, int, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_isotropic_hardening");
        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_isotropic_hardening_accelerated] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2>  maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_isotropic_hardening_accelerated
                              mass_density '=' exp
                              elastic_modulus '=' exp
                              poisson_ratio '=' exp
                              von_mises_radius '=' exp
                              isotropic_hardening_rate '=' exp
                              initial_confining_stress '=' exp
                              maximum_number_of_iterations '=' exp
                              tolerance_1 '=' exp
                              tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("isotropic_hardening_rate", &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back($26); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller10<int, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening");
        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_kinematic_hardening] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit> number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_kinematic_hardening
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        von_mises_radius '=' exp
        armstrong_frederick_ha '=' exp
        armstrong_frederick_cr '=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back($32); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back($41); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        $$ = new FeiDslCaller13<int, int, double, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_kinematic_hardening");
        for(int ii = 1;ii <=13; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_linear_kinematic_hardening] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> kinematic_hardening_rate = <.> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit> number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_linear_kinematic_hardening
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        von_mises_radius '=' exp
        kinematic_hardening_rate '=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("algoritm",          &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",       &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back($29); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("maximum_number_of_iterations",      &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_1",      &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        $$ = new FeiDslCaller12<int, int, double, double, double, double, double, double, int, int, double, double>(&add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening");
        for(int ii = 1;ii <=12; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_kinematic_hardening_accelerated] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> initial_confining_stress = <F/L^2> initial_confining_strain = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_kinematic_hardening_accelerated
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        von_mises_radius '=' exp
        armstrong_frederick_ha '=' exp
        armstrong_frederick_cr '=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",   &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",   &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("initial_confining_stress",   &isPressure));
        args.push_back($29); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller11<int, double, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening");
        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [vonmises_linear_kinematic_hardening_accelerated] mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> kinematic_hardening_rate = <.> initial_confining_stress = <F/L^2> initial_confining_strain = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE vonmises_linear_kinematic_hardening_accelerated
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        von_mises_radius '=' exp
        kinematic_hardening_rate '=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();
        args.push_back($3); signature.push_back(this_signature("number",            &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",      &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",  &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",    &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("von_mises_radius",  &isPressure));
        args.push_back($20); signature.push_back(this_signature("kinematic_hardening_rate",   &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress",   &isPressure));
        args.push_back($26); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller10<int, double, double, double, double, double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_vonmises_linear_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_vonmises_linear_kinematic_hardening");
        for(int ii = 1;ii <=10; ii++) nodes.pop();
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
    //!FEIDOC add material # <.> type [camclay] mass_density = <M/L^3> reference_void_ratio = <.> critical_stress_ratio_M = <.> lambda = <.> kappa = <.> poisson_ratio = <.> minimum_bulk_modulus = <F/L^2> initial_mean_pressure = <F/L^2> algorithm = <explicit|implicit>  number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE camclay
        mass_density '=' exp
        reference_void_ratio '=' exp
        critical_stress_ratio_M '=' exp
        lambda '=' exp
        kappa '=' exp
        poisson_ratio '=' exp
        minimum_bulk_modulus '=' exp
        pressure_reference_p0 '=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3);  signature.push_back(this_signature("number",                       &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("algoritm",                     &isAdimensional));
        args.push_back($8);  signature.push_back(this_signature("mass_density",                 &isDensity));
        args.push_back($11); signature.push_back(this_signature("reference_void_ratio",         &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("critical_stress_ratio_M",      &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("lambda",                       &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("kappa",                        &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("minimum_bulk_modulus",         &isPressure));
        args.push_back($29); signature.push_back(this_signature("pressure_reference_p0",        &isPressure));
        args.push_back($32); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($38); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($41); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($44); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($47); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        $$ = new FeiDslCaller15<int, int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_camclay, args, signature, "add_constitutive_model_NDMaterial_camclay");

        for(int ii = 1;ii <=15; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [camclay_accelerated] mass_density = <M/L^3> reference_void_ratio = <.> critical_stress_ratio_M = <.> lambda = <.> kappa = <.> poisson_ratio = <.> minimum_bulk_modulus = <F/L^2> pressure_reference_p0 = <F/L^2> initial_confining_stress = <F/L^2>  maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE camclay_accelerated
        mass_density '=' exp
        reference_void_ratio '=' exp
        critical_stress_ratio_M '=' exp
        lambda '=' exp
        kappa '=' exp
        poisson_ratio '=' exp
        minimum_bulk_modulus '=' exp
        pressure_reference_p0 '=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3);  signature.push_back(this_signature("number",                       &isAdimensional));
        args.push_back($8);  signature.push_back(this_signature("mass_density",                 &isDensity));
        args.push_back($11); signature.push_back(this_signature("reference_void_ratio",         &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("critical_stress_ratio_M",      &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("lambda",                       &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("kappa",                        &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("minimum_bulk_modulus",         &isPressure));
        args.push_back($29); signature.push_back(this_signature("pressure_reference_p0",        &isPressure));
        args.push_back($32); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($35); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($41); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller13<int,
                               double, double, double,
                               double, double, double, double,
                               double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_camclay, args, signature, "add_constitutive_model_NDMaterial_accelerated_camclay");
        for(int ii = 1;ii <=13; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [sanisand2004] mass_density = <M/L^3> e0 = <.> sanisand2004_G0 = <.> poisson_ratio = <.> sanisand2004_Pat = <stress>  sanisand2004_p_cut = <.>  sanisand2004_Mc = <.>  sanisand2004_c = <.> sanisand2004_lambda_c = <.> sanisand2004_xi = <.>  sanisand2004_ec_ref = <.>  sanisand2004_m = <.>  sanisand2004_h0 = <.> sanisand2004_ch = <.>  sanisand2004_nb = <.> sanisand2004_A0 = <.> sanisand2004_nd = <.> sanisand2004_z_max = <.>  sanisand2004_cz = <.> initial_confining_stress = <stress>  algorithm = <explicit|implicit>  number_of_subincrements = <.>  maximum_number_of_iterations = <.>  tolerance_1 = <.>  tolerance_2 = <.>;
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
    //!FEIDOC add material # <.> type [druckerprager_isotropic_hardening] mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit>  number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_isotropic_hardening
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        isotropic_hardening_rate '=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                         &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("algoritm",                      &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",                   &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",               &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                 &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",           &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("isotropic_hardening_rate",      &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress",      &isPressure));
        args.push_back($29); signature.push_back(this_signature("number_of_subincrements",       &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("maximum_number_of_iterations",  &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_1",  &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_2",      &isAdimensional));


        $$ = new FeiDslCaller12<int, int,
                                double, double, double,
                                double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening");

        for(int ii = 1;ii <= 12; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [druckerprager_isotropic_hardening_accelerated] mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = <F/L^2> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_isotropic_hardening_accelerated
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        isotropic_hardening_rate '=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("isotropic_hardening_rate",     &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($26); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller10<int, double,
                                double, double, double,
                                double, double, int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_isotropic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_isotropic_hardening");

        for(int ii = 1;ii <= 10; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [druckerprager_kinematic_hardening]  mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit>  number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_kinematic_hardening
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        armstrong_frederick_ha '=' exp
        armstrong_frederick_cr'=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("algoritm",                     &isAdimensional));

        args.push_back($8); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));  //units?
        args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));  //units?
        args.push_back($26); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($32); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($38); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($41); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        $$ = new FeiDslCaller13<int, int,
                                double, double, double,
                                double, double, double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening");

        for(int ii = 1;ii <= 13; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [druckerprager_kinematic_hardening_accelerated]  mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> initial_confining_stress = <F/L^2> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_kinematic_hardening_accelerated
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        armstrong_frederick_ha '=' exp
        armstrong_frederick_cr'=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",          &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("armstrong_frederick_ha",       &isAdimensional));  //units?
        args.push_back($23); signature.push_back(this_signature("armstrong_frederick_cr",       &isAdimensional));  //units?
        args.push_back($26); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($29); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller11<int, double,
                                double, double, double,
                                double, double, double,
                                int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening");

        for(int ii = 1;ii <= 11; ii++) nodes.pop();
        nodes.push($$);
    }

    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [druckerprager_perfectly_plastic]  mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> initial_confining_stress = <F/L^2> algorithm = <explicit|implicit>  number_of_subincrements = <.> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_perfectly_plastic
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        initial_confining_stress '=' exp
        ALGORITHM '=' exp
        number_of_subincrements '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("algoritm",                     &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($26); signature.push_back(this_signature("number_of_subincrements",      &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($32); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($35); signature.push_back(this_signature("tolerance_2",     &isAdimensional));


        $$ = new FeiDslCaller11<int, int,
                                double, double, double,
                                double, double,
                                int, int, double, double>(&add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic");

        for(int ii = 1;ii <= 11; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [druckerprager_perfectly_plastic_accelerated]  mass_density = <M/L^3> elastic_modulus = <F/L^2>  poisson_ratio = <.> druckerprager_angle = <F/L^2> initial_confining_stress = <F/L^2> maximum_number_of_iterations = <.> tolerance_1 = <.> tolerance_2 = <.>;
    | MATERIAL TEXTNUMBER exp TYPE druckerprager_perfectly_plastic_accelerated
        mass_density '=' exp
        elastic_modulus '=' exp
        poisson_ratio '=' exp
        druckerprager_angle '=' exp
        initial_confining_stress '=' exp
        maximum_number_of_iterations '=' exp
        tolerance_1 '=' exp
        tolerance_2 '=' exp
    {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                        &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("mass_density",                  &isDensity));
        args.push_back($11); signature.push_back(this_signature("elastic_modulus",              &isPressure));
        args.push_back($14); signature.push_back(this_signature("poisson_ratio",                &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("druckerprager_angle",         &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("initial_confining_stress",     &isPressure));
        args.push_back($23); signature.push_back(this_signature("maximum_number_of_iterations", &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("tolerance_1", &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("tolerance_2", &isAdimensional));


        $$ = new FeiDslCaller9<int,
                                double, double,
                                double, double, double,
                                int, double, double>(&add_constitutive_model_NDMaterial_accelerated_druckerprager_perfectly_plastic, args, signature, "add_constitutive_model_NDMaterial_accelerated_druckerprager_perfectly_plastic");
        for(int ii = 1;ii <= 9; ii++) nodes.pop();
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
    //!
    //!FEIDOC add material # <.> type [pisano] elastic_modulus = <F/L^2> poisson_ratio = <.> M_in = <F/L^2> kd_in = <.> xi_in = <.> h_in = <.> m_in = <.> mass_density = <M/L^2> initial_confining_stress = <F/L^2> beta_min = <.>;
    | MATERIAL TEXTNUMBER exp TYPE pisano
                        elastic_modulus '=' exp
                        poisson_ratio '=' exp
                        M_in '=' exp
                        kd_in '=' exp
                        xi_in '=' exp
                        h_in '=' exp
                        m_in '=' exp
                        mass_density '=' exp
                        initial_confining_stress '=' exp
                        beta_min '=' exp
      {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back($11); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back($32); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back($35); signature.push_back(this_signature("beta_min",                 &isAdimensional));

        $$ = new FeiDslCaller11<int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                double>(&add_constitutive_model_NDMaterial_pisano, args, signature, "add_constitutive_model_NDMaterial_pisano");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [pisanoLT] elastic_modulus = <F/L^2> poisson_ratio = <.> M_in = <F/L^2> kd_in = <.> xi_in = <.> h_in = <.> m_in = <.> mass_density = <M/L^2> initial_confining_stress = <F/L^2> beta_min = <.>;
    | MATERIAL TEXTNUMBER exp TYPE pisanoLT
                        elastic_modulus '=' exp
                        poisson_ratio '=' exp
                        M_in '=' exp
                        kd_in '=' exp
                        xi_in '=' exp
                        h_in '=' exp
                        m_in '=' exp
                        mass_density '=' exp
                        initial_confining_stress '=' exp
                        beta_min '=' exp
      {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back($11); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back($32); signature.push_back(this_signature("initial_confining_stress", &isPressure));
        args.push_back($35); signature.push_back(this_signature("beta_min",                 &isAdimensional));

        $$ = new FeiDslCaller11<int,
                                double, double, double,
                                double, double, double,
                                double, double, double,
                                double>(&add_constitutive_model_NDMaterialLT_pisano, args, signature, "add_constitutive_model_NDMaterialLT_pisano");

        for(int ii = 1;ii <=11; ii++) nodes.pop();
        nodes.push($$);
    }
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
    //!=========================================================================================================
    //!
    //!FEIDOC add material # <.> type [New_PisanoLT] elastic_modulus = <F/L^2> poisson_ratio = <.> M_in = <F/L^2> kd_in = <.> xi_in = <.> h_in = <.> m_in = <.> mass_density = <M/L^2> initial_confining_stress = <F/L^2> beta_min = <.>;
    | MATERIAL TEXTNUMBER exp TYPE New_PisanoLT
                        elastic_modulus '=' exp
                        poisson_ratio '=' exp
                        M_in '=' exp
                        kd_in '=' exp
                        xi_in '=' exp
                        h_in '=' exp
                        m_in '=' exp
                        mass_density '=' exp
                        initial_confining_stress '=' exp
      {
        args.clear(); signature.clear();

        args.push_back($3); signature.push_back(this_signature("number",                    &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("elastic_modulus",           &isPressure));
        args.push_back($11); signature.push_back(this_signature("poisson_ratio",            &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("M_in",                     &isAdimensional));
        args.push_back($17); signature.push_back(this_signature("kd_in",                    &isAdimensional));
        args.push_back($20); signature.push_back(this_signature("xi_in",                    &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("h_in",                     &isAdimensional));
        args.push_back($26); signature.push_back(this_signature("m_in",                     &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("mass_density",             &isDensity));
        args.push_back($32); signature.push_back(this_signature("initial_confining_stress", &isPressure));

        $$ = new FeiDslCaller10<int,
                                double, double, double,
                                double, double, double,
                                double, double, double>(&add_constitutive_model_NDMaterialLT_New_Pisano, args, signature, "add_constitutive_model_NDMaterialLT_New_Pisano");

        for(int ii = 1;ii <=10; ii++) nodes.pop();
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
    //!FEIDOC add element # <.> type [8NodeBrickLT] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
    | TEXTNUMBER exp TYPE EightNodeBrickLT WITH NODES
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

        $$ = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node_ltensor, args, signature, "add_element_brick_8node_ltensor");

        for(int ii = 1;ii <=10; ii++) nodes.pop(); //pop 10 exps
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add element # <.> type [8NodeBrick_elastic] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
    | TEXTNUMBER exp TYPE EightNodeBrickElastic WITH NODES
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

        $$ = new FeiDslCaller10<int,int,int,int,int,int,int,int,int,int>(&add_element_brick_8node_elastic, args, signature, "add_element_brick_8node_elastic");

        for(int ii = 1;ii <=10; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add element # <.> type 8NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
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
    //!FEIDOC add element # <.> type [27NodeBrickLT] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
    | TEXTNUMBER exp TYPE TwentySevenNodeBrickLT WITH NODES
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
                                int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_ltensor, args, signature, "add_element_brick_27node_ltensor");

        for(int ii = 1;ii <=29; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add element # <.> type 27NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
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
    //!FEIDOC add element # <.> type [27NodeBrick_elastic] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
    | TEXTNUMBER exp TYPE TwentySevenNodeBrickElastic WITH NODES
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
                                int,int,int,int,int,int,int,int,int>(&add_element_brick_27node_elastic, args, signature, "add_element_brick_27node_elastic");

        for(int ii = 1;ii <=29; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add element # <.> type variable_node_brick_8_to_27 using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
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
    //!FEIDOC add element # <.> type 20NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
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
    //!FEIDOC add element # <.> type [20NodeBrick_elastic] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;
    | TEXTNUMBER exp TYPE TwentyNodeBrickElastic WITH NODES
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
                                int,int>(&add_element_brick_20node_elastic, args, signature, "add_element_brick_20node_elastic");

        for(int ii = 1;ii <=22; ii++) nodes.pop();
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
    //!FEIDOC add element # <.> type [20NodeBrick_upU] with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;
    | TEXTNUMBER exp TYPE TwentyNodeBrick_uPU WITH NODES
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
        //
        //int add_element_beam_elastic_3d(int ElementNumber,
        //                 double A,
        //                 double E,
        //                 double G,
        //                 double Jx,
        //                 double Iy,
        //                 double Iz,
        //                 int iNode,
        //                 int jNode,
        //                 double rho,
        //                 double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        //                 double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        //                 double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)
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
        //
        //int add_element_beam_elastic_3d(int ElementNumber,
        //                 double A,
        //                 double E,
        //                 double G,
        //                 double Jx,
        //                 double Iy,
        //                 double Iz,
        //                 int iNode,
        //                 int jNode,
        //                 double rho,
        //                 double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        //                 double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        //                 double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)
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
    //!FEIDOC add element # <.> type penalty_for_applying_generalized_displacement with node (<.>) stiffness = <corresponding unit to dof> dof [DOFTYPE];
    | TEXTNUMBER exp TYPE penalty_for_applying_generalized_displacement WITH NODE  '(' exp  ')'
        stiffness '=' exp
        dof DOF
     {

        args.clear(); signature.clear();

        Expression* dof_number = dof2number(*$14);
        UnitCheckerFunctionPointerType function_ptr = dof2stiffnesschecker(*$14);

        args.push_back($2); signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back($8); signature.push_back(this_signature("node",        &isAdimensional));
        args.push_back($12); signature.push_back(this_signature("stiffness",  function_ptr));
        args.push_back(dof_number); signature.push_back(this_signature("dof", &isAdimensional));


        $$ = new FeiDslCaller4<int, int, double, int>(&add_element_penalty_for_applying_generalized_displacement, args, signature, "add_element_penalty_for_applying_generalized_displacement");

        for(int ii = 1;ii <=3; ii++) nodes.pop();
        nodes.push($$);
    }
    //!=========================================================================================================
    //!
    //!FEIDOC add element # <.> type penalty with nodes (<.> , <.>) stiffness = <corresponding unit to dof> dof to constrain <.>;
    | TEXTNUMBER exp TYPE penalty WITH NODES  '(' exp ',' exp ')'
        stiffness '=' exp
        dof_to_constrain DOF
    {
        args.clear(); signature.clear();


        Expression* dof_number = dof2number(*$16);
        UnitCheckerFunctionPointerType function_ptr = dof2stiffnesschecker(*$16);


        args.push_back($2);  signature.push_back(this_signature("number",      &isAdimensional));
        args.push_back($8);  signature.push_back(this_signature("node1",       &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("node2",       &isAdimensional));
        args.push_back($14); signature.push_back(this_signature("stiffness",    function_ptr));
        args.push_back(dof_number); signature.push_back(this_signature("dof",  isAdimensional));

         $$ = new FeiDslCaller5<int, int, int, double, int>(&add_element_penalty, args, signature, "add_element_penalty");


        for(int i = 1; i <= 4; i++) nodes.pop();
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
    //!FEIDOC add element # <.> type [contact] with nodes (<.>, <.>) normal_stiffness = <F/L^2> tangential_stiffness = <F/L^2> friction_ratio = <.> maximum_gap = <L> contact_plane_vector = (<.>, <.>, <.> );
    | TEXTNUMBER exp TYPE contact WITH NODES
        '(' exp ',' exp ')'
        normal_stiffness '=' exp
        tangential_stiffness '=' exp
        friction_ratio '=' exp
        maximum_gap '=' exp
        contact_plane_vector '=' '(' exp ','  exp ','  exp ')'
    {
        args.clear(); signature.clear();

        args.push_back($2); signature.push_back(this_signature("number",                  &isAdimensional));

        args.push_back($8); signature.push_back(this_signature("node1",                   &isAdimensional));
        args.push_back($10); signature.push_back(this_signature("node2",                  &isAdimensional));

        args.push_back($14); signature.push_back(this_signature("normal_stiffness",       &isThisUnit<1, 0, -2>));
        args.push_back($17); signature.push_back(this_signature("tangential_stiffness",   &isThisUnit<1, 0, -2>));
        args.push_back($20); signature.push_back(this_signature("friction_ratio",         &isAdimensional));
        args.push_back($23); signature.push_back(this_signature("maximum_gap",            &isLength));

        args.push_back($27); signature.push_back(this_signature("x_local_1",              &isAdimensional));
        args.push_back($29); signature.push_back(this_signature("x_local_2",              &isAdimensional));
        args.push_back($31); signature.push_back(this_signature("x_local_3",              &isAdimensional));


        $$ = new FeiDslCaller10<int, int, int,
                               double, double, double,
                               double, double, double, double>(&add_element_contact_nonlinear_3dof_to_3dof, args, signature, "add_element_contact_nonlinear_3dof_to_3dof");

        for(int ii = 1;ii <=9; ii++) nodes.pop();
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
// Segfault ensues if this is not done. Al


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

        exit(0);
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
        cerr << " \n <!!!>  Error at ---> " << loc << ": " << s << endl << endl;
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
    if (dof.compare("ux") == 0) return new Number(1, unitless);
    if (dof.compare("uy") == 0) return new Number(2, unitless);
    if (dof.compare("uz") == 0) return new Number(3, unitless);
    if (dof.compare("rx") == 0) return new Number(4, unitless);
    if (dof.compare("ry") == 0) return new Number(5, unitless);
    if (dof.compare("rz") == 0) return new Number(6, unitless);
    if (dof.compare("Ux") == 0) return new Number(5, unitless);
    if (dof.compare("Uy") == 0) return new Number(6, unitless);
    if (dof.compare("Uz") == 0) return new Number(7, unitless);
    if (dof.compare("p")  == 0) return new Number(4, unitless);

    return new Number(-100, unitless);
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
    if (dof.compare("Fx") == 0) return new Number(1, unitless);
    if (dof.compare("Fy") == 0) return new Number(2, unitless);
    if (dof.compare("Fz") == 0) return new Number(3, unitless);
    if (dof.compare("Mx") == 0) return new Number(4, unitless);
    if (dof.compare("My") == 0) return new Number(5, unitless);
    if (dof.compare("Mz") == 0) return new Number(6, unitless);
    if (dof.compare("F_fluid_x") == 0) return new Number(5, unitless);
    if (dof.compare("F_fluid_y") == 0) return new Number(6, unitless);
    if (dof.compare("F_fluid_z") == 0) return new Number(7, unitless);

    return new Number(-100, unitless);
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

        *outputcppfile << "#include \"CPPIncludes.h\"" << endl ;
        *outputcppfile << endl;
        *outputcppfile << endl;
        *outputcppfile << "int main()" << endl;
        *outputcppfile << "{" << endl;
    }
}





























// That's all folks.



