# - Setup of general build options for Gamos Libraries
#
# In addition to the core compiler/linker flags (configured in the 
# GamosMakeRules_<LANG>.cmake files) for Gamos, the build may require
# further configuration. This module performs this task whicj includes:
#
#  1) Extra build modes for developers
#  2) Additional compiler definitions to assist visualization or optimize
#     performance.
#  3) Additional compiler flags which may be added optionally.
#  4) Whether to build shared and/or static libraries.
#  5) Whether to build libraries in global or granular format.
#
#

#-----------------------------------------------------------------------
# Set up Build types or configurations
# If further tuning of compiler flags is needed then it should be done here.
# (It can't be done in the make rules override section).
# However, exercise care when doing this not to override existing flags!!
# We don't do this on WIN32 platforms yet because of some teething issues
# with compiler specifics and linker flags
if(NOT WIN32)
  include(GamosBuildModes)
endif(NOT WIN32)

#-----------------------------------------------------------------------
# Optional compiler definitions which are applicable globally
#
# - Gm_STORE_TRAJECTORY
# ON by default, switching off can improve performance. Needs to be on
# for visualization to work fully. Mark as advanced because most users
# should not need to worry about it.
# FIXES : Bug #1208
option(GAMOS_BUILD_STORE_TRAJECTORY 
  "Store trajectories in event processing. Switch off for improved performance but note that visualization of trajectories will not be possible" 
  ON)
mark_as_advanced(GAMOS_BUILD_STORE_TRAJECTORY)

if(GAMOS_BUILD_STORE_TRAJECTORY)
  add_definitions(-DGm_STORE_TRAJECTORY)
endif()

# - GmVERBOSE
# ON by default, switching off can improve performance, but at the cost 
# of fewer informational or warning messages. Mark as advanced because 
# most users should not need to worry about it.
option(GAMOS_BUILD_VERBOSE_CODE 
  "Enable verbose output from Gamos code. Switch off for better performance at the cost of fewer informational messages or warnings"
  ON)
mark_as_advanced(GAMOS_BUILD_VERBOSE_CODE)

if(GAMOS_BUILD_VERBOSE_CODE)
  add_definitions(-DGmVERBOSE)
endif()

#-----------------------------------------------------------------------
# Optional compiler flags
#
# - GAMOS_BUILD_CXXSTD
# Choose C++ Standard to build against, if supported.
# Mark as advanced because most users will not need it.
if(CXXSTD_IS_AVAILABLE)
  enum_option(GAMOS_BUILD_CXXSTD 
    DOC "C++ Standard to compile against"
    VALUES ${CXXSTD_IS_AVAILABLE}
    CASE_INSENSITIVE
    )
  mark_as_advanced(GAMOS_BUILD_CXXSTD)
  gamos_add_feature(GAMOS_BUILD_CXXSTD "Compiling against C++ Standard '${GAMOS_BUILD_CXXSTD}'")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${GAMOS_BUILD_CXXSTD}_FLAGS}")
endif()

#-----------------------------------------------------------------------
# Setup Library Format Option.
# Libraries can be built in one of two 'formats':
#  global   : One library per category.
#  granular : One library per module.
#
# This division does not always apply because some libraries are the same
# in both 'formats', e.g. Gmmaterials or GmOpenGL.
# Global libraries are built by default, but we provide an option to 
# switch to granular format. Granular format is only intended for 
# developers, so we mark this option as advanced and warn the user.

# Still warn, because the variable can still be set from the command line!
if(GAMOS_BUILD_GRANULAR_LIBS)
  message(WARNING " Granular libraries are only intended for developers!")
endif()

#-----------------------------------------------------------------------
# Setup Shared and/or Static Library builds
# We name these options without a 'GAMOS_' prefix because they are 
# really higher level CMake options.
# Default to building shared libraries, mark options as advanced because
# most user should not have to touch them.
option(BUILD_SHARED_LIBS "Build Gamos shared libraries" ON)
option(BUILD_STATIC_LIBS "Build Gamos static libraries" OFF)
mark_as_advanced(BUILD_SHARED_LIBS BUILD_STATIC_LIBS)

# Because both could be switched off accidently, FATAL_ERROR if neither
# option has been selected.
if(NOT BUILD_STATIC_LIBS AND NOT BUILD_SHARED_LIBS)
  message(FATAL_ERROR "Neither static nor shared libraries will be built")
endif()

