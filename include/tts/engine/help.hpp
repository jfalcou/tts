//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <iostream>

namespace tts
{
  //================================================================================================
  // Options on the command line
  //================================================================================================
  inline int usage(const char* name)
  {
    std::cout << "TTS Unit Tests Driver\n";
    std::cout << "Usage: " << name  << " [OPTION...]\n";
    std::cout << "\nFlags:\n";
    std::cout << "  -h, --help        Display this help message\n";
    std::cout << "  -x, --hex         Print the floating results in hexfloat mode\n";
    std::cout << "  -s, --scientific  Print the floating results in scientific mode\n";
    std::cout << "\nParameters:\n";
    std::cout << "  --precision=arg   Set the precision for displaying floating pint values\n";
    std::cout << "  --seed=arg        Set the PRNG seeds (default is time-based)\n";
    std::cout << "\nRange specifics Parameters:\n";
    std::cout << "  --block=arg       Set size of range checks samples (min. 32)\n";
    std::cout << "  --loop=arg        Repeat each range checks arg times\n";
    std::cout << "  --ulpmax=arg      Set global failure ulp threshold for range tests (default is 2.0)\n";
    std::cout << "  --valmax=arg      Set maximal value for range tests (default is code)\n";
    std::cout << "  --valmin=arg      Set minimal value for range tests (default is code)\n";
    std::cout << std::endl;
    return 0;
  }
}
