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
#define _USE_MATH_DEFINES
#include <cmath>

#include "GmPDSVProcess.hh"
#include "GmPDSInteractionAngleManager.hh"
#include "GmPDSScore.hh"
#include "GmPDSProcessHelper.hh"
#include "GmPDSDetector.hh"
#include "GmPDSUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4Geantino.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SteppingManager.hh"
#include "G4ParticleTable.hh"
#include "G4VMultipleScattering.hh"

//------------------------------------------------------------------
GmPDSVProcess::GmPDSVProcess(const G4String& name, G4SteppingManager* fpSM ): G4VDiscreteProcess( name ), fpSteppingManager(fpSM)
{
  /*  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = procVector->size()-1; ii >= 0; ii-- ) {
      //      G4cout << ii << " check RemoveProcess " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << " " << (*procVector)[ii]->GetProcessType() << " =? " << (*itep) << G4endl;
      //?      if( find ( itemmp.first, itemmp.second, (*procVector)[ii]->GetProcessName() ) != itemmp.second ) {
      //      if( "msc" == (*procVector)[ii]->GetProcessName() ) {
      if( static_cast<G4VMultipleScattering*>((*procVector)[ii]) ) {
	G4cout << " Found Process " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << G4endl;
	if( "msc" == (*procVector)[ii]->GetProcessName() ) {
	  G4VMultipleScattering* mscProc = (G4VMultipleScattering*)((*procVector)[ii]);
	  G4VMscModel* mscModel = mscProc->EmModel();
	  theMscModels[particle] = mscModel;
	  G4cout << " Found MSC Process " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << G4endl;
	}
      }
    }
  }
  */    

}

//------------------------------------------------------------------
GmPDSVProcess::~GmPDSVProcess()
{
}


//------------------------------------------------------------------
void GmPDSVProcess::ApplyRussianRoulette( G4Track* aTracknc, double weight )
{
  G4double threshold = 0.1;
  if( CLHEP::RandFlat::shoot() < threshold ) {
    aTracknc->SetWeight( weight/threshold );
    SetWeight( aTracknc, weight/threshold );
  } else {
    aTracknc->SetTrackStatus( fStopAndKill );
  }
}
    

//------------------------------------------------------------------
void GmPDSVProcess::AddHelper( GmPDSProcessHelper* helper  )
{
  //  G4cout << this << " GmPDSVProcess::AddHelper " << helper << " IsForNeutron " << helper->IsForNeutron() << G4endl;
  theHelpers[helper->IsForNeutron()] = helper;
  theCurrentHelper = helper;
  theClassifier = theCurrentHelper->theClassifier;
  if( helper->IsForNeutron() ) {
    bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:neutron:ClassifierOnTrack",1)); 
    bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:neutron:FiltersOnTrack",1)); 
  } else {
    bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:gamma:ClassifierOnTrack",1)); 
    bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:gamma:FiltersOnTrack",1)); 
  } 
} 


//------------------------------------------------------------------
G4double GmPDSVProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
			     G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX; //previousStepSize;
}



//------------------------------------------------------------------
G4double GmPDSVProcess::GetMeanFreePath(const G4Track& ,
				   G4double  ,
				   G4ForceCondition* )
{
  //  *fc = InActivated;
  //  return 0.1;
  return DBL_MAX;
}


