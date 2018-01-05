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

#include "GmSDSimple.hh"
#include "GmSDWithFilter.hh"
#include "GmSDSimpleExactPos.hh"
#include "GmSDVirtSegmentedSphereRThetaPhi.hh"
#include "GmSDVirtSegmentedSphereThetaPhi.hh"
#include "GmSDVirtSegmBox.hh"
#include "GmSDOpticalPhoton.hh"
#include "GmSDSeparateByTime.hh"

PLUGINSVC_FACTORY(GmSDSimple,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSimpleExactPos,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereRThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmBox,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDOpticalPhoton,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSeparateByTime,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDWithFilter,G4VSensitiveDetector*(G4String))

#include "GmRecHitBuilderByDistance.hh"
#include "GmRecHitBuilderByBlock.hh"
#include "GmRecHitBuilder1to1.hh"

PLUGINSVC_FACTORY(GmRecHitBuilderByDistance,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderByBlock,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilder1to1,GmVRecHitBuilder*())

#include "GmHitsHistosUA.hh"
#include "GmRecHitsHistosUA.hh"
#include "GmHitsWriteUA.hh"
#include "GmHitsReadUA.hh"
#include "GmRecHitsWriteUA.hh"
#include "GmHistosGammaAtSD.hh"

PLUGINSVC_FACTORY(GmHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsReadUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHistosGammaAtSD,GmUserAction*())

#include "GmSDVerbosity.hh"
PLUGINSVC_FACTORY(GmSDVerbosity,GmVVerbosity*())
