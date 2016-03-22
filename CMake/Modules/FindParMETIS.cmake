  #
  # Find the ParMETIS includes and libraries
  #
  # ParMETIS is an MPI-based parallel library that implements a variety of algorithms for 
  # partitioning unstructured graphs, meshes, and for computing fill-reducing orderings of 
  # sparse matrices. It can be found at:
  #   http://www-users.cs.umn.edu/~karypis/metis/ParMETIS/index.html
  #
  # ParMETIS_INCLUDE_DIRS - where to find autopack.h
  # ParMETIS_LIBRARIES   - List of fully qualified libraries to link against.
  # ParMETIS_FOUND       - Do not attempt to use if "no" or undefined.
  
  FIND_PATH(ParMETIS_INCLUDE_DIRS ParMETIS.h
    /usr/local/include
    /usr/include
    $(ParMETIS)
  )
  
  FIND_LIBRARY(ParMETIS_LIBRARY ParMETIS
    /usr/local/lib
    /usr/lib
    $(ParMETIS_DIR)
  )
  
  FIND_LIBRARY(METIS_LIBRARY metis
    /usr/local/lib
    /usr/lib
    $(METIS_DIR)
  )
  
  IF(ParMETIS_INCLUDE_DIRS)
    IF(ParMETIS_LIBRARY)
      SET( ParMETIS_LIBRARIES ${ParMETIS_LIBRARY} ${METIS_LIBRARY})
      SET( ParMETIS_FOUND "YES" )
    ENDIF(ParMETIS_LIBRARY)
  ENDIF(ParMETIS_INCLUDE_DIRS)