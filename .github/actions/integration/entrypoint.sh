#!/bin/sh -l
set -e

# Setup github/worspace as safe
git config --global --add safe.directory /github/workspace

# ID for various tests
INSTALL_TEST=0
FETCH_TEST=1
CPM_TEST=2

configure_and_ctest() {
  test_regex=$1

  cmake -B build -S . -G Ninja \
    -DTTS_BUILD_INTEGRATION=ON \
    -DTTS_BUILD_TEST=OFF

  # tts build not required: cmake --build build
  ctest --test-dir build --output-on-failure -R $test_regex
}

if [ $2 -eq $INSTALL_TEST ]; then
  echo "::group::Prepare TTS repository for branch " $1
  configure_and_ctest integration.install-tts.exe
  echo "::endgroup::"

  echo "::group::Test TTS via the install target"
  ctest --test-dir build --output-on-failure -R integration.install.exe
  echo "::endgroup::"
fi

if [ $2 -eq $FETCH_TEST ]; then
  echo "::group::Test TTS via FetchContent"
  configure_and_ctest integration.fetch.exe
  echo "::endgroup::"
fi

if [ $2 -eq $CPM_TEST ]; then
  echo "::group::Test TTS via CPM"
  configure_and_ctest integration.cpm.exe
  echo "::endgroup::"
fi

return 0;
