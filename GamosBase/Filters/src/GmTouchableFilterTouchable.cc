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
#include "GmTouchableFilterTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterTouchable::GmTouchableFilterTouchable()
{
  geomUtils = GmGeometryUtils::GetInstance();
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterTouchable::~GmTouchableFilterTouchable()
{ 
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterTouchable::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  for( unsigned int ii = 0; ii < theTouchableNames.size(); ii++){
    vpsi ancestors = theTouchableNames[ii];
    G4int sizV = ancestors.size();
    if( touch->GetHistoryDepth() < sizV ) continue;
    G4bool bFound = TRUE;
    for( G4int jj = 0; jj < sizV; jj++ ){
      G4VPhysicalVolume* pv = touch->GetVolume(jj);
      G4bool cn = !geomUtils->CheckPVCopyNo( pv, ancestors[jj].second ) ;
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable check " <<  pv->GetName() << " != " << ancestors[jj].first << " " 
	     << pv->GetCopyNo() << "- " << ancestors[jj].second 
	     << "?" << (pv->GetName() != ancestors[jj].first)
	     << "?" << G4int(cn)
	     <<G4endl;
#endif
      if( pv->GetName() != ancestors[jj].first ||
	  !geomUtils->CheckPVCopyNo( pv, ancestors[jj].second )  ){
	bFound = FALSE; 
	break; 
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return " << bFound << G4endl;
#endif
    if( bFound ) return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return 0 " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchable::show()
{
  G4cout << "----GmTouchableFilterTouchable volume list------"<<G4endl; 
  std::vector<vpsi>::const_iterator iter;
  for ( iter = theTouchableNames.begin(); iter != theTouchableNames.end(); iter++ ){
    //    G4cout << "GmTouchableFilterTouchable volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchable::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterTouchable::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    vpsi ancestors = geomUtils->ExtractAncestorsRequested( params[ii] );
    theTouchableNames.push_back(ancestors);
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmTouchableFilterTouchable::SetParameters add parameter " << params[ii] << G4endl;
#endif
  }
}
