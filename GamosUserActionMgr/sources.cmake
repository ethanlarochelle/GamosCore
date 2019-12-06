#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosUserActionMgr
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUserActionMgr
	 HEADERS
		GmUserEventActionList.hh
		GmFutureWithChildrenFilter.hh
		GmUserActionMgr.icc
		GmUserRunActionList.hh
		GmUserEventAction.hh
		GmUserActionFactory.hh
		GmUserRunAction.hh
		GmUserStackingAction.hh
		GmUserTrackingAction.hh
		GmUserStackingActionList.hh
		GmUserSteppingActionList.hh
		GmUserSteppingAction.hh
		GmUAVerbosity.hh
		GmUserActionMessenger.hh
		GmUserAction.hh
		GmStepMgr.hh
		GmUserTrackingActionList.hh
		GmUserActionMgr.hh
		GmFutureFilter.hh
		GmFutureFilterUA.hh
	 SOURCES
		GmUserSteppingAction.cc
		GmUserActionMgr.cc
		GmUserEventAction.cc
		GmUserAction.cc
		GmUserActionFactory.cc
		GmUserTrackingAction.cc
		GmUserRunAction.cc
		GmStepMgr.cc
		GmUserStackingActionList.cc
		GmFutureFilter.cc
		GmFutureWithChildrenFilter.cc
		GmFutureFilterUA.cc
		GmUserEventActionList.cc
		GmUserActionMessenger.cc
		GmUserRunActionList.cc
		GmUAVerbosity.cc
		GmUserTrackingActionList.cc
		plugin.cc
		GmUserStackingAction.cc
		GmUserSteppingActionList.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base

	LINK_LIBRARIES
)
