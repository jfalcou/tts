##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(compilers)
include(add_target_parent)

##===================================================================================================
## Process a list of source files to generate corresponding test target
##===================================================================================================
function(make_unit root)

  foreach(file ${ARGN})

    string(REPLACE ".cpp" ".unit" base ${file})
    set(test "${root}.${base}")

    add_executable(${test} ${file})
    set_property( TARGET ${test}
                  PROPERTY RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/unit"
                )

    add_test( NAME ${test}
              WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/unit"
              COMMAND $<TARGET_FILE:${test}>
            )

    set_target_properties ( ${test} PROPERTIES
                            EXCLUDE_FROM_DEFAULT_BUILD TRUE
                            EXCLUDE_FROM_ALL TRUE
                          )

    add_target_parent(${test})

    add_dependencies(unit ${test})

  endforeach()

endfunction()
