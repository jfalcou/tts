##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.18)
project(tts-cpm-test LANGUAGES CXX)
enable_testing()

message(STATUS "Testing CPM for branch ${GIT_BRANCH}")

# Setup CPM
include(cpm.cmake)

# Add dependencies
CPMAddPackage ( NAME tts
                GIT_REPOSITORY "https://github.com/jfalcou/tts.git"
                GIT_TAG "${GIT_BRANCH}"
                OPTIONS "TTS_BUILD_TEST OFF"
              )

# Use TTS
add_executable(test_tts ../main.cpp)
target_link_libraries(test_tts PUBLIC tts::tts)
add_test(NAME test_tts COMMAND test_tts)
