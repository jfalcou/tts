//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <tts/tts.hpp>

namespace tts::test
{
  // Keeps what TTS reported so an abort handler can check it: the run dies before main sees it.
  struct recording_sink : tts::output_sink
  {
    void write(tts::text const& t) override
    {
      seen += t;
    }

    void suite_aborted() override
    {
      aborted = true;
    }

    bool says(char const* what) const
    {
      return std::strstr(seen.data(), what) != nullptr; // NOSONAR - contains() is C++23
    }

    tts::text seen    = {};
    bool      aborted = false;
  };

  // Runs driver under a recording sink and prints marker when verdict accepts what was reported.
  // The run exits 1 whatever happens, so ctest reads the marker instead of the code.
  template<typename Driver, typename Verdict>
  int run_and_report(int          argc,
                     char const** argv,
                     char const*  marker,
                     Driver       driver,
                     Verdict      verdict)
  {
    ::tts::initialize(argc, argv);

    // main is still on the stack when the handler runs, so the capture stays valid.
    static recording_sink sink;

    ::tts::set_abort_handler(
    [ marker, verdict ](int reason)
    {
      if(verdict(sink, reason)) std::puts(marker);
    });

    ::tts::scoped_sink scope(sink);
    driver(argc, argv);

    return 1; // the case under test must not let us reach this
  }
}
