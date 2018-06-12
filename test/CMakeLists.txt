##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2018 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(make_unit)

set ( SOURCES
      absolute.cpp
      all_absolute.cpp
      all_relative.cpp
      all_ulp.cpp
      exceptions.cpp
      expect.cpp
      failure.cpp
      invalid.cpp
      pass.cpp
      relation.cpp
      relative.cpp
      types.cpp
      ulp.cpp
    )

make_unit( "tts" ${SOURCES})
