##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Compiler options
##======================================================================================================================
add_library(tts_test INTERFACE)

target_compile_features ( tts_test INTERFACE  cxx_std_20 )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options( tts_test INTERFACE /W3 /EHsc /WX)
  else()
    target_compile_options( tts_test INTERFACE -Werror -Wall -Wshadow -Wextra -Wunused-variable -Wdocumentation)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( tts_test INTERFACE /W3 /WX /EHsc /Zc:preprocessor)
else()
  target_compile_options( tts_test INTERFACE -Werror -Wall -Wshadow -Wextra -Wunused-variable)
endif()

target_include_directories( tts_test INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )
