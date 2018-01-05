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
// GmUserSteppingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserSteppingAction_h
#define GmUserSteppingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
#include "G4SteppingManager.hh"
class G4Step;
#include "G4TrackVector.hh"

class GmUserSteppingAction : public virtual GmUserAction
{
 public:
  GmUserSteppingAction();
  virtual ~GmUserSteppingAction();
  
  virtual void UserSteppingAction(const G4Step* ) = 0;

  G4TrackVector GetStepSecondaries();
  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetSteppingManager( G4SteppingManager* fptm ){
    fpSteppingManager = fptm; }

protected:
  G4SteppingManager* fpSteppingManager;

};

#endif
