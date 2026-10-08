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

namespace tts::_
{
  // Every accepted --sink=name value - also drives the "expected one of: ..." error message
  // below, so adding a new sink here is the only place that needs updating.
  inline constexpr char const* sink_names[] = {// NOSONAR - <array> costs compile time
                                               "colored",
                                               "tap",
                                               "diagnostics",
                                               "json",
                                               "junit"};
  inline constexpr std::size_t sink_count   = sizeof(sink_names) / sizeof(sink_names[ 0 ]);

  // Validates --sink=name (read from the current tts::arguments()). ok is set to false for an
  // unknown, non-empty name, in which case the returned text is ready to print as an error.
  inline ::tts::text validate_sink_name(::tts::text const& name, bool& ok)
  {
    ok = name.is_empty();
    for(auto candidate: sink_names)
      ok = ok || (name == ::tts::text {candidate});
    if(ok) return {};

    ::tts::text expected;
    for(std::size_t i = 0; i < sink_count; ++i)
      expected += ::tts::text {i ? ", %s" : "%s", sink_names[ i ]};

    return ::tts::text {
    "Unknown --sink value '%s', expected one of: %s", name.data(), expected.data()};
  }
}
