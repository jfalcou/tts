##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(add_target_parent)

##===================================================================================================
## Process a list of source files to generate corresponding test target
##===================================================================================================
function(make_unit root)

  if( MSVC )
    set( options /std:c++latest /W3 /EHsc)
  else()
    set( options -std=c++20 -Wall)
  endif()

  foreach(file ${ARGN})

    string(REPLACE ".cpp" ".unit" base ${file})
    string(REPLACE "/"    "." base ${base})
    string(REPLACE "\\"   "." base ${base})
    set(test "${root}.${base}")

    add_executable(${test} ${file})
    target_compile_options  ( ${test} PRIVATE ${options} )

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

    target_include_directories( ${test}
                                PUBLIC
                                    $<INSTALL_INTERFACE:include>
                                    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                PRIVATE
                                    ${PROJECT_SOURCE_DIR}/src
                              )

    add_target_parent(${test})
    add_dependencies(unit ${test})

  endforeach()

endfunction()
