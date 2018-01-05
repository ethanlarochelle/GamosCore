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
#ifndef GmPDSCreateAngleTablesUA_h
#define GmPDSCreateAngleTablesUA_h

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include <set>
class G4Material;

class GmPDSCreateAngleTablesUA : public GmUserRunAction, 
				 public GmUserEventAction,
				 public GmUserSteppingAction
{
  public:
  GmPDSCreateAngleTablesUA();
  ~GmPDSCreateAngleTablesUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  void BookHistos(G4String energyName, G4String mateName );
  G4Track* GetSecondaryTrackSameType();
  void Initialise( const G4Event* evt );

private:
  G4String thePrimaryParticleName;
  G4String theSecondaryParticleName;

  GmAnalysisMgr* theAnaMgr;

  std::map<G4String,int> theProcesses;

  G4double theLastEnergy;
  G4Material* theLastMaterial;
  G4int theCurrentEnergyID;
  G4int theCurrentMateID;
  std::map<G4double,int> theEnergyIDs;
  std::map<G4Material*,int> theMaterialIDs;

  const G4ProcessVector* theProclis;
  G4bool bInitialised;

  std::set<G4String> theHistos;
};
#endif
