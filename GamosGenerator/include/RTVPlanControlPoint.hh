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
#ifndef RTVPlanControlPoint_H
#define RTVPlanControlPoint_H
#include "G4ThreeVector.hh"
#include <map>
#include <iostream>

class RTVPlanBeam;
class RTVPlanControlPoint;

#include "RTPlanParamReader.hh"
struct ScanSpotData {
  G4double PositionX;
  G4double PositionY;
  G4double PositionZ;
  G4double Energy;
  G4double RotAngleX;
  G4double RotAngleY;
  G4double RotAngleZ;
  G4double SpotSize;
  G4double SourceAxisDistance;
  RTVPlanControlPoint* ControlPoint;
  RTVPlanBeam* Beam;
  G4ThreeVector IsocenterPosition;
};

class RTVPlanControlPoint : public RTPlanParamReader
{
public:
  RTVPlanControlPoint(){}
  ~RTVPlanControlPoint(){}

  G4bool operator==(const RTVPlanControlPoint& right) const;
    
  std::vector<G4double> GetMetersets() const {
    return theMetersets;
  }
  void SetMetersets( std::vector<G4double> metersets ) {
    theMetersets = metersets;
  }
  virtual ScanSpotData NextScanSpot( G4int& incp ) = 0;

  virtual void PlotScanSpots( G4double , G4double , G4double ){};
  G4int GetNumberOfPaintings() {
    return theNumberOfPaintings;
  }
  G4int GetIndex() const {
    return theIndex;
  }
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }
  
protected:

  std::vector<G4double> theMetersets;
  //  G4ThreeVector theIsocenterPosition;
  G4double theNominalBeamEnergy;
  G4double theGantryAngle;
  G4double theGantryPitchAngle;
  G4double theScanningSpotSize;
  G4double theNumberOfPaintings;
  G4double theSourceAxisDistance;
  G4int theIndex;
  RTVPlanBeam* theBeam;
  
  G4int iCurrentScanSpot;
  G4String theFileName;
};

std::ostream& operator<<(std::ostream& out, const RTVPlanControlPoint& cp);


#endif // RTVPlanControlPoint_H
