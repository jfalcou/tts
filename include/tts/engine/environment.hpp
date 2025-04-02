//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

//======================================================================================================================
// Test environment
//======================================================================================================================
namespace tts::_
{
  struct env
  {
    void pass()     { test_count++; success_count++; }
    void fail()     { test_count++; failure_count++; }
    void fatal()    { test_count++; failure_count++; fatal_count++; }
    void invalid()  { test_count++; invalid_count++; }

    int report(int fails, int invalids) const
    {
      auto test_txt = test_count    > 1 ? "s"  : "";
      auto pass_txt = success_count > 1 ? "es" : "";
      auto fail_txt = failure_count > 1 ? "s"  : "";
      auto inv_txt  = invalid_count > 1 ? "s"  : "";
      auto passes   = (fails || invalids) ?  0 : test_count;

      puts("----------------------------------------------------------------");
      printf("Results: %d test%s - %d/%d success%s - %d/%d failure%s - %d/%d invalid%s\n"
            , test_count, test_txt
            , success_count, passes, pass_txt
            , failure_count, fails , fail_txt
            , invalid_count, invalids, inv_txt
            );

      if(!fails && !invalids) return test_count == success_count ? 0 : 1;
      else                    return (failure_count == fails && invalid_count == invalids) ? 0 : 1;
    }

    int test_count = 0, success_count = 0, failure_count = 0, fatal_count = 0, invalid_count = 0;
    bool fail_status = false;
  };
}

namespace tts
{
  inline _::env global_runtime      = {};
  inline bool   fatal_error_status  = false;

  //====================================================================================================================
  /**
    @brief Final tests handler customization point

    Aggregates test results and validate the whole test suite with respect to expect numbers of failures and invalids.

    @param fails    Number of expected failures.
    @param invalids Number of expected invalid tests.

    @return 0 if all tests passed and 1 otherwise.
  **/
  //====================================================================================================================
  inline int report(int fails, int invalids) { return global_runtime.report(fails,invalids); }
}
