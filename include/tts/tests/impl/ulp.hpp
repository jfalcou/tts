//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_ULP_HPP_INCLUDED
#define TTS_TESTS_IMPL_ULP_HPP_INCLUDED

#include <string>
#include <tts/tests/impl/approx.hpp>
#include <tts/tests/impl/ulpdist.hpp>

namespace tts
{
  namespace detail
  {
    struct ulp_measure
    {
      template<typename T, typename U> double operator()(T const &data, U const &ref) const
      {
        return tts::ulpdist(data, ref);
      }

      template<typename Stream> static void to_stream(Stream &s, double v)
      {
        s << " (" << v << " ULPs)";
      }
    };
  }
  // Perform ULP distance computation and report
  template<typename R> using ulp_ = approx_<detail::ulp_measure, R>;

  // Simple ulp_ constructor like call
  template<typename R> inline ulp_<R> ulp(R const &t, double n) { return {t, n}; }
}

#endif
