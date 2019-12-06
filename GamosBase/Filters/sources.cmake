#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosBase_Filters
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosBase_Filters
	 HEADERS
		GmEndPhysicalVolumeFilter.hh
		GmStartLogicalVolumeFilter.hh
		GmHistoryOrAncestorsFilter.hh
		GmVParallelVolumeConditionFilter.icc
		GmStepNumberFilter.hh
		GmEnterParallelLogicalVolumeFilter.hh
		GmPositionZFilter.hh
		GmProdCutAllProcOutsideVoxelFilter.hh
		GmTraverseLogicalVolumeFilter.hh
		GmPositionXFilter.hh
		GmStartTouchableFilter.hh
		GmExitParallelPhysicalVolumeFilter.hh
		GmEndLogicalVolumeFilter.hh
		GmExitRegionChildrenFilter.hh
		GmGeomConditionTraverse.hh
		GmInTouchableChildrenFilter.hh
		GmPositionPreXFilter.hh
		GmEnergyChangeFilter.hh
		GmGeomConditionInParallel.hh
		GmMaterialFilter.hh
		GmInRegionFilter.hh
		GmEnterRegionFilter.hh
		GmExitPhysicalVolumeReplicatedFilter.hh
		GmProcessFilter.hh
		GmEMParticleFilter.hh
		GmInMassPhysicalVolumeReplicatedFilter.hh
		GmEnterLogicalVolumeChildrenFilter.hh
		GmGammaFilter.hh
		GmHistoryAllFilter.hh
		GmPrimaryOnAllSecondariesFilter.hh
		GmInPhysicalVolumeReplicatedFilter.hh
		GmTouchableFilterPhysicalVolumeReplicatedChildren.hh
		GmExitPhysicalVolumeChildrenFilter.hh
		GmEndParallelLogicalVolumeFilter.hh
		GmFilterVerbosity.hh
		GmDirectionPrePhiFilter.hh
		GmNumberOfSecondariesFilter.hh
		GmVVolumeConditionFilter.hh
		GmTraversePhysicalVolumeChildrenReplicatedFilter.hh
		GmEndPhysicalVolumeReplicatedFilter.hh
		GmExitParallelPhysicalVolumeReplicatedFilter.hh
		GmAncestorsFilter.hh
		GmVFilterOfFilters.hh
		GmHistoryAncestorsAllFilter.hh
		GmInteractionFilter.hh
		GmStartTouchableChildrenFilter.hh
		GmExitParallelLogicalVolumeFilter.hh
		GmEndParallelPhysicalVolumeReplicatedFilter.hh
		GmTraverseRegionFilter.hh
		GmInMassPhysicalVolumeFilter.hh
		GmPastHistoryFilter.hh
		GmStartPhysicalVolumeFilter.hh
		GmEnterPhysicalVolumeFilter.hh
		GmVParallelVolumeConditionFilter.hh
		GmStartParallelPhysicalVolumeReplicatedFilter.hh
		GmEndPhysicalVolumeChildrenFilter.hh
		GmStartParallelRegionFilter.hh
		GmVertexKineticEnergyFilter.hh
		GmDirectionPreThetaFilter.hh
		GmGeomConditionStartParallel.hh
		GmTraverseParallelPhysicalVolumeReplicatedFilter.hh
		GmORFilter.hh
		GmTouchableFilterLogicalVolumeChildren.hh
		GmInParallelRegionFilter.hh
		GmProtonFilter.hh
		GmGeomConditionStart.hh
		GmNeutralFilter.hh
		GmPhysicalVolumeFilter.hh
		GmTouchableFilterPhysicalVolume.hh
		GmVTouchableFilterVolume.hh
		GmVParallelGeomCondition.hh
		GmProdCutOutsideVoxelFilter.hh
		GmCreatorProcessFilter.hh
		GmPrimaryOn1SecondaryFilter.hh
		GmGeomConditionEnterParallel.hh
		GmTraverseParallelLogicalVolumeFilter.hh
		GmStartParallelLogicalVolumeFilter.hh
		GmExitTouchableFilter.hh
		GmSameLogicalVolumeFilter.hh
		GmOnSecondaryFilter.hh
		GmStartPhysicalVolumeReplicatedFilter.hh
		GmElectronFilter.hh
		GmInPhysicalVolumeFilter.hh
		GmEnterParallelPhysicalVolumeFilter.hh
		GmGeomConditionIn.hh
		GmSecondaryFilter.hh
		GmInPhysicalVolumeChildrenReplicatedFilter.hh
		GmMinRangeCutOutsideVoxelFilter.hh
		GmEndLogicalVolumeChildrenFilter.hh
		GmTraversePhysicalVolumeReplicatedFilter.hh
		GmEndRegionFilter.hh
		GmTraverseTouchableChildrenFilter.hh
		GmTraversePhysicalVolumeChildrenFilter.hh
		GmParticleProcessFilter.hh
		GmPositionPreYFilter.hh
		GmInPhysicalVolumeChildrenFilter.hh
		GmExitPhysicalVolumeFilter.hh
		GmInTouchableFilter.hh
		GmPositronFilter.hh
		GmTraverseParallelPhysicalVolumeFilter.hh
		GmXORFilter.hh
		GmANDFilter.hh
		GmEnterTouchableChildrenFilter.hh
		GmHistoryOrAncestorsFilterForScorer.hh
		GmEnterPhysicalVolumeChildrenFilter.hh
		GmEndPhysicalVolumeChildrenReplicatedFilter.hh
		GmEnterLogicalVolumeFilter.hh
		GmStartPhysicalVolumeChildrenReplicatedFilter.hh
		GmStartRegionChildrenFilter.hh
		GmEndParallelPhysicalVolumeFilter.hh
		GmInLogicalVolumeChildrenFilter.hh
		GmFilterFromClassifier.hh
		GmPositionPreZFilter.hh
		GmStartRegionFilter.hh
		GmParticleFilter.hh
		GmEnterRegionChildrenFilter.hh
		GmGeomConditionEnter.hh
		GmVProdCutOutsideVoxelFilter.hh
		GmEnterPhysicalVolumeChildrenReplicatedFilter.hh
		GmTraverseRegionChildrenFilter.hh
		GmInParallelPhysicalVolumeReplicatedFilter.hh
		GmGeomConditionInMass.hh
		GmExitLogicalVolumeChildrenFilter.hh
		GmExitParallelRegionFilter.hh
		GmVVolumeConditionFilter.icc
		GmVANDFilter.hh
		GmInverseFilter.hh
		GmKineticEnergyFilter.hh
		GmTouchableFilterTouchableChildren.hh
		GmGeomConditionTraverseParallel.hh
		GmTouchableFilterTouchable.hh
		GmDepositedEnergyFilter.hh
		GmTraverseLogicalVolumeChildrenFilter.hh
		GmEnterTouchableFilter.hh
		GmTraverseTouchableFilter.hh
		GmParentFilter.hh
		GmExitTouchableChildrenFilter.hh
		GmEndRegionChildrenFilter.hh
		GmEnterParallelPhysicalVolumeReplicatedFilter.hh
		GmStartParallelPhysicalVolumeFilter.hh
		GmExitLogicalVolumeFilter.hh
		GmExitRegionFilter.hh
		GmPositionYFilter.hh
		GmGeomConditionExitParallel.hh
		GmTouchableFilterPhysicalVolumeChildren.hh
		GmEndTouchableFilter.hh
		GmPostKineticEnergyFilter.hh
		GmGeomConditionExit.hh
		GmDirectionPhiFilter.hh
		GmVGeomCondition.hh
		GmChargedFilter.hh
		GmEndParallelRegionFilter.hh
		GmExitPhysicalVolumeChildrenReplicatedFilter.hh
		GmInMassLogicalVolumeFilter.hh
		GmEnterParallelRegionFilter.hh
		GmGeomConditionEnd.hh
		GmGeomConditionEndParallel.hh
		GmStartLogicalVolumeChildrenFilter.hh
		GmElectronOrPositronFilter.hh
		GmInParallelPhysicalVolumeFilter.hh
		GmTouchableFilterLogicalVolume.hh
		GmInMassRegionFilter.hh
		GmStartPhysicalVolumeChildrenFilter.hh
		GmHistoryFilter.hh
		GmEnterPhysicalVolumeReplicatedFilter.hh
		GmDirectionThetaFilter.hh
		GmTraverseParallelRegionFilter.hh
		GmTouchableFilterRegionChildren.hh
		GmDifferentLogicalVolumeFilter.hh
		GmDeltaEnergyFilter.hh
		GmPrimaryFilter.hh
		GmTraversePhysicalVolumeFilter.hh
		GmInLogicalVolumeFilter.hh
		GmInParallelLogicalVolumeFilter.hh
		GmTouchableFilterRegion.hh
		GmTouchableFilterPhysicalVolumeReplicated.hh
		GmEndTouchableChildrenFilter.hh
		GmNeutronFilter.hh
		GmInRegionChildrenFilter.hh
		GmVolumeFilters.hh
	 SOURCES
		GmInMassRegionFilter.cc
		GmAncestorsFilter.cc
		GmPositronFilter.cc
		GmTouchableFilterRegionChildren.cc
		GmPastHistoryFilter.cc
		GmGeomConditionInParallel.cc
		GmTouchableFilterTouchableChildren.cc
		GmProcessFilter.cc
		GmStepNumberFilter.cc
		GmGeomConditionIn.cc
		GmHistoryOrAncestorsFilterForScorer.cc
		GmPositionPreZFilter.cc
		GmVertexKineticEnergyFilter.cc
		GmNumberOfSecondariesFilter.cc
		GmPositionXFilter.cc
		GmGeomConditionStartParallel.cc
		GmPhysicalVolumeFilter.cc
		GmInMassPhysicalVolumeReplicatedFilter.cc
		GmDeltaEnergyFilter.cc
		GmProdCutOutsideVoxelFilter.cc
		GmGeomConditionEndParallel.cc
		GmPrimaryOnAllSecondariesFilter.cc
		GmInverseFilter.cc
		GmTouchableFilterRegion.cc
		GmSecondaryFilter.cc
		GmFilterFromClassifier.cc
		GmDirectionPrePhiFilter.cc
		GmVFilterOfFilters.cc
		GmFilterVerbosity.cc
		GmGeomConditionTraverseParallel.cc
		GmInMassLogicalVolumeFilter.cc
		GmMaterialFilter.cc
		GmGeomConditionTraverse.cc
		GmGeomConditionExitParallel.cc
		GmOnSecondaryFilter.cc
		GmParentFilter.cc
		GmPrimaryOn1SecondaryFilter.cc
		GmORFilter.cc
		GmGeomConditionStart.cc
		GmKineticEnergyFilter.cc
		GmTouchableFilterPhysicalVolumeReplicatedChildren.cc
		GmTouchableFilterPhysicalVolumeReplicated.cc
		GmElectronOrPositronFilter.cc
		GmVParallelGeomCondition.cc
		GmANDFilter.cc
		GmParticleProcessFilter.cc
		GmSameLogicalVolumeFilter.cc
		GmPostKineticEnergyFilter.cc
		GmDifferentLogicalVolumeFilter.cc
		GmPositionYFilter.cc
		GmMinRangeCutOutsideVoxelFilter.cc
		GmDirectionPhiFilter.cc
		GmElectronFilter.cc
		GmGeomConditionExit.cc
		GmEMParticleFilter.cc
		GmTouchableFilterPhysicalVolume.cc
		GmInMassPhysicalVolumeFilter.cc
		GmTouchableFilterLogicalVolumeChildren.cc
		GmHistoryAncestorsAllFilter.cc
		GmCreatorProcessFilter.cc
		GmHistoryFilter.cc
		GmProtonFilter.cc
		GmXORFilter.cc
		GmGeomConditionInMass.cc
		GmVProdCutOutsideVoxelFilter.cc
		GmPositionZFilter.cc
		GmPositionPreXFilter.cc
		GmGammaFilter.cc
		GmDirectionPreThetaFilter.cc
		GmTouchableFilterLogicalVolume.cc
		GmVANDFilter.cc
		GmNeutralFilter.cc
		GmHistoryOrAncestorsFilter.cc
		GmProdCutAllProcOutsideVoxelFilter.cc
		GmPositionPreYFilter.cc
		GmTouchableFilterPhysicalVolumeChildren.cc
		GmEnergyChangeFilter.cc
		GmInteractionFilter.cc
		GmNeutronFilter.cc
		GmGeomConditionEnter.cc
		GmGeomConditionEnd.cc
		GmParticleFilter.cc
		GmGeomConditionEnterParallel.cc
		GmDirectionThetaFilter.cc
		GmPrimaryFilter.cc
		GmVGeomCondition.cc
		plugin.cc
		GmHistoryAllFilter.cc
		GmChargedFilter.cc
		GmDepositedEnergyFilter.cc
		GmTouchableFilterTouchable.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosBase_Classifiers
	GamosCore_GamosUserActionMgr

	LINK_LIBRARIES
)