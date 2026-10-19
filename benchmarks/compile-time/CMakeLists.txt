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
set(TTS_BENCH_DOCTEST_VERSION 2.5.3)
set(TTS_BENCH_UT_VERSION 2.3.1)
set(TTS_BENCH_GTEST_VERSION 1.18.0)
set(TTS_BENCH_CATCH2_VERSION 3.16.1)
CPMAddPackage(NAME doctest GITHUB_REPOSITORY doctest/doctest GIT_TAG v${TTS_BENCH_DOCTEST_VERSION} DOWNLOAD_ONLY YES)
CPMAddPackage(NAME ut GITHUB_REPOSITORY boost-ext/ut GIT_TAG v${TTS_BENCH_UT_VERSION} DOWNLOAD_ONLY YES)
CPMAddPackage(NAME googletest GITHUB_REPOSITORY google/googletest GIT_TAG v${TTS_BENCH_GTEST_VERSION}
              DOWNLOAD_ONLY YES)
CPMAddPackage(NAME Catch2 GITHUB_REPOSITORY catchorg/Catch2 GIT_TAG v${TTS_BENCH_CATCH2_VERSION} DOWNLOAD_ONLY YES)

## Catch2 generates this header when it is configured, which DOWNLOAD_ONLY skips. The two values are its defaults.
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

set(BENCH_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(CATCH2_FLAGS "-I${Catch2_SOURCE_DIR}/src -I${CMAKE_CURRENT_BINARY_DIR}/catch2-config")
## The _fast series take the setting each documentation gives for faster compilation, Catch2 library included.
set(LIBRARIES
    "--library=tts=-I${PROJECT_SOURCE_DIR}/include"
    "--library=doctest=-I${doctest_SOURCE_DIR}"
    "--library=doctest_fast=-DDOCTEST_CONFIG_SUPER_FAST_ASSERTS -I${doctest_SOURCE_DIR}"
    "--library=ut=-I${ut_SOURCE_DIR}/include"
    "--library=gtest=-I${googletest_SOURCE_DIR}/googletest/include -I${googletest_SOURCE_DIR}/googletest"
    "--library=catch2=${CATCH2_FLAGS}"
    "--library=catch2_fast=-DCATCH_CONFIG_FAST_COMPILE ${CATCH2_FLAGS}"
    "--link=gtest=-pthread"
    "--main=doctest=${BENCH_DIR}/sources/mains/doctest.cpp"
    "--main=doctest_fast=${BENCH_DIR}/sources/mains/doctest_fast.cpp"
    "--main=catch2=@${CMAKE_CURRENT_BINARY_DIR}/catch2-main.txt"
    "--main=catch2_fast=@${CMAKE_CURRENT_BINARY_DIR}/catch2-main.txt"
    "--main=gtest=@${CMAKE_CURRENT_BINARY_DIR}/gtest-main.txt")

## The CI container checks the sources out under another user, which git refuses without safe.directory.
execute_process(COMMAND git -c safe.directory=* rev-parse --short HEAD WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                OUTPUT_VARIABLE TTS_BENCH_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
if(TTS_BENCH_COMMIT)
  set(TTS_BENCH_VERSION "at ${TTS_BENCH_COMMIT}")
else()
  set(TTS_BENCH_VERSION "${PROJECT_VERSION}")
endif()
set(VERSIONS
    "--version=tts=${TTS_BENCH_VERSION}"
    "--version=doctest=${TTS_BENCH_DOCTEST_VERSION}"
    "--version=catch2=${TTS_BENCH_CATCH2_VERSION}"
    "--version=gtest=${TTS_BENCH_GTEST_VERSION}"
    "--version=ut=${TTS_BENCH_UT_VERSION}")

if(TTS_BENCH_BOOST_INCLUDE)
  file(STRINGS "${TTS_BENCH_BOOST_INCLUDE}/boost/version.hpp" BOOST_LINE REGEX "define BOOST_LIB_VERSION")
  string(REGEX REPLACE ".*\"([0-9_]+)\".*" "\\1" BOOST_VERSION "${BOOST_LINE}")
  string(REPLACE "_" "." BOOST_VERSION "${BOOST_VERSION}")
  list(APPEND LIBRARIES "--library=boost=-I${TTS_BENCH_BOOST_INCLUDE}"
                        "--main=boost=${BENCH_DIR}/sources/mains/boost.cpp")
  list(APPEND VERSIONS "--version=boost=${BOOST_VERSION}")
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

add_custom_target(
  tts-compile-bench
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generate.py" "${BENCH_DIR}/sources"
          "--single=${TTS_BENCH_SINGLE}" "--files=${TTS_BENCH_FILES}"
          "--tests=${TTS_BENCH_TESTS}"
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/measure.py" "${BENCH_DIR}/sources"
          "${BENCH_DIR}/results.json" ${COMPILERS} ${LIBRARIES} ${VERSIONS} "--ar=${CMAKE_AR}"
          "--options=${TTS_BENCH_OPTIONS}"
          --runs ${TTS_BENCH_RUNS} --jobs ${TTS_BENCH_JOBS}
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/plot.py" "${BENCH_DIR}/results.json"
          "${PROJECT_BINARY_DIR}/doc/bench"
  COMMENT "[TTS] - Measuring what a test executable costs to build with TTS and with other test libraries"
  USES_TERMINAL VERBATIM)
