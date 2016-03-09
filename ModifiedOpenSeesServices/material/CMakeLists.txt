# Builds all the material module
# message("scanning material module...")

ADD_SUBDIR(uniaxial)
# ADD_SUBDIR(nD)
ADD_SUBDIR(section)
# ADD_SUBDIR(yieldSurface)

ADD_SOURCES(MOSS_SRC ON)

