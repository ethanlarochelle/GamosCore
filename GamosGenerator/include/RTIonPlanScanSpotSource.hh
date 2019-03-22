#ifndef RTIonPlanScanSpotSource_H
#define RTIonPlanScanSpotSource_H
//
#include "RTVPlanSource.hh"

class RTIonPlanScanSpotSource : public RTVPlanSource
{
public:
  RTIonPlanScanSpotSource( const G4String& name, const G4String& partName );
  ~RTIonPlanScanSpotSource(){}
  virtual void ReadPlanData();

protected:
  virtual G4double GetSpotSize( ScanSpotData spData );
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const ScanSpotData& spdata );
  virtual void PlotScanSpots();
};

#endif // RTIonPlanScanSpotSource_H