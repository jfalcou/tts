//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/color.hpp>
#include <iostream>

namespace tts
{
  //================================================================================================
  // Options on the command line
  //================================================================================================
  inline int usage(const char* name)
  {
    std::cout << "TTS Unit Tests Driver\n";
    std::cout << "Usage:\n";
    std::cout << name << " [OPTION...]\n";
    std::cout << "\nFlags:\n";
    std::cout << "  -h, --help        Display this help message\n";
    std::cout << "  -n, --no-color    Disable colored output\n";
    std::cout << "  -p, --pass        Report passing tests\n";
    std::cout << "  -x, --hex         Print the floating results in hexfloat mode\n";
    std::cout << "\nParameters:\n";
    std::cout << "  -f, --filter=str  Only run tests with `str` in their description\n";
    std::cout << "  -r, --repeat=arg  Repeat each tests arg times\n";
    std::cout << "  -l, --loop=arg    Repeat each range checks arg times\n";
    std::cout << "  -b, --block=arg   Set size of range checks samples (min. 32)\n";
    std::cout << "  -u, --ulpmax=arg  Set global failure ulp threshold for range tests (default is 2.0)\n";
    std::cout << "  -v, --valmin=arg  Set minimal value for range tests (default is code)\n";
    std::cout << "  -w, --valmax=arg  Set maximal value for range tests (default is code)\n";
    std::cout << "  -s, --seed=arg    Set the PRNG seeds (default is time-based)\n";
    std::cout << std::endl;

    return 0;
  }
}
