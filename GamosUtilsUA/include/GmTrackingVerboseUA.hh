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
#ifndef GmTrackingVerboseUA_h
#define GmTrackingVerboseUA_h 1

///////////////////////////////////////////////////////////////////////////////
// File : TrackingVerboseAction.h
// Author: P.Arce  12.09.01
// Description:
// Modifications:
// Class with the commands to switch on/off the verbosity of tracking and event, see TrackingVerboseAction for a detailed explanation
// for a given range of tracks each 'n' tracks
// the GEANT4 command '/tracking/verbose N' will be executed when the trackNo is
//     fTVTrackMin <= trackNo <= fTVTrackMax
// each fTVTrackStep tracks (starting at 1, not 0) and if the trackNo is
//     fTVTrackMin <= trackNo <= fTVTrackMax
// each fTVTrackStep tracks (starting at 1, not 0)
// 
///////////////////////////////////////////////////////////////////////////////

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "G4TrackingManager.hh"

class GmTrackingVerboseUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction
{
public:
  GmTrackingVerboseUA();
  ~GmTrackingVerboseUA(){};
  
 private:
  void SetTrackingVerbose( int verblev );

 public:
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

 private:
  int fTVTrackMin;
  int fTVTrackMax;
  int fTVTrackStep;
  int fTVEventMin;
  int fTVEventMax;
  int fTVEventStep;

  int fVerboseLevel;

  bool fTrackingVerboseON;
  bool fTkVerbThisEventON;

  G4TrackingManager* theTrackingManager;
};

#endif
