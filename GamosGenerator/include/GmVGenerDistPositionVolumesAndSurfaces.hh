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
#ifndef GmVGenerDistPositionVolumesAndSurfaces_HH
#define GmVGenerDistPositionVolumesAndSurfaces_HH
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class G4VSolid;
class G4TouchableHistory;

struct GVSTouchableInfo{
  G4String name;
  const G4VSolid* solid;
  G4ThreeVector pos;
  G4RotationMatrix rotmat;
};

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "GmVPositionObjectPos.hh"
#include "GmVPositionOrigin.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmGeometryUtils;
class GmVGenerDistPositionVolumesAndSurfaces : public GmVGenerDistPosition
{ 
public:
  GmVGenerDistPositionVolumesAndSurfaces();
  virtual ~GmVGenerDistPositionVolumesAndSurfaces();

  virtual void SetParams( const std::vector<G4String>& params );

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

protected:
  std::vector<GVSTouchableInfo*> theTouchableInfos;
  std::vector<G4double> theTotalObjectDimensions;

  GmVPositionObjectPos* theObject;
  GmVPositionOrigin* theOrigin;

  G4bool bOnlyVolume;
  GmGeometryUtils* theGeomUtils;
  G4TouchableHistory* theTouchable;

};


#endif
