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
#include "GmPSEMPhysics.hh"

//#include "GmPhysicsGammaEWPSLowEner.hh"
//#include "GmPhysicsGammaEWPSStandard.hh"
#include "GmPhysicsPositronUBSStandard.hh"
#include "GmPhysicsPositronZBSStandard.hh"
//#include "GmPhysicsPositronEWPSStandard.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsGammaStandard.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsPositronStandard.hh"
//#include "GmPhysicsElectronEWPSLowEner.hh"
#include "GmPhysicsElectronUBSLowEner.hh"
#include "GmPhysicsElectronZBSLowEner.hh"

#include "GmPhysicsElectronLowEnerDeexSplit.hh"
#include "GmPhysicsElectronPenelopeDeexSplit.hh"
#include "GmPhysicsGammaLowEnerDeexSplit.hh"
#include "GmPhysicsGammaPenelopeDeexSplit.hh"

#include "GmPhysicsElectronLowEnerUBSDeexSplit.hh"
#include "GmPhysicsElectronLowEnerZBSDeexSplit.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
using namespace CLHEP;

GmPSEMPhysics::GmPSEMPhysics(): GmEMPhysics()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 0.1 * mm;
  SetVerboseLevel(1);
  ConstructParticles();

}

GmPSEMPhysics::~GmPSEMPhysics()
{ 
}


void GmPSEMPhysics::ConstructParticles()
{
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
}

G4bool GmPSEMPhysics::ReplacePhysicsList(const G4String& name)
{
  G4bool bFound = GmEMPhysics::ReplacePhysicsList(name);

  if( bFound ) return bFound; 

  bFound = true;
  G4cout << "GmpSEMPhysics::Replacing PhysicsList component " << name << G4endl;
  
  if( name == "electron-lowener-UBS" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronUBSLowEner("electron-lowener-UBS") );
  } else if( name == "electron-lowener-ZBS" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronZBSLowEner("electron-lowener-ZBS") );
  } else if( name == "electron-lowener-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronLowEnerDeexSplit("electron-lowener-DeexSplit") );
  } else if( name == "electron-penelope-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronPenelopeDeexSplit("electron-penelope-DeexSplit") );
  } else if( name == "electron-lowener-UBS-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronLowEnerUBSDeexSplit("electron-lowener-DeexSplit") );
  } else if( name == "electron-lowener-ZBS-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronLowEnerZBSDeexSplit("electron-lowener-DeexSplit") );

  } else if( name == "positron-standard-UBS" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsPositronUBSStandard("positron-standard-UBS") );
  } else if( name == "positron-standard-ZBS" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsPositronZBSStandard("positron-standard-UBS") );

  } else if( name == "gamma-lowener-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsGammaLowEnerDeexSplit("gamma-lowener-DeexSplit") );
  } else if( name == "gamma-penelope-DeexSplit" ){
    G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	   << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsGammaPenelopeDeexSplit("gamma-penelope-DeexSplit") );

  } else {
    bFound = false;
  }

  return bFound;

}
                                                                                
