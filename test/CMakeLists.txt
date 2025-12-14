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
                        FILES         unit/infrastructure/bitcast.cpp
                                      unit/infrastructure/buffer.cpp
                                      unit/infrastructure/callable.cpp
                                      unit/infrastructure/display.cpp
                                      unit/infrastructure/main.cpp
                        ROOT          ${root}
                        NAME          unit.infrastructure.basic.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         unit/infrastructure/random.cpp
                        ROOT          ${root}
                        NAME          unit.infrastructure.random.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         unit/infrastructure/failure.cpp
                        ROOT          ${root}
                        NAME          unit.infrastructure.failure.cpp
                      )

##======================================================================================================================
## Test for TTS tests
##======================================================================================================================
copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  unit.infrastructure.exe
                        FILES         unit/basic/expect.cpp unit/basic/expect_not.cpp unit/basic/main.cpp
                        ROOT          ${root}
                        NAME          unit.basic.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  unit.infrastructure.exe
                        FILES         unit/precision/main.cpp
                                      unit/precision/absolute.cpp
                                      unit/precision/ieee.cpp
                                      unit/precision/relative.cpp
                                      unit/precision/ulp.cpp
                        ROOT          ${root}
                        NAME          unit.precision.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  unit.infrastructure.exe
                        FILES         unit/relation/main.cpp
                                      unit/relation/equal.cpp
                                      unit/relation/greater.cpp
                                      unit/relation/greater_equal.cpp
                                      unit/relation/less.cpp
                                      unit/relation/less_equal.cpp
                                      unit/relation/not_equal.cpp
                        ROOT          ${root}
                        NAME          unit.relation.cpp
                      )

copa_glob_unit( PATTERN "unit/tests/*.cpp"
                RELATIVE ${root} INTERFACE tts_test DEPENDENCIES unit.infrastructure.exe
              )


##======================================================================================================================
## Test for TTS Documentation
##======================================================================================================================
copa_glob_unit( PATTERN "unit/doc/*.cpp"
                RELATIVE ${root} INTERFACE tts_test DEPENDENCIES unit.infrastructure.exe
              )
