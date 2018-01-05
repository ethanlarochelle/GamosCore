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
#ifndef GmBOptrEWBS_hh
#define GmBOptrEWBS_hh 1

#include "GmVBiasingOperator.hh"
class GmBOptnEWBS;

class GmBOptrEWBS : public GmVBiasingOperator {
public:
  GmBOptrEWBS( G4String name );
  virtual ~GmBOptrEWBS() {}

  virtual void AddParticleProcess( G4String partName, G4String procName );

    // -- Call at run start:
  virtual void      StartRun();
  // -- Call at each track starting:
  virtual void StartTracking( const G4Track* track );
private:
  // -----------------------------
  // -- Mandatory from base class:
  // -----------------------------
  // -- Unused:
  virtual G4VBiasingOperation*
  ProposeNonPhysicsBiasingOperation(const G4Track* /* track */,
                                    const G4BiasingProcessInterface* /* callingProcess */)
  { return 0; }
  virtual G4VBiasingOperation* 
  ProposeOccurenceBiasingOperation (const G4Track* /* track */,
                                    const G4BiasingProcessInterface* /* callingProcess */)
  { return 0; }
  // -- Used:
  virtual G4VBiasingOperation*
  ProposeFinalStateBiasingOperation(const G4Track* track,
                                    const G4BiasingProcessInterface* callingProcess);
  
private:
  // -- Avoid compiler complaining for (wrong) method shadowing,
  // -- this is because other virtual method with same name exists.
  using GmVBiasingOperator::OperationApplied;

protected:
  GmBOptnEWBS* fSplittingOperation;
  G4int                          fNSplit;
  G4bool                         fBiasPrimaryOnly;
  G4bool                            fBiasOnlyOnce;
  G4int                             nInteractions;

};

#endif
