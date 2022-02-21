##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set(CMAKE_SYSTEM_NAME Linux         )
set(CMAKE_SYSTEM_PROCESSOR powerpc  )

set(CMAKE_C_COMPILER    powerpc-linux-gnu-gcc-11  )
set(CMAKE_CXX_COMPILER  powerpc-linux-gnu-g++-11  )
set(CMAKE_BUILD_TYPE    Debug                     )
set(CMAKE_CXX_FLAGS     "-static"                 )

set(CMAKE_CROSSCOMPILING_CMD qemu-ppc)
