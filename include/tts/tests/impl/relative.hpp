//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_RELATIVE_HPP_INCLUDED
#define TTS_TESTS_IMPL_RELATIVE_HPP_INCLUDED

#include <tts/tests/impl/approx.hpp>
#include <tts/tests/impl/reldist.hpp>
#include <string>

namespace tts
{
  namespace detail
  {
    struct relative_measure
    {
      template<typename T, typename U>
      double operator()(T const& data, U const& ref) const
      {
        return ::tts::reldist(data,ref);
      }

      template<typename Stream> static void to_stream(Stream& s, double v)
      {
        s.precision(2);
        s << " (" << std::fixed << v*100. << " %)\n";
      }
    };
  }

  // Perform relative distance computation and report
  template<typename R> using relative_ = approx_<detail::relative_measure, R>;

  // Simple relative_ constructor like call
  template<typename R> inline relative_<R> relative(R const& t, double n) { return {t,n/100.}; }
}

#endif
