##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
cmake_minimum_required(VERSION 3.18)

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
## Find OpenMP
##==================================================================================================
find_package(OpenMP)

##==================================================================================================
## Options
##==================================================================================================
option( TTS_BUILD_TEST    "Build tests for tts"     ON  )

##==================================================================================================
## Setup install process
##==================================================================================================
include(GNUInstallDirs)

install ( DIRECTORY ${PROJECT_SOURCE_DIR}/include/tts/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

export(PACKAGE tts)

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
