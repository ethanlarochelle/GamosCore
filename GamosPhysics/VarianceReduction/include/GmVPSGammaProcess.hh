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
#ifndef GmVPSGammaProcess_h
#define GmVPSGammaProcess_h 1

#include "GmVPSModelHelper.hh"
#include "G4VEmProcess.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Track.hh"
#include "G4EmModelManager.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleChangeForGamma.hh"

class G4Step;
class G4VEmModel;
class GmVPSModelHelper;
class G4DataVector;
class G4VParticleChange;
class G4PhysicsTable;
class G4PhysicsVector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class GmVPSGammaProcess : public G4VEmProcess
{
public:

  GmVPSGammaProcess(const G4String& name,
	       G4ProcessType type = fElectromagnetic);

  virtual ~GmVPSGammaProcess();

  //------------------------------------------------------------------------
  // Virtual methods to be implemented in concrete processes
  //------------------------------------------------------------------------

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual void PrintInfo();

  GmVPSModelHelper* GetPSModel() const {
    return thePSModel; }

  void AddPSEmModel( GmVPSModelHelper* psmod ) {
    thePSModel = psmod; 
    //     G4cout << this << " AddPSEmModel " << thePSModel << " " << GetProcessName() << G4endl;
  }


protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*) = 0;

  //------------------------------------------------------------------------
  // Implementation of virtual methods common to all Discrete processes 
  //------------------------------------------------------------------------

public:
  // implementation of virtual method, specific for GmVPSGammaProcess
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  GmVPSModelHelper* thePSModel;
};

#endif
