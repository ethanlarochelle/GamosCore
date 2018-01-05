#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_OtherPhysicsLists
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_OtherPhysicsLists

   HEADERS
       GmDummyPhysics.hh
       GmDNAPhysics.hh
       GmLEPTSPhysics.hh

   SOURCES
       plugin.cc
       GmDummyPhysics.cc
       GmDNAPhysics.cc
       GmLEPTSPhysics.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}

   LINK_LIBRARIES
)

# List any source specific properties here
