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
#ifndef GmGenerDistPositionPETImage_HH
#define GmGenerDistPositionPETImage_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;
class G4PhantomParameterisation;
#include "G4AffineTransform.hh"
#include <set>


class GmGenerDistPositionPETImage : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionPETImage();
  virtual ~GmGenerDistPositionPETImage(){};

  virtual void SetParams( const std::vector<G4String>& params );

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );


private:
  void Initialise();
  
  G4String theFileName;
  G4double theMinX, theMinY, theMinZ;
  G4double theVoxelDimX, theVoxelDimY, theVoxelDimZ;
  unsigned int theNVoxelX, theNVoxelY, theNVoxelZ;
  std::multiset<G4double> theProb;
  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;
};

#endif
