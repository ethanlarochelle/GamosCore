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
#ifndef GmVPSModelHelper_hh
#define GmVPSModelHelper_hh 1

#include "globals.hh"
#include "G4ios.hh"
//#include "G4ParticleChangeForGamma.hh"
#include "GmPSZPlaneDirChecker.hh"

class G4VBremAngularDistribution;
class G4BremsstrahlungCrossSectionHandler;
class G4VEmModel;
#include "G4Track.hh"

enum PSZone { PSZAll=1, PSZBeforeRR=1, PSZAfterRR=2, PSZAfterEleSplit=3 };

class GmVPSModelHelper
{
public: // with description
  //  constructor requires the process name and type
  GmVPSModelHelper();

public:
  //  destructor
  virtual ~GmVPSModelHelper(){};

public:
  //virtual G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step& stepData);
  virtual G4bool RussRoul();
  virtual G4double CheckGasNonGas(const G4Track& aTrack);
  virtual G4bool AcceptGamma(const G4ThreeVector vGammadir, const G4ThreeVector vGammapos);
  virtual PSZone GetZone(const G4double z_track);
 
  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track) = 0;

  G4bool IsApplicable(G4double posZ);

  void SetPlaneDirChecker( GmPSZPlaneDirChecker* pchk ){
    thePlaneChecker = pchk; }

private:
  //void SetAngularDistribution(const G4String& name);

protected:
  G4double zRussRoul, zEleSplit;
  G4double theGasMaxDensity;
  G4int bUseRRplane, bUseEplane;
  G4int theNSplit;
  G4int theNewNSplit;
  G4double theNSplitInv;
  G4double theWgt;
  G4double theNewWgt;
  G4int idZone;
  G4VEmModel* theAngularDistribution;
  //  G4ParticleChangeForGamma* fParticleChange;
  //  G4BremsstrahlungCrossSectionHandler* theCrossSectionHandler;
  GmPSZPlaneDirChecker* thePlaneChecker;
  G4double thePlaneZ;
  
};

#endif
