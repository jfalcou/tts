##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================

# Extract HEAD from git
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE tts_git_head
    RESULT_VARIABLE _git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT _git_error STREQUAL "0")
    message(SEND_ERROR
        "Failed to extract TTS's current git commit for integration tests: ${_git_error}")
    return()
endif()


# Builds and tests a separate project as an tts test
function(tts_build_and_ctest project_name)
  string(REPLACE "-test" "" test_name "integration.${project_name}.exe")

  add_test(
    NAME ${test_name}
    COMMAND
    "${CMAKE_CTEST_COMMAND}"
    --verbose
    --output-on-failure
    --build-generator "${CMAKE_GENERATOR}"
    --build-and-test
      "${CMAKE_CURRENT_SOURCE_DIR}/${project_name}"
      "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
    --build-options ${ARGN}
    --test-command "${CMAKE_CTEST_COMMAND}")
endfunction()


##==================================================================================================
## Create Integration Tests
##==================================================================================================

## ===== CPM =====
tts_build_and_ctest(cpm-test
    "-Dtts_git_head=${tts_git_head}"
    "-Dtts_SOURCE_DIR=${tts_SOURCE_DIR}"
    # cpm-test internally sets tts_* configure options
)

## ===== Fetch =====
tts_build_and_ctest(fetch-test
    "-Dtts_git_head=${tts_git_head}"
    "-Dtts_SOURCE_DIR=${tts_SOURCE_DIR}"
    "-DTTS_BUILD_TEST=OFF"
)

## ===== Install =====
# install tts as its own test and for 'install-test' to find
set(TTS_ROOT "${PROJECT_BINARY_DIR}/test/integration/install")
add_test(
  NAME integration.install-tts.exe
  COMMAND
  "${CMAKE_COMMAND}"
  --install "${PROJECT_BINARY_DIR}"
  --verbose
  --prefix  "${tts_ROOT}")

tts_build_and_ctest(install-test "-Dtts_ROOT=${TTS_ROOT}")
set_tests_properties(integration.install.exe PROPERTIES DEPENDS integration.install-tts.exe)

## ==== Aggregated ====
add_custom_target(integration
  COMMAND
  "${CMAKE_CTEST_COMMAND}"
  --test-dir "${PROJECT_BINARY_DIR}"
  --output-on-failure
  -R integration\.[a-z-]+\.exe)
