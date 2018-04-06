##===================================================================================================
##                                 Copyright 2018 Joel FALCOU
##
##                   Distributed under the Boost Software License, Version 1.0.
##                        See accompanying file LICENSE.txt or copy at
##                            http://www.boost.org/LICENSE_1_0.txt
##===================================================================================================

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
