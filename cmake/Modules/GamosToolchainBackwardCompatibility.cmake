# - Set up backward compatible Gamos GNU make toolchain
#
# The GNU make based buildsystem for Gamos provides a toolchain for 
# users building simple Gamos applications. The old style build and 
# install of Gamos provides a customized set of non-standard install 
# paths with use of the toolchain dependent on environment variables 
# pointing to the install paths.
#
# This script processes information on the CMake install paths, system 
# and compiler to determine the following variables for backward 
# compatibility:
#
#  GAMOS_SYSTEM      Old style system name, e.g. 'Linux', 'Darwin' 
#                     or 'WIN32'
#
#  GAMOS_COMPILER    Old system compiler id, e.g. 'g++', 'VC'. 
#
#  GmINSTALL          Location of 'config' subdirectory which contains
#                     all the GNU make toolchain fragments
#
#  GmINCLUDE          Old style path to location of Gamos headers
#
#  GmLIB              Old style library directory path. Rather than
#                     containing the actual libraries, it is expected to
#                     contain subdirectories named
#                     GAMOS_SYSTEM-GAMOS_COMPILER
#
# These variables are used in a CMake configuration file which is used 
# to generate shell scripts (C and Bourne flavour) the user can source 
# to set up their environment for use of the old toolchain. 
# These replace the old 'env.(c)sh' scripts to allow users to work with 
# the new CMake built libraries transparently if their application 
# relies on the old style toolchain.
#
# The scripts are generated for both the build and install trees so that
# developers wishing to write test applications do not have to install 
# their fresh build of Gamos.
#
# Compatibility with the library path style:
#
#  <prefix>/lib/GmSYSTEM-GmCOMPILER
#
# is provided by installing a directory 'gamos-<version>' in the 
# <prefix>/lib directory and creating a symbolic link inside here
# pointing up one directory level.
# This will not work on Windows however, and here users are recommended 
# to use Visual Studio directly, or to use CMake for application
# configuration.
#

#-----------------------------------------------------------------------
# - Functions and Macros to help configuration of shell scripts.
#-----------------------------------------------------------------------
# macro _gmtc_shell_setup(<shell>)
#       Set shell parameters such as program, family and common builtins 
#       for supplied shell (e.g. 'bourne' or 'cshell'
#
macro(_gmtc_shell_setup SHELL_FAMILY)
  if(${SHELL_FAMILY} STREQUAL "bourne")
    set(GAMOS_TC_SHELL_PROGRAM "/bin/sh")
    set(GAMOS_TC_SHELL_FAMILY "Bourne shell")
    set(GAMOS_TC_UNSET_COMMAND "unset")
    set(GAMOS_TC_SHELL_EXTENSION ".sh")
  elseif(${SHELL_FAMILY} STREQUAL "cshell")
    set(GAMOS_TC_SHELL_PROGRAM "/bin/csh")
    set(GAMOS_TC_SHELL_FAMILY "C shell")
    set(GAMOS_TC_UNSET_COMMAND "unsetenv")
    set(GAMOS_TC_SHELL_EXTENSION ".csh")
  else()
    message(FATAL_ERROR "Unsupported shell '${SHELL_FAMILY}'")
  endif()
endmacro()

