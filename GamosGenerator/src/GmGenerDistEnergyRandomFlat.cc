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
#include "GmGenerDistEnergyRandomFlat.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"


G4double GmGenerDistEnergyRandomFlat::GenerateEnergy( const GmParticleSource* )
{
 G4double ener = CLHEP::RandFlat::shoot( theEnergyMin, theEnergyMax );
#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyRandomFlat::Generate  energy " << ener << " " << theEnergyMin<< " " << theEnergyMax <<  G4endl;
#endif
 return ener;
}

void GmGenerDistEnergyRandomFlat::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 ) {
    G4Exception(" GmGenerDistEnergyRandomFlat::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: energyMin energyMax");
  }

  theEnergyMin = GmGenUtils::GetValue( params[0] );
  theEnergyMax = GmGenUtils::GetValue( params[1] );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyRandomFlat::SetParams " << theEnergyMin << " " <<  params[0] << " " <<  G4UnitDefinition::GetValueOf(params[1]) << G4endl;
#endif
}
