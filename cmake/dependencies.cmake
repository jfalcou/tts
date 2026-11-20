##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Take the dependencies from the environment, without CPM
##======================================================================================================================
option(TTS_EXTERNAL_DEPENDENCIES "Take dependencies from the environment instead of fetching them" OFF)

if(TTS_EXTERNAL_DEPENDENCIES)
  if(NOT COPACABANA_SOURCE_DIR)
    message(FATAL_ERROR "[TTS] - TTS_EXTERNAL_DEPENDENCIES needs COPACABANA_SOURCE_DIR")
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
