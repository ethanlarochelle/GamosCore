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
// GmG4PSData
#include "GmG4PSData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// (Description)
//   This is a primitive scorer class for scoring from a GmData
//

GmG4PSData::GmG4PSData(G4String name)
  :GmVPrimitiveScorer(name)
{
}

GmG4PSData::~GmG4PSData()
{;}

G4bool GmG4PSData::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  FillScorer( aStep, theData[0]->GetValueFromStep(aStep), aStep->GetPreStepPoint()->GetWeight());

  return TRUE;
}

void GmG4PSData::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmG4PSData::SetParameters",
		"There should be one parameter: DATA_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }
  
  theData.push_back( Build1Data( params[0] ) );

}
