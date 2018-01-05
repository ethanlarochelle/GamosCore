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
#include "GmRunDataTextFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmRunDataTextFileUA::GmRunDataTextFileUA()
{

  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmRunDataTextFileUA::BeginOfRunAction( const G4Run* aRun)
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << " theOutputType " << theOutputType << " filters " << theFilters.size() << G4endl;
  Init();
  OpenFileOut();

  theDataList.clear();
  theDataList.push_back("RunID");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData() ;
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutText( theFileOut );
    }

  WriteHeaderText();

  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteText( aRun, ite == theData.begin() );// print theDataAccumulated
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmRunDataTextFileUA::UserSteppingAction(const G4Step* aStep )
{

  //  G4cout << " GmRunDataTextFileUA::UserSteppingAction( " << theDataList.size() << " =? " << theData.size() << G4endl;
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
      //      G4cout << " GmRunDataTextFileUA::UserSteppingAction( " << G4endl;
    }

}


//----------------------------------------------------------------
void GmRunDataTextFileUA::EndOfRunAction( const G4Run* aRun)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteText( aRun, ite == theData.begin() );// print theDataAccumulated
      }
    }
  *theFileOut << G4endl;  
  
  theFileOut->close();
}
