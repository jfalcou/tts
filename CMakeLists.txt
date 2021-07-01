##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
cmake_minimum_required(VERSION 3.17)

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
## Options
##==================================================================================================
option( TTS_BUILD_TEST    "Build tests for tts"     ON  )

##==================================================================================================
## Setup install process
##==================================================================================================
include(GNUInstallDirs)

install ( DIRECTORY ${PROJECT_SOURCE_DIR}/include/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

export(PACKAGE tts)

##==================================================================================================
## Standalone generation
##==================================================================================================
add_custom_command(OUTPUT tts.hpp
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/src/embed.py"
          ${CMAKE_CURRENT_SOURCE_DIR}/src/tts/tts.hpp
          -I src/
          -o ${CMAKE_CURRENT_SOURCE_DIR}/include/tts/tts.hpp
          --include-match tts/*

  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  COMMENT "Generating standalone headers"
  VERBATIM
)

add_custom_target(standalone DEPENDS tts.hpp)

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
