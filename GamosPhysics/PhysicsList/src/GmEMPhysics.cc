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
#include "GmEMPhysics.hh"
#include "GmPhysicsMessenger.hh"
#include "GmPhysicsGammaStandard.hh"
#include "GmPhysicsGammaLowEner.hh"
#include "GmPhysicsGammaPenelope.hh"
#include "GmPhysicsElectronStandard.hh"
#include "GmPhysicsElectronLowEner.hh"
#include "GmPhysicsElectronPenelope.hh"
#include "GmPhysicsPositronStandard.hh"
#include "GmPhysicsPositronPenelope.hh"
#include "GmPhysicsDecay.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4BosonConstructor.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4EmProcessOptions.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


GmEMPhysics::GmEMPhysics(): G4VModularPhysicsList()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 0.1 * CLHEP::mm;
  messenger = new GmPhysicsMessenger(this);
  SetVerboseLevel(1);
  ConstructParticles();


  G4String name;
  name = "gamma-lowener";
  G4cout << "GmEMPhysics PhysicsList:DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsGammaLowEner(name,22) );

  name = "electron-lowener";
  G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsElectronLowEner(name, 11) );

  name = "positron-standard"; 
  G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsPositronStandard(name, 111) );
}

GmEMPhysics::~GmEMPhysics()
{ 
  //  delete messenger;
}

#include "G4ParticleTable.hh"

void GmEMPhysics::ConstructParticles()
{
  G4BosonConstructor bosonC;
  bosonC.ConstructParticle();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    particleIterator->value();
  }

}

G4bool GmEMPhysics::ReplacePhysicsList(const G4String& name)
{
  G4cout << "GmEMPhysics::Replaceing PhysicsList component " << name << G4endl;
  
  //
  // Electromagnetic GamosEMLowEnPhysics. 
  //
  // The user can choose three alternative approaches:
  // Standard, Low Energy based on the Livermore libraries and Low Energy Penelope
  //
  G4bool bFound = true;

  // Replace standard processes for gammas
  if (name == "gamma-standard") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsGammaStandard(name,22) );
  
      // Replace LowE-EPDL processes for gammas
    } else if (name == "gamma-lowener"  
    || name == "gamma-epdl") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsGammaLowEner(name,22) );

  // Replace processes a' la Penelope for gammas
    } else if (name == "gamma-penelope")
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsGammaPenelope(name,22) );
  
      // Replace standard processes for electrons
    } else if (name == "electron-standard") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsElectronStandard(name,11) );
      
      // Replace LowE-EEDL processes for electrons
    } else if (name == "electron-lowener"  
      || name == "electron-eedl") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsElectronLowEner(name,11) );
      
      // Replace processes a' la Penelope for electrons
    } else if (name == "electron-penelope")
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsElectronPenelope(name,11) );
      
      // Replace standard processes for positrons
    } else if (name == "positron-standard") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsPositronStandard(name,111) );
      
      // Replace penelope processes for positrons
    } else if (name == "positron-penelope") 
    {
      G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      ReplacePhysics( new GmPhysicsPositronPenelope(name,111) );

  } else {
    bFound = false;
  }  
  
  return bFound;
}


void GmEMPhysics::SetCuts()
{  
  // Set the threshold of production equal to the defaultCutValue
  // in the experimental set-up
  G4VUserPhysicsList::SetCutsWithDefault();
    
  if (verboseLevel>0) DumpCutValuesTable();
}

     
void GmEMPhysics::ConstructProcess()
{
  //  AddTransportation();


  /* t G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin()WLWLS; itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructProcess();
    } */

  //  G4EmProcessOptions popt;
  // popt.SetApplyCuts(true);

  // Deexcitation
  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  if( G4bool(theParamMgr->GetNumericValue("GmEMPhysics:AtomicDeexcitation",1)) ){
    G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    G4bool bFluo = theParamMgr->GetNumericValue("AtomicDeexcitation:Fluorescence",1);
    G4bool bAuger = theParamMgr->GetNumericValue("AtomicDeexcitation:Auger",1);
    G4bool bPIXE = theParamMgr->GetNumericValue("AtomicDeexcitation:PIXE",0);
    if( bFluo) de->SetFluo(true);
    if( bAuger ) de->SetAuger(true);
    if( bPIXE ) de->SetPIXE(true);

    std::vector<G4String> regions;
    regions = theParamMgr->GetVStringValue("AtomicDeexcitation:Regions",regions); 
    if( regions.size() == 0 ) regions.push_back("DefaultRegionForTheWorld");
   for( size_t ii = 0; ii < regions.size(); ii++ ){
      de->SetDeexcitationActiveRegion(regions[ii], 1, bAuger, bPIXE );
    }
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);
  }
  
  G4VModularPhysicsList::ConstructProcess();
}
