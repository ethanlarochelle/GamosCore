#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosSD
# Package: GamosCore
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)

add_definitions(-DGmSD_ALLOC_EXPORT)

include(Geant4MacroLibraryTargets)
GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
