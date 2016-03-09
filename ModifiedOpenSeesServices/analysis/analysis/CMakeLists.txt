# Builds all the analysis/analysis module
# message("scanning a_analysis module...")

set(remove_list "")
if(NOT IS_PARALLEL)
    set(remove_list ${remove_list} ${CMAKE_CURRENT_SOURCE_DIR}/DomainDecompositionAnalysis.cpp)
    set(remove_list ${remove_list} ${CMAKE_CURRENT_SOURCE_DIR}/StaticDomainDecompositionAnalysis.cpp)
    set(remove_list ${remove_list} ${CMAKE_CURRENT_SOURCE_DIR}/TransientDomainDecompositionAnalysis.cpp)
endif()

ADD_SOURCES(MOSS_SRC ON remove_list)

