##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.2)

##==================================================================================================
## Setup project
##==================================================================================================
project(tts LANGUAGES CXX)
set(TTS_MAJOR_VERSION 2)
set(TTS_MINOR_VERSION 2)
set(TTS_PATCH_VERSION 0)
set(TTS_VERSION ${TTS_MAJOR_VERSION}.${TTS_MINOR_VERSION}.${TTS_PATCH_VERSION})

set(PROJECT_VERSION   ${TTS_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/tts )

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
include(tts-install)

##==================================================================================================
## Standalone generation
##==================================================================================================
find_package(Python COMPONENTS Interpreter)

if(Python_FOUND)
  add_custom_command(OUTPUT tts.hpp
    COMMAND "${Python_EXECUTABLE}"
            ${CMAKE_CURRENT_SOURCE_DIR}/src/embed.py
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tts/tts.hpp
            -I src/
            -o ${CMAKE_CURRENT_SOURCE_DIR}/include/tts/tts.hpp
            --include-match tts/*

    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating standalone headers"
    VERBATIM
  )
add_custom_target(tts-standalone DEPENDS tts.hpp)
endif()

##==================================================================================================
## Setup Testing
##==================================================================================================
if( TTS_BUILD_TEST )
  enable_testing()
  include(CTest)
  add_custom_target(tests)
  add_custom_target(tts-unit)

  add_dependencies(tests tts-unit)
  add_subdirectory(test)
endif()
