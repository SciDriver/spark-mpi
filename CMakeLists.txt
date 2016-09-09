cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW) # CMake 3.0
endif()

if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

PROJECT (spark-mpi)

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

include_directories("include")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/include/)

find_package(Boost REQUIRED COMPONENTS system thread)

# Find CUSP and Thrust
find_package(CUDA REQUIRED)
mark_as_advanced(CUDA_BUILD_EMULATION)
mark_as_advanced(CUDA_SDK_ROOT_DIR)
mark_as_advanced(CUDA_SEPARABLE_COMPILATION)
mark_as_advanced(CUDA_VERBOSE_BUILD)
mark_as_advanced(CUDA_BUILD_CUBIN)
mark_as_advanced(CUDA_HOST_COMPILER)

find_package(THRUST REQUIRED)
mark_as_advanced(THRUST_INCLUDE_DIR)

set(GLOG_ROOT_DIR "$ENV{GLOG_HOME}")
find_package(Glog REQUIRED)

if(BUILD_DOCS)
  find_package(Doxygen)
endif()

# find_package(Caffe)

include(InstallRequiredSystemLibraries)

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

include_directories(${Boost_INCLUDE_DIRS})
LIST(APPEND LIBRARY_LIST ${Boost_LIBRARIES})

message("Boost: ${Boost_INCLUDE_DIRS} , ${Boost_LIBRARIES}")

FIND_PACKAGE(MPI REQUIRED)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_COMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x ${MPI_COMPILE_FLAGS}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Necessary due to CUDA compatibility problems
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  # The CUDA module thinks cc is gcc and passes dumpspecs.
  # This is a workaround
  if(CUDA_HOST_COMPILER STREQUAL "/usr/bin/cc")
    SET(CUDA_HOST_COMPILER "/usr/bin/clang")
  endif()
endif()

# Default to sm_20 and sm_35
if(NOT CUDA_NVCC_FLAGS)
  SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_20,code=compute_20 -gencode=arch=compute_35,code=compute_35 -DBOOST_DISABLE_ASSERTS")
endif()

if(TIMINGS)
  ADD_DEFINITIONS(-DTIMINGS)
  SET(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -DTIMINGS")
endif(TIMINGS)

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "release")
endif()

LIST(APPEND INCLUDE_LIST ${CUDA_INCLUDE_DIRS})
LIST(APPEND LIBRARY_LIST ${CUDA_CUDART_LIBRARY} ${CUDA_curand_LIBRARY} ${CUDA_CUBLAS_LIBRARIES})

LIST(APPEND INCLUDE_LIST ${MPI_INCLUDE_PATH})
LIST(APPEND LIBRARY_LIST ${MPI_LIBRARIES})

LIST(APPEND INCLUDE_LIST ${MPI_INCLUDE_PATH})
LIST(APPEND INCLUDE_LIST ${CUSP_INCLUDE_DIR})
LIST(APPEND INCLUDE_LIST ${CUB_INCLUDE_DIR})

LIST(APPEND INCLUDE_LIST ${GLOG_INCLUDE_DIR})
LIST(APPEND LIBRARY_LIST ${GLOG_LIBRARIES})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(examples)

option ( ENABLE_PYTHON_BINDINGS "Enable Python bindings via Swig" ON)
IF(ENABLE_PYTHON_BINDINGS)
  set(ANACONDA2_ROOT_DIR "$ENV{ANACONDA2_HOME}")
  ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYTHON_BINDINGS)

# target etags/tags
IF (UNIX OR APPLE)
  ADD_CUSTOM_TARGET(tags etags --members --declarations  `find .. -name *.cc -or -name *.hh -or -name *.cpp -or -name *.h -or -name *.c -or -name *.f -or -name *.cu`)
  ADD_CUSTOM_TARGET(etags DEPENDS tags)
ENDIF (UNIX OR APPLE)



