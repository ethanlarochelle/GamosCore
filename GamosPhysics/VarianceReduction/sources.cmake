#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_VarianceReduction
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_VarianceReduction

   HEADERS
       GmBSTrajectory.hh
       GmPSeIonisation.hh
       GmDeexSplitLivermorePhotoElectricModel.hh
       GmPhysicsPositronUBSStandard.hh
       GmDeexSpliteIonisation.hh
       GmUBSLivermoreBremsstrahlungModel.hh
       GmPSZPlaneDirCheckerAcceptAll.hh
       GmPSGammaConversion.hh
       GmSplitWrapperProcess.hh
       GmPSEMPhysics.hh
       GmPhysicsElectronLowEnerZBSDeexSplit.hh
       GmPhysicsElectronLowEnerDeexSplit.hh
       GmPSeplusAnnihilation.hh
       GmVDeexSplitEnergyLossProcess.hh
       GmDeexSplitPenelopeComptonModel.hh
       GmPhysicsElectronPenelopeDeexSplit.hh
       GmWrapperProcess.hh
       GmPhysicsGammaPenelopeDeexSplit.hh
       GmPSZPlaneDirChecker.hh
       GmPhysicsPositronZBSStandard.hh
       GmPhysicsGammaLowEnerDeexSplit.hh
       GmPhysicsElectronZBSLowEner.hh
       GmVarianceReductionMessenger.hh
       GmBSScanWeightsUA.hh
       GmImportanceSamplingProcess.hh
       GmDeexSplitPenelopePhotoElectricModel.hh
       GmBSControlHistosUA.hh
       GmPhysicsElectronUBSLowEner.hh
       GmPhysicsElectronLowEnerUBSDeexSplit.hh
       GmVPSModelHelper.hh
       GmInheritTrackInfoUA.hh
       GmVPSGammaProcess.hh
       GmPSPhotoElectricEffect.hh
       GmBSLivermoreBremsstrahlungModel.hh
       GmPSeBremsstrahlung.hh
       GmDeexSplitLivermoreComptonModel.hh
       GmPSComptonScattering.hh
       GmVPSEnergyLossProcess.hh

   SOURCES
       plugin.cc
       GmPSeBremsstrahlung.cc
       GmPSZPlaneDirCheckerAcceptAll.cc
       GmPhysicsElectronUBSLowEner.cc
       GmPhysicsElectronZBSLowEner.cc
       GmPSEMPhysics.cc
       GmWrapperProcess.cc
       GmPhysicsGammaLowEnerDeexSplit.cc
       GmPhysicsElectronPenelopeDeexSplit.cc
       GmVDeexSplitEnergyLossProcess.cc
       GmBSScanWeightsUA.cc
       GmVPSEnergyLossProcess.cc
       GmPSeIonisation.cc
       GmPhysicsElectronLowEnerDeexSplit.cc
       GmDeexSpliteIonisation.cc
       GmPhysicsElectronLowEnerUBSDeexSplit.cc
       GmVPSGammaProcess.cc
       GmSplitWrapperProcess.cc
       GmVarianceReductionMessenger.cc
       GmDeexSplitLivermorePhotoElectricModel.cc
       GmPSPhotoElectricEffect.cc
       GmPhysicsElectronLowEnerZBSDeexSplit.cc
       GmPhysicsPositronUBSStandard.cc
       GmPhysicsPositronZBSStandard.cc
       GmPSComptonScattering.cc
       GmBSControlHistosUA.cc
       GmPhysicsGammaPenelopeDeexSplit.cc
       GmPSZPlaneDirChecker.cc
       GmVPSModelHelper.cc
       GmBSTrajectory.cc
       GmDeexSplitPenelopeComptonModel.cc
       GmDeexSplitPenelopePhotoElectricModel.cc
       GmPSGammaConversion.cc
       GmDeexSplitLivermoreComptonModel.cc
       GmImportanceSamplingProcess.cc
       GmPSeplusAnnihilation.cc
       GmBSLivermoreBremsstrahlungModel.cc
       GmInheritTrackInfoUA.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosPhysics_PhysicsList
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base

   LINK_LIBRARIES
)

# List any source specific properties here
