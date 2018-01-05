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
#include "GmPDSGammaProcess.hh"
#include "GmPDSProcessHelper.hh"
#include "GmPDSInteractionAngleManager.hh"
#include "GmPDSScore.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GmPDSUtils.hh"

#include "G4Gamma.hh"
#include "G4Geantino.hh"
#include "G4SteppingManager.hh"
#include "G4LogicalVolumeStore.hh"

//#include <math.h>

//------------------------------------------------------------------
GmPDSGammaProcess::GmPDSGammaProcess(const G4String& name, G4SteppingManager* fpSM ): GmPDSVProcess( name, fpSM)
{

  theOriginalParticle = G4Gamma::Gamma();

  G4String angleFileName = GetPDSStringParameter("AngleDeviationFileName","gamma","");

  theInteractionAngleManager = new GmPDSInteractionAngleManager(angleFileName);

  bUseMaximumDistance = G4bool(GetPDSNumericParameter("UseVRMaximumDistance","gamma",0));
  theMaximumDistance = GetPDSNumericParameter("MaximumDistance","gamma",1000.);
  theInvMaximumDistanceRR = 1./(GetPDSNumericParameter("MaximumDistanceRRFactor","gamma",100));

  theExclusionRadius = GetPDSNumericParameter("ExclusionRadius",theOriginalParticle->GetParticleName(),1.*CLHEP::mm);

}
  
//------------------------------------------------------------------
GmPDSGammaProcess::~GmPDSGammaProcess()
{
}


//------------------------------------------------------------------
void GmPDSGammaProcess::AddHelper( GmPDSProcessHelper* helper )
{
  theHelpers[helper->IsForNeutron()] = helper;
  theCurrentHelper = helper; //there is only one helper

  theClassifier = theCurrentHelper->theClassifier;
  bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:ClassifierOnTrack",1)); 
  bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:FiltersOnTrack",1)); 
}
