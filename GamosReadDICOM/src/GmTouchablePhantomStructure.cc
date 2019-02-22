#include "GmTouchablePhantomStructure.hh"
#include "GmReadPhantomStMgr.hh"
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
  thePVMgr = GmReadPhantomStMgr::GetInstance();
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
    G4int idx = thePVMgr->GetStID( touch->GetReplicaNumber() );
    if( idx != -1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << pv->GetCopyNo() << " GmTouchablePhantomStructure idx " << idx << G4endl;
#endif
      if( CheckIndex(idx) ) {
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) )   G4cout << " GmTouchablePhantomStructure idx " << idx << " TRUE " << G4endl;   
#endif
	return TRUE;
      }else {
 #ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) ) G4cout << " GmTouchablePhantomStructure idx " << idx << " FALSE " << G4endl;     
#endif
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
  G4int ROIShift = thePVMgr->GetROIShift();
  for( G4int ii = 0;; ii++) {
    G4int ns = pow(ROIShift,ii);
    if( ns > idx ) break;
    G4int idx1 = (idx/ns)%ROIShift;
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
    theIndices.insert(thePVMgr->GetStIDFromPhysVolName(params[ii]));
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) )  G4cout << ii << " GmTouchablePhantomStructure theIndices " << thePVMgr->GetStIDFromPhysVolName(params[ii]) << G4endl;
#endif
  }

  G4int ROIShift = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPhantomStructure:ROIShift",100));
  thePVMgr->SetROIShift(ROIShift);  
}

