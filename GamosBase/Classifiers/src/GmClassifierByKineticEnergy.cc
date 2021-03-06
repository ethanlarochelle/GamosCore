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
#include "GmClassifierByKineticEnergy.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------
GmClassifierByKineticEnergy::GmClassifierByKineticEnergy(G4String name) : GmVClassifier( name )
{
}

//---------------------------------------------------------------
void GmClassifierByKineticEnergy::SetParameters( std::vector<G4String>& wl)
{
  G4bool bStepExp = 1;
  if( wl.size() != 3 &&  wl.size() != 4 ) {
    G4Exception("GmClassifierByKineticEnergy::SetParameters"
		,"Error in number of parameters"
  		,JustWarning
		,G4String("There should be 3 or 4 arguments: MIN MAX STEP (IS_EXPONENTIAL_STEP), and there are only "+GmGenUtils::itoa(wl.size())).c_str());
    G4cout << " Parameters will be taken as: 1E-9*MeV 10.*MeV 10. " << G4endl;
    theMin = 1.E-9;
    theMax = 10.;
    theStep = 10.;
  } else {
    theMin = GmGenUtils::GetValue( wl[0] );
    theMax = GmGenUtils::GetValue( wl[1] );
    theStep = GmGenUtils::GetValue( wl[2] );
    if( wl.size() == 4 ) {
      bStepExp = GmGenUtils::GetValue( wl[3] );
    }
  }

  G4bool bMinCorrected = 0;
  if( bStepExp && theMin == 0. ) {
    G4Exception("GmClassifierByKineticEnergy::SetParameters"
		,"Minimum energy is 0. and step is exponential: minimum bin will be taken as 0 - 1.E-6 MeV"
		,JustWarning
		,G4String("Set the minimum energy <> 0 or set the IS_EXPONENTIAL_STEP (4th argument) to 0"));
    theMin = 1.e-6; 
    bMinCorrected = 1;
  }
  if( bStepExp && theStep <= 1. ) {
    G4Exception("GmClassifierByKineticEnergy::SetParameters"
		,"Energy step is < 1. and step is exponential"
		,FatalErrorInArgument
		,G4String("Set the step > 1. or set the IS_EXPONENTIAL_STEP (4th argument) to 0"));
  }

  G4double dd= theMin;
  G4int ii = 0;
  for( ;; ) {
    theIndexMap[dd] = theIndexMap.size();
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << ii <<  " GmClassifierByKineticEnergy theIndexMap " << dd << G4endl;
#endif
    if( dd >= theMax )break;
    if(bStepExp) {
      dd *= theStep;
    }else{ 
      dd += theStep;
    }
    if(ii > 1000) {
      G4Exception("GmClassifierByKineticEnergy::SetParameters"
		  ,"More than 1000 classifications"
		  ,FatalErrorInArgument
		  ,G4String("Check your parameters: Emin= " + G4UIcommand::ConvertToString(theMin)+ 
			    " Emax= " + G4UIcommand::ConvertToString(theMax)+
			    " Estep= " + G4UIcommand::ConvertToString(theStep)+
			    " IsExponentialStep= " + G4UIcommand::ConvertToString(bStepExp)));
    }
    ii++;
  }

  if( bMinCorrected ) theMin = 0.;

}
  
//---------------------------------------------------------------
G4int GmClassifierByKineticEnergy::GetIndexFromStep(const G4Step* aStep)
{
  G4double ener = aStep->GetPreStepPoint()->GetKineticEnergy();
  if( ener < theMin || ener > theMax ) G4Exception("GmEnergyClassifier::Classify","Energy out of limits",FatalErrorInArgument,G4String(GmGenUtils::ftoa(theMin)+ " <=? " + GmGenUtils::ftoa(ener)+" <=? "+GmGenUtils::ftoa(theMax)).c_str());

  std::map<G4double,G4int>::const_iterator ite = theIndexMap.lower_bound( ener );
  G4int index = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByKineticEnergy::GetIndexFromStep " << index << " " << ener <<  " " << theIndexMap.size() << G4endl;
#endif

  return index;
}

//---------------------------------------------------------------
G4int GmClassifierByKineticEnergy::GetIndexFromTrack(const G4Track* aTrack)
{
  G4double ener = aTrack->GetKineticEnergy();
  if( ener < theMin || ener > theMax ) G4Exception("GmEnergyClassifier::Classify","Energy out of limits",FatalErrorInArgument,G4String(GmGenUtils::ftoa(theMin)+ " <=? " + GmGenUtils::ftoa(ener)+" <=? "+GmGenUtils::ftoa(theMax)).c_str());

  std::map<G4double,G4int>::const_iterator ite = theIndexMap.lower_bound( ener );
  G4int index = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByKineticEnergy::GetIndex " << index << " " << ener <<  " " << theIndexMap.size() << G4endl;
#endif

  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByKineticEnergy::GetIndexName(G4int index)
{
  G4String name;
  std::map<G4double,G4int>::const_iterator ite, ite2;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if( (*ite).second == index ) {
      ite2 = ite; ite2--;
      name = G4UIcommand::ConvertToString((*ite2).first)
	+ "-" + G4UIcommand::ConvertToString((*ite).first);
    }
  }

  return name;
}

//---------------------------------------------------------------
GmClassifierByKineticEnergy::~GmClassifierByKineticEnergy()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByKineticEnergy " << theName << " " << theIndexMap.size() << G4endl;
  std::map<G4double,G4int>::const_iterator ite1 = theIndexMap.begin();
  std::map<G4double,G4int>::const_iterator ite2 = theIndexMap.begin();
  ite2++;
  G4int ii = 0;
  for(  ; ite2 != theIndexMap.end(); ite1++,ite2++,ii++ ){
    G4cout << theName << ": " << (*ite1).first << "-" << (*ite2).first << " = " << (*ite2).second << G4endl;
  }
}


//-------------------------------------------------------------
void GmClassifierByKineticEnergy::SetIndices( std::vector<G4String> wl )
{
  theMin = DBL_MAX;
  theMax = -DBL_MAX;
  theIndexMap.clear();
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    G4double value = GmGenUtils::GetValue(wl[ii]);
    theIndexMap[value] = G4int(GmGenUtils::GetValue(wl[ii+1]));
    if( theMin > value ) theMin = value;
    if( theMax < value ) theMax = value;
  }
  
}
