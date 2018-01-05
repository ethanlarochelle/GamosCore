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
#include "GmPhysicsGammaNuclear.hh"
#include "G4ProcessManager.hh"
#include "G4Gamma.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSModel.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
using namespace CLHEP;

GmPhysicsGammaNuclear::GmPhysicsGammaNuclear(const G4String& name, G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsGammaNuclear::~GmPhysicsGammaNuclear()
{ }

void GmPhysicsGammaNuclear::ConstructProcess()
{
  G4CascadeInterface * lowEGammaModel = new G4CascadeInterface();
  lowEGammaModel->SetMaxEnergy(3.5*CLHEP::GeV);
  
  G4TheoFSGenerator * highEGammaModel(new G4TheoFSGenerator);
  highEGammaModel->SetTransport(new G4GeneratorPrecompoundInterface);
  
  G4QGSModel<G4GammaParticipants> * stringModel(new G4QGSModel<G4GammaParticipants>);
  stringModel->SetFragmentationModel(new G4ExcitedStringDecay(new G4QGSMFragmentation));
  
  highEGammaModel->SetHighEnergyGenerator(stringModel);
  highEGammaModel->SetMinEnergy(3.*CLHEP::GeV);
  highEGammaModel->SetMaxEnergy(100.*TeV); 
  
  G4ProcessManager * pmanager=G4Gamma::GammaDefinition()->GetProcessManager();
  if( GmG4Utils::CheckProcessExists( pmanager, "" )) return;
  G4PhotoNuclearProcess * photoNuclearProcess(new G4PhotoNuclearProcess);
  photoNuclearProcess->RegisterMe(lowEGammaModel);
  photoNuclearProcess->RegisterMe(highEGammaModel);
  pmanager->AddDiscreteProcess(photoNuclearProcess);
}
