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
#include "GmPhysicsGammaPenelopeDeexSplit.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPSPhotoElectricEffect.hh"
#include "GmDeexSplitPenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "GmDeexSplitPenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

#include "G4RegionStore.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
using namespace CLHEP;


GmPhysicsGammaPenelopeDeexSplit::GmPhysicsGammaPenelopeDeexSplit(const G4String& name): GmVPhysicsGamma(name,22)
{ }

GmPhysicsGammaPenelopeDeexSplit::~GmPhysicsGammaPenelopeDeexSplit()
{ }

void GmPhysicsGammaPenelopeDeexSplit::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4double PenelopeHighEnergyLimit = CLHEP::GeV;
  while( (*theParticleIterator)() )
  {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
    if (particleName == "gamma") {

      GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect();
      GmDeexSplitPenelopePhotoElectricModel* photoElectricModel = new GmDeexSplitPenelopePhotoElectricModel();
      AddDeexcitation();
      photoElectricModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      photoElectric->AddEmModel(0, photoElectricModel);
      photoElectric->AddPSEmModel(photoElectricModel);
      pmanager->AddDiscreteProcess(photoElectric);

      G4ComptonScattering* compt = new G4ComptonScattering();
      GmDeexSplitPenelopeComptonModel* comptModel = 
	new GmDeexSplitPenelopeComptonModel();
      comptModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);

      G4GammaConversion* conv = new G4GammaConversion();
      G4PenelopeGammaConversionModel* convModel = 
	new G4PenelopeGammaConversionModel();
      convModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4PenelopeRayleighModel* raylModel = new G4PenelopeRayleighModel();
      raylModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);

    }
  }
}
