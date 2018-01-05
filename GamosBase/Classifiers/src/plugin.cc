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

#include "GmClassifierBy1Ancestor.hh"
#include "GmClassifierByAncestors.hh"
#include "GmClassifierByLogicalVolume.hh"
#include "GmClassifierByKineticEnergy.hh"
#include "GmClassifierByPhysicalVolume.hh"
#include "GmClassifierByPhysicalVolumeReplicated.hh"
#include "GmClassifierByRegion.hh"
#include "GmClassifierByParticle.hh"
#include "GmClassifierByPrimaryParticle.hh"
#include "GmClassifierByProcess.hh"
#include "GmClassifierByCreatorProcess.hh"
#include "GmClassifierByParticleProcess.hh"
#include "GmClassifierByMaterial.hh"
#include "GmCompoundClassifier.hh"
#include "GmClassifierBySecondariesList.hh"
#include "GmClassifierBySecondariesListExclude.hh"
#include "GmClassifierByHadronicReaction.hh"

PLUGINSVC_FACTORY(GmClassifierByAncestors,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBy1Ancestor,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByLogicalVolume,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByKineticEnergy,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPhysicalVolume,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPhysicalVolumeReplicated,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByRegion,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByParticle,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPrimaryParticle,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByCreatorProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByParticleProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByMaterial,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmCompoundClassifier,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBySecondariesList,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBySecondariesListExclude,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByHadronicReaction,GmVClassifier*(G4String))
