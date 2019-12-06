#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosUtils
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUtils
	 HEADERS
		GmFileIn.hh
		GmVerbosity.hh
		GmGenUtils.hh
		GmCone.hh
		GmNumberOfEvent.hh
		GmMovementUtils.hh
		GmPlane.hh
		GmG4Utils.hh
		GmCylindricalSurface.hh
		GmLine.hh
	 SOURCES
		GmCylindricalSurface.cc
		GmMovementUtils.cc
		GmG4Utils.cc
		GmLine.cc
		GmNumberOfEvent.cc
		GmCone.cc
		GmPlane.cc
		GmGenUtils.cc
		GmVerbosity.cc
		GmFileIn.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)
