##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
copa_setup_test_targets()
set(TEST_ROOT "${CMAKE_SOURCE_DIR}/test")

##======================================================================================================================
## Test for TTS infrastructure code
##======================================================================================================================
copa_make_single_unit(
  INTERFACE tts_test
  FILES unit/infrastructure/bitcast.cpp
        unit/infrastructure/buffer.cpp
        unit/infrastructure/callable.cpp
        unit/infrastructure/display.cpp
        unit/infrastructure/options.cpp
        unit/infrastructure/output.cpp
        unit/infrastructure/main.cpp
  ROOT ${TEST_ROOT}
  NAME unit.infrastructure.basic.cpp)

copa_make_single_unit(
  INTERFACE tts_test
  FILES unit/infrastructure/random.cpp
  ROOT ${TEST_ROOT}
  NAME unit.infrastructure.random.cpp)

copa_make_single_unit(
  INTERFACE tts_test
  FILES unit/infrastructure/failure.cpp
  ROOT ${TEST_ROOT}
  NAME unit.infrastructure.failure.cpp)

## Replaces the global operator new and delete, so it cannot share an executable with other units
copa_make_single_unit(
  INTERFACE tts_test
  FILES unit/infrastructure/replaced_new.cpp
  ROOT ${TEST_ROOT}
  NAME unit.infrastructure.replaced_new.cpp)

## valgrind serves operator new itself unless told to leave the one an executable defines
set_property(TEST unit.infrastructure.replaced_new.exe APPEND
             PROPERTY ENVIRONMENT "VALGRIND_OPTS=--soname-synonyms=somalloc=nouserintercepts")

##======================================================================================================================
## Test for TTS tests
##======================================================================================================================
copa_make_single_unit(
  INTERFACE tts_test
  DEPENDENCIES unit.infrastructure.exe
  FILES unit/basic/main.cpp unit/basic/exceptions.cpp unit/basic/expect.cpp unit/basic/expect_not.cpp
        unit/basic/when.cpp
  ROOT ${TEST_ROOT}
  NAME unit.basic.cpp)

copa_make_single_unit(
  INTERFACE tts_test
  DEPENDENCIES unit.infrastructure.exe
  FILES unit/precision/main.cpp
        unit/precision/absolute.cpp
        unit/precision/ieee.cpp
        unit/precision/range.cpp
        unit/precision/relative.cpp
        unit/precision/sequence.cpp
        unit/precision/ulp.cpp
  ROOT ${TEST_ROOT}
  NAME unit.precision.cpp)

copa_make_single_unit(
  INTERFACE tts_test
  DEPENDENCIES unit.infrastructure.exe
  FILES unit/relation/main.cpp
        unit/relation/bit_equal.cpp
        unit/relation/comparison.cpp
        unit/relation/equal.cpp
        unit/relation/greater.cpp
        unit/relation/greater_equal.cpp
        unit/relation/less.cpp
        unit/relation/less_equal.cpp
        unit/relation/not_equal.cpp
  ROOT ${TEST_ROOT}
  NAME unit.relation.cpp)

copa_make_single_unit(
  INTERFACE tts_test
  DEPENDENCIES unit.infrastructure.exe
  FILES unit/generator/main.cpp unit/generator/customization.cpp unit/generator/generator.cpp unit/generator/limits.cpp
        unit/generator/types.cpp
  ROOT ${TEST_ROOT}
  NAME unit.generator.cpp)

##======================================================================================================================
## Test for TTS driver, options and sinks - one executable per file as each installs its own driver
##======================================================================================================================
copa_glob_unit(
  QUIET
  PATTERN "unit/driver/*.cpp"
  RELATIVE ${TEST_ROOT}
  INTERFACE tts_test
  DEPENDENCIES unit.infrastructure.exe)

## crash.cpp brings its own process down on purpose and always exits 1, so its verdict is the line
## its handler prints. Under emscripten the guard is compiled out and 77 marks the test skipped.
set_tests_properties(unit.driver.crash.exe PROPERTIES SKIP_RETURN_CODE 77 PASS_REGULAR_EXPRESSION
                                                      "CRASH ATTRIBUTION OK")

## timeout.cpp hangs on purpose until its own deadline kills it, and always exits 1, so its verdict
## is the line its handler prints. Under emscripten there is no process timer and 77 marks it skipped.
set_tests_properties(unit.driver.timeout.exe PROPERTIES SKIP_RETURN_CODE 77 PASS_REGULAR_EXPRESSION
                                                        "TIMEOUT ATTRIBUTION OK" TIMEOUT 30)

##======================================================================================================================
## Units that must fail to compile: the CTest entry builds the excluded target and passes when the
## build prints the diagnostic. Serial, since two builds in one tree would write the same ninja
## files at once.
##======================================================================================================================
function(tts_expect_compile_failure source diagnostic)
  get_filename_component(stem ${source} NAME_WE)
  set(test "ill_formed.${stem}")
  add_executable(${test} EXCLUDE_FROM_ALL ${TEST_ROOT}/ill_formed/${source})
  target_link_libraries(${test} PRIVATE tts_test)
  add_test(NAME ${test} COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${test})
  set_tests_properties(${test} PROPERTIES PASS_REGULAR_EXPRESSION "${diagnostic}" RUN_SERIAL TRUE)
endfunction()

tts_expect_compile_failure(constexpr_runtime_call.cpp "constant")
tts_expect_compile_failure(consteval_precision_check.cpp "constant")
tts_expect_compile_failure(template_case_without_context.cpp "takes TTS_CONTEXT")
tts_expect_compile_failure(generated_case_context.cpp "TTS_CASE_WITH takes no context")

##======================================================================================================================
## Test for TTS Documentation
##======================================================================================================================
copa_glob_unit(
  QUIET
  PATTERN "unit/doc/*.cpp"
  RELATIVE ${TEST_ROOT}
  INTERFACE tts_test)
