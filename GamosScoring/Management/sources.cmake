#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosScoring_Management
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Management
	 HEADERS
		GmVPrimitiveScorer.hh
		GmVChangeWorldAndSource.hh
		GmVPrimitiveScorerVector.hh
		GmEnergySplitter.hh
		GmPSPrinterMgr.hh
		GmScoringRun.hh
		GmVPSPrinter.hh
		GmEnergySplitter.icc
		GmScoringMgr.hh
		GmScoringVerbosity.hh
		GmScoringUA.hh
		GmPSPrinterFactory.hh
		GmCompoundScorer.hh
		GmPrimitiveScorerFactory.hh
		GmScoringMessenger.hh
	 SOURCES
		GmVPrimitiveScorerVector.cc
		GmEnergySplitter.cc
		GmScoringRun.cc
		GmScoringUA.cc
		GmCompoundScorer.cc
		GmVChangeWorldAndSource.cc
		GmScoringVerbosity.cc
		GmPSPrinterFactory.cc
		GmPSPrinterMgr.cc
		GmVPSPrinter.cc
		GmScoringMgr.cc
		GmPrimitiveScorerFactory.cc
		GmVPrimitiveScorer.cc
		GmScoringMessenger.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosBase_Filters
	GamosCore_GamosGeometry
	GamosCore_GamosReadDICOM
	GamosCore_GamosGenerator
	GamosCore_GamosData_Management
	GamosCore_GamosData_Users

	LINK_LIBRARIES
)
