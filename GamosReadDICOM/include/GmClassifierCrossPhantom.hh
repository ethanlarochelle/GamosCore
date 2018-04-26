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
#ifndef GmClassifierCrossPhantom_hh
#define GmClassifierCrossPhantom_hh 1

#include <vector>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class G4TouchableHistory;

class GmClassifierCrossPhantom  : public GmVClassifier
{
public: // with description
  GmClassifierCrossPhantom(G4String);
  virtual ~GmClassifierCrossPhantom(){ };
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int GetIndex(G4TouchableHistory* th);

  std::map<G4int,G4String> theIndexNames;
  std::map<G4String,G4int> theIndexMap;

  G4int theNVoxelsX;
  G4int theNVoxelsY;
  G4int theNVoxelsZ;
  G4int theNVoxelsXY;
  G4int theYPhantomNVoxels;
  
};

#endif
