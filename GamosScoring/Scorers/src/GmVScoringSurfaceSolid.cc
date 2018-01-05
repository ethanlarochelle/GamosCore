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
#include "GmVScoringSurfaceSolid.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmVScoringSurfaceSolid::GmVScoringSurfaceSolid()
{
  theSurfaceTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  theAngularTolerance = G4GeometryTolerance::GetInstance()->GetAngularTolerance();
}

//-----------------------------------------------------------------------
G4ThreeVector GmVScoringSurfaceSolid::GetLocalPoint( G4ThreeVector pos, G4Step* aStep ) 
{
  G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  //  G4cout << " GmVScoringSurfaceSolid::GetLocalPoint global " << pos << " local " << touchable->GetHistory()->GetTopTransform().TransformPoint(pos)
  // << "transform " << touchable->GetHistory()->GetTopTransform().NetRotation() 
  // << "transform " << touchable->GetHistory()->GetTopTransform().NetTranslation() << G4endl;
  return touchable->GetHistory()->GetTopTransform().TransformPoint(pos);
}

