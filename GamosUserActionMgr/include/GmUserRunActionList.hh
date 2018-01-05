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
// GmUserRunActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4RunActions: 
//   This class is registered to G4RunManager.
//   Other G4RunAction classes are registered to this
//  and the BeginOfRunAction/EndOfRunAction method of this class just makes a 
//  loop to the BeginOfRunAction/EndOfRunAction methods of the classes registered to it

#ifndef GmUserRunActionList_h
#define GmUserRunActionList_h 1

#include "G4UserRunAction.hh"
class GmUserRunAction;
#include "globals.hh"
#include <vector>
#include <map>
class G4Run;

typedef std::vector<GmUserRunAction*> vURunA;

class GmUserRunActionList : public G4UserRunAction
{

 public:
  GmUserRunActionList();
  ~GmUserRunActionList();
  
 public:
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);
  virtual G4Run* GenerateRun();

 public:
  void AddAction( GmUserRunAction* ua );
  void RemoveAction( GmUserRunAction* ua );

  GmUserRunAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);

  void DumpActions( std::ostream& out, G4int enabled);

  vURunA GetRunActions() const {
    return theRunActions;
  }
  
 private:
  vURunA theRunActions;
};

#endif
