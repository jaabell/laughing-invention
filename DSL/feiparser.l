  /* FEI Lexer! */
  /* NOTE: To comment this file use C "standard comments"  BUT leave a space before the slash-star */
  /* /////////////////////////////////////////////////////////////////////////////// */
  /* //                                                                              */
  /* // COPYRIGHT (C):      Version of a Creative Commons License,                   */
  /* //                     for details contact Boris Jeremic, jeremic@ucdavis.edu   */
  /* // PROJECT:            Real ESSI Simulator                                      */
  /* // PROGRAMMER:         Jose Abell                                               */
  /* // DATE:               Jan 2012                                                 */
  /* // UPDATE HISTORY:     Full update history in git repository.                   */
  /* // QUALITY ASSURANCE:  Developers have worked really hard to develop            */
  /* //                     an extensive verification of developed implementation    */
  /* //                     and with that can claim quality and fitness for intended */
  /* //                     purpose (modeling and simulation of Real ESSI Problems)  */
  /* //                     within confines of verification effort                   */
  /* //                                                                              */
  /* // LEGACY/DEFUNCT COPYLEFT (C):                                                 */
  /* //                     Woody's viral GPL-like license (adapted by BJ):          */
  /* //                     ``This    source  code is Copyrighted in                 */
  /* //                     worldwide for  an  indefinite  period,  and anybody      */
  /* //                     caught  using it without our permission, will be         */
  /* //                     mighty good friends of ourn, cause we don't give         */
  /* //                     a  darn.  Hack it. Compile it. Debug it. Run it.         */
  /* //                     Yodel  it.  Enjoy it. We wrote it, that's all we         */
  /* //                     wanted to do.''                                          */
  /* //                                                                              */
  /* /////////////////////////////////////////////////////////////////////////////   */



%option noyywrap yylineno
%x IFILE
%x COMMENT

%{
	#include <iostream>
	#include <fstream>
	#include <string>



	//Old C libraries are used in lexer....
	#include <fenv.h>  // For feenableexcept()
    #include <cmath>
    #include <cerrno>
    #pragma STDC FENV_ACCESS ON
    #include <stacktrace.h>
    #include <cstdlib>
	#include <cstring>
	#include <csignal>
	#include <stdio.h>                         // Provides printf, etc.
	#include <string.h>
	#include <getopt.h>                        // For the command-line argument parser
	/// =================================================================================
	// Please do not erase! Might make it into code someday.
	// #include <readline/readline.h>
	// #include <readline/history.h>
	// =================================================================================

	//Our FEI libraries
	#include "feiast.h"
	#include "dsl_actions.h"
	#include "siunit.h"
	#include "quantity.h"
    //#define __USE_GNU

    


	//From the parser generator
	#include "feiparser.tab.hh"

	//Boost for string to double conversion
	//#include <../boost/boost/lexical_cast.hpp>
	#include <boost/lexical_cast.hpp>

    #ifdef _PARALLEL_PROCESSING
    #include <mpi.h>
    #endif

	int yycolumn = 1;
	#define YY_DECL int yylex(yy::feiparser::semantic_type *yylval, \
							  yy::feiparser::location_type *yylloc)
	//#define YY_USER_ACTION yylloc -> columns (yyleng);
	#define YY_USER_ACTION yylloc->end.line = yylineno; \
	yylloc->end.column = yycolumn+yyleng; \
	yycolumn += yyleng;

	// =================================================================================
	// Please do not erase! Might make it into code someday.
	// This is to replace STDIN with GNU Readline (Caution!!! GPL Licence).
	// #undef YY_INPUT
	// #define YY_INPUT(buf, result, max_size) (result = my_yyinput(buf, max_size))
	// =================================================================================

	//static int my_yyinput(char *buf, int max_size);
	//static char *st_readline_buffer;
	//static int  st_readline_used_len;

	//Simplify the usage of tokens...
	typedef yy::feiparser::token token;

	//Standard namespace
	using namespace std;


	//
	//FILE * ;

	struct bufstack
	{
		struct bufstack *prev;
		YY_BUFFER_STATE bs;
		int lineno;
		char* filename;
		FILE *f;
	} *curbs = 0;

	char* curfilename;

	int newfile(char* fn);
	int newstdin(void);
	int popfile(void);
	int error_behavior(void);
	void welcome_msg();
	void set_known_units();
	void set_variables(char* cstring);


	char initial[] = "STDIN";

	//Command line options flags
	int FLAG_interactive_mode    = 1;
	int FLAG_generate_cpp_output = 0;
	int FLAG_dry_run             = 0;
	int FLAG_execute_apis        = 1;
	int FLAG_inclusion_mode      = 0;
	int FLAG_run_file            = 0;
    int FLAG_binary_output       = 0;
	int FLAG_debug_attach        = 0;

	/* EXTERNS - All defined in feiparser.yy */
	extern const string thePrompt;
	extern string out_cpp_filename;
	extern string prompt;
	extern map<string,Quantity> global_variables;
	extern set<string> locked_global_variables;
	extern int recovery_mode;
	extern void set_model_name(char* cstring);
	extern void set_outcppfile_name(string newfilename);
	extern void wipe_model();
	extern void set_dry_run_mode(int);
	extern void set_profiling_results_filename(char* cstring);
	extern void call_hdf5_flush_next_chance(void);

#ifndef VERSION
#define VERSION "--NOT FROM GIT REPO--"
#endif
%}

/* float exponent */
EXP	([Ee][-+]?[0-9]+)

%%

%{
	// start where previous token ended
	//yylloc -> step();
%}

 /* ================================================================================================= */
 /* ========              BEGIN LEXER                                                       ========= */
 /* ================================================================================================= */
 
 /* The order in this file matters. Patterns which are higher up are matched first */
 
 /* single character ops are directly passed to the parser (more grammatical clarity) */
"+"                         |
"-"                         |
"*"                         |
"%"                         |
"/"                         |
"^"                         |
"="                         |
"!"                         |
","                         |
"("                         |
"{"                         |
"}"                         |
"["                         |
"]"                         |
")"                         { return yytext[0]; }

";"                         {
								yylloc->begin.line = yylineno;
								yylloc->begin.column = yycolumn;

								return yytext[0];
							}
 /* assign operate */

"+="                        { return token::PLUSEQUAL; }
"-="                        { return token::MINUSEQUAL; }
"*="                        { return token::TIMESEQUAL; }
"/="                        { return token::DIVIDEEQUAL; }
"%="                        { return token::MODULUSEQUAL; }
"^="                        { return token::POWEQUAL; }


 /* comparison ops */
">"                         { yylval->fn = 1; return token::CMP; }
"<"                         { yylval->fn = 2; return token::CMP; }
"<>"                        { yylval->fn = 3; return token::CMP; }
"=="                        { yylval->fn = 4; return token::CMP; }
">="                        { yylval->fn = 5; return token::CMP; }
"<="                        { yylval->fn = 6; return token::CMP; }
"&"                         { yylval->fn = 7; return token::CMP; }
"|"                         { yylval->fn = 8; return token::CMP; }


 /* test for units */
"isAdimensional"            |
"isMass"                    |
"isLength"                  |
"isTime"                    |
"isArea"                    |
"isVolume"                  |
"isForce"                   |
"isEnergy"                  |
"isTorque"                  |
"isPressure"                |
"isBodyForce"               |
"isDensity"                 {yylval->ident = new string(yytext); return token::ISUNITTYPE;}

 /* Auto-variables (keywords)  */
