##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
copa_setup_test_targets()
set(root "${CMAKE_SOURCE_DIR}/test")

##======================================================================================================================
## Test for TTS infrastructure code
##======================================================================================================================
copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         infrastructure/bitcast.cpp
                                      infrastructure/buffer.cpp
                                      infrastructure/callable.cpp
                                      infrastructure/display.cpp
                                      #infrastructure/text.cpp
                                      infrastructure/main.cpp
                        ROOT          ${root}
                        NAME          infrastructure.basic.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         infrastructure/failure.cpp
                        ROOT          ${root}
                        NAME          infrastructure.failure.cpp
                      )

##======================================================================================================================
## Test for TTS tests
##======================================================================================================================
copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  infrastructure.exe
                        FILES         basic/expect.cpp basic/expect_not.cpp basic/main.cpp
                        ROOT          ${root}
                        NAME          tests.basic.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  infrastructure.exe
                        FILES         precision/main.cpp
                                      precision/absolute.cpp
                                      precision/ieee.cpp
                                      precision/relative.cpp
                                      precision/ulp.cpp
                        ROOT          ${root}
                        NAME          tests.precision.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  infrastructure.exe
                        FILES         relation/main.cpp
                                      relation/equal.cpp
                                      relation/greater.cpp
                                      relation/greater_equal.cpp
                                      relation/less.cpp
                                      relation/less_equal.cpp
                                      relation/not_equal.cpp
                        ROOT          ${root}
                        NAME          tests.relation.cpp
                      )

copa_glob_unit( PATTERN "tests/*.cpp"
                RELATIVE ${root} INTERFACE tts_test DEPENDENCIES infrastructure.exe
              )

