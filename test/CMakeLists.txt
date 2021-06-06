##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(make_unit)

message(STATUS "Current configuration: ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS}")

make_unit ( "basic"         basic/main.cpp 
                            basic/expect.cpp 
                            basic/expect_not.cpp
          )

# make_unit ( "precision"     precision/main.cpp
#                             precision/absolute.cpp
#                             precision/ieee.cpp
#                             precision/relative.cpp
#                             precision/ulp.cpp
#           )

make_unit ( "relation"      relation/main.cpp
                            relation/equal.cpp
                            relation/greater.cpp
                            relation/greater_equal.cpp
                            relation/less.cpp
                            relation/less_equal.cpp
                            relation/not_equal.cpp
          )
# make_unit ( "sequence"      sequence/main.cpp
#                             sequence/absolute.cpp
#                             sequence/equal.cpp
#                             sequence/ieee.cpp
#                             sequence/relative.cpp
#                             sequence/ulp.cpp
# )

make_unit ( "infos.fail"    infos/fail.cpp    )
make_unit ( "infos.invalid" infos/invalid.cpp )
make_unit ( "infos.pass"    infos/pass.cpp    )
# make_unit ( "exceptions"    exceptions.cpp    )
# make_unit ( "failure"       failure.cpp       )
make_unit ( "display"       display.cpp       )
# make_unit ( "range"         range.cpp         )
make_unit ( "sections"      sections.cpp      )
# make_unit ( "types"         types.cpp         )
