//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_PRECISION_HPP_INCLUDED
#define TTS_DETAIL_PRECISION_HPP_INCLUDED

#include <tts/api.hpp>
#include <string>
#include <vector>
#include <tts/detail/rt_helpers.hpp>

namespace tts
{
  struct env;

  struct error
  {
    double         value;
    std::string    data, ref;
    std::ptrdiff_t idx;
  };

  namespace detail
  {
    struct location;

    TTS_API void check_precision(env &,
                         location const &,
                         double,
                         double,
                         std::string const&,
                         std::string const&,
                         std::string_view,
                         std::string_view,
                         std::string_view);

    TTS_API bool has_matching_size(env &,
                           location const &,
                           std::string_view,
                           std::string_view,
                           std::size_t,
                           std::size_t);

    TTS_API void report_all_errors(env &,
                           location const &,
                           std::vector<error> const &,
                           double,
                           std::string_view,
                           std::string_view,
                           std::string_view);

    template<typename Values, typename Refs, typename Check>
    std::vector<error> all_check(Values const &v, Refs const &r, double target, Check fn)
    {
      std::vector<tts::error> errors;
      std::ptrdiff_t          idx = 0;

      auto br = tts::detail::begin(r);
      auto bv = tts::detail::begin(v);
      auto ev = tts::detail::end(v);

      while(bv != ev)
      {
        auto m = fn(*bv, *br);

        if(m > target)
          errors.push_back({m, tts::detail::to_string(*br), tts::detail::to_string(*bv), idx});

        idx++;
        br++;
        bv++;
      }

      return errors;
    }
  }
}

#endif
