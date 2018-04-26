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
#ifndef GmGeneratorScanVis_HH
#define GmGeneratorScanVis_HH

class G4Event;

#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "TCanvas.h"
#include "TH2F.h"
#include "TColor.h"

enum planeFig {PF_XY, PF_XZ, PF_YZ};
enum CBVIT { CBVIT_LV, CBVIT_PV, CBVIT_T};

typedef struct ColPoint {
  double x, y, z;
  char tag[64];          // border name
} ColPoint;

class GmGeometryUtils;

class GmGeneratorScanVis: public G4VUserPrimaryGeneratorAction,
			  public GmUserRunAction, 
			  public GmUserEventAction, 
			  public GmUserSteppingAction 
{
public:
  GmGeneratorScanVis();
  ~GmGeneratorScanVis();
  void GeneratePrimaries(G4Event* evt);
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  //  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );

private:
  void GenerateParticle(G4ThreeVector pos, G4Event* evt );
  void SetDir( G4double angle );
  G4bool IncrementPosTransv(); 
  void SetColors();
  EColor GetColor( int icol );

  G4bool bInitialized;
  G4double theStepScan;
  G4double theStepFig;
  G4double thePosTransv;
  G4double theScanWorldMinX;
  G4double theScanWorldMaxX;
  G4double theScanWorldMinY;
  G4double theScanWorldMaxY;
  G4double theScanWorldMinZ;
  G4double theScanWorldMaxZ;
  planeFig thePlaneFig;
  G4String thePlaneFigName;
  std::vector<G4double> theAngles;
  G4ThreeVector theDir;
  G4double theMarkerSize;
  G4int theMarkerStyle;
  
  std::map<G4String, std::vector<G4ThreeVector> > thePoints;
  TCanvas* theCanvas;
  TH2F* theHisto;
  G4int theWorldNDigits;
  G4int theNameNDigits;

  FILE * theFileOut;
  std::map<G4String,EColor> theColorMap;
  G4bool bColorByName;
  G4int theCanvasNPixels;
  // vertical and horizontal limits
  G4double theDrawMinH;
  G4double theDrawMaxH;
  G4double theDrawMinV;
  G4double theDrawMaxV;
  
  GmGeometryUtils* theGeomUtils;

  G4bool bDrawBorderNames;

  std::map<G4int,EColor> theGoodColors;
  std::map<G4String,G4int> theColorVolumeIdx;
  G4String theColorByVolumeIndexFN;
  CBVIT theColorByVolumeIndexType;
  G4bool bColorByVolumeIndex;
};
#endif
