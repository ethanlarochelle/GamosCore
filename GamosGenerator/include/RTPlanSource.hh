#ifndef RTPlanSource_H
#define RTPlanSource_H
//
#include "RTVPlanSource.hh"

class RTPlanSource : public RTVPlanSource
{
public:
  RTPlanSource( const G4String& name, const G4String& partName );
  ~RTPlanSource(){}
  virtual void ReadPlanData();

private:
  //  virtual G4double GetSpotSize( ScanSpotData spData );
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const ScanSpotData& spdata );
  virtual void PlotScanSpots();
};

#endif // RTPlanSource_H
