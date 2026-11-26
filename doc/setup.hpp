#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  setup  Setup

  @tableofcontents

  @section setup-source Install from the source

  Code source of **TTS** is available on GitHub and can be retrieved via the following command:

  @code
  $ git clone https://github.com/jfalcou/tts.git
  @endcode

  Once retrieved, you should have a `tts` folder which contains the whole source code.

  Create a `build` directory here and enter it. Once in the `build` directory,
  you can use  **CMake** to generate the build system for **TTS**. We recommend using
  Ninja but any build system is fine.

  @code
  $ mkdir build
  $ cd build
  $ cmake .. -G Ninja
  @endcode

  Once **CMake** completes, you can use the `install` target to build and install **TTS**.
  By default, the library will be installed in the `/usr/local` directory, thus requiring
  root privileges.

  @code
  $ sudo ninja install
  @endcode

  You can select an alternative installation path by specifying the `CMAKE_INSTALL_PREFIX`
  option at configuration time.

  @code
  $ cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=path/to/install
  $ ninja install
  @endcode

  Once installed, **TTS** is usable directly by providing the path to its installed files.

  @section setup-standalone Standalone setup

  You can also use **TTS** via a single standalone file that can be vendored in your own project without
  having to deal with **TTS** as a dependency.

  Simply use `wget` to fetch the latest version and place it where you want:

  @code
  wget https://raw.githubusercontent.com/jfalcou/tts/refs/heads/standalone/tts.hpp
  @endcode

  Use **TTS** by just compiling your code with the include path pointing to the location of this single file.

  The standalone branch also holds `benchmarks.hpp`, the benchmark utilities (see
  @ref setup-benchmarks), which includes `<tts/tts.hpp>`. Put both files in a `tts` directory,
  point the include path to its parent, and include `<tts/benchmarks.hpp>` in a benchmark, with
  `nanobench.h` v4.6.0 on the include path as well:

  @code
  wget -P tts https://raw.githubusercontent.com/jfalcou/tts/refs/heads/standalone/tts.hpp
  wget -P tts https://raw.githubusercontent.com/jfalcou/tts/refs/heads/standalone/benchmarks.hpp
  @endcode

  @section setup-fetchcontent CMake FetchContent

  You can also use CMake FetchContent operation and use the `tts::tts` library target that our CMake exports.

  @code{cmake}
  ##==================================================================================================
  ## Your project setup
  ##==================================================================================================
  cmake_minimum_required(VERSION 3.22)
  project(tts-fetch LANGUAGES CXX)

  include(FetchContent)
  FetchContent_Declare(tts GIT_REPOSITORY "https://github.com/jfalcou/tts.git")
  FetchContent_MakeAvailable(tts)

  add_executable(test_tts ../main.cpp)
  target_link_libraries(test_tts PUBLIC tts::tts)
  @endcode

  @section setup-cpm Setup with CPM

  The **TTS** library can be setup using [CPM](https://github.com/cpm-cmake/CPM.cmake):

  @code{cmake}
  ##==================================================================================================
  ## Your project setup
  ##==================================================================================================
  cmake_minimum_required(VERSION 3.18)
  project(tts-cpm LANGUAGES CXX)

  # Setup CPM - See https://github.com/cpm-cmake/CPM.cmake#adding-cpm
  include(cpm.cmake)

  CPMAddPackage ( NAME tts
                  GIT_REPOSITORY "https://github.com/jfalcou/tts.git"
                  OPTIONS "TTS_BUILD_TEST OFF"
                )

  add_executable(test_tts ../main.cpp)
  target_link_libraries(test_tts PUBLIC tts::tts)
  @endcode

  @section setup-benchmarks Benchmark utilities

  The CMake option `TTS_BENCHMARKS` is `OFF` by default. When it is enabled, **TTS** defines the
  target `tts::benchmarks` for the benchmark utilities (see @ref tools-benchmarks), which links
  `tts::tts` and adds the include path of `nanobench.h`. For that target, **TTS** fetches
  [nanobench](https://nanobench.ankerl.com) v4.6.0, or finds `nanobench.h` in the environment under
  `TTS_EXTERNAL_DEPENDENCIES`. With CPM, the option goes in the `OPTIONS` of `CPMAddPackage`:

  @code{cmake}
  cmake_minimum_required(VERSION 3.18)
  project(tts-bench LANGUAGES CXX)

  include(cpm.cmake)

  CPMAddPackage ( NAME tts
                  GIT_REPOSITORY "https://github.com/jfalcou/tts.git"
                  OPTIONS "TTS_BUILD_TEST OFF" "TTS_BENCHMARKS ON"
                )

  add_executable(my_bench my_bench.cpp)
  target_link_libraries(my_bench PRIVATE tts::benchmarks)
  @endcode

  - With FetchContent, `set(TTS_BENCHMARKS ON)` comes before `FetchContent_MakeAvailable(tts)`.
  - An installed **TTS** provides no `tts::benchmarks` target, so a project using an installation
    puts `nanobench.h` v4.6.0 on its include path itself.
  - With the standalone files, a benchmark includes `<tts/benchmarks.hpp>` from a `tts` directory
    that holds both files (see @ref setup-standalone), and `nanobench.h` v4.6.0 goes on its
    include path.
**/
//==================================================================================================
