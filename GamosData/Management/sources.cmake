#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosData_Management
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Management
	 HEADERS
		GmDataAncestorStepNumericData.hh
		GmVDataProf2DHisto.hh
		GmVDataString.hh
		GmVDataAccumulated.hh
		GmVData2DHisto.hh
		GmVDataProf1DHisto.hh
		GmVDataInt.hh
		GmDataMgr.hh
		GmDataAncestorStepStringData.hh
		GmDataVerbosity.hh
		GmVDataNon1DHisto.hh
		GmVData.hh
		GmDataFactory.hh
	 SOURCES
		GmVData2DHisto.cc
		GmDataAncestorStepNumericData.cc
		GmDataVerbosity.cc
		GmVDataInt.cc
		GmVData.cc
		GmVDataNon1DHisto.cc
		GmVDataString.cc
		GmVDataProf2DHisto.cc
		GmDataAncestorStepStringData.cc
		GmVDataProf1DHisto.cc
		GmDataMgr.cc
		GmDataFactory.cc
		plugin.cc
		GmVDataAccumulated.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosBase_Base
	GamosCore_GamosUserActionMgr

	LINK_LIBRARIES
)
