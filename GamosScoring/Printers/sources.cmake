#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosScoring_Printers
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Printers
	 HEADERS
		GmPSPrinterTextFile.hh
		GmPSPrinterXYZ.hh
		GmPSPrinterCout.hh
		GmPSPrinter3ddose.hh
		GmPSPrinter3ddoseSplitZ.hh
		GmPSPrinterBinFile.hh
		GmPSPrinterCoutContIndex.hh
		GmPSPrinterHistos.hh
		GmPSPrinterCSVFile.hh
		GmPSPrinterSqdose.hh
	 SOURCES
		GmPSPrinterCSVFile.cc
		GmPSPrinterSqdose.cc
		GmPSPrinterHistos.cc
		GmPSPrinter3ddose.cc
		GmPSPrinterCout.cc
		GmPSPrinterXYZ.cc
		GmPSPrinter3ddoseSplitZ.cc
		GmPSPrinterBinFile.cc
		GmPSPrinterTextFile.cc
		GmPSPrinterCoutContIndex.cc
		plugin.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosBase_Classifiers
	GamosCore_GamosGeometry
	GamosCore_GamosReadDICOM
	GamosCore_GamosScoring_Management

	LINK_LIBRARIES
)
