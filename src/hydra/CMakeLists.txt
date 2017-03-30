
set (CMAKE_C_FLAGS "-DUSE_SIGNAL -DHYDRA_CONF_FILE=\\\"mpiexec.hydra.conf\\\" ")
set (CMAKE_CXX_FLAGS "")

SET(HYDRA_SRC "$ENV{MPI_SRC}/pm/hydra")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/ui/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/ui/mpich")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/ui/utils")
# LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/ml/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/bootstrap/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/bootstrap/external")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/bootstrap/utils")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/ckpoint")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/debugger")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/demux")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/ftb")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/topo")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/topo/hwloc/hwloc/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/topo/hwloc/hwloc/include/private")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/utils/dbg")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/pm/include")
LIST(APPEND INCLUDE_LIST "${HYDRA_SRC}/pm/pmiserv")

# message("include list: ${INCLUDE_LIST}")

# LIST(APPEND LIBRARY_LIST "-lnuma")

# message("library list: ${LIBRARY_LIST}")

INCLUDE_DIRECTORIES(${INCLUDE_LIST})

set (libmpl_src
  "${HYDRA_SRC}/mpl/src/mplstr.c"
  "${HYDRA_SRC}/mpl/src/mpltrmem.c"
  "${HYDRA_SRC}/mpl/src/mplenv.c"
#  "${HYDRA_SRC}/mpl/src/mplmsg.c" # mpich
  "${HYDRA_SRC}/mpl/src/mplsock.c"
)

ADD_LIBRARY(mpl SHARED ${libmpl_src})
TARGET_LINK_LIBRARIES(mpl ${LIBRARY_LIST})
INSTALL(TARGETS mpl LIBRARY DESTINATION lib)

set (libhwloc_embedded_src
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/traversal.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/distances.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/components.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/bind.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/bitmap.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/pci-common.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/diff.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/misc.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/base64.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-noos.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-synthetic.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-custom.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-xml.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-xml-nolibxml.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-linux.c"
  "${HYDRA_SRC}/tools/topo/hwloc/hwloc/src/topology-x86.c"
)

ADD_LIBRARY(libhwloc_embedded SHARED ${libhwloc_embedded_src})
TARGET_LINK_LIBRARIES(libhwloc_embedded ${LIBRARY_LIST})
INSTALL(TARGETS libhwloc_embedded LIBRARY DESTINATION lib)

