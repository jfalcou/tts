##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.18)
project(tts-install-test LANGUAGES CXX)
enable_testing()

find_package(tts CONFIG REQUIRED)
add_executable(test_tts ../main.cpp)
target_link_libraries(test_tts PUBLIC tts::tts)
add_test(NAME test_tts COMMAND test_tts)
