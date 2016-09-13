# Builds all the actor/channel module
# message("scanning channel module...")

set(remove_list "")
if(NOT IS_PARALLEL)
    set(remove_list ${remove_list} ${CMAKE_CURRENT_SOURCE_DIR}/MPI_Channel.cpp)
endif()

ADD_SOURCES(MOSS_SRC ON remove_list)

