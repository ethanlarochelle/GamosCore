//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "Reflex/PluginService.h"

#include "GmGenerator.hh"
#include "GmGeneratorFromTextFile.hh"
#include "GmGeneratorFromBinFile.hh"
#include "GmGeneratorChangeEnergyAndMaterial.hh"

PLUGINSVC_FACTORY (GmGenerator,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorFromTextFile,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorFromBinFile,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY (GmGeneratorChangeEnergyAndMaterial,G4VUserPrimaryGeneratorAction*())

#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyBetaDecay.hh"
#include "GmGenerDistEnergyConstantIsotopeDecay.hh"
#include "GmGenerDistEnergyRandomFlat.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistTimeDecay.hh"
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
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistDirectionGaussian.hh"
#include "GmGenerDistPositionDirectionInVolumeSurface.hh"
#include "GmGenerDistPositionVoxelPhantomMaterials.hh"
#include "GmGenerDistPositionInVoxelsFromFile.hh"
#include "GmGenerDistPositionDirectionTowardsBox.hh"
#include "GmGenerDistEnergyIsolethargical.hh"
#include "GmGenerDistEnergyMCNPisolethargical.hh"

PLUGINSVC_FACTORY(GmGenerDistEnergyConstant,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyBetaDecay,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyConstantIsotopeDecay,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyRandomFlat,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyGaussian,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyFromFile,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyIsolethargical,GmVGenerDistEnergy*())
PLUGINSVC_FACTORY(GmGenerDistEnergyMCNPisolethargical,GmVGenerDistEnergy*())

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
PLUGINSVC_FACTORY(GmGenerDistPositionVoxelPhantomMaterials,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInVoxelsFromFile,GmVGenerDistPosition*())

PLUGINSVC_FACTORY(GmGenerDistDirectionRandom,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionConst,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionCone,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistDirectionGaussian,GmVGenerDistDirection*())

PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInVolumeSurface,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionInVolumeSurface,GmVGenerDistDirection*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionTowardsBox,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionDirectionTowardsBox,GmVGenerDistDirection*())


#include "GmGenerVerbosity.hh"
PLUGINSVC_FACTORY(GmGenerVerbosity,GmVVerbosity*())

