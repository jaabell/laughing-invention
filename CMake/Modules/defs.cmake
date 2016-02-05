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
set(DEBUG_MODE "NO_DEBUG" CACHE STRING
  "Will RealESSI be configured for Debug? [NO_DEBUG|DEBUG]")
set(DEBUG_OPTS "NO_DEBUG;DEBUG" CACHE INTERNAL
  "List of possible values for the Debug Mode cache variable")
# Possible options for debugging
set_property(CACHE DEBUG_MODE PROPERTY STRINGS ${DEBUG_OPTS})

# Set the programming mode for the compilation
set(PROGRAMMING_MODE "SEQUENTIAL" CACHE STRING
  "What programmig mode will RealESSI be configured for? [SEQUENTIAL|PARALLEL]")
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
    message(WARNING "Debug mode is not set, defaulting to NO_DEBUG")
    set(DEBUG_MODE "NO_DEBUG" CACHE STRING
        "Will RealESSI be configured for Debug? [NO_DEBUG|DEBUG]" FORCE)
endif()

#Check if there is no potential error with the programming mode
list(FIND PROGRAMMING_OPTS "${PROGRAMMING_MODE}" L_HAS_PROG)
if(L_HAS_PROG EQUAL -1)
    message(WARNING "Programming mode is not set, defaulting to SEQUENTIAL")
    set(PROGRAMMING_MODE "SEQUENTIAL" CACHE STRING
         "What programmig mode will RealESSI be configured for? [SEQUENTIAL|PARALLEL]" FORCE)
endif()

# Set compiling and linking options
## Common flags for goth debug and no debug
set(COMPILER_FLAGS "-Wall")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -g")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_LINUX")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -D_UNIX")
set(COMPILER_FLAGS "${COMPILER_FLAGS} ${PROGRAMMING_FLAG}")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -DBrzi_nDarray_val")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -std=c++11")
set(COMPILER_FLAGS "${COMPILER_FLAGS} -Wno-write-strings")
# set(COMPILER_FLAGS "${COMPILER_FLAGS} -fdiagnostics-color=auto ")
if(${DEBUG_MODE} EQUAL "NO_DEBUG")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${LTO_FLAGS}")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -O2")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${INLINING_OPTS}")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -funroll-loops")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -static")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -static-libgcc")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -static-libstdc++")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -march=native")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -ftree-vectorize")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -ftree-slp-vectorize")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -ffast-math")
    set(COMPILER_FLAGS "${COMPILER_FLAGS} -fvariable-expansion-in-unroller")
    
    set(LINKER_FLAGS "-O2")
    set(LINKER_FLAGS "${LINKER_FLAGS} ${LTO_FLAGS}")
    # set(LINKER_FLAGS "${LINKER_FLAGS} ${MORE_LINKFLAGS} ")
else()
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
