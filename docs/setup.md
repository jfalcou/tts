# Setup

## Retrieving the source

**TTS** is available on GitHub and can be retrieved via the following command:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ git clone -b main https://github.com/jfalcou/tts.git
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once retrieved, you should have a `tts` folder which contains the whole source code.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
 $ cd tts
 $ ls
   appveyor.yml  build  cmake  CMakeLists.txt  CTestConfig.cmake  doc
   docs  format.sh  include  LICENSE.md  README.md  src  test
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you want to develop new features for **TTS**, you can also checkout the `develop` version.

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
- `$CMAKE_INSTALL_PREFIX/include/tts/ranges.hpp`

Once installed, **TTS** is usable directly by providing the path to its installed files.
