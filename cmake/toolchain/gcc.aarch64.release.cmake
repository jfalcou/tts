##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2020-2021 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER    aarch64-linux-gnu-gcc-11  )
set(CMAKE_CXX_COMPILER  aarch64-linux-gnu-g++-11  )
set(CMAKE_BUILD_TYPE    Release                   )

set(CMAKE_CROSSCOMPILING_CMD qemu-aarch64)
