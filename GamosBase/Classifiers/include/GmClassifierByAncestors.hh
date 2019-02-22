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
#ifndef GmClassifierByAncestors_hh
#define GmClassifierByAncestors_hh 1

#include <vector>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByAncestors  : public GmVClassifier
{
public: // with description
  GmClassifierByAncestors(G4String);
  virtual ~GmClassifierByAncestors(){ };
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual G4String GetIndexName(int64_t index);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int theNAncestors;
  G4int theNShift;
  int64_t theMaxIndex;

  std::map<int64_t,G4String> theIndexNames;
  std::map<G4String,int64_t> theIndexMap;
};

#endif
