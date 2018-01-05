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
#include "GmHistoryAncestorsAllFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmHistoryAncestorsAllFilter::GmHistoryAncestorsAllFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryAncestorsAllFilter::~GmHistoryAncestorsAllFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryAncestorsAllFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) {
    bNotPassed.clear();
  } else {
    if( bNotPassed.find(aTrack->GetTrackID()) != bNotPassed.end() || 
	bNotPassed.find(aTrack->GetParentID()) != bNotPassed.end() ) {
      return FALSE;
    }
  }

  // Ancestors passed, see if track also passes
  G4bool bAccept = AcceptTrackAND(aTrack);

  if( !bAccept ) bNotPassed.insert(aTrack->GetTrackID());

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryAncestorsAllFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;
 
  if( aStep->GetTrack()->GetParentID() == 0 &&
      aStep->GetTrack()->GetCurrentStepNumber() == 1 ) bNotPassed.clear();
  
  //--- Check if any of the previous step of this track or of the parent track did not pass
  G4int trkID = aStep->GetTrack()->GetTrackID();
  if( bNotPassed.find(trkID) != bNotPassed.end() || 
      bNotPassed.find(aStep->GetTrack()->GetParentID()) != bNotPassed.end() ) {
    bAccept = FALSE;
  }

  //--- Check this step
  if( bAccept ) {
    bAccept = AcceptStepAND(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( bAccept ) if( BaseVerb(debugVerb) ) G4cout << " GmHistoryAncestorsFilter::AcceptStep return 1, because current step is accepted " << G4endl;
#endif
  }

  if( !bAccept ) {
    bAccept = FALSE;
    bNotPassed.insert(trkID);
  }

  return bAccept;

}
