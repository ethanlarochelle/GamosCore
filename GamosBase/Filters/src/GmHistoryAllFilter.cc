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
#include "GmHistoryAllFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmHistoryAllFilter::GmHistoryAllFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryAllFilter::~GmHistoryAllFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryAllFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 ) bNotPassed = TRUE;

  if( !bNotPassed ) return FALSE;

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( !bAccept ) bNotPassed = FALSE;

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryAllFilter::AcceptStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 ) bNotPassed = TRUE;

  if( !bNotPassed ) return FALSE; // if one step failed, do not continue checking

  G4bool bAccept = AcceptStepAND(aStep);

  if( !bAccept ) bNotPassed = FALSE;

  return bAccept;

}
