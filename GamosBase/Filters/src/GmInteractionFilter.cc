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
#include "GmInteractionFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmInteractionFilter::GmInteractionFilter(G4String name)
  :GmVFilter(name)
{
}

//--------------------------------------------------------------------
GmInteractionFilter::~GmInteractionFilter()
{ 
}

//--------------------------------------------------------------------
G4bool GmInteractionFilter::AcceptStep(const G4Step* aStep)
{
  G4VProcess* proc = const_cast<G4VProcess*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  G4ProcessType procType = proc->GetProcessType();
  if( procType == fElectromagnetic
      || procType == fOptical
      || procType == fHadronic
      || procType == fPhotolepton_hadron
      || procType == fDecay
      || procType == fGeneral
      || procType == fParameterisation ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInteractionFilter::AcceptStep 1 " << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInteractionFilter::AcceptStep 0 " << G4endl;
#endif
  return FALSE;
}

