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
#include "GmGenerDistEnergyIsolethargical.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyIsolethargical::GmGenerDistEnergyIsolethargical()
{

}


//-----------------------------------------------------------------------
G4double GmGenerDistEnergyIsolethargical::GenerateEnergy( const GmParticleSource* )
{
  G4double energy = theEnergyMin * pow(theEnergyRatio, CLHEP::RandFlat::shoot());
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyIsolethargical energy = " << energy << G4endl;
#endif
  
  return energy;

}

//---------------------------------------------------------------------
void GmGenerDistEnergyIsolethargical::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 2 ) {
    G4Exception("GmGenerDistEnergyIsolethargical::SetParams",
		 "Wrong number of parameters",
		 FatalErrorInArgument,
		 G4String("There should be two parameters: ENERGY_MIN ENERGY_MAX, while there are "+ GmGenUtils::itoa(params.size())).c_str());
  }
  theEnergyMin = GmGenUtils::GetValue(params[0]);
  theEnergyMax = GmGenUtils::GetValue(params[1]);  

  theEnergyRatio = theEnergyMax / theEnergyMin;
}
