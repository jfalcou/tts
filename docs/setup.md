# Setup

## Retrieving the source

**TTS** is available on GitHub and can be retrieved via the following command:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ git clone https://github.com/jfalcou/tts.git
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once retrieved, you should have a `tts` folder which contains the whole source code.

## Installation

Create a `build` directory here and enter it. Once in the `build` directory,
you can use  **CMake** to generate the build system for **TTS**. We recommend using
Ninja but any build system is fine.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ mkdir build
 $ cd build
 $ cmake .. -G Ninja
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once **CMake** completes, you can use the `install` target to build and install **TTS**.
By default, the library will be installed in the `/usr/local` directory, thus requiring
root privileges.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ sudo ninja install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can select an alternative installation path by specifying the `CMAKE_INSTALL_PREFIX`
option at configuration time.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=path/to/install
 $ ninja install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once completed, the following files and folders are installed:

- `$CMAKE_INSTALL_PREFIX/include/tts/tts.hpp`

Once installed, **TTS** is usable directly by providing the path to its installed files.

## CMake `FetchContent`

You can also use CMake FetchContent operation and use the `tts::tts` library
target that our CMake exports.

``` cmake
##==================================================================================================
## Your project setup
##==================================================================================================
cmake_minimum_required(VERSION 3.2)
project(test_tts LANGUAGES CXX)

include(FetchContent)

##==================================================================================================
## Fetch tts and disable its test targets
##==================================================================================================
set(tts_BUILD_TEST    OFF CACHE INTERNAL "OFF")
FetchContent_Declare( tts
                      GIT_REPOSITORY https://github.com/jfalcou/tts.git
                      GIT_TAG main
                    )

FetchContent_MakeAvailable(tts)

##==================================================================================================
## Using tts
##==================================================================================================
add_executable( my_app my_app.cpp)
target_link_libraries(my_app tts::tts)
```

## Standalone setup

**TTS** also provides a standalone file that can be used to be vendored in your own project without
having to deal with **TTS** as a dependecy.

Simply use `wget` to fetch the latest version and place it where you want:

```bash
wget https://raw.githubusercontent.com/jfalcou/tts/main/stanalone/tts/tts.hpp
```

Use **TTS** by just compiling your code with the include path pointing to the location of this single file.
