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
#include "GmGenerDistEnergyMCNPisolethargical.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyMCNPisolethargical::GmGenerDistEnergyMCNPisolethargical()
{

}

//-----------------------------------------------------------------------
void GmGenerDistEnergyMCNPisolethargical::ReadEnergyDist()
{
  // Read energy - probability  pairs
  GmFileIn fin = GmFileIn::GetInstance(theFileName);
  std::vector<G4String> wl;
  std::map<G4double,G4double> enerprob;
  G4int i1 = 1;

  fin.GetWordsInLine( wl );
  if( wl.size() != 3) {
    G4Exception("GmGenerDistEnergyMCNPisolethargical::ReadEnergyDist",
		"Error in number of words in line",
		FatalErrorInArgument,
		G4String("Reading file " + theFileName 
			 + ", line number " + GmGenUtils::itoa(i1) 
			 + " All lines must have two words: ENERGY PROBABILITY").c_str());
  }
   
  G4double minE = GmGenUtils::GetValue(wl[0]);
  G4double maxE = GmGenUtils::GetValue(wl[2]);
  G4int nBins = G4int(GmGenUtils::GetValue(wl[1]));

  for( G4int ii = 0; ii < nBins; ii++ ){
    G4double ener = minE * pow(double(maxE/minE), double(double(ii)/nBins));
    enerprob[ ener ] = 1./nBins;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical  adding energy " << ener << " prob " << enerprob[ ener ] << G4endl;
#endif
  }
  enerprob[ maxE ] = 0.;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical  adding energy " << maxE << " prob  0. " << G4endl;
#endif

  //---- Calculate sum of probabilities to normalize 
  G4double tp = 0.;
  std::map<G4double,G4double>::iterator ite;
  for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
    tp += (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistEnergyMCNPisolethargical::ReadEnergyDist probabilities do not sum up to 1., but to " << tp << " diff= " << tp-1. << " They will be normalized to 1. " << G4endl;
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
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical  Reading data: Energy =  " << theProbEner[tp] 
				     << " Integrated probability= " << tp << " N prob " << theProbEner.size()
				     << "  prob " << prob << " " << std::distance( enerprob.begin(), ite ) << "== " <<  G4int(enerprob.size())-1  
				     << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
G4double GmGenerDistEnergyMCNPisolethargical::GenerateEnergy( const GmParticleSource* )
{
  G4double energy;
  G4double pv = CLHEP::RandFlat::shoot();
  std::map<G4double,G4double>::iterator ite = theProbEner.upper_bound( pv );
  energy = (*ite).second;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical energy bin found =  " << energy << " prob = " << pv << G4endl;
#endif
  
  if( std::distance(ite, theProbEner.end() ) != 1) ite++;  //because of precision it may happen that it returns the last one when pv=0.99999...
  G4double energyBin = (*ite).second - energy;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical old energy =  " << energy << " second energy " << (*ite).second << G4endl;
#endif
  energy +=  energyBin * CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical new energy =  " << energy << " energyBin " <<  energyBin << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyMCNPisolethargical  energy =  " << energy << G4endl;
#endif

  return energy;

}

//---------------------------------------------------------------------
void GmGenerDistEnergyMCNPisolethargical::SetParams( const std::vector<G4String>& params )
{
  theFileName = params[0];

  ReadEnergyDist();

}
