##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
#!/bin/sh
#
# Usage:
#
#  source format.sh <root folder>
#
format_com=clang-format
n=0

for m in `find ${1} -name "*.?pp" ` ; do
        let n++
        echo "$n: Processing " "$m"
        $format_com -i "$m"
        git add "$m"
done

