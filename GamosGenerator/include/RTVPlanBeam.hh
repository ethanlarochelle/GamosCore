#ifndef RTVPlanBeam_H
#define RTVPlanBeam_H
#include "RTVPlanControlPoint.hh"
#include "RTPlanParamReader.hh"

class RTVPlanBeam : public RTPlanParamReader
{
public: 
  RTVPlanBeam(){}
  ~RTVPlanBeam(){}
  std::vector<RTVPlanControlPoint*> GetControlPoints() const {
    return theControlPoints;
  }
  virtual ScanSpotData NextScanSpot( G4int& incp );
  G4int GetIndex() const {
    return theIndex;
  }

  G4int GetNoControlPoints() const {
    return theControlPoints.size();
  }
  RTVPlanControlPoint* GetControlPoint( G4int ii );
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }

protected:
  virtual void ReorderControlPoints() = 0;

  std::vector<RTVPlanControlPoint*> theControlPoints;

  G4int theIndex;
  G4int iCurrentControlPoint;
  G4String theFileName;
};

#endif // RTVPlanBeam_H
