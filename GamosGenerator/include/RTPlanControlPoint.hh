#ifndef RTPlanControlPoint_H
#define RTPlanControlPoint_H

#include "RTVPlanControlPoint.hh"
class RTPlanBeam;

class RTPlanControlPoint : public RTVPlanControlPoint
{
public:
  RTPlanControlPoint( const G4String& fileName, RTPlanBeam* beam );
  ~RTPlanControlPoint(){}

  virtual ScanSpotData NextScanSpot( G4int& incp );

  /*  G4ThreeVector GetsocenterPosition() const {
    return theIsocenterPosition;
    }*/
  
private:

  RTPlanBeam* theBeam;
  
};

#endif // RTPlanControlPoint_H
