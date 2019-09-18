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

if( MSVC )
  set( options /std:c++latest /W3 /EHsc)
else()
  set( options -std=c++17 -Wall -Wno-missing-braces -fvisibility=hidden)
endif()

target_compile_definitions(tts PRIVATE TTS_API_EXPORTS=1)

target_include_directories(tts
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_options( tts PRIVATE ${options} )
