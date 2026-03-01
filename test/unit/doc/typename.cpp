//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet1]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <vector>

TTS_CASE_TPL("Check typename_ utility", int, float, std::vector<double>)
<typename T>(tts::type<T>)
{
  TTS_PASS("Type name: '" + tts::as_text(tts::typename_<T>) + "'");
};
//! [snippet1]

#undef TTS_MAIN

//! [snippet2]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <vector>

TTS_CASE_WITH("Check name() utility", (int, float, std::vector<double>), tts::value {37})
(auto const& args)
{
  TTS_PASS("tts::name(args) = '" + tts::as_text(tts::name(args)) + "'");
};
//! [snippet2]
