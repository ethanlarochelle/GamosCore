#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosRunManager
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosRunManager
	 HEADERS
		GmFactoriesMessenger.hh
		GmDeprecatedCommandsMessenger.hh
		GmRunMessenger.hh
		GmUIMessenger.hh
		GmUIterminal.hh
		GmRunManager.hh
	 SOURCES
		GmRunManager.cc
		GmUIterminal.cc
		GmUIMessenger.cc
		GmFactoriesMessenger.cc
		GmRunMessenger.cc
		GmDeprecatedCommandsMessenger.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosBase_Base
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosUserActionMgr
	GamosCore_GamosAnalysis
	GamosCore_GamosSD
	GamosCore_GamosScoring_Management
	GamosCore_GamosPhysics_PhysicsList
	GamosCore_GamosPhysics_Cuts
	GamosCore_GamosPhysics_Biasing
	GamosCore_GamosPhysics_VarianceReduction
	MagFieldManager

	LINK_LIBRARIES
)
