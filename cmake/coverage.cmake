##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Instrument a target for coverage collection and setup the report generation targets
##======================================================================================================================
function(TTS_SETUP_COVERAGE target)
  if(NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    message(FATAL_ERROR "[${PROJECT_NAME}] - Coverage requires GCC or Clang, got ${CMAKE_CXX_COMPILER_ID}")
  endif()

  set(COVERAGE_FLAGS --coverage)

  # Without this, GCC stores the notes' paths relative to the object file and gcov resolves none
  # of the include/tts headers - which, for a header-only library, is the entirety of the report.
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND COVERAGE_FLAGS -fprofile-abs-path)
  endif()

  target_compile_options(${target} INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_FLAGS}>)
  target_link_options(${target} INTERFACE --coverage)

  find_program(GCOVR_EXECUTABLE gcovr)
  if(NOT GCOVR_EXECUTABLE)
    message(FATAL_ERROR "[${PROJECT_NAME}] - Coverage requires gcovr, install it with 'pip install gcovr'")
  endif()

  # gcovr 8 sums a header's lines once per translation unit including it instead of taking their
  # union, which on a header-only template library reports totals larger than the files themselves.
  execute_process( COMMAND ${GCOVR_EXECUTABLE} --version
                   OUTPUT_VARIABLE GCOVR_VERSION_OUTPUT
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                 )

  if(GCOVR_VERSION_OUTPUT MATCHES "gcovr ([0-9]+)" AND CMAKE_MATCH_1 GREATER_EQUAL 8)
    message(WARNING
            "[${PROJECT_NAME}] - gcovr ${CMAKE_MATCH_1}.x inflates per-file totals on templates, "
            "its numbers will not match the CI's gcovr 7.x")
  endif()

  # Clang writes notes GCC's gcov cannot read, llvm-cov's gcov mode is what reads them back - and
  # only the one matching the compiler, hence looking for the versioned name first as distributions
  # usually ship no unsuffixed llvm-cov at all.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    string(REGEX MATCH "^[0-9]+" CLANG_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
    find_program(LLVM_COV_EXECUTABLE NAMES "llvm-cov-${CLANG_VERSION}" llvm-cov)

    if(NOT LLVM_COV_EXECUTABLE)
      message(FATAL_ERROR
              "[${PROJECT_NAME}] - Coverage with Clang requires llvm-cov, install package llvm-${CLANG_VERSION}")
    endif()

    set(GCOV_TOOL "${LLVM_COV_EXECUTABLE} gcov")
  else()
    set(GCOV_TOOL "gcov")
  endif()

  set(COVERAGE_DIR "${PROJECT_BINARY_DIR}/coverage")

  # Only the library itself is worth reporting on - the tests exercising it are not. The search path
  # must stay on this build tree: --root alone lets gcovr wander into a sibling build made by
  # another compiler, whose notes the local gcov refuses to read.
  set(GCOVR_COMMAND ${GCOVR_EXECUTABLE}
                    "${PROJECT_BINARY_DIR}"
                    --root "${PROJECT_SOURCE_DIR}"
                    --filter "${PROJECT_SOURCE_DIR}/include/tts/"
                    --gcov-executable "${GCOV_TOOL}"
                    --exclude-unreachable-branches
                    --exclude-throw-branches
                    --print-summary
  )

  add_custom_target( tts-coverage-report
                     COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_DIR}"
                     COMMAND ${GCOVR_COMMAND}
                             --html-details "${COVERAGE_DIR}/index.html"
                             --json-summary "${COVERAGE_DIR}/summary.json"
                             --json-summary-pretty
                     WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                     COMMENT "[${PROJECT_NAME}] - Generating coverage report in ${COVERAGE_DIR}"
                     VERBATIM
                   )

  add_custom_target( tts-coverage
                     COMMAND ${CMAKE_COMMAND} -DCOVERAGE_BUILD_DIR=${PROJECT_BINARY_DIR}
                                              -P "${PROJECT_SOURCE_DIR}/cmake/reset_coverage.cmake"
                     COMMAND ${CMAKE_CTEST_COMMAND} --build-config $<CONFIG> --output-on-failure
                     WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                     COMMENT "[${PROJECT_NAME}] - Running the test suite under coverage"
                     VERBATIM
                   )

  add_dependencies(tts-coverage-report tts-coverage)
  add_dependencies(tts-coverage tts-test)

  message(STATUS "[${PROJECT_NAME}] - Coverage enabled for target '${target}' via ${GCOVR_EXECUTABLE}")
endfunction()
