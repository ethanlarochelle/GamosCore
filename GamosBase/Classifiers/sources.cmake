#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosBase_Classifiers
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosBase_Classifiers
	 HEADERS
		GmClassifierByPhysicalVolumeReplicated.hh
		GmClassifierBySecondariesListExclude.hh
		GmCompoundClassifier.hh
		GmClassifierByParallelLogicalVolume.hh
		GmClassifierByProcess.hh
		GmClassifierByCreatorProcess.hh
		GmClassifierByParticleProcess.hh
		GmClassifierByPhysicalVolume.hh
		GmClassifierBy1Ancestor.hh
		GmClassifierBySecondariesList.hh
		GmClassifierByPrimaryParticle.hh
		GmClassifierByRegion.hh
		GmClassifierByKineticEnergy.hh
		GmClassifierByParticle.hh
		GmClassifierByLogicalVolume.hh
		GmClassifierByHadronicReaction.hh
		GmClassifierByAncestors.hh
		GmClassifierByMaterial.hh
		GmClassifierVerbosity.hh
	 SOURCES
		GmClassifierByMaterial.cc
		GmCompoundClassifier.cc
		GmClassifierByPhysicalVolumeReplicated.cc
		GmClassifierByParallelLogicalVolume.cc
		GmClassifierByParticle.cc
		GmClassifierByKineticEnergy.cc
		GmClassifierByProcess.cc
		GmClassifierByRegion.cc
		GmClassifierBySecondariesList.cc
		GmClassifierByHadronicReaction.cc
		GmClassifierByPrimaryParticle.cc
		GmClassifierBy1Ancestor.cc
		GmClassifierByPhysicalVolume.cc
		GmClassifierBySecondariesListExclude.cc
		GmClassifierByParticleProcess.cc
		GmClassifierByLogicalVolume.cc
		GmClassifierByCreatorProcess.cc
		GmClassifierVerbosity.cc
		plugin.cc
		GmClassifierByAncestors.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosGeometry

	LINK_LIBRARIES
)
