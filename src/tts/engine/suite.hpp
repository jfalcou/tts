//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

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

    int report(int fails, int invalids) const
    {
      auto test_txt = test_count    > 1 ? "tests" : "test";
      auto pass_txt = success_count > 1 ? "successes" : "success";
      auto fail_txt = failure_count > 1 ? "failures" : "failure";
      auto inv_txt  = invalid_count > 1 ? "invalids" : "invalid";
      auto passes   = (fails || invalids) ?  0 : test_count;

      std::cout << "----------------------------------------------------------------\n";
      std::cout << "Results: " << test_count << " " << test_txt << " - "
                << success_count << "/" << passes << " " << pass_txt << " - "
                << failure_count << "/" << fails << " " << fail_txt << " - "
                << invalid_count << "/" << invalids << " " << inv_txt << "\n";

      if(!fails && !invalids) return test_count == success_count ? 0 : 1;
      else                    return (failure_count == fails && invalid_count == invalids) ? 0 : 1;
    }

    int test_count    = 0,
        success_count = 0,
        failure_count = 0,
        fatal_count   = 0,
        invalid_count = 0;

    bool fail_status = false;
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