//------------------------------------------------------------------
G4VParticleChange* GmPDSVProcess::PostStepDoIt( const G4Track& aTrack, const G4Step& aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPDSVProcess::PostStepDoIt weight " << GetWeight(&aTrack, FALSE) << " = " << aTrack.GetWeight() << " " << aTrack.GetUserInformation() << " pos " << aTrack.GetVertexPosition() << G4endl;
#endif

  aParticleChange.Initialize(aTrack);

  const G4ParticleDefinition* particle = aTrack.GetDefinition();

  //---- Check if detector has been reached
  G4int DR = -1;
  if( particle == theOriginalParticle ) {
    DR = theCurrentHelper->DetectorReached( aStep );
    if( DR != -1 ) {
      if( !aTrack.GetUserInformation() ) {
	G4Track* aTrackNC = const_cast<G4Track*>(&aTrack);
	G4Point3D point = theCurrentHelper->GetDetector(DR)->GetCentrePoint();
	GmTrackInfo* trkInfo = new GmTrackInfo("GmPDSTrackInfo");
	trkInfo->SetStringValue("ProcessName","Primary");
	trkInfo->SetThreeVectorValue("Point",point);
	trkInfo->SetBoolValue("IsOriginalParticleSplit",0);
	aTrackNC->SetUserInformation(trkInfo);
	SetWeight(aTrackNC, aTrack.GetWeight());
      }

      //      if( ScoringVerb(-testVerb) )
      theCurrentHelper->FillScores(aTrack, FALSE,DR);

    }

    if( theCurrentHelper->bControlHistos ) {
      FillHistoEachStep( aStep );
    }

  }
  
  if( DR == -1 ) {
    //----- Get neutron/gamma secondary track 
    std::map<G4int,G4int> detIDs = theCurrentHelper->GetDetectorIDs();
    std::map<G4int,G4int>::const_iterator iteid;
    std::vector< G4Track*> secoTracks = GetSecondaryAndPrimaryTracks(&aTrack);
    std::vector< G4Track*>::const_iterator itetrk;
    
    //---- Get process defining step
    G4String procDefStepName = GetProcessDefiningStepName(aStep);
    for( iteid = detIDs.begin(); iteid != detIDs.end(); iteid++ ){
      for( itetrk = secoTracks.begin(); itetrk != secoTracks.end(); itetrk++ ){
	//----- check filters first
	std::vector<GmVFilter*>::const_iterator itef;
	G4bool bAccepted = true;
	for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt checking filter " << (*itef)->GetName()  << " : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
	  if( bFiltersOnTrack ) {
	    if( !(*itef)->AcceptTrack( &aTrack ) ) {
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track rejected by filter " << (*itef)->GetName() << G4endl;
#endif
	      bAccepted = false; 
	      break;
	    }
	  } else {
	    if( !(*itef)->AcceptStep( &aStep ) ) {
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step rejected by filter " << (*itef)->GetName() << G4endl;
#endif
	      bAccepted = false; 
	      break;
	    }
	  }
	}
	
	if( bAccepted) {
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt accepted by all filters : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
	  //--- Get the classification index
	  G4int index = -1;
	  if( theClassifier) {
	    if(bClassifierOnTrack ) {
	      index = theClassifier->GetIndexFromTrack(&aTrack); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
	    } else {
	      index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
	    }
	  }
	  
	  CreateGeantino( aTrack, aStep.GetPreStepPoint()->GetKineticEnergy(), aStep.GetPostStepPoint(), (*itetrk)->GetKineticEnergy() ,(*iteid).second, procDefStepName, index );
	}
      }
      
      if( particle == theOriginalParticle ) {
	//----- Create a geantino for primary neutrons/gammas at first track step 
	if( aTrack.GetParentID() == 0 
	    && aTrack.GetCurrentStepNumber() == 1 ) {
	  
	  //----- check filters first
	  std::vector<GmVFilter*>::const_iterator itef;
	  G4bool bAccepted = true;
	  for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt checking filter " << (*itef)->GetName() << " : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
	    if( bFiltersOnTrack ) {
	      if( !(*itef)->AcceptTrack( &aTrack ) ) {
		bAccepted = false; 
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track rejected by filter " << (*itef)->GetName() << G4endl;
#endif
		break;
	      }
	    } else {
	      if( !(*itef)->AcceptStep( &aStep ) ) {
		bAccepted = false; 
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step rejected by filter " << (*itef)->GetName() << G4endl;
#endif
		break;
	      }
	    }
	  }
	  
	  if( bAccepted) {
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt accepted by all filters : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
	    //--- Get the classification index
	    G4int index = -1;
	    if( theClassifier) {
	      if(bClassifierOnTrack ) {
		index = theClassifier->GetIndexFromTrack(&aTrack); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
	      } else {
		index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
	      }
	    }
	    
	    CreateGeantino( aTrack, aStep.GetPreStepPoint()->GetKineticEnergy(), aStep.GetPreStepPoint(), aStep.GetPreStepPoint()->GetKineticEnergy(), (*iteid).second, "Primary", index );
	  }
	  
	}
      }
    }
  }
  
  ClearNumberOfInteractionLengthLeft();
  
  return pParticleChange;

  //  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}


