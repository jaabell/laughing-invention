///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell & Boris Jeremic
// DATE:               Jan 2013
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





// Node related
//--------------------------------------------------------------------------------------------------
#include "MODELING/add_node.h"
#include "MODELING/add_mass_to_node.h"
#include "MODELING/add_single_point_constraint_to_node.h"
#include "MODELING/add_support_to_node.h"
#include "MODELING/add_support_to_all_dofs_of_node.h"
#include "MODELING/add_equaldof_to_two_nodes.h"
#include "MODELING/add_equaldof_to_two_nodes_two_dofs.h"


// Loading related
//--------------------------------------------------------------------------------------------------
#include "MODELING/add_acceleration_field.h"
#include "MODELING/add_load.h"
#include "MODELING/add_load_to_brick_surface.h"
#include "MODELING/add_load_selfweight_to_element.h"
#include "MODELING/add_load_selfweight_to_node.h"
#include "MODELING/add_load_to_brick_surface_from_contact_pore_pressure.h"
// #include "MODELING/add_load_pattern_domain_reduction_method_old_inputfiles_format.h"
// #include "MODELING/add_load_pattern_domain_reduction_method.h"
#include "MODELING/add_load_pattern_domain_reduction_method_hdf5.h"
// #include "MODELING/add_load_pattern_domain_reduction_method_save_forces.h"
// #include "MODELING/add_load_pattern_domain_reduction_method_restore_forces.h"
// #include "MODELING/add_plastic_bowl_method_load_pattern.h"
#include "MODELING/add_force_time_history_linear.h"
#include "MODELING/add_force_time_history_path_series.h"
#include "MODELING/add_force_time_history_path_time_series.h"
// #include "MODELING/add_penalty_displacement_time_history_linear.h"
// #include "MODELING/add_penalty_displacement_time_history_path_series.h"
#include "MODELING/add_imposed_motion.h"
#include "MODELING/add_time_history_of_ground_motion_for_uniform_excitation.h"
#include "MODELING/add_load_to_node_from_reaction.h"


// Elements
//--------------------------------------------------------------------------------------------------

// #################################################
// Coupled Elements   [Sumeet July, 2016]
// #################################################

#include "MODELING/add_element_hard_contact.h"
#include "MODELING/add_element_soft_contact.h"

// #include "MODELING/add_element_contact_3dof_to_3dof.h"
// #include "MODELING/add_element_contact_nonlinear_3dof_to_7dof.h"
// #include "MODELING/add_element_contact_nonlinear_3dof_to_3dof.h"

//####################################################
// General Brick Elements  [Sumeet September, 2016]
//####################################################

// Eight Node Brick
#include "MODELING/add_element_brick_8node.h"
#include "MODELING/add_element_brick_8node_variable_number_of_gauss_points.h"
// Twenty Node Brick
#include "MODELING/add_element_brick_20node.h"
#include "MODELING/add_element_brick_20node_variable_number_of_gauss_points.h"
// Twenty Seven Node Brick
#include "MODELING/add_element_brick_27node.h"
#include "MODELING/add_element_brick_27node_variable_number_of_gauss_points.h"
// Variable Node Brick
#include "MODELING/add_element_brick_variable_node.h"
#include "MODELING/add_element_brick_variable_node_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_variable_node_8_to_27.h"

// ####################################################################
// Coupled upU Elements   [Sumeet September, 2016]
// ####################################################################

#include "MODELING/add_element_brick_8node_upU.h"
#include "MODELING/add_element_brick_8node_upU_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_20node_upU.h"
#include "MODELING/add_element_brick_20node_upU_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_27node_upU.h"
#include "MODELING/add_element_brick_27node_upU_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_variable_node_upU.h"
#include "MODELING/add_element_brick_variable_node_upU_variable_number_of_gauss_points.h"


// ####################################################################
// Coupled up Elements   [Sumeet September, 2016]
// ####################################################################

