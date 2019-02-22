#ifndef GmVGenerDist_HH
#define GmVGenerDist_HH

#include "globals.hh"
#include <vector>
class GmParticleSource;

enum EFFCalcType { EFFCT_Fixed, EFFCT_Histogram, EFFCT_Interpolate, EFFCT_InterpolateLog };

class GmVGenerDist
{
public:
  GmVGenerDist(){};
  virtual ~GmVGenerDist(){};

  virtual void SetParams( const std::vector<G4String>& wl ){
    theOrigParams = wl; }; 

  virtual void ResetParams(); // if not implemented it will call method above

  void CopyParams( std::vector<G4String> params ) {
    theOrigParams = params; }

  G4String GetName() const { 
    return theName; }

  GmParticleSource* GetParticleSource() const {
    return theParticleSource; }
  void SetParticleSource( GmParticleSource* src ) {
    theParticleSource = src; }


protected:
  G4String theName;

  std::vector<G4String> theOrigParams;

  GmParticleSource* theParticleSource;
};

#endif
