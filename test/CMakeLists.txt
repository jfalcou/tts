##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
copa_setup_test_targets()
set(root "${CMAKE_SOURCE_DIR}/test")

##======================================================================================================================
## Derivative tests
##======================================================================================================================
copa_glob_unit( PATTERN "others/*.cpp"
                QUIET RELATIVE ${root} INTERFACE tts_test DEPENDENCIES tts-standalone
              )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         basic/expect.cpp basic/expect_not.cpp basic/main.cpp
                        ROOT          ${root}
                        NAME          basic.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         precision/main.cpp
                                      precision/absolute.cpp
                                      precision/ieee.cpp
                                      precision/relative.cpp
                                      precision/ulp.cpp
                        ROOT          ${root}
                        NAME          precision.cpp
                      )

copa_make_single_unit ( INTERFACE     tts_test
                        DEPENDENCIES  tts-standalone
                        FILES         relation/main.cpp
                                      relation/equal.cpp
                                      relation/greater.cpp
                                      relation/greater_equal.cpp
                                      relation/less.cpp
                                      relation/less_equal.cpp
                                      relation/not_equal.cpp
                        ROOT          ${root}
                        NAME          relation.cpp
                      )
