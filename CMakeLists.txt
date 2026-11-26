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

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)

copa_add_option(TTS_BUILD_TEST "Build tests for TTS" ON LABEL "Unit tests")
copa_add_option(TTS_BUILD_DOCUMENTATION "Build Doxygen for TTS" OFF LABEL "Doxygen")
copa_add_option(TTS_BENCHMARK_DOCUMENTATION "Compare TTS with other test libraries in the documentation" OFF)
copa_add_option(TTS_ENABLE_SANITIZERS "Build tests with AddressSanitizer + UndefinedBehaviorSanitizer" OFF
                LABEL "Sanitizers")
copa_add_option(TTS_ENABLE_COVERAGE "Build tests with code coverage instrumentation" OFF LABEL "Coverage")
copa_add_option(TTS_COMPILE_COST "Measure what every test unit costs to compile" OFF)
copa_add_option(TTS_BENCHMARKS "Activate tts::benchmarks" OFF LABEL "Benchmarks")
copa_add_option(TTS_EXTERNAL_DEPENDENCIES "Take dependencies from the environment instead of fetching them" OFF)
copa_project_version(
  MAJOR 4
  MINOR 1
  PATCH 0
  REPOSITORY https://github.com/jfalcou/tts)

##======================================================================================================================
## Summary Display
##======================================================================================================================
copa_show_options()

if(NOT TTS_QUIET)
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
copa_setup_install(
  LIBRARY tts
  FEATURES cxx_std_20
  DOC ${PROJECT_SOURCE_DIR}/LICENSE.md
  INCLUDE ${PROJECT_SOURCE_DIR}/include/tts)

if(TTS_BENCHMARKS)
  add_library(tts_benchmarks INTERFACE)
  add_library(tts::benchmarks ALIAS tts_benchmarks)
  target_link_libraries(tts_benchmarks INTERFACE tts::tts)
  target_include_directories(tts_benchmarks SYSTEM INTERFACE $<BUILD_INTERFACE:${TTS_NANOBENCH_INCLUDE_DIR}>)
endif()

##======================================================================================================================
## Standalone generation
##======================================================================================================================
copa_setup_standalone(
  QUIET
  FILE tts.hpp
  SOURCE include
  ROOT tts
  TARGET tts-standalone
  OUTPUT "${PROJECT_BINARY_DIR}")

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(TTS_BUILD_DOCUMENTATION)
  copa_setup_doxygen(
    ${QUIET_OPTION}
    TARGET tts-doxygen
    DESTINATION "${PROJECT_BINARY_DIR}/doc"
    GODBOLT_LIBRARIES tts nanobench@4311
    COLOR_HUE 189
    COLOR_SATURATION 48
    COLOR_LIGHTNESS 48)

  if(TTS_BENCHMARK_DOCUMENTATION)
    add_subdirectory(benchmarks/compile-time)
    add_dependencies(tts-doxygen tts-compile-bench)
  else()
    # The page includes this block whether or not the comparison runs.
    foreach(
      block
      meta
      include-gcc
      include-clang
      one-gcc
      one-clang
      mains-gcc
      mains-clang
      cases-gcc
      cases-clang
      tests-gcc
      tests-clang
      files-gcc
      files-clang)
      file(WRITE "${PROJECT_BINARY_DIR}/doc/bench/bench-${block}.html"
           "<p>This build of the documentation did not run the comparison.</p>\n")
    endforeach()
  endif()
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(TTS_BUILD_TEST)
  if(TTS_ENABLE_SANITIZERS)
    copa_setup_sanitizers(tts_test ENABLE_ASAN ENABLE_UBSAN)
  endif()

  set(MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/test/valgrind.supp")
  include(CTest)
  enable_testing()
  add_custom_target(tts-unit COMMENT "[tts] - Building the unit tests")
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
