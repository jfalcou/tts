//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_ABSOLUTE_HPP_INCLUDED
#define TTS_TESTS_IMPL_ABSOLUTE_HPP_INCLUDED

#include <tts/tests/impl/approx.hpp>
#include <tts/tests/impl/absdist.hpp>
#include <string>

namespace tts
{
  namespace detail
  {
    struct absolute_measure
    {
      template<typename T, typename U>
      double operator()(T const& data, U const& ref) const
      {
        return ::tts::absdist(data,ref);
      }

      template<typename Stream> static void to_stream(Stream& s, double v)
      {
        s.precision(5);
        s << " (" <<  v << ")\n";
      }
    };
  }

  // Perform absolute distance computation and report
  template<typename R> using absolute_ = approx_<detail::absolute_measure, R>;

  // Simple absolute_ constructor like call
  template<typename R> inline absolute_<R> absolute(R const& t, double n) { return {t,n}; }
}

#endif
