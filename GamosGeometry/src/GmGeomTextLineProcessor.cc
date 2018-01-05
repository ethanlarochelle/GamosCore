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
#include "GmGeomTextLineProcessor.hh"
#include "GmRegionCutsMgr.hh"
#include "GmModuleJaws.hh"
#include "GmModuleMLC.hh"
#include "GmModuleRangeModulator.hh"
#include "GmOpticalPropertiesMgr.hh"

#include "G4tgrUtils.hh"

//---------------------------------------------------------------
GmGeomTextLineProcessor::GmGeomTextLineProcessor() : G4tgrLineProcessor()
{
}


//---------------------------------------------------------------
G4bool GmGeomTextLineProcessor::ProcessLine( const std::vector<G4String>& wl )
{

  G4bool iret = G4tgrLineProcessor::ProcessLine( wl );

  G4String wl0 = wl[0];
  for( unsigned int ii = 0; ii < wl0.length(); ii++ ){
    wl0[ii] = toupper( wl0[ii] );
  }

  if( !iret ) {
    iret = 1; 
    //------------------------------- parameter number
    if( wl0 == ":REGION" ) {
      std::vector<G4String>::const_iterator ite = wl.begin()+1;
      std::vector<G4String> wlc;
      for( ; ite != wl.end(); ite++ ) { //loop skipping the first one
	wlc.push_back( *ite );
      }
      //      wlc = wlc.erase( wlc.begin() );
      GmRegionCutsMgr::GetInstance()->AddRegionData( wlc );

    } else if( wl0 == ":CUTS" ) {
      std::vector<G4String>::const_iterator ite = wl.begin()+1;
      std::vector<G4String> wlc;
      for( ; ite != wl.end(); ite++ ) { //loop skipping the first one
	wlc.push_back( *ite );
      }
      GmRegionCutsMgr::GetInstance()->AddRegionCuts( wlc );
    } else if( wl0 == ":MODULE" ) {
      GmVModule* module = 0;
      if( wl[1] == "JAWS" ) {
	module = new GmModuleJaws( wl );
      } else if( wl[1] == "MLC" ) {
	module = new GmModuleMLC( wl );
      } else if( wl[1] == "RANGE_MODULATOR" ) {
	module = new GmModuleRangeModulator( wl );
      } else {
	G4Exception("GmGeomTextLineProcessor::ProcessLine",
		    "Wrong argument",
		    FatalException,
		    ":MODULE argument should be JAWS, MLC or RANGE_MODULATOR ");
      } 
      module->SetLineProcessor(this);
      module->BuildObjects();
    } else if( wl0 == ":MATE_PROPERTIES_TABLE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_EQ, " :MATE_PROPERTIES_TABLE");
      GmOpticalPropertiesMgr::GetInstance()->CreateMaterialPropertiesTable( wl );
    } else if( wl0 == ":MATEPT_ADD_ENERGIES" ) {
      G4tgrUtils::CheckWLsize( wl, 3, WLSIZE_GE, " :MATEPT_ADD_ENERGIES");
      GmOpticalPropertiesMgr::GetInstance()->AddEnergiesToTable( wl );
    } else if( wl0 == ":MATEPT_ADD_PROPERTY" ) {
      G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_GE, " :MATEPT_ADD_PROPERTY");
      GmOpticalPropertiesMgr::GetInstance()->AddPropertyToTable( wl );
    } else if( wl0 == ":MATEPT_ADD_CONST_PROPERTY" ) {
      G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_EQ, " :MATEPT_ADD_CONST_PROPERTY");
      GmOpticalPropertiesMgr::GetInstance()->AddConstPropertyToTable( wl );
    } else if( wl0 == ":MATEPT_ATTACH_TO_MATERIAL" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_MATERIAL");
      GmOpticalPropertiesMgr::GetInstance()->AttachTableToMaterial( wl );
    } else if( wl0 == ":MATEPT_ATTACH_TO_OPTICAL_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_OPTICAL_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->AttachTableToOpsurface( wl );  
    } else if( wl0 == ":OPTICAL_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :OPTICAL_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateOpticalSurface( wl );
    } else if( wl0 == ":LOGICAL_BORDER_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_BORDER_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateLogicalBorderSurface( wl );
    } else if( wl0 == ":LOGICAL_SKIN_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_SKIN_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateLogicalSkinSurface( wl );
    } else {
      iret = 0; 
    } 
  }
  
  return iret;
  
}
