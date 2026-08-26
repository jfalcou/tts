##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
## Drops the counters left by a previous run. Run via 'cmake -DCOVERAGE_BUILD_DIR=<dir> -P'.
##
## Counters accumulate across runs, and a rebuild leaves them stamped for object files that no
## longer match, which gcov reports as an error rather than ignoring.
##======================================================================================================================
file(GLOB_RECURSE TTS_STALE_COUNTERS "${COVERAGE_BUILD_DIR}/*.gcda")

if(TTS_STALE_COUNTERS)
  file(REMOVE ${TTS_STALE_COUNTERS})
endif()
