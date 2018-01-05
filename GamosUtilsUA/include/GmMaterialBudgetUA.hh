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
#ifndef GmMaterialBudgetUA_hh
#define GmMaterialBudgetUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <set>
#include <map>

class GmMaterialBudgetUA : public GmUserRunAction, 
		       public GmUserSteppingAction, 
		       public GmUserTrackingAction, 
		       public GmVHistoBuilder
{
public:
  GmMaterialBudgetUA();
  ~GmMaterialBudgetUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* aTrack );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:
  void BookHistos(G4int index);

  std::set<G4int> theHistos;

  std::map<G4int,G4double> theMatBud;
};

#endif
