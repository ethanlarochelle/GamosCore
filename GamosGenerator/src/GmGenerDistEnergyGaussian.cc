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
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"


G4double GmGenerDistEnergyGaussian::GenerateEnergy( const GmParticleSource* )
{
 G4double ener = theSigma * CLHEP::RandGauss::shoot();
 ener += theMeanEnergy;

#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyGaussian::Generate energy " << ener/CLHEP::MeV << " MeV with mean kinetic energy = " << theMeanEnergy/CLHEP::MeV << " MeV and sigma = " << theSigma/CLHEP::MeV << " MeV "<< G4endl;
#endif
 return ener;
}

void GmGenerDistEnergyGaussian::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 ) {
    G4Exception(" GmGenerDistEnergyGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: MEAN_ENERGY SIGMA");
  }

  theMeanEnergy = GmGenUtils::GetValue( params[0] );
  theSigma = GmGenUtils::GetValue( params[1] );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyGaussian::SetParams theMeanEnergy = " << theMeanEnergy/CLHEP::MeV << " MeV and theSigma = " << theSigma/CLHEP::MeV << " MeV" << G4endl;
#endif
}