"NUMBER_OF_NODES"           {return token::NUMBER_OF_NODES;}
"NUMBER_OF_ELEMENTS"        {return token::NUMBER_OF_ELEMENTS;}
"CURRENT_TIME"              {return token::CURRENT_TIME;}
"NUMBER_OF_SP_CONSTRAINTS"  {return token::NUMBER_OF_SP_CONSTRAINTS;}
"NUMBER_OF_MP_CONSTRAINTS"  {return token::NUMBER_OF_MP_CONSTRAINTS;}
"NUMBER_OF_LOADS"           {return token::NUMBER_OF_LOADS;}
"IS_PARALLEL"               {return token::IS_PARALLEL;}

 /* Flow control keywords */
"if"                        { return token::IF; }
"then"                      { return token::THEN; }
"else"                      { return token::ELSE; }
"while"                     { return token::WHILE; }
"do"                        { return token::DO; }
"let"                       { return token::LET;}
"in"                        { return token::IN; }
"exit safe mode"            { return token::EXITSAFEMODE;}
"continue"                  { return token::CONTINUE;}

 /* Include statement is handled by the lexer */
[ \t]*include[ \t]*\"       {BEGIN IFILE; }
<IFILE>[^ \t\n\"]+          {
								{
									//This ignores anything up to the newline.
									int c;
									while((c=yyinput()) && c != '\n');
								}
								yylineno++;
								if(newfile(yytext) == 0)
									yyterminate();  /*file not found*/
								yylloc -> begin.filename = yylloc -> end.filename = new std::string(curfilename);
								prompt = "";
								BEGIN INITIAL;
								FLAG_inclusion_mode = 1;
							}
<IFILE>.|\n                 {
								cerr << curfilename << ":" << yylineno << ": bad include line." << endl;
								yyterminate();
							}


 /* built in functions */
"cos"                       |
"sin"                       |
"tan"                       |
"cosh"                      |
"sinh"                      |
"tanh"                      |
"acos"                      |
"asin"                      |
"atan"                      |
"atan2"                     |
"sqrt"                      |
"exp"                       |
"log10"                     |
"ceil"                      |
"fabs"                      |
"floor"                     |
"log"                       { yylval->ident = new string(yytext); return token::FUNCTION; }

"vector"                    {return token::VECTOR;}

 /* ================================================================================================= */
 /* ========              INITIAL COMMANDS (VERBS)                                          ========= */
 /* ================================================================================================= */

"model"                             {return token::MODEL;}
"add"                               {return token::ADD;}
"define"                            {return token::DEFINE;}
"fix"                               {return token::FIX;}
"free"                              {return token::FREE;}
"remove"                            {return token::REMOVE;}
"simulate"                          {return token::SIMULATE;}
"compute"                           {return token::COMPUTE;}
"print"                             {return token::CMD_print;}
"help"                              {return token::CMD_help;}
"whos"                              {return token::CMD_whos;}
"check"                             {return token::CMD_check;}
"mesh"                              {return token::MESH;}
"require"                           {return token::REQUIRE;}
"set"                               {return token::SET;}


  /* New loading stage.... should change */
"new"                               {return token::NEW;}
"loading"                           {return token::LOADING;}
"stage"                             {return token::STAGE;}
"restart"                           {return token::RESTART;}

 /* ================================================================================================= */
 /* ========                     ADD                                                        ========= */
 /* ================================================================================================= */
 
 /* Things which can be added (nouns). May be several phrases */
"node"                              {return token::NODE;}
"element"                           {return token::ELEMENT;}
"elements"                           {return token::ELEMENTS;}
"material"                          {return token::MATERIAL;}
"load"                              {return token::LOAD;}
"acceleration field"                {return token::ACCELERATION_FIELD;}
"time history"                      {return token::TIMEHISTORY;}
"mass"                              {return token::MASS;}
"imposed motion"                    {return token::IMPOSEDMOTION;}
"damping"                           {return token::DAMPING;}
"constraint"                        {return token::CONSTRAINT;}
"reaction"                          {return token::REACTION;}
"forces"                            {return token::FORCES;}
"domain reduction method"           {return token::DRM;}
"section"                           {return token::SECTION;}
"load pattern"                      {return token::LOADPATTERN;}
 /* "penalty displacement"              {return token::PENALTYDISPLACEMENT;} */
"load value"                        {return token::LOADVALUE;}

 /* Element names */
"8NodeBrick"                        {return token::EightNodeBrick;}
"8NodeBrickLT"                      {return token::EightNodeBrickLT;}
"8NodeBrickLTNoOutput"              {return token::EightNodeBrickLTNoOutput;}
"20NodeBrick"                       {return token::TwentyNodeBrick;}
"27NodeBrick"                       {return token::TwentySevenNodeBrick;}
"27NodeBrickLT"                     {return token::TwentySevenNodeBrickLT;}
"8NodeBrick_elastic"                {return token::EightNodeBrickElastic;}
"20NodeBrick_elastic"               {return token::TwentyNodeBrickElastic;}
"27NodeBrick_elastic"               {return token::TwentySevenNodeBrickElastic;}
"8NodeBrick_upU"                    {return token::EightNodeBrick_upU;}
"8NodeBrick_up"                     {return token::EightNodeBrick_up;}
"20NodeBrick_upU"                   {return token::TwentyNodeBrick_uPU;}
"beam_displacement_based"           {return token::beam_displacement_based;}
"beam_elastic"                      {return token::beam_elastic;}
"beam_9dof_elastic"                 {return token::beam_9dof_elastic;}
"beam_elastic_lumped_mass"          {return token::beam_elastic_lumped_mass;}
"4NodeShell_MITC4"                  {return token::FourNodeShellMITC4;}
"4NodeShell_NewMITC4"               {return token::FourNodeShellNewMITC4;}
"3NodeShell_ANDES"                  {return token::ThreeNodeShellANDES;}
"4NodeShell_ANDES"                  {return token::FourNodeShellANDES;}
"truss"                             {return token::truss;}
"contact"                           {return token::contact;}
"FrictionalPenaltyContact"          {return token::FrictionalPenaltyContact;}
"variable_node_brick_8_to_27"       {return token::variable_node_brick_8_to_27;}
"ShearBeamLT"                       {return token::ShearBeamLT;}


 /* Element options */
"porosity"                          {return token::porosity;}
"rho_s"                             {return token::rho_s;}
"rho_f"                             {return token::rho_f;}
"k_x"                               {return token::k_x;}
"k_y"                               {return token::k_y;}
"k_z"                               {return token::k_z;}
"K_s"                               {return token::K_s;}
"K_f"                               {return token::K_f;}
"pressure"                          {return token::pressure;}
"cross_section"                     {return token::cross_section;}
"shear_modulus"                     {return token::shear_modulus;}
"torsion_Jx"                        {return token::torsion_Jx;}
"bending_Iz"                        {return token::bending_Iz;}
"bending_Iy"                        {return token::bending_Iy;}
"mass_density"                      {return token::mass_density;}
"xz_plane_vector"                   {return token::xz_plane_vector;}
"joint_1_offset"                    {return token::joint_1_offset;}
"joint_2_offset"                    {return token::joint_2_offset;}
"direction"                         {return token::direction;}
"stiffness"                         {return token::stiffness;}
"normal_stiffness"                  {return token::normal_stiffness;}
"tangential_stiffness"              {return token::tangential_stiffness;}
"normal_damping"                    {return token::normal_damping;}
"tangential_damping"                {return token::tangential_damping;}
"friction_ratio"                    {return token::friction_ratio;}
"maximum_gap"                       {return token::maximum_gap;}
"contact_plane_vector"              {return token::contact_plane_vector;}


 /* section types */
"Membrane_Plate_Fiber"              {return token::MembranePlateFiber;}
"Elastic_Membrane_Plate"            {return token::ElasticMembranePlate;}

 /* section options */
"thickness"                         {return token::thickness;}

 /* Material Names*/
