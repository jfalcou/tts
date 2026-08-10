//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>
#include <array>

namespace tts::_
{
  // Every accepted --sink=name value - also drives the "expected one of: ..." error message
  // below, so adding a new sink here is the only place that needs updating.
  inline constexpr std::array<char const*, 4> sink_names {"colored", "tap", "diagnostics", "json"};

  // Validates --sink=name (read from the current tts::arguments()). ok is set to false for an
  // unknown, non-empty name, in which case the returned text is ready to print as an error.
  inline ::tts::text validate_sink_name(::tts::text const& name, bool& ok)
  {
    ok = name.is_empty();
    for(auto candidate: sink_names)
      ok = ok || (name == ::tts::text {candidate});
    if(ok) return {};

    ::tts::text expected;
    for(std::size_t i = 0; i < sink_names.size(); ++i)
      expected += ::tts::text {i ? ", %s" : "%s", sink_names[ i ]};

    return ::tts::text {
    "Unknown --sink value '%s', expected one of: %s", name.data(), expected.data()};
  }
}
