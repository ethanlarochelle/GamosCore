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
#ifndef GmVProdCutOutsideVoxelFilter_h
#define GmVProdCutOutsideVoxelFilter_h 1

class G4Track;
class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class G4Material;
#include <map>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class is the base for prod cuts filter
// 
///////////////////////////////////////////////////////////////////////////////

#include <set>

class GmVProdCutOutsideVoxelFilter : public GmVFilter 
{

public: // with description
  GmVProdCutOutsideVoxelFilter(G4String name);
  virtual ~GmVProdCutOutsideVoxelFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  
  void SetParameters( std::vector<G4String>& params);
  
  virtual G4bool CheckProcess(const G4VProcess*) = 0;

  virtual G4double GetTrackEnergy( const G4Track* aTrack );
  virtual G4bool CheckStepNumber( G4int nstep );

protected:
  std::set<G4int> theParticleIDs;
  std::map<G4int,G4int> theCreationVoxels;
  std::map<G4Material*, G4double> theGammaCuts;
  std::map<G4Material*, G4double> theElectronCuts;
  std::map<G4Material*, G4double> thePositronCuts;

  G4int theOldEventID;
  G4int theNewEventID;
};

#endif
