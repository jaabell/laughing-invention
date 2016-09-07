# Builds all the CompGeoMechUCD_MaterialModeling module
# message("scanning CompGeoMechUCD_MaterialModeling module...")

ADD_SUBDIR(NewTemplate3Dep)
#ADD_SUBDIR(Template3Dep)
ADD_SUBDIR(Constitutive_Domain)
#ADD_SUBDIR(Accelerated3Dep)
ADD_SUBDIR(LTensor_Based)
ADD_SUBDIR(ClassicElastoplasticMaterial)
# ADD_SUBDIR(FiniteDeformation)

BUILD_LIB("base_3dep" ESSI_LIBS)

