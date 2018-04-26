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
#include "GmEnergyChangeFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

GmEnergyChangeFilter::GmEnergyChangeFilter(G4String name)
  :GmVFilter(name)
{
  theTrackElost = 0.;
  theLastTrackID = 0;
  theLastEventID = 0;
}

GmEnergyChangeFilter::~GmEnergyChangeFilter()
{;}

G4bool GmEnergyChangeFilter::AcceptStep(const G4Step* aStep)
{
  G4double elost = aStep->GetPostStepPoint()->GetKineticEnergy() - aStep->GetPreStepPoint()->GetKineticEnergy();
  G4int trackID = aStep->GetTrack()->GetTrackID();
  G4int eventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( trackID != theLastTrackID || eventID != theLastEventID ) theTrackElost = 0.;
  theTrackElost += elost;
  theLastTrackID = trackID; 
  theLastEventID = eventID;
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "  GmEnergyChangeFilter::AcceptStep " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( elost < fLowEnergy  ) return FALSE;
  if ( elost > fHighEnergy ) return FALSE;

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "  GmEnergyChangeFilter::AcceptStep accepted " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  return TRUE;
}

G4bool GmEnergyChangeFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetTrackStatus() != fStopAndKill ) { // start of track does not have energy lost
    return FALSE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) )  G4cout << "  GmEnergyChangeFilter::AcceptTrack " << theTrackElost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( theTrackElost < fLowEnergy  ) return FALSE;
  if ( theTrackElost > fHighEnergy ) return FALSE;
  return TRUE;
}

void GmEnergyChangeFilter::show() 
{
    G4cout << " GmEnergyChangeFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmEnergyChangeFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmEnergyChangeFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
