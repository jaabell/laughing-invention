# Builds all the analysis module
# message("scanning analysis module...")

ADD_SUBDIR(analysis)
ADD_SUBDIR(integrator)
ADD_SUBDIR(model)
ADD_SUBDIR(numberer)
ADD_SUBDIR(handler)
ADD_SUBDIR(algorithm)
ADD_SUBDIR(dof_grp)
ADD_SUBDIR(fe_ele)

ADD_SOURCES(MOSS_SRC ON)

