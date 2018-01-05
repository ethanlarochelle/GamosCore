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

#include "GmReadPhantomG4Geometry.hh"
#include "GmReadPhantomG4BinGeometry.hh"
#include "GmReadPhantomEGSGeometry.hh"
#include "GmSimplePhantomGeometry.hh"
#include "GmReadPhantomEGSwithPSGeometry.hh"
#include "GmReadPhantomG4withPSGeometry.hh"
#include "GmReadPhantomPartialG4Geometry.hh"

PLUGINSVC_FACTORY(GmReadPhantomG4Geometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4BinGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmSimplePhantomGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSwithPSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4withPSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomPartialG4Geometry,G4VUserDetectorConstruction*())

#include "GmReadDICOMVerbosity.hh"
PLUGINSVC_FACTORY(GmReadDICOMVerbosity,GmVVerbosity*())

#include "GmBuildPhantomStructuresUA.hh"
PLUGINSVC_FACTORY(GmBuildPhantomStructuresUA,GmUserAction*())

#include "GmGenerDistPositionInPhantomVoxels.hh"
PLUGINSVC_FACTORY(GmGenerDistPositionInPhantomVoxels,GmVGenerDistPosition*())
#include "GmGenerDistPositionPETImage.hh"
PLUGINSVC_FACTORY(GmGenerDistPositionPETImage,GmVGenerDistPosition*())

#include "GmPhantomStructures.hh"
PLUGINSVC_FACTORY(GmEnterPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhantomStructureFilter,GmVFilter*(G4String))

