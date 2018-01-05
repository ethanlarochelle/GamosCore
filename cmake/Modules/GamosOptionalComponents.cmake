# - Setup core required and optional components of Gamos
#
# Here we provide options to enable and configure required and optional
# components, which may require third party libraries.
#
# We don't configure User Interface options here because these require
# a higher degree of configuration so to keep things neat these have their
# own Module.
#
# Options configured here:
#  CLHEP  - Control use of internal Gmclhep, or locate external CLHEP
#  EXPAT  - Control use of internal Gmexpat, or locate external EXPAT.
#  ZLIB   - Control use of internal Gmzlib, or locate external ZLIB
#           (NOTIMPLEMENTEDYET - always uses internal zlib)
#  GDML   - Requires external XercesC
#  G3TOG4 - UNIX only

#-----------------------------------------------------------------------
# Find required CLHEP package
# We prefer to use our internal CLHEP library, but provide an option
# to allow an external CLHEP to be used should the user require this. 
# We also allow that it can be automatically enabled by providing
# the CLHEP_ROOT_DIR option (which FindCLHEP will recognize)
#
# KNOWNISSUE : For internal CLHEP, how to deal with static and shared?
if(CLHEP_ROOT_DIR)
  set(_default_use_system_clhep ON)
else()
  set(_default_use_system_clhep OFF)
endif()

option(GAMOS_USE_SYSTEM_CLHEP "Use system CLHEP library" ${_default_use_system_clhep})

if(GAMOS_USE_SYSTEM_CLHEP)
  # We keep this as required, because if the user chooses to use a
  # system option we assume that we absolutely, positively require this.
  find_package(CLHEP 2.1.2.3 REQUIRED)
  set(GAMOS_USE_SYSTEM_CLHEP TRUE)
else()
  set(CLHEP_FOUND TRUE)
  set(CLHEP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/source/externals/clhep/include")
  if(BUILD_SHARED_LIBS)
    set(CLHEP_LIBRARIES Gmclhep)
  else()
    set(CLHEP_LIBRARIES Gmclhep-static)
  endif()
endif()

GAMOS_ADD_FEATURE(GAMOS_USE_SYSTEM_CLHEP "Use system CLHEP library")

#-----------------------------------------------------------------------
# Find required EXPAT package
# We always use the internal Gmexpat on WIN32.
# On other platforms, we default to use the system library.
# If we use the internal Gmexpat, fix the EXPAT_XXX variables to point 
# to the internal location of expat headers and library target so Gamos 
# clients of expat have a consistent interface.
#
# KNOWNISSUE : For internal expat, how to deal with static and shared?
if(WIN32)
  set(EXPAT_FOUND TRUE)
  set(EXPAT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/source/externals/expat/include)
  set(EXPAT_LIBRARIES Gmexpat)
else()
  option(GAMOS_USE_SYSTEM_EXPAT "Use system Expat library" ON)

  if(GAMOS_USE_SYSTEM_EXPAT)
    # If system package requested, make damn sure we find it
    find_package(EXPAT REQUIRED)
  else()
    set(EXPAT_FOUND TRUE)
    set(EXPAT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/source/externals/expat/include)
    if(BUILD_SHARED_LIBS)
      set(EXPAT_LIBRARIES Gmexpat)
    else()
      set(EXPAT_LIBRARIES Gmexpat-static)
    endif()
  endif()
endif()

GAMOS_ADD_FEATURE(GAMOS_USE_SYSTEM_EXPAT "Use system EXPAT library")

#-----------------------------------------------------------------------
# Find required ZLIB package
# For now, we always use the internal Gamos zlib...
#
#option(GAMOS_USE_SYSTEM_ZLIB "Use the system's zlib library" OFF)
if(GAMOS_USE_SYSTEM_ZLIB)
  # This needs more work - use ITK's way of doing it as an example.
  find_package(ZLIB REQUIRED)
endif(GAMOS_USE_SYSTEM_ZLIB)

GAMOS_ADD_FEATURE(GAMOS_USE_SYSTEM_ZLIB "Use system zlib library")

#-----------------------------------------------------------------------
# Optional Support for GDML - requires Xerces-C package
#
if(XERCESC_ROOT_DIR)
  set(_default_use_gdml ON)
else()
  set(_default_use_gdml OFF)
endif()

option(GAMOS_USE_GDML "Build Gamos with GDML support" ${_default_use_gdml}
)

if(GAMOS_USE_GDML)
  find_package(XercesC REQUIRED)
endif(GAMOS_USE_GDML)

GAMOS_ADD_FEATURE(GAMOS_USE_GDML "Build Gamos with GDML support")

#-----------------------------------------------------------------------
# Optional support for G3TOG4 convertion interface.
# We do not build the rztogm application.
# -- OLDER NOTES --
# The G3toGm *library* should always be built, but the rztogm application 
# requires a Fortran compiler AND CERNLIB, so is optional.
# Only on *NIX because converter requires CERNLIB, and Windows support for
# this is an unknown quantity at present (can always change later).
# -- OLDER NOTES --
#
if(UNIX)
  option(GAMOS_USE_G3TOG4 "Build Geant3 ASCII call list reader library" OFF)
  GAMOS_ADD_FEATURE(GAMOS_USE_G3TOG4 "Build Geant3 ASCII call list reader library")
endif()

