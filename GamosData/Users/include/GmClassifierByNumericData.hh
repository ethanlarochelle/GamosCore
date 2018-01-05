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
#ifndef GmClassifierByNumericData_hh
#define GmClassifierByNumericData_hh 1

#include <vector>
#include "globals.hh"
class G4Step;
class G4Track;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GmVDataUser.hh"

class GmClassifierByNumericData  : public GmVClassifier,
				   public GmVDataUser
{
public: // with description
  GmClassifierByNumericData(G4String);
  virtual ~GmClassifierByNumericData();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);
  virtual G4int GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);
  G4int GetIndexFromValue(const G4double val );

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4double,G4int> theIndexMap;
  G4double theMin, theMax, theStep;

  G4bool bAllowOutOfLimits;
};

#endif
