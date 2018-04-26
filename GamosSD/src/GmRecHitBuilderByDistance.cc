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
#include "GmRecHitBuilderByDistance.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmRecHitBuilderByDistance::GmRecHitBuilderByDistance() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theHitsDist = parmgr->GetNumericValue("SD:GmRecHitBuilderByDistance:HitsDistInRecHit", 10*CLHEP::mm);
}


std::vector<GmRecHit*> GmRecHitBuilderByDistance::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByDistance::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*> recHits;
  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( itec = recHits.begin(); itec != recHits.end(); itec++ ){
      if( CheckHitInRecHit (*itec, *iteh ) ) {
	rhitFound = true;
	(*itec)->AddHit( *iteh );
	break;
      }
    }
    if( !rhitFound ) {
      recHits.push_back( new GmRecHit( *iteh ) );
    }
  }

  return recHits;
}


G4bool GmRecHitBuilderByDistance::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(infoVerb) ) G4cout << this << " ItacaRecHit::CheckHitInRecHit  rhit= " << *rhit << " hit= " << *hit << G4endl;
#endif
  if( (rhit->GetPosition() - hit->GetPosition() ).mag() < theHitsDist ){
    return true;
  } else {
    return false;
  }

}