#include "MODELING/add_element_brick_8node_up.h"
#include "MODELING/add_element_brick_8node_up_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_20node_up.h"
#include "MODELING/add_element_brick_20node_up_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_27node_up.h"
#include "MODELING/add_element_brick_27node_up_variable_number_of_gauss_points.h"

#include "MODELING/add_element_brick_variable_node_up.h"
#include "MODELING/add_element_brick_variable_node_up_variable_number_of_gauss_points.h"

// ####################################################################
// Structural Elements    [Sumeet September, 2016]
// ####################################################################

#include "MODELING/add_element_shear_beam.h"
#include "MODELING/add_element_beam_elastic_lumped_mass.h"
#include "MODELING/add_element_beam_elastic.h"  // Nima Tafazzoli (Dec. 2012)
#include "MODELING/add_element_rank_one_deficient_elastic_pinned_fixed_beam.h"  // Nima Tafazzoli (Dec. 2012)
#include "MODELING/add_element_truss.h"
#include "MODELING/add_element_beam_displacement_based.h"
// #include "MODELING/add_element_penalty.h"
// #include "MODELING/add_element_penalty_for_applying_generalized_displacement.h"
#include "MODELING/add_element_shell_MITC4.h"     // Jose Added July 3 2012
#include "MODELING/add_element_shell_NewMITC4.h"     // Jose Added July 12 2012
#include "MODELING/add_element_shell_andes_3node.h"     // Jose Added July 22 2012
#include "MODELING/add_element_shell_andes_4node.h"     // Jose Added Sept 20 2012

#include "MODELING/add_element_DispBeamColumn3d.h" // Yuan (Feb.2016)


// Materials
//--------------------------------------------------------------------------------------------------
// NDarray based
#include "MODELING/add_constitutive_model_NDMaterial_camclay.h"
#include "MODELING/add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d.h"
#include "MODELING/add_constitutive_model_NDMaterial_vonmises_perfectly_plastic.h"
#include "MODELING/add_constitutive_model_NDMaterial_vonmises_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_vonmises_isotropic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic.h"
#include "MODELING/add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_sanisand_2004.h"
#include "MODELING/add_constitutive_model_NDMaterial_sanisand_2008.h"
#include "MODELING/add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_linear_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_druckerprager_perfectly_plastic.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_druckerprager_isotropic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterial_accelerated_camclay.h"
#include "MODELING/add_constitutive_model_NDMaterial_pisano.h"

// LTensor Based
#include "MODELING/add_constitutive_model_NDMaterialLT_vonmises_perfectly_plastic.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_pisano.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_New_Pisano.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_linear_elastic_isotropic_3d.h"
#include "MODELING/add_constitutive_model_NDMaterial_nonlinear_elastic_isotropic_3d.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_vonmises.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_vonMises_ArmstrongFrederick.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerprager.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerpragervonmises.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_le.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerprager_armstrong_frederick_ne.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerprager_non_associate_linear_hardening.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_druckerprager_non_associate_armstrong_frederick.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_camclay.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_roundedMohrCoulomb.h"
#include "MODELING/add_constitutive_model_NDMaterialLT_sanisand_2004.h"

// Uniaxial
#include "MODELING/add_constitutive_model_uniaxial_elastic.h"
#include "MODELING/add_constitutive_model_uniaxial_steel01.h"
#include "MODELING/add_constitutive_model_uniaxial_steel02.h"
#include "MODELING/add_constitutive_model_uniaxial_concrete02.h"


// Damping
//--------------------------------------------------------------------------------------------------
#include "MODELING/add_damping_rayleigh.h"
#include "MODELING/add_damping_caughey3rd.h"
#include "MODELING/add_damping_caughey4th.h"
#include "MODELING/add_damping_to_element.h"
#include "MODELING/add_damping_to_node.h"

