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
#include "GmEventDataHistosUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmEventDataHistosUA::GmEventDataHistosUA()
{
  theDataType = DTEvent;

}


//----------------------------------------------------------------
void GmEventDataHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("GmEventDataHistosUA",theFilters,theClassifier);
}


//----------------------------------------------------------------
void GmEventDataHistosUA::BeginOfRunAction(const G4Run* )
{
  SetUserName( theName );

  theDataList.clear();
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  G4int index = 0;
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier);
}


//----------------------------------------------------------------
void GmEventDataHistosUA::BeginOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( evt, 0 );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::UserSteppingAction(const G4Step* aStep )
{
  /*  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndex(aStep);
  } else {
    index = 0;
  }
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );
  */

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep, 0 );
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::EndOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( evt, 0 ); // print theDataAccumulated
      }
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
