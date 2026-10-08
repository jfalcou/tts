##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## What a test unit costs to compile with TTS and with other test libraries, drawn into the documentation
##======================================================================================================================
find_package(Python3 REQUIRED COMPONENTS Interpreter)

## Only the headers are compiled, so nothing here is built.
CPMAddPackage(NAME doctest GITHUB_REPOSITORY doctest/doctest GIT_TAG v2.4.11 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME ut GITHUB_REPOSITORY boost-ext/ut GIT_TAG v2.1.1 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME googletest GITHUB_REPOSITORY google/googletest GIT_TAG v1.15.2 DOWNLOAD_ONLY YES)
CPMAddPackage(NAME Catch2 GITHUB_REPOSITORY catchorg/Catch2 GIT_TAG v3.7.1 DOWNLOAD_ONLY YES)

## Catch2 generates this header when it is configured, which DOWNLOAD_ONLY skips.
configure_file("${Catch2_SOURCE_DIR}/src/catch2/catch_user_config.hpp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/catch2-config/catch2/catch_user_config.hpp")

find_path(TTS_BENCH_BOOST_INCLUDE boost/test/included/unit_test.hpp)
find_program(TTS_BENCH_GXX NAMES g++)
find_program(TTS_BENCH_CLANGXX NAMES clang++)

set(TTS_BENCH_OPTIONS "-O0" CACHE STRING "Options every measured compilation gets")
set(TTS_BENCH_RUNS 3 CACHE STRING "Compilations per measured point")
set(TTS_BENCH_JOBS 2 CACHE STRING "Compilations measured at once")
set(TTS_BENCH_SIMPLE_CASES "1;50;200" CACHE STRING "Numbers of simple test cases measured")
set(TTS_BENCH_TEMPLATE_CASES "1;25;100" CACHE STRING "Numbers of template test cases measured")
set(TTS_BENCH_TYPES_CASES "1;25;100" CACHE STRING "Numbers of types, as separate cases or one template case")

set(LIBRARIES
    "--library=tts=-I${PROJECT_SOURCE_DIR}/include"
    "--library=doctest=-I${doctest_SOURCE_DIR}"
    "--library=ut=-I${ut_SOURCE_DIR}/include"
    "--library=gtest=-I${googletest_SOURCE_DIR}/googletest/include"
    "--library=catch2=-I${Catch2_SOURCE_DIR}/src -I${CMAKE_CURRENT_BINARY_DIR}/catch2-config")
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
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generate.py" "${BENCH_DIR}/units"
          "--simple=${TTS_BENCH_SIMPLE_CASES}" "--template=${TTS_BENCH_TEMPLATE_CASES}"
          "--types=${TTS_BENCH_TYPES_CASES}"
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/measure.py" "${BENCH_DIR}/units"
          "${BENCH_DIR}/results.json" ${COMPILERS} ${LIBRARIES} "--options=${TTS_BENCH_OPTIONS}" --runs ${TTS_BENCH_RUNS} --jobs ${TTS_BENCH_JOBS}
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/plot.py" "${BENCH_DIR}/results.json"
          "${PROJECT_BINARY_DIR}/doc/bench"
  COMMENT "[TTS] - Measuring what a test unit costs to compile with TTS and with other test libraries"
  USES_TERMINAL VERBATIM)
