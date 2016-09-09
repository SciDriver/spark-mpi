find_path(THRUST_INCLUDE_DIR 
    HINTS ${CMAKE_SOURCE_DIR}/include /usr/include/cuda /usr/local/include /usr/local/cuda/include
    NAMES thrust/version.h 
    DOC "Thrust headers" 
) 
if( THRUST_INCLUDE_DIR ) 
    list( REMOVE_DUPLICATES THRUST_INCLUDE_DIR ) 
    include_directories( ${THRUST_INCLUDE_DIR} ) 
endif( THRUST_INCLUDE_DIR ) 

# Check for required components 
include( FindPackageHandleStandardArgs ) 
find_package_handle_standard_args( THRUST
    REQUIRED_VARS 
        THRUST_INCLUDE_DIR 
) 

set( THRUST_FOUND TRUE ) 

