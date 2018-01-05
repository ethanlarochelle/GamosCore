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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "GmBSLivermoreBremsstrahlungModel.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ParticleChangeForLoss.hh"
#include "G4Generator2BS.hh"

#include "G4Physics2DVector.hh"
#include "G4Exp.hh"
#include "G4Log.hh"

#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmBSLivermoreBremsstrahlungModel::GmBSLivermoreBremsstrahlungModel(
  const G4ParticleDefinition* p, const G4String& nam)
  : G4LivermoreBremsstrahlungModel(p,nam)
{
  SetLowEnergyLimit(10.0*eV);
  SetLPMFlag(false);
  nwarn = 0;
  idx = idy = 0;
  SetAngularDistribution(new G4Generator2BS());
  theAnaMgr = GmAnalysisMgr::GetInstance("testBrems");
  theAnaMgr->CreateHisto1D("ENERGY LOST BY e-",120,0.,6.,98229);
  theAnaMgr->CreateHisto1D("DIR THETA ",180,0.,180.,98230);
  theAnaMgr->CreateHisto1D("ENERGY BREMS ALL",120,0.,6.,98231);
  theAnaMgr->CreateHisto1D("ENERGY BREMS ACCEPTED",120,0.,6.,98232);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED",120,0.,6.,98233);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED PASSED RR",120,0.,6.,98234);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED NOT PASSED RR",120,0.,6.,98235);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmBSLivermoreBremsstrahlungModel::~GmBSLivermoreBremsstrahlungModel()
{
  if(IsMaster()) {
    for(size_t i=0; i<101; ++i) { 
      if(dataSB[i]) {
	delete dataSB[i]; 
	dataSB[i] = 0;
      } 
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmBSLivermoreBremsstrahlungModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries, 
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicParticle,
		    G4double cutEnergy,
		    G4double maxEnergy,
		    const G4Track& track)
{
 //PS<
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(+testVerb) ) G4cout << " GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif

  G4ThreeVector position = track.GetPosition();
  G4bool bIsApplicable = IsApplicable(position.z());  
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "  GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    //g494    SampleSecondaries(secondaries, currentCouple, aDynamicParticle, tcut, 100.*TeV );
    SampleSecondaries(secondaries, couple, aDynamicParticle, cutEnergy, 100.*CLHEP::TeV );
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }

  const G4double epeaklimit= 300*CLHEP::MeV; 
  const G4double elowlimit = 20*CLHEP::keV; 
  //PS>
  
  G4double kineticEnergy = aDynamicParticle->GetKineticEnergy();
  G4double cut  = std::min(cutEnergy, kineticEnergy);
  G4double emax = std::min(maxEnergy, kineticEnergy);
  if(cut >= emax) { return; }

 theWgt = track.GetWeight();   //PS<
  theNewNSplit = theNSplit;
  if( theWgt > 0.99 ) {
    theWgt *= theNSplitInv;   
  } else {
    theNewNSplit = 1;
  }
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(+debugVerb) ) G4cout << "  GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS wgt " << theWgt << " NSplit " << theNewNSplit << G4endl;
#endif

  G4int nCOUNT = 0;
  G4int nCOUNTACC = 0;
  G4ThreeVector gammaDirection;
  G4double gammaEnergy, v; 
  G4double totMomentum;

  //PS>

  G4double tGamma = 0.;

  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    
    SetupForMaterial(particle, couple->GetMaterial(), kineticEnergy);
    
    const G4Element* elm = 
      SelectRandomAtom(couple,particle,kineticEnergy,cut,emax);
    SetCurrentElement(elm->GetZ());
    G4int Z = G4int(currentZ);
    
    totalEnergy = kineticEnergy + particleMass;
    densityCorr = densityFactor*totalEnergy*totalEnergy;
    totMomentum = sqrt(kineticEnergy*(totalEnergy + electron_mass_c2));
    /*
      G4cout << "G4LivermoreBremsstrahlungModel::SampleSecondaries E(MeV)= " 
      << kineticEnergy/MeV
      << " Z= " << Z << " cut(MeV)= " << cut/MeV 
      << " emax(MeV)= " << emax/MeV << " corr= " << densityCorr << G4endl;
    */
    G4double xmin = G4Log(cut*cut + densityCorr);
    G4double xmax = G4Log(emax*emax  + densityCorr);
    G4double y = G4Log(kineticEnergy/MeV);
    
    // majoranta
    G4double x0 = cut/kineticEnergy;
    G4double vmax = dataSB[Z]->Value(x0, y, idx, idy)*1.02;
    //  G4double invbeta1 = 0;
    
    // majoranta corrected for e-
    if(isElectron && x0 < 0.97 && 
       ((kineticEnergy > epeaklimit) || (kineticEnergy < elowlimit))) {
      G4double ylim = std::min(ylimit[Z],1.1*dataSB[Z]->Value(0.97,y,idx,idy));
      if(ylim > vmax) { vmax = ylim; }
    }
    if(x0 < 0.05) { vmax *= 1.2; }
    
    //G4cout<<"y= "<<y<<" xmin= "<<xmin<<" xmax= "<<xmax
    //<<" vmax= "<<vmax<<G4endl;
    //  G4int ncount = 0;
    do {
      //++ncount;
      G4double x = G4Exp(xmin + G4UniformRand()*(xmax - xmin)) - densityCorr;
      if(x < 0.0) { x = 0.0; }
      gammaEnergy = sqrt(x);
      G4double x1 = gammaEnergy/kineticEnergy;
      v = dataSB[Z]->Value(x1, y, idx, idy);
      
      // correction for positrons        
      if(!isElectron) {
	G4double e1 = kineticEnergy - cut;
	G4double invbeta1 = (e1 + particleMass)/sqrt(e1*(e1 + 2*particleMass));
	G4double e2 = kineticEnergy - gammaEnergy;
	G4double invbeta2 = (e2 + particleMass)/sqrt(e2*(e2 + 2*particleMass));
	G4double xxx = twopi*fine_structure_const*currentZ*(invbeta1 - invbeta2);
	
	if(xxx < expnumlim) { v = 0.0; }
	else { v *= G4Exp(xxx); }
      }
      
      if (v > 1.05*vmax && nwarn < 5) {
	++nwarn;
	G4ExceptionDescription ed;
	ed << "### G4LivermoreBremsstrahlungModel Warning: Majoranta exceeded! "
	   << v << " > " << vmax << " by " << v/vmax
	   << " Egamma(MeV)= " << gammaEnergy
	   << " Ee(MeV)= " << kineticEnergy
	   << " Z= " << Z << "  " << particle->GetParticleName();
	
	if ( 20 == nwarn ) {
	  ed << "\n ### GmBSLivermoreBremsstrahlungModel Warnings stopped";
	}
	G4Exception("GmBSLivermoreBremsstrahlungModel::SampleScattering","em0044",
		    JustWarning, ed,"");
	
      }
    } while (v < vmax*G4UniformRand());
    
    //
    // angles of the emitted gamma. ( Z - axis along the parent particle)
    // use general interface
    //
    
    gammaDirection = 
      GetAngularDistribution()->SampleDirection(aDynamicParticle, totalEnergy-gammaEnergy,
						Z, couple->GetMaterial());
    
    
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(+debugVerb) ) G4cout << " GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS  gammaDir " << gammaDirection << " elecDir " << aDynamicParticle->GetMomentumDirection() << G4endl;
#endif
    theAnaMgr->GetHisto1(98230)->Fill(gammaDirection.theta()/deg,1.); 
    
    theAnaMgr->GetHisto1(98231)->Fill(tGamma,1.); 
    nCOUNT++;
    if( AcceptGamma(gammaDirection,position) ) {   //PS<
    nCOUNTACC++;
      theNewWgt = theWgt;
      theAnaMgr->GetHisto1(98232)->Fill(tGamma,theNewWgt); 
    } else {
      theAnaMgr->GetHisto1(98233)->Fill(tGamma,theWgt); 
      //- Try Russian Roulette
      
      if ( RussRoul() ) {
	theNewWgt = 1.;
	theAnaMgr->GetHisto1(98234)->Fill(tGamma,theNewWgt); 
      } else {
	theAnaMgr->GetHisto1(98235)->Fill(tGamma,theWgt); 
	continue;
      }
    }    //PS>
  }

  // create G4DynamicParticle object for the Gamma
  G4DynamicParticle* gamma = 
    new G4DynamicParticle(theGamma,gammaDirection,gammaEnergy);
  secondaries->push_back(gamma);
   secondaryWeights.push_back( theNewWgt );
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(+debugVerb) ) G4cout << "GmBSLivermoreBremsstrahlungModel new particle of weight " << theNewWgt << " " << theNewNSplit << G4endl;
#endif

  G4ThreeVector direction = (totMomentum*aDynamicParticle->GetMomentumDirection()
			     - gammaEnergy*gammaDirection).unit();

  /*
  G4cout << "### G4SBModel: v= "
	 << " Eg(MeV)= " << gammaEnergy
	 << " Ee(MeV)= " << kineticEnergy
	 << " DirE " << direction << " DirG " << gammaDirection
	 << G4endl;
  */
  // energy of primary
  G4double finalE = kineticEnergy - gammaEnergy;

  // stop tracking and create new secondary instead of primary
  if(gammaEnergy > SecondaryThreshold()) {
    fParticleChange->ProposeTrackStatus(fStopAndKill);
    fParticleChange->SetProposedKineticEnergy(0.0);
    G4DynamicParticle* el = 
      new G4DynamicParticle(const_cast<G4ParticleDefinition*>(particle),
			    direction, finalE);
    secondaries->push_back(el);

    // continue tracking
  } else {
    fParticleChange->SetProposedMomentumDirection(direction);
    fParticleChange->SetProposedKineticEnergy(finalE);
  }
  
  theAnaMgr->GetHisto1(98229)->Fill(tGamma,theNewWgt); 

}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