// Remove commands
//--------------------------------------------------------------------------------------------------
#include "MODELING/remove_element.h"
#include "MODELING/remove_load.h"
#include "MODELING/remove_node.h"
#include "MODELING/remove_support_from_node.h"
#include "MODELING/remove_equaldof_from_node.h"
#include "MODELING/remove_strain_from_element.h"
#include "MODELING/remove_displacement_from_node.h"


// Section related c_bindings
//--------------------------------------------------------------------------------------------------
#include "MODELING/add_build_section.h"
#include "MODELING/add_patch_circle_to_section.h"
#include "MODELING/add_patch_quad_to_section.h"
#include "MODELING/add_patch_rectangle_to_section.h"
#include "MODELING/add_reinforcing_bars_arc_to_section.h"
#include "MODELING/add_reinforcing_bars_circle_to_section.h"
#include "MODELING/add_reinforcing_bars_layer_to_section.h"
#include "MODELING/add_section_elastic_to_beam.h"
#include "MODELING/add_section_elastic_shear_to_beam.h"
#include "MODELING/add_section_membrane_plate_fiber_to_shell.h"     // Jose Added July 3 2012
#include "MODELING/add_section_elastic_membrane_plate_to_shell.h"   // Jose Added July 3 2012
#include "MODELING/add_single_fiber_to_section.h"
#include "MODELING/add_section_to_model.h"


//Model query
//--------------------------------------------------------------------------------------------------
#include "MODELING/get_nodal_coordinate.h"
#include "MODELING/obtain_pseudo_time.h"
#include "MODELING/query_domain.h"
#include "SIMULATION/query_simlate_flag.h"

// Output control
//--------------------------------------------------------------------------------------------------
#include "SIMULATION/enable_output.h"
#include "SIMULATION/output_every_nsteps.h"
#include "SIMULATION/set_output_compression_level.h"
#include "SIMULATION/output_non_converged_iterations.h"
#include "SIMULATION/output_support_reactions.h"
#include "MODELING/output_of_node_to_screen.h"
#include "MODELING/output_of_element_to_screen.h"


// Model Name and New Stage
// -----------------------------------------------------------------------------------------------
#include "MODELING/start_new_stage.h"
#include "MODELING/define_model_name.h"


// Algorithms
//--------------------------------------------------------------------------------------------------
#include "SIMULATION/define_NDMaterialLT_constitutive_integration_algorithm.h"
#include "SIMULATION/define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler.h"
#include "SIMULATION/define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler_Subincrement.h"
#include "SIMULATION/define_algorithm_with_no_convergence_check_for_analysis.h"
#include "SIMULATION/define_algorithm_newton_for_analysis.h"
#include "SIMULATION/define_algorithm_newtonlinesearch_for_analysis.h"
#include "SIMULATION/define_algorithm_modifiednewton_for_analysis.h"


// Convergence tests
//--------------------------------------------------------------------------------------------------
#include "SIMULATION/define_convergence_test_energyincrement_for_analysis.h"
#include "SIMULATION/define_convergence_test_normdisplacementincrement_for_analysis.h"
#include "SIMULATION/define_convergence_test_normunbalance_for_analysis.h"
#include "SIMULATION/define_convergence_test_RelativeEnergyincrement_for_analysis.h"
#include "SIMULATION/define_convergence_test_RelativeNormdisplacementincrement_for_analysis.h"
#include "SIMULATION/define_convergence_test_RelativeNormunbalance_for_analysis.h"

// Integrators
//--------------------------------------------------------------------------------------------------
#include "SIMULATION/define_static_solution_advancement_integrator_load_control.h"
#include "SIMULATION/define_static_solution_advancement_integrator_displacement_control.h"
#include "SIMULATION/define_dynamic_solution_advancement_integrator_newmark.h"
#include "SIMULATION/define_dynamic_solution_advancement_integrator_hilber_hughes_taylor.h"


