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
#include "GmDataAccumulatedEnergyDeposited.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedEnergyDeposited::GmDataAccumulatedEnergyDeposited()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedEnergyDeposited::~GmDataAccumulatedEnergyDeposited()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedEnergyDeposited::Accumulate( const G4Step* aStep, G4int index )
{
  theDataAccumulated[index] += aStep->GetTotalEnergyDeposit();
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmDataAccumulatedEnergyDeposited::Accumulate " << index << " = " << theDataAccumulated[index] << " energyDepo " <<  aStep->GetTotalEnergyDeposit() << G4endl;
#endif
}

//----------------------------------------------------------------
G4double GmDataAccumulatedEnergyDeposited::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTotalEnergyDeposit();
}
