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
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "GmBSControlHistosUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
// For event ID:
#include "G4EventManager.hh"
#include "G4Event.hh"


//----------------------------------------------------------------
GmBSControlHistosUA::GmBSControlHistosUA()
{  
}


//----------------------------------------------------------------
void GmBSControlHistosUA::BeginOfRunAction( const G4Run* )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("BScontrol");
  BookHistos();
}


//----------------------------------------------------------------
void GmBSControlHistosUA::BookHistos()
{
  G4double hEMin = 0.;
  G4double hEMax = 10.*CLHEP::MeV;
  G4int hENbins = 100;

  G4double hPosXYMin = -100;
  G4double hPosXYMax = 100;
  G4int hPosXYNbins = 100;

  G4double hPosZMin = -250;
  G4double hPosZMax = 100;
  G4int hPosZNbins = 750;

  G4double hAngleMin = 0.;
  G4double hAngleMax = 3.5;
  G4int hAngleNbins = 35;

  // Create histograms
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" E",hENbins,hEMin,hEMax,314201);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" E parent",hENbins,hEMin,hEMax,314202);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" E - E parent",2*hENbins,-hEMax,hEMax,314203);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" E vs E parent",hENbins,hEMin,hEMax,hENbins,hEMin,hEMax,314204);

  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" position X",hPosXYNbins,hPosXYMin,hPosXYMax,314211);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" position Y",hPosXYNbins,hPosXYMin,hPosXYMax,314212);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" position Z",hPosZNbins,hPosZMin,hPosZMax,314213);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" position R",hPosXYNbins,0.,hPosXYMax,314214);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" position R vs Z",hPosXYNbins,0.,hPosXYMax,hPosZNbins,hPosZMin,hPosZMax,314215);

  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho posX vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314216);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho posY vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314217);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho posZ vs Wgt",hPosZNbins, hPosZMin, hPosZMax, 120,-0.1,1.1,314218);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele posX vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314316);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele posY vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314317);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele posZ vs Wgt",hPosZNbins, hPosZMin, hPosZMax, 120,-0.1,1.1,314318);

  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho X vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314416);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho Y vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314417);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho Z vs Wgt",hPosZNbins, hPosZMin, hPosZMax, 120,-0.1,1.1,314418);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho X-Y-Fat", hPosXYNbins,hPosXYMin,hPosXYMax,hPosXYNbins,hPosXYMin,hPosXYMax,314419);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" pho X-Y-NonFat",hPosXYNbins,hPosXYMin,hPosXYMax,hPosXYNbins,hPosXYMin,hPosXYMax,314420);

  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele X vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314516);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele Y vs Wgt",hPosXYNbins,hPosXYMin,hPosXYMax,120,-0.1,1.1,314517);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele Z vs Wgt",hPosZNbins, hPosZMin, hPosZMax, 120,-0.1,1.1,314518);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele X-Y-Fat", hPosXYNbins,hPosXYMin,hPosXYMax,hPosXYNbins,hPosXYMin,hPosXYMax,314519);
  theAnaMgr->CreateHisto2D("GmBSControlHistosUA"+sepa+" ele X-Y-NonFat",hPosXYNbins,hPosXYMin,hPosXYMax,hPosXYNbins,hPosXYMin,hPosXYMax,314520);

  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" angle theta",hAngleNbins,hAngleMin,hAngleMax,314221);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" angle phi",2*hAngleNbins,-hAngleMax,hAngleMax,314222);
  theAnaMgr->CreateHisto1D("GmBSControlHistosUA"+sepa+" angle vs parent",hAngleNbins,hAngleMin,hAngleMax,314223);

}


