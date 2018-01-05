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
//---------------------------------------------------------------
//
// GmBOptnEWPS
//
// Class Description:
//    A G4VBiasingOperation to change a process cross-section.
//    
//
//---------------------------------------------------------------
//   Initial version                         Nov. 2014 M. Verderi


#ifndef GmBOptnEWPS_hh
#define GmBOptnEWPS_hh 1

#include "GmVEWBiasingOperation.hh"
#include "G4ParticleChange.hh"
#include "G4UImessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

class G4BiasingProcessInterface;

class GmBOptnEWPS : public GmVEWBiasingOperation, public G4UImessenger {
public:
  // -- Constructor :
  GmBOptnEWPS(G4String name);
  // -- destructor:
  virtual ~GmBOptnEWPS();
  
public:
  // ----------------------------------------------
  // -- Methods from G4VBiasingOperation interface:
  // ----------------------------------------------
  // -- Unused:
  virtual const G4VBiasingInteractionLaw* 
  ProvideOccurenceBiasingInteractionLaw( const G4BiasingProcessInterface*,
                                         G4ForceCondition& )
  { return 0; }
  
  // --Used:
  virtual G4VParticleChange*   ApplyFinalStateBiasing( const G4BiasingProcessInterface*,
                                                       const G4Track*,
                                                       const G4Step*,
                                                       G4bool&                          );
  
  // -- Unsued:
  virtual G4double           DistanceToApplyOperation( const G4Track*,
                                                       G4double,
                                                       G4ForceCondition*)
  {return DBL_MAX;}
  virtual G4VParticleChange* GenerateBiasingFinalState( const G4Track*,
                                                        const G4Step*   ) 
  {return 0;}

  void SetNewValue(G4UIcommand * command,G4String newValues);
  
  void SetNSplit(G4int splittingFactor);

private:
  GmUIcmdWithAString* addPartProcCmd;
  std::map<G4String,GmVEWBiasingOperation*> theOperationNames;
  std::map<const G4BiasingProcessInterface*,GmVEWBiasingOperation*> theOperations;
};

#endif
