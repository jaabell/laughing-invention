# Builds all the graph module
# message("scanning graph module...")

ADD_SUBDIR(graph)
ADD_SUBDIR(numberer)
if(IS_PARALLEL)
    ADD_SUBDIR(partitioner)
endif()

ADD_SOURCES(MOSS_SRC ON)

