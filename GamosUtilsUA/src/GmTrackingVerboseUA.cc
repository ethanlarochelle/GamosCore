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
#include "GmTrackingVerboseUA.hh"

#include "G4UserTrackingAction.hh"

#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"

#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4TrackingManager.hh"
#include "G4EventManager.hh"

//---------------------------------------------------------------------------
GmTrackingVerboseUA::GmTrackingVerboseUA()
{

}


//---------------------------------------------------------------------------
void GmTrackingVerboseUA::BeginOfRunAction( const G4Run* )
{
  //----- Set which events are verbose
  fTVEventMin = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EventMin",0));
  fTVEventMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EventMax",1E8));
  fTVEventStep = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EventStep",1));

  //----- Set which tracks of those events are verbose
  fTVTrackMin = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":TrackMin",0));
  fTVTrackMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":TrackMax",1E8));
  fTVTrackStep = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":TrackStep",1));

  //----- Set the verbosity level
  fVerboseLevel = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":VerboseLevel",1));

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << "TV: fTVTrackMin " <<  fTVTrackMin << " fTVTrackMax " <<  fTVTrackMax <<  " fTVTrackStep " << fTVTrackStep << " fTVEventMin " << fTVEventMin << " fTVEventMax " << fTVEventMax << " fTVEventStep " << fTVEventStep << " fVerboseLevel " << fVerboseLevel << G4endl;
#endif

  //  G4UserTrackingAction * ta = dynamic_cast<G4UserTrackingAction*>(G4EventManager::GetEventManager()->GetUserTrackingAction());
  theTrackingManager = G4EventManager::GetEventManager()->GetTrackingManager();
	 //ta->fpTrackingManager;
}

//----------------------------------------------------------------
void GmTrackingVerboseUA::BeginOfEventAction( const G4Event* anEvent )
{
  //----------- Set /tracking/verbose for this event 
  G4int eventNo = anEvent->GetEventID();

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << "TV: trackID: NEW EVENT " << eventNo << G4endl;
#endif

  fTkVerbThisEventON = false;
  //----- Check if event is in the selected range
  G4bool trackingVerboseThisEvent = false;
  if( eventNo >= fTVEventMin && eventNo <= fTVEventMax ) {
    if( (eventNo-fTVEventMin) % fTVEventStep == 0 ) {
      fTkVerbThisEventON = true;
      trackingVerboseThisEvent = true;
    }
  }


#ifndef GAMOS_NO_VERBOSE
 if( UtilsUAVerb(debugVerb) )  G4cout << " TV: fTkVerbThisEventON " <<  fTkVerbThisEventON << " fTrackingVerboseON " << fTrackingVerboseON << " fTVEventMin " << fTVEventMin << " fTVEventMax " << fTVEventMax << G4endl;
#endif
  //----- check if verbosity has to be changed
  if( fTkVerbThisEventON && !fTrackingVerboseON ) {
    SetTrackingVerbose( fVerboseLevel );
    fTrackingVerboseON = 1;
#ifndef GAMOS_NO_VERBOSE
 if( UtilsUAVerb(debugVerb) ) G4cout << "TV: VERBOSEet1 " << eventNo << G4endl;
#endif
  } else if( trackingVerboseThisEvent == 0 && fTrackingVerboseON == 1 ) {
    SetTrackingVerbose( 0 );
    fTrackingVerboseON = 0;
#ifndef GAMOS_NO_VERBOSE
 if( UtilsUAVerb(debugVerb) ) G4cout << "TV: VERBOSEet0 " << eventNo << G4endl;
#endif
  }

}

//----------------------------------------------------------------
void GmTrackingVerboseUA::PreUserTrackingAction(const G4Track* aTrack)
{
  //---------- Set /tracking/verbose
  //----- track is verbose only if event is verbose
  if( fTkVerbThisEventON ) {
    G4int trackNo = aTrack->GetTrackID();
    
    G4bool trackingVerboseThisTrack = false;
    //----- Check if track is in the selected range
    if( trackNo >= fTVTrackMin && trackNo <= fTVTrackMax ) {
      if( (trackNo-fTVTrackMin) % fTVTrackStep == 0 ) {
	trackingVerboseThisTrack = true;
      }
    }
    
    //----- Set the /tracking/verbose for this track 
    if( trackingVerboseThisTrack == 1 && fTrackingVerboseON == 0 ) {
      SetTrackingVerbose( fVerboseLevel );
      fTrackingVerboseON = 1;
#ifndef GAMOS_NO_VERBOSE
 if( UtilsUAVerb(debugVerb) ) G4cout << "TV: VERBOSEtt1 " << trackNo << G4endl;
#endif
    } else if( !trackingVerboseThisTrack && ( fTrackingVerboseON ) ) {
    SetTrackingVerbose( 0 );
      fTrackingVerboseON = 0;
#ifndef GAMOS_NO_VERBOSE
      if( UtilsUAVerb(debugVerb) ) G4cout << "TV: VERBOSEtt0 " << trackNo << G4endl;
#endif
    }
  }

}

//----------------------------------------------------------------
void GmTrackingVerboseUA::SetTrackingVerbose( int verblev )
{
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << " setting verbose level " << verblev <<G4endl;
#endif
  if (theTrackingManager!=0) theTrackingManager->SetVerboseLevel( verblev );
}
 
