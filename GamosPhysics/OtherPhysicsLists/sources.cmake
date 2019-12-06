#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics_OtherPhysicsLists
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_OtherPhysicsLists
	 HEADERS
		GmDummyPhysics.hh
		PhysListEmStandardSS.hh
		GmEmDNAPhysics.hh
		GmQGSP_BIC_HP_EMopt3.hh
		GmEmDNAPhysicsMessenger.hh
		GmG4PhysicsLists.hh
		GmQGSP_BIC_HP_EMopt3.icc
	 SOURCES
		GmEmDNAPhysics.cc
		GmEmDNAPhysicsMessenger.cc
		GmDummyPhysics.cc
		PhysListEmStandardSS.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosPhysics_PhysicsList

	LINK_LIBRARIES
)
