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
#ifndef GmScoringSurfaceTubs_h
#define GmScoringSurfaceTubs_h 1

#include "GmVScoringSurfaceSolid.hh"

#include "G4Tubs.hh"
#include <set>

class GmScoringSurfaceTubs : public GmVScoringSurfaceSolid
{
 
public: 
  GmScoringSurfaceTubs(G4VSolid* tubs);
  virtual ~GmScoringSurfaceTubs(){};

  virtual void SetSurfaces( std::vector<G4String> surfaces );
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep );
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint );
  virtual void ComputeArea();

private:
  G4bool CheckSurfaceRadius(G4ThreeVector localPos, G4double radius );
  G4bool CheckSurfaceCap(G4double localPos, G4double dim );
  G4bool CheckSurfacePhi(G4double localPhi, G4double phi );

private:
  G4Tubs* theTubs;
  G4ThreeVector theNormal;
};

#endif
