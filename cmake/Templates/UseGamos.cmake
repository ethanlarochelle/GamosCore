# - Use file for Gamos
# This file should be included after a find_package call has successfully
# located Gamos. If Gamos has been located via the GamosConfig.cmake
# config file, this will have set the following variable:
#
#  Gamos_USE_FILE : Point to the location of the use file for the found
#                    Gamos installation.
#
# Inclusion of this file, e.g. via
# 
#  include(${Gamos_USE_FILE})
#
# results in the addition of the Gamos compile definitions and 
# include directories to those of the directory in which this file is 
# included.
#
# The recommended Gamos compiler flags are also added to CMAKE_CXX_FLAGS
# but duplicated flags are NOT removed.
# Advanced users requiring special sets of flags, or the removal of 
# duplicate flags should therefore *not* use this file, preferring the 
# direct use of the Gamos_XXXX variables set by the GamosConfig file.
#
# The use file also defines a simple macro to help with collating sources
# for simple user applications.
#
#  macro GAMOS_COLLATE_APPLICATION_SOURCES(sources)
#        Create the list of sources in a standard Gamos application 
#        example.
#        If the project is organized as
#        +- project
#           +- CMakeLists.txt
#           +- project.cc
#           +- include/
#           |  +- project_impl_a.hh
#           |  +- ...
#           +- src/
#              +- project_impl_a.cc
#              +- ...
#        Then when called in the CMakeLists.txt under 'project', it will
#        collate all the .hh and .cc files into the sources variable, and
#        add the include directory to those to be searched for.
#        The CMakeLists.txt for project would then read:
#       
#        cmake_minimum_required(VERSION 2.6.4)
#        project(myproject)
#
#        find_package(Gamos REQUIRED)
#        include(${Gamos_USE_FILE})
#
#        GAMOS_COLLATE_APPLICATION_SOURCES(my_project_sources)
#
#        add_executable(myproject ${my_project_sources})
#        target_link_libraries(myproject ${Gamos_LIBRARIES})
#
#
#        This gives you maximum flexibility in setting up your project, as
#        you can either use this macro for simplicity and to easily convert
#        an existing application to CMake, or you can set the sources manually
#        with your own arrangement of headers and source files.
#
#
# The last thing the module does is to optionally include an internal Use 
# file. This file can contain variables, functions and macros for strict
# internal use in Gamos, such as building and running validation tests.
#

#-----------------------------------------------------------------------
# macro GAMOS_COLLATE_SOURCES(source_dest_var)
#
macro(GAMOS_COLLATE_APPLICATION_SOURCES source_dest_var)
  message(" WARNING: macro gamos_collate_application sources is deprecated and will be removed in GAMOS version 10.0.")
  file(GLOB_RECURSE 
    ${source_dest_var} 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hh 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
    )
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
endmacro()

#-----------------------------------------------------------------------
# We need to set the compile definitions and include directories
#
add_definitions(${Gamos_DEFINITIONS})
include_directories(${Gamos_INCLUDE_DIRS})

#-----------------------------------------------------------------------
# Because Gamos is sensitive to the compiler flags, lets set the base set
# here. This reproduces as far as possible the behaviour of the original
# makefile system.
#
set(CMAKE_CXX_FLAGS                "${Gamos_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG          "${Gamos_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "${Gamos_CXX_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_RELEASE        "${Gamos_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${Gamos_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_EXE_LINKER_FLAGS         "${Gamos_EXE_LINKER_FLAGS}")



#-----------------------------------------------------------------------
# Locate ourselves
#
get_filename_component(_use_gamos_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

#-----------------------------------------------------------------------
# Append the local module path to CMAKE_MODULE_PATH to automatically
# make FindXXX modules for examples available
#
list(APPEND CMAKE_MODULE_PATH ${_use_gamos_dir}/Modules)

#-----------------------------------------------------------------------
# Include internal use file if it exists. It should only exist in the
# build tree!
#
include(${_use_gamos_dir}/UseGamos_internal.cmake OPTIONAL)

