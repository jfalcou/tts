##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## What a test executable costs to build with TTS and with other test libraries, drawn into the documentation
##======================================================================================================================
find_package(Python3 REQUIRED COMPONENTS Interpreter)

## DOWNLOAD_ONLY: the benchmark compiles everything it measures itself.
CPMAddPackage(NAME doctest GITHUB_REPOSITORY doctest/doctest GIT_TAG v2.4.11 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME ut GITHUB_REPOSITORY boost-ext/ut GIT_TAG v2.1.1 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME googletest GITHUB_REPOSITORY google/googletest GIT_TAG v1.15.2 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME Catch2 GITHUB_REPOSITORY catchorg/Catch2 GIT_TAG v3.7.1 DOWNLOAD_ONLY YES)

## Catch2 generates this header when it is configured, which DOWNLOAD_ONLY skips; the two values are its defaults.
set(CATCH_CONFIG_DEFAULT_REPORTER "console")
set(CATCH_CONFIG_CONSOLE_WIDTH "80")
configure_file("${Catch2_SOURCE_DIR}/src/catch2/catch_user_config.hpp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/catch2-config/catch2/catch_user_config.hpp")

file(GLOB_RECURSE CATCH2_MAIN_SOURCES CONFIGURE_DEPENDS "${Catch2_SOURCE_DIR}/src/catch2/*.cpp")
list(JOIN CATCH2_MAIN_SOURCES "\n" CATCH2_MAIN_LIST)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/catch2-main.txt" "${CATCH2_MAIN_LIST}\n")
set(GTEST_MAIN_SOURCES "${googletest_SOURCE_DIR}/googletest/src/gtest-all.cc"
                       "${googletest_SOURCE_DIR}/googletest/src/gtest_main.cc")
list(JOIN GTEST_MAIN_SOURCES "\n" GTEST_MAIN_LIST)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/gtest-main.txt" "${GTEST_MAIN_LIST}\n")

find_path(TTS_BENCH_BOOST_INCLUDE boost/test/included/unit_test.hpp)
find_program(TTS_BENCH_GXX NAMES g++)
find_program(TTS_BENCH_CLANGXX NAMES clang++)

set(TTS_BENCH_OPTIONS "-O0" CACHE STRING "Options every measured compilation gets")
set(TTS_BENCH_RUNS 3 CACHE STRING "Builds per measured point")
set(TTS_BENCH_JOBS 2 CACHE STRING "Builds measured at once")
set(TTS_BENCH_SINGLE "1;2;5;10;20;50" CACHE STRING "Numbers of tests in the executables of one file")
set(TTS_BENCH_FILES "1;2;5;10;20;50" CACHE STRING "Numbers of one-test files in the executables of many files")
set(TTS_BENCH_TESTS 200 CACHE STRING "Number of tests in the file without runner that gives the cost of one test")

set(LIBRARIES
    "--library=tts=-I${PROJECT_SOURCE_DIR}/include"
    "--library=doctest=-I${doctest_SOURCE_DIR}"
    "--library=ut=-I${ut_SOURCE_DIR}/include"
    "--library=gtest=-I${googletest_SOURCE_DIR}/googletest/include -I${googletest_SOURCE_DIR}/googletest"
    "--library=catch2=-I${Catch2_SOURCE_DIR}/src -I${CMAKE_CURRENT_BINARY_DIR}/catch2-config"
    "--link=gtest=-pthread"
    "--main=catch2=@${CMAKE_CURRENT_BINARY_DIR}/catch2-main.txt"
    "--main=gtest=@${CMAKE_CURRENT_BINARY_DIR}/gtest-main.txt")
if(TTS_BENCH_BOOST_INCLUDE)
  list(APPEND LIBRARIES "--library=boost=-I${TTS_BENCH_BOOST_INCLUDE}")
else()
  message(STATUS "[TTS] - Boost.Test not found, the compile-time benchmark goes without it")
endif()

set(COMPILERS "")
if(TTS_BENCH_GXX)
  list(APPEND COMPILERS "--compiler=gcc=${TTS_BENCH_GXX}")
endif()
if(TTS_BENCH_CLANGXX)
  list(APPEND COMPILERS "--compiler=clang=${TTS_BENCH_CLANGXX}")
endif()

set(BENCH_DIR "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(
  tts-compile-bench
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generate.py" "${BENCH_DIR}/sources"
          "--single=${TTS_BENCH_SINGLE}" "--files=${TTS_BENCH_FILES}"
          "--tests=${TTS_BENCH_TESTS}"
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/measure.py" "${BENCH_DIR}/sources"
          "${BENCH_DIR}/results.json" ${COMPILERS} ${LIBRARIES} "--ar=${CMAKE_AR}" "--options=${TTS_BENCH_OPTIONS}"
          --runs ${TTS_BENCH_RUNS} --jobs ${TTS_BENCH_JOBS}
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/plot.py" "${BENCH_DIR}/results.json"
          "${PROJECT_BINARY_DIR}/doc/bench"
  COMMENT "[TTS] - Measuring what a test executable costs to build with TTS and with other test libraries"
  USES_TERMINAL VERBATIM)
