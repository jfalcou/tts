##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.22)
project(tts-fetch-test LANGUAGES CXX)
enable_testing()

message(STATUS "Testing FetchContent for branch ${GIT_BRANCH}")

# Enable FetchContent
include(FetchContent)

# Retrieve TTS from relative git directory
FetchContent_Declare(tts
  GIT_REPOSITORY "https://github.com/jfalcou/tts.git"
  GIT_TAG ${GIT_BRANCH}
)

# make available
FetchContent_MakeAvailable(tts)

add_executable(test_tts ../main.cpp)
target_link_libraries(test_tts PUBLIC tts::tts)
add_test(NAME test_tts COMMAND test_tts)
