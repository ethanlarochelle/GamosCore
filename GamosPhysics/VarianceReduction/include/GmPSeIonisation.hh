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
#ifndef GmPSeIonisation_h
#define GmPSeIonisation_h 1

#include "GmVPSEnergyLossProcess.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4VEmModel.hh"

class G4Material;
class G4ParticleDefinition;

class GmPSeIonisation : public GmVPSEnergyLossProcess
{

public:

  GmPSeIonisation(const G4String& name = "eIoni");

  virtual ~GmPSeIonisation();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
					   const G4ParticleDefinition*);

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition*,
				    const G4Material*, G4double cut);

private:

  // hide assignment operator
  GmPSeIonisation & operator=(const GmPSeIonisation &right);
  GmPSeIonisation(const GmPSeIonisation&);

  const G4ParticleDefinition* theElectron;
  const G4ParticleDefinition* particle;

  G4bool isElectron;
  G4bool isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
