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
#include "GmDataAccumulatedLengthInVolume.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedLengthInVolume::GmDataAccumulatedLengthInVolume()
{
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.clear(); // delete DTRun
  theVolumeNames = GmParameterMgr::GetInstance()->GetVStringValue("GmDataAccumulatedLengthInVolume:Volumes",theVolumeNames);
  if( theVolumeNames.size() == 0 ) {
    G4Exception("GmDataAccumulatedLengthInVolume::GmDataAccumulatedLengthInVolume",
		"",
		FatalException,
		"No volume name given, please use '/gamos/setParam GmDataAccumulatedLengthInVolume:Volumes VOL_NAME_1 VOL_NAME_2 ...'");
  }
}

//----------------------------------------------------------------
GmDataAccumulatedLengthInVolume::~GmDataAccumulatedLengthInVolume()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedLengthInVolume::Accumulate( const G4Step* aStep, G4int index )
{
  G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  for( size_t ii = 0; ii < theVolumeNames.size(); ii++ ) {
    if( GmGenUtils::AreWordsEquivalent(volName,theVolumeNames[ii]) ) {
      theDataAccumulated[index] += aStep->GetStepLength();
    }
    //    G4cout << " GmDataAccumulatedLengthInVolume::Accumulate " << volName <<" : " <<  theDataAccumulated[index] << G4endl; //GDEB
  }
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLengthInVolume::GetValueFromStep( const G4Step* aStep, G4int )
{
  return GetValueFromTrack( aStep->GetTrack() );
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLengthInVolume::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* , G4int )
{ 
  return GetValueFromTrack( aTrack1 );
}
