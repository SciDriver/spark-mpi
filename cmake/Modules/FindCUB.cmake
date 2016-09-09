# 
# Try to find CUB  library  
# (see https://github.com/arrayfire/arrayfire)
# Once run this will define: 
# 
# CUB_FOUND
# CUB_INCLUDE_DIR 
#
# Filipe RNC Maia 06/2015
# --------------------------------

find_path(CUB_INCLUDE_DIR  
  cub/cub.cuh
  HINTS /usr/include
  /usr/local/include
  $ENV{C_INCLUDE_PATH}
  $ENV{CPLUS_INCLUDE_PATH}
  ${CUB_DIR}
  ${CUB_INCLUDE_DIR}
  /usr/local/cuda/include
  ${CMAKE_SOURCE_DIR}/include
  DOC "CUB headers"
 )



IF (CUB_INCLUDE_DIR)
    SET(CUB_FOUND TRUE)
ELSE (CUB_INCLUDE_DIR)
    MESSAGE("CUB include dir not found. Set CUB_DIR to find it.")
ENDIF(CUB_INCLUDE_DIR)

MARK_AS_ADVANCED(
  CUB_INCLUDE_DIR
)

include( FindPackageHandleStandardArgs ) 
find_package_handle_standard_args( CUB 
    REQUIRED_VARS 
        CUB_INCLUDE_DIR 
) 

