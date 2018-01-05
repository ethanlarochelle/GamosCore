# - Macros for organizing and specializing code in Gamos modules
#
# This file defines the following macros for Gamos developers needing to
# define the sources, headers and library dependencies for a standard 
# Gamos granular library module, plus utlities for specializing source 
# file properties
#
# GAMOS_DEFINE_MODULE      - define a standard Gamos Granular Library 
#                             Module
#
# ======================
# A Gamos Module is defined as a directory containing subdirectories
#
#   include - holds all header files for the module
#   src     - holds all source files for the module
#
# GAMOS_DEFINE_MODULE will take the name of the module, a list of header 
# files, a list of source files and dependencies:
#
# GAMOS_DEFINE_MODULE(NAME name 
#                      HEADERS hdr1 hdr2 ... 
#                      SOURCES src1 src2 ...
#                      GRANULAR_DEPENDENCIES dep1 dep2 ...
#                      GLOBAL_DEPENDENCIES dep1 dep2 ...
#                      LINK_LIBRARIES lib1 lib2 ...)
#
# It assumes that it will be called from a CMake located in the module
# root directory (i.e. the directory containing the include and src
# subdirectories). It uses this location to define absolute paths to the
# headers and sources, and will ignore any absolute paths passed in HEADERS
# or SOURCES so that generated files can be passed in.
#
# The macro defines the variables
#
# GmMODULENAME = name
# ${GmMODULENAME}_HEADERS = List of absolute paths to files given in HEADERS
# ${GmMODULENAME}_SOURCES = List of absolute paths to files given in SOURCES
# ${GmMODULENAME}_GRANULAR_DEPS  = List of granular libraries on which this
#                                  module depends
# ${GmMODULENAME}_GLOBAL_DEPS    = List of global libraries on which this module
#                                  depends
# ${GmMODULENAME}_LINK_LIBRARIES = List of external libraries to be linked to
#                                  this module
#
# It will also add the module include directory to the list of directories
# using include_directories
#
#
# GAMOS_ADD_COMPILE_DEFINITIONS - add compile defintions to a list of files
# ================================
# GAMOS_ADD_COMPILE_DEFINITIONS(SOURCES src1 src2 ...
#                                COMPILE_DEFINITIONS def1 def 2)
#
# Here, SOURCES is the list of source files to which compile definitions
# will be added. COMPILE_DEFINITIONS gives the list of definitions that
# should be added. These definitions will be appended to any existing
# definitions given to the sources.
#

if(__GAMOSMACRODEFINEMODULE_ISLOADED)
  return()
endif()
set(__GAMOSMACRODEFINEMODULE_ISLOADED TRUE)

include(CMakeMacroParseArguments)

MACRO(GAMOS_DEFINE_MODULE)
    CMAKE_PARSE_ARGUMENTS(GmDEFMOD
    ""
    "NAME" 
    "HEADERS;SOURCES;GRANULAR_DEPENDENCIES;GLOBAL_DEPENDENCIES;LINK_LIBRARIES"    
        ${ARGN}
    )

    set(GmMODULENAME ${GmDEFMOD_NAME})

    get_filename_component(${GmMODULENAME}_BASEDIR ${CMAKE_CURRENT_LIST_FILE} PATH)
    set(${GmMODULENAME}_SRCDIR ${${GmMODULENAME}_BASEDIR}/src)
    set(${GmMODULENAME}_INCDIR ${${GmMODULENAME}_BASEDIR}/include)

    # We now create absolute paths to the headers and sources,
    # ignoring any already absolute paths
    foreach(_HDR ${GmDEFMOD_HEADERS})
        if(IS_ABSOLUTE ${_HDR})
            list(APPEND ${GmMODULENAME}_HEADERS ${_HDR})
        else()
            list(APPEND ${GmMODULENAME}_HEADERS ${${GmMODULENAME}_INCDIR}/${_HDR})
        endif()
    endforeach()


    foreach(_SRC ${GmDEFMOD_SOURCES})
        if(IS_ABSOLUTE ${_SRC})
            list(APPEND ${GmMODULENAME}_SOURCES ${_SRC})
        else()
            list(APPEND ${GmMODULENAME}_SOURCES ${${GmMODULENAME}_SRCDIR}/${_SRC})
        endif()
    endforeach()


    foreach(_LIB ${GmDEFMOD_GRANULAR_DEPENDENCIES})
        list(APPEND ${GmMODULENAME}_GRANULAR_DEPENDENCIES ${_LIB})
    endforeach()


    foreach(_LIB ${GmDEFMOD_GLOBAL_DEPENDENCIES})
        list(APPEND ${GmMODULENAME}_GLOBAL_DEPENDENCIES ${_LIB})
    endforeach()


    foreach(_LIB ${GmDEFMOD_LINK_LIBRARIES})
        list(APPEND ${GmMODULENAME}_LINK_LIBRARIES ${_LIB})
    endforeach()

    include_directories(${${GmMODULENAME}_INCDIR})
ENDMACRO()


# GAMOS_ADD_COMPILE_DEFINITIONS
MACRO(GAMOS_ADD_COMPILE_DEFINITIONS)
    CMAKE_PARSE_ARGUMENTS(GmADDDEF
        ""
        ""
        "SOURCES;COMPILE_DEFINITIONS"    
        ${ARGN}
    )

    # We assume that the sources have been added at the level of a
    # a sources.cmake, so are inside the src subdir of the sources.cmake
    get_filename_component(_ACD_BASE_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
    
    # Now for each file, add the definitions
    foreach(_acd_source ${GmADDDEF_SOURCES})
        # Extract any existing compile definitions
        get_source_file_property(
            _acd_existing_properties 
            ${_ACD_BASE_PATH}/src/${_acd_source}
            COMPILE_DEFINITIONS)

        if(_acd_existing_properties)
            set(_acd_new_defs ${_acd_existing_properties}
                ${GmADDDEF_COMPILE_DEFINITIONS})
        else()
            set(_acd_new_defs ${GmADDDEF_COMPILE_DEFINITIONS})
        endif()

        # quote compile defs because this must epand to space separated list
        set_source_files_properties(${_ACD_BASE_PATH}/src/${_acd_source}
            PROPERTIES COMPILE_DEFINITIONS "${_acd_new_defs}")
    endforeach()
ENDMACRO()

