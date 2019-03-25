##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================

add_library ( tts
              args.cpp
              env.cpp
              exceptions.cpp
              helpers.cpp
              info.cpp
              info.cpp
              test.cpp
              type_id.cpp
              usage.cpp
            )

if( MSVC )
  set( options /std:c++latest /W3 /EHsc)
else()
  set( options -std=c++17 -Wall -Wno-missing-braces )
endif()

target_include_directories(tts
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_options( tts PRIVATE ${options} )
