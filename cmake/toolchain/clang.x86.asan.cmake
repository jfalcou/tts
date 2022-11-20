##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set(CMAKE_C_COMPILER    clang++ )
set(CMAKE_CXX_COMPILER  clang++ )
set(CMAKE_BUILD_TYPE    Debug   )

set(CMAKE_CXX_FLAGS        "-fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
