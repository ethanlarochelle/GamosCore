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
#include "GmCopyWeightToRDSecondaryUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


//----------------------------------------------------------------
GmCopyWeightToRDSecondaryUA::GmCopyWeightToRDSecondaryUA()
{
}


//----------------------------------------------------------------
void GmCopyWeightToRDSecondaryUA::UserSteppingAction(const G4Step* aStep )
{
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc ) {
    if( ! (proc->GetProcessName() == "RadioactiveDecay") ) return;
  }

  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4Track* aTrack = aStep->GetTrack();
  for( ite = secos.begin(); ite != secos.end(); ite++) {
    (*ite)->SetWeight( (*ite)->GetWeight() * aTrack->GetWeight() );
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) {
    G4cout << " GmCopyWeightToRDSecondaryUA::PreUserTrackingAction new weight " << (*ite)->GetWeight() 
	   << " old weight " <<  (*ite)->GetWeight()/aTrack->GetWeight() 
	   << " value " << aTrack->GetWeight() << G4endl; 
  }
#endif
  }

}
