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
#ifndef GmEWBSSplittingProcess_HH
#define GmEWBSSplittingProcess_HH 1

/////////////
// Includes
/////////////

#include "G4VDiscreteProcess.hh"

/////////////////////
// Class Definition
/////////////////////

class GmEWBSSplittingProcess : public G4VDiscreteProcess
{

public:

        ////////////////////////////////
        // Constructors and Destructor
        ////////////////////////////////

        GmEWBSSplittingProcess(const G4String& processName = "GmEWBSSplittingProcess",
                             G4ProcessType type = fUCN);
	virtual ~GmEWBSSplittingProcess();

private:

        GmEWBSSplittingProcess(const GmEWBSSplittingProcess &right);

        //////////////
        // Operators
        //////////////

        GmEWBSSplittingProcess& operator=(const GmEWBSSplittingProcess &right);

public:

        ////////////
        // Methods
        ///////////

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable' only for an UCN.

  /*        G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
  		     G4ForceCondition* condition);
  */
        G4double GetMeanFreePath(const G4Track& aTrack,
                                 G4double ,
                                 G4ForceCondition* condition);

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                                        const G4Step&  aStep);

private:
  G4double thePlaneMinZ;
  G4double thePlaneMaxZ;
  G4int theNSplit;
  G4double theMinimumWeight;
  
};

#endif

