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
#include "GmGeometryFromText.hh"
#include <vector>
#include <map>
#include "GmGeomTextDetectorBuilder.hh"
#include "GmGeometryUtils.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "GmParallelWorldCreator.hh"
#include "G4tgrMessenger.hh"
#include "G4VUserParallelWorld.hh"

#include "TROOT.h"
#include "TPluginManager.h"
GmGeometryFromText::GmGeometryFromText()
{
  new G4tgrMessenger;
  gROOT->GetPluginManager()->AddHandler("G4VUserDetectorConstruction", "^geomtext:", "GmGeometryFromText",
			 "GamosCore_GamosGeometry", "GmGeometryFromText()");
}


//---------------------------------------------------------------------
G4VPhysicalVolume* GmGeometryFromText::Construct()
{
  //------------------------------------------------ 
  // Get the file names 
  //------------------------------------------------ 
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmGeometryFromText:FileName", "");
  
  if( filename == "" ) {
    G4Exception("GeometryFromText::Construct",
		"Wrong argument",
		FatalErrorInArgument,
		"filename is not defined, please use '/gamos/setParam GmGeometryFromText:FileName MU_FILE_NAME'");
  }

  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );

  //------------------------------------------------ 
  // Read the text files 
  //------------------------------------------------ 
  GmGeomTextDetectorBuilder* gtb = new GmGeomTextDetectorBuilder;
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->SetDetectorBuilder( gtb );
  volmgr->AddTextFile(filename);

  std::vector<G4String> fnamepar;
  fnamepar = GmParameterMgr::GetInstance()->GetVStringValue("GmGeometryFromText:FileNameParallel", fnamepar);
  if( fnamepar.size() != 0 ){
    if( fnamepar.size()%2 != 0 ){
      G4Exception("GmGeometryFromText::Construct",
		  "Error in number of arguments of parameter 'GmGeometryFromText:FileNameParallel'",
		  FatalException,
		  (G4String("There should be a multiple of two: FILE_NAME_1 PARALLEL_WORLD_NUMBER_1 FILE_NAME_2 PARALLEL_WORLD_NUMBER_2 ..., there are ")
		   +GmGenUtils::itoa(fnamepar.size())).c_str());
    }
    for( unsigned int ii = 0; ii < fnamepar.size(); ii+=2 ) {
      volmgr->AddTextFileParallel(fnamepar[ii],GmGenUtils::GetInteger(fnamepar[ii+1]));
    }

  }

  //-  const G4tgrVolume* tgrVoltop = volmgr->ReadDetector();
  //-  G4VPhysicalVolume* physiWorld = volmgr->ConstructDetector(tgrVoltop);
  const G4tgrVolume* tgrVoltop = gtb->ReadDetector();
 //------------------------------------------------ 
  // Construct the Geant4 parallel worlds
  //------------------------------------------------ 
  G4tgbParallelGeomMgr* parallelMgr = G4tgbParallelGeomMgr::GetInstance();
  parallelMgr->SetWorldCreator( new GmParallelWorldCreator );
  std::vector<G4VUserParallelWorld*> parallelWorlds = parallelMgr->CreateParalleWorlds();
  for( unsigned int ii = 0; ii < parallelWorlds.size(); ii++ ) {
    G4cout << " RegisterParallelWorld " << parallelWorlds[ii]->GetName() << G4endl;
    RegisterParallelWorld( parallelWorlds[ii] );  
  }

  //------------------------------------------------ 
  // Build the Geant4 mass geometry
  //------------------------------------------------ 

  G4VPhysicalVolume* physiWorld = gtb->ConstructDetectorGAMOS(tgrVoltop, -1, 1 );

  //  G4VPhysicalVolume* physiWorld = volmgr->ReadAndConstructDetector();

    return physiWorld;
}
