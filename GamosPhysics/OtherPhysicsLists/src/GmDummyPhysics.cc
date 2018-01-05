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
#include "globals.hh"
#include "GmDummyPhysics.hh"

#include "G4BosonConstructor.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

GmDummyPhysics::GmDummyPhysics(): G4VModularPhysicsList()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 1. * CLHEP::km;

  ConstructParticles();

}

GmDummyPhysics::~GmDummyPhysics()
{ 
}

void GmDummyPhysics::ConstructParticles()
{
  G4BosonConstructor bosonC;
  bosonC.ConstructParticle();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();

}

void GmDummyPhysics::SetCuts()
{  
  // Set the threshold of production equal to the defaultCutValue
  // in the experimental set-up
  G4VUserPhysicsList::SetCutsWithDefault();
    
  if (verboseLevel>0) DumpCutValuesTable();
}

     
void GmDummyPhysics::ConstructProcess()
{
  AddTransportation();

}
                                                                                
