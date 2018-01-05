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
#include "GmMovementRotation.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"

//------------------------------------------------------------------------
GmMovementRotation::GmMovementRotation(std::vector<G4String>& words, GmMovementType mt )
  :GmVMovement( words, mt )
{
}

//------------------------------------------------------------------------
G4bool GmMovementRotation::Move()
{
  //  G4RunManager::GetRunManager()->AbortRun();
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();

  std::vector<G4VPhysicalVolume*>::iterator itev;
  for(itev = theVolumes.begin(); itev != theVolumes.end(); itev++ ){
    geomMgr->OpenGeometry(*itev);

    G4RotationMatrix* rm = (*itev)->GetRotation();
    rm->rotate(theValue,theAxis);
    (*itev)->SetRotation(rm);

    geomMgr->CloseGeometry(true,false,*itev);
  }
  
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  return true;
}

//------------------------------------------------------------------------
GmMovementRotation::~GmMovementRotation() 
{
}