#-----------------------------------------------------------------------
# function _gmtc_selflocate(<output> <shell> <script> <variable name>)
#          Set output to string containing shell commands needed to 
#          locate the directory in which script is located if the
#          script is sourced. This derived location is set as the
#          value of the shell variable name.
#
function(_gmtc_selflocate TEMPLATE_NAME SHELL_FAMILY SCRIPT_NAME LOCATION_VARIABLE)
  if(${SHELL_FAMILY} STREQUAL "bourne")
    set(${TEMPLATE_NAME} 
      "# Self locate script when sourced
if [ -z \"\$BASH_VERSION\" ]; then
  # Not bash, so rely on sourcing from correct location
  if [ ! -f ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} ]; then
    echo 'ERROR: ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} could NOT self-locate Gamos installation'
    echo 'This is most likely because you are using ksh, zsh or similar'
    echo 'To fix this issue, cd to the directory containing this script'
    echo 'and source it in that directory.'
    return 1
  fi
  ${LOCATION_VARIABLE}=\$\(pwd\)
else
  gmsls_sourced_dir=\$\(dirname \${BASH_ARGV[0]}\)
  ${LOCATION_VARIABLE}=$\(cd \$gmsls_sourced_dir > /dev/null ; pwd\)
fi
      "
      PARENT_SCOPE
      )
    # For bourne shell, set the values of the guard variables
    set(GAMOS_TC_IF_SELFLOCATED "" PARENT_SCOPE)
    set(GAMOS_TC_ENDIF_SELFLOCATED "" PARENT_SCOPE)

  elseif(${SHELL_FAMILY} STREQUAL "cshell")
    set(${TEMPLATE_NAME}
      "# Self locate script when sourced
# If sourced interactively, we can use $_ as this should be
#
#   source path_to_script_dir/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}
#
set ARGS=($_)
if (\"$ARGS\" != \"\") then
  set gmsls_sourced_dir=\"`dirname \${ARGS[2]}`\"
else
  # Oh great, we were sourced non-interactively. This means that $_
  # won't be set, so we need an external source of information on
  # where the script is located.
  # We obtain this in one of two ways:
  #   1) Current directory:
  #     cd script_dir ; source ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}
  #
  #   2) Supply the directory as an argument to the script:
  #     source script_dir/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} script_dir
  #
  if ( -e ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} ) then
    set gmsls_sourced_dir=\"`pwd`\"
  else if ( \"\$1\" != \"\" )  then
    if ( -e \${1}/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} ) then
      set gmsls_sourced_dir=\${1}
    else
      echo \"ERROR \${1} does not contain a Gamos installation\"
    endif
  else
    unset gmsls_sourced_dir
    echo \"ERROR: ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} could NOT self-locate Gamos installation\"
    echo \"because it was sourced (i.e. embedded) in another script.\"
    echo \"This is due to limitations of (t)csh but can be worked around by providing\"
    echo \"the directory where ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} is located\"
    echo \"to it, either via cd-ing to the directory before sourcing:\"
    echo \"  cd where_script_is ; source ${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}\"
    echo \"or by supplying the directory as an argument to the script:\"
    echo \"  source where_script_is/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION} where_script_is\"
    echo \" \"
  endif
endif

if (\$?gmsls_sourced_dir) then
  set ${LOCATION_VARIABLE}=\"`cd \${gmsls_sourced_dir} > /dev/null ; pwd`\"
endif
"
      PARENT_SCOPE
      )

    # For C-shell, set the values of the guard variables
    set(GAMOS_TC_IF_SELFLOCATED "if (\${?gmsls_sourced_dir}) then" PARENT_SCOPE)
   set(GAMOS_TC_ENDIF_SELFLOCATED "endif" PARENT_SCOPE)
  endif()
endfunction()

#-----------------------------------------------------------------------
# function _gmtc_setenv_command(<output> <shell> <name> <value>)
#          Set output to a string whose value is the shell command to 
#          set an environment variable with name and value
#
function(_gmtc_setenv_command TEMPLATE_NAME SHELL_FAMILY VARIABLE_NAME VARIABLE_VALUE)
  if(${SHELL_FAMILY} STREQUAL "bourne")
    set(${TEMPLATE_NAME} 
      "export ${VARIABLE_NAME}=${VARIABLE_VALUE}"
      PARENT_SCOPE
      )
  elseif(${SHELL_FAMILY} STREQUAL "cshell")
    set(${TEMPLATE_NAME} 
      "setenv ${VARIABLE_NAME} ${VARIABLE_VALUE}"
      PARENT_SCOPE
      )
  endif()
endfunction()

#-----------------------------------------------------------------------
# function _gmtc_setenv_ifnotset_command(<output> <shell> <name> <value>)
#          Set output to a string whose value is the shell command to 
#          set an environment variable with name and value if the
#          variable is not already set
#
function(_gmtc_setenv_ifnotset_command TEMPLATE_NAME SHELL_FAMILY VARIABLE_NAME VARIABLE_VALUE)
  # -- bourne
  if(${SHELL_FAMILY} STREQUAL "bourne")
    # Have to make this section verbatim to get correct formatting
    set(${TEMPLATE_NAME}
      "
if test \"x\$${VARIABLE_NAME}\" = \"x\" ; then
  export ${VARIABLE_NAME}=${VARIABLE_VALUE}
fi
"
      PARENT_SCOPE
      )
  # -- cshell
  elseif(${SHELL_FAMILY} STREQUAL "cshell")
    # Again, verbatim to get correct formatting...
    set(${TEMPLATE_NAME} 
      "
if ( ! \${?${VARIABLE_NAME}} ) then
  setenv ${VARIABLE_NAME} ${VARIABLE_VALUE}
endif
"
       PARENT_SCOPE
       )
  endif()
