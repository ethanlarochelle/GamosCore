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
#ifndef GmInheritTrackInfoUA_hh
#define GmInheritTrackInfoUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <vector>
class  G4SteppingManager;

class GmInheritTrackInfoUA : public GmUserRunAction,
			     public GmUserSteppingAction
{
public:
  GmInheritTrackInfoUA();
  ~GmInheritTrackInfoUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction(const G4Step* );

  void SetTrackInfoName( const G4String name ) {
    theTrackInfoName = name;
  }

  void AddBoolValueName( const G4String name ) {
    theBoolValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddIntValueName( const G4String name ) {
    theIntValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddDoubleValueName( const G4String name ) {
    theDoubleValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddStringValueName( const G4String name ) {
    theStringValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddThreeVectorValueName( const G4String name ) {
    theThreeVectorValueNames.push_back(name);
    bSomethingToInherit = true;
  }

private:
  std::vector<G4String> theBoolValueNames;
  std::vector<G4String> theIntValueNames;
  std::vector<G4String> theDoubleValueNames;
  std::vector<G4String> theStringValueNames;
  std::vector<G4String> theThreeVectorValueNames;

  G4String theTrackInfoName;

  G4bool bSomethingToInherit;

  G4SteppingManager* theSteppingManager;
};

#endif
