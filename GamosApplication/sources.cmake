#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosApplication
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosApplication
	 HEADERS
	 SOURCES
		dummy.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	
	LINK_LIBRARIES
		${Gamos_LIBRARIES} 
)
