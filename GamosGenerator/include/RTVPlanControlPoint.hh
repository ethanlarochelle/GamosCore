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
  RTVPlanControlPoint();
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
  G4int GetIndex() const {
    return theIndex;
  }
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }

  G4double GetNominalBeamEnergy() const {
    return theNominalBeamEnergy;
  }
  G4double GetGantryAngle() const {
    return theGantryAngle;
  }
  G4double GetGantryPitchAngle() const {
    return theGantryPitchAngle;
  }
  G4double GetScanningSpotSize() const {
    return theScanningSpotSize;
  }
  G4int GetNumberOfPaintings() const {
    return theNumberOfPaintings;
  }
  G4double GetSourceAxisDistance() const {
    return theSourceAxisDistance;
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
