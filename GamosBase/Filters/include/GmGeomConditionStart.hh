#ifndef GmGeomConditionStart_h
#define GmGeomConditionStart_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of entering particle (if in GeomBoundary)
// 
///////////////////////////////////////////////////////////////////////////////
class G4TouchableHistory;
class G4Navigator;
#include "GmVGeomCondition.hh"

class GmGeomConditionStart : public GmVGeomCondition
{
public: // with description
  GmGeomConditionStart();
  
  virtual ~GmGeomConditionStart(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;  
  G4Navigator* theNavigator;
};
#endif
