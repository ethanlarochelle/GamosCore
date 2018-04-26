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
#include "GmMaterialBudgetUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

//----------------------------------------------------------------
GmMaterialBudgetUA::GmMaterialBudgetUA()
{
}

void GmMaterialBudgetUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("matbud",theFilters,theClassifier);
}

//----------------------------------------------------------------
void GmMaterialBudgetUA::BeginOfRunAction( const G4Run* )
{
}

 
//----------------------------------------------------------------
void GmMaterialBudgetUA::BookHistos(G4int index)
{
  theHistos.insert(index);

  G4double worldHalfX;
  G4double worldHalfY;
  G4double worldHalfZ;
  G4LogicalVolume* worldLV = GmGeometryUtils::GetInstance()->GetTopLV();
  if( worldLV->GetSolid()->GetEntityType() != "G4Box" ){
    G4Box* world = static_cast<G4Box*>(worldLV->GetSolid());
    worldHalfX = world->GetXHalfLength();
    worldHalfY = world->GetYHalfLength();
    worldHalfZ = world->GetZHalfLength();
  } else if( worldLV->GetSolid()->GetEntityType() != "G4Tubs" ){
    G4Tubs* world = static_cast<G4Tubs*>(worldLV->GetSolid());
    worldHalfX = world->GetOuterRadius();
    worldHalfY = world->GetOuterRadius();
    worldHalfZ = world->GetZHalfLength();
  } else {
    G4Exception("GmMaterialBudgetUA::BookHistos",
		"",
		FatalErrorInArgument,
		G4String("Only world shapes supported are G4Box and G4Tubs, your world is a " + worldLV->GetSolid()->GetEntityType() ).c_str());
  } 
  G4int nBins = 100;
  
  G4String hname;
  if( theClassifier ) hname += theClassifier->GetIndexName(index);
  G4String histoName = theHistoName + "_" + hname;
  G4int histoNumber = theHistoNumber + index*100;
  // Create histograms
  theAnaMgr->CreateHistoProfile1D(histoName+": Position X",nBins,-worldHalfX,worldHalfX,histoNumber+11);
  theAnaMgr->CreateHistoProfile1D(histoName+": Position Y",nBins,-worldHalfY,worldHalfY,histoNumber+12);
  theAnaMgr->CreateHistoProfile1D(histoName+": Position Z",nBins,-worldHalfZ,worldHalfZ,histoNumber+13);
  theAnaMgr->CreateHistoProfile2D(histoName+": Position XY",nBins,worldHalfX,worldHalfX,nBins,worldHalfY,worldHalfY,histoNumber+21);
  theAnaMgr->CreateHistoProfile2D(histoName+": Position XZ",nBins,worldHalfX,worldHalfX,nBins,worldHalfZ,worldHalfZ,histoNumber+22);
  theAnaMgr->CreateHistoProfile2D(histoName+": Position YZ",nBins,worldHalfY,worldHalfY,nBins,worldHalfZ,worldHalfZ,histoNumber+23);

}

//----------------------------------------------------------------
void GmMaterialBudgetUA::UserSteppingAction(const G4Step* aStep )
{
  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index);

  //  G4cout << " COPY " << aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0) << G4endl;
  G4double matBud = 1./aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetRadlen() * 
    aStep->GetStepLength();
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) {
    G4cout << " MATBUD " << matBud  << " -> " << theMatBud[index] << " " << aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetRadlen()  << " " << aStep->GetStepLength() << " " << aStep->GetTrack()->GetPosition().z() << G4endl; 
    G4cout << " mate " << aStep->GetTrack()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
  }
#endif
  if(matBud > 1.e30) matBud = 0.;

  theMatBud[index] += matBud;
}

//----------------------------------------------------------------
void GmMaterialBudgetUA::PostUserTrackingAction(const G4Track* aTrack )
{

  std::map<G4int,G4double>::const_iterator ite;
  for( ite = theMatBud.begin(); ite != theMatBud.end(); ite++) {
    G4int index = (*ite).first;
    G4int histoNumber = theHistoNumber + index*100;
    G4ThreeVector pos = aTrack->GetPosition();

    //    G4cout << "MATBUDTRK " << pos << " " << (*ite).second << G4endl;
    theAnaMgr->GetHistoProf1(histoNumber+11)->Fill(pos.x(),(*ite).second);
    theAnaMgr->GetHistoProf1(histoNumber+12)->Fill(pos.y(),(*ite).second);
    theAnaMgr->GetHistoProf1(histoNumber+13)->Fill(pos.z(),(*ite).second);
    theAnaMgr->GetHistoProf2(histoNumber+21)->Fill(pos.x(),pos.y(),(*ite).second);
    theAnaMgr->GetHistoProf2(histoNumber+22)->Fill(pos.x(),pos.z(),(*ite).second);
    theAnaMgr->GetHistoProf2(histoNumber+23)->Fill(pos.y(),pos.z(),(*ite).second);
  }

  theMatBud.clear();

}


//----------------------------------------------------------------
void GmMaterialBudgetUA::EndOfRunAction( const G4Run* )
{
}
