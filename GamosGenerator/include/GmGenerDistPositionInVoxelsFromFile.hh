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
#ifndef GmGenerDistPositionInVoxelsFromFile_HH
#define GmGenerDistPositionInVoxelsFromFile_HH

#include "GmVGenerDistPosition.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include <map>

class GmParticleSource;
typedef std::map<G4double,G4int> mdi;

class GmGenerDistPositionInVoxelsFromFile : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInVoxelsFromFile(){};
  virtual ~GmGenerDistPositionInVoxelsFromFile(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:

  void ErrorTooShortFile();

  mdi theProbAccum;
  G4double theProbSum;
  G4int theNVoxelX;
  G4int theNVoxelY;
  G4int theNVoxelZ;
  G4int theNVoxelXY;
  G4double theOffsetX;
  G4double theOffsetY;
  G4double theOffsetZ;
  G4double theDimX;
  G4double theDimY;
  G4double theDimZ;

  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;

};

#endif