"linear_elastic_isotropic_3d"       {return token::linear_elastic_isotropic_3d;}
"VonMisesLT"        				{return token::VonMisesLT;}
"DruckerPragerLT"        				{return token::DruckerPragerLT;}
"sanisand2008"                      {return token::sanisand2008;}
"camclay"                           {return token::camclay;}
"camclay_accelerated"               {return token::camclay_accelerated;}
"sanisand2004"                      {return token::sanisand2004;}
"linear_elastic_crossanisotropic"   {return token::linear_elastic_crossanisotropic;}
"uniaxial_elastic"                  {return token::uniaxial_elastic_1d;}
"uniaxial_steel01"                  {return token::uniaxial_steel01;}
"uniaxial_steel02"                  {return token::uniaxial_steel02;}
"uniaxial_concrete02"               {return token::uniaxial_concrete02;}

 /* LTensor based materials */

"NDMaterialLT"    					{return token::NDMaterialLT;}
"linear_elastic_isotropic_3d_LT"    {return token::linear_elastic_isotropic_3d_LT;}
"PisanoLT"                      	|
"FTSModel"                          {return token::PisanoLT;}


 /* Material options */
"elastic_modulus"                   {return token::elastic_modulus;}
"elastic_modulus_horizontal"        {return token::elastic_modulus_horizontal;}
"elastic_modulus_vertical"          {return token::elastic_modulus_vertical;}
"viscoelastic_modulus"              {return token::viscoelastic_modulus;}
"poisson_ratio"                     {return token::poisson_ratio;}
"poisson_ratio_h_v"                 {return token::poisson_ratio_h_v;}
"poisson_ratio_h_h"                 {return token::poisson_ratio_h_h;}
"shear_modulus_h_v"                 {return token::shear_modulus_h_v;}
"von_mises_radius"                  {return token::von_mises_radius;}
"druckerprager_angle"               {return token::druckerprager_angle;}
"druckerprager_k"               	{return token::druckerprager_k;}
"armstrong_frederick_ha"            {return token::armstrong_frederick_ha;}
"armstrong_frederick_cr"            {return token::armstrong_frederick_cr;}
"initial_confining_stress"          {return token::initial_confining_stress;}
"pressure_reference_p0"             {return token::pressure_reference_p0;}
"strain"                            {return token::strain;}
"stress"                            {return token::stress;}
"control"                           {return token::control;}

"explicit"                          {
										yylval->value = new Quantity(0.0,ESSIunits::unitless);
										return token::NUMBER;
									}
"implicit"                          {
										yylval->value = new Quantity(1.0,ESSIunits::unitless);
										return token::NUMBER;
									}
"isotropic_hardening_rate"          {return token::isotropic_hardening_rate;}
"kinematic_hardening_rate"          {return token::kinematic_hardening_rate;}
"initial_void_ratio"                {return token::initial_void_ratio;}
"initial_shear_modulus"             {return token::initial_shear_modulus;}
"initial_bulk_modulus"              {return token::initial_bulk_modulus;}
"reference_void_ratio"              {return token::reference_void_ratio;}
"critical_stress_ratio_M"           {return token::critical_stress_ratio_M;}
"minimum_bulk_modulus"              {return token::minimum_bulk_modulus;}
"initial_mean_pressure"             {return token::initial_mean_pressure;}
"e0"                      			{return token::e0;}
"sanisand2008_Pat"                      {return token::sanisand2008_Pat;}
"sanisand2008_k_c"                      {return token::sanisand2008_k_c;}
"sanisand2008_K0"                      {return token::sanisand2008_K0;}
"sanisand2008_G0"                      {return token::sanisand2008_G0;}
"sanisand2008_alpha_cc"                 {return token::sanisand2008_alpha_cc;}
"sanisand2008_c"                        {return token::sanisand2008_c;}
"sanisand2008_xi"                       {return token::sanisand2008_xi;}
"sanisand2008_lambda"                   {return token::sanisand2008_lambda;}
"sanisand2008_ec_ref"                   {return token::sanisand2008_ec_ref;}
"sanisand2008_m"                        {return token::sanisand2008_m;}
"sanisand2008_h0"                       {return token::sanisand2008_h0;}
"sanisand2008_ch"                       {return token::sanisand2008_ch;}
"sanisand2008_nb"                       {return token::sanisand2008_nb;}
"sanisand2008_A0"                       {return token::sanisand2008_A0;}
"sanisand2008_nd"                       {return token::sanisand2008_nd;}
"sanisand2008_p_r"                      {return token::sanisand2008_p_r;}
"sanisand2008_rho_c"                    {return token::sanisand2008_rho_c;}
"sanisand2008_theta_c"                  {return token::sanisand2008_theta_c;}
"sanisand2008_X"                        {return token::sanisand2008_X;}
"sanisand2008_z_max"                    {return token::sanisand2008_z_max;}
"sanisand2008_cz"                       {return token::sanisand2008_cz;}
"sanisand2008_p0"                       {return token::sanisand2008_p0;}
"sanisand2008_p_in"                     {return token::sanisand2008_p_in;}
"yield_strength"                    {return token::yield_strength;}
"strain_hardening_ratio"            {return token::strain_hardening_ratio;}
"a2"                                {return token::a2;}
"a3"                                {return token::a3;}
"a4"                                {return token::a4;}
"R0"                                {return token::R0;}
"cR1"                               {return token::cR1;}
"cR2"                               {return token::cR2;}
"compressive_strength"              {return token::compressive_strength;}
"strain_at_compressive_strength"    {return token::strain_at_compressive_strength;}
"crushing_strength"                 {return token::crushing_strength;}
"strain_at_crushing_strength"       {return token::strain_at_crushing_strength;}
"tensile_strength"                  {return token::tensile_strength;}
"tension_softening_stiffness"       {return token::tension_softening_stiffness;}
"sanisand2004_Pat"                            {return token::sanisand2004_Pat;}
"sanisand2004_G0"                            {return token::sanisand2004_G0;}
"sanisand2004_p_cut"                          {return token::sanisand2004_p_cut;}
"sanisand2004_Mc"                             {return token::sanisand2004_Mc;}
"sanisand2004_c"                              {return token::sanisand2004_c;}
"sanisand2004_lambda_c"                       {return token::sanisand2004_lambda_c;}
"sanisand2004_xi"                             {return token::sanisand2004_xi;}
"sanisand2004_ec_ref"                         {return token::sanisand2004_ec_ref;}
"sanisand2004_m"                              {return token::sanisand2004_m;}
"sanisand2004_h0"                             {return token::sanisand2004_h0;}
"sanisand2004_ch"                             {return token::sanisand2004_ch;}
"sanisand2004_nb"                             {return token::sanisand2004_nb;}
"sanisand2004_A0"                             {return token::sanisand2004_A0;}
"sanisand2004_nd"                             {return token::sanisand2004_nd;}
"sanisand2004_z_max"                          {return token::sanisand2004_z_max;}
"sanisand2004_cz"                             {return token::sanisand2004_cz;}
"strain_increment_size"             {return token::strain_increment_size;}
"maximum_strain"                      {return token::maximum_strain;}
"number_of_times_reaching_maximum_strain" {return token::number_of_times_reaching_maximum_strain;}
"constitutive"                      {return token::CONSTITUTIVE;}
"testing"                           {return token::testing;}
"constant"                          {return token::constant;}
"mean"                              {return token::mean;}
"triaxial"                          {return token::triaxial;}
"drained"                           {return token::drained;}
"undrained"                         {return token::undrained;}
"simple"                            {return token::simple;}
"shear"                             {return token::shear;}
"number_of_subincrements"           {return token::number_of_subincrements;}
"maximum_number_of_iterations"      {return token::maximum_number_of_iterations;}
"tolerance_1"                       {return token::tolerance_1;}
"tolerance_2"                       {return token::tolerance_2;}
"Gauss"                             {return token::Gauss;}
"each"                              {return token::each;}
"points"                            {return token::points;}
"point"                             {return token::point;}
"single"                            {return token::single;}
"value"                             {return token::value;}


 /* Pisano Model options  */
