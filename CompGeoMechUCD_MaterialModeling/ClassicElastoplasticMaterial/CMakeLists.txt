# Builds all the CompGeoMechUCD_MaterialModeling/ClassicElastoplasticMaterial module
# message("scanning ClassicElastoplasticMaterial module...")

set(CEPM_SRC "")
ADD_SUBDIR(EvolvingVariables)
ADD_SUBDIR(ElasticityModels)
ADD_SUBDIR(ConstitutiveModels)
# ADD_SUBDIR(PlasticFlowDirections)   # Only contains headers
# ADD_SUBDIR(YieldFunctions)          # Only contains headers

ADD_SOURCES(CEPM_SRC OFF)

BUILD_LIB("classicelastoplasticmaterials" ESSI_LIBS CEPM_SRC)

# list(APPEND EXECUTABLE test_nonAssociate_DP_AF)