//------------------------------------------------------------------
void GmPDSVProcess::CreateGeantino( const G4Track& aTrack, const G4double primPreEner, const G4StepPoint* stepPoint2, const G4double secoEner, const G4int detID, const G4String procDefStepName, G4int classifierIndex )
{

  G4double dist = theCurrentHelper->GetDistanceToDetectorIncm(stepPoint2->GetPosition(), detID);
  G4double wei = GetWeight(&aTrack,FALSE);

  if( bUseMaximumDistance ) {
     //---- Play Russian Roulette if distance is big
    if( dist > theMaximumDistance ) {
      if( CLHEP::RandFlat::shoot() < theInvMaximumDistanceRR ) {
	wei /= theInvMaximumDistanceRR;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino big distance " << dist << " weight increased by Russian Roulette to " << wei << G4endl;
#endif
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino big distance " << dist << " killed by Russian Roulette " << G4endl;
#endif
	return;
      }
    }
  }
  
  //----- Get angle between current track direction and line to detector point
  //--- Get point in detector
  G4Point3D point = theCurrentHelper->GetDetector(detID)->GetCentrePoint();
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino  det point = " << point << " detID " << detID << G4endl;
#endif
  
  //--- Get direction to point
  G4ThreeVector trkpos  = stepPoint2->GetPosition();
  G4ThreeVector stepPtDir = point - trkpos;
  stepPtDir *= 1./stepPtDir.mag();
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino  dir to point = " << stepPtDir << " trkpos " << trkpos << G4endl;
#endif
  
  //--- Get angle between two directions
  //t  G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPostStepPoint()->GetMomentumDirection();
  G4ParticleDefinition* particle = aTrack.GetDefinition();
  /* G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPreStepPoint()->GetMomentumDirection();
  // Compute multiple scattering
  std::map<G4ParticleDefinition*,G4VMscModel*>::const_iterator itemm = theMscModels.find(particle);
  if( itemm != theMscModels.end() ) {
    G4cout << " MSC OLD DIR " << secoTrkDir2;
    G4double minLen = 1.;
    (*itemm).second->ComputeTruePathLengthLimit(aTrack, minLen); // just to set couple
    secoTrkDir2 = ((*itemm).second)->SampleScattering(secoTrkDir2,DBL_MAX);
    G4cout << " NEW DIR " << secoTrkDir2 << G4endl;
    }*/

  G4double emissionCosAngle = secoTrkDir2 * stepPtDir;
  
  //--- Get probability of producing particle at this angle
  G4double probEmiss;
  //  const G4ParticleDefinition* particle = aTrack.GetDefinition();
  //t tables should be made for other particles
  //  if( particle != theOriginalParticle ) {
  //    probEmiss = 0.5;
  //  } else {
  probEmiss = theInteractionAngleManager->GetHistoValue( particle->GetParticleName() + " - " + theOriginalParticle->GetParticleName() + " : " + procDefStepName, stepPoint2->GetMaterial()->GetName(), primPreEner, emissionCosAngle );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino ener(pre) " << primPreEner  << " emissionCosAngle = " << emissionCosAngle << " prob " << probEmiss << " secoTrkDir2 "<< secoTrkDir2 << " stepPtDir " << stepPtDir << " primTrkDir " << aTrack.GetMomentumDirection() << G4endl;
#endif
  //  }
  if( probEmiss == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << "GmPDSVProcess::CreateGeantino ener(pre) " << primPreEner << " emissionCosAngle = " << emissionCosAngle << " prob " << probEmiss << G4endl;
#endif
    return;
  }

  //------ Create a geantino to be tracked until detector
  G4Track* geantino = new G4Track( new G4DynamicParticle( G4Geantino::Geantino(), stepPtDir, secoEner ), stepPoint2->GetGlobalTime(), trkpos );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt  new G4Track: dir " << stepPtDir << " pos " << trkpos << " ener " << secoEner << " time " << stepPoint2->GetGlobalTime() << G4endl;
#endif
  //  G4Track* geantino = new G4Track( new G4DynamicParticle( G4Geantino::Geantino(), stepPoint2->GetMomentumDirection(), stepPoint2->GetKineticEnergy() ), 
  
  geantino->SetGoodForTrackingFlag();
  geantino->SetTouchableHandle(stepPoint2->GetTouchableHandle());
  G4VUserTrackInformation* trkiOrig = aTrack.GetUserInformation();
  G4bool bOrigSplit = false;
  if( trkiOrig != 0 ) { 
    GmTrackInfo* gmtrkiOrig = dynamic_cast<GmTrackInfo*>( trkiOrig );
    //    if( gmtrkiOrig->BoolValueExists("IsOriginalParticleSplit") )  bOrigSplit = gmtrkiOrig->GetBoolValue("IsOriginalParticleSplit");
    if( gmtrkiOrig->GetType() == "GmPDSTrackInfo" )  bOrigSplit = gmtrkiOrig->GetBoolValue("IsOriginalParticleSplit");
  }
  GmTrackInfo* trkInfo = new GmTrackInfo("GmPDSTrackInfo");
  trkInfo->SetStringValue("ProcessName",procDefStepName);
  trkInfo->SetThreeVectorValue("Point",point);
  trkInfo->SetBoolValue("IsOriginalParticleSplit",bOrigSplit);
  trkInfo->SetThreeVectorValue("OriginalParticleDir",stepPoint2->GetMomentumDirection() );
  trkInfo->SetParticle(theOriginalParticle);
  trkInfo->SetIntValue("DetectorID", detID );
  //--- Store classification index in track info
  trkInfo->SetIntValue("ClassifierIndex",classifierIndex );
  trkInfo->SetDoubleValue("ParentWeight", GetWeight(&aTrack,FALSE) );
  
  geantino->SetUserInformation( trkInfo );
  
//  G4double dist = (point-trkpos).mag()/cm;
  SetWeight( geantino, wei * probEmiss / (2.*M_PI*sqr(dist) ) );
  //SetWeight( geantino, 1. / (2.*M_PI*pow( dist,2 ) ) );
  // SetWeight( geantino, 1. );
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt  probEmiss = " << probEmiss << " dist (cm) " << dist << " weight old " << wei << " weight new " << wei * probEmiss / (2.*M_PI*sqr(dist) ) << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  aParticleChange.AddSecondary(geantino);  if( ScoringVerb(infoVerb) ) G4cout << " n secondaries " << aParticleChange.GetNumberOfSecondaries() << G4endl;
#endif
}
 

