# Builds all the FiniteDeformation module
ADD_SUBDIR(fdYield)
ADD_SUBDIR(fdFlow)
ADD_SUBDIR(fdEvolution)


set(D_LIB FiniteDeformation)

file(GLOB SOURCES "*.cpp")

add_library(${D_LIB} ${LIB_MOD} ${SOURCES})
set(ESSI_LIBS "${ESSI_LIBS} ${D_LIB}" PARENT_SCOPE)

