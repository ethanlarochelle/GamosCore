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
#ifndef GmEMStandardPhysics_h
#define GmEMStandardPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class GmEMStandardPhysicsMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEMStandardPhysics: public G4VModularPhysicsList
{
public:
  GmEMStandardPhysics();
  virtual ~GmEMStandardPhysics();

  void ConstructParticle();
        
  void ReplacePhysicsList(const G4String& name);
    
  void ConstructProcess();    
  void AddDecay();
  void SelectBremssAngularDist();
    
  void SetCuts();
  //  void SetCutForGamma(G4double);
  //  void SetCutForElectron(G4double);
  //  void SetCutForPositron(G4double);
    
private:
  
  GmEMStandardPhysicsMessenger* pMessenger; 

  G4String emName;
  G4VPhysicsConstructor*  emPhysics;
    
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

