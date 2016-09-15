# Builds all the CompGeoMechUCD_FiniteElements module
# message("scanning CompGeoMechUCD_FiniteElements module...")

################### Contact Element ########################## 

ADD_SUBDIR(Contact_Elements)

#################### Brick Elements ##########################

#---# Uncoupled Elements 

	ADD_SUBDIR(solid_brick_8_nodes)
	ADD_SUBDIR(solid_brick_8_nodes_variable_gauss_points)

	ADD_SUBDIR(solid_brick_20_nodes)
	ADD_SUBDIR(solid_brick_20_nodes_variable_gauss_points)

	ADD_SUBDIR(solid_brick_27_nodes)
	ADD_SUBDIR(solid_brick_27_nodes_variable_gauss_points)

	ADD_SUBDIR(solid_brick_variable_nodes_8_to_27)

# #---# Coupled Elements (up)

	# ADD_SUBDIR(coupled_up_bricks)

# ---# Coupled Elements (upU)

	ADD_SUBDIR(coupled_upU_bricks)

# ---# Non_Linear Shear Element

	ADD_SUBDIR(nonlinear_shear_beam)


ADD_SUBDIR(coupled_u-p-U_brick_8_nodes)
ADD_SUBDIR(coupled_u-p_brick_8_nodes)
ADD_SUBDIR(coupled_u-p-U_brick_20_nodes)

#################### Structural Elements ######################

	ADD_SUBDIR(rank_one_deficient_elastic_pinned_fixed_beam)
	ADD_SUBDIR(elastic_beam)
	ADD_SUBDIR(shell_andes)

# ADD_SUBDIR(coupled_u-p_large_deformation_brick_8_nodes)
# ADD_SUBDIR(solid_brick_large_deformation_total_Lagrangian_8_nodes)
# ADD_SUBDIR(solid_brick_large_deformation_total_Lagrangian_20_nodes)
set(ESSI_LIBS ${ESSI_LIBS} PARENT_SCOPE)
# message(STATUS "ESSI_LIBS=${ESSI_LIBS}")  # 
