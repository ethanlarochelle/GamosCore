# - Define useful macros for building and installing Gamos library targets
#
# This file defines the following macros for Gamos developers needing to
# add shared and static library targets.
#
# GAMOS_LIBRARY_TARGET        - define standard Gamos library targets
#
# The macro will take the name of the library and its sources, defining
# static and shared targets depending on the value of BUILD_SHARED_LIBS and
# BUILD_STATIC_LIBS. Install targets are also created.

if(__GAMOSMACROLIBRARYTARGETS_ISLOADED)
  return()
endif()
set(__GAMOSMACROLIBRARYTARGETS_ISLOADED TRUE)

include(CMakeMacroParseArguments)

#----------------------------------------------------------------------------
# - GAMOS_LIBRARY_TARGET
# General build and install of a Gamos library target
#
MACRO(GAMOS_LIBRARY_TARGET)
   CMAKE_PARSE_ARGUMENTS(GmLIBTARGET
        ""
        "NAME" "SOURCES;GAMOS_LINK_LIBRARIES;LINK_LIBRARIES"
        ${ARGN}
    )

    if(BUILD_SHARED_LIBS)
        # Add the shared library target and link its dependencies
        # WIN32 first
        if(WIN32)
            # We have to generate the def export file from an archive library.
            # If we're building Static libraries already, use that existing
            # target, otherwise, build a temporary uninstalled archive...
            if(BUILD_STATIC_LIBS)
                set(_archive ${GmLIBTARGET_NAME}-static)
            else()
                add_library(_${GmLIBTARGET_NAME}-archive STATIC EXCLUDE_FROM_ALL ${GmLIBTARGET_SOURCES})
                set(_archive _${GmLIBTARGET_NAME}-archive)
            endif()

            # - Create the .def file for this library
            # Note that we have to pass the actual full path to the library
            # to the command. CMake unfortunately won't generate this for us.
            # Note also that because we're likely to be on a platform with
            # multiconfig build tools. we use the CFG_INTDIR to locate the
            # archive we need...
            add_custom_command(OUTPUT _${GmLIBTARGET_NAME}.def
                COMMAND genwindef -o _${GmLIBTARGET_NAME}.def -l ${GmLIBTARGET_NAME} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/${_archive}.lib
                DEPENDS ${_archive} genwindef)

            # - Now we can build the DLL
            # We create it from a dummy empty C++ file plus the def file.
            file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/_${GmLIBTARGET_NAME}.cpp
                "// empty _${GmLIBTARGET_NAME}.cpp\n")

            add_library(${GmLIBTARGET_NAME} SHARED _${GmLIBTARGET_NAME}.cpp
                _${GmLIBTARGET_NAME}.def)

            # - Link the DLL.
            # We link it to the archive, and the supplied libraries,
            # but then remove the archive from the LINK_INTERFACE.
            target_link_libraries(${GmLIBTARGET_NAME}
                ${_archive}
                ${GmLIBTARGET_GAMOS_LINK_LIBRARIES} 
                ${GmLIBTARGET_LINK_LIBRARIES})

            set_target_properties(${GmLIBTARGET_NAME}
                PROPERTIES LINK_INTERFACE_LIBRARIES "${GmLIBTARGET_GAMOS_LINK_LIBRARIES};${GmLIBTARGET_LINK_LIBRARIES}")

        else()
            # - We build a Shared library in the usual fashion...
            add_library(${GmLIBTARGET_NAME} SHARED ${GmLIBTARGET_SOURCES})
            target_link_libraries(${GmLIBTARGET_NAME}
                ${GmLIBTARGET_GAMOS_LINK_LIBRARIES} 
                ${GmLIBTARGET_LINK_LIBRARIES})
        endif()

        # This property is set to prevent concurrent builds of static and shared
        # libs removing each others files.
        set_target_properties(${GmLIBTARGET_NAME} 
            PROPERTIES CLEAN_DIRECT_OUTPUT 1)

        # Set the INSTALL_NAME_DIR of the library to its final installation
        # location (Only affects Mac OS X). This will only affect the library
        # when installed, BUT it does hard code this in. One should still be
        # able to bundle up the libraries later as CMake should build the
        # library with headerpad_max_install_names
        set_target_properties(${GmLIBTARGET_NAME}
            PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR})

        # Install the library - note the use of RUNTIME, LIBRARY and ARCHIVE
        # this helps with later DLL builds.
        # Export to standard depends file for later install
        # NEEDS WORK TO REMOVE HARDCODED LIB/BIN DIR
        install(TARGETS ${GmLIBTARGET_NAME}
            EXPORT GamosLibraryDepends
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)

        # Append the library target to a global property so that build tree
        # export of library dependencies can pick up all targets
        set_property(GLOBAL APPEND
            PROPERTY GAMOS_EXPORTED_TARGETS ${GmLIBTARGET_NAME})
    endif()

    #
    # As above, but for static rather than shared library
    if(BUILD_STATIC_LIBS)
        # We have to distinguish the static from shared lib, so use -static in
        # name. Link its dependencies, and ensure we actually link to the
        # -static targets (We should strictly do this for the external
        # libraries as well if we want a pure static build).
        add_library(${GmLIBTARGET_NAME}-static STATIC ${GmLIBTARGET_SOURCES})

        set(GmLIBTARGET_GAMOS_LINK_LIBRARIES_STATIC )
        foreach(_tgt ${GmLIBTARGET_GAMOS_LINK_LIBRARIES})
            list(APPEND GmLIBTARGET_GAMOS_LINK_LIBRARIES_STATIC ${_tgt}-static)
        endforeach()

        target_link_libraries(${GmLIBTARGET_NAME}-static 
            ${GmLIBTARGET_GAMOS_LINK_LIBRARIES_STATIC}
            ${GmLIBTARGET_LINK_LIBRARIES})

        # But we can rename the output library to the correct name
        # On WIN32 we *retain* the -static postfix because otherwise
        # we'll conflict with the .lib from the DLL build...
        # We could also install differently...
        if(NOT WIN32)
            set_target_properties(${GmLIBTARGET_NAME}-static 
                PROPERTIES OUTPUT_NAME ${GmLIBTARGET_NAME})
        endif()
    
        set_target_properties(${GmLIBTARGET_NAME}-static
        	PROPERTIES CLEAN_DIRECT_OUTPUT 1)

        install(TARGETS ${GmLIBTARGET_NAME}-static
            EXPORT GamosLibraryDepends
        	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
	        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)

        set_property(GLOBAL APPEND
            PROPERTY GAMOS_EXPORTED_TARGETS ${GmLIBTARGET_NAME}-static)
    endif()
