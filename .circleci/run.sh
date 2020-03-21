##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2019 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
#!/usr/bin/env bash

$1 --version

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$2 -G Ninja -DCMAKE_CXX_COMPILER=$1
ninja unit -j 8
ctest  -j 8
