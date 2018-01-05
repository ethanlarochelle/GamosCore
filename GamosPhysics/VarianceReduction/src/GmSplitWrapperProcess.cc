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
#include "GmSplitWrapperProcess.hh"

GmSplitWrapperProcess::GmSplitWrapperProcess(const G4String& aName,
                                         G4ProcessType   aType)
  : G4WrapperProcess(aName,aType), theNSplit(0)
{
  pRegProcess = (G4VProcess*)(0);
}

GmSplitWrapperProcess::~GmSplitWrapperProcess()
{
  if (pRegProcess!=0) delete pRegProcess;
}

G4VParticleChange* GmSplitWrapperProcess::PostStepDoIt( const G4Track& track,
                                                   const G4Step&  stepData )
{
  std::vector<G4Track*> theSecos;
  G4VParticleChange* pchan;
  for( G4int ii = 0; ii < theNSplit; ii++ ) {
    pchan = pRegProcess->PostStepDoIt( track, stepData );        
    if( ii != theNSplit -1 ) { // the last one has already the list of secondaries, do not add it
      G4int nseco = pchan->GetNumberOfSecondaries(); 
      for( G4int in = 0; in < nseco; in++ ) {
	theSecos.push_back( pchan->GetSecondary(in) );
      }
    }
  }

  G4int nseco = theSecos.size(); 
  pchan->SetNumberOfSecondaries( nseco + pchan->GetNumberOfSecondaries() );
  for( G4int in = 0; in < nseco; in++ ) {
    pchan->AddSecondary( theSecos[in] );
  }

  return pchan;

}
