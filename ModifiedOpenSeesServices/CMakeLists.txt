# Builds all the Modified Open Sees Services module
# message("scanning MOSS module...")

set(MOSS_SRC "")
ADD_SUBDIR(matrix)
ADD_SUBDIR(system_of_eqn)
ADD_SUBDIR(tagged)
ADD_SUBDIR(material)
ADD_SUBDIR(analysis)
ADD_SUBDIR(graph)
ADD_SUBDIR(convergenceTest)
ADD_SUBDIR(domain)
ADD_SUBDIR(handler)
ADD_SUBDIR(actor)
ADD_SUBDIR(element)
ADD_SUBDIR(utility)

# list(LENGTH MOSS_SRC SRC_SIZE)
# message(STATUS "MOSS_SRC(${SRC_SIZE})")  # =${MOSS_SRC}

BUILD_LIB("MOSS" ESSI_LIBS MOSS_SRC)

