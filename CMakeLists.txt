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
## Find OpenMP
##==================================================================================================
find_package(OpenMP)

##==================================================================================================
## Add src CMake's
##==================================================================================================
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

##==================================================================================================
## Options
##==================================================================================================
option( TTS_BUILD_TEST    "Build tests for tts"     ON  )
option( TTS_BUILD_DOC     "Build doc for tts"       ON  )
option( TTS_IS_DEPENDENT  "Dependent mode for tts"  OFF )

##==================================================================================================
## Setup Documentation
##==================================================================================================
if( TTS_BUILD_DOC )
  add_subdirectory(doc)
endif()

##==================================================================================================
## Setup Install target only if not used as dependent
##==================================================================================================
if( NOT TTS_IS_DEPENDENT )
  include(install)
else()
  message(STATUS "[tts] TTS is used as a in-project dependency - No install targets available")
endif()

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
