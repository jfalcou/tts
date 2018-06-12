##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
if(NOT TTS_COMPILER_OPTIONS_INCLUDED)
  set(TTS_COMPILER_OPTIONS_INCLUDED 1)

  if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wshadow -Wextra")
  endif()

  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if("${CMAKE_CXX_FLAGS}" MATCHES "/W[1-4]")
      string(REGEX REPLACE "/W[1-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++-latest /W4")
    endif()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++-latest /W4 /DNOMINMAX")
  endif()

  if(CMAKE_CXX_FLAGS MATCHES "[^ ]")
    message(STATUS "[tts] Compilation flags: ${CMAKE_CXX_FLAGS}")
  endif()

  ## Remove /EHsc from CMAKE_CXX_FLAGS and re-add per configuration to avoid 'overriding' warnings
  if(CMAKE_CXX_FLAGS MATCHES "/EHsc")
    string(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    foreach(config Debug Release)
      string(TOUPPER ${config} config_U)
      set(CMAKE_CXX_FLAGS_${config_U} "/EHsc ${CMAKE_CXX_FLAGS_${config_U}}")
    endforeach()
  endif()

endif()
