//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/engine/env.hpp>
#include <tts/tests/infos.hpp>
#include <tts/detail/color.hpp>
#include <tts/detail/precision.hpp>
#include <tts/tests/basic.hpp>
#include <string>
#include <tts/api.hpp>

namespace tts::detail
{
  void check_precision( tts::env& runtime, location const& l, double r, double x
                      , std::string_view a, std::string_view b, std::string_view m
                      )
  {
    if(r <= x)
      pass(runtime,l) << "Expecting: " << white_(a) << " == " << white_(b) << " within "
                      << green_(x) << " " << m << "\n";
    else
      fail(runtime,l) << "Expecting: " << white_(a) << " == " << white_(b) << " within "
                      << red_(x) <<  " " << m << " but found: "
                      << red_(r) <<  " " << m << " instead.\n";
  }

  bool has_matching_size( tts::env& runtime, location const& l
                        , std::string_view a, std::string_view b
                        , std::size_t s1, std::size_t s2
                        )
  {
    bool ok = (s1==s2);

    if(!ok)
    {
      fail(runtime,l) << white_(a) << " and " << white_(b) << red_(" sizes mismatch") << ".\n";
    }

    return ok;
  }

  void report_all_errors( env& runtime, location const& l, std::vector<error> const& err
                        , double x, std::string_view a, std::string_view b, std::string_view m
                        )
  {
    if(err.empty())
    {
      pass(runtime,l) << "Expecting: " << white_(a) << " == " << white_(b) << " within "
                      << green_(x) << " " << m << "\n";
    }
    else
    {
      fail(runtime,l) << "Expecting: "  << white_(a) << " == " << white_(b) << " within "
                      << red_(x) << " " << m << " but found:\n{\n";

      for(auto& e : err)
      {
        runtime.stream()  << "  [" << to_string(e.idx) << "]: "
                        << e.data << " vs " << e.ref  << " (" + to_string(e.value)
                        <<  " " << m << ")\n";
      }

      runtime.stream() << "}\n";
    }
  }
}
