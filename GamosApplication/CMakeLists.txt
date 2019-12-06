#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosApplication
# Package: GamosCore
#
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

add_definitions(-DGmApplication_bin_ALLOC_EXPORT)

set(CMAKE_VERBOSE_MAKEFILE ON)
#include(Geant4MacroLibraryTargets)
#GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
project(gamos)


option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
#find_package(Gamos REQUIRED)
#find_package(ROOT REQUIRED)


include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

include(${Geant4_DIR}/UseGamosAtGeant4.cmake)  ##!! HAS TO BE COPIED BY HAND

#include_directories(${CMAKE_SOURCE_DIR}/../../../source)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(gamos gamos.cc ${sources} ${headers})
target_link_libraries(gamos ${Gamos_LIBRARIES} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

install(TARGETS gamos DESTINATION bin)

