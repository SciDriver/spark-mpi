set (CMAKE_C_FLAGS "")
set (CMAKE_CXX_FLAGS "")

SET(OMPI_SRC "$ENV{MPI_SRC}")

# ORTE: Open Run-Time Environment
# OPAL: Open Portable Access Layer
# MCA: Modular Component Architecture
# ODLS: 

LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/orte/tools/orterun")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/opal/include")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/ompi/include")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/oshmem/include")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/opal/mca/hwloc/hwloc2a/hwloc/include/private/autogen")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/opal/mca/hwloc/hwloc2a/hwloc/include/hwloc/autogen")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/orte/include")
LIST(APPEND INCLUDE_LIST "${OMPI_SRC}/opal/mca/hwloc/hwloc2a/hwloc/include")

LIST(APPEND LIBRARY_LIST ${MPI_SRC_LIBRARIES})
LIST(APPEND LIBRARY_LIST "-lrt -lm -lutil  -lz  -levent -levent_pthreads")

INCLUDE_DIRECTORIES(${INCLUDE_LIST})

# set (orterun_objs
#  "${OMPI_SRC}/orte/tools/orterun/main.c"
#  "orterun.c"
# )

# ADD_EXECUTABLE(pmixsrv ${orterun_objs})
# TARGET_LINK_LIBRARIES(pmixsrv ${LIBRARY_LIST})
# INSTALL(TARGETS pmixsrv RUNTIME DESTINATION bin)

# Spark-MPI plugin

set (mca_odls_sparkmpi_objs
  "./orte/mca/odls/sparkmpi/odls_sparkmpi_module.c"
  "./orte/mca/odls/sparkmpi/odls_sparkmpi_component.c"  
)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
ADD_LIBRARY(mca_odls_sparkmpi SHARED ${mca_odls_sparkmpi_objs})
INSTALL(TARGETS mca_odls_sparkmpi LIBRARY DESTINATION lib)


