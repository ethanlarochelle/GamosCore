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
#include "GmMovementMgr.hh"
#include "GmVMovement.hh"
#include "GmMovementFromFile.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//----------------------------------------------------------------------
GmMovementMgr* GmMovementMgr::theInstance = 0;

//----------------------------------------------------------------------
GmMovementMgr* GmMovementMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmMovementMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmMovementMgr::GmMovementMgr()
{
  bApplyMovements = FALSE;

}

//-----------------------------------------------------------------------
std::vector<GmVMovement*> GmMovementMgr::GetNewMovements( G4int eventID, G4double time )
{
  std::vector<GmVMovement*> movementsToDo; 

  std::vector<GmVMovement*>::const_iterator ite;
  for( ite = theMovements.begin(); ite != theMovements.end(); ite++ ){
    if( (*ite)->GetType() == AfterNEvents ){
      if( (*ite)->IsNewMovement( eventID ) ) movementsToDo.push_back( *ite );
    } else if( (*ite)->GetType() == AfterTime ){
      if( (*ite)->IsNewMovement( time ) ) movementsToDo.push_back( *ite );
    }
  }

  if( movementsToDo.size() == 0 ) {
    bApplyMovements = FALSE;
  } else {
    bApplyMovements = TRUE;
  }

  return movementsToDo;
}

//----------------------------------------------------------------------
void GmMovementMgr::InitialiseLastValue()
{
  std::vector<GmVMovement*>::const_iterator ite;
  for( ite = theMovements.begin(); ite != theMovements.end(); ite++ ){
    (*ite)->InitialiseLastValue();
  }
}

//----------------------------------------------------------------------
void GmMovementMgr::ReadMovementFile( G4String fileName )
{
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4int iline = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() < 4 || wl.size() > 6 ) {   
      G4Exception("GmMovementMgr::ReadMovementFile",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(iline) 
			   + " All lines must have four (or optionally two more) words: N_MOVEMENTS VOLUME_NAME MOVEMENT_TYPE NEVENTS_INTERVAL OFFSET NUMBER_OF_INTERVALS").c_str());
    }
    iline++;
    G4int nMovements = GmGenUtils::GetInteger( wl[wl.size()-1] );
    wl.pop_back();
    GmMovementFromFile* mFF = new GmMovementFromFile( wl );
    for( G4int ii = 0; ii < nMovements; ii++ ){
       if(! fin.GetWordsInLine( wl ) ) { 
	 G4Exception("GGmMovementMgr::ReadMovementFile",
		  "Error in number of lines ",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(iline) 
			   + " Trying to read " + GmGenUtils::itoa(nMovements) + " but there are not enough lines in file").c_str());
       }
       if( wl.size() != 8 ) {   
	 G4Exception("GmMovementMgr::ReadMovementFile",
		     "Error in number of words in line",
		     FatalErrorInArgument,
		     G4String("Reading file " + fileName 
			      + ", line number " + GmGenUtils::itoa(iline) 
			      + " All lines must have eight words: DISP_AXIS_X DISP_AXIS_Y DISP_AXIS_Z DISP_VALUE  ROT_AXIS_X ROT_AXIS_Y ROT_AXIS_Z ROT_VALUE").c_str());
       }
       iline++;

       mFF->AddMovement( wl );
    }
  }

}


//-----------------------------------------------------------------------
GmMovementMgr::~GmMovementMgr()
{
}
