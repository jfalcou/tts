//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/engine/test.hpp>
#include <tts/tools/color.hpp>
#include <cstddef>
#include <iostream>

//==================================================================================================
// Test environment
//==================================================================================================
namespace tts::detail
{
  struct env
  {
    void pass()     { test_count++; success_count++; }
    void fail()     { test_count++; failure_count++; }
    void fatal()    { test_count++; failure_count++; fatal_count++; }
    void invalid()  { test_count++; invalid_count++; }

    int report(std::ptrdiff_t fails, std::ptrdiff_t invalids) const
    {
      auto test_txt = test_count    > 1 ? "tests" : "test";
      auto pass_txt = success_count > 1 ? "successes" : "success";
      auto fail_txt = failure_count > 1 ? "failures" : "failure";
      auto inv_txt  = invalid_count > 1 ? "invalids" : "invalid";

      std::cout << reset << std::string(80, '-') << std::endl;
      std::cout << bold << "Results: "
                << test_count << " "  << test_txt
                << " - "      << success_count   << "/"   << test_count << " "
                << green      << pass_txt       << reset  << bold
                << " - "      << failure_count  << "/"    << fails << " "
                << red        << fail_txt       << reset  << bold
                << " - "      << invalid_count  << "/"    << invalids << " " << bold
                << magenta    << inv_txt        << reset
                << std::endl;

      if(!fails && !invalids) return test_count == success_count ? 0 : 1;
      else                    return (failure_count == fails && invalid_count == invalids) ? 0 : 1;
    }

    int test_count    = 0,
        success_count = 0,
        failure_count = 0,
        fatal_count   = 0,
        invalid_count = 0;
  };
}

namespace tts
{
  inline ::tts::detail::env global_runtime;

  inline int report(std::ptrdiff_t fails, std::ptrdiff_t invalids)
  {
    return global_runtime.report(fails,invalids);
  }
}
