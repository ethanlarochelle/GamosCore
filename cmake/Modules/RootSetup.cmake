# - Define standard directories for GEANT4

# Installation directories
#
set(ROOTSYS "/home/arce/gamos/external/root/5.34.09/root" CACHE PATH "")

# List external includes needed.
include_directories(${ROOTSYS}/include)

#-----------------------------------------------------------------------------

set(Root_LIBRARIES
  -L${ROOTSYS}/lib
   -lHist 
   -lGpad 
   -lGraf 
   -lMatrix 
   -lPhysics 
   -lPostscript
   -lCint 
   -lCore
   -lTree 
   -lNet 
   -lRIO 
   -lMathCore 
   -lThread
 )

