
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Numpy REQUIRED)

include_directories(${NUMPY_INCLUDE_DIRS})
include_directories(${MPI_INCLUDE_PATH})
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INCLUDE_LIST})

execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print sysconfig.get_python_lib(plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}')"
  OUTPUT_VARIABLE _PYTHON_INSTDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# SET(PYTHON_INSTDIR ${_PYTHON_INSTDIR} CACHE PATH "Installation directory for python module.")

SET(PYTHON_INSTDIR "${ANACONDA2_ROOT_DIR}/lib/python2.7/site-packages")

message("${PYTHON_INSTDIR}")
message("${CMAKE_INSTALL_PREFIX}")

FILE(GLOB sharpspark_headers "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.hpp")

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sparkmpi_wrap.cpp COMMAND ${SWIG_EXECUTABLE} -python -c++ -outcurrentdir ${SWIG_EXTRA_FLAGS} -I${CUDA_INCLUDE_DIRS} -I${CMAKE_CURRENT_SOURCE_DIR}/../include/ -o ${CMAKE_CURRENT_BINARY_DIR}/sparkmpi_wrap.cpp ${CMAKE_CURRENT_SOURCE_DIR}/sparkmpi.i DEPENDS ${sparkmpi_headers} ${CMAKE_CURRENT_SOURCE_DIR}/sparkmpi.i)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${PYTHON_NUMPY_INCLUDE_DIR})

ADD_LIBRARY(_sparkmpi SHARED ${CMAKE_CURRENT_BINARY_DIR}/sparkmpi_wrap.cpp python_sparkmpi.cpp)

TARGET_LINK_LIBRARIES(_sparkmpi spark-mpi ${PYTHON_LIBRARIES})

set_target_properties(
  _sparkmpi
  PROPERTIES SOVERSION 1
  VERSION 1
  PREFIX ""
  SUFFIX ".so"
  INSTALL_NAME_DIR ${PYTHON_INSTDIR}
  )

INSTALL(TARGETS _sparkmpi
  RUNTIME DESTINATION ${PYTHON_INSTDIR}
  LIBRARY DESTINATION ${PYTHON_INSTDIR}
  ARCHIVE DESTINATION ${PYTHON_INSTDIR}
  )

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/sparkmpi.py ${CMAKE_CURRENT_SOURCE_DIR}/sparkmpi/main.py ${CMAKE_CURRENT_SOURCE_DIR}/sparkmpi/__init__.py DESTINATION ${PYTHON_INSTDIR}/sparkmpi)
