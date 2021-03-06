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
#ifndef GmVScoringSurfaceSolid_h
#define GmVScoringSurfaceSolid_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmPSWithDirection.hh"
#include "G4THitsMap.hh"

#include "G4Sphere.hh"
#include <set>

class GmVScoringSurfaceSolid 
{
 
public: 
  GmVScoringSurfaceSolid();
  virtual ~GmVScoringSurfaceSolid(){};

  G4ThreeVector GetLocalPoint( G4ThreeVector pos, G4Step* aStep );
  virtual void SetSurfaces( std::vector<G4String> surfaces ) = 0;
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep ) = 0;
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint ) = 0;
  virtual void ComputeArea() = 0;
  G4double GetArea(){
    return theArea;
  }

protected:
  std::set<G4String> theSurfaces;
  G4double theSurfaceTolerance;
  G4double theAngularTolerance;
  G4double theArea;
  G4ThreeVector theNormal;
  G4int theNSurfaceMax;
};

#endif
