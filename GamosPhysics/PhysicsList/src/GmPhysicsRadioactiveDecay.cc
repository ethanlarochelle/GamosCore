#include "GmPhysicsRadioactiveDecay.hh"
#include "G4ProcessManager.hh"

#include "G4RadioactiveDecay.hh"
#include "G4GenericIon.hh"
#include "G4ParticleTable.hh"
#include "G4LossTableManager.hh"
#include "G4NuclearLevelData.hh"
#include "G4PhysicsListHelper.hh"
#include "G4UAtomicDeexcitation.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::GmPhysicsRadioactiveDecay()
: theRadioactiveDecay(0)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetAugerCascade(true);
  param->AddMsc("world","G4RadioactiveDecay");

  G4NuclearLevelData::GetInstance()->GetParameters()->SetUseFilesNEW(true);
  G4NuclearLevelData::GetInstance()->GetParameters()->SetStoreAllLevels(true);
}

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::~GmPhysicsRadioactiveDecay()
{
  delete theRadioactiveDecay;
}
 
//----------------------------------------------------------------------
void GmPhysicsRadioactiveDecay::ConstructProcess()
{
  G4LossTableManager* man = G4LossTableManager::Instance();
  G4VAtomDeexcitation* ad = man->AtomDeexcitation();
  if(!ad) {
    ad = new G4UAtomicDeexcitation();
    man->SetAtomDeexcitation(ad);
    ad->InitialiseAtomicDeexcitation();
  }

  G4PhysicsListHelper::GetPhysicsListHelper()->
    RegisterProcess(new G4RadioactiveDecay(), G4GenericIon::GenericIon());

}


