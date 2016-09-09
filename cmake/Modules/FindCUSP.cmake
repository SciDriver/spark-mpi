find_path( CUSP_INCLUDE_DIR 
    HINTS ${CMAKE_SOURCE_DIR}/include /usr/include/cuda /usr/local/include 
    NAMES cusp/version.h 
    DOC "Cusp headers" 
) 
if( CUSP_INCLUDE_DIR ) 
    list( REMOVE_DUPLICATES CUSP_INCLUDE_DIR ) 
    include_directories( ${CUSP_INCLUDE_DIR} ) 
endif( CUSP_INCLUDE_DIR ) 

# Check for required components 
include( FindPackageHandleStandardArgs ) 
find_package_handle_standard_args( CUSP
    REQUIRED_VARS 
        CUSP_INCLUDE_DIR 
) 

set( CUSP_FOUND TRUE ) 

