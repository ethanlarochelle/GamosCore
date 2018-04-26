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
#include "GmHistoryOrAncestorsFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmHistoryOrAncestorsFilter::GmHistoryOrAncestorsFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryOrAncestorsFilter::~GmHistoryOrAncestorsFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end()  // passed a previous step
      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptTrack return 1, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStep bPassed clear " << G4endl;
#endif
    bPassed.clear();
  }

  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* gmTrkInfo = dynamic_cast<GmTrackInfo*>(trkInfo);
  //-  if( !gmTrkInfo ) return FALSE;

  //--- Check if passed a previous step of current track or previous track at AcceptTrack
  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end()  
      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << this << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStep return 1, because previous step of current track or ancestor was accepted " << G4endl;
#endif
    bPassed.insert(trkID);
    bAccept = TRUE;
  } else {

    //--- Check if passed an step of a previous track, that is the step where the current track originated
    if( gmTrkInfo ) {
      if( gmTrkInfo->IntValueExists("HistoryOrAncestors"+GetName()) ) {
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return 1, because ancestor was accepted " << G4endl;
#endif
	bAccept = TRUE;
      }
    }
  }

  //--- Check if this step passes
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( bAccept ) if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return 1, because current step is accepted " << G4endl;
#endif
  }

  if( bAccept ) {
    bPassed.insert(aTrack->GetTrackID());
    //--- Set GmTrackInfo of daughter tracks (for current track bPassed is used)
    G4int ancestorLevel = 1;
    if( gmTrkInfo ) {
      G4int il = gmTrkInfo->GetIntValue("HistoryOrAncestors"+GetName())+1;
      if( il != INT_MAX ) ancestorLevel = il;
    }
    
    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    for( unsigned int ii = 0; ii < secos.size(); ii++) {
      GmTrackInfo* gmTrkInfo2 = new GmTrackInfo();
      gmTrkInfo2->SetIntValue("HistoryOrAncestors"+GetName(),ancestorLevel);
      secos[ii]->SetUserInformation(gmTrkInfo2);
    } 
  }
    
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return " << bAccept << G4endl;
#endif

  return bAccept;

}
