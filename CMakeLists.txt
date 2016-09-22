#cmake -G "Unix Makefiles" -DBaseName:STRING=mybase -DCMAKE_CXX_COMPILER=/usr/bin/clang++ 
# -DCMAKE_C_COMPILER=/usr/bin/clang -DEXTRA_INCLUDE=<path>
# For parallel:  cmake -DCMAKE_CXX_COMPILER=/usr/bin/mpic++ -DPROGRAMMING_MODE=PARALLEL ..
# For parallel with MUMPS solver:
#               cmake -DCMAKE_CXX_COMPILER=/usr/bin/mpic++ -DPETSC_HAS_MUMPS=TRUE -DPROGRAMMING_MODE=PARALLEL ..
cmake_minimum_required(VERSION 2.8.9)
project (RealESSI)
#Generates a Release version
# set(CMAKE_BUILD_TYPE Release)

# set version number
set(RealESSI_VERSION_MAJOR 1)
set(RealESSI_VERSION_MINOR 0)

set(EXECUTABLE "essi")      # Name of the executable
set(LIB_MOD "STATIC")       # options ${LIB_MOD} | STATIC

set(FINAL_SUMMARY OFF)
set(RealESSI_SRC "${CMAKE_SOURCE_DIR}")
set(RealESSI_DEP "${CMAKE_SOURCE_DIR}/../RealESSI_Dependencies")
# set(ESSI_LIB "${RealESSI_SRC}/lib")
set(ESSI_LIB "${CMAKE_BINARY_DIR}/lib")
set(NUMERIC_LIBS_DIR "${RealESSI_DEP}/lib")

set(INCLUDE_PATHS "")       # Will contain the path to all the .h files
set(PROGRAMMING_FLAG "")    # Will contain the value that will be added to the compiling flags
set(COMPILER_FLAGS "")      # Will contain compiling flags
set(LINKER_FLAGS "")        # Will contain the flags for the linker
set(EXT_INCLUDE "")         # List of external
set(ESSI_LIBS "")           # List of internal libs to be included
set(NUMERIC_LIBS "")        # List of external libs to be included
set(MACHINE_LIBS "")        # List of standard libs to be included

set(ON_NERSC FALSE CACHE BOOL "TRUE if compiling on NERSC infrastructure. Implies NO_BLASLAPACK=True too")  # Set to "TRUE" if BLAS and LAPACK are provided some other way (ie. on NERSC the CC compiler wrapper automatically links with these)

if(ON_NERSC)
    set(NO_BLASLAPACK FALSE CACHE BOOL "TRUE if BLAS and LAPACK are provided some other means. (eg. directly in the compiler wrapper)")  # Set to "TRUE" if BLAS and LAPACK are provided some other way (ie. on NERSC the CC compiler wrapper automatically links with these)
else()
    set(NO_BLASLAPACK FALSE CACHE BOOL "TRUE if BLAS and LAPACK are provided some other means. (eg. directly in the compiler wrapper)")  # Set to "TRUE" if BLAS and LAPACK are provided some other way (ie. on NERSC the CC compiler wrapper automatically links with these)
endif()

set(PETSC_HAS_MUMPS FALSE CACHE BOOL "TRUE if PETSc library was configured to use the MUMPS solver")  # Set to "TRUE" if BLAS and LAPACK are provided some other way (ie. on NERSC the CC compiler wrapper automatically links with these)
message(STATUS "NO_BLASLAPACK=${NO_BLASLAPACK}")

set(IS_PARALLEL OFF)
set(PROGRAMMING_FLAG  "")

set(COMP_ERRS OFF)
set(COMP_ERRS_LOG "")

# Load modules
include(${CMAKE_SOURCE_DIR}/CMake/Modules/essi.cmake)
# Add the modules required by the make process
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules")
# define the GUI variables and basic checks done, also sets the compilation flags
include(${CMAKE_SOURCE_DIR}/CMake/Modules/defs.cmake)

# Populate the common standard libraries
SYSTEM_LIBS("dl" MACHINE_LIBS)
SYSTEM_LIBS("z" MACHINE_LIBS)
SYSTEM_LIBS("m" MACHINE_LIBS)

