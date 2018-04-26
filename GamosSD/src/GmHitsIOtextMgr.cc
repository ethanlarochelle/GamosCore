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
#include "GmHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmEventTimeExtractorFromHit.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

//-----------------------------------------------------------------------
GmHitsIOtextMgr::GmHitsIOtextMgr()
{ 
  theOutputType = "hits.text";
  Init();
  OpenFileOut();
}

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
void GmHitsIOtextMgr::WriteHit(GmHit* hit)
{
  G4String sdtype =  "/" + GmParameterMgr::GetInstance()->GetStringValue("GmHitsIOtextMgr:SDType","") + "/";
  if( sdtype != "//" && sdtype != hit->GetSDType() ) return;

  if( bDebugOut ) hit->Print( G4cout, bDebugOut );
  hit->Print( *theFileOut, bDebugOut );
}


//-----------------------------------------------------------------------
void GmHitsIOtextMgr::ReadHits()
{
  if( theFileIn == 0 ) {
    G4Exception("GmHitsIOtextMgr::ReadHits",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileIn() first ");
  }

  if( theEventTimeExtractor == 0 ) {
    theEventTimeExtractor = new GmEventTimeExtractorFromHit();
    theHitsEventMgr->SetEventTimeExtractor(theEventTimeExtractor);
  }

  if( bLastHitRead ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(warningVerb) ) G4cerr << " GmHitsIOtextMgr::ReadHits : last hit read, run is being aborted " << G4endl;
#endif
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }

  G4int evtID = 0;
  if( theLastHit != 0 ) {
    theHitsEventMgr->AddHit(theLastHit,theLastHit->GetSDType());
    evtID = theLastHit->GetEventID();
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits SetEventTime = " << theLastHit->GetTimeMin() << G4endl;
#endif
    theEventTimeExtractor->SetEventTime( theLastHit->GetTimeMin() ); // this is only approximately the event time (= initial time for first particle, but it may serve)
    theHitsEventMgr->BuildEventTime( 0 );
  }

  std::vector<G4String> wl;
  for( ;; ) {
    if( !(theFileIn->GetWordsInLine(wl)) ) {
      bLastHitRead = TRUE;
    } 
    if( wl.size() == 0) {
      bLastHitRead = TRUE;
      break;
    } else {
      GmHit* hit = new GmHit( wl );
      if( hit->GetEnergy() == 0. ) continue; // problem reading hit, E is set to 0.
      theLastHit = hit;
    }
    if( theLastHit->GetEventID() == evtID || !bFirstHitRead ) {
      evtID = theLastHit->GetEventID();
      theHitsEventMgr->AddHit(theLastHit,theLastHit->GetSDType());
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits new hit added " << theLastHit->GetEventID() << " " << theLastHit->GetSDType() << " " << theLastHit->GetDetUnitID() << G4endl;
#endif
      G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventID(evtID);
      if( !bFirstHitRead ) { // for the first hit, set the event time here
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits SetEventTime = " << theLastHit->GetTimeMin() << G4endl;
#endif
	theEventTimeExtractor->SetEventTime( theLastHit->GetTimeMin() ); // this is only approximately the event time (= initial time for first particle, but it may serve)
	theHitsEventMgr->BuildEventTime( 0 );
      }

     } else {
      break;
    }
    bFirstHitRead = TRUE;
  }

  //  G4cout << " call BuildHitsCompatibleInTime " << G4endl;
  theHitsEventMgr->BuildHitsCompatibleInTime();
  
  if( theLastHit ){
    theHitsEventMgr->AddHitToDeadTimeDetUnitList( theLastHit->GetSDType() );
  }
  
  theHitsEventMgr->DigitizeAndReconstructHits();

}


//-----------------------------------------------------------------------
GmHitsIOtextMgr::~GmHitsIOtextMgr()
{
  CloseFileIn();
  CloseFileOut();
}