// Solvers
//--------------------------------------------------------------------------------------------------
#ifdef _PARALLEL_PROCESSING
#include "SIMULATION/define_solver_petsc_for_analysis.h"
#endif
#include "SIMULATION/define_solver_parallel_for_analysis.h"
#include "SIMULATION/define_solver_umfpack_for_analysis.h"
#include "SIMULATION/define_solver_profilespd_for_analysis.h"




// Simulation
//--------------------------------------------------------------------------------------------------
#include "SIMULATION/simulate_using_static_multistep.h"
#include "SIMULATION/simulate_using_transient_multistep.h"
#include "SIMULATION/simulate_using_transient_variable_multistep.h"
#include "SIMULATION/simulate_using_eigen_analysis.h"
#include "MODELING/wipe_model.h"
#include "MODELING/check_mesh.h"
#include "SIMULATION/compute_reaction_forces.h"
// #include "./SIMULATION/analyze_static_multistep.h"
// #include "./SIMULATION/analyze_transient_multistep.h"


// To add stuff from files (Deprecated)
//--------------------------------------------------------------------------------------------------
// #include "MODELING/add_nodes_from_file.h"
// #include "MODELING/add_elements_from_file.h"
// #include "MODELING/add_elements_from_file_8node_brick.h"
// #include "MODELING/add_elements_from_file_8node_brick_elastic.h"
// #include "MODELING/add_elements_from_file_27node_brick.h"
// #include "MODELING/add_elements_from_file_27node_brick_elastic.h"
// #include "MODELING/add_support_from_file_to_nodes.h"
// #include "MODELING/add_equaldof_from_file_to_two_nodes.h"
// #include "MODELING/add_constitutive_models_from_file.h"
// #include "MODELING/add_support_to_nodes_from_GID_file.h"
// #include "MODELING/output_of_node_all_dofs_to_file_from_file.h"
// #include "MODELING/add_same_damping_from_file_to_elements.h"
// #include "MODELING/add_same_damping_from_file_to_nodes.h"
// #include "MODELING/add_elements_from_file_contact_3dof_3dof.h"


// Saving and restoring models (Database control)
//--------------------------------------------------------------------------------------------------
// #ifndef _PARALLEL_PROCESSING
// #include "MODELING/save_state_of_model_mysql_format.h"
// #include "MODELING/restore_state_of_model_mysql_format.h"
// #include "MODELING/save_response_of_model_mysql_format.h"
// #include "MODELING/restore_response_of_model_mysql_format.h"
// #endif

// Constitutive testing
//--------------------------------------------------------------------------------------------------
#include "Constitutive_Driver/simulate_constitutive_testing_for_constant_p_triaxial.h"
#include "Constitutive_Driver/simulate_constitutive_testing_for_drained_triaxial.h"
#include "Constitutive_Driver/simulate_constitutive_testing_for_undrained_triaxial_stress_control.h"
#include "Constitutive_Driver/simulate_constitutive_testing_for_undrained_simple_shear.h"
#include "Constitutive_Driver/simulate_constitutive_testing_for_undrained_triaxial.h"
#include "Constitutive_Driver/simulate_constitutive_testing_BardetLT_path.h"
#include "Constitutive_Driver/simulate_constitutive_testing_DirectStrain_path.h"


// Adding some by Babak 5/29/14:
//---------------------------------------------------------------------------------------------------
// #include "MODELING/define_constraint_handler_plain_for_analysis.h"
#include "SIMULATION/define_integrator_loadcontrol_for_static_analysis.h"
// #include "SIMULATION/define_algorithm_linear_for_analysis.h"
// #include "SIMULATION/define_algorithm_with_no_convergence_check_for_analysis.h"
#include "SIMULATION/define_parallel_numberer_plain_for_analysis.h"
// #include "SIMULATION/parallel_analyze_static_multistep.h"
// #include "MODELING/add_to_node_support.h"
// #include "MODELING/add_support_to_node.h"
// #include "MODELING/add_support_from_file_to_nodes.h"



// For running tests
#include "SIMULATION/run_test.h"