# On MSVC, warn if both shared and static are built - this has duplicate
# symbol issues on VS2010 Express.
# TODO: Resolve and understand this issue...
if(MSVC)
  if(BUILD_SHARED_LIBS AND BUILD_STATIC_LIBS)
    message(WARNING " Building shared AND static libraries on VS2010 may result in link errors.
 You are welcome to try building both, but please be aware of this warning.
 Problems can be reported to the Gamos Bugzilla system:
 
 http://bugzilla-gamos.kek.jp
    ")
  endif()
endif()

#------------------------------------------------------------------------
# Setup symbol visibility (library interface)
# We need to define that we're building Gamos
#

#------------------------------------------------------------------------
# Build options for building examples and tests
option(GAMOS_BUILD_EXAMPLES "Build all the examples of the project" OFF)
GAMOS_ADD_FEATURE(GAMOS_BUILD_EXAMPLES "Build all the examples of the project")
mark_as_advanced(GAMOS_BUILD_EXAMPLES)

option(GAMOS_BUILD_TESTS "Build all the tests of the project" OFF)
GAMOS_ADD_FEATURE(GAMOS_BUILD_TESTS "Build all the tests of the project")
mark_as_advanced(GAMOS_BUILD_TESTS)

# - Testing system only functional on 2.8 and above
if(NOT ${CMAKE_VERSION} VERSION_GREATER 2.7)
  if(GAMOS_ENABLE_TESTING)
    message(STATUS "WARNING: GAMOS_ENABLE_TESTING requires CMake >= 2.8 -- deactivating")
  endif()
  set(GAMOS_ENABLE_TESTING OFF CACHE INTERNAL "Testing NOT supported on CMake <2.8"
    FORCE)
else()
  option(GAMOS_ENABLE_TESTING "Enable and define all the tests of the project" OFF)
  GAMOS_ADD_FEATURE(GAMOS_ENABLE_TESTING "Enable and define all the tests of the project")
  mark_as_advanced(GAMOS_ENABLE_TESTING)
endif()


# On WIN32, we need to build the genwindef application to create export
# def files for building DLLs.
# We only use it as a helper application at the moment so we exclude it from 
# the ALL target.
# TODO: We could move this section into the GamosMacroLibraryTargets.cmake
# if it can be protected so that the genwindef target wouldn't be defined
# more than once... Put it here for now...
if(WIN32)
    add_definitions(-DGmLIB_BUILD_DLL)
    # Assume the sources are co-located 
    get_filename_component(_genwindef_src_dir ${CMAKE_CURRENT_LIST_FILE} PATH)
    add_executable(genwindef EXCLUDE_FROM_ALL 
        ${_genwindef_src_dir}/genwindef/genwindef.cpp 
        ${_genwindef_src_dir}/genwindef/LibSymbolInfo.h
        ${_genwindef_src_dir}/genwindef/LibSymbolInfo.cpp)
endif()


#------------------------------------------------------------------------
# Setup Locations for Build Outputs
# Because of the highly nested structure of Gamos, targets will be 
# distributed throughout this tree, potentially making usage and debugging 
# difficult (especially if developers use non-CMake tools).
#
# We therefore set the output directory of runtime, library and archive
# targets to some low level directories under the build tree.
#
# On Unices, we try to make the output directory backward compatible
# with the old style 'SYSTEM-COMPILER' format so that applications may be
# built against the targets in the build tree.
#
# Note that for multi-configuration generators like VS and Xcode, these
# directories will have the configuration type (e.g. Debug) appended to 
# them, so are not backward compatible with the old Make toolchain in 
# these cases.
#
# Also, we only do this on UNIX because we want to avoid mixing static and
# dynamic libraries on windows until the differences are better understood.
#------------------------------------------------------------------------
# Determine the backward compatible system name
#
if(NOT WIN32)
  set(GAMOS_SYSTEM ${CMAKE_SYSTEM_NAME})
else()
  set(GAMOS_SYSTEM "WIN32")
endif()

#------------------------------------------------------------------------
# Determine the backward compatible compiler name
# NB: At present Clang detection only works on CMake > 2.8.1
if(CMAKE_COMPILER_IS_GNUCXX)
  set(GAMOS_COMPILER "g++")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(GAMOS_COMPILER "clang")
elseif(MSVC)
  set(GAMOS_COMPILER "VC")
elseif(CMAKE_CXX_COMPILER MATCHES "icpc.*|icc.*")
  set(GAMOS_COMPILER "icc")
else()
  set(GAMOS_COMPILER "UNSUPPORTED")
endif()

#-----------------------------------------------------------------------
# Set the output paths to be backward compatible on UNIX
if(NOT UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/library)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/archive)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/runtime)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/outputs/library/${GAMOS_SYSTEM}-${GAMOS_COMPILER})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()


