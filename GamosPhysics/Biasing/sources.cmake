#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics_Biasing
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_Biasing
	 HEADERS
		GmBOptnBremsSplitting.hh
		GmBiasingMgr.hh
		GmBOptnEWAnnihilation.hh
		GmBOptnEWRayleigh.hh
		GmVBiasingOperator.hh
		GmBOptnEWBremsstrahlung.hh
		GmBOptrDirBremsSplitting.hh
		GmBOptnEWPhotoElectric.hh
		GmVEWBiasingOperation.hh
		GmBOptrEWBS.hh
		GmBOptnEWPairProduction.hh
		GmVEWBiasingOperation.h
		GmGenericBiasingPhysics.hh
		GmEWBSSplittingProcess.hh
		GmBiasingOperatorFactory.hh
		GmBOptnEWBS.hh
		GmBOptnDirBremsSplitting.hh
		GmBOptnEWCompton.hh
		GmBOptrBremsSplitting.hh
		GmBOptrForceCollisionTrackData.hh
		GmBiasingMessenger.hh
		GmBOptrChangeCrossSection.hh
		GmBiasingVerbosity.hh
		GmBOptrForceCollision.hh
	 SOURCES
		GmEWBSSplittingProcess.cc
		GmBOptnEWAnnihilation.cc
		GmBOptrEWBS.cc
		GmBOptrForceCollision.cc
		GmBOptnEWBS.cc
		GmBOptrDirBremsSplitting.cc
		GmBOptnEWPhotoElectric.cc
		GmBOptnBremsSplitting.cc
		GmBiasingMessenger.cc
		GmBiasingMgr.cc
		GmBOptnDirBremsSplitting.cc
		GmBiasingVerbosity.cc
		GmBOptnEWCompton.cc
		GmVEWBiasingOperation.cc
		GmBiasingOperatorFactory.cc
		GmBOptrForceCollisionTrackData.cc
		GmVBiasingOperator.cc
		GmBOptrBremsSplitting.cc
		GmBOptnEWRayleigh.cc
		GmBOptrChangeCrossSection.cc
		GmBOptnEWPairProduction.cc
		GmBOptnEWBremsstrahlung.cc
		GmGenericBiasingPhysics.cc
		plugin.cc
	 
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
