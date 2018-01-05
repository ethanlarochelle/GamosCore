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
#include "G4VEmProcess.hh"

#include "GmVPhysicsGamma.hh"
#include "GmPhysicsVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4RegionStore.hh"

#include "G4LivermorePhotoElectricModel.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

//----------------------------------------------------------------------
GmVPhysicsGamma::GmVPhysicsGamma(const G4String& name, G4int type): G4VPhysicsConstructor(name, type)
{ }

//----------------------------------------------------------------------
GmVPhysicsGamma::~GmVPhysicsGamma()
{ }


// ----------------------------------------------------------------------------
void GmVPhysicsGamma::AddDeexcitation()
{
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  G4bool bFluo = theParamMgr->GetNumericValue("AtomicDeexcitation:Fluorescence",1);
  G4bool bAuger = theParamMgr->GetNumericValue("AtomicDeexcitation:Auger",1);
  G4bool bPIXE = theParamMgr->GetNumericValue("AtomicDeexcitation:PIXE",0);
  if( bFluo) de->SetFluo(true);
  if( bAuger ) de->SetAuger(true);
  if( bPIXE ) de->SetPIXE(true);
  
  std::vector<G4String> regions;
  regions = theParamMgr->GetVStringValue("AtomicDeexcitation:Regions",regions); 
  if( regions.size() == 0 ) regions.push_back("DefaultRegionForTheWorld");
  for( size_t ii = 0; ii < regions.size(); ii++ ){
    de->SetDeexcitationActiveRegion(regions[ii], 1, bAuger, bPIXE );
  }
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);

}