set (libhydra_src
   "${HYDRA_SRC}/utils/alloc/alloc.c"
   "${HYDRA_SRC}/utils/args/args.c" 
   "${HYDRA_SRC}/utils/dbg/dbg.c"
   "${HYDRA_SRC}/utils/env/env.c"
   "${HYDRA_SRC}/utils/launch/launch.c" 
   "${HYDRA_SRC}/utils/others/others.c"
   "${HYDRA_SRC}/utils/signals/signals.c"
   "${HYDRA_SRC}/utils/sock/sock.c"
   "${HYDRA_SRC}/utils/string/string.c"
   "${HYDRA_SRC}/tools/topo/topo.c" 
   "${HYDRA_SRC}/tools/topo/hwloc/topo_hwloc.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_init.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_finalize.c" 
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_launch.c" 
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_query_proxy_id.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_wait.c"
   "${HYDRA_SRC}/tools/bootstrap/src/bsci_env.c"
   "${HYDRA_SRC}/tools/bootstrap/utils/bscu_wait.c"
   "${HYDRA_SRC}/tools/bootstrap/utils/bscu_cb.c"
   "${HYDRA_SRC}/tools/bootstrap/external/external_common.c"
   "${HYDRA_SRC}/tools/bootstrap/external/external_common_launch.c"
   "${HYDRA_SRC}/tools/bootstrap/external/fork_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/user_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/manual_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/rsh_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/rsh_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ssh_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ssh.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ssh_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ssh_finalize.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_launch.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/external/slurm_query_proxy_id.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_launch.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_query_proxy_id.c"
   "${HYDRA_SRC}/tools/bootstrap/external/ll_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/lsf_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/lsf_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/lsf_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/external/lsf_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/sge_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/sge_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/sge_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/external/sge_env.c"
   "${HYDRA_SRC}/tools/bootstrap/external/pbs_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/pbs_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/pbs_query_node_list.c"
   "${HYDRA_SRC}/tools/bootstrap/external/cobalt_init.c"
   "${HYDRA_SRC}/tools/bootstrap/external/cobalt_query_native_int.c"
   "${HYDRA_SRC}/tools/bootstrap/external/cobalt_query_node_list.c"
#   "${HYDRA_SRC}/tools/bootstrap/external/pbs_finalize.c"
#   "${HYDRA_SRC}/tools/bootstrap/external/pbs_launch.c"
#   "${HYDRA_SRC}/tools/bootstrap/external/pbs_wait.c"
#   "${HYDRA_SRC}/tools/bootstrap/external/pbs_env.c"
   "${HYDRA_SRC}/tools/bootstrap/persist/persist_init.c"
   "${HYDRA_SRC}/tools/bootstrap/persist/persist_launch.c"
   "${HYDRA_SRC}/tools/bootstrap/persist/persist_wait.c"
   "${HYDRA_SRC}/tools/ckpoint/ckpoint.c"
#   "${HYDRA_SRC}/tools/ckpoint/blcr/ckpoint_blcr.c"
   "${HYDRA_SRC}/tools/demux/demux.c"
   "${HYDRA_SRC}/tools/demux/demux_poll.c"
   "${HYDRA_SRC}/tools/demux/demux_select.c"
   "${HYDRA_SRC}/tools/debugger/debugger.c"
#   "${HYDRA_SRC}/tools/ftb/hydt_ftb.c"
   "${HYDRA_SRC}/tools/ftb/hydt_ftb_dummy.c"
)

ADD_LIBRARY(hydra SHARED ${libhydra_src})
TARGET_LINK_LIBRARIES(hydra ${LIBRARY_LIST})
INSTALL(TARGETS hydra LIBRARY DESTINATION lib)

set (libpm_src
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_pmi.c"
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_pmi_v1.c"
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_pmi_v2.c"
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_pmci.c"
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_cb.c"
  "${HYDRA_SRC}/pm/pmiserv/pmiserv_utils.c"
  "${HYDRA_SRC}/pm/pmiserv/common.c"
  "${HYDRA_SRC}/pm/pmiserv/pmi_v2_common.c"
)

ADD_LIBRARY(pm SHARED ${libpm_src})
TARGET_LINK_LIBRARIES(pm ${LIBRARY_LIST})
INSTALL(TARGETS pm LIBRARY DESTINATION lib)

set (mpiexec_src
  "${HYDRA_SRC}/ui/mpich/mpiexec.c"
  "${HYDRA_SRC}/ui/mpich/utils.c"
  "${HYDRA_SRC}/ui/utils/uiu.c"
)

ADD_EXECUTABLE(pmiserv ${mpiexec_src})
TARGET_LINK_LIBRARIES(pmiserv pm hydra libhwloc_embedded mpl)
INSTALL(TARGETS pmiserv RUNTIME DESTINATION bin)

set (spark_mpi_proxy_src
  "${HYDRA_SRC}/pm/pmiserv/pmip.c"
  pmip_cb.c
  "${HYDRA_SRC}/pm/pmiserv/pmip_utils.c"
  "${HYDRA_SRC}/pm/pmiserv/pmip_pmi_v1.c"
  "${HYDRA_SRC}/pm/pmiserv/pmip_pmi_v2.c"
  "${HYDRA_SRC}/pm/pmiserv/common.c"
  "${HYDRA_SRC}/pm/pmiserv/pmi_v2_common.c"
)

ADD_EXECUTABLE(hydra_pmi_proxy ${spark_mpi_proxy_src})
TARGET_LINK_LIBRARIES(hydra_pmi_proxy hydra libhwloc_embedded)
INSTALL(TARGETS hydra_pmi_proxy RUNTIME DESTINATION bin)






