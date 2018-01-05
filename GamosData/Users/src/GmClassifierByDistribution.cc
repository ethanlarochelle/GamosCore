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
#include "GmClassifierByDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmStringDistribution.hh"

#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------
GmClassifierByDistribution::GmClassifierByDistribution(G4String name) : GmVClassifier( name )
{
}

//---------------------------------------------------------------
void GmClassifierByDistribution::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 1 ) {
    G4Exception("GmClassifierByDistribution::SetParameters",
		"Error in number of parameters",
  		JustWarning,
		G4String("There should be one argument: DISTRIBUTION_NAME, and there are "+GmGenUtils::itoa(params.size())).c_str());
  }

  theDistribution = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);

  if( theDistribution->GetClass().find("Numeric") != std::string::npos ) {
    GmVNumericDistribution* distN = (GmVNumericDistribution*)(theDistribution);
    std::map<G4double,G4double> distValues = distN->GetValues();
    std::map<G4double,G4double>::const_iterator ite1 = distValues.begin();
    std::map<G4double,G4double>::const_iterator ite2 = ite1;
    ite2++;
    G4int ii = 0;
    for( ; ite2 != distValues.end(); ite1++, ite2++,ii++ ){
      G4int index = G4int( distN->GetNumericValueFromIndex( ((*ite1).first + (*ite2).first)/2.) ) ;
      theIndexNamesMap[index] = GmGenUtils::ftoa((*ite1).first) + "-" + GmGenUtils::ftoa((*ite2).first);
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(debugVerb) ) G4cout << " GmClassifierByDistribution::SetParameters add IndexNameMap " << ii << " " << index << " = " << theIndexNamesMap[index] << G4endl;
#endif
    }
  } else {
    G4Exception("GmClassifierByDistribution::SetParameters",
		"Only numeric distributions are supported",
		FatalErrorInArgument,
		G4String("Distribution: " + theDistribution->GetName() + " of class " + theDistribution->GetClass()).c_str());
  }

}
  
//---------------------------------------------------------------
G4int GmClassifierByDistribution::GetIndexFromStep(const G4Step* aStep)
{
  G4double val = theDistribution->GetValueFromStep( aStep );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
G4int GmClassifierByDistribution::GetIndexFromTrack(const G4Track* aTrack)
{
  G4double val = theDistribution->GetValueFromTrack( aTrack );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
G4int GmClassifierByDistribution::GetIndexFromValue(const G4double val )
{
 G4int index = G4int(val);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmClassifierByDistribution::GetIndexFromValue " << index << " " << val << G4endl;
#endif

  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByDistribution::GetIndexName(G4int index)
{
  G4String name;
  std::map<G4int,G4String>::const_iterator ite = theIndexNamesMap.find(index);
  if( ite != theIndexNamesMap.end() ) {
    name = (*ite).second;
  }

  return name;
}

//---------------------------------------------------------------
GmClassifierByDistribution::~GmClassifierByDistribution()
{
  // not implemented for all types of distributions yet
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByDistribution " << theName << " " << theIndexNamesMap.size() << G4endl;
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = theIndexNamesMap.begin() ; ite != theIndexNamesMap.end(); ite++ ){
    G4cout << theName << " = " << (*ite).first << ": " << (*ite).second << G4endl;
  }
  
}

