# - Find StatTest
# This module tries to find the StatTest application
# Once done this will define
# 
#	STATTEST_FOUND    - Application found
#	STATTEST_APP      - Application
# STATTEST_CMD      - Command line to run the application
# STATTEST_ADD_TEST - Helper function to define a ctest using
#      			          StatTest (Only if we are using for Gm testing)
#
# Variables used by this module, which can change the default behaviour 
# and need to be set before calling find_package
#
#	STATTEST_ROOT_DIR	Root directory to StatTest package
#

#Search application
#Note that the second suggested path is Gm specific... 
find_path(STATTEST_APP_DIR
		NAMES StatTestVersion.py
		PATHS ${STATTEST_ROOT_DIR} ${CMAKE_SOURCE_DIR}/verification/StatTest
		NO_DEFAULT_PATH
    )

#If we didn't find it there, fall back to some standard search paths
find_path(STATTEST_APP_DIR NAMES StatTest/StatTestVersion.py)

####
#### Run-time dependencies...
####
# - Check if ROOT is available
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
  MESSAGE(STATUS "StatTest: ROOT package not found --> StatTest package disabled")
  set(_root_isok FALSE)
else()
  set(_root_isok TRUE)
endif()

# - Check if python interpreter is correct version 
# (the one compatible with root)
find_package(PythonInterp ${ROOT_PYTHONVER} QUIET)
if(NOT PYTHONINTERP_FOUND)
  MESSAGE(STATUS "StatTest: Python interpreter of version ${ROOT_PYTHONVER} not found --> StatTest package disabled")
  set(_python_isok FALSE)
else()
	set(_python_isok TRUE)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  StatTest
  DEFAULT_MSG
  STATTEST_APP_DIR
  _root_isok
  _python_isok
  )

if(STATTEST_FOUND)
  set(STATTEST_APP ${STATTEST_APP_DIR}/runtests.py)
  set(STATTEST_CMD ${PYTHON_EXECUTABLE} ${STATTEST_APP})

  if(GAMOS_ENABLE_TESTING)
    # Let's create a function that helps in building tests for 
    # regression testing
    # function STATTEST_ADD_TEST(<name> 
    #	   		                     GmTEST testname
    #                            CONFIG conffile
    #                            INPUT inputfile
    #			                       [DEBUG]
    #                            [REFERENCE reference]
    #                            [LABELS label1 label2 ...]
    #                            [IMG filename])
    function(STATTEST_ADD_TEST stattest)
      CMAKE_PARSE_ARGUMENTS(ARG "DEBUG" 
        "CONFIG;INPUT;REFERENCE;GmTEST;IMG"
        "LABELS" ${ARGN}
        )

      #check mandatory arguments
      list(LENGTH ARG_CONFIG _len)
      if(_len LESS 1)
        message(FATAL_ERROR "STATTEST_ADD_TEST: conffile is mandatory")
      endif()
      
      list(LENGTH ARG_INPUT _len)
      if(_len LESS 1)
        message(FATAL_ERROR "STATTEST_ADD_TEST: inputfile is mandatory")
      endif()
      
      list(LENGTH ARG_GmTEST _len)
      if(_len LESS 1)
        message(FATAL_ERROR "STATTEST_ADD_TEST: testname is mandatory")
      endif()

      #Set basic command line
      if(ARG_IMG)
        set(_command ${STATTEST_CMD} -g ${ARG_IMG})
      else()
        set(_command ${STATTEST_CMD})
      endif() 

      #Mandatory parameters
      set(_command ${_command} ${ARG_CONFIG} ${ARG_INPUT})
      if(ARG_REFERENCE)
        set(_command ${_command} ${ARG_REFERENCE})
      endif()

      if(ARG_LABELS)
        set(_labels ${ARG_LABELS})
      else()
        set(_labels "")
      endif()

      include(GamosCTest)
    
      #Now build Gm test
      GAMOS_ADD_TEST(${stattest} 
        COMMAND ${_command}
        DEPENDS ${ARG_GmTEST}
        LABELS ${_labels}
        )
    endfunction()
  endif(GAMOS_ENABLE_TESTING)
endif(STATTEST_FOUND)

mark_as_advanced(STATTEST_APP_DIR)
