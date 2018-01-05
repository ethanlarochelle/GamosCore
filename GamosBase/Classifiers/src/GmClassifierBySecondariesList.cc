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
#include "GmClassifierBySecondariesList.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <set>
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"

//------------------------------------------------------------------
GmClassifierBySecondariesList::GmClassifierBySecondariesList(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierBySecondariesList::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierBySecondariesList::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//------------------------------------------------------------------
G4int GmClassifierBySecondariesList::GetIndexFromStep(const G4Step* )
{
  G4int index;
  std::vector<G4Track*> secoTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  std::vector<G4Track*>::const_iterator itet;
  std::multiset<G4String> secoNames;
  for( itet = secoTracks.begin(); itet != secoTracks.end(); itet++ ){
    secoNames.insert( (*itet)->GetDefinition()->GetParticleName() );
  }
  G4String name = "";
  std::multiset<G4String>::const_iterator ites;
  for( ites = secoNames.begin(); ites != secoNames.end(); ites++ ){
    if( ites != secoNames.begin() ) name += "_";
    name += (*ites);
  }

  std::map<const G4String,G4int>::const_iterator ite = theIndexMap.find(name);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1;
    theIndexMap[name] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesList::GetIndexFromStep " << index << " Secondary_names " << name << G4endl;
#endif
  
  return index;
}

//------------------------------------------------------------------
G4int GmClassifierBySecondariesList::GetIndexFromTrack(const G4Track* )
{
  return 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesList::GetIndex  0 " << G4endl;
#endif

}

//--------------------------------------------------------------
G4String GmClassifierBySecondariesList::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      return (*ite).first;
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierBySecondariesList::~GmClassifierBySecondariesList()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierBySecondariesList " << theName << G4endl;
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << (*ite).first << " = " << (*ite).second << G4endl;
  }
}

