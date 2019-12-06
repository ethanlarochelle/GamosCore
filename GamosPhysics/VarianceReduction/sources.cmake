#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics_VarianceReduction
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_VarianceReduction
	 HEADERS
		GmDeexSpliteIonisation.hh
		GmVDeexSplitEnergyLossProcess.hh
		GmPhysicsGammaPenelopeDeexSplit.hh
		GmDeexSplitLivermoreComptonModel.hh
		GmPhysicsElectronLowEnerDeexSplit.hh
		GmPhysicsElectronLowEnerUBSDeexSplit.hh
		GmPhysicsGammaLowEnerDeexSplit.hh
		GmWrapperProcess.hh
		GmDeexSplitPenelopeComptonModel.hh
		GmPhysicsElectronPenelopeDeexSplit.hh
		GmSplitWrapperProcess.hh
		GmImportanceSamplingProcess.hh
		GmDeexSplitLivermorePhotoElectricModel.hh
		GmInheritTrackInfoUA.hh
		GmVarianceReductionMessenger.hh
		GmDeexSplitPenelopePhotoElectricModel.hh
	 SOURCES
		GmWrapperProcess.cc
		GmVarianceReductionMessenger.cc
		GmInheritTrackInfoUA.cc
		GmSplitWrapperProcess.cc
		GmImportanceSamplingProcess.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosPhysics_PhysicsList
	GamosCore_GamosUserActionMgr
	GamosCore_GamosUtils

	LINK_LIBRARIES
)
