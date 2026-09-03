##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.28)
project(TTS LANGUAGES CXX)

if(DEFINED CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)

##======================================================================================================================
option( TTS_BUILD_TEST          "Build tests for TTS"                                          ON  )
option( TTS_BUILD_DOCUMENTATION "Build Doxygen for TTS"                                        OFF )
option( TTS_ENABLE_SANITIZERS   "Build tests with AddressSanitizer + UndefinedBehaviorSanitizer" OFF )
option( TTS_ENABLE_COVERAGE     "Build tests with code coverage instrumentation"               OFF )
option( TTS_COMPILE_COST        "Measure what every test unit costs to compile"                OFF )

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)
copa_project_version(MAJOR 3 MINOR 0 PATCH 0 REPOSITORY https://github.com/jfalcou/tts)

##======================================================================================================================
## Summary Display
##======================================================================================================================
if(NOT TTS_QUIET)
  if(CMAKE_BUILD_TYPE)
    message(STATUS "[${PROJECT_NAME}] - Building in ${CMAKE_BUILD_TYPE} mode")
  endif()
  message(STATUS "[${PROJECT_NAME}] - Unit tests   : ${TTS_BUILD_TEST} (via TTS_BUILD_TEST)")
  message(STATUS "[${PROJECT_NAME}] - Doxygen      : ${TTS_BUILD_DOCUMENTATION} (via TTS_BUILD_DOCUMENTATION)")
  message(STATUS "[${PROJECT_NAME}] - Sanitizers   : ${TTS_ENABLE_SANITIZERS} (via TTS_ENABLE_SANITIZERS)")
  message(STATUS "[${PROJECT_NAME}] - Coverage     : ${TTS_ENABLE_COVERAGE} (via TTS_ENABLE_COVERAGE)")
  message(STATUS "[${PROJECT_NAME}] - Compile cost : ${TTS_COMPILE_COST} (via TTS_COMPILE_COST)")
  set(QUIET_OPTION "")
else()
  set(QUIET_OPTION "QUIET")
endif()

##======================================================================================================================
## Precommit hooks setup
##======================================================================================================================
copa_setup_precommit_hooks(${QUIET_OPTION})

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
  copa_setup_doxygen(
    ${QUIET_OPTION}
    TARGET tts-doxygen
    DESTINATION "${PROJECT_BINARY_DIR}/doc"
    GODBOLT_LIBRARIES tts
    COLOR_HUE 189
    COLOR_SATURATION 48
    COLOR_LIGHTNESS 48)
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(TTS_BUILD_TEST)
  if(TTS_ENABLE_SANITIZERS)
    copa_setup_sanitizers(tts_test ENABLE_ASAN ENABLE_UBSAN)
  endif()

  include(CTest)
  enable_testing()
  add_custom_target(tts-unit)
  add_subdirectory(test)

  # After the test targets exist, so the coverage targets can depend on them
  if(TTS_ENABLE_COVERAGE)
    copa_setup_coverage(tts_test)
  endif()

  # clang only: -fproc-stat-report is its flag. After the test targets, as the measurement rebuilds them.
  if(TTS_COMPILE_COST)
    copa_setup_compile_cost(tts_test)
  endif()
endif()
