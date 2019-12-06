#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosData_Distributions
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Distributions
	 HEADERS
		GmDistributionVerbosity.hh
		GmNumericDistributionLinLin.hh
		GmNumericDistributionUpper.hh
		GmNumericDistributionLogLin.hh
		GmNumericDistributionLower.hh
		GmNumericDistributionLogLog.hh
		GmPolynomialDistribution.hh
		GmDistributionRatio.hh
		GmNumericNDimDistributionUpper.hh
		GmGaussianDistribution.hh
		GmVNumericNDimDistribution.hh
		GmVStringDistribution.hh
		GmGeometricalBiasingDistribution.hh
		GmStringDistribution.hh
		GmNumericDistributionLinLog.hh
		GmVNumericDistribution.hh
	 SOURCES
		GmStringDistribution.cc
		GmVStringDistribution.cc
		GmDistributionRatio.cc
		GmNumericDistributionLogLin.cc
		GmNumericDistributionLogLog.cc
		GmNumericDistributionLinLog.cc
		GmNumericDistributionUpper.cc
		GmVNumericNDimDistribution.cc
		GmVNumericDistribution.cc
		GmGeometricalBiasingDistribution.cc
		GmNumericDistributionLower.cc
		GmPolynomialDistribution.cc
		GmNumericNDimDistributionUpper.cc
		GmGaussianDistribution.cc
		plugin.cc
		GmNumericDistributionLinLin.cc
		GmDistributionVerbosity.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosData_Management

	LINK_LIBRARIES
)
