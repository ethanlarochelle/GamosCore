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
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"

//------------------------------------------------------------------------
GmVGenerDistPositionVolumesAndSurfaces::GmVGenerDistPositionVolumesAndSurfaces()
{
 
  theName = "GmVGenerDistPositionVolumesAndSurfaces";

 //- theTouchable = new G4TouchableHistory();

  theGeomUtils = GmGeometryUtils::GetInstance();

  bOnlyVolume = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmVGenerDistPositionVolumesAndSurfaces:OnlyVolume",0));
}

//------------------------------------------------------------------------
GmVGenerDistPositionVolumesAndSurfaces::~GmVGenerDistPositionVolumesAndSurfaces()
{
}


//------------------------------------------------------------------------
void GmVGenerDistPositionVolumesAndSurfaces::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    G4Exception("GmVGenerDistPositionVolumesAndSurfaces::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"No volume defined: you have to add at least one, as the last argument in the command line setting this distribution ");
  }

  std::vector<GVSTouchableInfo*> tinfos = theOrigin->AddTouchableInfos( params );
  for(unsigned int jj=0; jj<theTouchableInfos.size(); jj++) { //gdl
    delete theTouchableInfos[jj];//gdl
  }//gdl
  theTouchableInfos.clear();
  theTotalObjectDimensions.clear();

  //--- Set total objects dimensions
  for( std::vector<GVSTouchableInfo*>::const_iterator ite = tinfos.begin(); ite != tinfos.end(); ite++ ) {
    theTouchableInfos.push_back( *ite );

    const G4VSolid* solid = (*ite)->solid;
    theTotalObjectDimensions.push_back(theObject->GetObjectDimension(solid) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::AddVolume adding volume " << (*ite)->name << " dimension " << theObject->GetObjectDimension(solid) << G4endl;
#endif
    unsigned int siz = theTotalObjectDimensions.size();
    if( siz > 1 ) theTotalObjectDimensions[siz-1] += theTotalObjectDimensions[siz-2];

    theObject->BuildSurfaceAreas( solid );
  }

}

//------------------------------------------------------------------------
G4ThreeVector GmVGenerDistPositionVolumesAndSurfaces::GeneratePosition( const GmParticleSource* )
{
  //--------- Select an object
  unsigned int siz = theTotalObjectDimensions.size();
  G4double rvol = CLHEP::RandFlat::shoot() * theTotalObjectDimensions[siz-1];
  //-  G4cout << "rvolshoot " << rvol << " " <<  theTotalObjectDimensions[siz-1] << G4endl;
  unsigned int io;
  for( io = 0; io < siz; io++ ){
    if( rvol <= theTotalObjectDimensions[io] ) {
      break;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate in volume io " << io << " " << theTouchableInfos[io]->name << " rvol " << rvol << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  //  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4Volumes::Generate in volume " << theTouchables[io]->GetSolid()->GetName() << G4endl;
#endif

  G4ThreeVector pos;
  G4String volName;
  do {
    
    pos = theObject->GeneratePosition( theTouchableInfos[io] );
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate pos before trans " << pos << G4endl;
#endif
    
    pos = theTouchableInfos[io]->rotmat * pos;
    pos += theTouchableInfos[io]->pos;
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate pos " << pos << " touchable global pos " <<theTouchableInfos[io]->pos  << G4endl;
#endif
    volName = theGeomUtils->BuildTouchableName( pos );
    
  } while(volName != theTouchableInfos[io]->name && bOnlyVolume );
    
  return pos;

}
