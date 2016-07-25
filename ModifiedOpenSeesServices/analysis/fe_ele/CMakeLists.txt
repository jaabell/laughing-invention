# Builds all the analysis/fe_ele module
# message("scanning fe_ele module...")

ADD_SUBDIR(penalty)
ADD_SUBDIR(lagrange)
ADD_SUBDIR(transformation)

ADD_SOURCES(MOSS_SRC ON)

