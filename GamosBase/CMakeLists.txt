#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosBase
# Package: GamosCore
#
#------------------------------------------------------------------------------
add_definitions(-D_ALLOC_EXPORT)

add_definitions(-DGmBase_ALLOC_EXPORT)
 
add_subdirectory(Base)
add_subdirectory(Classifiers)
add_subdirectory(Filters)
