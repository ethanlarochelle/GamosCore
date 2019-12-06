#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics
# Package: GamosCore
#
#------------------------------------------------------------------------------

add_definitions(-DGmPhysics_ALLOC_EXPORT)

# 
add_subdirectory(PhysicsList)
add_subdirectory(OtherPhysicsLists)
add_subdirectory(Cuts)
add_subdirectory(VarianceReduction)
add_subdirectory(Biasing)
