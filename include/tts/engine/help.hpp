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
  /*
  inline std::ostream& reset  (std::ostream &stream) { return detail::modifiy_stream(stream, -1); }
  inline std::ostream& grey   (std::ostream &stream) { return detail::modifiy_stream(stream,  0); }
  inline std::ostream& blue   (std::ostream &stream) { return detail::modifiy_stream(stream,  1); }
  inline std::ostream& green  (std::ostream &stream) { return detail::modifiy_stream(stream,  2); }
  inline std::ostream& cyan   (std::ostream &stream) { return detail::modifiy_stream(stream,  3); }
  inline std::ostream& red    (std::ostream &stream) { return detail::modifiy_stream(stream,  4); }
  inline std::ostream& magenta(std::ostream &stream) { return detail::modifiy_stream(stream,  5); }
  inline std::ostream& yellow (std::ostream &stream) { return detail::modifiy_stream(stream,  6); }
  inline std::ostream& white  (std::ostream &stream) { return detail::modifiy_stream(stream,  7); }
  inline std::ostream& bold   (std::ostream &stream) { return detail::modifiy_stream(stream,  8); }
  */
  //================================================================================================
  // Options on the command line
  //================================================================================================
  inline int usage(const char* name)
  {
    std::cout << bold << cyan << "TTS Unit Tests Driver\n" << reset;
    std::cout << bold << white << "Usage: " << name << grey << " [OPTION...]\n" << reset;
    std::cout << bold << yellow << "\nFlags:\n" << reset;
    std::cout << green << "  -h, --help        " << reset
              << bold << white << "Display this help message\n" << reset;
    std::cout << green << "  -n, --no-color    " << reset
              << bold << white << "Disable colored output\n" << reset;
    std::cout << green << "  -p, --pass        " << reset
              << bold << white << "Report passing tests\n" << reset;
    std::cout << green << "  -x, --hex         " << reset
              << bold << white << "Print the floating results in hexfloat mode\n" << reset;
    std::cout << green << "  -s, --scientific  " << reset
              << bold << white << "Print the floating results in scientific mode\n" << reset;
    std::cout << bold << yellow << "\nParameters:\n" << reset;
    std::cout << green << "  --filter=str      " << reset
              << bold << white << "Only run tests with `str` in their description\n" << reset;
    std::cout << green << "  --precision=arg   " << reset
              << bold << white << "Set the precision for displaying floating pint values\n" << reset;
    std::cout << green << "  --repeat=arg      " << reset
              << bold << white << "Repeat each tests arg times\n" << reset;
    std::cout << green << "  --seed=arg        " << reset
              << bold << white << "Set the PRNG seeds (default is time-based)\n";
    std::cout << bold << yellow << "\nRange specifics Parameters:\n" << reset;
    std::cout << green << "  --block=arg       " << reset
              << bold << white << "Set size of range checks samples (min. 32)\n" << reset;
    std::cout << green << "  --loop=arg        " << reset
              << bold << white << "Repeat each range checks arg times\n" << reset;
    std::cout << green << "  --ulpmax=arg      " << reset
              << bold << white << "Set global failure ulp threshold for range tests (default is 2.0)\n" << reset;
    std::cout << green << "  --valmax=arg      " << reset
              << bold << white << "Set maximal value for range tests (default is code)\n" << reset;
    std::cout << green << "  --valmin=arg      " << reset
              << bold << white << "Set minimal value for range tests (default is code)\n" << reset;
    std::cout << std::endl;

    return 0;
  }
}
