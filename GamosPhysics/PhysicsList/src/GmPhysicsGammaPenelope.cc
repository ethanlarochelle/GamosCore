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
#include "GmPhysicsGammaPenelope.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4PenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4PenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

GmPhysicsGammaPenelope::GmPhysicsGammaPenelope(const G4String& name, G4int type): GmVPhysicsGamma(name,type)
{ }

GmPhysicsGammaPenelope::~GmPhysicsGammaPenelope()
{ }

void GmPhysicsGammaPenelope::ConstructProcess()
{
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator ->reset();

  G4double PenelopeHighEnergyLimit = 1.0*CLHEP::GeV;
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4ProcessManager* pmanager = particle -> GetProcessManager();
    G4String particleName = particle -> GetParticleName();
    
    if (particleName == "gamma") 
    {

      //Photo-electric effect
      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4PenelopePhotoElectricModel* thePEPenelopeModel = new 
	G4PenelopePhotoElectricModel();
      thePEPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0,thePEPenelopeModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);
      
      //Compton scattering
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4PenelopeComptonModel* theComptonPenelopeModel = 
	new G4PenelopeComptonModel();
      theComptonPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theComptonScattering->AddEmModel(0,theComptonPenelopeModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      //Gamma conversion
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4PenelopeGammaConversionModel* theGCPenelopeModel = 
	new G4PenelopeGammaConversionModel();
      theGammaConversion->AddEmModel(0,theGCPenelopeModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      //Rayleigh scattering
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4PenelopeRayleighModel* theRayleighPenelopeModel = 
	new G4PenelopeRayleighModel();
      theRayleighPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theRayleigh->AddEmModel(0,theRayleighPenelopeModel);
      pmanager->AddDiscreteProcess(theRayleigh);
    }   
  }
}
