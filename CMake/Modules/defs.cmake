# Definition of all the variables that are modifiable by the user when running cmake
# - Builds the GUI
# - Sets the flags for compilation

# --- Global definitions  ---
# Contained in Makefile.compilers
# Possible compilers: [GNU (GCC)|CLANG|INTEL]
message(STATUS "Compiler='${CMAKE_CXX_COMPILER_ID}'")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(IS_CLANG ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
   set(IS_GNU ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
   set(IS_INTEL ON)
endif()

# Build UI for cmake-gui and ccmake
# Set the debug mode for the compilation
set(DEBUG_MODE "OPTIMIZED" CACHE STRING
  "Will RealESSI be configured for Debug? [OPTIMIZED|DEBUG]")
set(DEBUG_OPTS "OPTIMIZED;DEBUG" CACHE INTERNAL
  "List of possible values for the Debug Mode cache variable")
# Possible options for debugging
set_property(CACHE DEBUG_MODE PROPERTY STRINGS ${DEBUG_OPTS})

# Set the programming mode for the compilation
set(PROGRAMMING_MODE "SEQUENTIAL" CACHE STRING
  "What programming mode will RealESSI be configured for? [SEQUENTIAL|PARALLEL]")
set(PROGRAMMING_OPTS "SEQUENTIAL;PARALLEL" CACHE INTERNAL
  "List of possible values for the Debug Mode cache variable")
# Possible options for programming
set_property(CACHE PROGRAMMING_MODE PROPERTY STRINGS ${PROGRAMMING_OPTS})

#  -flto    --------------------------------------------------------
set(LTO_FLAGS "" CACHE STRING "Flags for LTO") # TODO: Improve message
 
set(INLINING_OPTS "-finline -finline-functions -fkeep-inline-functions -fdefault-inline " CACHE STRING
  "Inlining options ") # TODO: how does these work?

set(LOGFILE  "essi_compilation.log" CACHE STRING "Log file for the compilation process")
# --------------------------------------------------------


# Check if any compiler is selected
if((NOT IS_CLANG) AND (NOT IS_GNU) AND (NOT IS_INTEL))
    message(FATAL_ERROR "Compiler is not set.")
endif()

#Check if there is no potential error with the debug mode
list(FIND DEBUG_OPTS "${DEBUG_MODE}" L_HAS_DEBUG) 
if(L_HAS_DEBUG EQUAL -1)
    message(WARNING "Debug mode is not set, defaulting to OPTIMIZED")
    set(DEBUG_MODE "OPTIMIZED" CACHE STRING
        "Will RealESSI be configured for Debug? [OPTIMIZED|DEBUG]" FORCE)
endif()

#Check if there is no potential error with the programming mode
list(FIND PROGRAMMING_OPTS "${PROGRAMMING_MODE}" L_HAS_PROG)
if(L_HAS_PROG EQUAL -1)
    message(WARNING "Programming mode is not set, defaulting to SEQUENTIAL")
    set(PROGRAMMING_MODE "SEQUENTIAL" CACHE STRING
         "What programmig mode will RealESSI be configured for? [SEQUENTIAL|PARALLEL]" FORCE)
endif()

# Set compiling and linking options


message(STATUS "Programming mode='${PROGRAMMING_MODE}'")
if(${PROGRAMMING_MODE} STREQUAL "PARALLEL")
    message(STATUS "YES! Parallel")
    set(IS_PARALLEL ON)
    set(PROGRAMMING_FLAG  "-D_PDD ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -D_PARALLEL_PROCESSING ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -D_TIMING ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -D_PETSC ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -D_BDEBUG ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -DPETSC_USE_BOPT_O ")
    set( PROGRAMMING_FLAG "${PROGRAMMING_FLAG} -DPETSC_USE_EXTERN_CXX ") 
    
    list(APPEND EXT_INCLUDE "${RealESSI_DEP}/petsc-3.6.0/include")
    list(APPEND EXT_INCLUDE "${RealESSI_DEP}/petsc-3.6.0/arch-linux2-c-opt/include")
    list(APPEND EXT_INCLUDE "${RealESSI_DEP}/include")
    # list(APPEND EXT_INCLUDE "${RealESSI_DEP}/parmetis-4.0.2_install/include")
    # list(APPEND EXT_INCLUDE "${RealESSI_DEP}/metis-4.0.2_install/include")
    list(APPEND EXT_INCLUDE "${RealESSI_DEP}/hdf5_sequential/include")
    # set( EXT_INCLUDE "${EXT_INCLUDE} ${RealESSI_DEP}/hdf5_parallel/include")

    list(APPEND NUMERIC_LIBS "petsc")
    list(APPEND NUMERIC_LIBS "superlu_4.3")
    list(APPEND NUMERIC_LIBS "superlu_dist_4.0")
    list(APPEND NUMERIC_LIBS "spai")
    list(APPEND NUMERIC_LIBS "scalapack")
    list(APPEND NUMERIC_LIBS "flapack")
    list(APPEND NUMERIC_LIBS "fblas")
    list(APPEND NUMERIC_LIBS "parmetis")
    list(APPEND NUMERIC_LIBS "metis")
    
    list(APPEND NUMERIC_LIBS "lapack")
    list(APPEND NUMERIC_LIBS "arpack")
    list(APPEND NUMERIC_LIBS "umfpack")
    list(APPEND NUMERIC_LIBS "amd")
    list(APPEND NUMERIC_LIBS "cholmod")
    list(APPEND NUMERIC_LIBS "colamd")
    list(APPEND NUMERIC_LIBS "camd")
    list(APPEND NUMERIC_LIBS "ccolamd")
    list(APPEND NUMERIC_LIBS "cblas")
    list(APPEND NUMERIC_LIBS "f77blas")
    list(APPEND NUMERIC_LIBS "atlas")
    list(APPEND NUMERIC_LIBS "suitesparseconfig")
    
    # SYSTEM_LIBS("quadmath" MACHINE_LIBS)
    # SYSTEM_LIBS("gcc_s" MACHINE_LIBS)
    SYSTEM_LIBS("ssl" MACHINE_LIBS)
    SYSTEM_LIBS("crypto" MACHINE_LIBS)
    SYSTEM_LIBS("pthread" MACHINE_LIBS)
    SYSTEM_LIBS("ieee" MACHINE_LIBS)
    SYSTEM_LIBS("c" MACHINE_LIBS)
    SYSTEM_LIBS("rt" MACHINE_LIBS)

else()  # sequential
    message(STATUS "YES! SEQUENTIAL")
    list(APPEND EXT_INCLUDE "${RealESSI_DEP}/hdf5_sequential/include")
    
    list(APPEND NUMERIC_LIBS "lapack")
    list(APPEND NUMERIC_LIBS "arpack")
    list(APPEND NUMERIC_LIBS "umfpack")
    list(APPEND NUMERIC_LIBS "amd")
    list(APPEND NUMERIC_LIBS "cholmod")
    list(APPEND NUMERIC_LIBS "colamd")
    list(APPEND NUMERIC_LIBS "camd")
    list(APPEND NUMERIC_LIBS "ccolamd")
    list(APPEND NUMERIC_LIBS "metis")
    list(APPEND NUMERIC_LIBS "cblas")
    list(APPEND NUMERIC_LIBS "f77blas")
    list(APPEND NUMERIC_LIBS "atlas")
    list(APPEND NUMERIC_LIBS "suitesparseconfig")
    
    
    SYSTEM_LIBS("ieee" MACHINE_LIBS)
    SYSTEM_LIBS("c" MACHINE_LIBS)
    SYSTEM_LIBS("pthread" MACHINE_LIBS)
    SYSTEM_LIBS("rt" MACHINE_LIBS)
endif()


FIND_PACKAGE(HDF5)

if(HDF5_FOUND)
    message(STATUS HDF5_LIBRARY_DIRS)
    # IMPORT_LIB()
else()
    IMPORT_LIB("${RealESSI_DEP}/hdf5_sequential/lib" hdf5)
endif()




## Common flags for goth debug and no debug
set(COMPILER_FLAGS "-Wall")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_LINUX")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_UNIX")
set(COMPILER_FLAGS "${COMPILER_FLAGS} ${PROGRAMMING_FLAG}")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -DBrzi_nDarray_val")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -std=c++11")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -Wno-write-strings")
# set(COMPILER_FLAGS "${COMPILER_FLAGS} -fdiagnostics-color=auto ")
if(${DEBUG_MODE} STREQUAL "OPTIMIZED")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${LTO_FLAGS}")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -O3")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${INLINING_OPTS}")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -funroll-loops")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -march=native")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -ftree-vectorize")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -ftree-slp-vectorize")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -fvariable-expansion-in-unroller")
    
    set(LINKER_FLAGS "-O3")
    set(LINKER_FLAGS "${LINKER_FLAGS} ${LTO_FLAGS}")
    # set(LINKER_FLAGS "${LINKER_FLAGS} ${MORE_LINKFLAGS} ")
else()
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -g")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -pg")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_G3DEBUG")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_DEBUG_CONTACT")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_TEACHING_MODE")
    
    set(LINKER_FLAGS "-g")
    set(LINKER_FLAGS "${LINKER_FLAGS} -pg")
    # set(LINKER_FLAGS "${LINKER_FLAGS} ${MORE_LINKFLAGS} ")
endif()
# message("Compiler flags: ${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}" )
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_AR "gcc-ar")
# Set the location where all the libraries will be created
set(LIBRARY_OUTPUT_PATH ${ESSI_LIB})

if(COMP_ERRS) 
    FAIL() 
endif()
