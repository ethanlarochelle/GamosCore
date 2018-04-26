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
#include "GmAncestorsFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmAncestorsFilter::GmAncestorsFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmAncestorsFilter::~GmAncestorsFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmAncestorsFilter::AcceptTrack(const G4Track* aTrack)
{
  G4int trkID = aTrack->GetTrackID();
  G4int parentID = aTrack->GetParentID();
  if( parentID == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  if( bPassed.find(trkID) != bPassed.end()  // passed a previous step
      || bPassed.find( parentID) != bPassed.end()) { // passed parent in any step
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptTrack return 1, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmAncestorsFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) bPassed.clear();

  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* gmTrkInfo = dynamic_cast<GmTrackInfo*>(trkInfo);
  if( !gmTrkInfo ) return FALSE;

  //--- Check if passed previous track at AcceptTrack
  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptStep return 1, because ancestor was accepted at AcceptTrack " << G4endl;
#endif
    bPassed.insert(trkID);
    bAccept = TRUE;
  } else {

    //--- Check if passed the step of the previous track where the current track originated
    if( gmTrkInfo ) {
      if( gmTrkInfo->IntValueExists("HistoryAncestors") ) {
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptStep return 1, because ancestor was accepted at AcceptStep " << G4endl;
#endif
	bAccept = TRUE;
      }
    }
  }

  //--- Check if this step passes
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( bAccept ) if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptStep return 1, because current step is accepted " << G4endl;
#endif
  }

  if( bAccept ) {
    //--- Set GmTrackInfo of daughter tracks (for current track bPassed is used)
    G4int ancestorLevel = 1;
    if( gmTrkInfo ) {
      ancestorLevel = gmTrkInfo->GetIntValue("HistoryAncestors")+1;
    }
    
    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    for( unsigned int ii = 0; ii < secos.size(); ii++) {
      GmTrackInfo* gmTrkInfot = new GmTrackInfo();
      gmTrkInfot->SetIntValue("HistoryAncestors",ancestorLevel);
      secos[ii]->SetUserInformation(gmTrkInfot);
    } 
  }
    
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmAncestorsFilter::AcceptStep return " << bAccept << G4endl;
#endif

  return bAccept;

}
