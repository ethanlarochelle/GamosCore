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
#ifndef GmPDSVProcess_h
#define GmPDSVProcess_h

#include "G4VDiscreteProcess.hh"
class GmPDSProcessHelper;

class G4SteppingManager;
class GmPDSInteractionAngleManager;
class GmAnalysisMgr;
class GmVFilter;
class GmVClassifier;

#include <map>
#include <set>
class G4ParticleDefinition;
class G4VMscModel;

class GmPDSVProcess : public G4VDiscreteProcess
{
public:
  
  GmPDSVProcess(const G4String&, G4SteppingManager* );
  
  virtual ~GmPDSVProcess();

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

  virtual void AddHelper( GmPDSProcessHelper* helper );

  void SetFilters( std::vector<GmVFilter*> filts ) {
    theFilters = filts;
  };

protected:
  void ApplyRussianRoulette( G4Track* aTracknc, 
			     double weight 
			     );

  void CreateGeantino( const G4Track& aTrack, 
		       const G4double primPreEner, 
		       const G4StepPoint* stepPoint2, 
		       const G4double secoEner, 
		       const G4int detID, 
		       const G4String procDefStepName,
		       G4int classifierIndex );

  std::vector<G4Track*> GetSecondaryAndPrimaryTracks( const G4Track* aTrack);
  
  void FillHistoEachStep(const G4Step& aStep);

  G4String GetProcessDefiningStepName(const G4Step& aStep);

  G4ParticleDefinition* GetOriginalParticle() const {
    return theOriginalParticle;
  }
  void SetOriginalParticle( G4ParticleDefinition* op ){
    theOriginalParticle = op;
  }

protected:
  G4ParticleDefinition* theOriginalParticle;

  GmPDSInteractionAngleManager* theInteractionAngleManager;

  std::map<G4String,G4int> theVolumes;

  G4double theMinimumWeightForSplitting;
  G4double theMaximumWeightForSplitting;
  G4double theMaximumSplitting;
  G4double theSplittingFactor;

  G4bool bUseMaximumDistance;
  G4double theMaximumDistance;
  G4double theInvMaximumDistanceRR;

  G4double theExclusionRadius;

  G4SteppingManager* fpSteppingManager;

  std::map<G4int, GmPDSProcessHelper*> theHelpers;
  GmPDSProcessHelper* theCurrentHelper;

  std::vector<GmVFilter*> theFilters;
  G4bool bFiltersOnTrack;

  GmVClassifier* theClassifier;
  G4bool bClassifierOnTrack;

  std::map<G4ParticleDefinition*,G4VMscModel*>theMscModels;

};
#endif
