//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main

//! [snippet1]
#define TTS_MAIN                   // No need for main()
#include <tts/tts.hpp>

struct payload
{
  double           d;
  unsigned int     i, j;
  constexpr bool   operator==(payload const&) const = default;

  friend tts::text to_text(payload const& p)
  {
    return "payload(" + tts::as_text(p.d) + ")[" + tts::as_text(p.i) + "][" + tts::as_text(p.j) +
           "]";
  }
};

TTS_CASE("Display a type with custom to_text")
{
  payload p {1.5, 0xAABBCCDD, 0x11223344};
  payload q {1.5, 0xFFEEDDCC, 0x99887766};

  TTS_EQUAL(p, q);
};
//! [snippet1]

#undef TTS_MAIN

//! [snippet2]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

namespace sample
{
  struct payload
  {
    double         d;
    unsigned int   i, j;
    constexpr bool operator==(payload const&) const = default;
  };

  tts::text to_text(payload const& p) { return tts::text("payload(%f)[%d][%d]", p.d, p.i, p.j); }
}

TTS_CASE("Display another type with custom to_text")
{
  sample::payload p {1.5, 0xAABBCCDD, 0x11223344};
  sample::payload q {1.5, 0xFFEEDDCC, 0x99887766};

  TTS_EQUAL(p, q);
};
//! [snippet2]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);
  fail_main(argc, argv);
  return ::tts::report(2, 0);
}
