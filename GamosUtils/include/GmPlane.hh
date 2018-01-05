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
#ifndef GmPlane_hh
#define GmPlane_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Plane3D.hh"

class GmPlane : G4Plane3D
{
  public:  // with description

    GmPlane(G4double a=0, G4double b=0,
                              G4double c=0, G4double d=0);
      // Constructs plane by parameters: ax+by+cz+d = 0

    GmPlane(const G4Normal3D &n,
                              const G4Point3D &p);
      // Constructs plane by point and normal

    GmPlane(const G4Point3D &p1,
                              const G4Point3D &p2,
                              const G4Point3D &p3);
      // Constructs plane by three points

    ~GmPlane();

    virtual G4ThreeVector Intersect( const G4ThreeVector& point,
                                     const G4ThreeVector& direc ) const;
      // Intersects the surface with the line given by point and direction
  
    virtual G4double GetDistanceFromPoint( const G4ThreeVector& point,
                                           const G4ThreeVector& direc ) const;
      // Distance from point to surface in a given direction

    virtual G4double GetDistanceFromPoint( const G4ThreeVector& pt ) const;
      // Closest distance from point to surface

    virtual void Dump( const G4String& msg ) const;
      // Dump plane surface parameter

};

#endif
