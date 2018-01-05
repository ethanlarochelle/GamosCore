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

#include "GmImportanceSamplingProcess.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GmInheritTrackInfoUA.hh"

GmInheritTrackInfoUA* GmImportanceSamplingProcess::theInheritTrackInfoUA = 0;

//------------------------------------------------------------------
GmImportanceSamplingProcess::GmImportanceSamplingProcess(const G4String& name , GmVDistribution* distrib ): G4VDiscreteProcess( name ), theDistribution(distrib)
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  theMaxSplitTimes = G4int(parMgr->GetNumericValue(theProcessName+":MaxSplitTimes",INT_MAX));

  bSplitAtSeveralSteps = G4bool(parMgr->GetNumericValue(theProcessName+":SplitAtSeveralSteps",1));
  std::vector<G4String> filterNames;
  filterNames = parMgr->GetVStringValue(theProcessName+":Filters",filterNames);

  for( unsigned int ii = 0; ii < filterNames.size(); ii++ ){
    std::vector<G4String> params;
    params.push_back( filterNames[ii] );
    params.push_back( filterNames[ii] );
    GmVFilter* filter = GmFilterMgr::GetInstance()->FindOrBuildFilter(params, true );
    theFilters.push_back(filter);
  }

  bApplyRussianRoulette = G4bool(parMgr->GetNumericValue(theProcessName+":ApplyRussianRoulette",1));

  bApplySplitting = G4bool(parMgr->GetNumericValue(theProcessName+":ApplySplitting",1));

  if( !theInheritTrackInfoUA ) {
    theInheritTrackInfoUA = new GmInheritTrackInfoUA();
    theInheritTrackInfoUA->AddIntValueName("NSplitTimes");
  }

}

//------------------------------------------------------------------
GmImportanceSamplingProcess::~GmImportanceSamplingProcess()
{
}


//------------------------------------------------------------------
G4double GmImportanceSamplingProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
			     G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX; //previousStepSize;
}



//------------------------------------------------------------------
G4double GmImportanceSamplingProcess::GetMeanFreePath(const G4Track& ,
				   G4double  ,
				   G4ForceCondition* )
{
  //  *fc = InActivated;
  //  return 0.1;
  return DBL_MAX;
}


