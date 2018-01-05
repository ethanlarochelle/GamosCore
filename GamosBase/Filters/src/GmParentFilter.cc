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
#include "GmParentFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmParentFilter::GmParentFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmParentFilter::~GmParentFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmParentFilter::AcceptTrack(const G4Track* aTrack)
{
  G4int parentID = aTrack->GetParentID();
  if( parentID == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4bool bAccept = AcceptTrackAND(aTrack);
  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

  if( bPassed.find( parentID ) != bPassed.end()) { // passed parent in any step
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptTrack return TRUE, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptTrack return FALSE" << G4endl;
#endif

  return FALSE;

}


//----------------------------------------------------------------
G4bool GmParentFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) bPassed.clear();

  //--- Check if this step passes
  G4int trkID = aTrack->GetTrackID();
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
    if( bAccept ) bPassed.insert(trkID);
  } 

  //--- Check if passed previous track at AcceptTrack
  if( bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptStep returns TRUE, because ancestor was accepted at AcceptTrack " << G4endl;
#endif
    return TRUE;
  } 
    
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptStep return FALSE" << G4endl;
#endif

  return FALSE;

}
