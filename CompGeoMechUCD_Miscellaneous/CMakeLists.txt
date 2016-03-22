# Builds all the CompGeoMechUCD_Miscellaneous module
# message("scanning CompGeoMechUCD_Miscellaneous module...")

ADD_SUBDIR(units)
ADD_SUBDIR(output)
ADD_SUBDIR(timing)

# if(IS_PARALLEL)
#     ADD_SUBDIR(MachineTopology)
# endif()

# ADD_SUBDIR(system_tests)

set(ESSI_LIBS ${ESSI_LIBS} PARENT_SCOPE)
