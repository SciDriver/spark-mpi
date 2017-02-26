cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW) # CMake 3.0
endif()

if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

PROJECT (spark-mpi)

# set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
# set (CMAKE_INSTALL_PREFIX "/opt/spark-mpi")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MPI-oriented extension of the Spark platform")
SET(CPACK_PACKAGE_VENDOR "Brookhaven National Laboratory")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
# SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake
${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

IF(WIN32 AND NOT UNIX)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/spark-mpi")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "spark-mpi" "spark-mpi")
INCLUDE(CPack)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
 ENDIF("${isSystemDir}" STREQUAL "-1")

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "release")
endif()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories("include")
include(InstallRequiredSystemLibraries)

######################################################
# Boost
######################################################

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(${Boost_INCLUDE_DIRS})
LIST(APPEND LIBRARY_LIST ${Boost_LIBRARIES})

######################################################
# MPI
######################################################

FIND_PACKAGE(MPI REQUIRED)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_COMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x ${MPI_COMPILE_FLAGS}")

LIST(APPEND INCLUDE_LIST ${MPI_INCLUDE_PATH})
LIST(APPEND LIBRARY_LIST ${MPI_LIBRARIES})

######################################################
# MPI SRC
######################################################

set(MPI_SRC "$ENV{MPI_SRC}")

LIST(APPEND INCLUDE_LIST "${MPI_SRC}/include/")
LIST(APPEND INCLUDE_LIST "${MPI_SRC}/mpl/include")
LIST(APPEND INCLUDE_LIST "${MPI_SRC}/pmi/simple")

######################################################
# target etags/tags
######################################################

IF (UNIX OR APPLE)
  ADD_CUSTOM_TARGET(tags etags --members --declarations  `find .. -name *.cc -or -name *.hh -or -name *.cpp -or -name *.h -or -name *.c -or -name *.f -or -name *.cu`)
  ADD_CUSTOM_TARGET(etags DEPENDS tags)
ENDIF (UNIX OR APPLE)

######################################################
# 
######################################################

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)



