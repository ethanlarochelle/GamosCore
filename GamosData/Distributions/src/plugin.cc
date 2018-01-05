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

#include "GmStringDistribution.hh"
#include "GmGeometricalBiasingDistribution.hh"
#include "GmNumericDistributionLinLin.hh"
#include "GmNumericDistributionLinLog.hh"
#include "GmNumericDistributionLogLin.hh"
#include "GmNumericDistributionLogLog.hh"
#include "GmNumericDistributionUpper.hh"
#include "GmNumericDistributionLower.hh"
#include "GmDistributionRatio.hh"
#include "GmGaussianDistribution.hh"

PLUGINSVC_FACTORY(GmStringDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGeometricalBiasingDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionUpper,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLower,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmDistributionRatio,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGaussianDistribution,GmVDistribution*(G4String))

#include "GmDistributionVerbosity.hh"
PLUGINSVC_FACTORY(GmDistributionVerbosity,GmVVerbosity*())
