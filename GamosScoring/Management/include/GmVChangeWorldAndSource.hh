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
 #ifndef GmVChangeWorldAndSource_h
#define GmVChangeWorldAndSource_h

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
class G4PVPlacement;
#include <map>

class GmVChangeWorldAndSource : public GmUserEventAction, 
				public GmUserStackingAction
{
  public:
  GmVChangeWorldAndSource();
  ~GmVChangeWorldAndSource(){};
  virtual void BeginOfEventAction(const G4Event* evt);
  virtual void EndOfEventAction(const G4Event* evt);
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void InitialiseRun();

protected:
  void BuildWorlds();
  G4PVPlacement* BuildWorld(G4Material* mate);
  void BuildSources();
  virtual void ChangeWorldOrSource();
  virtual void ChangeWorld();
  virtual void ChangeSource();

protected:
  G4bool bInitialised;
  G4String thePrimaryParticleName;
  G4String theSecondaryParticleName;
  G4int theCurrentWorld;
  G4int theCurrentSource;
  G4double theNEventsPerRun;
  G4double theMinE, theMaxE;
  unsigned int theNstepsE;
  G4bool bLogE;
  std::map<G4Material*,G4PVPlacement*> theWorlds;
  std::map<G4double,G4String> theSourceCommands;
  G4bool bTooManyEvents;

};
#endif