endfunction()

#-----------------------------------------------------------------------
# function _gmtc_prepend_path(<output> <shell> <name> <value>)
#          Set output to a string whose value is the shell command to
#          prepend supplied value to the path style environment variable
#          name (e.g. 'PATH')
#
function(_gmtc_prepend_path TEMPLATE_NAME SHELL_FAMILY PATH_VARIABLE
  APPEND_VARIABLE)
  # -- bourne block
  if(${SHELL_FAMILY} STREQUAL "bourne")
    # We have to make this section verbatim
    set(${TEMPLATE_NAME}
    "
if test \"x\$${PATH_VARIABLE}\" = \"x\" ; then
  export ${PATH_VARIABLE}=${APPEND_VARIABLE}
else
  export ${PATH_VARIABLE}=${APPEND_VARIABLE}:\${${PATH_VARIABLE}}
fi
"
    PARENT_SCOPE
    )
  # -- cshell block
  elseif(${SHELL_FAMILY} STREQUAL "cshell")
    # Again, this is verbatim so final output is formatted correctly
    set(${TEMPLATE_NAME}
      "
if ( ! \${?${PATH_VARIABLE}} ) then
  setenv ${PATH_VARIABLE} ${APPEND_VARIABLE}
else
  setenv ${PATH_VARIABLE} ${APPEND_VARIABLE}:\${${PATH_VARIABLE}}
endif
      "
      PARENT_SCOPE
      )
  endif()
endfunction()

