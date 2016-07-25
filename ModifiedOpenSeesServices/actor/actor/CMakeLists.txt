# Builds all the actor/actor module
# message("scanning a_actor module...")

set(remove_list "")
if(NOT IS_PARALLEL)
    set(remove_list ${remove_list} ${CMAKE_CURRENT_SOURCE_DIR}/Actor.cpp)
endif()

ADD_SOURCES(MOSS_SRC ON remove_list)

