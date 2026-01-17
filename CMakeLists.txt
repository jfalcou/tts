##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(TTS LANGUAGES CXX)
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
copa_project_version(MAJOR 3 MINOR 0 PATCH 0)

##======================================================================================================================
## Summary Display
##======================================================================================================================
if(NOT TTS_QUIET)
  if(CMAKE_BUILD_TYPE)
    message(STATUS "[${PROJECT_NAME}] - Building in ${CMAKE_BUILD_TYPE} mode")
  endif()
  message(STATUS "[${PROJECT_NAME}] - Unit tests : ${TTS_BUILD_TEST} (via TTS_BUILD_TEST)")
  message(STATUS "[${PROJECT_NAME}] - Doxygen    : ${TTS_BUILD_DOCUMENTATION} (via TTS_BUILD_DOCUMENTATION)")
  set(QUIET_OPTION "")
else()
  set(QUIET_OPTION "QUIET")
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
copa_setup_standalone ( QUIET
                        FILE tts.hpp SOURCE include ROOT tts TARGET tts-standalone
                        OUTPUT "${PROJECT_BINARY_DIR}"
                      )

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(TTS_BUILD_DOCUMENTATION)
  copa_setup_doxygen(${QUIET_OPTION} TARGET tts-doxygen DESTINATION "${PROJECT_BINARY_DIR}/doc")
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(TTS_BUILD_TEST)
  enable_testing()
  add_custom_target(tts-unit)
  add_subdirectory(test)
endif()
