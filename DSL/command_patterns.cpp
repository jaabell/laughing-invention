patterns->push_back("add acceleration field # <.> ax = <accel> ay = <accel> az = <aaccel> ;");
patterns->push_back("add constraint equal dof with master node # <.> and slave node # <.> dof to constrain <.>;");
patterns->push_back("add constraint equal dof with node # <.> dof <.> master and node # <.> dof <.> slave;");
patterns->push_back("add damping # <.> to element # <.>;");
patterns->push_back("add damping # <.> to node # <.>;");
patterns->push_back("add damping # <.> type Caughey3rd with a0 = <time> a1 = <1/time> a2 = <.> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;");
patterns->push_back("add damping # <.> type Caughey4th with a0 = <time> a1 = <1/time> a2 = <.> a3 = <.> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;");
patterns->push_back("add damping # <.> type Rayleigh with a0 = <1/time> a1 = <time> stiffness_to_use = <Initial_Stiffness|Current_Stiffness|Last_Committed_Stiffness>;");
patterns->push_back("add domain reduction method loading # <.> hdf5_file = <string> scale_factor = <.>;");
patterns->push_back("add domain reduction method loading # <.> hdf5_file = <string>;");
patterns->push_back("add element # <.> type 20NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 20NodeBrick with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 20NodeBrick_elastic with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 20NodeBrick_upU with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> and porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;");
patterns->push_back("add element # <.> type 20NodeBrickLT with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 27NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 27NodeBrick with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 27NodeBrick_elastic with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 27NodeBrickLT with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 3NodeShell_ANDES with nodes (<.>, <.>, <.>) use material # <.> thickness = <l> ;");
patterns->push_back("add element # <.> type 4NodeShell_ANDES with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <l> ;");
patterns->push_back("add element # <.> type 4NodeShell_MITC4 with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <L>;");
patterns->push_back("add element # <.> type 4NodeShell_NewMITC4 with nodes (<.>, <.>, <.>, <.>) use material # <.> thickness = <L>;");
patterns->push_back("add element # <.> type 8NodeBrick using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 8NodeBrick with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 8NodeBrick_elastic with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type 8NodeBrick_up with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;");
patterns->push_back("add element # <.> type 8NodeBrick_upU with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.> porosity = <.> alpha = <.>  rho_s = <M/L^3>  rho_f = <M/L^3> k_x = <L^3T/M>  k_y = <L^3T/M>  k_z = <L^3T/M>  K_s = <stress> K_f = <stress>;");
patterns->push_back("add element # <.> type 8NodeBrickLT with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type beam_9dof_elastic with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );");
patterns->push_back("add element # <.> type beam_displacement_based with nodes (<.>, <.>) with # <.> integration_points use section # <.> mass_density = <M/L^3> IntegrationRule = \"\" xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );");
patterns->push_back("add element # <.> type beam_elastic with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );");
patterns->push_back("add element # <.> type beam_elastic_lumped_mass with nodes (<.>, <.>) cross_section = <area> elastic_modulus = <F/L^2> shear_modulus = <F/L^2> torsion_Jx = <length^4> bending_Iy = <length^4> bending_Iz = <length^4> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );");
patterns->push_back("add element # <.> type BeamColumnDispFiber3d with nodes (<.>, <.>) number_of_integration_points = <.> section_number = <.> mass_density = <M/L^3>  xz_plane_vector = (<.>, <.>, <.> ) joint_1_offset = (<L>, <L>, <L> ) joint_2_offset = (<L>, <L>, <L> );");
patterns->push_back("add element # <.> type EightNodeBrickLTNoOutput with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add element # <.> type HardContact with nodes (<.>, <.>) normal_stiffness = <F/L> tangential_stiffness = <F/L> normal_damping = <F/L> tangential_damping = <F/L>  friction_ratio = <.>  contact_plane_vector = (<.>, <.>, <.> );");
patterns->push_back("add element # <.> type ShearBeamLT with nodes (<.>, <.>) cross_section = <l^2> use material # <.>;");
patterns->push_back("add element # <.> type SoftContact with nodes (<.>, <.>) initial_normal_stiffness = <F/L> stiffning_rate = <m^-1> tangential_stiffness = <F/L> normal_damping = <F/L> tangential_damping = <F/L>  friction_ratio = <.>  contact_plane_vector = (<.>, <.>, <.> );");
patterns->push_back("add element # <.> type truss with nodes (<.>, <.>) use material # <.> cross_section = <length^2> mass_density = <M/L^3> ;");
patterns->push_back("add element # <.> type variable_node_brick_8_to_27 using <.> Gauss points each direction with nodes (<.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>, <.>) use material # <.>;");
patterns->push_back("add fiber # <.> using material # <.> to section # <.>  fiber_cross_section = <area> fiber_location = (<L>,<L>);");
patterns->push_back("add imposed motion # <.> to node # <.> dof DOFTYPE displacement_scale_unit = <displacement> displacement_file = \"filename\" velocity_scale_unit = <velocity> velocity_file = \"filename\" acceleration_scale_unit = <acceleration> acceleration_file = \"filename\";");
patterns->push_back("add imposed motion # <.> to node # <.> dof DOFTYPE time_step = <t> displacement_scale_unit = <length> displacement_file = \"filename\" velocity_scale_unit = <velocity> velocity_file = \"filename\" acceleration_scale_unit = <acceleration> acceleration_file = \"filename\";");
patterns->push_back("add load # <.> to all elements type self_weight use acceleration field # <.>;");
patterns->push_back("add load # <.> to element # <.> type self_weight use acceleration field # <.>;");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>) with magnitude <.>;");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>) with magnitudes (<.> , <.> , <.> , <.>);");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>) with magnitude <.>;");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>) with magnitudes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>);");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>) with magnitude <.>;");
patterns->push_back("add load # <.> to element # <.> type surface at nodes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>) with magnitudes (<.> , <.> , <.> , <.>, <.>, <.>, <.>, <.>, <.>);");
patterns->push_back("add load # <.> to node # <.> type from_reactions;");
patterns->push_back("add load # <.> to node # <.> type linear FORCETYPE = <force or moment>; //FORCETYPE = Fx Fy Fz Mx My Mz F_fluid_x F_fluid_y F_fluid_z");
patterns->push_back("add load # <.> to node # <.> type path_series FORCETYPE = <force or moment> time_step = <time> series_file = \"STRING\";");
patterns->push_back("add load # <.> to node # <.> type path_time_series FORCETYPE = <force or moment> series_file = \"STRING\";");
patterns->push_back("add load # <.> to node # <.> type self_weight use acceleration field # <.>;");
patterns->push_back("add mass to node # <.> mx = <mass> my = <mass> mz = <mass> Imx = <mass*length^2> Imy = <mass*length^2> Imz = <mass*length^2>;");
patterns->push_back("add mass to node # <.> mx = <mass> my = <mass> mz = <mass>;");
patterns->push_back("add material # <.> type DruckerPragerArmstrongFrederickLT mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> armstrong_frederick_ha = <F/L^2> armstrong_frederick_cr = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = exp;");
patterns->push_back("add material # <.> type DruckerPragerLT mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = exp;");
patterns->push_back("add material # <.> type DruckerPragerVonMisesLT mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> druckerprager_k = <> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> initial_confining_stress = exp;");
patterns->push_back("add material # <.> type linear_elastic_crossanisotropic mass_density = <mass_density> elastic_modulus_horizontal = <F/L^2> elastic_modulus_vertical = <F/L^2> poisson_ratio_h_v = <.> poisson_ratio_h_h = <.> shear_modulus_h_v = <F/L^2>;");
patterns->push_back("add material # <.> type linear_elastic_isotropic_3d mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.>;");
patterns->push_back("add material # <.> type linear_elastic_isotropic_3d_LT mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.>;");
patterns->push_back("add material # <.> type NonlinearIsotropic3DLT mass_density = <M/L^3>  Niso3d_K = <F/L^2> Niso3d_Kur = <F/L^2> Niso3d_n = <.> Niso3d_c = <.> Niso3d_phi0 = <.> Niso3d_dphi = <.> Niso3d_Rf = <.> Niso3d_K0 = <.> Niso3d_Kb = <.> Niso3d_m = <.> Niso3d_pa = <F/L^2> Niso3d_K2 = <.> Niso3d_B = <.> Niso3d_Et = <F/L^2> Niso3d_Ei = <F/L^2> Niso3d_Er = <F/L^2>");
patterns->push_back("add material # <.> type sanisand2004 mass_density = <M/L^3> e0 = <.> sanisand2004_G0 = <.> poisson_ratio = <.> sanisand2004_Pat = <stress>  sanisand2004_p_cut = <.>  sanisand2004_Mc = <.>  sanisand2004_c = <.> sanisand2004_lambda_c = <.> sanisand2004_xi = <.>  sanisand2004_ec_ref = <.>  sanisand2004_m = <.>  sanisand2004_h0 = <.> sanisand2004_ch = <.>  sanisand2004_nb = <.> sanisand2004_A0 = <.> sanisand2004_nd = <.> sanisand2004_z_max = <.>  sanisand2004_cz = <.> initial_confining_stress = <stress>  algorithm = <explicit|implicit>  number_of_subincrements = <.>  maximum_number_of_iterations = <.>  tolerance_1 = <.>  tolerance_2 = <.>;");
patterns->push_back("add material # <.> type sanisand2008 mass_density = <M/L^3>  e0 = <.>  sanisand2008_G0 = <.>  sanisand2008_K0 = <.> sanisand2008_Pat = <stress> sanisand2008_k_c = <.>  sanisand2008_alpha_cc = <.> sanisand2008_c = <.>  sanisand2008_xi = <.>  sanisand2008_lambda = <.>  sanisand2008_ec_ref = <.>  sanisand2008_m = <.>  sanisand2008_h0 = <.>  sanisand2008_ch = <.>  sanisand2008_nb = <.>  sanisand2008_A0 = <.> sanisand2008_nd = <.>  sanisand2008_p_r = <.>  sanisand2008_rho_c = <.>  sanisand2008_theta_c = <.>  sanisand2008_X = <.>  sanisand2008_z_max = <.>  sanisand2008_cz = <.>  sanisand2008_p0 = <stress>  sanisand2008_p_in = <.>  algorithm = <explicit|implicit>  number_of_subincrements = <.>  maximum_number_of_iterations = <.>  tolerance_1 = <.>  tolerance_2 = <.>;");
patterns->push_back("add material # <.> type uniaxial_concrete02 compressive_strength = <F/L^2> strain_at_compressive_strength = <.> crushing_strength = <F/L^2>  strain_at_crushing_strength = <.> lambda = <.> tensile_strength = <F/L^2> tension_softening_stiffness = <F/L^2>;");
patterns->push_back("add material # <.> type uniaxial_elastic elastic_modulus = <F/L^2> viscoelastic_modulus = <mass / length / time> ;");
patterns->push_back("add material # <.> type uniaxial_steel01 yield_strength = <F/L^2> elastic_modulus = <F/L^2> strain_hardening_ratio = <.>  a1 = <.>  a2 = <.>  a3 = <>  a4 = <.> ;");
patterns->push_back("add material # <.> type uniaxial_steel02 yield_strength = <F/L^2> elastic_modulus = <F/L^2> strain_hardening_ratio = <.> R0 = <.> cR1 = <.> cR2 = <.>  a1 = <.>  a2 = <.>  a3 = <>  a4 = <.> ;");
patterns->push_back("add material # <.> type VonMisesLT mass_density = <M/L^3> elastic_modulus = <F/L^2> poisson_ratio = <.> von_mises_radius = <F/L^2> kinematic_hardening_rate = <F/L^2> isotropic_hardening_rate = <F/L^2> ;");
patterns->push_back("add node # <.> at (<length>,<length>,<length>)  with <.> dofs;");
patterns->push_back("add section # <.> type elastic3d elastic_modulus = <F/L^2> cross_section = <L^2> bending_Iz = <L^4> bending_Iy=<L^4> torsion_Jx=<L^4> ;");
patterns->push_back("add section # <.> type Elastic_Membrane_Plate elastic_modulus = <F/L^2> poisson_ratio = <.> thickness = <length> mass_density = <M/L^3>;");
patterns->push_back("add section # <.> type FiberSection TorsionConstant_GJ = <F*L^2>");
patterns->push_back("add section # <.> type Membrane_Plate_Fiber thickness = <length> use material # <.>;");
patterns->push_back("add single point constraint to node # <.> dof to constrain <dof_type> constraint value of <corresponding unit>;");
patterns->push_back("add uniform acceleration # <.> to all nodes dof <.> time_step = <T> scale_factor = <.> initial_velocity = <L/S> acceleration_file = <string>;");
patterns->push_back("check element # <.>;");
patterns->push_back("check mesh filename;");
patterns->push_back("compute reaction forces;");
patterns->push_back("define algorithm With_no_convergence_check / Newton / Modified_Newton;");
patterns->push_back("define convergence test Norm_Displacement_Increment / Energy_Increment / Norm_Unbalance tolerance = <.> maximum_iterations = <.> verbose_level = <0>|<1>|<2>;");
patterns->push_back("define dynamic integrator Hilber_Hughes_Taylor with alpha = <.>;");
patterns->push_back("define dynamic integrator Newmark with gamma = <.> beta = <.>;");
patterns->push_back("define load factor increment <.>;");
patterns->push_back("define NDMaterialLT constitutive integration algorithm Forward_Euler|Multistep_Forward_Euler|Modified_Euler_Error_Control|Runge_Kutta_45_Error_Control|Backward_Euler yield_function_relative_tolerance  = <.> stress_relative_tolerance = <.> maximum_iterations = <.>;");
patterns->push_back("define solver ProfileSPD / UMFPack;");
patterns->push_back("define static integrator displacement_control using node # <.> dof DOFTYPE increment <length>;");
patterns->push_back("disable element # <.> output;");
patterns->push_back("disable element output;");
patterns->push_back("disable output;");
patterns->push_back("enable element output;");
patterns->push_back("enable output;");
patterns->push_back("fix node # <.> dofs <.>;");
patterns->push_back("fix node # <.> dofs all;");
patterns->push_back("free node # <.> dofs <.>;");
patterns->push_back("help;");
patterns->push_back("if (.) { } else {};");
patterns->push_back("if (.) { };");
patterns->push_back("model name \"name_string\";");
patterns->push_back("new loading stage \"name_string\";");
patterns->push_back("output every <.> steps;");
patterns->push_back("print <.> in <.>;");
patterns->push_back("print <.>;");
patterns->push_back("print domain;");
patterns->push_back("print element # <.>;");
patterns->push_back("print material # <.>;");
patterns->push_back("print node # <.>;");
patterns->push_back("remove constraint equaldof node # <.>;");
patterns->push_back("remove displacement from  node # <.>;");
patterns->push_back("remove element # <.>;");
patterns->push_back("remove imposed motion # <.>;");
patterns->push_back("remove load # <.>;");
patterns->push_back("remove node # <.>;");
patterns->push_back("remove strain from element # <.>;");
patterns->push_back("runTest;");
patterns->push_back("save <.> non_converged_substeps;");
patterns->push_back("set output compression level to <.>;");
patterns->push_back("simulate <.> steps using static algorithm;");
patterns->push_back("simulate <.> steps using transient algorithm time_step = <time>;");
patterns->push_back("simulate <.> steps using variable transient algorithm time_step = <time> minimum_time_step = <time> maximum_time_step = <time> number_of_iterations = <.>;");
patterns->push_back("simulate constitutive testing constant mean pressure triaxial strain control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;");
patterns->push_back("simulate constitutive testing drained triaxial strain control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;");
patterns->push_back("simulate constitutive testing undrained simple shear use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;");
patterns->push_back("simulate constitutive testing undrained triaxial stress control use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;");
patterns->push_back("simulate constitutive testing undrained triaxial use material # <.> strain_increment_size = <.> maximum_strain = <.> number_of_times_reaching_maximum_strain = <.>;");
patterns->push_back("simulate using eigen algorithm number_of_modes = <.>;");
patterns->push_back("ux uy uz Ux Uy Uz rx ry rz;");
patterns->push_back("while (.) { };");
patterns->push_back("whos;");
patterns->push_back("");
