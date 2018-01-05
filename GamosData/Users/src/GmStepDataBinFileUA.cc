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
#include "GmStepDataBinFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include <iostream>

//----------------------------------------------------------------
GmStepDataBinFileUA::GmStepDataBinFileUA()
{
  theDataType = DTStep;
}

//----------------------------------------------------------------
void GmStepDataBinFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );
  
  theOutputType = theName;

  //  G4cout << this << " GmStepDataBinFileUA::BeginOfRunAction theOutputType " << theOutputType << G4endl;
  Init();
  OpenFileOutBin();

  theDataList.clear();
  theDataList.push_back("EventID");
  theDataList.push_back("TrackID");
  theDataList.push_back("Particle");
  theDataList.push_back("FinalPosX");
  theDataList.push_back("FinalPosY");
  theDataList.push_back("FinalPosZ");
  theDataList.push_back("FinalMomX");
  theDataList.push_back("FinalMomY");
  theDataList.push_back("FinalMomZ");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutBin( theFileOutBin );
    }

  WriteHeaderBin();
}

#include "G4Event.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------
void GmStepDataBinFileUA::UserSteppingAction(const G4Step* aStep )
{
  std::vector<GmVData*>::const_iterator ite;
  G4int ii = 0;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      GmVData* data = *ite;
      
      data->WriteBin( aStep );
      /*      if( ii == 11 ) {
	theFileOutBin->close();
	exit(1); 
      }*/
      ii++;
    }

  theNCallsWritten++;
/*  if(  theNStepsWritten == 3) {
    theFileOutBin->close();
    exit(1);
    } */

}


//----------------------------------------------------------------
void GmStepDataBinFileUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
