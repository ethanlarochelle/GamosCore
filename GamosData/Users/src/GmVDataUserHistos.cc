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
#include "GmVDataUserHistos.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmVDataUserHistos::GmVDataUserHistos()
{
}

 
//----------------------------------------------------------------
void GmVDataUserHistos::BookHistos(G4int index, GmVClassifier* theClassifier)
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  theHistos.insert(index);
  G4String histoName = theHistoName;
  G4String hname;
  if( theClassifier ) {
    hname += theClassifier->GetIndexName(index);
    histoName += sepa + hname;
  }
  unsigned int MAXNDATA = 100;
  if( theData.size() > MAXNDATA ) {
    G4Exception("GmDataUserHistos::BookHistos",
		"Too many data",
		FatalErrorInArgument,
		G4String("Maximum number of data allowed is " + GmGenUtils::itoa(MAXNDATA) + ", while you are asking for " + GmGenUtils::itoa(theData.size())).c_str());
  }
  G4int histoNumber = theHistoNumber + index*MAXNDATA;

  std::vector<GmVData*>::const_iterator ite;
  // Create histograms 1D
  for( ite = theData.begin(); ite != theData.end(); ite++ ){
    histoNumber++;
    GmVData* data = *ite;
    data->BookHisto( histoName, histoNumber, index, theAnaMgr );
  }

  // Create histograms 2D
  //  std::vector<GmVData*>::const_iterator ite;
  /*  for( ite = theDoubleData.begin(); ite != theDoubleData.end(); ite++ ){
    GmVData* data = *ite;
    G4String dname = data->GetName();

  theAnaMgr->CreateHisto2D(histoName+": Position pre Y vs Z",theHPosNbins,theHPosMin,theHPosMax,theHPosNbins,theHPosMin,theHPosMax,histoNumber+35);
  } */

}


//----------------------------------------------------------------
void GmVDataUserHistos::EndOfRun()
{
  theHistos.clear();
  
  if( !bPrintHistoStats ) return;

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      //      GmVData* data = *ite;
      //t      data->PrintHisto();
    }
}
