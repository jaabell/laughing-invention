# Builds all the FiniteDeformation/fdFlow module

set(D_LIB fdFlow)

file(GLOB SOURCES "*.cpp")

add_library(${D_LIB} ${LIB_MOD} ${SOURCES})
set(ESSI_LIBS "${ESSI_LIBS} ${D_LIB}" PARENT_SCOPE)

