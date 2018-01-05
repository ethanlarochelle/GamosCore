# - Build GamosConfig.cmake file and support scripts for build and install.
#

#-----------------------------------------------------------------------
# Collect all global variables we need to export to the config files
# Do this here for now, later on we could collect them as we go.
#

# Compiler flags (because user apps are a bit dependent on them...)
set(GAMOS_COMPILER_FLAG_HINTS "#
set(Gamos_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\")
set(Gamos_EXE_LINKER_FLAGS \"${CMAKE_EXE_LINKER_FLAGS}\")")

foreach(_mode DEBUG MINSIZEREL RELEASE RELWITHDEBINFO)
    set(GAMOS_COMPILER_FLAG_HINTS "${GAMOS_COMPILER_FLAG_HINTS}
set(Gamos_CXX_FLAGS_${_mode} \"${CMAKE_CXX_FLAGS_${_mode}}\")")
endforeach()


# Core compile definitions...
set(GAMOS_CORE_DEFINITIONS )

# Third party includes (libraries *should* be handled by the imports)
set(GAMOS_THIRD_PARTY_INCLUDES )

# Imports of third party packages used with imported targets
set(GAMOS_THIRD_PARTY_IMPORT_SETUP )

# Externals libraries that may be present
set(GAMOS_EXTERNALS_TARGETS )


# - Stuff from GamosLibraryBuildOptions.cmake
if(GAMOS_BUILD_STORE_TRAJECTORY)
  list(APPEND GAMOS_CORE_DEFINITIONS -DGm_STORE_TRAJECTORY)
endif()

if(GAMOS_BUILD_VERBOSE_CODE)
  list(APPEND GAMOS_CORE_DEFINITIONS -DGmVERBOSE)
endif()

# - We do actually need GmLIB_BUILD_DLL on Windows, even for user 
# applications...
if(WIN32)
  list(APPEND GAMOS_CORE_DEFINITIONS -DGmLIB_BUILD_DLL)
endif()


# - Stuff from GamosOptionalComponents.cmake
# - CLHEP
# If it's internal, add it to the externals list, if it's external, add the 
# include directories to the list of third party header paths
if(GAMOS_USE_SYSTEM_CLHEP)
  list(APPEND GAMOS_THIRD_PARTY_INCLUDES ${CLHEP_INCLUDE_DIRS})
else()
  list(APPEND GAMOS_EXTERNALS_TARGETS Gmclhep)
endif()

# - Expat
# If it's internal, add it to the externals list
if(NOT GAMOS_USE_SYSTEM_EXPAT)
  list(APPEND GAMOS_EXTERNALS_TARGETS Gmexpat)
endif()

# - ZLIB
# If it's internal, add it to the externals list
if(NOT GAMOS_USE_SYSTEM_ZLIB)
  list(APPEND GAMOS_EXTERNALS_TARGETS Gmzlib)
endif()

# - GDML
# Need to include Xerces-C headers becuase these do appear in the public
# interface of GDML. The library should then be in the LINK_INTERFACE of
# persistency...
if(GAMOS_USE_GDML)
  list(APPEND GAMOS_THIRD_PARTY_INCLUDES ${XERCESC_INCLUDE_DIRS})
endif()

# - Stuff from GamosInterfaceOptions.cmake
if(GAMOS_USE_QT)
  list(APPEND GAMOS_THIRD_PARTY_INCLUDES 
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QT_QTOPENGL_INCLUDE_DIR}
    )

  # On WIN32, re-import the Qt targets.    
  if(WIN32)
    set(GAMOS_THIRD_PARTY_IMPORT_SETUP "${GAMOS_THIRD_PARTY_IMPORT_SETUP}
# Qt reimport on WIN32
set(QT_QMAKE_EXECUTABLE ${QT_QMAKE_EXECUTABLE})
set(QT_USE_IMPORTED_TARGETS ON)
find_package(Qt4 REQUIRED)"
        )
  endif()
endif()

#-----------------------------------------------------------------------
# - Generate Build Tree Configuration Files
#-----------------------------------------------------------------------
# Set needed variables for the build tree
set(GAMOS_CMAKE_DIR "${PROJECT_BINARY_DIR}")

# Set include path for build tree. This is always an absolute path, or 
# rather paths. We extract the paths from the global 
# GAMOS_BUILDTREE_INCLUDE_DIRS property and use this to create the 
# header setup
#
get_property(__gamos_buildtree_include_dirs GLOBAL PROPERTY
    GAMOS_BUILDTREE_INCLUDE_DIRS)

set(GAMOS_INCLUDE_DIR_SETUP "
# Gamos configured for use from the build tree - absolute paths are used.
set(Gamos_INCLUDE_DIR ${__gamos_buildtree_include_dirs})
")

set(GAMOS_MODULE_PATH_SETUP "
# Gamos configured for use CMake modules from source tree
set(CMAKE_MODULE_PATH \${CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})
")

# Export targets from the build tree. We rely on the GAMOS_EXPORTED_TARGETS
# global property to list these for us.
#
get_property(__gamos_exported_targets GLOBAL PROPERTY GAMOS_EXPORTED_TARGETS)

export(TARGETS ${__gamos_exported_targets} FILE
    ${PROJECT_BINARY_DIR}/GamosLibraryDepends.cmake)


# Configure the build tree config file...
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/GamosConfig.cmake.in
    ${PROJECT_BINARY_DIR}/GamosConfig.cmake
    @ONLY)

# Configure the build tree versioning file
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/GamosConfigVersion.cmake.in
    ${PROJECT_BINARY_DIR}/GamosConfigVersion.cmake
    @ONLY)

# Copy the custom modules into the build tree
configure_file(${PROJECT_SOURCE_DIR}/cmake/Modules/CMakeMacroParseArguments.cmake
  ${PROJECT_BINARY_DIR}/Modules/CMakeMacroParseArguments.cmake
  COPYONLY
  )

foreach(_mod AIDA HepMC Pythia6 ROOT StatTest)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Modules/Find${_mod}.cmake
    ${PROJECT_BINARY_DIR}/Modules/Find${_mod}.cmake
    COPYONLY
    )
endforeach()

# Copy the Main and Internal Use file into the build tree
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/UseGamos.cmake
    ${PROJECT_BINARY_DIR}/UseGamos.cmake
    COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/UseGamos_internal.cmake
    ${PROJECT_BINARY_DIR}/UseGamos_internal.cmake
    COPYONLY)


