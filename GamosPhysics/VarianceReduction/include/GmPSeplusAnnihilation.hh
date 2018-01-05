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
#ifndef GmPSeplusAnnihilation_h
#define GmPSeplusAnnihilation_h 1

#include "GmVPSGammaProcess.hh"
#include "G4Positron.hh"
#include "GmVPSEnergyLossProcess.hh"

class GmPSeplusAnnihilation : public GmVPSGammaProcess
{

public:

  GmPSeplusAnnihilation(const G4String& name = "annihil");

  virtual ~GmPSeplusAnnihilation();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& track,
                             const G4Step& stepData);

  virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            );

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  
  G4bool  isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
