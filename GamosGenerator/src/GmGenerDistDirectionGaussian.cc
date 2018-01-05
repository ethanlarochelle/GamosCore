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
#include "GmGenerDistDirectionGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"

G4ThreeVector GmGenerDistDirectionGaussian::GenerateDirection( const GmParticleSource* )
{
  G4double dirX = CLHEP::RandGauss::shoot( 0., theSigmaX );
  G4double dirY = CLHEP::RandGauss::shoot( 0., theSigmaY );

  G4ThreeVector dir = G4ThreeVector( dirX, dirY, 1 );

  dir.rotateZ( theTheta );
  dir.rotateX( thePhi );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionGaussian::Generate  dir "  << dir << G4endl;
#endif
  return dir;

}
//---------------------------------------------------------------------
void GmGenerDistDirectionGaussian::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 5 ) {
    G4Exception("GmGenerDistDirectionConst::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 3 parameters: DIR_X DIR_Y DIR_Z SIGMA_Y SIGMA_Z");
  }
  
  G4ThreeVector dir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );
  thePhi = dir.phi();
  theTheta = dir.theta();
  theSigmaX = GmGenUtils::GetValue( params[3] );
  theSigmaY = GmGenUtils::GetValue( params[4] );
}
