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
#ifndef GmSDVirtSegmentedSphereRThetaPhiZ_HH
#define GmSDVirtSegmentedSphereRThetaPhiZ_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;

class GmSDVirtSegmentedSphereRThetaPhiZ: public GmVSD{

public:
  GmSDVirtSegmentedSphereRThetaPhiZ(G4String type);
  ~GmSDVirtSegmentedSphereRThetaPhiZ();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

private:
  void DefineSegmentation();
  
private:
  G4LogicalVolume* theLogVol;
  float theRadius;
  float theZLength;
  float theThetaAngle;
  std::vector<float> thePhiAngles;
  G4int MAXNDIV;
};

#endif
