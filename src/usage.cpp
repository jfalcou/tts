//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include <iostream>

namespace tts::detail
{
  int usage(std::string const& name)
  {
    std::cout <<  "\nUsage: " << name << " [options]\n"
                  "\n"
                  "Options:\n"
                  "  --help             Display this help message\n"
                  "  --pass             Report passing tests\n"
                  "  --no-color         Disable color display\n"
                  "  --seed n           Use n as random generator seed\n"
                  "  --seed time        Use time for random generator seed\n"
                  "  --order random     Execute tests randomly\n"
                  "  --order sorted     Execute tests sorted by name\n"
                  "  --repeat n         Repeat each tests n times\n"
                  "\n";
    return 0;
  }
}
