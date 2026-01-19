//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main

//! [snippet]
#define TTS_MAIN                   // No need for main()
#include <tts/tts.hpp>

struct payload
{
  double         d;
  unsigned int   i, j;
  constexpr bool operator==(payload const&) const = default;
};

TTS_CASE("Display an unknown type")
{
  payload p {1.5, 0xAABBCCDD, 0x11223344};
  payload q {1.5, 0xFFEEDDCC, 0x99887766};

  TTS_EQUAL(p, q);
};
//! [snippet]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);
  fail_main(argc, argv);
  return ::tts::report(1, 0);
}
