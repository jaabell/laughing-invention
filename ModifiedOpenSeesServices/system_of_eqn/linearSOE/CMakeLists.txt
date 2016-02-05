# Builds all the system of equtions linear SOE module
# message("scanning linear SOE module...")
# --- Not considered in old build ?? TODO: remove? ---
# ADD_SUBDIR(Bjsolvers)
# ADD_SUBDIR(cg)
# ADD_SUBDIR(diagonal)
# ADD_SUBDIR(fullGEN)
# ADD_SUBDIR(itpack)
# ADD_SUBDIR(sparseGEN)
# ADD_SUBDIR(sparseSYM)
# ADD_SUBDIR(umfGEN3.1)
# ADD_SUBDIR(umfGEN_old)

if(IS_PARALLEL)
    ADD_SUBDIR(petsc)
else()
    ADD_SUBDIR(bandSPD)
    ADD_SUBDIR(bandGEN)
    ADD_SUBDIR(profileSPD)
    ADD_SUBDIR(umfGEN)
endif()

ADD_SOURCES(MOSS_SRC ON)