#-----------------------------------------------------------------------
# MACRO(_gmtc_configure_tc_variables)
# Macro to perform the actual setting of the low level toolchain variables
# which need to be set in the final shell files.
# We do this in a separate macro so that we can wrap it in different ways for
# the install and build trees.
#
macro(_gmtc_configure_tc_variables SHELL_FAMILY SCRIPT_NAME)
  # - Set up the requested shell
  _gmtc_shell_setup(${SHELL_FAMILY})

  # - Locate self
  _gmtc_selflocate(GAMOS_TC_LOCATE_SELF_COMMAND ${SHELL_FAMILY} ${SCRIPT_NAME} gamosmake_root)


  # - Standard Setup and Paths
  _gmtc_setenv_command(GAMOS_TC_GmSYSTEM ${SHELL_FAMILY} GmSYSTEM ${GmSYSTEM})
  _gmtc_setenv_command(GAMOS_TC_GmINSTALL ${SHELL_FAMILY} GmINSTALL ${GmINSTALL})
  _gmtc_setenv_command(GAMOS_TC_GmINCLUDE ${SHELL_FAMILY} GmINCLUDE ${GmINCLUDE})
  _gmtc_setenv_command(GAMOS_TC_GmLIB ${SHELL_FAMILY} GmLIB ${GmLIB})

  if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    _gmtc_prepend_path(GAMOS_TC_GmLIB_PATH_SETUP ${SHELL_FAMILY} DYLD_LIBRARY_PATH ${GmLIB_DIR})
  else()
    _gmtc_prepend_path(GAMOS_TC_GmLIB_PATH_SETUP ${SHELL_FAMILY} LD_LIBRARY_PATH ${GmLIB_DIR})
  endif()

  _gmtc_setenv_ifnotset_command(GAMOS_TC_GmWORKDIR_SETUP ${SHELL_FAMILY} GmWORKDIR ${GmWORKDIR_DEFAULT})
  _gmtc_prepend_path(GAMOS_TC_GmWORKDIR_PATH_SETUP ${SHELL_FAMILY} PATH
  \${GmWORKDIR}/bin/\${GmSYSTEM})

  # - Gamos Library build setup
  # We prefer shared libs if these are built, otherwise fall back to static
  # On Win32, we also want DLLs?
  if(BUILD_SHARED_LIBS)
    _gmtc_setenv_command(GAMOS_TC_GmLIB_BUILD_SHARED ${SHELL_FAMILY} GmLIB_BUILD_SHARED 1)
    if(WIN32)
      _gmtc_setenv_command(GAMOS_TC_GmLIB_USE_DLL ${SHELL_FAMILY} GmLIB_USE_DLL 1)
    endif()
  else()
    _gmtc_setenv_command(GAMOS_TC_GmLIB_BUILD_STATIC ${SHELL_FAMILY} GmLIB_BUILD_STATIC 1)
  endif()


  # - CLHEP...
  if(GAMOS_USE_SYSTEM_CLHEP)
    # Have to use detected CLHEP paths to set base dir and others
    get_filename_component(_CLHEP_BASE_DIR ${CLHEP_INCLUDE_DIR} PATH)
    get_filename_component(_CLHEP_LIB_DIR ${CLHEP_LIBRARY} PATH)

    set(GAMOS_TC_GmLIB_USE_CLHEP "# USING SYSTEM CLHEP")
    _gmtc_setenv_command(GAMOS_TC_CLHEP_BASE_DIR ${SHELL_FAMILY} CLHEP_BASE_DIR ${_CLHEP_BASE_DIR})

    _gmtc_setenv_command(GAMOS_TC_CLHEP_INCLUDE_DIR ${SHELL_FAMILY} CLHEP_INCLUDE_DIR ${CLHEP_INCLUDE_DIR})

    _gmtc_setenv_command(GAMOS_TC_CLHEP_LIB_DIR ${SHELL_FAMILY} CLHEP_LIB_DIR ${_CLHEP_LIB_DIR})

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
      _gmtc_prepend_path(GAMOS_TC_CLHEP_LIB_PATH_SETUP ${SHELL_FAMILY} DYLD_LIBRARY_PATH \${CLHEP_LIB_DIR})
    else()
      _gmtc_prepend_path(GAMOS_TC_CLHEP_LIB_PATH_SETUP ${SHELL_FAMILY} LD_LIBRARY_PATH \${CLHEP_LIB_DIR})
    endif()

  else()
    # We have to configure things to point to the internal CLHEP...
    # Probably sufficient to do nothing...
    set(GAMOS_TC_GmLIB_USE_CLHEP "# USING INTERNAL CLHEP")
  endif()

  # - ZLIB...
  if(GAMOS_USE_SYSTEM_ZLIB)
    set(GAMOS_TC_GmLIB_USE_ZLIB "# USING SYSTEM ZLIB")
  else()
    _gmtc_setenv_command(GAMOS_TC_GmLIB_USE_ZLIB ${SHELL_FAMILY} GmLIB_USE_ZLIB 1)
  endif()

  # - GDML...
  if(GAMOS_USE_GDML)
    _gmtc_setenv_command(GAMOS_TC_GmLIB_USE_GDML ${SHELL_FAMILY} GmLIB_USE_GDML 1)
    # Backward compatibility requires XERCESCROOT to be set
    # As this is a 'rootdir' determine it from the XERCESC_INCLUDE_DIR
    # variable...
    get_filename_component(_xercesc_root ${XERCESC_INCLUDE_DIR} PATH)
    _gmtc_setenv_command(GAMOS_TC_GDML_PATH_SETUP ${SHELL_FAMILY} XERCESCROOT ${_xercesc_root})
  else()
    set(GAMOS_TC_GmLIB_USE_GDML "# NOT BUILT WITH GDML SUPPORT")
  endif()

  # - G3TOG4...
  if(GAMOS_USE_G3TOG4)
    _gmtc_setenv_command(GAMOS_TC_GmLIB_USE_G3TOG4 ${SHELL_FAMILY} GmLIB_USE_G3TOG4 1)
  else()
    set(GAMOS_TC_GmLIB_USE_G3TOG4 "# NOT BUILT WITH G3TOG4 SUPPORT")
  endif()

  # - USER INTERFACE AND VISUALIZATION MODULES...
  # - Terminals
  if(NOT WIN32)
    _gmtc_setenv_command(GEANT4_TC_G4UI_USE_TCSH ${SHELL_FAMILY} G4UI_USE_TCSH 1)
    set(GEANT4_TC_G4UI_USE_WIN32 "# WIN32 TERMINAL UI NOT AVAILABLE ON ${CMAKE_SYSTEM_NAME}")
  else()
    set(GEANT4_TC_G4UI_USE_TCSH "# TCSH TERMINAL UI NOT AVAILABLE ON ${CMAKE_SYSTEM_NAME}")
    _gmtc_setenv_command(GEANT4_TC_G4UI_USE_WIN32 ${SHELL_FAMILY} G4UI_USE_WIN32 1)
  endif()

  # - Qt UI AND VIS
  if(GAMOS_USE_QT)
    _gmtc_setenv_command(GEANT4_TC_G4UI_USE_QT ${SHELL_FAMILY} G4UI_USE_QT 1)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENGLQT ${SHELL_FAMILY} G4VIS_USE_OPENGLQT 1)

    # Might need library setup, but for now recommend system install....
  else()
    set(GEANT4_TC_G4UI_USE_QT "# NOT BUILT WITH QT INTERFACE")
  endif()

  # - XM UI AND VIS
  if(GAMOS_USE_XM)
    _gmtc_setenv_command(GEANT4_TC_G4UI_USE_XM ${SHELL_FAMILY} G4UI_USE_XM 1)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENGLXM ${SHELL_FAMILY} G4VIS_USE_OPENGLXM 1)

    # Might need library setup, but for now recommend system install....
  else()
    set(GEANT4_TC_G4UI_USE_XM "# NOT BUILT WITH XM INTERFACE")
  endif()

  # - Network DAWN
  if(GAMOS_USE_NETWORKDAWN)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_DAWN ${SHELL_FAMILY} G4VIS_USE_DAWN 1)
  else()
    set(GEANT4_TC_G4VIS_USE_DAWN "# NOT BUILT WITH NETWORK DAWN SUPPORT")
  endif()

  # - Network VRML
  if(GAMOS_USE_NETWORKVRML)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_VRML ${SHELL_FAMILY} G4VIS_USE_VRML 1)
  else()
    set(GEANT4_TC_G4VIS_USE_VRML "# NOT BUILT WITH NETWORK VRML SUPPORT")
  endif()

  # - OpenInventor
  if(GAMOS_USE_INVENTOR)
    if(UNIX)
      _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENINVENTOR ${SHELL_FAMILY} G4VIS_USE_OIX 1)
    else()
      _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENINVENTOR ${SHELL_FAMILY} G4VIS_USE_OIWIN32 1)
    endif()
  else()
    set(GEANT4_TC_G4VIS_USE_OPENINVENTOR "# NOT BUILT WITH INVENTOR SUPPORT")
  endif()


  # - X11 OpenGL
  if(GAMOS_USE_OPENGL_X11)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENGLX ${SHELL_FAMILY} G4VIS_USE_OPENGLX 1)
  else()
    set(GEANT4_TC_G4VIS_USE_OPENGLX "# NOT BUILT WITH OPENGL(X11) SUPPORT")
  endif()

  # - WIN32 OpenGL
  if(GAMOS_USE_OPENGL_WIN32)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_OPENWIN32 ${SHELL_FAMILY} G4VIS_USE_OPENWIN32 1)
  else()
    set(GEANT4_TC_G4VIS_USE_OPENGLWIN32 "# NOT BUILT WITH OPENGL(WIN32) SUPPORT")
  endif()

  # - X11 RayTracer
  if(GAMOS_USE_RAYTRACER_X11)
    _gmtc_setenv_command(GEANT4_TC_G4VIS_USE_RAYTRACERX ${SHELL_FAMILY} G4VIS_USE_RAYTRACERX 1)
  else()
    set(GEANT4_TC_G4VIS_USE_RAYTRACERX "# NOT BUILT WITH RAYTRACER(X11) SUPPORT")
  endif()
