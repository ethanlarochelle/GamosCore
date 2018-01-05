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
#include "GmSecondaryTrackDataCoutUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmSecondaryTrackDataCoutUA::GmSecondaryTrackDataCoutUA()
{
  theDataType = DTSeco;
}

//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theDataList.clear(); 
  theDataList.push_back("EventID");
  theDataList.push_back("TrackID");
  theDataList.push_back("InitialPrimMinusSecoKineticEnergy");
  theDataList.push_back("FinalPrimMinusSecoKineticEnergy");
  theDataList.push_back("SecoDividedInitialPrimKineticEnergy");  
  theDataList.push_back("SecoKineticEnergy");
  theDataList.push_back("FinalPrimSecoAngle");        
  theDataList.push_back("InitialPrimKineticEnergy");
  theDataList.push_back("FinalPrimKineticEnergy");

  BuildData();
 
}

//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::UserSteppingAction(const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();
  G4TrackVector secondaries = GetStepSecondaries();
  unsigned int nSeco = secondaries.size();

  std::vector<GmVData*>::const_iterator ite;
  std::vector<GmVFilter*>::const_iterator itef;

  // pass GmOnSecondaryFilter to each secondary track
  for(unsigned int ii = 0; ii < nSeco; ii++ ){
    G4Track* secoTrack = secondaries[ii];
    G4bool bSecoAccepted = true;
    for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
      if( (*itef)->GetClass() == "GmOnSecondaryFilter" ) {
	//       	G4cout << " GmOnSecondaryFilter " << (*itef)->GetName() << " = " << (*itef)->AcceptTrack(secoTrack)  << G4endl;
	if( !(*itef)->AcceptTrack(secoTrack) ) {
	  bSecoAccepted = false; 
	  break;
	}
      }
    }
    if( !bSecoAccepted ) continue;
    for( ite = theData.begin(); ite != theData.end(); ite++ ) {
      (*ite)->WriteCout( aTrack, secoTrack, ite == theData.begin() );
    }
  }
  if( nSeco != 0 ) G4cout << G4endl;  

}


//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::EndOfRunAction( const G4Run* )
{
}
