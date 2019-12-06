#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosPhysics_PhysicsList
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_PhysicsList
	 HEADERS
		GmPhysicsMuonStandard.hh
		HadrontherapyPhysicsMessenger.hh
		GmPhysicsPositronPenelope.hh
		GmEMPhysicsMessenger.hh
		GmPhysicsGammaPenelope.hh
		GmPhysicsElectronLowEner.hh
		HadrontherapyStepMax.hh
		GmPhysicsGammaStandard.hh
		GmPhysicsElectronPenelope.hh
		GmPhysicsGammaLowEner.hh
		GmPhysicsParallel.hh
		GmPhysicsElectronStandard.hh
		GmEMStandardPhysics.hh
		GmPhysicsPositronStandard.hh
		GmEMPhysics.hh
		GmVPhysicsElectron.hh
		GmEMExtendedPhysics.hh
		GmEMStandardPhysicsMessenger.hh
		GmPhysicsGammaNuclear.hh
		LocalIonIonInelasticPhysic.hh
		GmPhysicsDecay.hh
		GmPhysicsXrayRefraction.hh
		GmPhysicsOpticalPhoton.hh
		GmPhysicsElectronNuclear.hh
		HadrontherapyPhysics.hh
		GmPhysicsFactory.hh
		GmPhysicsCoulombScattering.hh
		GmVPhysicsGamma.hh
		GmExtraPhysicsMessenger.hh
		GmAcollinearEplusAnnihilation.hh
		GmPhysicsVerbosity.hh
		GmXrayRefraction.hh
		GmPhysicsRadioactiveDecay.hh
		GmPhysicsCerenkov.hh
		GmPhysicsMultipleScattering.hh
		GmPhysicsPositronNuclear.hh
		HadrontherapyStepMaxMessenger.hh
	 SOURCES
		GmPhysicsGammaLowEner.cc
		GmPhysicsElectronStandard.cc
		GmEMStandardPhysics.cc
		GmPhysicsXrayRefraction.cc
		GmEMStandardPhysicsMessenger.cc
		GmVPhysicsElectron.cc
		HadrontherapyStepMaxMessenger.cc
		GmExtraPhysicsMessenger.cc
		GmPhysicsVerbosity.cc
		HadrontherapyPhysics.cc
		GmPhysicsGammaStandard.cc
		GmPhysicsPositronStandard.cc
		GmPhysicsElectronNuclear.cc
		GmPhysicsPositronPenelope.cc
		GmEMPhysics.cc
		GmXrayRefraction.cc
		GmPhysicsElectronPenelope.cc
		GmPhysicsOpticalPhoton.cc
		GmPhysicsCoulombScattering.cc
		HadrontherapyPhysicsMessenger.cc
		GmPhysicsPositronNuclear.cc
		GmPhysicsElectronLowEner.cc
		GmPhysicsMuonStandard.cc
		GmPhysicsCerenkov.cc
		GmPhysicsGammaNuclear.cc
		GmAcollinearEplusAnnihilation.cc
		GmPhysicsGammaPenelope.cc
		GmPhysicsParallel.cc
		GmVPhysicsGamma.cc
		HadrontherapyStepMax.cc
		GmPhysicsMultipleScattering.cc
		LocalIonIonInelasticPhysic.cc
		GmEMExtendedPhysics.cc
		plugin.cc
		GmPhysicsDecay.cc
		GmPhysicsRadioactiveDecay.cc
		GmPhysicsFactory.cc
		GmEMPhysicsMessenger.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base

	LINK_LIBRARIES
)
