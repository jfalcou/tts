##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================

add_library ( tts SHARED
              args.cpp
              env.cpp
              exceptions.cpp
              helpers.cpp
              info.cpp
              precision.cpp
              relation.cpp
              test.cpp
              type_id.cpp
            )

add_library(tts::tts ALIAS tts)

if( MSVC )
target_compile_options  ( tts PRIVATE /W3 /EHsc)
else()
target_compile_options  ( tts PRIVATE -Wall -fvisibility=hidden)
endif()

set_target_properties     ( tts PROPERTIES  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/unit" )
target_compile_features   ( tts PRIVATE     cxx_std_17                                            )
target_compile_definitions( tts PRIVATE     TTS_API_EXPORTS=1                                     )

target_include_directories( tts
                            PUBLIC
                                $<INSTALL_INTERFACE:include>
                                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            PRIVATE
                                ${PROJECT_SOURCE_DIR}/src
                          )

target_compile_options( tts PRIVATE ${options} )
