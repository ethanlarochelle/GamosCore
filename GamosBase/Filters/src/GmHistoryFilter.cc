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
#include "GmHistoryFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmHistoryFilter::GmHistoryFilter(G4String name)
  :GmVANDFilter(name)
{ 
  bPassed = FALSE;
}

//----------------------------------------------------------------
GmHistoryFilter::~GmHistoryFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 )  bPassed = FALSE;

  if( bPassed )  {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << theName << " GmHistoryFilter::AcceptTrack 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << " GmHistoryFilter::AcceptTrack " << bAccept << G4endl;
#endif

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryFilter::AcceptStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 )  bPassed = FALSE;

  if( bPassed ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << theName << " GmHistoryFilter::AcceptStep 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptStepAND(aStep);

  if( bAccept ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << " GmHistoryFilter::AcceptStep " << bAccept << G4endl;
#endif

  return bAccept;

}
