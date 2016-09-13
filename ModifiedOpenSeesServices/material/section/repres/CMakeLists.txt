# Builds all the material/repres module
# message("scanning repres module...")

ADD_SUBDIR(cell)
ADD_SUBDIR(patch)
ADD_SUBDIR(reinfBar)
ADD_SUBDIR(reinfLayer)
ADD_SUBDIR(section)

ADD_SOURCES(MOSS_SRC ON)

