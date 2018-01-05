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
// GmUserTrackingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserTrackingAction_h
#define GmUserTrackingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
#include "G4TrackingManager.hh"
class G4Track;

class GmUserTrackingAction : public virtual GmUserAction
{
public:
  GmUserTrackingAction();
  virtual ~GmUserTrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track* ){};
  virtual void PostUserTrackingAction(const G4Track* ){};

  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetTrackingManager( G4TrackingManager* fptm ){
    fpTrackingManager = fptm; }

protected:
  G4TrackingManager* fpTrackingManager;

};

#endif
