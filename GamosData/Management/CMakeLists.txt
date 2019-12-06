#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosData_Management
# Package: GamosCore
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroLibraryTargets)

# Add allocation export symbol for this category                                                                       
add_definitions(-DGmDataMgt_ALLOC_EXPORT)

GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
