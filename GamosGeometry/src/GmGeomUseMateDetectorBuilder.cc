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
#include "GmGeomUseMateDetectorBuilder.hh"
#include "GmGeomTextLineProcessor.hh"
#include "GmOpticalPropertiesMgr.hh"
#include "GmRegionCutsMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4tgrFileReader.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4tgbVolumeMgr.hh"

//---------------------------------------------------------------------
GmGeomUseMateDetectorBuilder::GmGeomUseMateDetectorBuilder() : G4tgbDetectorBuilder()
{
}


//---------------------------------------------------------------------
const G4tgrVolume* GmGeomUseMateDetectorBuilder::ReadDetector()
{
  //------------------------------------------------ 
  // Get the file to include (containing description of materials)
  //------------------------------------------------ 
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmGeometryUseMaterials:FileName", "");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->AddTextFile(filename);

  //------------------- construct g4 geometry
  GmGeomTextLineProcessor* tlproc = new GmGeomTextLineProcessor;
  G4tgrFileReader* tfr = G4tgrFileReader::GetInstance();
  tfr->SetLineProcessor( tlproc );
  if( filename != "" ) tfr->ReadFiles();

  //--------- Get the list of materials
  std::vector<G4String> materials;
  materials = GmParameterMgr::GetInstance()->GetVStringValue("GmGeometryUseMaterials:Materials", materials);
  if( materials.size() == 0 ) {
    G4Exception("GmGeomUseMateDetectorBuilder",
		"No material defined",
		FatalErrorInArgument,
		"Use parameter /gamos/setParam GmGeometryUseMaterials:Materials MATE_1 MATE_2 ...");
  }
  
  //----- Build a subworld for each material
  //---------- Build top volume  
  G4String worldSize = GmParameterMgr::GetInstance()->GetStringValue("GmGeometryUseMaterials:WorldSize", "1.E9");
  std::vector<G4String> wlV;
  wlV.push_back(":VOLU");
  wlV.push_back("world");
  wlV.push_back("BOX");
  wlV.push_back(worldSize);
  wlV.push_back(worldSize);
  wlV.push_back(worldSize);
  wlV.push_back(materials[0]);
  tlproc->ProcessLine( wlV );
  
  std::vector<G4String> wlR;
  wlR.push_back(":ROTM");
  wlR.push_back("UseMateRM0");
  wlR.push_back("0");
  wlR.push_back("0");
  wlR.push_back("0");
  tlproc->ProcessLine( wlR );
  
  for( unsigned int ii = 1; ii < materials.size(); ii++ ){
    wlV[1] = "subworld"+GmGenUtils::itoa(ii);
    wlV[3] = "1.";
    wlV[4] = "1.";
    wlV[5] = "1.";
    wlV[6] = materials[ii];
    tlproc->ProcessLine( wlV );

    std::vector<G4String> wlP;
    wlP.push_back(":PLACE");
    wlP.push_back("subworld"+GmGenUtils::itoa(ii));
    wlP.push_back("1");
    wlP.push_back("world");
    wlP.push_back("UseMateRM0");
    wlP.push_back("0");
    wlP.push_back("0");
    wlP.push_back("3*"+GmGenUtils::itoa(ii));
    tlproc->ProcessLine( wlP );

  }

  //---------- Get top volume
  G4tgrVolumeMgr* tgrVolmgr = G4tgrVolumeMgr::GetInstance();
  const G4tgrVolume* tgrVoltop = tgrVolmgr->GetTopVolume();  
  return tgrVoltop;
}

