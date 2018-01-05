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
#ifndef GmExtraPhysicsMessenger_HH
#define GmExtraPhysicsMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include <map>
#include <vector>

class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class GmUIcmdWithAString;
class G4LivermorePhotoElectricModel;
class G4LivermoreIonisationModel;
class G4VEmProcess;
class G4VEnergyLossProcess;

typedef std::multimap<G4String,G4String> mmss;

class GmExtraPhysicsMessenger: public G4UImessenger {

public:
  
  GmExtraPhysicsMessenger();
  
  ~GmExtraPhysicsMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  void AddPhysics(G4String newValue);
  void RemoveProcessesByType(G4String newValue);
  void RemoveProcessesByName(G4String newValue);
  void RemoveProcessesByParticleAndName(G4String newValue);
  void ReplaceModelSet(G4String newValue);  
  void ReplaceModel(G4String newValue);
  void AddDeexcitation();
  void Replace1Model(G4String newParticleName, G4String newProcessName, G4String newModelName);

private:
  G4UIcmdWithABool* theApplyCutsCmd;
  G4UIcmdWithoutParameter* theParallelPhysCmd;
  GmUIcmdWithAString* theAddPhysicsCmd;
  GmUIcmdWithAString* theRemoveProcessesByTypeCmd;
  GmUIcmdWithAString* theRemoveProcessesByNameCmd;
  GmUIcmdWithAString* theRemoveProcessesByParticleAndNameCmd;
  GmUIcmdWithAString* theReplaceModelSetCmd;
  GmUIcmdWithAString* theReplaceModelCmd;

};
#endif
  
