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
// GmUserAction:
// Author: P.Arce  02/03/2002
// Base class for all G4 user actions
// Holds the name and group of user actions
// Keeps and empty method start/end run, start/end event, start/end track and stepping for calling all user actions with a pointer of this class type

#ifndef GmUserAction_h
#define GmUserAction_h 1

#include "globals.hh"
class G4Run;
class G4Event;
class G4Track;
class G4Step;
#include "G4ClassificationOfNewTrack.hh"
#include "globals.hh"
class GmVFilter;
class GmVClassifier;
#include <vector>

class GmUserAction
{
 protected:
  GmUserAction();
 public:
  // public:
  GmUserAction( const G4String& name );
  virtual ~GmUserAction();

 public:
  virtual void InitialiseHistoNames(){
  }

  const G4String GetName() const{ 
    return theName; 
  }
  bool IsEnabled() const{
    return isEnabled; 
  }

  void SetFilterOrClassifier(const G4String& objectName);

  //  virtual void DefineName( const G4String& Name ) = 0;

  void SetName( const G4String& Name ){
    theName = Name; 
  }
  void Disable() {
    G4cout << " disabling " << this << G4endl;
    isEnabled = FALSE; 
  }
  void Enable() {
    isEnabled = TRUE; 
  }

  G4bool ApplyFiltersToStepping() const {
    return bApplyFiltersToStepping; 
  }
  G4bool ApplyFiltersToPreTracking() const {
    return bApplyFiltersToPreTracking; 
  }
  G4bool ApplyFiltersToPostTracking() const {
    return bApplyFiltersToPostTracking; 
  }
  G4bool ApplyFiltersToStacking() const {
    return bApplyFiltersToStacking; 
  }
  void SetApplyFiltersToStepping(G4bool af ) {
    bApplyFiltersToStepping = af; 
  }
  G4bool CheckAllFiltersAtStepping() const {
    return bCheckAllFiltersAtStepping; 
  }
  G4bool CheckAllFiltersAtPreTracking() const {
    return bCheckAllFiltersAtPreTracking; 
  }
  G4bool CheckAllFiltersAtPostTracking() const {
    return bCheckAllFiltersAtPostTracking; 
  }
  G4bool CheckAllFiltersAtStacking() const {
    return bCheckAllFiltersAtStacking; 
  }

public:
  std::vector<GmVFilter*> theFilters;
  GmVClassifier* theClassifier;
protected:
  G4String theName;
  G4bool isEnabled;

  G4bool bApplyFiltersToStepping;
  G4bool bApplyFiltersToPreTracking;
  G4bool bApplyFiltersToPostTracking;
  G4bool bApplyFiltersToStacking;

  G4bool bCheckAllFiltersAtStepping;
  G4bool bCheckAllFiltersAtPreTracking;
  G4bool bCheckAllFiltersAtPostTracking;
  G4bool bCheckAllFiltersAtStacking;

};
#endif
