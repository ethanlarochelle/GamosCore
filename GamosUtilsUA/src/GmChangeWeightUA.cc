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
#include "GmChangeWeightUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------
GmChangeWeightUA::GmChangeWeightUA()
{
}

//----------------------------------------------------------------
void GmChangeWeightUA::BeginOfRunAction( const G4Run* )
{
  G4String distName = GmParameterMgr::GetInstance()->GetStringValue(theName+":Distribution","");
  if( distName == "" ) {
    G4Exception("GmChangeWeightUA::GmChangeWeightUA",
		"Distribution not defined",
		FatalErrorInArgument,
		G4String("Use command /gamos/setParam " + theName + ":Distribution DISTRIBUTION_NAME").c_str());
  }

  std::vector<G4String> params;
  params.push_back(distName);
  params.push_back(distName);
  //  G4cout << " PARAMS " << params[0] << G4endl;
  //  G4cout << " PARAMS " << params[1] << G4endl;
  theDistribution = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);

}


//----------------------------------------------------------------
GmChangeWeightUA::~GmChangeWeightUA() 
{}

//----------------------------------------------------------------
void GmChangeWeightUA::UserSteppingAction( const G4Step* )
{
  /*
  G4double value = theDistribution->GetValueFromStep( aStep );

  G4Track* aTrack = aStep->GetTrack();

  // aTrack->SetWeight( aTrack->GetWeight()*value);
  aStep->GetPreStepPoint()->SetWeight( aTrack->GetWeight()*value);

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) {
    G4cout << " GmChangeWeightUA::UserSteppingAction " << aTrack->GetWeight() 
	   << " old weight " << aTrack->GetWeight()/value 
	   << " value " << value << G4endl; 
   }
#endif
  */

}

//----------------------------------------------------------------
void GmChangeWeightUA::PreUserTrackingAction(const G4Track* aTrack)
{
  G4double value = theDistribution->GetValueFromTrack( aTrack );

  //  G4Track* aTrackNC = const_cast<G4Track*>(aTrack);
  //  aTrackNC->SetWeight( aTrack->GetWeight()*value);

  G4double newWeight = aTrack->GetWeight()*value;
  G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager()->GetStep()->GetPostStepPoint()->SetWeight( newWeight );

#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) {
  G4cout << " GmChangeWeightUA::PreUserTrackingAction new weight: " << newWeight 
	 << " = old weight: " << aTrack->GetWeight()
	 << " * value: " << value << G4endl;
  }
#endif

}

