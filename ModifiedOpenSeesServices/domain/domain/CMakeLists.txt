# Builds all the domain/domain module
# message("scanning d_domain module...")

ADD_SUBDIR(single)
if(IS_PARALLEL)
    ADD_SUBDIR(partitioned)
endif()

ADD_SOURCES(MOSS_SRC ON)

