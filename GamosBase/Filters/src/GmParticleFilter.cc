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
#include "GmParticleFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Track.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//------------------------------------------------------------------------
GmParticleFilter::GmParticleFilter(G4String name)
  :GmVFilter(name)
{
  theParticles.clear();
}


//------------------------------------------------------------------------
GmParticleFilter::~GmParticleFilter()
{ 
  theParticles.clear();
}


//------------------------------------------------------------------------
G4bool GmParticleFilter::AcceptTrack(const G4Track* aTrack)
{
  if( theParticles.find( aTrack->GetDefinition() ) != theParticles.end() ){
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParticleFilter::AcceptTrack 1 " <<  aTrack->GetDefinition()->GetParticleName() << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParticleFilter::AcceptTrack 0 " <<  aTrack->GetDefinition()->GetParticleName() << G4endl;
#endif
    return FALSE;
  }
}


//------------------------------------------------------------------------
void GmParticleFilter::show()
{
  G4cout << "----G4ParticleFilter particle list------"<<G4endl;
  std::set<G4ParticleDefinition*>::const_iterator ite;
  for( ite = theParticles.begin(); ite != theParticles.end(); ite++ ){
    G4cout << (*ite)->GetParticleName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}


//------------------------------------------------------------------------
void GmParticleFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmParticleFilter::SetParameters","There should be at least one parameter: PARTICLE_NAME",FatalErrorInArgument,"There is no parameter");
  }
  
  std::vector<G4ParticleDefinition*>::const_iterator ite;
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4ParticleDefinition*> parts = GmGetParticleMgr::GetInstance()->GetG4ParticleList( params[ii], 1 );

    for( ite = parts.begin(); ite != parts.end(); ite++ ){
      G4ParticleDefinition* part = partTable->FindParticle( *ite);
      if( part == 0 ) {
	G4Exception("GmParticleFilter::SetParameters","particle not found ", FatalErrorInArgument, params[ii]);
      } else {
	theParticles.insert(part);
#ifndef GAMOS_NO_VERBOSE
	if( BaseVerb(debugVerb) ) G4cout << " GmParticleFilter::SetParameters add particle " << params[ii] << G4endl;
#endif
      }     
    }
  }
  
}