ENDMACRO()


#----------------------------------------------------------------------------
# - GAMOS_HEADER_MODULE_TARGET
# Build and install for a header only Gamos module.
#
MACRO(GAMOS_HEADER_MODULE_TARGET)
    CMAKE_PARSE_ARGUMENTS(GmHEADERMOD
        ""
        "COMPONENT"
        ""
        ${ARGN}
    )

    # Only has one component, and we just have to pick out the headers
    include(${GmHEADERMOD_COMPONENT})

    # Header install?
    install(FILES ${${GmMODULENAME}_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT Development)

    # Store the include path of the component so that the build tree
    # config file can pick up all needed header paths
    set_property(GLOBAL APPEND
        PROPERTY GAMOS_BUILDTREE_INCLUDE_DIRS ${${GmMODULENAME}_INCDIR})
ENDMACRO()



#----------------------------------------------------------------------------
# - GAMOS_GRANULAR_LIBRARY_TARGET
# Build and install for a Gamos module (granular) library
#
MACRO(GAMOS_GRANULAR_LIBRARY_TARGET)
    CMAKE_PARSE_ARGUMENTS(GmGRANLIB
        ""
        "COMPONENT"
        ""
        ${ARGN}
    )

    # Granular lib only has one component, but we must pick out
    # the granular dependencies
    include(${GmGRANLIB_COMPONENT})

    # Add the library target, using variables set by the inclusion of
    # the component file
    GAMOS_LIBRARY_TARGET(NAME ${GmMODULENAME}
        SOURCES ${${GmMODULENAME}_SOURCES} ${${GmMODULENAME}_HEADERS}
        GAMOS_LINK_LIBRARIES ${${GmMODULENAME}_GRANULAR_DEPENDENCIES}
        LINK_LIBRARIES ${${GmMODULENAME}_LINK_LIBRARIES})

    # Header install?
    install(FILES ${${GmMODULENAME}_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT Development)

    # Store the include path of the component so that the build tree
    # config file can pick up all needed header paths
    set_property(GLOBAL APPEND
        PROPERTY GAMOS_BUILDTREE_INCLUDE_DIRS ${${GmMODULENAME}_INCDIR})
ENDMACRO()



#----------------------------------------------------------------------------
# - GAMOS_GLOBAL_LIBRARY_TARGET
# Build and install of a Gamos category (global) library
#
MACRO(GAMOS_GLOBAL_LIBRARY_TARGET)
    CMAKE_PARSE_ARGUMENTS(GmGLOBLIB
        ""
        "NAME"
        "COMPONENTS"
        ${ARGN}
    )

    # We loop over the component sources one at a time,
    # appending properties as we go.
    foreach(_comp ${GmGLOBLIB_COMPONENTS})
        include(${_comp})
        # In case we have a global lib with one component, ensure name gets set
        if(NOT GmGLOBLIB_NAME)
            set(GmGLOBLIB_NAME ${GmMODULENAME})
        endif()

        list(APPEND ${GmGLOBLIB_NAME}_GLOBAL_SOURCES ${${GmMODULENAME}_SOURCES})
        list(APPEND ${GmGLOBLIB_NAME}_GLOBAL_HEADERS ${${GmMODULENAME}_HEADERS})

        list(APPEND ${GmGLOBLIB_NAME}_GLOBAL_DEPENDENCIES
            ${${GmMODULENAME}_GLOBAL_DEPENDENCIES})

        list(APPEND ${GmGLOBLIB_NAME}_LINK_LIBRARIES
            ${${GmMODULENAME}_LINK_LIBRARIES})

        list(APPEND ${GmGLOBLIB_NAME}_BUILDTREE_INCLUDES ${${GmMODULENAME}_INCDIR})
    endforeach()

    # Filter out duplicates in GLOBAL_DEPENDENCIES and LINK_LIBRARIES
    if(${GmGLOBLIB_NAME}_GLOBAL_DEPENDENCIES)
        list(REMOVE_DUPLICATES ${GmGLOBLIB_NAME}_GLOBAL_DEPENDENCIES)
    endif()
    if(${GmGLOBLIB_NAME}_LINK_LIBRARIES)
        list(REMOVE_DUPLICATES ${GmGLOBLIB_NAME}_LINK_LIBRARIES)
    endif()

    # Now add the library target
    GAMOS_LIBRARY_TARGET(NAME ${GmGLOBLIB_NAME} 
        SOURCES 
            ${${GmGLOBLIB_NAME}_GLOBAL_SOURCES} 
            ${${GmGLOBLIB_NAME}_GLOBAL_HEADERS}
        GAMOS_LINK_LIBRARIES 
            ${${GmGLOBLIB_NAME}_GLOBAL_DEPENDENCIES}
        LINK_LIBRARIES 
            ${${GmGLOBLIB_NAME}_LINK_LIBRARIES})

    # Header install?
    install(FILES ${${GmGLOBLIB_NAME}_GLOBAL_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT Development)

    # Store the include path of the component so that the build tree
    # config file can pick up all needed header paths
    set_property(GLOBAL APPEND
        PROPERTY GAMOS_BUILDTREE_INCLUDE_DIRS ${${GmGLOBLIB_NAME}_BUILDTREE_INCLUDES})

ENDMACRO()



