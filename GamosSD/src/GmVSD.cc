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
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

#include "GmVSD.hh"
#include "GmVSDList.hh"
#include "GmHit.hh"
#include "GmHitsEventMgr.hh"
#include "GmHitList.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

//--------------------------------------------------------------------
SDSegmType GmVSD::theSDSegmType = SDvirtSegm; 
G4bool GmVSD::bFirstSDBegin = TRUE;
G4bool GmVSD::bFirstSDEnd = TRUE;

//--------------------------------------------------------------------
GmVSD::GmVSD(G4String my_det_name) : G4VSensitiveDetector(my_det_name)
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmVSD " << my_det_name 
				<< " path name " << GetPathName() 
				<< " SensitiveDetectorName: " << SensitiveDetectorName 
				<< G4endl;
#endif

  theCurrentHit = 0;

  //----- add this SD to list of SDs
  GmVSDList::GetInstance()->AddSD( this );

  theHitsEventMgr = GmHitsEventMgr::GetInstance();

  theHitList = theHitsEventMgr->CreateHitList( GetType() );

  theSDType = GetType();
  theSDType = theSDType.substr(1,theSDType.size()-2);

}

//--------------------------------------------------------------------
GmVSD::~GmVSD(){}

//--------------------------------------------------------------------
void GmVSD::Initialize(G4HCofThisEvent*)
{
  const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();

  //----- Things that are done once per event
  if( GmVSD::bFirstSDBegin ) {
#ifndef GAMOS_NO_VERBOSE
    //    if( SDVerb(debugVerb) ) G4cout << " GmVSD::bFirstSDBegin " << GmVSD::bFirstSDBegin << G4endl;
#endif
    theHitsEventMgr->BuildEventTime( event );
    theHitsEventMgr->CleanHits();
    theHitsEventMgr->CleanDigitsAndRecHits();
    //    theHitsEventMgr->CleanDeadTimeDetUnitList();

    GmVSD::bFirstSDBegin = FALSE;
    GmVSD::bFirstSDEnd = TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(debugVerb) ) G4cout << " Initialize(G4HCofThisEvent*HCE) " << G4endl;
#endif
  ClearHits(); // clear hits of previous event
  // Increases the Hit Collection with a new Hit. HEP experiments need to have
  // ready the full GmHit history in order to process the event.
  // This is overrided and I use my own variables for storage. In this way
  // I avoid memory overloading.

  theEventID = event->GetEventID();
}


//----------------------------------------------------------------------
G4double GmVSD::GetEnergyInSD( G4Step* aStep )
{
  return aStep->GetTotalEnergyDeposit();
}


//--------------------------------------------------------------------
G4bool GmVSD::IsInterestingStep( G4Step* aStep )
{
  G4double eDepo = aStep->GetTotalEnergyDeposit();
  if( eDepo == 0. ) {
    return FALSE;
  } else { 
    return TRUE;
  }

}

//--------------------------------------------------------------------
G4bool GmVSD::ProcessHits(G4Step* aStep,
                             G4TouchableHistory* )
{
  if( !IsInterestingStep( aStep ) ) return FALSE;

  unsigned long long detunitID = GetDetUnitID( aStep );

  //----- Check if hit belongs to current hit
  G4bool hitUpdated = FALSE;

  if( theCurrentHit != 0 ){
    if( theCurrentHit->CheckSameCell( detunitID ) ) { 
      UpdateHit( theCurrentHit, aStep );
      hitUpdated = TRUE;
    }
  }

  //----- Check if hit belongs to any other hit
  if( !hitUpdated ) {
    G4int isiz = theHitsInEvent.size();
    for( int ii=0; ii<isiz-1; ii++ ){
      if( theHitsInEvent[ii] == theCurrentHit ) continue;
      if( theHitsInEvent[ii]->CheckSameCell( detunitID ) ) {
	UpdateHit( theHitsInEvent[ii], aStep );
	hitUpdated = TRUE;
	break;
      }
    }     
  } 
  

  //----- Create hit if is not updating an existing one
  if( !hitUpdated ) CreateHit( aStep, detunitID );

  return TRUE;
}

//--------------------------------------------------------------------
void GmVSD::CreateHit( G4Step* aStep, unsigned long long id )
{
  G4String type = GetPathName();
  G4double energy = GetEnergyInSD( aStep );

  theCurrentHit = new GmHit( aStep, energy, id, type, theEventID );
  
  CalculateAndSetPosition( theCurrentHit, aStep ); //it is the SD that knows how to calculate the hit position
  theHitsInEvent.push_back( theCurrentHit );

  //- done in GmHit  theHitsEventMgr->AddHit( theCurrentHit, GetType() );
  
}

//--------------------------------------------------------------------
void GmVSD::UpdateHit( GmHit* hit, G4Step* aStep )
{
  G4double energy = GetEnergyInSD( aStep );
  hit->UpdateMe( aStep, energy );
}


