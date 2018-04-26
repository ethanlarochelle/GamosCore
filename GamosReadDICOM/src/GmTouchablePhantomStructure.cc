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
#include "GmTouchablePhantomStructure.hh"
#include "GmReadPhantomSVMgr.hh"
#include "GmRegularParamUtils.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

//---------------------------------------------------------------------
GmTouchablePhantomStructure::GmTouchablePhantomStructure()
{
  theRegularUtils = GmRegularParamUtils::GetInstance();
  thePVMgr = GmReadPhantomSVMgr::GetInstance();
}

//---------------------------------------------------------------------
GmTouchablePhantomStructure::~GmTouchablePhantomStructure()
{ 
}

//---------------------------------------------------------------------
G4bool GmTouchablePhantomStructure::AcceptTouchable(const G4VTouchable* touch)
{
  G4VPhysicalVolume* pv = touch->GetVolume();
  if( !pv ) {
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << " GmTouchablePhantomStructure::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }

  if( theRegularUtils->IsPhantomVolume( pv ) ){
    //    G4PhantomParameterisation* pparam = theRegularUtils->GetPhantomParam( true );  
    G4int idx = thePVMgr->GetSVID( touch->GetReplicaNumber() );
    if( idx != -1 ) {
      //      G4cout << pv->GetCopyNo() << " GmTouchablePhantomStructure idx " << idx << G4endl;
      if( CheckIndex(idx) ) {
      //      G4cout << " GmTouchablePhantomStructure idx " << idx << " TRUE " << G4endl;   
	return TRUE;
      }else {
      //      G4cout << " GmTouchablePhantomStructure idx " << idx << " FALSE " << G4endl;     
	return FALSE;
      }
    }
  }

  return FALSE;
}

//---------------------------------------------------------------------
G4bool GmTouchablePhantomStructure::CheckIndex( G4int idx ) 
{
  //split index 
  for( G4int ii = 0;; ii++) {
    G4int ns = pow(theIdxShift,ii);
    if( ns >= idx ) break;
    G4int idx1 = (idx/ns)%theIdxShift;
    if( theIndices.find(idx1) != theIndices.end() ) {
      return true;
    }
  }

  return false;

}
//---------------------------------------------------------------------
void GmTouchablePhantomStructure::show()
{
  G4cout << "----GmTouchablePhantomStructure index list------"<<G4endl; 
  std::set<G4int>::const_iterator ite;
  for( ite = theIndices.begin(); ite != theIndices.end(); ite++ ){
    G4cout << "GmTouchablePhantomStructure index " << (*ite) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchablePhantomStructure::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchablePhantomStructure::SetParameters","There should be at least one parameter: PARTICLE_NAME"
		,FatalErrorInArgument,"There is no parameter");
  }
  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    theIndices.insert(thePVMgr->GetSVIDFromPhysVolName(params[ii]));
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) )  G4cout << ii << " GmTouchablePhantomStructure theIndices " << thePVMgr->GetSVIDFromPhysVolName(params[ii]) << G4endl;
#endif
  }

  theIdxShift = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPhantomStructure:IDShift",100));
}

