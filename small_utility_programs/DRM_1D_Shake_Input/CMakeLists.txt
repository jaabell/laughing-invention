# Builds all the small_utility_programs/DRM_1D_Shake_Input module

set(D_LIB DRM_1D_Shake_Input)

file(GLOB SOURCES "*.cpp")

add_library(${D_LIB} ${LIB_MOD} ${SOURCES})
set(ESSI_LIBS "${ESSI_LIBS} ${D_LIB}" PARENT_SCOPE)