//------------------------------------------------------------------
G4VParticleChange* GmImportanceSamplingProcess::PostStepDoIt( const G4Track& aTrack, const G4Step& aStep)
{

  pParticleChange->Initialize(aTrack);

  std::vector<GmVFilter*>::const_iterator ite;
  for( ite = theFilters.begin(); ite != theFilters.end(); ite++ ) {
    if( ! (*ite)->AcceptStep(&aStep) ) return pParticleChange;
  }

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << &aTrack << " GmImportanceSamplingProcess::PostStepDoIt  weight " << aTrack.GetWeight() << " " << aTrack.GetUserInformation() << G4endl;
#endif

  
  //----- If reached OutOfWorld do not split
  if( !aStep.GetPostStepPoint()->GetPhysicalVolume() ) return pParticleChange;
  
  //----- Get splitting number from distribution
  G4double indexVal = theDistribution->GetValueFromStep( &aStep );
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt " << theProcessName << " : indexVal " << indexVal << " TrkID " << aTrack.GetTrackID() << G4endl;
#endif

  if( indexVal <= 0 || fabs(indexVal-1) < 1.E-9 ) return pParticleChange;
  if( !bApplyRussianRoulette && indexVal < 1. ) return pParticleChange; 
  if( !bApplySplitting && indexVal > 1. ) return pParticleChange; 

  G4VUserTrackInformation* trackInfo = aTrack.GetUserInformation();
  G4int parentNSplitTimes = 1;
  if( trackInfo && trackInfo->GetType() == "GmImpSamplInfo" ) {
    GmTrackInfo* gmTrackInfo = (GmTrackInfo*)(trackInfo);
    G4int nSplitTimes = gmTrackInfo->GetIntValue("NSplitTimes");
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt "
					 << " NSplitTimes " << nSplitTimes 
					 << " MaxSplitTimes " << theMaxSplitTimes
					 << " weight " << aTrack.GetWeight() <<  G4endl;
#endif
    //    G4cout << " IMPOR " << bSplitAtSeveralSteps << G4endl;//GDEB
    if( !bSplitAtSeveralSteps ) {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt not split at several steps NsplitTimes " << nSplitTimes << G4endl;
#endif
      if( nSplitTimes > 0 ) return pParticleChange;
    } 
    if( nSplitTimes >= theMaxSplitTimes ) return pParticleChange;
    parentNSplitTimes = nSplitTimes+1;
    gmTrackInfo->SetIntValue("NSplitTimes", parentNSplitTimes );

  } else {
    GmTrackInfo* gmTrackInfo = new GmTrackInfo("GmImpSamplInfo");
    gmTrackInfo->SetIntValue("NSplitTimes", 1);
    G4Track* aTrackNC = (G4Track*)(&aTrack);
    aTrackNC->SetUserInformation( gmTrackInfo);
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt first time split " << G4endl;
    if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt NSplitTimes 0 MaxSplitTimes " << theMaxSplitTimes << " weight " << aTrack.GetWeight() <<  G4endl;
#endif
  }
  
  if( indexVal > 1. ) {
    G4int indexInt = G4int(indexVal);
    G4double indexTrunc = indexVal - indexInt;
    if( CLHEP::RandFlat::shoot() < indexTrunc ) indexInt += 1;

    G4double newWeight = aTrack.GetWeight() / (indexInt);
    pParticleChange->ProposeParentWeight( newWeight );
    
    G4ParticleDefinition* particle = aTrack.GetDefinition();
    G4ThreeVector dir = aTrack.GetMomentumDirection();
    G4double ener = aTrack.GetKineticEnergy();
    G4double time = aTrack.GetGlobalTime();
    G4ThreeVector pos = aTrack.GetPosition();
    
    pParticleChange->SetNumberOfSecondaries(pParticleChange->GetNumberOfSecondaries()+indexInt-1) ;
    
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt splitting particle " << indexInt << " new weight " << newWeight << " old weight " << aTrack.GetWeight() << G4endl;
#endif

    for( G4int ii = 0; ii < indexInt-1; ii++ ) {
      //------ Create a repeated track
      G4Track* newTrack = new G4Track( new G4DynamicParticle( particle, dir, ener ), time, pos );
      //      newTrack->SetGoodForTrackingFlag();
      //   newTrack->SetTouchableHandle(aStep.GetPostStepPoint()->GetTouchableHandle());
      pParticleChange->AddSecondary(newTrack);
      newTrack->SetWeight( newWeight );
      GmTrackInfo* gmTrackInfo = new GmTrackInfo("GmImpSamplInfo");
      gmTrackInfo->SetIntValue("NSplitTimes", parentNSplitTimes);
      newTrack->SetUserInformation( gmTrackInfo );
    }
    
  } else {
    // Play Russian roulette
    G4double rndN = CLHEP::RandFlat::shoot();
    if( rndN < indexVal ) {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt Russian roulette survives " << indexVal << " new weight " << aTrack.GetWeight() / indexVal << " old weight " << aTrack.GetWeight() << G4endl;
#endif
      pParticleChange->ProposeParentWeight( aTrack.GetWeight() / indexVal );
    }else {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb)  ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt Russian rouletter killed " << G4endl;
#endif
      pParticleChange->ProposeTrackStatus(fStopAndKill);
    }
  
  }
    
  ClearNumberOfInteractionLengthLeft();
  
  return pParticleChange;

}
 

//------------------------------------------------------------------
void GmImportanceSamplingProcess::FillHistoEachStep(const G4Step&  )
{
  
}
