# Builds all the CompGeoMechUCD_FiniteElements module
# message("scanning CompGeoMechUCD_FiniteElements module...")

################### Contact Element ########################## 

ADD_SUBDIR(Contact_Elements)

#################### Brick Elements ##########################

#---# Uncoupled Elements 

    ADD_SUBDIR(Solid_Bricks)

#---# Coupled Elements (up)

	ADD_SUBDIR(Solid_Bricks_up)

# ---# Coupled Elements (upU)

	ADD_SUBDIR(Solid_Bricks_upU)

# ---# Non_Linear Shear Element

	ADD_SUBDIR(nonlinear_shear_beam)

#################### Structural Elements ######################

	ADD_SUBDIR(rank_one_deficient_elastic_pinned_fixed_beam)
	ADD_SUBDIR(elastic_beam)
	ADD_SUBDIR(shell_andes)

# ----- Old Elements ------ #
ADD_SUBDIR(coupled_u-p_brick_8_nodes)
ADD_SUBDIR(solid_brick_variable_nodes_8_to_27)


# ADD_SUBDIR(Large_Deformation_Elements)
set(ESSI_LIBS ${ESSI_LIBS} PARENT_SCOPE)
# message(STATUS "ESSI_LIBS=${ESSI_LIBS}")  # 
