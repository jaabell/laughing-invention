#cmake -G "Unix Makefiles" -DBaseName:STRING=mybase -DCMAKE_CXX_COMPILER=/usr/bin/clang++
# -DCMAKE_C_COMPILER=/usr/bin/clang -DEXTRA_INCLUDE=<path>
# For parallel:  cmake -DCMAKE_CXX_COMPILER=/usr/bin/mpic++ -DPROGRAMMING_MODE=PARALLEL ..
cmake_minimum_required(VERSION 2.8.9)
project (RealESSI)
#Generates a Release version
set(CMAKE_BUILD_TYPE Release)
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
set(RealESSI_TestFile "${CMAKE_BINARY_DIR}/CMakeToTest.txt")

set(INCLUDE_LIBS "")        #List of the libraries in the CMakeInclude.txt file
#List of the path of the libraries in the CMakeInclude.txt file
set(INCLUDE_LIBS_PATH "")
set(INCLUDE_PATHS "")   # Will contain the path to all the .h files
# Will contain the value that will be added to the compiling flags
set(PROGRAMMING_FLAG "")
set(COMPILER_FLAGS "") # Will contain compiling flags
set(LINKER_FLAGS "")        # Will contain the flags for the linker
set(EXT_INCLUDE "")          # List of external
set(ESSI_LIBS "")                   # List of internal libs to be included
set(NUMERIC_LIBS "")        # List of external libs to be included
set(NUMERIC_LIBS_PATH "")        # List of external libs to be included
set(MACHINE_LIBS "")        # List of standard libs to be included

set(NO_BLASLAPACK FALSE CACHE BOOL
    "TRUE if BLAS and LAPACK are provided some other means. (eg. directly in the compiler wrapper)")
# Set to "TRUE" if BLAS and LAPACK are provided some other way (ie. on NERSC the CC compiler
# wrapper automatically links with these)
message(STATUS "NO_BLASLAPACK=${NO_BLASLAPACK}")

set(IS_PARALLEL OFF)
set(PROGRAMMING_FLAG  "")
set(INCLUDE_DEFAULTS OFF)
set(COMP_ERRS OFF)
set(COMP_ERRS_LOG "")
set(DO_TESTS ON)

# Load modules
include(${CMAKE_SOURCE_DIR}/CMake/Modules/essi.cmake)
# Add the modules required by the make process
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules")
# define the GUI variables and basic checks done, also sets the compilation flags
include(${CMAKE_SOURCE_DIR}/CMake/Modules/defs.cmake)
#message(STATUS "INCLUDE_LIBS='${INCLUDE_LIBS}'")
#message(STATUS "INCLUDE_LIBS_PATH='${INCLUDE_LIBS_PATH}'")
#message(STATUS "INCLUDE_DEFAULTS='${INCLUDE_DEFAULTS}'")

# Populate the common standard libraries
SYSTEM_LIBS("dl" MACHINE_LIBS)
SYSTEM_LIBS("z" MACHINE_LIBS)
SYSTEM_LIBS("m" MACHINE_LIBS)
#SYSTEM_LIBS("gfortran" MACHINE_LIBS)
set(MACHINE_LIBS ${MACHINE_LIBS} "gfortran")
set(MACHINE_LIBS ${MACHINE_LIBS} "m")
set(MACHINE_LIBS ${MACHINE_LIBS} "supc++")

# Checks that the prerequisites are available
include(${CMAKE_SOURCE_DIR}/CMake/Modules/reqs.cmake)
# Include external headers
if(EXTRA_INCLUDE)
	 LIST(APPEND EXT_INCLUDE "${EXTRA_INCLUDE}")
endif()
LIST(APPEND EXT_INCLUDE "${RealESSI_DEP}/include")
LIST(REMOVE_DUPLICATES EXT_INCLUDE)
#message(STATUS "num libs-: ${NUMERIC_LIBS}")
#message(STATUS "num libs path-: ${NUMERIC_LIBS_PATH}")
#message(STATUS "includes-: ${EXT_INCLUDE}")

INCLUDE_DIRS(EXT_INCLUDE)
# generate the cmake external libraries to later link
IMPORT_LIBS(NUMERIC_LIBS_PATH NUMERIC_LIBS)
#list(APPEND NUMERIC_LIBS "hdf5")
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
# Link standar libraries
LINK_LIBS(${EXECUTABLE} MACHINE_LIBS)

install(TARGETS ${EXECUTABLE} RUNTIME DESTINATION "${RealESSI_SRC}/bin")

if(DO_TESTS)
    include(${CMAKE_SOURCE_DIR}/CMake/Modules/tests.cmake)
endif()

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
