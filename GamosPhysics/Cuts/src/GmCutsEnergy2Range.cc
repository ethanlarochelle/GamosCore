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
#include "GmCutsEnergy2Range.hh"

#include "G4Material.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4RunManager.hh"

#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"
#include "G4EnergyLossTables.hh"
#include "G4MaterialCutsCouple.hh"

//-----------------------------------------------------------------------
GmCutsEnergy2Range::GmCutsEnergy2Range()
{
  theR2EGamma = new G4RToEConvForGamma();
  theR2EGamma->SetVerboseLevel(0);
  theR2EElectron = new G4RToEConvForElectron();
  theR2EElectron->SetVerboseLevel(0);
  theR2EPositron = new G4RToEConvForPositron();
  theR2EPositron->SetVerboseLevel(0);
}

//-----------------------------------------------------------------------
GmCutsEnergy2Range::~GmCutsEnergy2Range()
{
}


//-----------------------------------------------------------------------
G4double GmCutsEnergy2Range::Convert( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple)  
{
  G4double rcut = 1.e-9*CLHEP::mm;
  if( part == G4Gamma::Gamma() ){
    rcut = Convert( theR2EGamma, ener, part, cutsCouple); 
  } else if( part == G4Electron::Electron() || 
	     part == G4Positron::Positron() ){
    rcut = G4EnergyLossTables::GetRange(part,ener,cutsCouple);
    //    G4cout << " G4EnergyLossTables rcut " << rcut << " ener " << ener << G4endl;
  } 

  return rcut;
}

//-----------------------------------------------------------------------
G4double GmCutsEnergy2Range::ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple)  
{
  return ConvertSlow( ener, part, cutsCouple->GetMaterial() );
}

//-----------------------------------------------------------------------
G4double GmCutsEnergy2Range::ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4Material* mate)  
{
  G4double rcut = 1.e-9*CLHEP::mm;
  if( part == G4Gamma::Gamma() ){
    rcut = Convert( theR2EGamma, ener, part, mate); 
  } else if( part == G4Electron::Electron() ){ 
    rcut = Convert( theR2EElectron, ener, part, mate); 
  } else if( part == G4Positron::Positron() ){
    rcut = Convert( theR2EPositron, ener, part, mate); 
  } 

  return rcut;
}


//-----------------------------------------------------------------------
G4double GmCutsEnergy2Range::Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple)  
{
  return Convert( R2E, ener, part, cutsCouple->GetMaterial()) ;
}

//-----------------------------------------------------------------------
G4double GmCutsEnergy2Range::Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* , const G4Material* mate)  
{
  G4double rcut = 1.e-9*CLHEP::mm;

  G4double diff = DBL_MAX;
  G4double diffold = 0.;
  G4int ie = 0;
  //  G4double rstep = 100.; // more precise, but slower
  //  G4int nrsteps = 7;
  G4double rstep = 100.;
  G4int nrsteps = 7;
  do{
    for( G4int ii = 0; ii < nrsteps; ii++ ){
      rcut *= rstep;
      ie++;
      G4double ecut = R2E->Convert(rcut,mate);
      diff = ener-ecut; 
	if( ie == 1 && diff <= 0. ) {
	  G4cerr << " GmCutsEnergy2Range: too low cut energy, do not trust result. Minimum energy = " << ecut << " MeV, > " << ener << " MeV; for range = " << rcut << " mm" << G4endl; 
	  rcut /= rstep;
	  ie = 1000;
	  break;
	}
	//	G4cout << "GmCutsEnergy2Range: MATE: " << mate->GetName() << "  PART: " << part->GetParticleName() << "  E CUT: " << ecut << " RANGE CUT: "<< rcut << "  E DIFF: "<< diff << " rstep " << rstep << G4endl;
	if( diff*diffold < 0 ) {
	  rcut /= rstep;
	  break;
	}
	diffold = diff;
    }
    rstep = std::pow(rstep,G4double(1./(nrsteps-1)));
  } while( fabs(diff)/ener > 1.e-2 && ie < 100 );
  
  rcut *= std::pow(rstep,G4double(nrsteps-1));
  //    G4cout << "GmCutsEnergy2Range:  MATERIAL:  " << mate->GetName() << "  PART: gamma  ENERGY CUT: " << ener << " (MeV)  = RANGE CUT: "<< rcut << " (diff in ener " << diff << G4endl;

  return rcut;
}
