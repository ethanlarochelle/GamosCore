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
// GmUserEventActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4EventActions: 
//   This class is registered to G4EventManager.
//   Other G4EventAction classes are registered to this
//  and the BeginOfEventAction/EndOfEventAction method of this class just makes a 
//  loop to the BeginOfEventAction/EndOfEventAction methods of the classes registered to it

#ifndef GmUserEventActionList_h
#define GmUserEventActionList_h 1

#include "G4UserEventAction.hh"
class GmUserEventAction;
#include "globals.hh"
#include <vector>
#include <map>
class G4Event;

typedef std::vector<GmUserEventAction*> vUEventA;

class GmUserEventActionList : public G4UserEventAction
{
 public:
  GmUserEventActionList();
  ~GmUserEventActionList();
  
 public:
  virtual void BeginOfEventAction(const  G4Event* anEvent);
  virtual void EndOfEventAction(const  G4Event* anEvent);
  
 public:
  void AddAction( GmUserEventAction* ua  );
  void RemoveAction( GmUserEventAction* ua  );

  GmUserEventAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);

  void DumpActions( std::ostream& out, G4int enabled);
  
  vUEventA GetEventActions() const {
    return theEventActions;
  }

 private:
  vUEventA theEventActions;
};

#endif
