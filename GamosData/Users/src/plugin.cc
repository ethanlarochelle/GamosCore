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

//----- DATA USERS
#include "GmStepDataHistosUA.hh"
#include "GmTrackDataHistosUA.hh"
#include "GmSecondaryTrackDataHistosUA.hh"
#include "GmEventDataHistosUA.hh"
#include "GmRunDataHistosUA.hh"
#include "GmStackDataHistosUA.hh"
#include "GmStepDataTextFileUA.hh"
#include "GmTrackDataTextFileUA.hh"
#include "GmSecondaryTrackDataTextFileUA.hh"
#include "GmEventDataTextFileUA.hh"
#include "GmRunDataTextFileUA.hh"
#include "GmStackDataTextFileUA.hh"
#include "GmStepDataBinFileUA.hh"
#include "GmTrackDataBinFileUA.hh"
#include "GmSecondaryTrackDataBinFileUA.hh"
#include "GmEventDataBinFileUA.hh"
#include "GmRunDataBinFileUA.hh"
#include "GmStackDataBinFileUA.hh"
#include "GmStepDataCoutUA.hh"
#include "GmTrackDataCoutUA.hh"
#include "GmSecondaryTrackDataCoutUA.hh"
#include "GmEventDataCoutUA.hh"
#include "GmRunDataCoutUA.hh"
#include "GmStackDataCoutUA.hh"

PLUGINSVC_FACTORY(GmStepDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataCoutUA,GmUserAction*())

#ifndef GAMOS_NO_ROOT
#include "GmDataTTreeUA.hh"
PLUGINSVC_FACTORY(GmDataTTreeUA,GmUserAction*())
#endif

#include "GmNumericDataFilter.hh"
#include "GmStringDataFilter.hh"

PLUGINSVC_FACTORY(GmNumericDataFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStringDataFilter,GmVFilter*(G4String))

#include "GmClassifierByNumericData.hh"
#include "GmClassifierByStringData.hh"
#include "GmClassifierByDistribution.hh"

PLUGINSVC_FACTORY(GmClassifierByNumericData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByStringData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByDistribution,GmVClassifier*(G4String))
