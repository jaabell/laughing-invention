# Builds all the CompGeoMechUCD_FiniteElements module
# message("scanning CompGeoMechUCD_FiniteElements module...")

#################### Brick Elements ##########################

ADD_SUBDIR(solid_brick_8_nodes_up)
ADD_SUBDIR(solid_brick_20_nodes_up)
ADD_SUBDIR(solid_brick_27_nodes_up)
ADD_SUBDIR(solid_brick_variable_nodes_up)

set(ESSI_LIBS ${ESSI_LIBS} PARENT_SCOPE)

