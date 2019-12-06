#include "GmGenerator.hh"
#include "GmGeneratorFromTextFile.hh"
#include "GmGeneratorFromBinFile.hh"
#include "GmGeneratorChangeEnergyAndMaterial.hh"
#include "GmGeneratorScanVis.hh"

#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyBetaDecay.hh"
#include "GmGenerDistEnergyConstantIsotopeDecay.hh"
#include "GmGenerDistEnergyRandomFlat.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistTimeDecay.hh"
#include "GmGenerDistWavelengthFromFile.hh"
#include "GmGenerDistWavelengthRandomFlat.hh"
#include "GmGenerDistTimeConstantChange.hh"
#include "GmGenerDistPositionPoint.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmGenerDistPositionInG4Volumes.hh"
#include "GmGenerDistPositionInUserVolumes.hh"
#include "GmGenerDistPositionInG4Surfaces.hh"
#include "GmGenerDistPositionInUserSurfaces.hh"
#include "GmGenerDistPositionInG4VolumesGeneral.hh"
#include "GmGenerDistPositionLineSteps.hh"
#include "GmGenerDistPositionSquare.hh"
#include "GmGenerDistPositionRectangle.hh"
#include "GmGenerDistPositionDisc.hh"
#include "GmGenerDistPositionDiscGaussian.hh"
#include "GmGenerDistPositionDiscFromFile.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistDirectionCone2D.hh"
#include "GmGenerDistDirectionConeGaussian.hh"
#include "GmGenerDistDirectionCone2DGaussian.hh"
#include "GmGenerDistDirectionGaussian.hh"
#include "GmGenerDistDirectionThetaFromFile.hh"
#include "GmGenerDistPositionDirectionInVolumeSurface.hh"
#include "GmGenerDistPositionVoxelPhantomMaterials.hh"
#include "GmGenerDistPositionInVoxelsFromFile.hh"
#include "GmGenerDistPositionDirectionTowardsBox.hh"
#include "GmGenerDistPositionDirectionInUserSurfaces.hh"
#include "GmGenerDistEnergyIsolethargical.hh"
#include "GmGenerDistEnergyMCNPisolethargical.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY (GmGenerator,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorFromTextFile,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorFromBinFile,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorChangeEnergyAndMaterial,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorScanVis,G4VUserPrimaryGeneratorAction*())

PLUGINSVC_FACTORY(GmGenerDistEnergyConstant,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyBetaDecay,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyConstantIsotopeDecay,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyRandomFlat,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyGaussian,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyFromFile,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyIsolethargical,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyMCNPisolethargical,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistWavelengthFromFile,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistWavelengthRandomFlat,GmVGenerDistEnergy*())

PLUGINSVC_FACTORY(GmGenerDistTimeConstant,GmVGenerDistTime*())
PLUGINSVC_FACTORY(GmGenerDistTimeDecay,GmVGenerDistTime*())
PLUGINSVC_FACTORY(GmGenerDistTimeConstantChange,GmVGenerDistTime*())

PLUGINSVC_FACTORY(GmGenerDistPositionPoint,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInG4VolumesGeneral,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInG4Volumes,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInUserVolumes,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInG4Surfaces,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInUserSurfaces,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionLineSteps,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionSquare,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionRectangle,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDisc,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDiscGaussian,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDiscFromFile,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionVoxelPhantomMaterials,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInVoxelsFromFile,GmVGenerDistPosition*())

PLUGINSVC_FACTORY(GmGenerDistDirectionRandom,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionThetaFromFile,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionConst,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionCone,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionCone2D,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionConeGaussian,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionCone2DGaussian,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionGaussian,GmVGenerDistDirection*())

PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInVolumeSurface,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInVolumeSurface,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionTowardsBox,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionTowardsBox,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInUserSurfaces,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInUserSurfaces,GmVGenerDistDirection*())


#include "GmGenerVerbosity.hh"
PLUGINSVC_FACTORY(GmGenerVerbosity,GmVVerbosity*())

#else 

#include "GmGeneratorFactory.hh"
#include "GmGeneratorDistributionFactories.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_GENERATOR (GmGenerator);
DEFINE_GAMOS_GENERATOR (GmGeneratorFromTextFile);
DEFINE_GAMOS_GENERATOR (GmGeneratorFromBinFile);
DEFINE_GAMOS_GENERATOR (GmGeneratorChangeEnergyAndMaterial);
DEFINE_GAMOS_GENERATOR (GmGeneratorScanVis);

DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyConstant);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyBetaDecay);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyConstantIsotopeDecay);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyRandomFlat);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyGaussian);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyFromFile);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyIsolethargical);
DEFINE_GAMOS_GENER_DIST_ENERGY(GmGenerDistEnergyMCNPisolethargical);

DEFINE_GAMOS_GENER_DIST_TIME(GmGenerDistTimeConstant);
DEFINE_GAMOS_GENER_DIST_TIME(GmGenerDistTimeDecay);
DEFINE_GAMOS_GENER_DIST_TIME(GmGenerDistTimeConstantChange);

DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionPoint);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInG4VolumesGeneral);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInG4Volumes);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInUserVolumes);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInG4Surfaces);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInUserSurfaces);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionLineSteps);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionSquare);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionRectangle);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDisc);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDiscGaussian);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDiscFromFile);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionVoxelPhantomMaterials);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInVoxelsFromFile);

DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionRandom);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionThetaFromFile);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionConst);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionCone);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionCone2D);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionConeGaussian);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionCone2DGaussian);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistDirectionGaussian);

DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDirectionInVolumeSurface);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistPositionDirectionInVolumeSurface);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDirectionTowardsBox);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistPositionDirectionTowardsBox);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionDirectionInUserSurfaces);
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmGenerDistPositionDirectionInUserSurfaces);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmGenerVerbosity.hh"

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmGenerVerbosity, "GmGenerVerbosity");

#endif
