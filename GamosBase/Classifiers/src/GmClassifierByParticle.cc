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
#include "GmClassifierByParticle.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
//---------------------------------------------------------------
GmClassifierByParticle::GmClassifierByParticle(G4String name) : GmVClassifier( name )
{
}

//---------------------------------------------------------------
void GmClassifierByParticle::SetParameters( std::vector<G4String>& params)
{
  if(params.size() != 0 ){
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii < params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByParticle::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//---------------------------------------------------------------
G4int GmClassifierByParticle::GetIndexFromStep(const G4Step* aStep)
{
  return GetIndexFromTrack( aStep->GetTrack() );
}


//---------------------------------------------------------------
G4int GmClassifierByParticle::GetIndexFromTrack(const G4Track* aTrack)
{
  G4int index;
  G4ParticleDefinition* part = aTrack->GetDefinition();
  std::map<G4ParticleDefinition*,G4int>::const_iterator ite = theIndexMap.find(part);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[part] = index;
  } else {
    index = (*ite).second;
  }
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByParticle::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<G4ParticleDefinition*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetParticleName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByParticle::~GmClassifierByParticle()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByParticle " << theName << G4endl;
  std::map<G4ParticleDefinition*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << theName << ": " << (*ite).first->GetParticleName() << " = " << (*ite).second << G4endl;
  }
}

//-------------------------------------------------------------
void GmClassifierByParticle::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
    std::vector<G4ParticleDefinition*> keys = GmGetParticleMgr::GetInstance()->GetG4ParticleList( wl[ii] );
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4ParticleDefinition* key = keys[jj];
      G4int index = G4int(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
      //      G4cout << " key " << key->GetParticleName() << " " <<  GmGenUtils::GetValue(wl[ii+1]) << G4endl;
    }
    
  }
 
  theMaxIndex -= theIndexMap.size()+1;   
}