"M_in"                              {return token::M_in;}
"kd_in"                             {return token::kd_in;}
"xi_in"                             {return token::xi_in;}
"h_in"                              {return token::h_in;}
"m_in"                              {return token::m_in;}
"beta_min"                          {return token::beta_min;}
"n_in"                              {return token::n_in;}
"a_in"                              {return token::a_in;}
"elastic_modulus_1atm"              {return token::elastic_modulus_1atm;}
"eplcum_cr_in"                      {return token::eplcum_cr_in;}

 /* Load options */
"Fx"                                |
"Fy"                                |
"Fz"                                |
"Mx"                                |
"My"                                |
"Mz"                                |
"F_fluid_x"                         |
"F_fluid_y"                         |
"F_fluid_z"                         {
										yylval->ident = new string(yytext);
										return token::FORCE;
									}

 /* Acceleration field options*/
"ax"                                {return token::ax;}
"ay"                                {return token::ay;}
"az"                                {return token::az;}

 /* Time-History types */
"path_series"                       {return token::TH_PATH_SERIES;}
"path_time_series"                  {return token::TH_PATH_TIME_SERIES;}
"linear"                            {return token::TH_LINEAR;}
"from_reactions"                    {return token::TH_FROM_REACTIONS;}

 /* Element load types */
"self_weight"                       {return token::self_weight;}
"surface"                           {return token::surface;}

 /* Time-History options */
"start_time"                        {return token::startTime;}
"end_time"                          {return token::endTime;}
"period"                            {return token::Period;}
"phase"                             {return token::Phase;}
"amplitude"                         {return token::Amplitude;}
"frequency"                         {return token::frequency;}
"frequency1"                        {return token::frequency1;}
"frequency2"                        {return token::frequency2;}
"frequency3"                        {return token::frequency3;}
"frequency4"                        {return token::frequency4;}
"max_time"                          {return token::MaxTime;}
"magnitudes"                        {return token::MAGNITUDES;}
"magnitude"                         {return token::MAGNITUDE;}

 /* Mass options */
"mx"                                {return token::mx;}
"my"                                {return token::my;}
"mz"                                {return token::mz;}
"Imx"                               {return token::Imx;}
"Imy"                               {return token::Imy;}
"Imz"                               {return token::Imz;}

 /* Damping model types */
"Rayleigh"                          {return token::DAMPING_RAYLEIGH;}
"Caughey3rd"                        {return token::DAMPING_CAUGHEY3;}
"Caughey4th"                        {return token::DAMPING_CAUGHEY4;}

 /* Damping model options */
"a0"                                {return token::a0;}
"a1"                                {return token::a1;}
"stiffness_to_use"                  {return token::stiffness_to_use;}
"Initial_Stiffness"                 |
"Current_Stiffness"                 |
"Last_Committed_Stiffness"           {
										yylval->ident = new string(yytext);
										return token::stiffness_to_use_opt;
									}

 /* Constraint types */
"equal dof"                         {return token::equaldof;}
"equaldof"                          {return token::equaldof;}

 /* Constraint options */
"master"                            {return token::master;}
"slave"                             {return token::slave;}
"dof to constrain"                  {return token::dof_to_constrain;}

 /* DRM OPtions */
"scale_factor"               {return token::scale_factor;}
"displacement_scale_unit"    {return token::displacement_scale_unit;}
"velocity_scale_unit"        {return token::velocity_scale_unit;}
"acceleration_scale_unit"    {return token::acceleration_scale_unit;}
"number_of_steps"            {return token::number_of_steps;}
"number_of_boundary_nodes"      {return token::number_of_boundary_nodes;}
"number_of_exterior_nodes"      {return token::number_of_exterior_nodes;}
"number_of_drm_elements"     {return token::number_of_drm_elements;}
"element_file"               {return token::element_file;}
"boundary_nodes_file"                 {return token::boundary_nodes_file;}
"exterior_nodes_file"                 {return token::exterior_nodes_file;}
"displacement_file"          {return token::displacement_file;}
"acceleration_file"          {return token::acceleration_file;}
"hdf5_file"             {return token::hdf5_file;}
"velocity_file"              {return token::velocity_file;}
"force_file"                 {return token::force_file;}
"series_file"                {return token::series_file;}
"time_series_file"           {return token::time_series_file;}


 /* ================================================================================================= */
 /* ========                     DEFINE                                                     ========= */
 /* ================================================================================================= */

 /* Defineable things */
"algorithm"                          {return token::ALGORITHM;}
"convergence test"                   {return token::CONVERGENCE_TEST;}
"integration"                   	 {return token::INTEGRATION;}
"solver"                             {return token::SOLVER;}
"dynamic integrator"                 {return token::DYNAMICINTEGRATOR;}
"static integrator"                  {return token::STATICINTEGRATOR;}
"output"                             {return token::OUTPUT;}

 /* Constitutive algorithm options*/
"Euler_One_Step"          				          |
"Euler_Multistep"          						  |
"Modified_Euler_Error_Control"                    |
"Runge_Kutta_45_Error_Control"                    |
"Backward_Euler"                     {yylval->ident = new string(yytext); return token::CONSTITUTIVE_ALGNAME;}

 /* Algorithm options*/
"With_no_convergence_check"          |
"with_no_convergence_check"          |
"Modified_Newton"                    |
"modified_newton"                    |
"newton"                             |
"Newton"                             {
										yylval->ident = new string(yytext);
										return token::ALGNAME;
									 }

"yield_function_relative_tolerance"   	{return token::yield_function_relative_tolerance;}
"stress_relative_tolerance" 		  	{return token::stress_relative_tolerance;}

 /* Test options */
"Norm_Displacement_Increment"       |
"Norm_Unbalance"                    |
"Energy_Increment"                  {
										yylval->ident = new string(yytext);
										return token::TESTNAME;
									}
"tolerance"                         {return token::tolerance;}
"maximum_iterations"                {return token::maximum_iterations;}
"verbose_level"                     {return token::verbose_level;}
"minimum_time_step"                 {return token::minimum_time_step;}
"maximum_time_step"                 {return token::maximum_time_step;}
"number_of_iterations"              {return token::number_of_iterations;}

 /* define solver options */
"ProfileSPD"                        |
"profilespd"                        |
"parallel"                          |
"umfpack"                           |
"UMFPack"                           {
										yylval->ident = new string(yytext);
										return token::SOLVERNAME;
									}

 /* define static and dynamic integrator options */
"Newmark"                           {return token::DYNAMICINTEGRATOR_NEWMARK;}
"Hilber_Hughes_Taylor"              {return token::DYNAMICINTEGRATOR_HHT;}
"displacement_control"              {return token::STATICINTEGRATOR_DISPLACEMENT;}


 /* SIMULATE*/
"static"                            {return token::STATIC;}
"dynamic"                           {return token::DYNAMIC;}
"transient"                         {return token::TRANSIENT;}
"variable transient"                {return token::VARIABLETRANSIENT;}
"eigen"                             {return token::EIGEN;}
"using"                             {return token::USING;}
"time_step"                         {return token::time_step;}
"number_of_modes"                   {return token::number_of_modes;}

 /* OUTPUT */
"binary"                            {return token::BINARY;}
"text"                              {return token::TEXT;}
"enable"                            {return token::ENABLE;}
"disable"                           {return token::DISABLE;}
"compression"                       {return token::COMPRESSION;}


 /* ================================================================================================= */
 /* ========                    AUXILLIARY WORDS                                            ========= */
 /* ================================================================================================= */

