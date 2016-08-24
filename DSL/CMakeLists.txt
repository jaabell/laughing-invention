# Builds all the DSL module
# message("scanning analysis module...")

ADD_SOURCES(EXEC_SRC ON)

execute_process(COMMAND make --silent --quiet
                 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/DSL
               )
