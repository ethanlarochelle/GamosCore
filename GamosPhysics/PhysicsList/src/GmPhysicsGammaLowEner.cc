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
#include "G4LivermorePhotoElectricModel.hh"
#include "GmPhysicsGammaLowEner.hh"
#include "GmPhysicsVerbosity.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "G4PhotoElectricEffect.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4RegionStore.hh"

GmPhysicsGammaLowEner::GmPhysicsGammaLowEner(const G4String& name, G4int type): GmVPhysicsGamma(name,type)
{ }

GmPhysicsGammaLowEner::~GmPhysicsGammaLowEner()
{ }

void GmPhysicsGammaLowEner::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4double LivermoreHighEnergyLimit = CLHEP::GeV;
  G4cout << "GmPhysicsGammaLowEner::ConstructProcess " << G4endl;
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4ProcessManager* pmanager = particle -> GetProcessManager();
    G4String particleName = particle -> GetParticleName();
    
    if (particleName == "gamma") {

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* photModel = new G4LivermorePhotoElectricModel();

      photModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0, photModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4LivermoreComptonModel* theLivermoreComptonModel = 
	new G4LivermoreComptonModel();
      theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = 
	new G4LivermoreGammaConversionModel();
      theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4LivermoreRayleighModel* theRayleighModel = new G4LivermoreRayleighModel();
      theRayleighModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theRayleigh->AddEmModel(0, theRayleighModel);
      pmanager->AddDiscreteProcess(theRayleigh);
      
    }
  }
}
