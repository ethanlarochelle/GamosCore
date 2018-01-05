#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosData_Management
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Management

   HEADERS
       GmVDataNon1DHisto.hh
       GmDataAncestorStepNumericData.hh
       GmVDataString.hh
       GmVDataProf2DHisto.hh
       GmVDataAccumulated.hh
       GmVDataProf1DHisto.hh
       GmDataVerbosity.hh
       GmVDataInt.hh
       GmVData2DHisto.hh
       GmDataMgr.hh
       GmDataAncestorStepStringData.hh
       GmVData.hh

   SOURCES
       plugin.cc
       GmDataMgr.cc
       GmDataVerbosity.cc
       GmVDataNon1DHisto.cc
       GmVDataString.cc
       GmVDataAccumulated.cc
       GmVDataProf2DHisto.cc
       GmDataAncestorStepStringData.cc
       GmVData.cc
       GmVData2DHisto.cc
       GmVDataProf1DHisto.cc
       GmDataAncestorStepNumericData.cc
       GmVDataInt.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr

   LINK_LIBRARIES
)

# List any source specific properties here