//--------------------------------------------------------------------
void GmVSD::EndOfEvent(G4HCofThisEvent*)
{
  //----- Things that are done once per event (only in 1 SD)
  G4int isiz = theHitsInEvent.size();

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << G4endl 
	 << "%%%%%%% GmVSD::EndOfEvent " << GetType()
	 << " Nhits in this event= " << isiz 
	 << "  is first SD? " << GmVSD::bFirstSDEnd << G4endl;
#endif

  if( GmVSD::bFirstSDEnd ){
#ifndef GAMOS_NO_VERBOSE
    //    if( SDVerb(debugVerb) ) G4cout << " GmVSD::bFirstSDEnd " << GmVSD::bFirstSDEnd << G4endl;
#endif
    theHitsEventMgr->CleanDeadTimeDetUnitList();
    theHitsEventMgr->BuildHitsCompatibleInTime();

    theHitsEventMgr->AddHitToDeadTimeDetUnitList( GetType() );

    theHitsEventMgr->DigitizeAndReconstructHits();

    GmVSD::bFirstSDEnd = FALSE;
    GmVSD::bFirstSDBegin = TRUE;
  }


  //GmHitsEventMgr->addHits( theHitsInEvent );
  //GmHitsEventMgr->classifyEvent( theHitsInEvent );
 
  //  std::vector<GmHit*> hits = GetHitsInEvent();

#ifndef GAMOS_NO_VERBOSE 
  if( SDVerb(debugVerb) ) {
    G4cout << "%% GmVSD::EndOfEvent N HITS IN THIS EVENT: " << isiz << G4endl;
    for( int ii=0; ii<isiz; ii++ ){
      theHitsInEvent[ii]->Print();
    }

    gamosSDDigitMap digits = theHitsEventMgr->GetDigits();
    if( digits.size() != 0 ) {

      gamosSDDigitMap::const_iterator ited;
      std::vector<GmDigit*> digiv = (*ited).second;
      G4cout << "%% GmVSD::EndOfEvent N DIGITS IN SD of TYPE " << (*ited).first << " : " << digits.size() << G4endl;
      for( size_t ii = 0; ii < digiv.size(); ii++ ){
	digiv[ii]->Print();
      }
    }

    gamosSDRecHitMap rechits = theHitsEventMgr->GetRecHits();
    if( rechits.size() != 0 ){
      gamosSDRecHitMap::const_iterator iterh;
      for( iterh = rechits.begin(); iterh != rechits.end(); iterh++ ){
	std::vector<GmRecHit*> rhitv = (*iterh).second;
	G4cout << "EVENT: " << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << " %% GmVSD::EndOfEvent N RECHITS IN SD of TYPE " << (*iterh).first << " : " << rhitv.size() << G4endl;
	for( size_t ii = 0; ii < rhitv.size(); ii++ ){
	  rhitv[ii]->Print();
	}
      }
    }

     /*
  const std::map< G4String, GmVRecHitBuilder* > rhb = theHitsEventMgr->GetRecHitBuilders();
    //   if( theHitsEventMgr->GetRecHitBuilders().find( GetType() ) != theHitsEventMgr->GetRecHitBuilders().end() ) {
    if( rhb.size() != 0 && rhb.find( GetType() ) != rhb.end() ) {
#ifndef GAMOS_NO_VERBOSE
      //      if( SDVerb(infoVerb) ) G4cout << " rechitbuilder size " << theHitsEventMgr->GetRecHitBuilders().size() << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmVSD:: " << GetType() << " EndOfEvent nRecHits = " << theHitsEventMgr->GetRecHits(theSDTyp).size() << G4endl;
#endif
    }
    }*/
  }
#endif
  
}


//--------------------------------------------------------------------
G4String GmVSD::CleanPathName( G4String name )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " CleanPathName " << name << G4endl;
#endif
  int isiz = name.length();
  if( isiz == 0 ) {
    G4Exception("GmVSD::CleanPathName",
		"Wrong argument",
		FatalErrorInArgument,
		"Name is blank ");
  }
 
  //  if( name.c_str()[isiz-1] == '/' ) name.substr( 0, isiz-1 );
  //  if( name.c_str()[0] == '/' ) name.substr( 1, isiz );
 
  if( name.substr(isiz-1,1) == "/" ) name = name.substr( 0, isiz-1 );
  if( name.substr(0,1) == "/" ) name = name.substr( 1, isiz );
 
  return name;
}


//--------------------------------------------------------------------
void GmVSD::ClearHits()
{
  theHitsInEvent.clear();
  theCurrentHit = 0;
}


//--------------------------------------------------------------------
void GmVSD::DrawAll()
{
}


//--------------------------------------------------------------------
void GmVSD::PrintAll()
{
}
