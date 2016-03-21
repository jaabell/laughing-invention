string(TOLOWER ${PROGRAMMING_MODE} p_mode)

#FIND_PACKAGE(LAPACK)
# FIND_PACKAGE(BLAS)
FIND_PACKAGE(HDF5)

message(STATUS "Checking external dependencies")
if(NOT EXISTS "${RealESSI_DEP}")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../RealESSI_Dependencies.tgz")
        LOG_ERR("RealESSI dependencies missing!! ...\nPlease request the RealESSI_Dependencies.tgz file from developers and place it in ${CMAKE_SOURCE_DIR}/..")
        FAIL() 
    else()
        # unzip
        message(STATUS "Extracting dependencies...  This will take a while! Grab a coffee...")
        execute_process(
            COMMAND cmake -E tar xvf RealESSI_Dependencies.tgz
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
        execute_process(
            COMMAND ./compile_libraries_${p_mode}.sh
            WORKING_DIRECTORY ${RealESSI_DEP})
    endif()
endif()

#Check that all the include folders exist
message(STATUS "Checking external includes")
foreach(inc_dir ${EXT_INCLUDE})
    # message("Including ${inc_dir}")
    if(NOT EXISTS ${inc_dir})
        LOG_ERR("Missing include folder ${inc_dir} please verify")
    endif()
    #include_directories(${inc_dir})
endforeach()

# Checking that all the .a libraries exist in RealESSI Dependencies
message(STATUS "Checking external libraries")
set(NO_LIBS OFF)
foreach(dep_lib ${NUMERIC_LIBS})
    if(NOT EXISTS "${NUMERIC_LIBS_DIR}/lib${dep_lib}.a")
        set(NO_LIBS ON)
        LOG_ERR("lib${dep_lib}.a not found")
    endif()
endforeach()
if(NO_LIBS)
    LOG_ERR("One or multiple libraries were not found on ${NUMERIC_LIBS_DIR}\nPlease re-run manually the script compile_libraries_${p_mode}.sh at ${RealESSI_DEP}")
endif()
if(COMP_ERRS) 
    FAIL() 
endif()
