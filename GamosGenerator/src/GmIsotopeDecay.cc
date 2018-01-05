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
#include "GmIsotopeDecay.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4ParticleTable.hh"
#include "G4tgrUtils.hh"

GmIsotopeDecay::GmIsotopeDecay( const G4String& energy, const G4String& prob, const G4String& product )
{
  if( !GmGenUtils::IsNumber( energy ) ){
    G4Exception("GmIsotopeDecay::GmIsotopeDecay",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Energy should be given as a number " + energy).c_str());
  }
  theEnergy = G4tgrUtils::GetDouble( energy, CLHEP::keV); 

  if( !GmGenUtils::IsNumber( prob ) ){
    G4Exception("GmIsotopeDecay::GmIsotopeDecay",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Probability should be given as a number " + prob).c_str());
  }
  theProbability = GmGenUtils::GetValue( prob );

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable(); 
  theProduct = partTable->FindParticle(product);
  if( !theProduct ) {
    G4Exception("GmIsotopeDecay::GmIsotopeDecay","Error in argument",FatalErrorInArgument,(" particle does not exist: " + product).c_str() );
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmIsotopeDecay::GmIsotopeDecay  product " << theProduct << " energy " << theEnergy << " prob " << theProbability << G4endl;
#endif
}


G4String GmIsotopeDecay::GetProductName() const
{
  return theProduct->GetParticleName();
}
