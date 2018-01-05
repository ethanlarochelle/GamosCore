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
#ifndef GmCheckOriginalGamma_hh
#define GmCheckOriginalGamma_hh

#include "globals.hh"
class G4Track;
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include <set>
#include <map>

class GmCheckOriginalGamma : public GmUserEventAction, public GmUserTrackingAction
{
  public:
  GmCheckOriginalGamma();
  ~GmCheckOriginalGamma(){};

  virtual void BeginOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

  static G4int CheckIfOriginalGamma(const G4Track* aTrack);
  static G4int GetOriginalGamma(const G4Track* aTrack);

  static void SetCheckParentID( G4int val ){
    bCheckParentID = val;
  }

private:
  static std::set<G4int> theOrigGammas;
  static std::map<G4int,G4int> theTrackAndParentIDs;
  static std::map<G4int,G4bool> isFromRadiactiveDecay;

  static G4int bCheckParentID;
};
#endif
