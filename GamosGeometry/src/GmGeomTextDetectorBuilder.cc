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
#include "GmGeomTextDetectorBuilder.hh"
#include "GmGeomTextLineProcessor.hh"
#include "GmOpticalPropertiesMgr.hh"
#include "GmRegionCutsMgr.hh"

#include "G4tgrFileReader.hh"
#include "G4tgrVolumeMgr.hh"

//---------------------------------------------------------------------
GmGeomTextDetectorBuilder::GmGeomTextDetectorBuilder() : G4tgbDetectorBuilder()
{
}


//---------------------------------------------------------------------
const G4tgrVolume* GmGeomTextDetectorBuilder::ReadDetector()
{
  //------------------- construct g4 geometry
  GmGeomTextLineProcessor* tlproc = new GmGeomTextLineProcessor;
  G4tgrFileReader* tfr = G4tgrFileReader::GetInstance();
  tfr->SetLineProcessor( tlproc );
  tfr->ReadFiles();
  //---------- find top G4tgrVolume 
  G4tgrVolumeMgr* tgrVolmgr = G4tgrVolumeMgr::GetInstance();
  const G4tgrVolume* tgrVoltop = tgrVolmgr->GetTopVolume();  
  return tgrVoltop;
}


//---------------------------------------------------------------------
G4VPhysicalVolume* GmGeomTextDetectorBuilder::ConstructDetectorGAMOS( const G4tgrVolume* tgrVoltop, G4int , G4bool bBuildRegionCuts )
{
  G4VPhysicalVolume* topPV = G4tgbDetectorBuilder::ConstructDetector( tgrVoltop );

  if( bBuildRegionCuts ) {
    //--- Create regions
    GmRegionCutsMgr::GetInstance()->BuildRegions();
    
    //--- Set cuts to regions
    GmRegionCutsMgr::GetInstance()->BuildProductionCuts();
  }

  GmOpticalPropertiesMgr::GetInstance()->BuildG4();

  return topPV;
}
