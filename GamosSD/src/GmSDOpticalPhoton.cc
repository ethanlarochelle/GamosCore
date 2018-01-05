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
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"

#include "GmSDOpticalPhoton.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmSDOpticalPhoton::GmSDOpticalPhoton(G4String sdtype): GmSDSimple( sdtype )
{
}

//--------------------------------------------------------------------
GmSDOpticalPhoton::~GmSDOpticalPhoton(){}

//--------------------------------------------------------------------
G4bool GmSDOpticalPhoton::IsInterestingStep( G4Step* aStep )
{
  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption" ) {
    return TRUE;
  } else { 
    return FALSE;
  }
}


//----------------------------------------------------------------------
G4double GmSDOpticalPhoton::GetEnergyInSD( G4Step* aStep )
{
  return aStep->GetPostStepPoint()->GetKineticEnergy();
  
}
