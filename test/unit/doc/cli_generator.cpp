//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#include <tts/tts.hpp>

struct cli_generator
{
  cli_generator()
  {
    value_ = ::tts::arguments().value( 1.f, "--gen-value" );
  }

  template<typename Idx, typename Count> float operator()(Idx i, Count c)
  {
    return (value_ * i)/c;
  }

  friend tts::text to_text( cli_generator const& p )
  {
    return "cli_generator(" + tts::as_text(p.value_) + ")";
  }

  private:
  float value_;
};

float f (float x) { return x; }
float g(float x)  { return x + x*1e-7f; }

TTS_CASE( "Test stateless range check" )
{
  TTS_ULP_RANGE_CHECK ( cli_generator{}
                    , float, float, f, g
                    , 2.
                    );
};
//! [snippet]