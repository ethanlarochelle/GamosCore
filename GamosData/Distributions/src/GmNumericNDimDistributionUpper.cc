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
#include "GmNumericNDimDistributionUpper.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericNDimDistributionUpper::GmNumericNDimDistributionUpper(G4String name)
  : GmVNumericNDimDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericNDimDistributionUpper::InterpolateValue( const std::vector<std::vector<G4int> >& indices )
{
  
  std::vector<G4int> indexVal;
  std::vector<G4int> ind1 = indices[indices.size()-1];
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    indexVal.push_back( ind1[ii] );
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      G4cout << "NDim Upper indexVal " << indexVal[ii] << G4endl; //GDEB
    }
#endif
  }
  G4double val = theValuesNDim[indexVal];
  
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericNDimDistributionUpper::InterpolateValue " << val << G4endl;
#endif

  //   std::map<std::vector<G4int>,G4double> theValuesNDim;

  return val;

}

