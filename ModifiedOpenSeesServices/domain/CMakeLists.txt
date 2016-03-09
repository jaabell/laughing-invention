# Builds all the domain module
# message("scanning domain module...")

ADD_SUBDIR(domain)
ADD_SUBDIR(component)
ADD_SUBDIR(load)
ADD_SUBDIR(node)
ADD_SUBDIR(constraints)
ADD_SUBDIR(pattern)
ADD_SUBDIR(groundMotion)
ADD_SUBDIR(damping)
#ADD_SUBDIR(loadBalancer)
if(IS_PARALLEL)
    ADD_SUBDIR(subdomain)
    ADD_SUBDIR(partitioner)
endif()

ADD_SOURCES(MOSS_SRC ON)

