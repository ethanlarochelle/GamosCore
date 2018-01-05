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
// class description
//
// A "process" to be registered to the process manager of each particle,
// in the UserPhysicsList, in order to take into account the user limits
// defined in the G4UserLimits object attached to a logical volume.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GmUserSpecialCuts_h
#define GmUserSpecialCuts_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VProcess.hh"

class G4LossTableManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmUserSpecialCuts : public G4VProcess 
{
  public:  // with description     

     GmUserSpecialCuts(const G4String& processName ="GmUserSpecialCut" );

     virtual ~GmUserSpecialCuts();

     virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );

     virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& ,
                             const G4Step& 
                            );
                            
  public:  // without description 
                                 
     //  no operation in  AtRestGPIL
     virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& ,
                             G4ForceCondition* 
                            ){ return -1.0; };
                            
     //  no operation in  AtRestDoIt      
     virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& ,
                             const G4Step&
                            ){return 0;};

     //  no operation in  AlongStepGPIL
     virtual G4double AlongStepGetPhysicalInteractionLength(
                             const G4Track&,
                             G4double  ,
                             G4double  ,
                             G4double& ,
                             G4GPILSelection*
                            ){ return -1.0; };

     //  no operation in  AlongStepDoIt
     virtual G4VParticleChange* AlongStepDoIt(
                             const G4Track& ,
                             const G4Step& 
                            ) {return 0;};

  private:
  
  // hide assignment operator as private 
      GmUserSpecialCuts(GmUserSpecialCuts&);
      GmUserSpecialCuts& operator=(const GmUserSpecialCuts& right);

  G4LossTableManager* theLossTableManager;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
