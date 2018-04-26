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

#include "GmCountTracksUA.hh"
#include "GmTrackingVerboseUA.hh"
#include "GmCountProcessesUA.hh"
#include "GmCountTracksAndStepsUA.hh"
#include "GmGenerHistosUA.hh"
#include "GmKillAllUA.hh"
#include "GmKillAtStackingActionUA.hh"
#include "GmKillAtSteppingActionUA.hh"
#include "GmKillAtTrackingActionUA.hh"
//#include "GmPrintEMPhysicsParamsUA.hh"
#include "GmMaterialBudgetUA.hh"
#include "GmTimeStudyUA.hh"
#include "GmTimeStudyEventUA.hh"
#include "GmShowerShapeUA.hh"
#include "GmGamosVerboseByEventUA.hh"
#include "GmChangeWeightUA.hh"
#include "GmCopyWeightToSecondaryUA.hh"
#include "GmCopyWeightToRDSecondaryUA.hh"
#include "GmStopRunAfterTimeUA.hh"
#include "GmSaveHistosAfterNEvents.hh"
#include "GmPrintEMParamsUA.hh"

PLUGINSVC_FACTORY(GmCountTracksUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackingVerboseUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountProcessesUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountTracksAndStepsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGenerHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAllUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtStackingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtSteppingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtTrackingActionUA,GmUserAction*())
//PLUGINSVC_FACTORY(GmPrintEMPhysicsParamsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmMaterialBudgetUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTimeStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTimeStudyEventUA,GmUserAction*())
PLUGINSVC_FACTORY(GmShowerShapeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGamosVerboseByEventUA,GmUserAction*())
PLUGINSVC_FACTORY(GmChangeWeightUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToRDSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStopRunAfterTimeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSaveHistosAfterNEvents,GmUserAction*())
PLUGINSVC_FACTORY(GmPrintEMParamsUA,GmUserAction*())

#include "GmUtilsUAVerbosity.hh"
PLUGINSVC_FACTORY(GmUtilsUAVerbosity,GmVVerbosity*())

