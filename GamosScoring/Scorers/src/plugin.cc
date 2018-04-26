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
#include "Reflex/PluginService.h"

#include "GmG4PSCellCharge.hh"
#include "GmG4PSCellFlux.hh"
#include "GmG4PSDoseDeposit_old.hh"
#include "GmG4PSDoseDeposit.hh"
#include "GmG4PSDoseDeposit_NoSkipEqualMate.hh"
#include "GmG4PSDoseDeposit_SkipEqualMateSimple.hh"
#include "GmPSSphericalDoseDeposit.hh"
#include "GmPSCylindricalRZDoseDeposit.hh"
#include "GmPSCylindricalRPhiDoseDeposit.hh"
#include "GmPSCylindricalZPhiDoseDeposit.hh"
#include "GmG4PSEnergyDeposit.hh"
#include "GmG4PSEnergyLost.hh"
#include "GmG4PSMinKinEAtGeneration.hh"
#include "GmG4PSNofCollision.hh"
#include "GmG4PSNofSecondary.hh"
#include "GmG4PSNofStep.hh"
#include "GmG4PSPassageCellFlux.hh"
#include "GmG4PSPassageCellCurrent.hh"
#include "GmG4PSPassageTrackLength.hh"
#include "GmG4PSPopulation.hh"
#include "GmG4PSTrackCounter.hh"
#include "GmG4PSTrackLength.hh"
#include "GmG4PSTrackLengthFluence.hh"
#include "GmG4PSKerma.hh"

#include "GmG4PSData.hh"

#include "GmPSSurfaceFlux.hh"
#include "GmPSVolumeFlux.hh"


PLUGINSVC_FACTORY(GmG4PSCellCharge,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSCellFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_old,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_NoSkipEqualMate,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_SkipEqualMateSimple,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyLost,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSMinKinEAtGeneration,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofCollision,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofSecondary,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofStep,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageCellFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageCellCurrent,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageTrackLength,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPopulation,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTrackCounter,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTrackLength,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSKerma,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSData,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSurfaceFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSVolumeFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSphericalDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRZDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalZPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTrackLengthFluence, GmVPrimitiveScorer*(G4String))

#include "GmPSLET.hh"
#include "GmPSEdep_LET.hh"
#include "GmPSLETD.hh"
#include "GmPSLETPhi.hh"

#include "GmPSLETBydEdx.hh"
#include "GmPSdx_LETBydEdx.hh"
#include "GmPSEdep_LETBydEdx.hh"
#include "GmPSLETBydEdxD.hh"
#include "GmPSLETBydEdxPhi.hh"

#include "GmPSLETByESpect.hh"
#include "GmPSLET_LETByESpect.hh"
#include "GmPSLETByESpectD.hh"
#include "GmPSLETByESpectPhi.hh"

PLUGINSVC_FACTORY(GmPSLET,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEdep_LET,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETPhi,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETD,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSdx_LETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEdep_LETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETBydEdxD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETBydEdxPhi,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLETByESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_LETByESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETByESpectD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETByESpectPhi,GmVPrimitiveScorer*(G4String))



