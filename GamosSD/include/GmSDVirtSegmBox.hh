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
#ifndef GmSDVirtSegmBox_HH
#define GmSDVirtSegmBox_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class G4Box;
class G4VSolid;

class GmSDVirtSegmBox: public GmVSD
{

public:
  GmSDVirtSegmBox(G4String type);
  ~GmSDVirtSegmBox(){};
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* );
  void GetIDAndLocalPosition( unsigned long long& detUnitID, G4ThreeVector& localPos, G4ThreeVector position, const G4NavigationHistory* touchable, G4VSolid* solid );
  void GetNextVoxel( unsigned long long& detUnitID, G4ThreeVector& localPos, G4double& stepLength, const G4ThreeVector& localDir );
  void CreateHitVS( G4Step* aStep, G4double energy, G4ThreeVector position, G4double time, unsigned long long id );
  void UpdateHitVS( GmHit* hit, G4double energy, G4ThreeVector& position, G4double& time, G4Track* aTrack );
  
  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual void CalculateAndSetPosition( GmHit*, G4Step* ){
  };
  virtual unsigned long long GetDetUnitID( G4Step* ){
    return 0;  // it is never called indeed
  }
  
private:
  void StoreNewHit( G4double energy, G4double time, G4ThreeVector positionPre, unsigned long long detUnitIDPre, G4Step* aStep);

private:
  G4int theNDivX;
  G4int theNDivY;
  G4int theNDivZ;
  G4int theNDivXY;
  G4Box* theG4Box;
  G4double theWidthX;
  G4double theWidthY;
  G4double theWidthZ;
  G4double theOffsetX;
  G4double theOffsetY;
  G4double theOffsetZ;
  G4bool bOffsetsAreSet;
  G4int theMaxNVoxels;
  G4double theTolerance;
  G4bool bOffsetOnce;
  G4bool bOffsetOnceIsDone;

  G4int theNAncestors;
  G4int theNShift;

};

#endif
