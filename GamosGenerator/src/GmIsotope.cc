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
#include "GmIsotope.hh"
#include "CLHEP/Random/RandFlat.h"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
 
//-----------------------------------------------------------------------
GmIsotope::GmIsotope( const G4String& name ): theName( name )
{
}

//-----------------------------------------------------------------------
void GmIsotope::AddDecay( GmIsotopeDecay* dec )
{
  theDecays.push_back( dec );
}

//-----------------------------------------------------------------------
std::vector<GmIsotopeDecay*> GmIsotope::GenerateDecayProducts()
{
  std::vector<GmIsotopeDecay*> decs;
  while(decs.size() == 0 ){
    for( unsigned int ii=0; ii < theDecays.size(); ii++ ) {
      if( CLHEP::RandFlat::shoot() < theDecays[ii]->GetProbability() ){
	decs.push_back( theDecays[ii] );
      }
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "GmIsotope::GenerateDecayProducts() trying decay " << theDecays[ii]->GetProductName() << " decs size " << decs.size() << G4endl;
#endif
    }
  }

  return decs;
}
