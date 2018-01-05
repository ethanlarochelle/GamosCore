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
#ifndef GmVClassifier_hh
#define GmVClassifier_hh 1

#include <vector>
#include <set>
#include "globals.hh"
#include "G4THitsMap.hh"
class G4Step;
class G4Track;
// class description:
//
// This is the base class of the GmVClassifier.


class GmVClassifier 
{
public: // with description
  GmVClassifier(G4String );
  virtual ~GmVClassifier(){ 
  }
  
  virtual G4int GetIndexFromStep(const G4Step* aStep ) = 0;

  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual G4int GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);

  virtual G4String GetIndexName(G4int);

  virtual void SetParameters( std::vector<G4String>& ){};

  virtual std::set<G4int> GetIndexSet() const{
    std::set<G4int> si;
    return si; 
  }

  virtual void SetIndices( std::vector<G4String> wl ) = 0;

  void CheckNumberOfIndices( std::vector<G4String> wl );

  G4String GetName() const {
    return theName; 
  }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }

  G4bool IsCompound() const {
    return bIsCompound; }

  G4bool IsOnSecondary() {
    return bOnSecondary; }

protected:
  G4String theName;
  G4String theClass;

  G4int theMaxIndex;

  G4bool bIsCompound;

  G4bool bOnSecondary;
};

#endif
