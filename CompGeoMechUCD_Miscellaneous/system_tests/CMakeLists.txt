# Builds all the CompGeoMechUCD_Miscellaneous/system_tests module

set(D_LIB system_tests)

file(GLOB SOURCES "*.cpp")

add_library(${D_LIB} ${LIB_MOD} ${SOURCES})
set(ESSI_LIBS "${ESSI_LIBS} ${D_LIB}" PARENT_SCOPE)

