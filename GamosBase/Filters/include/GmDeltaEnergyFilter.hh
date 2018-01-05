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
#ifndef GmDeltaEnergyFilter_h
#define GmDeltaEnergyFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if energy is in a defined user range. It uses PreStepPoint energy in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmDeltaEnergyFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmDeltaEnergyFilter(G4String name);
  // Constructor
  
  virtual ~GmDeltaEnergyFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double fLowEnergy;
  G4double fHighEnergy;

  G4double theTrackElost;
  G4int theLastTrackID;
  G4int theLastEventID;

};

#endif
