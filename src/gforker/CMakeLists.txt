
set (CMAKE_C_FLAGS "-D USE_SIGNAL")
set (CMAKE_CXX_FLAGS "")

LIST(APPEND INCLUDE_LIST "${MPI_SRC}/pm/util")

# message("include list: ${INCLUDE_LIST}")
# message("library list: ${LIBRARY_LIST}")

INCLUDE_DIRECTORIES(${INCLUDE_LIST})

set (pmiutil_src
    "$ENV{MPI_SRC}/pm/util/cmnargs.c"
    "$ENV{MPI_SRC}/pm/util/env.c"
    "$ENV{MPI_SRC}/pm/util/ioloop.c"
    "$ENV{MPI_SRC}/pm/util/labelout.c"
    "$ENV{MPI_SRC}/pm/util/process.c"
    "$ENV{MPI_SRC}/pm/util//pmiserv.c"
    "$ENV{MPI_SRC}/pm/util/pmiport.c"
    "$ENV{MPI_SRC}/pm/util/rm.c"
    ./mpiexec_usage.c
    )

ADD_LIBRARY(pmiutil SHARED ${pmiutil_src})
TARGET_LINK_LIBRARIES(pmiutil ${LIBRARY_LIST})
INSTALL(TARGETS pmiutil LIBRARY DESTINATION lib)

ADD_EXECUTABLE(pmiserv pmiserv.cpp)
TARGET_LINK_LIBRARIES(pmiserv pmiutil)
INSTALL(TARGETS pmiserv RUNTIME DESTINATION bin)






