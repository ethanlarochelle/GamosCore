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
#include "GmDistributionMgr.hh"
#include "GmVDistribution.hh"
#include "GmBaseVerbosity.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "Reflex/PluginService.h"

GmDistributionMgr* GmDistributionMgr::theInstance = 0;

//----------------------------------------------------------------------
GmDistributionMgr::GmDistributionMgr()
{
}

//----------------------------------------------------------------------
GmDistributionMgr* GmDistributionMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmDistributionMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmDistributionMgr::~GmDistributionMgr()
{
}

//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::CreateDistribution( std::vector<G4String> params, G4bool bExists )
{
  GmVDistribution* distrib = Reflex::PluginService::Create<GmVDistribution*>(params[1],params[0]);

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::CreateDistribution NAME=" << params[0] << " CLASS=" << params[1] << " = " << distrib << G4endl;
#endif
  if( distrib != 0 ) {
    distrib->SetClass(params[1]);
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    distrib->SetParameters( params );
    AddDistribution( distrib );

  } else if( bExists ) {
    G4Exception(" GmDistributionMgr::CreateDistribution  distrib type not found ",params[1],FatalException,"Please check with 'SealPluginDump ' and '/gamos/distrib' commands");
  }

  return distrib;
}

//----------------------------------------------------------------------
void GmDistributionMgr::AddDistribution( GmVDistribution* distrib )
{
  std::map<G4String,GmVDistribution*>::const_iterator ite = theDistributions.find( distrib->GetName() );
  if( ite == theDistributions.end() ) {
    theDistributions[distrib->GetName()] = distrib;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::AddDistribution " << distrib->GetName() << G4endl;
#endif
  } else {
    G4Exception("GmDistributionMgr::Adddistrib"," Adding two distribs with the same name, review your '/gamos/distrib' commands",FatalErrorInArgument,distrib->GetName());
  }

}


//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::FindOrBuildDistribution(std::vector<G4String> params, G4bool bExists )
{
  GmVDistribution* distrib = GetDistributionFromList( params[0] );
  if( distrib != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::FindOrBuildDistribution distrib found " << params[0] << G4endl;
#endif
    return distrib;
  } else {
    return CreateDistribution( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::GetDistributionFromList(const G4String& distribName ) const
{
  std::map<G4String,GmVDistribution*>::const_iterator ite = theDistributions.find( distribName );
  if( ite != theDistributions.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::GetDistributionFromList found " << distribName << G4endl;
#endif
    return (*ite).second;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::GetDistributionFromList NOT found " << distribName << G4endl;
#endif
    return 0;
  }
}


//----------------------------------------------------------------------
void GmDistributionMgr::DeleteDistributions()
{
  std::map<G4String,GmVDistribution*>::iterator ite;

  for( ite = theDistributions.begin(); ite != theDistributions.end(); ite++ ){
    delete (*ite).second;
  }

  theDistributions.clear();

}