#-----------------------------------------------------------------------
# - Generate Install Tree Configuration Files
#-----------------------------------------------------------------------
# Set needed variables for the install tree
set(GAMOS_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION})

# Header path for install tree is dependent on whether we have a relocatable
# install.
if(CMAKE_INSTALL_IS_NONRELOCATABLE)
    # Use ABSOLUTE paths...
    set(GAMOS_INCLUDE_DIR_SETUP "
# Gamos configured for the install tree with absolute paths, so use these
set(Gamos_INCLUDE_DIR \"${CMAKE_INSTALL_FULL_INCLUDEDIR}/Gamos\")
")
else()
    # Use RELATIVE paths... Where we measure relative to GAMOS_CMAKE_DIR
    file(RELATIVE_PATH GAMOS_RELATIVE_HEADER_PATH
        ${CMAKE_INSTALL_PREFIX}/${GAMOS_CMAKE_DIR}
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    )

    set(GAMOS_INCLUDE_DIR_SETUP "
# Gamos configured for the install with relative paths, so use these
get_filename_component(Gamos_INCLUDE_DIR \"\${_thisdir}/${GAMOS_RELATIVE_HEADER_PATH}\" ABSOLUTE)
")
endif()

set(GAMOS_MODULE_PATH_SETUP)

# Install exported targets file for the install tree - we just install
# the named export
install(EXPORT GamosLibraryDepends 
    DESTINATION ${GAMOS_CMAKE_DIR}
    COMPONENT Development
)


# Configure the install tree config file...
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/GamosConfig.cmake.in
    ${PROJECT_BINARY_DIR}/InstallTreeFiles/GamosConfig.cmake
    @ONLY
)

# Configure the install tree config versioning file...
configure_file(${PROJECT_SOURCE_DIR}/cmake/Templates/GamosConfigVersion.cmake.in
    ${PROJECT_BINARY_DIR}/InstallTreeFiles/GamosConfigVersion.cmake
    @ONLY
)

# Install the config, config versioning and use files
install(FILES
  ${PROJECT_BINARY_DIR}/InstallTreeFiles/GamosConfig.cmake
  ${PROJECT_BINARY_DIR}/InstallTreeFiles/GamosConfigVersion.cmake
  ${PROJECT_SOURCE_DIR}/cmake/Templates/UseGamos.cmake
  DESTINATION ${GAMOS_CMAKE_DIR}
  COMPONENT Development
  )

# Install the custom modules for the examples
install(DIRECTORY
  ${PROJECT_BINARY_DIR}/Modules
  DESTINATION ${GAMOS_CMAKE_DIR}
  COMPONENT Development
  )

