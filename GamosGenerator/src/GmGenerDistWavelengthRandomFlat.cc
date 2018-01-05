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
////////////////////////////////////////////////////////////////////////
// Optical Photon User-Defined Wavelength Distribution
////////////////////////////////////////////////////////////////////////
//
// File G4GenerDistWavelengthRandomFlat.cc
// Description: Generates optical photons uniformly in a waveband.
// Created: 2013-02-22
// Author: Adam Glaser
//
// This subroutine will generate a uniform whitelight spectrum.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "GmGenerDistWavelengthRandomFlat.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"


G4double GmGenerDistWavelengthRandomFlat::GenerateEnergy( const GmParticleSource* )
{
 G4double ener = CLHEP::RandFlat::shoot( theEnergyMin, theEnergyMax );

#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistWavelengthRandomFlat::Generate  energy " << ener << " " << theEnergyMin<< " " << theEnergyMax <<  G4endl;
#endif
 return 1239.84187/ener/1000000./1000000.;
}

void GmGenerDistWavelengthRandomFlat::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 ) {
    G4Exception(" GmGenerDistWavelengthRandomFlat::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: wavelengthMin wavelengthMax");
  }

  	G4String str1=params[0];
	G4int pos1=str1.find("*wv");
	G4String str2=params[1];
	G4int pos2=str2.find("*wv");
	G4String str_new1;
	G4String str_new2;

		if( pos1 != -1 )
	    {
		str_new1=str1;
		str_new1.erase(pos1+1,2);
		theEnergyMin=GetValue(str_new1);
	    }
		else
		{
			G4Exception(" GmGenerDistWavelengthRandomFlat",
				"Wrong units",
				FatalErrorInArgument,
				"Please use wavelength units, *wv");
		}  

		if( pos2 != -2 )
	    {
		str_new2=str2;
		str_new2.erase(pos2+1,2);
		theEnergyMax=GetValue(str_new2);
	    }
		else
		{
			G4Exception(" GmGenerDistWavelengthRandomFlat",
				"Wrong units",
				FatalErrorInArgument,
				"Please use wavelength units, *wv");
		}

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistWavelengthRandomFlat::SetParams " << theEnergyMin << " " <<  params[0] << " " <<  G4UnitDefinition::GetValueOf(params[1]) << G4endl;
#endif
}
