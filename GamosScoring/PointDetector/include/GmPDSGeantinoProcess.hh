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
#ifndef GmPDSGeantinoProcess_h
#define GmPDSGeantinoProcess_h

#include "GmPDSVProcess.hh"

class G4SteppingManager;
class G4HadronicProcess;
class G4ParticleDefinition;
class GmPDSInvertGeantinoStackUA;

class GmPDSGeantinoProcess : public GmPDSVProcess
{
public:
  
  GmPDSGeantinoProcess(const G4String&, G4SteppingManager*, G4ParticleDefinition* origParticle );
  
  virtual ~GmPDSGeantinoProcess();
  
  virtual G4VParticleChange* PostStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			     );

  void ChangeGeantinoWeight(const G4Step* aStep);


protected:// with description
  virtual G4double GetMeanFreePath(const G4Track& aTrack,
				   G4double   previousStepSize,
				   G4ForceCondition* condition
				   );
  //  Calculates from the macroscopic cross section a mean
  //  free path, the value is returned in units of distance.

private:
  void SplitOriginalParticle( const G4Track& aTrack, const G4Step& aStep, G4ParticleChange& aParticleChange );

private:
  /*  GmAnalysisMgr* theAnaMgr;
  std::map<G4String,G4int> theVolumes;
  */

  G4double theMinimumGeantinoWeight;
  G4double theInvMinimumGeantinoWeightRR;

  G4bool bUseOriginalParticleSplitting;
  G4bool bUseMinimumGeantinoWeight;
  GmPDSInvertGeantinoStackUA* theInverseStackingUA;

};
#endif