endmacro()


#-----------------------------------------------------------------------
# macro _gmtc_configure_build_tree_scripts()
#       Macro to configure toolchain compatibility scripts for the 
#       build tree
#
macro(_gmtc_configure_build_tree_scripts SCRIPT_NAME)
  # Need to process for bourne and cshell families
  foreach(_shell bourne;cshell)
    # Generate the variables
    _gmtc_configure_tc_variables(${_shell} ${SCRIPT_NAME})

    # Configure the file - goes straight into the binary dir
    configure_file(
      ${CMAKE_SOURCE_DIR}/cmake/Templates/gamosmake-skeleton.in
      ${PROJECT_BINARY_DIR}/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}
      @ONLY
      )
  endforeach()
endmacro()


#-----------------------------------------------------------------------
# macro _gmtc_configure_install_tree_script()
#       Macro to configure toolchain compatibility scripts for the 
#       install tree
#
macro(_gmtc_configure_install_tree_scripts CONFIGURE_DESTINATION SCRIPT_NAME INSTALL_DESTINATION)
  # Need to process for bourne and cshell families
  foreach(_shell bourne;cshell)
    # Generate the variables
    _gmtc_configure_tc_variables(${_shell} ${SCRIPT_NAME})

    # Configure the file
    configure_file(
      ${CMAKE_SOURCE_DIR}/cmake/Templates/gamosmake-skeleton.in
      ${CONFIGURE_DESTINATION}/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}
      @ONLY
      )

    # Install it to the required location
    install(FILES
      ${CONFIGURE_DESTINATION}/${SCRIPT_NAME}${GAMOS_TC_SHELL_EXTENSION}
      DESTINATION ${INSTALL_DESTINATION}
      PERMISSIONS 
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
      COMPONENT Development
      )
  endforeach()
