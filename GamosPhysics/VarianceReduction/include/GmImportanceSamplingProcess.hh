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
#ifndef GmImportanceSamplingProcess_h
#define GmImportanceSamplingProcess_h

#include "G4VDiscreteProcess.hh"

class GmAnalysisMgr;
class GmVDistribution;
class GmVFilter;
class GmInheritTrackInfoUA;

class GmImportanceSamplingProcess : public G4VDiscreteProcess
{
public:
  
  GmImportanceSamplingProcess(const G4String&, GmVDistribution*  );
  
  virtual ~GmImportanceSamplingProcess();

  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			     );

  virtual G4double GetMeanFreePath(const G4Track& aTrack,
				   G4double   previousStepSize,
				   G4ForceCondition* condition
				   );
  //  Calculates from the macroscopic cross section a mean
  //  free path, the value is returned in units of distance.
 
  virtual G4VParticleChange* PostStepDoIt( const G4Track& ,
					   const G4Step& 
					   );

private:
  void FillHistoEachStep(const G4Step& aStep);

private:
  GmVDistribution* theDistribution;

  G4int theMaxSplitTimes;
  G4bool bSplitAtSeveralSteps;
  G4bool bApplyRussianRoulette;
  G4bool bApplySplitting;

  std::vector<GmVFilter*> theFilters;

  static GmInheritTrackInfoUA* theInheritTrackInfoUA;

};
#endif
