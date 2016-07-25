# Builds all the analysis/algorithm module
# message("scanning algorithm module...")

if(IS_PARALLEL)
    ADD_SUBDIR(domainDecompAlgo)
endif()
ADD_SUBDIR(equiSolnAlgo)
ADD_SUBDIR(eigenAlgo)

ADD_SOURCES(MOSS_SRC ON)
