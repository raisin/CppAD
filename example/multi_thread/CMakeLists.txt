# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# ----------------------------------------------------------------------------

# initialize list as empty
SET(check_example_multi_thread_depends "")

# Define the operation
# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
#  LIBRARY  - the name of the library you are looking for
#  FUNCTION - the name of the function
#  LOCATION - location where the library should be found
#  VARIABLE - variable to store the result
INCLUDE(CheckLibraryExists)

#
# openmp
IF ( OpenMP_CXX_FOUND )
   # OpenMP_CXX_FLAGS - flags to add to the CXX compiler for OpenMP support
   ADD_SUBDIRECTORY(openmp)
ENDIF ( OpenMP_CXX_FOUND )
#
# pthread_lib_path
SET( pthread_ok FALSE )
FIND_LIBRARY(pthread_lib_path pthread)
MESSAGE(STATUS "pthread library path = ${pthread_lib_path}")
IF ( pthread_lib_path )
   SET(CMAKE_REQUIRED_DEFINITIONS "")
   SET(CMAKE_REQUIRED_FLAGS       "")
   SET(CMAKE_REQUIRED_INCLUDES    "")
   SET(CMAKE_REQUIRED_LIBRARIES   "")
   CHECK_LIBRARY_EXISTS(
      pthread pthread_barrier_wait ${pthread_lib_path} pthread_ok
   )
   IF ( pthread_ok )
      ADD_SUBDIRECTORY(pthread)
   ENDIF ( pthread_ok )
ENDIF ( pthread_lib_path )
IF( NOT pthread_ok )
   SET( pthread_lib_path "" )
ENDIF( NOT pthread_ok )
#
# sthread
# Put sthread after pthread because it can somtimes require pthread_lib_path
# https://stackoverflow.com/questions/46994982
ADD_SUBDIRECTORY(sthread)
#
# bthread
IF ( Boost_FOUND )
   SET(CMAKE_REQUIRED_DEFINITIONS "")
   SET(CMAKE_REQUIRED_FLAGS       "")
   SET(CMAKE_REQUIRED_INCLUDES    "${Boost_INCLUDE_DIRS}")
   SET(CMAKE_REQUIRED_LIBRARIES   "${Boost_LIBRARIES}")
   SET(source "
# include <boost/thread.hpp>
int main(void)
{  boost::barrier wait(1);
   return 0;
}"
   )
   compile_source_test(${cmake_defined_ok} "${source}" boost_multi_thread_ok )
   #
   IF( boost_multi_thread_ok )
      ADD_SUBDIRECTORY(bthread)
   ENDIF( boost_multi_thread_ok )
   #
ENDIF ( Boost_FOUND )

IF( NOT( "${check_example_multi_thread_depends}" STREQUAL "" ) )
   #
   # check_example_multi_thread
   ADD_CUSTOM_TARGET(
      check_example_multi_thread
      DEPENDS ${check_example_multi_thread_depends}
   )
   MESSAGE(STATUS "make check_example_multi_thread: available")
   #
   # Change check depends in parent environment
   add_to_list(check_example_depends check_example_multi_thread)
   SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
   #
ENDIF( NOT( "${check_example_multi_thread_depends}" STREQUAL "" ) )
