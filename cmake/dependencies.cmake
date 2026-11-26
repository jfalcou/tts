##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Take the dependencies from the environment, without CPM
##======================================================================================================================
option(TTS_EXTERNAL_DEPENDENCIES "Take dependencies from the environment instead of fetching them" OFF)
option(TTS_BENCHMARKS "Provide tts::benchmarks, which benchmarks functions with nanobench" OFF)

if(TTS_EXTERNAL_DEPENDENCIES)
  if(NOT COPACABANA_SOURCE_DIR)
    message(FATAL_ERROR "[TTS] - TTS_EXTERNAL_DEPENDENCIES needs COPACABANA_SOURCE_DIR")
  endif()
  if(TTS_BENCHMARKS)
    find_path(TTS_NANOBENCH_INCLUDE_DIR nanobench.h REQUIRED)
  endif()
  return()
endif()

##======================================================================================================================
## CPM is vendored: the download bootstrap reports nothing on a failure and leaves an empty file.
##======================================================================================================================
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

##======================================================================================================================
## Retrieve dependencies
##======================================================================================================================
CPMAddPackage(NAME COPACABANA GITHUB_REPOSITORY jfalcou/copacabana GIT_TAG eb45d6163c3cafed95d34b541400500f500ad585)

if(TTS_BENCHMARKS)
  CPMAddPackage(
    NAME nanobench
    VERSION 4.6.0
    GITHUB_REPOSITORY martinus/nanobench
    GIT_TAG b709728dba4df0356d467abfb0ba20781a8031c0
    DOWNLOAD_ONLY YES)
  set(TTS_NANOBENCH_INCLUDE_DIR "${nanobench_SOURCE_DIR}/src/include")
endif()
