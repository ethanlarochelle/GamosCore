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
#ifndef __GmProdCutsStudyUA__
#define __GmProdCutsStudyUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "GmCutsStudyMgr.hh"

class GmAnalysisMgr;
class GmCSTrackInfo;
class G4Region;
class G4VProcess;
#include <map>

//-typedef std::vector<std::pair<G4VProcess*, G4int> > mpi;
typedef std::map<G4VProcess*, G4int> mpi;
typedef std::multimap<G4ParticleDefinition*, mpi* > mmppi;
typedef std::map< G4Region*, mmppi* > mrmmppi;
///typedef std::vector<std::pair<G4VProcess*, G4double> > mpd;
typedef std::map<G4VProcess*, G4double> mpd;
typedef std::multimap<G4ParticleDefinition*, mpd* > mmppd;
typedef std::map< G4Region*, mmppd* > mrmmppd;

typedef std::pair<G4int, mmppd* > pimmppd;
typedef std::pair<G4ParticleDefinition*, G4VProcess*> ppp;


class GmProdCutsStudyUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmProdCutsStudyUA();
  ~GmProdCutsStudyUA(){};
  virtual void BeginOfRunAction(const G4Run* ); 
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run* );
  pimmppd HistosAndStatistics(G4Region* reg, G4ParticleDefinition* part, G4VProcess* proc);
  void BookHistos( G4int ih, G4String hnam );
  void CreateTrackInfo(const G4Track* aTrack );
  void SetTrackInfoAccepted();

private:
  void BookHistosAndStatistics();
  G4String GetRegionName(G4Region* reg);
  G4String GetParticleName(G4ParticleDefinition* part);
  G4String GetProcessName(G4VProcess* proc);
  void BuildRegionIDs();

  GmAnalysisMgr* theAnaMgr;
  GmCSTrackInfo* theCurrentTrackInfo;
  mrmmppi theHistosIDs;
  mrmmppd theRanges;
  std::map<G4Region*,G4int> theRegionIDs;
  std::map<ppp,G4int> thePartProcIDs;

protected:
  G4int theNAccepted;
  G4int theNAcceptedAncestors;
};

#endif
