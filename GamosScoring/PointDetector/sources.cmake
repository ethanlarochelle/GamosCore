#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosScoring_PointDetector
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_PointDetector
	 HEADERS
		GmPDSGeantinoProcess.hh
		GmPDSGammaProcess.hh
		GmPDSUtils.hh
		GmPDSInteractionAngleManager.hh
		GmPDSScore.hh
		GmPDSUA.hh
		GmPDSVProcess.hh
		GmPDSProcessHelper.hh
		GmPDSNeutronProcess.hh
		GmPDSScoringTrackInfo.hh
		GmPDSDetector.hh
		GmPDSInvertGeantinoStackUA.hh
		GmPDSCreateAngleTablesUA.hh
	 SOURCES
		GmPDSVProcess.cc
		GmPDSProcessHelper.cc
		GmPDSGammaProcess.cc
		GmPDSNeutronProcess.cc
		GmPDSDetector.cc
		GmPDSScore.cc
		GmPDSGeantinoProcess.cc
		GmPDSCreateAngleTablesUA.cc
		GmPDSUtils.cc
		GmPDSInteractionAngleManager.cc
		GmPDSUA.cc
		GmPDSInvertGeantinoStackUA.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosGeometry
	GamosCore_GamosReadDICOM
	GamosCore_GamosGenerator
	GamosCore_GamosData_Management
	GamosCore_GamosData_Users
	GamosCore_GamosScoring_Management

	LINK_LIBRARIES
)