if(NOT ON_NERSC)
    set(MACHINE_LIBS ${MACHINE_LIBS} "gfortran")
endif()

set(MACHINE_LIBS ${MACHINE_LIBS} "m")
set(MACHINE_LIBS ${MACHINE_LIBS} "supc++")
# Include external headers
if(EXTRA_INCLUDE)
	 LIST(APPEND EXT_INCLUDE "${EXTRA_INCLUDE}")
endif()
list(APPEND EXT_INCLUDE "${RealESSI_DEP}/include")


# Checks that the prerequisites are available
include(${CMAKE_SOURCE_DIR}/CMake/Modules/reqs.cmake)

if(LAPACK_FOUND)
    # message(STATUS "LAPACK installed.")
    # TODO: which are the lapack equivalent in parallel
    # list(REMOVE_ITEM NUMERIC_LIBS "lapack")
    # list(REMOVE_ITEM NUMERIC_LIBS "scalapack")
    # list(REMOVE_ITEM NUMERIC_LIBS "flapack")
endif()
if(BLAS_FOUND)
    # message(STATUS "BLAS installed.")
    # TODO: which are the lapack equivalent in parallel
    # list(REMOVE_ITEM NUMERIC_LIBS "f77blas")
    # list(REMOVE_ITEM NUMERIC_LIBS "cblas")
    # list(REMOVE_ITEM NUMERIC_LIBS "fblas")
endif()
# message(STATUS "num libs: ${NUMERIC_LIBS}")

INCLUDE_DIRS(EXT_INCLUDE)
# generate the cmake external libraries to later link
IMPORT_LIBS(${NUMERIC_LIBS_DIR} NUMERIC_LIBS)
list(APPEND NUMERIC_LIBS "hdf5")
if(COMP_ERRS) 
    FAIL() 
endif()

# Generate the list of folders containing .h files
HEADER_DIRECTORIES(INCLUDE_PATHS)
include_directories(${INCLUDE_PATHS})
# message("Include dirs: ${INCLUDE_PATHS}")
# Create internal libraries
ADD_SUBDIR(CompGeoMechUCD_FiniteElements)
ADD_SUBDIR(ModifiedOpenSeesServices "MOSS")
ADD_SUBDIR(UCB_Elements)
ADD_SUBDIR(CompGeoMechUCD_MaterialModeling)
ADD_SUBDIR(nDarray)
ADD_SUBDIR(CompGeoMechUCD_Miscellaneous)
ADD_SUBDIR(small_utility_programs)

list(LENGTH ESSI_LIBS SRC_SIZE)
message(STATUS "ESSI_LIBS(${SRC_SIZE}) created...\n") # =${ESSI_LIBS} 
link_directories(${ESSI_LIB} ${NUMERIC_LIBS_DIR})

# Create executable located in DSL folder
set(EXEC_SRC "")
ADD_SUBDIR(DSL)

if(COMP_ERRS) 
    FAIL() 
endif()

message(STATUS "Ready to compile in ${PROGRAMMING_MODE} mode...")
add_executable(${EXECUTABLE} ${EXEC_SRC})
# Link internal libraries
LINK_LIBS(${EXECUTABLE} ESSI_LIBS)
# Link external libraries
LINK_LIBS(${EXECUTABLE} NUMERIC_LIBS)
# Link standard libraries
LINK_LIBS(${EXECUTABLE} MACHINE_LIBS)

install(TARGETS ${EXECUTABLE} RUNTIME DESTINATION "${RealESSI_SRC}/bin")

# set(EXECUTABLE2 "testCamClay") 
# add_executable(${EXECUTABLE2} "./CompGeoMechUCD_MaterialModeling/ClassicElastoplasticMaterial/test_CamClay.cpp")
# LINK_LIBS(${EXECUTABLE2} ESSI_LIBS)
# LINK_LIBS(${EXECUTABLE2} NUMERIC_LIBS)
# LINK_LIBS(${EXECUTABLE2} MACHINE_LIBS)
# install(TARGETS ${EXECUTABLE2} RUNTIME DESTINATION "${RealESSI_SRC}/bin")

if(FINAL_SUMMARY)
    # Prints all variables available
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()
if(COMP_ERRS) 
    FAIL() 
else()
    SUCCESS()
endif()