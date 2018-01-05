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
#include "GmDataAccumulatedNofTracks.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedNofTracks::GmDataAccumulatedNofTracks()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTRun);

}

//----------------------------------------------------------------
GmDataAccumulatedNofTracks::~GmDataAccumulatedNofTracks()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedNofTracks::Accumulate( const G4Step* aStep, G4int index )
{
  theTracks[index].insert(aStep->GetTrack()->GetTrackID());
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::Accumulate " << index << " : " << theTracks[index].size() << G4endl;
#endif 

}

//----------------------------------------------------------------
G4double GmDataAccumulatedNofTracks::GetValueFromTrack( const G4Track*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromTrack " << index << " = 1 " << G4endl;
#endif 
  return 1;
}

//----------------------------------------------------------------
G4double GmDataAccumulatedNofTracks::GetValueFromEvent( const G4Event*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromEvent " << index << " = " << theTracks[index].size() << G4endl;
#endif 
  return theTracks[index].size();
}

//----------------------------------------------------------------
void GmDataAccumulatedNofTracks::Initialise()
{
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVDataAccumulated::Initialise data " << theName << G4endl; 
#endif
  theTracks.clear();
}

