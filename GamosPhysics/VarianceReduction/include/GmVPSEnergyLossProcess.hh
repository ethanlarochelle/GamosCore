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
#ifndef GmVPSEnergyLossProcess_hh
#define GmVPSEnergyLossProcess_hh 1
//
// Class Description
//
// This is the base class to implement energy loss particle splitting processes

#include "globals.hh"
#include "G4ios.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ParticleChange.hh"
class GmVPSModelHelper;

class GmVPSEnergyLossProcess : public G4VEnergyLossProcess
{
public: // with description
  //  constructor requires the process name and type
  GmVPSEnergyLossProcess( const G4String& aName = "GmParticleSplitting",
			  G4ProcessType aType = fUserDefined );

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition* p,
					   const G4ParticleDefinition* p2) = 0;

public:
  //  destructor
  virtual ~GmVPSEnergyLossProcess(){};
  void SetWrappedProcess( G4VEnergyLossProcess* wp ) {
    theWrappedProcess = wp; }

public:
  virtual G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step& stepData);

  GmVPSModelHelper* GetPSModel() const {
    return thePSModel; }

  void AddPSEmModel( GmVPSModelHelper* psmod ) {
    thePSModel = psmod; 
    //     G4cout << this << " AddPSEmModel " << thePSModel << " " << GetProcessName() << G4endl;
  }

protected:
  G4VEnergyLossProcess* theWrappedProcess;

  //  G4ParticleChange fParticleChange;
  GmVPSModelHelper* thePSModel;
};

#endif
