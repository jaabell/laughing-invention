# Builds all the CompGeoMechUCD_FiniteElements module
# message("scanning CompGeoMechUCD_FiniteElements module...")

#################### Brick Elements ##########################

ADD_SUBDIR(solid_brick_8_nodes_upU)
ADD_SUBDIR(solid_brick_20_nodes_upU)
ADD_SUBDIR(solid_brick_27_nodes_upU)
ADD_SUBDIR(solid_brick_variable_nodes_upU)
set(ESSI_LIBS ${ESSI_LIBS} PARENT_SCOPE)

