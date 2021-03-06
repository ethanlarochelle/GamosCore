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
#include "GmClassifierByLogicalVolume.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"

//------------------------------------------------------------------
GmClassifierByLogicalVolume::GmClassifierByLogicalVolume(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByLogicalVolume::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByLogicalVolume::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}


//------------------------------------------------------------------
G4int GmClassifierByLogicalVolume::GetIndexFromStep(const G4Step* aStep)
{
  G4int index;
  G4LogicalVolume* lv = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
  std::map<G4LogicalVolume*,G4int>::const_iterator ite = theIndexMap.find(lv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[lv] = index;
  } else {
    index = (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByLogicalVolume::GetIndexFromStep " << index << " lv " << lv->GetName() << G4endl;
#endif

  return index;
}

//------------------------------------------------------------------
G4int GmClassifierByLogicalVolume::GetIndexFromTrack(const G4Track* aTrack)
{
  G4int index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  std::map<G4LogicalVolume*,G4int>::const_iterator ite = theIndexMap.find(lv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[lv] = index;
  } else {
    index = (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByLogicalVolume::GetIndex " << index << " lv " << lv->GetName() << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByLogicalVolume::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<G4LogicalVolume*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByLogicalVolume::~GmClassifierByLogicalVolume()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByLogicalVolume " << theName << G4endl;
  std::map<G4LogicalVolume*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << theName << ": " << (*ite).first->GetName() << " = " << (*ite).second << G4endl;
  }
}


#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
//#include "GamosCore/GamosBase/Base/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmModuleJaws.hh"

//-------------------------------------------------------------
void GmClassifierByLogicalVolume::SetIndices( std::vector<G4String> wl )
{
	const std::vector<G4String> par;
	new GmModuleJaws(par);
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
//  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
	//GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
    //std::vector<G4LogicalVolume*> keys = GmGeometryUtils::GetInstance()->GetLogicalVolumes( wl[ii], 1, -1 );
/*    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4LogicalVolume* key = keys[jj];
      G4int index = G4int(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
  */  
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
