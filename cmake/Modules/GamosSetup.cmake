# - Define standard directories for GEANT4

# Installation directories
#
######################## GAMOS INCLUDES
set(GAMOSINSTALL "/home/arce/gamos/GAMOS.5.0.0" CACHE PATH "")

include_directories(${GAMOSINSTALL}/source)

#-----------------------------------------------------------------------------


set(Gamos_LIBRARIES
# -L${GAMOSINSTALL}/build/lib64/ 
 -L${GAMOSINSTALL}/build/outputs/library/Linux-g++/ 
  -lGamosCore_GamosUtilsUA
  -lGamosCore_GamosSD 
  -lGamosCore_GamosGenerator 
  -lGamosCore_GamosMovement 
  -lGamosCore_GamosPhysics_Cuts 
  -lGamosCore_GamosPhysics_VarianceReduction 
  -lGamosCore_GamosPhysics_PhysicsList 
  -lGamosCore_GamosPhysics_OtherPhysicsLists 
  -lGamosCore_GamosScoring_Management
  -lGamosCore_GamosScoring_Scorers 
  -lGamosCore_GamosScoring_Printers 
  -lGamosCore_GamosScoring_PointDetector 
  -lGamosCore_GamosReadDICOM 
  -lGamosCore_GamosGeometry  
  -lGamosCore_GamosAnalysis 
  -lGamosCore_GamosData_Management 
  -lGamosCore_GamosData_Data 
  -lGamosCore_GamosData_Users 
  -lGamosCore_GamosUserActionMgr 
  -lGamosCore_GamosBase_Filters 
  -lGamosCore_GamosBase_Classifiers 
  -lGamosCore_GamosBase_Base 
  -lGamosCore_GamosUtils
  -lMagFieldManager
)
