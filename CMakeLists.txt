##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
cmake_minimum_required(VERSION 3.5)

##==================================================================================================
## Setup project
##==================================================================================================
project(TTS VERSION 0.1 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##==================================================================================================
## Prevent in-source build
##==================================================================================================
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message( FATAL_ERROR "[tts] Building in-source, not recommended! Build in a separate directory." )
endif()

##==================================================================================================
## Compute version string and mode
##==================================================================================================
include(parse_revision)

##==================================================================================================
## Add src CMake's
##==================================================================================================
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

##==================================================================================================
## Options
##==================================================================================================
option( TTS_BUILD_TEST  "Build tests for tts" ON )
option( TTS_BUILD_DOC   "Build doc for tts"   ON )

##==================================================================================================
## Setup Documentation
##==================================================================================================
if( TTS_BUILD_DOC )
  add_subdirectory(doc)
endif()

##==================================================================================================
## Install process
##==================================================================================================
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/tts)

install(TARGETS tts
    EXPORT tts-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

##==================================================================================================
## This is required so that the exported target has the name TTS and not tts
##==================================================================================================
set_target_properties(tts PROPERTIES EXPORT_NAME tts)

install ( DIRECTORY ${PROJECT_SOURCE_DIR}/include/tts
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

##==================================================================================================
## Export the targets to a script
##==================================================================================================
install(EXPORT tts-targets
  FILE
    ttsTargets.cmake
  NAMESPACE
    tts::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

##==================================================================================================
## Create a ConfigVersion.cmake file
##==================================================================================================
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ttsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ttsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ttsConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

##==================================================================================================
##Install the config, configversion and custom find modules
##==================================================================================================
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ttsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ttsConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##==================================================================================================
## Exporting from the build tree
##==================================================================================================
export(EXPORT tts-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/ttsTargets.cmake NAMESPACE tts::)

##==================================================================================================
## Register package in user's package registry
##==================================================================================================
export(PACKAGE tts)

##==================================================================================================
## Find OpenMP
##==================================================================================================
find_package(OpenMP)

##==================================================================================================
## Setup Testing
##==================================================================================================
if( TTS_BUILD_TEST )
  enable_testing()
  include(CTest)
  add_custom_target(tests)
  add_custom_target(unit)

  add_dependencies(tests unit)
  add_subdirectory(test)
endif()
