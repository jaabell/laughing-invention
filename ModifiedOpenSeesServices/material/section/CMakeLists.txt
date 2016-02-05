# Builds all the material/section module
# message("scanning section module...")

ADD_SUBDIR(repres)
ADD_SUBDIR(fiber)
ADD_SUBDIR(matrixutil)
# ADD_SUBDIR(yieldSurface)

ADD_SOURCES(MOSS_SRC ON)

