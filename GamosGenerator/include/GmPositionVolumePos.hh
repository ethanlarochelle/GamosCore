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
#ifndef GmPositionVolumePos_HH
#define GmPositionVolumePos_HH

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UImessenger.hh"
#include <map>
class G4VSolid;
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionObjectPos.hh"
#include <vector>
#include <algorithm>
#include <complex>

#define doublePRECISION 1e-12

// Mcc. May 2010. Structs for Polycone
struct TRIdata{
  G4double Atot, r1, r2, r3, z1, z2, z3;
};

struct Vpoint{
  G4double r, z;
};

struct CompVpoints_Z{
  bool operator()(const Vpoint& v1, const Vpoint& v2)
  {return v1.z < v2.z;}
};
// Mcc

class GmPositionVolumePos : public GmVPositionObjectPos
{
public:
  GmPositionVolumePos();
  ~GmPositionVolumePos();

  virtual G4double GetObjectDimension(const G4VSolid* solid) const;
  virtual G4ThreeVector GeneratePosition( const GVSTouchableInfo* tinfo );
  virtual void BuildSurfaceAreas( const G4VSolid* ){}

// Mcc.  May 2010. Polycone
  virtual G4double areaTRI(G4double r1, G4double z1, G4double r2, G4double z2, G4double r3, G4double z3);
  virtual G4double areaPOND(G4double Arr, G4double Azz, G4double Brr, G4double Bzz, G4double Crr, G4double Czz);
  virtual std::vector<TRIdata> trianguliza(std::vector<Vpoint> vect_p, std::vector<TRIdata> list_tr);
  virtual G4double find_R(G4double a, G4double b, G4double c, G4double d, G4double Rmin, G4double Rmax);

private:
  G4ThreeVector GeneratePosInSolid( const G4VSolid* solid );

  std::map< G4String, std::vector<TRIdata> > MapTRI;

};

#endif
