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
#ifndef GmG4PSDoseDeposit_SkipEqualMateSimple_hh
#define GmG4PSDoseDeposit_SkipEqualMateSimple_hh 1

#include <vector>
#include <map>
#include "G4THitsMap.hh"
class GmVFilter;
//#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class GmVPSPrinter;
class GmVClassifier;
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   Primitive scorer class for scoring dose deposit in the geometry volume, with corrections of energy loss and msc for the case of regular navigation skipping of voxel frontiers
//
// Created: 2008-11-10 Pedro Arce
// 
///////////////////////////////////////////////////////////////////////////////
class   G4EnergyLossForExtrapolator;
class GmGeometryUtils;

class GmG4PSDoseDeposit_SkipEqualMateSimple : public GmVPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmG4PSDoseDeposit_SkipEqualMateSimple(G4String name);
  virtual ~GmG4PSDoseDeposit_SkipEqualMateSimple(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  G4bool FillScorer(G4Step* aStep, G4double val, G4double wei);

  G4double GetGeom2TrueStepLength( G4double kinEnergy ) ;

  public: 
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void DrawAll();
      virtual void PrintAll();
  virtual G4int GetIndex(G4Step*);

  virtual G4double GetUnit() const {
    return 1.E12; }

  virtual G4String GetUnitName() const {
    return "Gy"; }

private:
  G4EnergyLossForExtrapolator* theElossExt;
  
  GmGeometryUtils* theGeomUtils;

};

#endif