//----------------------------------------------------------------
void GmBSControlHistosUA::UserSteppingAction(const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();

  G4TrackVector fSec = GetStepSecondaries();

  G4double weight = aTrack->GetWeight();
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "ControlHistosUA: Wgt " << weight << G4endl;
#endif
#endif
  G4ThreeVector pos = aTrack->GetPosition();
  if (aTrack->GetDefinition() == G4Gamma::Gamma()) {
	theAnaMgr->GetHisto2(314216)->Fill(pos.x(),weight,1);
	theAnaMgr->GetHisto2(314217)->Fill(pos.y(),weight,1);
	theAnaMgr->GetHisto2(314218)->Fill(pos.z(),weight,1);
  }
  if (aTrack->GetDefinition() == G4Electron::Electron()) {
	theAnaMgr->GetHisto2(314316)->Fill(pos.x(),weight,1);
	theAnaMgr->GetHisto2(314317)->Fill(pos.y(),weight,1);
	theAnaMgr->GetHisto2(314318)->Fill(pos.z(),weight,1);
  }
  G4double Zpla = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:ZPos",1000.*CLHEP::mm);
  /*
  std::vector<G4double> theZStops = GmParameterMgr::GetInstance()->GetVNumericValue("RTPhaseSpaceUA:ZStops",std::vector<G4double>());
  std::sort(theZStops.begin(),theZStops.end());
  G4double Zpla = theZStops[theZStops.size()-1];
  */

  G4double vPre = aStep->GetPreStepPoint()->GetPosition().z();
  G4double vPost = aStep->GetPostStepPoint()->GetPosition().z();

  G4ThreeVector pos1 = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector pos2 = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector dir1 = aStep->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector dire;
  G4double magx, magy, magz, magn;
  magx = pos2.x()-pos1.x();
  magy = pos2.y()-pos1.y();
  magz = pos2.z()-pos1.z();
  magn = std::sqrt( magx*magx + magy*magy + magz*magz );
  if (magn==0.) {
	dire = dir1;
  } else {
	dire.setX(magx/magn);
	dire.setY(magy/magn);
	dire.setZ(magz/magn);
  }
  if (fabs(dir1.x()-dire.x())>0.000001 && fabs(dir1.y()-dire.y())>0.000001 && fabs(dir1.z()-dire.z())>0.000001 ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "ControlHistosUA: Momentum Direction, Pre : " << dir1 << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "ControlHistosUA: Calc. Momentum Direction: " << dire << G4endl;
#endif
#endif
  }

  if ((vPre-Zpla)<=0.0 && (vPost-Zpla)>=0.0) {
    G4double Xpla = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:XDim",100.*CLHEP::mm);
    G4double Ypla = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:YDim",100.*CLHEP::mm);
	G4double diffZ    = Zpla - pos1.z();
	G4double xInPlane = pos1.x()+diffZ*dire.x()/dire.z();
	G4double yInPlane = pos1.y()+diffZ*dire.y()/dire.z();
	G4int theEventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
	if (aTrack->GetDefinition() == G4Gamma::Gamma()) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	  if( PhysicsVerb(debugVerb) ) G4cout << "ControlHistosUA: gamma xInPlane / yInPlane / Weight  " << xInPlane << "  " << yInPlane << "  " << weight << G4endl;
#endif
#endif
	  theAnaMgr->GetHisto2(314416)->Fill(xInPlane,weight);
	  theAnaMgr->GetHisto2(314417)->Fill(yInPlane,weight);
	  theAnaMgr->GetHisto2(314418)->Fill(pos1.z(),weight);
	  if (weight > 0.99) {
		theAnaMgr->GetHisto2(314419)->Fill(xInPlane,yInPlane);
		if (fabs(xInPlane)<Xpla && fabs(yInPlane)<Ypla) {
#ifndef GAMOS_NO_VERBOSE
		  G4cout << "ControlHistos: Fat photon inside FOI! Event ID " << theEventID << " X " << xInPlane << " Y " << yInPlane << " Z1 " << pos1.z() << " Z2 " << pos2.z() << G4endl;
#endif
		}
	  } else {
		theAnaMgr->GetHisto2(314420)->Fill(xInPlane,yInPlane);
		if (fabs(xInPlane)>Xpla && fabs(yInPlane)>Ypla) {
#ifndef GAMOS_NO_VERBOSE
		  G4cout << "ControlHistos: Non-fat photon outside FOI! Event ID " << theEventID << " X " << xInPlane << " Y " << yInPlane << " Z1 " << pos1.z() << " Z2 " << pos2.z() << G4endl;
#endif
		}
	  }
	}
	if (aTrack->GetDefinition() == G4Electron::Electron()) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	  if( PhysicsVerb(debugVerb) ) G4cout << "ControlHistosUA: electron xInPlane / yInPlane / Weight  " << xInPlane << "  " << yInPlane << "  " << weight << G4endl;
#endif
#endif
	  theAnaMgr->GetHisto2(314516)->Fill(xInPlane,weight);
	  theAnaMgr->GetHisto2(314517)->Fill(yInPlane,weight);
	  theAnaMgr->GetHisto2(314518)->Fill(pos1.z(),weight);
	  if (weight > 0.99) {
		theAnaMgr->GetHisto2(314519)->Fill(xInPlane,yInPlane);
		if (fabs(xInPlane)<Xpla && fabs(yInPlane)<Ypla) {
#ifndef GAMOS_NO_VERBOSE
		  G4cout << "ControlHistos: Fat electron inside FOI! Event ID " << theEventID << " X " << xInPlane << " Y " << yInPlane << " Z1 " << pos1.z() << " Z2 " << pos2.z() << G4endl;
#endif
		}
	  } else {
		theAnaMgr->GetHisto2(314520)->Fill(xInPlane,yInPlane);
		if (fabs(xInPlane)>Xpla && fabs(yInPlane)>Ypla) {
#ifndef GAMOS_NO_VERBOSE
		  G4cout << "ControlHistos: Non-fat electron outside FOI! Event ID " << theEventID << " X " << xInPlane << " Y " << yInPlane << " Z1 " << pos1.z() << " Z2 " << pos2.z() << G4endl;
#endif
		}
	  }
	}
  }

  G4double enerParent = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector dirParent = aStep->GetPreStepPoint()->GetMomentumDirection();
  for ( size_t ii = 0; ii < fSec.size(); ii++ ) {
    G4Track* secoTrack = fSec[ii];
    if ( !IsSecondaryGamma(secoTrack) ) return;

    G4double weight2 = secoTrack->GetWeight();
    G4double ener = secoTrack->GetKineticEnergy();
    theAnaMgr->GetHisto1(314201)->Fill(ener,weight2);
    theAnaMgr->GetHisto1(314202)->Fill(enerParent,weight2);
    theAnaMgr->GetHisto1(314203)->Fill(ener-enerParent,weight2);
    theAnaMgr->GetHisto2(314204)->Fill(ener,enerParent,weight2);

    G4ThreeVector pos3 = aTrack->GetPosition();
    theAnaMgr->GetHisto1(314211)->Fill(pos3.x(),weight2);
    theAnaMgr->GetHisto1(314212)->Fill(pos3.y(),weight2);
    theAnaMgr->GetHisto1(314213)->Fill(pos3.z(),weight2);
    theAnaMgr->GetHisto1(314214)->Fill(pos3.perp(),weight2);
    theAnaMgr->GetHisto2(314215)->Fill(pos3.perp(),pos3.z(),weight2);

    G4ThreeVector dir = secoTrack->GetMomentumDirection();
    G4double angleVsParent = acos( dir*dirParent );

    theAnaMgr->GetHisto1(314221)->Fill(dir.theta(),weight2);
    theAnaMgr->GetHisto1(314222)->Fill(dir.phi(),weight2);
    theAnaMgr->GetHisto1(314223)->Fill(angleVsParent,weight2);
  }

}


//----------------------------------------------------------------
G4bool GmBSControlHistosUA::IsSecondaryGamma(const G4Track* aTrack )
{
  if ( aTrack->GetParentID() != 0 && aTrack->GetDefinition() == G4Gamma::Gamma() ){
    return TRUE;
  } else {
    return FALSE;
  }
}
