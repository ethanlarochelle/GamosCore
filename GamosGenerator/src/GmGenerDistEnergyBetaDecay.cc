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
#include "GmGenerDistEnergyBetaDecay.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//-----------------------------------------------------------------------
GmGenerDistEnergyBetaDecay::GmGenerDistEnergyBetaDecay()
{
  bFileRead = FALSE;
}


//-----------------------------------------------------------------------
G4double GmGenerDistEnergyBetaDecay::GenerateEnergy( const GmParticleSource* source )
{
  if( source->GetType() != "IsotopeSource" ) {
    G4Exception(" GmGenerDistEnergyBetaDecay::GenerateEnergy",
		"Wrong argument",
		FatalErrorInArgument,
		"Source is not isotope");
  }

  GmIsotopeSource* isoSource = static_cast<GmIsotopeSource*>(const_cast<GmParticleSource*>(source));

  if( isoSource->GetCurrentDecay()->GetProductName() == "gamma" ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay decays to gamma, using constant energy " << isoSource->GetName() << G4endl;
#endif
    return isoSource->GetCurrentDecay()->GetEnergy();
  } else {
    G4String filename;
    if( isoSource->GetCurrentDecay()->GetProductName() == "e+" ){
      filename = "EnergyDist."+source->GetParticleName()+".BetaPlus.dat";
    } else if( isoSource->GetCurrentDecay()->GetProductName() == "e-" ){
      filename = "EnergyDist."+source->GetParticleName()+".BetaMinus.dat";
    }
    if( !bFileRead ) {
      ReadFile( filename );
      bFileRead = TRUE;
    }
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay::Generate " << G4endl;
#endif
    G4bool bFound = false;
    G4double energy, prob;
    do{
      energy = CLHEP::RandFlat::shoot(theProb.size()-1.E-6);
      prob = CLHEP::RandFlat::shoot(theProbMax);
      if( prob < theProb[int(energy)] ) bFound = true;
    }while(!bFound);
    
    return energy*CLHEP::keV;    
  }

  return 0; //for compilation warning
}


//-----------------------------------------------------------------------
void GmGenerDistEnergyBetaDecay::ReadFile( const G4String fname )
{

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay reading file " << fname << G4endl;
#endif

  // read input file 
  G4String filename = fname;
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );
  GmFileIn fin = GmFileIn::GetInstance( filename );
  fin.SetSuppressQuotes(0);

  std::vector<G4String> wl;
  G4bool readingData = false;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( readingData == true ) {
      //      if( wl.size() == 1 && wl[0] == "\">" ) break;
      if( wl[0].find(";") == std::string::npos ) break;
      //--- separate line in words by ";"
      unsigned int i1 = 0;
      for(unsigned int ii = 0; ii < wl[0].length(); ii++ ) {
	if( wl[0][ii] == ';' ) {
	  G4String val = wl[0].substr(i1,ii-i1);
	  theProb.push_back( GmGenUtils::GetValue( val ) );
	  i1 = ii+1;
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay " << theProb.size()-1 << " prob " << theProb[theProb.size()-1] << " = " << val <<  G4endl;
#endif
	}
      }
    }
    for( unsigned int iw = 0; iw < wl.size(); iw++ ){
      //      if( wl[iw] == "name=\"y[]\"" ) {
      if( wl[iw] == "name=y[]" ) {
	readingData = true;
	break;
      }
    }
  }

  unsigned int siz = theProb.size();
  if( siz == 0 ) G4Exception(" GmGenerDistEnergyBetaDecay::ReadFile",	
			     "Wrong argument",
			     FatalErrorInArgument,
			     G4String("Data file is wrong: " + fname + " Please contact the GAMOS experts").c_str());

  theProbMax = 0.;
  for( unsigned int ii = 0; ii < siz; ii++ ) {
    theProbMax = std::max( theProbMax, theProb[ii] );
  }

  fin.Close();

}