"at"                                {return token::AT;}
"all"                               {return token::ALL;}
"and"                               {return token::AND;}
"dofs"                              {return token::TEXTDOFS;}
"factor"                            {return token::FACTOR;}
"increment"                         {return token::INCREMENT;}
"name"                              {return token::NAME;}
"number"                            {return token::TEXTNUMBER;}
"No"                                {return token::TEXTNUMBER;}
"#"                                 {return token::TEXTNUMBER;}
"of"                                {return token::of;}
"steps"                             {return token::STEPS;}
"to"                                {return token::TO;}
"type"                              {return token::TYPE;}
"use"                               {return token::USE;}
"with"                              {return token::WITH;}
"nodes"                             {return token::NODES;}
"domain"                            {return token::DOMAIN_;}
"dof"                               {return token::dof;}
"response"                          {return token::RESPONSE;}
"file"                              {return token::FILE;}
"from"                              {return token::FROM;}
"every"                             {return token::EVERY;}
"level"                             {return token::LEVEL;}

 /* Greek */
"alpha"                             {return token::alpha;}
"beta"                              {return token::beta;}
"gamma"                             {return token::gamma;}
"lambda"                            {return token::lambda;}
"kappa"                             {return token::kappa;}
"delta"                             {return token::delta;}


"integration_points"                {return token::INTEGRATIONPOINTS;}
"IntegrationRule"                   {return token::IntegrationRule;}


 /* DOFS */
"ux"                                |
"uy"                                |
"uz"                                |
"rx"                                |
"ry"                                |
"rz"                                |
"Ux"                                |
"Uy"                                |
"Uz"                                |
"p"                                 {
										yylval->ident = new string(yytext);
										return token::DOF;
									}

 /*Other language commands*/
"quit"                          |
"exit"                          |
"bye"                           {
									yylval->ident = new string(yytext);
									return token::BYE;
								}
"einstein;"                       {void show_einstein(); show_einstein();}




 /* ================================================================================================= */
 /* ========                     NON KEYWORDS                                               ========= */
 /* ================================================================================================= */

 /* names */
[_a-zA-Z][_a-zA-Z0-9]*              {
										yylval->ident = new string(yytext);
										return token::IDENT;
									}

 /*numbers*/
[0-9]+"."[0-9]*{EXP}?               |
"."?[0-9]+{EXP}?                    {
										yylval->value = new Quantity(atof(yytext), ESSIunits::unitless);
										return token::NUMBER;
									}
 /* End-of-file discards current file and returns to the previous, otherwise to the interactive prompt */
<<EOF>>                             {
										if (popfile()==0)
										{
											if(FLAG_interactive_mode == 1)
											{
												newstdin();
												yyrestart(yyin);
												//yylloc -> begin.filename = yylloc -> end.filename = new std::string(curfilename);
												prompt = thePrompt;
												//cout << prompt;
											}
											else
											{
												cout << endl << "You forgot to say \"bye\"... " << endl;
												wipe_model();
												exit(0);
											}

										}
										if (strncmp(curfilename, initial, 5) == 0) //I dont like strncmp :(
										{
											prompt = thePrompt;
											//cout << prompt;
										}
										yylloc -> begin.filename = yylloc -> end.filename = new std::string(curfilename);
									}

 /* comments */
"//".*
"#!".*


 /* ignore white space, tabs and carriage return*/
[ \t\r]                               //{yylloc -> step();}

 /* ignore newline*/
 /*\n                                  {yylloc -> lines(1);} */
\n                                  {
										yycolumn = 1;
									}

 /* Match text strings */
L?\"(\\.|[^\\"])*\"                 {
										std::string* thisString = new std::string(yytext);
										yylval->ident = thisString;
										return token::STRING;
									}

  /* catch-all of the rest and report (only useless characters like ? get to this point */
.	                                { cout << "Failed to parse the character at line " << yylineno << " : " << *yytext << endl; }

%%


int newfile(char* fn) {
	FILE *f = fopen(fn, "r");

	bufstack *bs = (bufstack*) malloc(sizeof( bufstack)) ;

	if (!f)     {perror(fn); return 0 ;}
	if (!bs)    {perror("Out of memory."); exit(1);}

	if (curbs) curbs->lineno = yylineno;
	bs->prev = curbs;

	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs -> f = f;
	bs -> filename = fn;
	bs -> lineno = 1;
	yy_switch_to_buffer(bs->bs);
	curbs = bs;
	yylineno = 1;
	curfilename = fn;

	cout << "Including: \"" << fn << "\"" << endl << endl;

	return 1;
}



int newstdin()
{

	FILE *f = stdin;

	bufstack *bs = (bufstack *) malloc(sizeof( bufstack)) ;


	if (!f)
	{
		perror("Could not open STDIN!");
		return 0 ;
	}
	if (!bs)
	{
		perror("Out of memory.");
		exit(1);
	}

	if (curbs)
	{
		curbs->lineno = yylineno;
	}
	bs->prev = curbs;
	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs -> f = f;
	bs -> lineno = 1;
	bs -> filename = initial;

	yy_switch_to_buffer(bs->bs);
	curbs = bs;


	yylineno = 1;
	curfilename = initial;

	cout << "Input: STDIN" << endl << endl;
	prompt = thePrompt;
	//cout << prompt ;
	return 1;
}

int popfile(void)
{
	bufstack *bs = curbs;
	bufstack *prevbs;

	if (!bs)
	{
		return 0;
	}

	fclose(bs->f);
	yy_delete_buffer(bs->bs);

	prevbs = bs-> prev;
	free(bs);

	if (!prevbs)
	{
		return 0;
	}

	yy_switch_to_buffer(prevbs->bs);
	curbs = prevbs;

	cout << "Done including: \"" << curfilename << "\" (" << yylineno << " lines included)." << endl;
	cout << "Continuing with \"" << curbs->filename << "\" at line " << curbs->lineno << "." << endl;

	cout << endl << prompt;

	yylineno = curbs->lineno;
	curfilename = curbs->filename;

	return 1;
}

int error_behavior()
{
	//while (popfile() != 0); // Discard everything!

	//Start new interactive mode
	if ( FLAG_interactive_mode == 1 )
	{
		newstdin();
		yyrestart(yyin);

		prompt = thePrompt;
		//if (recovery_mode == 1)
		//    prompt = "(safe) " + thePrompt;
	}
	else
	{
		exit(-1);
	}

	return 0;
}


void print_usage()
{
	cout <<
		 "RealESSI Simulator: UCDavis Earthquake-Soil-Structure-Interaction Simulator \n"
		 "              -          -    -         -                     \n"
		 "Usage: essi [-cfhnsmbe FILENAME]\n"
		 "  -c --cpp-output                : Output cpp version of the analysis done. \n"
		 "  -f --filename [FILENAME]       : Include and run FILENAME. \n"
		 "  -h --help                      : Print this message. \n"
		 "  -n --no-interactive            : Disable interactive mode. \n"
		 "  -s --set-variable              : Set a variable from the command line. \n"
		 "  -d --dry-run                   : Do not execute ESSI API calls. Just parse. \n"
		 "  -m --model-name [NAME]         : Set the model name from the command line. \n"
		 "  -p --profile-report [FILENAME] : Set the filename for the profiler report (and activate lightweight profiling)\n"
		 " \n\n"
		 "Example to set a variable name from command line:\n"
		 "    essi -s a=10,b=20,c=30\n"
		 "Runs ESSI with variables a, b, and c set to 10, 20 and 30 respectively.\n"
		 "At this time, only ESSIunits::unitless variables can be set. \n"
		 "\n\n";

}





