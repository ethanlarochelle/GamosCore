#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosMovement
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosMovement
	 HEADERS
		GmMovementDisplacement.hh
		GmMovementMgr.hh
		GmMovementFromFile.hh
		GmMovementNEvents.hh
		GmVMovement.hh
		GmMovementEventAction.hh
		GmMovementTime.hh
		GmMovementVerbosity.hh
		GmMovementMessenger.hh
		GmMovementRotation.hh
	 SOURCES
		GmMovementRotation.cc
		GmMovementMessenger.cc
		GmMovementEventAction.cc
		GmMovementDisplacement.cc
		GmMovementVerbosity.cc
		GmMovementFromFile.cc
		GmMovementMgr.cc
		GmVMovement.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosUserActionMgr
	GamosCore_GamosGeometry
	GamosCore_GamosGenerator

	LINK_LIBRARIES
)
