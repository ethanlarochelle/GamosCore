#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics_Cuts
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_Cuts
	 HEADERS
		GmUserLimitsMgr.hh
		GmCutsEnergy2Range.hh
		GmStepLimiter.hh
		GmProdCutsStudyUA.hh
		GmUserSpecialCuts.hh
		GmCSTrackStepInfo.hh
		GmRangeFilter.hh
		GmMinRangeLimitsStudyCreateInfoUA.hh
		GmProdCutsForAllProcessesUA.hh
		GmCSTrackInfo.hh
		GmCutsEnergy2RangeUA.hh
		GmCutsTrackInfo.hh
		GmUserLimitsMessenger.hh
		GmRangeRejectionUA.hh
		GmInitialRangeFilter.hh
		GmMinRangeLimitsStudyUA.hh
		GmCutsStudyMgr.hh
	 SOURCES
		GmMinRangeLimitsStudyUA.cc
		GmUserLimitsMessenger.cc
		GmRangeFilter.cc
		GmCSTrackStepInfo.cc
		GmUserLimitsMgr.cc
		GmCutsTrackInfo.cc
		GmStepLimiter.cc
		GmCutsStudyMgr.cc
		GmCutsEnergy2Range.cc
		GmRangeRejectionUA.cc
		GmUserSpecialCuts.cc
		GmProdCutsForAllProcessesUA.cc
		GmProdCutsStudyUA.cc
		GmCSTrackInfo.cc
		GmInitialRangeFilter.cc
		GmCutsEnergy2RangeUA.cc
		plugin.cc
		GmMinRangeLimitsStudyCreateInfoUA.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosPhysics_PhysicsList

	LINK_LIBRARIES
)
