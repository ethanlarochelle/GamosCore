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
#include "GmClassifierMgr.hh"
#include "GmBaseVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "Reflex/PluginService.h"

GmClassifierMgr* GmClassifierMgr::theInstance = 0;

//----------------------------------------------------------------------
GmClassifierMgr::GmClassifierMgr()
{
}

//----------------------------------------------------------------------
GmClassifierMgr* GmClassifierMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmClassifierMgr;
  }

  return theInstance;

}

//----------------------------------------------------------------------
GmClassifierMgr::~GmClassifierMgr()
{
}

//----------------------------------------------------------------------
GmVClassifier* GmClassifierMgr::CreateClassifier( std::vector<G4String> params, G4bool bExists )
{
  GmVClassifier* classifier = Reflex::PluginService::Create<GmVClassifier*>(params[1],params[0]);
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " CreateClassifier NAME=" << params[0] << " CLASS=" << params[1] << " = " << classifier->GetName() << G4endl;
#endif

  if( classifier != 0 ) {
    classifier->SetClass( params[1] );
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    classifier->SetParameters( params );
    AddClassifier( classifier );

  } else if( bExists ) {
    G4Exception(" GmClassifierMgr::CreateClassifier() classifier type not found ",params[1],FatalException,"Please check documentation and your /gamos/classifier commands");
  }

  return classifier;
}

//----------------------------------------------------------------------
void GmClassifierMgr::AddClassifier( GmVClassifier* classifier )
{
  std::map<G4String,GmVClassifier*>::const_iterator ite = theClassifiers.find( classifier->GetName() );
  if( ite == theClassifiers.end() ) {
    theClassifiers[classifier->GetName()] = classifier;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierMgr::AddClassifier " << classifier->GetName() << G4endl;
#endif
  } else {
    G4Exception("GmClassifierMgr::Addclassifier"," Adding two classifiers with the same name, review your '/gamos/classifier' commands",FatalErrorInArgument,classifier->GetName());
  }

}


//----------------------------------------------------------------------
GmVClassifier* GmClassifierMgr::FindOrBuildClassifier( std::vector<G4String> params, G4bool bExists )
{
  GmVClassifier* classifier = GetClassifierFromList( params[0] );
  if( classifier != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierMgr::FindOrBuildClassifier classifier found " << params[0] << G4endl;
#endif
    return classifier;
  } else {
    return CreateClassifier( params, bExists );
  }

}

//----------------------------------------------------------------------
void GmClassifierMgr::SetClassifierIndices( std::vector<G4String> wl )
{
  GmVClassifier* classifier = GetClassifierFromList( wl[0] );
  if( classifier == 0 ) {
    G4Exception("GmClassifierMgr::SetClassifierIndices",
		"classifier not found at command /gamos/classifier/setIndices",
		FatalErrorInArgument,
		G4String("Classifier name= " + wl[0]).c_str());
  } else {
    wl.erase(wl.begin());
    classifier->CheckNumberOfIndices( wl );
    classifier->SetIndices( wl );
  }

}

//----------------------------------------------------------------------
GmVClassifier* GmClassifierMgr::GetClassifierFromList(const G4String& classifierName ) const
{
  std::map<G4String,GmVClassifier*>::const_iterator ite = theClassifiers.find( classifierName );
  if( ite != theClassifiers.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierMgr::GetClassifierFromList found " << classifierName << G4endl;
#endif
    return (*ite).second;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierMgr::GetClassifierFromList NOT found " << classifierName << G4endl;
#endif
    return 0;
  }
}

//----------------------------------------------------------------------
void GmClassifierMgr::DeleteClassifiers()
{
  std::map<G4String,GmVClassifier*>::iterator ite;

  for( ite = theClassifiers.begin(); ite != theClassifiers.end(); ite++ ){
    delete (*ite).second;
  }

  theClassifiers.clear();

}
