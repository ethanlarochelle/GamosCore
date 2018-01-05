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
// File G4GenerDistWavelengthFromFile.cc
// Description: Generates optical photons from a user-defined spectrum.
// Created: 2013-02-22
// Author: Adam Glaser
//
// This subroutine will generate a user-defined whitelight spectrum.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "GmGenerDistWavelengthFromFile.hh"
#include "GmGenerVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

GmGenerDistWavelengthFromFile::GmGenerDistWavelengthFromFile()
{

}

void GmGenerDistWavelengthFromFile::ReadWavelengthDist()
{
  // Read energy - probability  pairs
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  theFileName = FileInPath( path, theFileName );

  GmFileIn fin = GmFileIn::GetInstance(theFileName);

  std::vector<G4String> wl;
  std::map<G4double,G4double> enerprob;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistWavelengthFromFile::ReadWavelengthDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + theFileName 
			   + ", line number " + itoa(ii) 
			   + " All lines must have two words: WAVELENGTH PROBABILITY").c_str());
    } 

	G4String str=wl[0];
	
	G4int pos1=str.find("*wv");
	G4String str_new;
	G4double value=0;

		if( pos1 != -1 )
	    {
		str_new=str;
		str_new.erase(pos1+1,2);
		value=GetValue(str_new);
		enerprob[ GetValue( str_new )] = GetValue( wl[1] );
	    }
		else
		{
			G4Exception(" GmGenerDistWavelengthRandomFlat",
				"Wrong units",
				FatalErrorInArgument,
				"Please use wavelength units, *wv");
		}
    ii++;
  }

  //---- For histogram: Check that the binning is fixed
  std::map<G4double,G4double>::iterator ite, ite2;
  if( theCalculationType2 == EFFCT_Histogram2 ){
    for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
      ite2 = ite; ite2++;
      if( ite == enerprob.begin() ) {
	theHBin = (*ite2).first - (*ite).first;
      }
      if( ite2 != enerprob.end() ){
	G4double hbinn = (*ite2).first - (*ite).first;
	if( fabs(hbinn - theHBin) > 1.E-6*theHBin ) {
	 	  G4Exception("GmGenerDistWavelengthFromFile::ReadWavelengthDist",
	 		      "Asking for histogram type, and energy bin is not fixed",
	 		      FatalErrorInArgument,
	 		      G4String("Previous bin = " + ftoa(theHBin) + " new bin = " + ftoa(hbinn) + " difference= " + ftoa(hbinn-theHBin) ).c_str());
	 	}
      }
    }
  }

  //---- Calculate sum of probabilities to normalize 
  G4double tp = 0.;
  for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
    tp += (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistWavelengthFromFile::ReadWavelengthDist probabilities do not sum up to 1.0, but to " << tp << ". They will be normalized to 1.0 " << G4endl;
#endif
  G4double maxProbInv = 1./tp;

  //--- Get the inverse of probabilities - energies distribution
  tp = 0.;
  for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
    G4double prob = (*ite).second; 
      if( prob == 0. && std::distance( enerprob.begin(), ite ) == G4int(enerprob.size())-1 ) {
      prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
    }
    tp += prob * maxProbInv;
    theProbEner[tp] = (*ite).first;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistWavelengthFromFile  Reading data: Energy =  " << theProbEner[tp] 
	   << " Integrated probability= " << tp << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------//

G4double GmGenerDistWavelengthFromFile::GenerateEnergy( const GmParticleSource* )
{
  G4double energy;
  G4double pv = CLHEP::RandFlat::shoot();
  std::map<G4double,G4double>::iterator ite = theProbEner.upper_bound( pv );
  energy = (*ite).second;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile energy bin found =  " << energy << " prob = " << pv << G4endl;
#endif
  if( theCalculationType2 == EFFCT_Histogram2 ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile histogram prev energy =  " << energy << G4endl;
#endif
    energy += (-0.5 + CLHEP::RandFlat::shoot() ) * theHBin;
  } else if( theCalculationType2 == EFFCT_Interpolate2 ){
    if( std::distance(ite, theProbEner.end() ) != 1) ite++;  //because of precision it may happen that it returns the last one when pv=0.99999...
    G4double energyBin = (*ite).second - energy;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile old energy =  " 
				      << energy << " second energy " << (*ite).second << G4endl;
#endif
    energy +=  energyBin * CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile new energy =  " 
				      << energy << " energyBin " <<  energyBin << G4endl;
#endif
  } else if( theCalculationType2 == EFFCT_InterpolateLog2 ){
    if( std::distance(ite, theProbEner.end() ) != 1) ite++;  //because of precision it may happen that it returns the last one when pv=0.99999...
    G4double energyBin = log((*ite).second) - log(energy);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile old energy =  " 
				      << energy << " second energy " << (*ite).second << " energyBin " <<  energyBin << G4endl;
#endif
    energy *= exp(energyBin * CLHEP::RandFlat::shoot());
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile new energy =  " 
				      << energy << G4endl;
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistWavelengthFromFile  energy =  " 
				    << energy << G4endl;
#endif

  energy=1239.84187/energy/1000000./1000000.;
   
  return energy;

}

//---------------------------------------------------------------------//

void GmGenerDistWavelengthFromFile::SetParams( const std::vector<G4String>& params )
{

  G4int nParams = params.size();
  theUnit = 1.;
  G4String calcType = "histogram";

  switch (nParams) {
  case 3:
    theUnit = 1.;
  case 2:
    calcType = params[1];
  case 1:
    theFileName = params[0];
    break;
  default:
    G4Exception("GmGenerDistWavelengthFromFile::SetParams",
		  "Wrong number of parameters", 
		  FatalErrorInArgument,
		G4String("There should be 1, 2 or 3 parameters: FILE_NAME CALCULATION_TYPE UNIT, there are " + itoa(nParams)).c_str());
  }

  if( calcType == "fixed" ) {
    theCalculationType2 = EFFCT_Fixed2;
  } else if( calcType == "histogram" ) {
    theCalculationType2 = EFFCT_Histogram2;
  } else if( calcType == "interpolate" ) {
    theCalculationType2 = EFFCT_Interpolate2;
  } else if( calcType == "interpolate_log" ) {
    theCalculationType2 = EFFCT_InterpolateLog2;
  } else {
    G4Exception("GmGenerDistWavelengthFromFile::SetParams",
		"Error in type of calculation", 
		  FatalErrorInArgument,
		  G4String("only options supported: fixed , histogram, you have selected " + calcType).c_str());
  }

  ReadWavelengthDist();

}