#ifdef _PARALLEL_PROCESSING
  #include "../ModifiedOpenSeesServices/domain/domain/partitioned/PartitionedDomain.h"
  #include "../ModifiedOpenSeesServices/graph/partitioner/GraphPartitioner.h"
  #include <../ModifiedOpenSeesServices/actor/objectBroker/FEM_ObjectBroker.h>
  #include <../ModifiedOpenSeesServices/actor/machineBroker/MachineBroker.h>
  #include <../ModifiedOpenSeesServices/actor/machineBroker/MPI_MachineBroker.h>

  extern PartitionedDomain theDomain;
  extern int OPS_PARALLEL_PROCESSING;
  extern int OPS_NUM_SUBDOMAINS;
  extern bool OPS_PARTITIONED;
  extern FEM_ObjectBroker *OPS_OBJECT_BROKER;
  extern MachineBroker  *OPS_MACHINE;
  extern GraphPartitioner  *OPS_GRAPH_PARTITIONER;
  static MPI_MachineBroker *theMachineBroker = 0;
#endif


int start_parser(int argc, char**argv)
{

	//Allegedly, this forces cout to be non-buffered. Always flushes!!
	std::cout<<std::unitbuf; 

	
	//Welcome message
	welcome_msg();

	// Generate previously known units list
	set_known_units();

	//Parse Command Line Parameters
	int c;
	char *filename_from_command_line = 0;

	while (1)
	{
		static struct option long_options[] =
		{
			{"cpp-output",       no_argument,       0, 'c'},
			{"file",             required_argument, 0, 'f'},
			{"help",             no_argument,       0, 'h'},
			{"model-name",       required_argument, 0, 'm'},
			{"no-interactive",   no_argument,       0, 'n'},
			{"set-variable",     required_argument, 0, 's'},
			{"no-cpp-output",    no_argument,       0, 'x'},
			{"dry-run",          no_argument,       0, 'd'},
            {"profile-report",   required_argument, 0, 'p'},
			{"debug-attach",     no_argument      , 0, 'a'},
			{0, 0, 0, 0}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		//This is the command line option parser
		c = getopt_long(argc, argv, "cf:hns:xm:dp:a", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		{
			break;
		}

		switch (c)
		{

			case 'c':  // -c --cpp-output: Create .cpp output
				FLAG_generate_cpp_output = 1;
				break;
			case 'f': // -f --file: Take input from file
				filename_from_command_line = optarg;
				FLAG_run_file = 1;
				break;
			case 'h': // -h --help: print usage
				print_usage();
				return (0);
				break;
			case 'n': // -n --no-interactive: disable interactive mode
				FLAG_interactive_mode = 0;
				break;
			case 's': // -s --set-variable: Set a variable from the command line
				set_variables(optarg);
				break;
			case 'x': // -x --no-cpp-output: disable cpp output (default)
				FLAG_generate_cpp_output = 0;
				break;
			case 'd': // -d --dry-run: do not call API... just make parser call and produce cpp output if so enabled
				FLAG_dry_run = 1;
				set_dry_run_mode(FLAG_dry_run);
				break;
			case 'm': // -m --model-name: Set model name from the command line
				set_model_name(optarg);
				break;
			case 'b': //  -b --binary-output: The output generated will be in binary instead of plain text.
				FLAG_binary_output = 1;
				break;
			case 'p': // -p --profile-report: Set the filename for the profiler report (and activate profiling)
				//cout << "Setting report file to:" << optarg << endl;
				set_profiling_results_filename(optarg);
				break;
            case 'a': // Debugger attach loop
                FLAG_debug_attach = 1;
                break;
			default:
				print_usage();
				return (0);
		}
	}

	if (optind < argc)
	{
		cout << "Unexpected option" << endl << endl;
		print_usage();
		return (0);
	}

	if (FLAG_interactive_mode)
	{
		newstdin();
	}

	if (FLAG_run_file)
	{
		newfile(filename_from_command_line);
		set_outcppfile_name(string(filename_from_command_line));
		prompt = "";
	}

    if(FLAG_debug_attach)
    {       
            int i = 0;
            char hostname[256];
            gethostname(hostname, sizeof(hostname));
            printf("PID %d on %s ready for attach\n", getpid(), hostname);
            fflush(stdout);
            while (0 == i)
                sleep(5);
    }

#ifdef _PARALLEL_PROCESSING
	//Kill other processes in case of dry run... parallel is not needed.
	if(FLAG_dry_run)
	{
		theMachineBroker->shutdown();
	}
#endif

	// Parse flag
	int exitflag = -1;

	//Construct a parser object
	yy::feiparser parser;

	// Start parsing!
	cout << prompt;
	exitflag = parser.parse();

	//Exit
	return exitflag;
}




/*  SIGNAL HANDLING  */


void signalHandler(int signum)
{
    #ifdef _PARALLEL_PROCESSING
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "(" << rank << ") ";
    #endif
	if(signum == SIGINT)
	{
		cout << "Caught SIGINT! Program ending." << endl;
	}
	else if(signum == SIGABRT)
	{
		cout << "Caught SIGABRT! Program ending." << endl;
	}
	else if(signum == SIGSEGV)
	{
		cout << "Caught SIGSEGV! Program ending." << endl;
	}
	else if(signum == SIGTERM)
	{
		cout << "Caught SIGTERM! Program ending." << endl;
	}
	else if(signum == SIGILL)
	{
		cout << "Caught SIGILL! Program ending." << endl;
	}
	else if(signum == SIGFPE)
	{
		cout << "Caught SIGFPE! Program ending." << endl;
        int exceptions;
        exceptions = fetestexcept(FE_INVALID | FE_DIVBYZERO| FE_OVERFLOW);
        if(exceptions & FE_INVALID)
        {
            cout << "   Floating point error -- (FE_INVALID) - Undefined operation (0/0, 0/1, sqrt(-1), inf - int, etc.)\n" << endl;
        }
        if(exceptions & FE_DIVBYZERO)
        {
            cout << "   Floating point error -- (FE_DIVBYZERO) - 0/0\n" << endl;
        }
        if(exceptions & FE_OVERFLOW)
        {
            cout << "   Floating point error -- (FE_OVERFLOW) - Result is larger than the largest representable number at current accuracy. \n" << endl;
        }
        if(exceptions & FE_UNDERFLOW)
        {
            cout << "   Floating point error -- (FE_UNDERFLOW) - Result too small (but not zero) to be represented as a number at current accuracy. \n" << endl;
        }

	}
	else if(signum == SIGUSR1 || signum == SIGUSR2)
	{
		cout << "HDF5 set to flush, please wait...\n\n";
		call_hdf5_flush_next_chance();
		return; // Do not quit on these signals.
	}
    print_stacktrace();
	exit(signum);
}

 /* END SIGNALS */

int main (int argc, char**argv)
{
    feclearexcept(FE_ALL_EXCEPT);

    feenableexcept(FE_INVALID   | 
                   FE_DIVBYZERO | 
                   FE_OVERFLOW);

	signal(SIGINT, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);


	int exitflag = -1;

#ifdef _PARALLEL_PROCESSING
	// In parallel processing, a few things have to be setup before starting.

   FEM_ObjectBroker theBroker;
   MPI_MachineBroker theMachine(&theBroker, argc, argv);
   theMachineBroker = &theMachine;
   int rank = theMachine.getPID();
   int np = theMachine.getNP();

   if (rank != 0) //If this is the main machine
   {

		exitflag = theMachine.runActors();
   } 
   else  // Or if it is a slave node
   {
		OPS_OBJECT_BROKER       = &theBroker;
		OPS_MACHINE             = &theMachine;
		OPS_PARALLEL_PROCESSING = np;
		OPS_NUM_SUBDOMAINS      = np - 1;
		OPS_PARTITIONED         = false;

		exitflag = start_parser(argc, argv);
	}

	//this got moved to the wipe_model() API call...... 
	theDomain.clearAll();
	theMachine.shutdown();

	cout << "Process # " << rank << " ended with exitflag = " << exitflag << endl;
#else
	exitflag = start_parser(argc, argv);
#endif


	return exitflag;
}
  


int smaller(int a, int b)
{
	if (a >= b)
	{
		return b;
	}
	else
	{
		return a;
	}
}

// =================================================================================
// Please do not erase! Might make it into the code some day. (Jose Abell)
//
// This code replaces standard input as the main input mechanism, and implements
// a smarter prompt, which has auto replace, code history, etc.
//
// Buggy and unstable as it is. (To make it work, the is a macro that needs to be enabled on top of
// this file)
//
//static int my_yyinput(char *buf, int max_size)
//{
//    int   len;
//
//    if (st_readline_buffer == NULL)
//    {
//        st_readline_used_len = 0;
//        st_readline_buffer = readline(prompt.c_str());
//
//        if (st_readline_buffer == NULL)
//            return 0;
//        else
//        {
//            //strncat(st_readline_buffer, " ", 1);
//        }
//    }
//
//    len = smaller(strlen(st_readline_buffer) - st_readline_used_len, max_size);
//
//    if (len == 0)
//    {
//        strncat(st_readline_buffer, " ", 1);
//        len = 1;
//    }
//
//    strncpy(buf, &st_readline_buffer[st_readline_used_len], len);
//    add_history(st_readline_buffer);
//
//    st_readline_used_len += len;
//    if (st_readline_buffer[st_readline_used_len] == '\0')
//    {
//        free(st_readline_buffer);
//        st_readline_buffer = NULL;
//    }
//
//    return len;
//}
//// =================================================================================

#include <ctime>
void welcome_msg()
{
	time_t rawtime;
	struct tm *timeinfo;
	char timestring[80];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (timestring, 80, "%b %d %Y at %H:%M:%S", timeinfo);
	cout << "                                                               " << endl;
	cout << "                                                               " << endl;
	cout << "  The Finite Element Interpreter                               " << endl;
	cout << "                                                               " << endl;
	cout << "  Real ESSI                                                  " << endl;
	cout << "  Earthquake Soil Structure Interaction Simulator            " << endl;
	cout << "                                                             " << endl;
	#ifdef _PARALLEL_PROCESSING
	cout << " Parallel processing mode. Main machine!                     " << endl;
	#else
	cout << "  Sequential processing mode.                                " << endl;
	#endif
	cout << "                                                             " << endl;
	cout << "Version : " << VERSION << endl;
	cout << "Compiled: " << __DATE__ << " at " << __TIME__  << endl;
	cout << "Time Now: " << timestring  << endl;
	cout << "                                                               " << endl;
	cout << "Static startup tips:                                           " << endl;
	cout << " * Remember: Every command ends with a semicolon ';'.          " << endl;
	cout << " * Type 'quit;' or 'exit;' to finish.                          " << endl;
	cout << " * Run 'essi -h' to see available command line options.        " << endl;
	cout << "                                                               " << endl;
}

void set_known_units()
{
	//Base units*/
	global_variables["m"] = Quantity(1.0, ESSIunits::m);
	global_variables["kg"] = Quantity(1.0, ESSIunits::kg);
	global_variables["s"] = Quantity(1.0, ESSIunits::s);

	// Length
	global_variables["cm"] = Quantity(1.0, ESSIunits::cm);
	global_variables["mm"] = Quantity(1.0, ESSIunits::mm);
	global_variables["Km"] = Quantity(1.0, ESSIunits::km);

	//Time
	global_variables["Hz"] = Quantity(1.0, ESSIunits::Hz);
	global_variables["ms"] = Quantity(1.0, ESSIunits::ms);
	global_variables["ns"] = Quantity(1.0, ESSIunits::ns);

	//Force
	global_variables["N"] = Quantity(1.0, ESSIunits::N);
	global_variables["kN"] = Quantity(1.0, ESSIunits::kN);

	//Pressure
	global_variables["Pa"] = Quantity(1.0, ESSIunits::Pa);
	global_variables["kPa"] = Quantity(1.0, ESSIunits::kPa);
	global_variables["MPa"] = Quantity(1.0, ESSIunits::MPa);
	global_variables["GPa"] = Quantity(1.0, ESSIunits::GPa);

	//Constants
	global_variables["g"] = ESSIunits::g;  // gravitational acceleration on Earth at sea level and 45degrees of latitude
	global_variables["pi"] = Quantity(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986, ESSIunits::unitless); // pi to a useless ammount of significant digits

	// Put these variables in the locked list.... cuz we own this!
	map<string, Quantity>::const_iterator it;
	for (it = global_variables.begin();
			it != global_variables.end();
			it++)
	{
		locked_global_variables.insert(it->first);
	}
}

void set_variables(char *cstring)
{
	string variables(cstring);

	cout << endl << "The following variables are being set from the command line:" << endl << endl;

	while (1)
	{
		size_t i1 = variables.find("=");
		size_t i2 = variables.find(",");

		if (i1 == string::npos)
		{
			break;
		}

		string var_name(variables.substr(0, i1));
		string var_value(variables.substr(i1 + 1, i2 - i1 - 1));

		if (i2 == string::npos)
		{
			variables = "";
		}
		else
		{
			variables = variables.substr(i2 + 1);
		}

		cout << var_name << " = " << var_value << endl;

		double value = boost::lexical_cast<double>(var_value);
		global_variables[var_name] = Quantity(value, ESSIunits::unitless); // :) love boost!
	}
	cout << endl << endl;
}




























void show_einstein()
{

	cout << " "                                                                                 << endl;
	cout << "                    .+~                :xx++::"                                    << endl;
	cout << "                   :`. -          .!!X!~\"?!`~!~!. :-:."                           << endl;
	cout << "                  <             .!!!H\":.~ ::+!~~!!!~ `%X."                        << endl;
	cout << "                  '             ~~!M!!>!!X?!!!!!!!!!!...!~."                       << endl;
	cout << "                              <!:!MM!~:XM!!!!!!.:!..~ !.  `<"                      << endl;
	cout << "                  <: `   :~ .:<~!!M!XXHM!!!X!XXHtMMHHHX!  ~ ~"                     << endl;
	cout << "                ~~~~<' ~!!!:!!!!!XM!!M!!!XHMMMRMSXXX!!!!!!:  <`"                   << endl;
	cout << "                  `<  <::!!!!!X!X?M!!M!!XMMMMXXMMMM?!!!!!!?!:~<"                   << endl;
	cout << "               : '~~~<!!!XMMH!!XMXMXHHXXXXM!!!!MMMMSXXXX!!!!!!!~"                  << endl;
	cout << "            :    ::`~!!!MMMMXXXtMMMMMMMMMMMHX!!!!!!HMMMMMX!!!!!: ~"                << endl;
	cout << "               '~:~!!!!!MMMMMMMMMMMMMMMMMMMMMMXXX!!!M??MMMM!!X!!i:"                << endl;
	cout << "               <~<!!!!!XMMMMMMMMMMMM8M8MMMMM8MMMMMXX!!!!!!!!X!?t?!:"               << endl;
	cout << "               ~:~~!!!!?MMMMMM@M@RMRRR$@@MMRMRMMMMMMXSX!!!XMMMX<?X!"               << endl;
	cout << "             :XX <!!XHMMMM88MM88BR$M$$$$8@8RN88MMMMMMMMHXX?MMMMMX!!!"              << endl;
	cout << "           .:X! <XMSM8M@@$$$$$$$$$$$$$$$$$$$B8R$8MMMMMMMMMMMMMMMMX!X"              << endl;
	cout << "          :!?! !?XMMMMM8$$$$8$$$$$$$$$$$$$$BBR$$MMM@MMMMMMMMMMMMMM!!X"             << endl;
	cout << "        ~<!!~ <!!XMMMB$$$$$$$$$$$$$$$$$$$$$$$$MMR$8MR$MMMMMMMMMMMMM!?!:"           << endl;
	cout << "        :~~~ !:X!XMM8$$$$$$$$$$$$$$$$$$$$$$$RR$$MMMMR8NMMMMMMMMMMMMM<!`-"          << endl;
	cout << "    ~:<!:~`~':!:HMM8N$$$$$$$$$$$$$$$$$$$$$$$$$8MRMM8R$MRMMMMMMMMRMMMX!"            << endl;
	cout << "  !X!``~~   :~XM?SMM$B$$$$$$$$$$$$$$$$$$$$$$BR$$MMM$@R$M$MMMMMM$MMMMX?L"           << endl;
	cout << " X~.      : `!!!MM#$RR$$$$$$$$$$$$$$$$$R$$$$$R$M$MMRRRM8MMMMMMM$$MMMM!?:"          << endl;
	cout << " ! ~ <~  !! !!~`` :!!MR$$$$$$$$$$RMM!?!??RR?#R8$M$MMMRM$RMMMM8MM$MMM!M!:>"         << endl;
	cout << ": ' >!~ '!!  !   .!XMM8$$$$$@$$$R888HMM!!XXHWX$8$RM$MR5$8MMMMR$$@MMM!!!< ~"        << endl;
	cout << "!  ' !  ~!! :!:XXHXMMMR$$$$$$$$$$$$$$$$8$$$$8$$$MMR$M$$$MMMMMM$$$MMM!!!!"          << endl;
	cout << " ~<!!!  !!! !!HMMMMMMMM$$$$$$$$$$$$$$$$$$$$$$$$$$MMM$M$$MM8MMMR$$MMXX!!!!/:`"      << endl;
	cout << "  ~!!!  !!! !XMMMMMMMMMMR$$$$$$$$$$$$R$RRR$$$$$$$MMMM$RM$MM8MM$$$M8MMMX!!!!:"      << endl;
	cout << "  !~ ~  !!~ XMMM%!!!XMMX?M$$$$$$$$B$MMSXXXH?MR$$8MMMM$$@$8$M$B$$$$B$MMMX!!!!"      << endl;
	cout << "  ~!    !! 'XMM?~~!!!MMMX!M$$$$$$MRMMM?!%MMMH!R$MMMMMM$$$MM$8$$$$$$MR@M!!!!!"      << endl;
	cout << "  <>    !!  !Mf x@#\"~!t?M~!$$$$$RMMM?Xb@!~`??MS$M@MMM@RMRMMM$$$$$$RMMMMM!!!!"     << endl;
	cout << "  !    '!~ <!!:!?M   !@!M<XM$$R5M$8MMM$! -XXXMMRMBMMM$RMMM@$R$BR$MMMMXX?!X!!"      << endl;
	cout << "  !    '!  !!X!!!?::xH!HM:MM$RM8M$RHMMMX...XMMMMM$RMMRRMMMMMMM8MMMMMMMMX!!X!"      << endl;
	cout << "  !     ~  !!?:::!!!MXMR~!MMMRMM8MMMMMS!!M?XXMMMMM$$M$M$RMMMM8$RMMMMMMMM%X!!"      << endl;
	cout << "  ~     ~  !~~X!!XHMMM?~ XM$MMMMRMMMMMM@MMMMMMMMMM$8@MMMMMMMMRMMMMM?!MMM%HX!"      << endl;
	cout << "           !!!!XSMMXXMM .MMMMMMMM$$$BB8MMM@MMMMMMMR$RMMMMMMMMMMMMMMMXX!?H!XX"      << endl;
	cout << "           XHXMMMMMMMM!.XMMMMMMMMMR$$$8M$$$$$M@88MMMMMMMMMMMMMMM!XMMMXX!!!XM"      << endl;
	cout << "      ~   <!MMMMMMMMRM:XMMMMMMMMMM8R$$$$$$$$$$$$$$$NMMMMMMMM?!MM!M8MXX!!/t!M"      << endl;
	cout << "      '   ~HMMMMMMMMM~!MM8@8MMM!MM$$8$$$$$$$$$$$$$$8MMMMMMM!!XMMMM$8MR!MX!MM"      << endl;
	cout << "          'MMMMMMMMMM'MM$$$$$MMXMXM$$$$$$$$$$$$$$$$RMMMMMMM!!MMM$$$$MMMMM<!M"      << endl;
	cout << "          'MMMMMMMMM!'MM$$$$$RMMMMMM$$$$$$$$$$$$$$$MMM!MMMX!!MM$$$$$M$$M$M!M"      << endl;
	cout << "           !MMMMMM$M! !MR$$$RMM8$8MXM8$$$$$$$$$$$$NMMM!MMM!!!?MRR$$RXM$$MR!M"      << endl;
	cout << "           !M?XMM$$M.< !MMMMMMSUSRMXM$8R$$$$$$$$$$#$MM!MMM!X!t8$M$MMMHMRMMX$"      << endl;
	cout << "    ,-,   '!!!MM$RMSMX:.?!XMHRR$RM88$$$8M$$$$$R$$$$8MM!MMXMH!M$$RMMMMRNMMX!$"      << endl;
	cout << "   -'`    '!!!MMMMMMMMMM8$RMM8MBMRRMR8RMMM$$$$8$8$$$MMXMMMMM!MR$MM!M?MMMMMM$"      << endl;
	cout << "          'XX!MMMMMMM@RMM$MM@$$BM$$$M8MMMMR$$$$@$$$$MM!MMMMXX$MRM!XH!!??XMMM"      << endl;
	cout << "          `!!!M?MHMMM$RMMMR@$$$$MR@MMMM8MMMM$$$$$$$WMM!MMMM!M$RMM!!.MM!%M?~!"      << endl;
	cout << "           !!!!!!MMMMBMM$$RRMMMR8MMMMMRMMMMM8$$$$$$$MM?MMMM!f#RM~    `~!!!~!"      << endl;
	cout << "           ~!!HX!!~!?MM?MMM??MM?MMMMMMMMMRMMMM$$$$$MMM!MMMM!!"                     << endl;
	cout << "           '!!!MX!:`~~`~~!~~!!!!XM!!!?!?MMMM8$$$$$MMMMXMMM!!"                      << endl;
	cout << "            !!~M@MX.. <!!X!!!!XHMHX!!``!XMMMB$MM$$B$M!MMM!!"                       << endl;
	cout << "            !!!?MRMM!:!XHMHMMMMMMMM!  X!SMMX$$MM$$$RMXMMM~"                        << endl;
	cout << "             !M!MMMM>!XMMMMMMMMXMM!!:!MM$MMMBRM$$$$8MMMM~"                         << endl;
	cout << "             `?H!M$R>'MMMM?MMM!MM6!X!XM$$$MM$MM$$$$MX$f"                           << endl;
	cout << " _______      `MXM$8X MMMMMMM!!MM!!!!XM$$$MM$MM$$$RX@\""                           << endl;
	cout << "/       \\_/|   ~M?$MM !MMMMXM!!MM!!!XMMM$$$8$XM$$RM!`"                            << endl;
	cout << "|Go AGS! _/    !XMMM !MMMMXX!XM!!!HMMMM$$$$RH$$M!~"                                << endl;
	cout << "\\_______/       'M?MM `?MMXMM!XM!XMMMMM$$$$$RM$$#"                                << endl;
	cout << "                 `>MMk ~MMHM!XM!XMMM$$$$$$BRM$M\""                                 << endl;
	cout << "                  ~`?M. !M?MXM!X$$@M$$$$$$RMM#"                                    << endl;
	cout << "                    `!M  !!MM!X8$$$RM$$$$MM#`"                                     << endl;
	cout << "                      !% `~~~X8$$$$8M$$RR#`"                                       << endl;
	cout << "                       !!x:xH$$$$$$$R$R*`"                                         << endl;
	cout << "                        ~!?MMMMRRRM@M#`       -Sushil-"                            << endl;
	cout << "                         `~???MMM?M\"`"                                            << endl;
	cout << "                             ``~~"                                                 << endl;
}