endmacro()


#-----------------------------------------------------------------------
# Implementation section
#-----------------------------------------------------------------------
# Configure shell scripts for BUILD TREE
# This means we have to point to libraries in the build tree, but 
# includes and resource files will be in the source tree
# This script never needs to be relocatable, so we don't need to use the
# self location functionality.
# N.B. IT WILL NOT WORK when building with VS/Xcode or any multiconfig
# buildtool because we cannot reconcile the output paths these use with 
# those expected by the old toolchain...
#
set(GmSYSTEM  "${GAMOS_SYSTEM}-${GAMOS_COMPILER}")
set(GmINSTALL ${PROJECT_SOURCE_DIR})
set(GmINCLUDE ${PROJECT_SOURCE_DIR}/this_is_a_deliberate_dummy_path)
set(GmLIB ${PROJECT_BINARY_DIR}/outputs/library)
set(GmLIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(GmWORKDIR_DEFAULT "\$HOME/gamos_workdir")

# - Configure the shell scripts for the BUILD TREE
_gmtc_configure_build_tree_scripts(gamosmake)


#-----------------------------------------------------------------------
# Configure shell scripts for INSTALL TREE
# This means we have to point things to their final location when 
# installed. These paths are all determined by the CMAKE_INSTALL_FULL 
# directories and others.
# If we are relocatable, then the structure we will have is
# +- CMAKE_INSTALL_PREFIX
#    +- LIBDIR/Gamos-VERSION (GmLIB)
#    +- INCLUDEDIR/Gamos     (GmINCLUDE)
#    +- DATAROOTDIR/Gamos-VERSION/
#       +- gamosmake              (GmINSTALL!)
#          +- gamosmake.(c)sh
#          +- config/

# - Construct universal backward compatible INSTALL TREE PATHS.
set(GmSYSTEM  "${GAMOS_SYSTEM}-${GAMOS_COMPILER}")
set(GmINSTALL "\"\$gamosmake_root\"")

# - Now need relative paths between 'GmINSTALL' and include/lib dirs
# - Include dir
file(RELATIVE_PATH
  GmMAKE_TO_INCLUDEDIR
  ${CMAKE_INSTALL_FULL_DATAROOTDIR}/Gamos-${Gamos_VERSION}/gamosmake
  ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${PROJECT_NAME}
  )
set(GmINCLUDE "\"`cd \$gamosmake_root/${GmMAKE_TO_INCLUDEDIR} > /dev/null \; pwd`\"")

# - Lib dir
file(RELATIVE_PATH
  GmMAKE_TO_LIBDIR
  ${CMAKE_INSTALL_FULL_DATAROOTDIR}/Gamos-${Gamos_VERSION}/gamosmake
  ${CMAKE_INSTALL_FULL_LIBDIR}
  )
set(GmLIB "\"`cd \$gamosmake_root/${GmMAKE_TO_LIBDIR}/Gamos-${Gamos_VERSION} > /dev/null \; pwd`\"")
set(GmLIB_DIR "\"`cd \$gamosmake_root/${GmMAKE_TO_LIBDIR} > /dev/null \; pwd`\"")

set(GmWORKDIR_DEFAULT "\$HOME/gamos_workdir")

# - Configure the shell scripts for the INSTALL TREE
_gmtc_configure_install_tree_scripts(
    ${CMAKE_BINARY_DIR}/InstallTreeFiles
    gamosmake
    ${CMAKE_INSTALL_DATAROOTDIR}/Gamos-${Gamos_VERSION}/gamosmake
    )


# - For install tree, we also need to install the config directory 
#   which contains all the old toolchain scripts, and to create a 
#   softlink to the GmSYSTEM directory.
#
install(DIRECTORY config
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Gamos-${Gamos_VERSION}/gamosmake
    COMPONENT Development
    FILES_MATCHING PATTERN "*.gmk"
    PATTERN "CVS" EXCLUDE
    PATTERN ".svn" EXCLUDE
    PATTERN "scripts/" EXCLUDE
)

# Compatibility softlink to library directory, we do this on all
# platforms, but it does nothing on Windows (well, at least the
# attempted symlink creation does not)
# Take care to quote the path names to avoid issues with spaces
install(CODE "execute_process(COMMAND \${CMAKE_COMMAND} -E make_directory \"\$ENV{DESTDIR}${CMAKE_INSTALL_FULL_LIBDIR}/Gamos-${Gamos_VERSION}\")")

install(CODE "execute_process(COMMAND \${CMAKE_COMMAND} -E create_symlink .. ${GAMOS_SYSTEM}-${GAMOS_COMPILER} WORKING_DIRECTORY \"\$ENV{DESTDIR}${CMAKE_INSTALL_FULL_LIBDIR}/Gamos-${Gamos_VERSION}\")")


#-----------------------------------------------------------------------
# TEMPORARY
# Configure environment setup script for install of Gamos
# Temporarily here to keep all shell setup in one place.
# Later, should be refactored into its own module, with module containing
# all the shell tools above.
#
# - Script base name (without extension
set(_scriptbasename gamos)

# - Relative path between bindir (where script is) and library directory
file(RELATIVE_PATH
  GmENV_BINDIR_TO_LIBDIR
  ${CMAKE_INSTALL_FULL_BINDIR}
  ${CMAKE_INSTALL_FULL_LIBDIR}
  )


# - Configure for each shell
foreach(_shell bourne;cshell)
  # Setup the shell
  _gmtc_shell_setup(${_shell})

  # Set script full name
  set(_scriptfullname ${_scriptbasename}${GAMOS_TC_SHELL_EXTENSION})

  # Set locate self command
  _gmtc_selflocate(GAMOS_ENV_SELFLOCATE_COMMAND 
    ${_shell} 
    ${_scriptbasename} 
    gamos_envbindir
    )

  # Set path, which should be where the script itself is installed
  _gmtc_prepend_path(GAMOS_ENV_BINPATH_SETUP 
    ${_shell}
    PATH
    "\"\$gamos_envbindir\""
    )

  # Set library path, based on relative paths between bindir and libdir
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(_libpathname DYLD_LIBRARY_PATH)
  else()
    set(_libpathname LD_LIBRARY_PATH)
  endif()

  _gmtc_prepend_path(GAMOS_ENV_LIBPATH_SETUP
    ${_shell}
    ${_libpathname}
    "\"`cd $gamos_envbindir/${GmENV_BINDIR_TO_LIBDIR} > /dev/null ; pwd`\""
    )

  # Configure the file
  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/Templates/gamos-env-skeleton.in
    ${PROJECT_BINARY_DIR}/InstallTreeFiles/${_scriptfullname}
    @ONLY
    )

  # Install it to the required location
  install(FILES
    ${PROJECT_BINARY_DIR}/InstallTreeFiles/${_scriptfullname}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
    COMPONENT Runtime
    )
endforeach()

