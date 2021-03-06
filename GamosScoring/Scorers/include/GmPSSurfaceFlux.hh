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
#ifndef GmPSSurfaceFlux_h
#define GmPSSurfaceFlux_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmPSWithDirection.hh"
#include "G4THitsMap.hh"

#include "G4Sphere.hh"
#include "G4PSDirectionFlag.hh"

#include <set>
#include <vector>

class GmVScoringSurfaceSolid;

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//   The flux is given in the unit of area.
//
///////////////////////////////////////////////////////////////////////////////

class GmPSSurfaceFlux : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSSurfaceFlux(G4String name);
  virtual ~GmPSSurfaceFlux(){};
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void DrawAll();
      virtual void PrintAll();
  virtual G4int GetIndex(G4Step*);
  
  virtual G4double GetUnit() const {
    return 100.; }
  
  virtual G4String GetUnitName() const {
    return "[cm^-2]"; }
  
private:
  G4bool ProcessHits(G4StepPoint* aStepPoint, G4Step* aStep, G4int aDirFlag );

private:
  std::vector<G4String> theSurfaces;
  std::map<G4VSolid*, GmVScoringSurfaceSolid*> theScoringSolids;
  GmVScoringSurfaceSolid* theScoringSolid;
  G4PSFluxFlag theDirection;
  G4bool bArea;
  G4bool bAngleFactor;
};

#endif
