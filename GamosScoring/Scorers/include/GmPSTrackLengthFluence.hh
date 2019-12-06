////////////////////////////////////////////////////////////////////////
// Optical Photon Tracklength Fluence Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4PSTrackLengthFluence.hh
// Description: Scores the tracklength fluence.
// Created: 2013-02-22
// Author: Adam Glaser
// Updated: 2019-12-06 by Ethan LaRochelle, port to 6.1
//
// This subroutine will perform record the fluence in voxelised geometry.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef GmPSTrackLengthFluence_h
#define GmPSTrackLengthFluence_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class GmPSTrackLengthFluence : public GmVPrimitiveScorer
{

 public: // with description
  GmPSTrackLengthFluence(G4String name);
  virtual ~GmPSTrackLengthFluence();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void DrawAll();
  virtual void PrintAll();
};


#endif

