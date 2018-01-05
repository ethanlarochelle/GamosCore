# - Basic setup for testing Gamos using CMake/CTest
#

#-----------------------------------------------------------------------
# Configure CTest and relevant Gamos settings, if required
#
if(GAMOS_ENABLE_TESTING)
  # - Core CTest
  enable_testing()
  include(CTest)

  # - Gamos_DIR is needed to locate GeantConfig.cmake file required 
  # by tests and examples
  set(Gamos_DIR ${CMAKE_BINARY_DIR} CACHE PATH "Current build directory")

  #-----------------------------------------------------------------------
  # OLD STYLE DATA CONFIGURATION
  # - Configure data location, as most tests will require access to these
  #if(GAMOS_INSTALL_DATA)
  #  set(GAMOS_DATA_DIR ${CMAKE_BINARY_DIR}/data CACHE PATH "Directory where the Gamos data is located")
  #elseif(NOT "$ENV{GAMOS_DATA_DIR}" STREQUAL "")
  #  set(GAMOS_DATA_DIR  "$ENV{GAMOS_DATA_DIR}" CACHE PATH "Directory where the Gamos data is located" FORCE)
  #endif()
  
  #if(NOT GAMOS_DATA_DIR)
  #  message(STATUS  "  GAMOS_DATA_DIR not defined! This may cause many Gamos tests to fail\n"
  #                 "     Add -DGAMOS_DATA_DIR=<path> to the cmake command or define environment")
                  #  return()
                  #endif()

  # - Configure test environment (basically, the data library pointers)
  #foreach( tuple "GmNEUTRONHPDATA;GmNDL"
  #               "GmLEDATA;GmEMLOW"
  #               "GmLEVELGAMMADATA;PhotonEvaporation"
  #               "GmRADIOACTIVEDATA;RadioactiveDecay"
  #               "GmNEUTRONXSDATA;GmNEUTRONXS"
  #               "GmPIIDATA;GmPII"
  #               "GmREALSURFACEDATA;RealSurface"
  #               "GmSAIDXSDATA;GmSAIDDATA"                 
  #               )
  #  list(GET tuple 0 envname)
  #  list(GET tuple 1 dirname)
  #  GAMOS_LATEST_VERSION(${GAMOS_DATA_DIR} ${dirname} _result)
  #  #list(APPEND GAMOS_TEST_ENVIRONMENT ${envname}=${_result})
  #endforeach()

  #-----------------------------------------------------------------------
  # - Configure data using new style API methods
  gamos_get_datasetnames(_dslist)
  foreach(_ds ${_dslist})
    gamos_get_dataset_property(${_ds} ENVVAR _dsenvvar)
    gamos_get_dataset_property(${_ds} BUILD_DIR _dspath)
    list(APPEND GAMOS_TEST_ENVIRONMENT ${_dsenvvar}=${_dspath})
  endforeach()
endif()

#-----------------------------------------------------------------------
# Add Unit Tests if required
#
if(GAMOS_BUILD_TESTS)
  file(GLOB_RECURSE files RELATIVE ${CMAKE_SOURCE_DIR} source/CMakeLists.txt)
  foreach( file ${files} )
    get_filename_component(path ${file} PATH)
    if(path MATCHES "/test$")
      add_subdirectory(${path})
    endif()
  endforeach()
endif()
