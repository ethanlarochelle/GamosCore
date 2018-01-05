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
#ifndef GmGenerDistPositionInG4VolumesGeneral_HH
#define GmGenerDistPositionInG4VolumesGeneral_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class G4TouchableHistory;
class GmParticleSource;
class GmGeometryUtils;

class GmGenerDistPositionInG4VolumesGeneral : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInG4VolumesGeneral();
  virtual ~GmGenerDistPositionInG4VolumesGeneral(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  void SetWorldDimensions();

private:
  G4double theWorldHalfX, theWorldHalfY, theWorldHalfZ;
  std::vector<G4String> theVolumes;
  G4TouchableHistory* theTouchable;

  GmGeometryUtils* theGeomUtils;
};

#endif
