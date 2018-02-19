

SET(OMPI_POSSIBLE_LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /usr/lib64
  /usr/local/lib64
  $ENV{LD_LIBRARY_PATH}
  $ENV{LIBRARY_PATH}
)

FIND_LIBRARY(ORTE_LIBRARY
  NAMES open-rte
  PATHS 
  ${OMPI_POSSIBLE_LIBRARY_PATH}
  )

FIND_LIBRARY(OPAL_LIBRARY
  NAMES open-pal
  PATHS 
  ${OMPI_POSSIBLE_LIBRARY_PATH}
  )

IF(ORTE_LIBRARY)
  LIST(APPEND OMPI_LIBRARIES "${ORTE_LIBRARY}")
ELSE(ORTE_LIBRARY)
  MESSAGE("open-rte library not found.")
ENDIF(ORTE_LIBRARY)

IF(OPAL_LIBRARY)
  LIST(APPEND OMPI_LIBRARIES "${OPAL_LIBRARY}")
ELSE(OPAL_LIBRARY)
  MESSAGE("open-pal library not found.")
ENDIF(OPAL_LIBRARY)

# --------------------------------

MARK_AS_ADVANCED(
  OMPI_LIBRARIES
)

include( FindPackageHandleStandardArgs ) 
find_package_handle_standard_args( OMPI 
    REQUIRED_VARS 
        OMPI_LIBRARIES 
) 
