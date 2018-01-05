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
#ifndef GmSSData_hh
#define GmSSData_hh

#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

#include <vector>
#include <iostream>

class GmSSData 
{
public:
  GmSSData( G4int showerID, const G4Step* aStep, TSPUse pointToUse );
  ~GmSSData(){};
  void AddStep( const G4Step* aStep );

  G4double GetTotalEnergy() const;  

  G4ThreeVector GetFirstTrackPos() const;
  G4ThreeVector GetFirstTrackDir() const;

  G4int GetShowerID() const {
    return theShowerID; 
  }
  G4int GetFirstTrackID() const {
    return theFirstTrackID;
  }
  G4int GetClassifierIndex() const {
    return theClassifierIndex;
  }
  std::vector<GmTrajStep*> GetSteps() const {
    return theSteps;
  }

  void SetClassifierIndex( G4int idx ){
    theClassifierIndex = idx;
  }

  void Dump( FILE* fout );
  void Dump( std::ostream& fout );

private:
  void DumpStepPre( GmTrajStep* step, std::ostream& fout );
  void DumpStepPost( GmTrajStep* step, std::ostream& fout );
  void DumpStepPre( GmTrajStep* step, FILE* fout );
  void DumpStepPost( GmTrajStep* step, FILE* fout );

private:
  TSPUse theTSPUse;
  G4int theShowerID;
  G4int theFirstTrackID;
  G4int theClassifierIndex;
  std::vector<GmTrajStep*> theSteps;
};

#endif
