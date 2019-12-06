#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosAnalysis
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosAnalysis
	 HEADERS
		GmTrajStep.hh
		GmVHistoBuilder.hh
		GmVTrajPoint.hh
		GmTrajPointPosMom.hh
		GmTrajPointProcess.hh
		GmTrajPointSeco.hh
		GmTrajectorySteps.hh
		GmCheckOriginalGamma.hh
		GmVTrajectory.hh
		GmVHistoMgr.hh
		GmVTrajStep.hh
		GmTrajectoryPosMom.hh
		GmTrajStepEner.hh
		GmTrajStepProcess.hh
		GmVEventClassifier.hh
		GmTrajPoint.hh
		GmEventClassifierByGammaInteraction.hh
		GmTrajectory.hh
		GmTrajStepPosMom.hh
		GmAnalysisVerbosity.hh
	 SOURCES
		GmTrajStep.cc
		GmTrajStepProcess.cc
		GmTrajPointPosMom.cc
		GmTrajPointSeco.cc
		GmVTrajectory.cc
		GmTrajectoryPosMom.cc
		GmTrajStepEner.cc
		GmCheckOriginalGamma.cc
		GmTrajectorySteps.cc
		GmTrajStepPosMom.cc
		GmVHistoBuilder.cc
		GmTrajPointProcess.cc
		GmTrajectory.cc
		GmTrajPoint.cc
		GmAnalysisVerbosity.cc
		GmEventClassifierByGammaInteraction.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosUserActionMgr

	LINK_LIBRARIES
)