//------------------------------------------------------------------
void GmPDSVProcess::FillHistoEachStep(const G4Step& aStep )
{
  //----- Count each step 
  G4int nh = theCurrentHelper->theHistoNumber;

  G4int index = -1;
  if( theClassifier) {
    if(bClassifierOnTrack ) {
      index = theClassifier->GetIndexFromTrack(aStep.GetTrack()); // beware which classifier can be applied, as some of these are secondary tracks
    } else {
      index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
    }
  }

  theCurrentHelper->CheckNewIndex(index);

  //----- Count also starting neutron step as source (take preStep info)
  if( aStep.GetTrack()->GetCurrentStepNumber() == 1 ){
    theCurrentHelper->theAnaMgr->GetHisto1(nh+100000*index+90)->Fill(log10(aStep.GetPreStepPoint()->GetKineticEnergy()));
  }

  //----- Count each interaction if not transportation
  if( aStep.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "Transportation" ){
    //    G4cout << "GmPDSVProcess::FillHistoEachStep filling histo  " << theCurrentHelper->theAnaMgr->GetHisto1(nh+0*100+90)->GetName() << " " << nh+0*100+90 << " " << log10(aStep.GetPostStepPoint()->GetKineticEnergy()) << G4endl;
    theCurrentHelper->theAnaMgr->GetHisto1(nh+100000*index+90)->Fill(log10(aStep.GetPostStepPoint()->GetKineticEnergy()));
  }
  
}


//------------------------------------------------------------------
std::vector<G4Track*> GmPDSVProcess::GetSecondaryAndPrimaryTracks( const G4Track* aTrack)
{
  std::vector<G4Track*> secos; 
  G4TrackVector secoList = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  for( ite = secoList.begin(); ite != secoList.end(); ite++ ){
    if( (*ite)->GetDefinition() == theOriginalParticle ){
      secos.push_back(*ite);
#ifndef GAMOS_NO_VERBOSE
     if( ScoringVerb(debugVerb) ) G4cout << " GmPDSVProcess::GetSecondaryAndPrimaryTracks  new seco " << theOriginalParticle->GetParticleName() << " ENER " <<  (*ite)->GetKineticEnergy() << G4endl;    
#endif

    }
  }

  //----- Get current track unless it is stopped (case of inelastic)
  if( aTrack->GetDefinition() == theOriginalParticle 
      && aTrack->GetTrackStatus() != fStopAndKill 
      && aTrack->GetTrackStatus() != fStopButAlive
      && aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "Transportation" ) {
    secos.push_back( const_cast<G4Track*>(aTrack) );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPDSVProcess::GetSecondaryAndPrimaryTracks  process deviates particle " << aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;    
#endif
  }

  return secos;
}


//------------------------------------------------------------------
G4String GmPDSVProcess::GetProcessDefiningStepName(const G4Step& aStep)
{
  G4String procName;

  const G4VProcess* proc = aStep.GetPostStepPoint()->GetProcessDefinedStep();
  if( proc ) {
    procName = proc->GetProcessName();
  } else {
    G4Exception("GmPDSVNProcess::GetProcessDefiningStepName",
                "Wrong argument",
                FatalErrorInArgument,
                "Step has no process defining it");
   }
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << "GmPDSVProcess::GetProcessDefiningStepName  " << procName << G4endl;
#endif

  return procName;

}
