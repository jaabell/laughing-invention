# Builds all the UCB_Elements module
# message("scanning UCB_Elements module...")

set(UCBE_SRC "")
ADD_SUBDIR(truss)
ADD_SUBDIR(zeroLength)
ADD_SUBDIR(beamWithHinges)
ADD_SUBDIR(shell)
ADD_SUBDIR(joint)
ADD_SUBDIR(dispBeamColumn)
ADD_SUBDIR(elasticBeamLumpedMass)
ADD_SUBDIR(integration)
ADD_SUBDIR(transformation)

BUILD_LIB("ucbelements" ESSI_LIBS UCBE_SRC)

