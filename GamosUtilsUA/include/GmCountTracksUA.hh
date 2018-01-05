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
#ifndef __GmCountTracksUA__
#define __GmCountTracksUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"

class G4Run;
class G4Event;
class G4Track;

class GmCountTracksUA : public GmUserRunAction, 
			public GmUserEventAction, 
			public GmUserTrackingAction
{
public:
  GmCountTracksUA();
  ~GmCountTracksUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void PostUserTrackingAction(const G4Track* aTrack);

private:
  G4double theNTracks;
  G4int theNTracksThisEvent;
  G4int theEachNEvent;
  G4int theFirstEvent;

};

#endif
