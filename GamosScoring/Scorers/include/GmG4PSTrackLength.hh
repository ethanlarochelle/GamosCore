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
#ifndef GmG4PSTrackLength_h
#define GmG4PSTrackLength_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"


////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring track length.
// 
///////////////////////////////////////////////////////////////////////////////

class GmG4PSTrackLength : public GmVPrimitiveScorer
{
 
 public: // with description
  GmG4PSTrackLength(G4String name);
  virtual ~GmG4PSTrackLength();
  
  inline void MultiplyKineticEnergy(G4bool flg=true) { multiplyKinE = flg; }
  // Multiply Kinetic Energy
  
  inline void DivideByVelocity(G4bool flg=true) { divideByVelocity = flg; }
  // Divide by velocity
  
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void DrawAll();
      virtual void PrintAll();
  virtual G4int GetIndex(G4Step*);
  
  virtual G4double GetUnit() const;
  virtual G4String GetUnitName() const;
  
  void SetParameters( const std::vector<G4String>& params);
  
private:
  G4bool multiplyKinE;
  G4bool divideByVelocity;
};


#endif

