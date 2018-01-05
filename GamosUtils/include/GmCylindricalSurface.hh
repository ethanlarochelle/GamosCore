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
#ifndef GmCylindricalSurface_h
#define GmCylindricalSurface_h

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Plane3D.hh"
class GmLine;

class GmCylindricalSurface 
{
public:
  GmCylindricalSurface( const G4float& radius, const G4ThreeVector& trans=G4ThreeVector(), const G4RotationMatrix& rotm=G4RotationMatrix());
  GmCylindricalSurface( const G4float& radius, const G4Transform3D& trans3D);

  ~GmCylindricalSurface(){};

public:

  G4ThreeVector Intersect( const GmLine& line ) const;
  G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4ThreeVector IntersectLocal( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4double GetDistanceFromPoint( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4double GetDistanceFromPoint( const G4ThreeVector& point ) const;
  G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const;
  void Dump( G4String msg ) const;

 private:
  G4float fradius;
  G4Transform3D ftransform3D;

};

#endif
