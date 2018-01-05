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
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4PhysicalVolumeStore.hh"

GmRegularParamUtils* GmRegularParamUtils::theInstance = 0;

//-----------------------------------------------------------------------
GmRegularParamUtils* GmRegularParamUtils::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmRegularParamUtils;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
G4PhantomParameterisation* GmRegularParamUtils::GetPhantomParam(G4bool bMustExist)
{
  G4PhantomParameterisation* paramreg = 0;

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    if( IsPhantomVolume( *cite ) ) {
      const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*cite);
      G4VPVParameterisation* param = pvparam->GetParameterisation();
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //      G4cout << "G4PhantomParameterisation volume found  " << (*cite)->GetName() << G4endl;
      paramreg = static_cast<G4PhantomParameterisation*>(param);
    }
  }
  
  if( !paramreg && bMustExist ) G4Exception("GmRegularParamUtils::GetPhantomParam",
					    "Wrong argument",
					    FatalErrorInArgument,
					    "No G4PhantomParameterisation found ");
  
  return paramreg;
  
}


//-----------------------------------------------------------------------
G4PhantomParameterisation* GmRegularParamUtils::GetPhantomParam( G4VPhysicalVolume* pv, G4bool bMustExist )
{
  G4PhantomParameterisation* paramreg = 0;
  
  if( IsPhantomVolume( pv ) ) {
    const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(pv);
    G4VPVParameterisation* param = pvparam->GetParameterisation();
    //    if( static_cast<const G4PhantomParameterisation*>(param) ){
    //    if( static_cast<const G4PhantomParameterisation*>(param) ){
    //    G4cout << "G4PhantomParameterisation volume found  " << pv->GetName() << G4endl;
    paramreg = static_cast<G4PhantomParameterisation*>(param);
  } else { 
    if( bMustExist ) G4Exception("GmRegularParamUtils::GetPhantomParam",
		"Wrong argument",
		FatalErrorInArgument,
				 G4String("Volume does not contain a regular structure of type 1:  " + pv->GetName()).c_str());
  }

  return paramreg;
    
}


//-----------------------------------------------------------------------
G4bool GmRegularParamUtils::IsPhantomVolume( G4VPhysicalVolume* pv )
{
  EAxis axis;
  G4int nReplicas;
  G4double width,offset;
  G4bool consuming;
  pv->GetReplicationData(axis,nReplicas,width,offset,consuming);
  EVolume type = (consuming) ? kReplica : kParameterised;
  if( type == kParameterised && pv->GetRegularStructureId() == 1 ) {  
    return TRUE;
  } else {
    return FALSE;
  }

} 


//-----------------------------------------------------------------------
G4ThreeVector GmRegularParamUtils::GetPhantomMotherTranslation( G4bool bMustExist )
{
  G4ThreeVector translation;

  G4LogicalVolume* phantomVol = GetPhantomMotherVolume( bMustExist);
  
  //  translation = phantomVol->GetTranslation();
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( phantomVol->GetName());
  GmTouchable* touch = touchs[0];

  translation = touch->GetGlobalPosition();
  for( std::vector<GmTouchable*>::iterator ite = touchs.begin(); ite != touchs.end(); ite++ ){
    delete *ite;
  }

  G4cout << phantomVol->GetName() << " GmRegularParamUtils::GetPhantomMotherTranslation " << translation << " global " << touch->GetGlobalPosition() << G4endl;
  return translation;
  
}

//-----------------------------------------------------------------------
G4RotationMatrix GmRegularParamUtils::GetPhantomMotherRotation( G4bool bMustExist )
{
  G4RotationMatrix rotation;
  
  G4LogicalVolume* phantomVol = GetPhantomMotherVolume( bMustExist);
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( phantomVol->GetName() );
  GmTouchable* touch = touchs[0];
  rotation = touch->GetGlobalRotation();
  
  G4cout << phantomVol->GetName() << " GmRegularParamUtils::GetPhantomMotherRotation " << rotation << " global " << touch->GetGlobalRotation() << G4endl;
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    delete *ite;
  }

  return rotation;
  
}


//-----------------------------------------------------------------------
G4LogicalVolume* GmRegularParamUtils::GetPhantomMotherVolume( G4bool bMustExist)
{
  G4LogicalVolume* phantVol = 0;

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    if( IsPhantomVolume( *cite ) ) {
      phantVol = (*cite)->GetMotherLogical();
      break;
      //      G4cout << "G4PhantomParameterisation volume found  " << (*cite)->GetName() << G4endl;
    }
  }
  
  if( !phantVol && bMustExist ) G4Exception("GmRegularParamUtils::GetPhantomMotherVolume",
		"Wrong argument",
		FatalErrorInArgument,
					    "No G4PhantomParameterisation volume found ");
  
  return phantVol;
  
}

 
