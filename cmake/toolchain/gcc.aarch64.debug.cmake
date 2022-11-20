##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set(CMAKE_SYSTEM_NAME Linux     )
set(CMAKE_SYSTEM_PROCESSOR arm  )

set(CMAKE_C_COMPILER    aarch64-linux-gnu-gcc-11  )
set(CMAKE_CXX_COMPILER  aarch64-linux-gnu-g++-11  )
set(CMAKE_BUILD_TYPE    Debug                     )

set(CMAKE_CROSSCOMPILING_CMD qemu-aarch64)
