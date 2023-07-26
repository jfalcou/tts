##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(tts LANGUAGES CXX)
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)

##======================================================================================================================
option( TTS_BUILD_TEST          "Build tests for TTS"   ON  )
option( TTS_BUILD_DOCUMENTATION "Build Doxygen for TTS" OFF )

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)
copa_project_version(MAJOR 2 MINOR 3 PATCH 0)

##======================================================================================================================
## Summary Display
##======================================================================================================================
if(NOT TTS_QUIET)
  if(CMAKE_BUILD_TYPE)
    message(STATUS "[${PROJECT_NAME}] - Building in ${CMAKE_BUILD_TYPE} mode")
  endif()
  message(STATUS "[${PROJECT_NAME}] - Unit tests : ${TTS_BUILD_TEST} (via TTS_BUILD_TEST)")
  message(STATUS "[${PROJECT_NAME}] - Doxygen    : ${TTS_BUILD_DOCUMENTATION} (via TTS_BUILD_DOCUMENTATION)")
endif()

##======================================================================================================================
## Install Process setup
##======================================================================================================================
copa_setup_install( LIBRARY tts
                    FEATURES cxx_std_20
                    DOC     ${PROJECT_SOURCE_DIR}/LICENSE.md
                    INCLUDE ${PROJECT_SOURCE_DIR}/include/tts
                  )

##======================================================================================================================
## Standalone generation
##======================================================================================================================
copa_setup_standalone(FILE tts.hpp SOURCE include DESTINATION standalone ROOT tts TARGET tts-standalone)

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(TTS_BUILD_DOCUMENATION)
  copa_setup_doxygen(QUIET TARGET tts-doxygen DESTINATION "${PROJECT_BINARY_DIR}/doc")
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(TTS_BUILD_TEST)
  enable_testing()
  add_custom_target(tts-unit)
  add_subdirectory(test)
endif()
