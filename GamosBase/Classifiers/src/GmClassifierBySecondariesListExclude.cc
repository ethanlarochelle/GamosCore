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
#include "GmClassifierBySecondariesListExclude.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//------------------------------------------------------------------
GmClassifierBySecondariesListExclude::GmClassifierBySecondariesListExclude(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierBySecondariesListExclude::SetParameters( std::vector<G4String>& params)
{

  for( unsigned int ii=0; ii < params.size(); ii++ ){
    theParticlesToExclude.insert(params[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesListExclude::SetParameters add particle " << params[ii] << " TOTAL= " << theParticlesToExclude.size() << G4endl;
#endif
  }
}

//------------------------------------------------------------------
G4int GmClassifierBySecondariesListExclude::GetIndexFromStep(const G4Step* )
{
  G4int index;
  std::vector<G4Track*> secoTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  std::vector<G4Track*>::const_iterator itet;
  std::multiset<G4String> secoNames;
  for( itet = secoTracks.begin(); itet != secoTracks.end(); itet++ ){
    G4String partName = (*itet)->GetDefinition()->GetParticleName();
    if( theParticlesToExclude.find(partName) != theParticlesToExclude.end() ) continue;
    secoNames.insert( partName );
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
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesListExclude::GetIndexFromStep add IndexMap " << name << " = " << index << G4endl;
#endif
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesListExclude::GetIndexFromStep " << index << " Secondary_names " << name << G4endl;
#endif
  
  return index;
}

//------------------------------------------------------------------
G4int GmClassifierBySecondariesListExclude::GetIndexFromTrack(const G4Track* )
{
  return 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesListExclude::GetIndex  0 " << G4endl;
#endif

}

//--------------------------------------------------------------
G4String GmClassifierBySecondariesListExclude::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesListExclude::GetIndexName " << index << G4endl;
#endif
    if((*ite).second == index ){
      return (*ite).first;
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierBySecondariesListExclude::~GmClassifierBySecondariesListExclude()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierBySecondariesListExclude " << theName << G4endl;
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << (*ite).first << " = " << (*ite).second << G4endl;
  }
}

