##==================================================================================================
##  TTS - Tiny Test System
##  Copyright 2019 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
#!/usr/bin/env bash

g++ --version

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja unit -j 8
ctest -j 8
