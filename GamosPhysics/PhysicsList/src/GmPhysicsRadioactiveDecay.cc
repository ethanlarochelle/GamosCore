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


