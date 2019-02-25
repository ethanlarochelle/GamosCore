#ifndef RTIonPlanControlPoint_H
#define RTIonPlanControlPoint_H
#include "globals.hh"
#include <map>

class RTIonPlanBeam;
#include "RTVPlanControlPoint.hh"

class RTIonPlanControlPoint : public RTVPlanControlPoint
{
public:
  RTIonPlanControlPoint( const G4String& fileName, RTIonPlanBeam* beam );
  ~RTIonPlanControlPoint(){}

  std::vector<G4double> GetPosXs() const {
    return thePosXs;
  }
  std::vector<G4double> GetPosYs() const {
    return thePosYs;
  }

  virtual ScanSpotData NextScanSpot( G4int& incp );

  G4double GetScanningSpotSize() const {
    return theScanningSpotSize;
  }

  /*  G4ThreeVector GetsocenterPosition() const {
    return theIsocenterPosition;
    }*/

  void PlotScanSpots( G4double minmaxX, G4double minmaxY, G4double maxMeterset);
  
private:

  std::vector<G4double> thePosXs;
  std::vector<G4double> thePosYs;
  //  G4ThreeVector theIsocenterPosition;
  G4double iCurrentPainting;
  RTIonPlanBeam* theBeam;

};

#endif // RTIonPlanControlPoint_H
