%module sparkmpi

%{
#define SWIG_FILE_WITH_INIT
#define PY_ARRAY_UNIQUE_SYMBOL sparkmpi_ARRAY_API
#include <dlfcn.h>
#include "python_sparkmpi.hpp"
%}

%include "numpy.i"

#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <std_string.i>
%include <std_vector.i>
%include <std_shared_ptr.i>

%shared_ptr(sparkmpi::AddressServer)
%shared_ptr(sparkmpi::Communicator)

%init %{
  import_array();
  // This is here to avoid problems with missing symbols due to
  // the bad interactions between python and the crazy ways MPI
  // loads its libraries.
  // Check for example https://code.google.com/p/petsc4py/issues/detail?id=14
  dlopen("libmpi.so", RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD);
%}

%include "python_sparkmpi.hpp"
