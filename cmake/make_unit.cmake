##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================

##==================================================================================================
# Unit test Configuration
##==================================================================================================
add_library(tts_test INTERFACE)
target_compile_features ( tts_test INTERFACE  cxx_std_20 )
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( tts_test INTERFACE /W3 /EHsc)
else()
  target_compile_options( tts_test INTERFACE -Wall -Werror)
endif()

##===================================================================================================
## Process a list of source files to generate corresponding test target
##===================================================================================================
function(make_unit root)

  set(test "${root}.exe")
  add_executable(${test} ${ARGN})
  add_dependencies(unit ${test})

  set_property( TARGET ${test}
                PROPERTY RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/unit"
              )

  target_link_libraries(${test} PUBLIC tts_test)

  target_include_directories( ${test}
                              PUBLIC
                                $<INSTALL_INTERFACE:include>
                                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                              PRIVATE
                                ${tts_SOURCE_DIR}/include
                                ${PROJECT_SOURCE_DIR}/test
                            )


  if (CMAKE_CROSSCOMPILING_CMD)
    add_test( NAME ${test}
              WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
              COMMAND ${CMAKE_CROSSCOMPILING_CMD} $<TARGET_FILE:${test}> --no-color --pass
            )
  else()
    add_test( NAME ${test}
              WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
              COMMAND $<TARGET_FILE:${test}>
            )
  endif()

endfunction()
