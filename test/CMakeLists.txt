##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2019 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(make_unit)

set ( SOURCES
      basic/expect.cpp
      basic/expect_not.cpp
      exceptions.cpp
      failure.cpp
      infos/fail.cpp
      infos/invalid.cpp
      infos/pass.cpp
      precision/absolute.cpp
      precision/ieee.cpp
      precision/relative.cpp
      precision/ulp.cpp
      relation/equal.cpp
      relation/greater.cpp
      relation/greater_equal.cpp
      relation/less.cpp
      relation/less_equal.cpp
      relation/not_equal.cpp
      sections.cpp
      sequence/absolute.cpp
      sequence/equal.cpp
      sequence/ieee.cpp
      sequence/relative.cpp
      sequence/ulp.cpp
      types.cpp
    )

make_unit( "tts" ${SOURCES})
