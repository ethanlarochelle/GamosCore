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
// GmUserStackingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserStackingAction_h
#define GmUserStackingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
class G4Track;
#include "G4StackManager.hh"

class GmUserStackingAction : public virtual GmUserAction
{
 public:
  GmUserStackingAction();
  virtual ~GmUserStackingAction();
  
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) = 0;
  virtual void NewStage(){};
  virtual void PrepareNewEvent(){}; 

  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetStackManager( G4StackManager* fsm ){
    stackManager = fsm; 
  }
  
 protected:
  G4StackManager* stackManager;

};

#endif
