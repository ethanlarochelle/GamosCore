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
#include "GmRecHitBuilderByBlock.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
GmRecHitBuilderByBlock::GmRecHitBuilderByBlock() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByBlock:NAncestors", 2));
  theNShift = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByBlock:NShift", 100));

  theNShift = G4int(pow(double(theNShift),NAncestors-1));
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlock: NShift " << theNShift << G4endl;
#endif
}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilderByBlock::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlock::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( itec = theRecHits.begin(); itec != theRecHits.end(); itec++ ){
      if( CheckHitInRecHit (*itec, *iteh ) ) {
	rhitFound = true;
	(*itec)->AddHit( *iteh );
	break;
      }
    }
    if( !rhitFound ) {
      theRecHits.push_back( new GmRecHit( *iteh ) );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlock::ReconstructHits:  n RecHits = " << theRecHits.size() << G4endl;
#endif

  return theRecHits;
}


//------------------------------------------------------------------------
G4bool GmRecHitBuilderByBlock::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{
  
  if( int(rhit->GetHits()[0]->GetDetUnitID()/theNShift) == int(hit->GetDetUnitID()/theNShift) ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByBlock::CheckHitInRecHit merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByBlock::CheckHitInRecHit do not merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return false;
  }

}
