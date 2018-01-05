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
/// \file GmBOptrBremsSplitting.cc
/// \brief Implementation of the GmBOptrBremsSplitting class

#include "GmBOptrBremsSplitting.hh"
#include "GmBOptnBremsSplitting.hh"

#include "G4BiasingProcessInterface.hh"
#include "G4GenericMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptrBremsSplitting::GmBOptrBremsSplitting( G4String name )
: GmVBiasingOperator(name)
{
  fBremsSplittingOperation = new GmBOptnBremsSplitting("BremsSplittingOperation");

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  fNSplit = theParamMgr->GetIntegerValue(GetName()+":NSplit",2);
  fBiasPrimaryOnly = theParamMgr->GetBooleanValue(GetName()+":BiasPrimaryOnly",true);
  fBiasOnlyOnce = theParamMgr->GetBooleanValue(GetName()+":BiasOnlyOnce",true);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmBOptrBremsSplitting::StartRun()
{
  ConvertParticleProcessToPointers();
  
  fBremsSplittingOperation->SetNSplit ( fNSplit );
  G4cout << GetName() << " : starting run with brem. splitting factor = " 
         << fNSplit;
  if ( fBiasPrimaryOnly ) G4cout << ", biasing only primaries ";
  else                    G4cout << ", biasing primary and secondary tracks ";
  if ( fBiasOnlyOnce )    G4cout << ", biasing only once per track ";
  else                    G4cout << ", biasing several times per track ";
  G4cout << " . " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmBOptrBremsSplitting::StartTracking( const G4Track* /* track */ )
{
  // -- reset the number of times the brem. splitting was applied:
  nInteractions = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation*
GmBOptrBremsSplitting::
ProposeFinalStateBiasingOperation(const G4Track* track,
                                  const G4BiasingProcessInterface* callingProcess )
{
  if( !CheckParticleProcess( track->GetDefinition(), callingProcess ) ) return 0;

      // -- Check if biasing of primary particle only is requested. If so, and
  // -- if particle is not a primary one, don't ask for biasing:
  if ( fBiasPrimaryOnly && ( track->GetParentID() !=0 ) ) return 0;
  // -- Check if brem. splitting should be applied only once to the track,
  // -- and if so, and if brem. splitting already occured, don't ask for biasing:
  if ( fBiasOnlyOnce    && ( nInteractions > 0 )        ) return 0;

  // -- Count the number of times the brem. splitting is applied:
  nInteractions++;
  // -- Return the brem. splitting operation:
  return fBremsSplittingOperation;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
