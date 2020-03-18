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
#include <tts/api.hpp>
#include <string>
#include <iomanip>

namespace tts::detail
{
  void check_precision( tts::env& runtime, location const& l, double r, double x
                      , double va, double vb
                      , std::string_view a, std::string_view b
                      , std::string_view m
                      )
  {
    if(r <= x)
      pass(runtime,l) << "Expecting: " << white << a << reset
                      << " == " << white << b << reset << " within "
                      << green << x << reset << " " << m << " and found: "
                      << green << r << reset << "\n";
    else
      fail(runtime,l) << "Expecting: " << white << a << reset
                      << " == " << white << b << reset << " within "
                      << red << x << reset <<  " " << m << " but found: "
                      << white << va << reset << " == " << white << vb << reset << " within "
                      << red << r << reset <<  " " << m << " instead.\n";
  }

  bool has_matching_size( tts::env& runtime, location const& l
                        , std::string_view a, std::string_view b
                        , std::size_t s1, std::size_t s2
                        )
  {
    bool ok = (s1==s2);

    if(!ok)
    {
      fail(runtime,l) << white << a << reset << " and "
                      << white << b << red << " sizes mismatch" << reset << ".\n";
    }

    return ok;
  }

  void report_all_errors( env& runtime, location const& l, std::vector<error> const& err
                        , double x, std::string_view a, std::string_view b, std::string_view m
                        )
  {
    if(err.empty())
    {
      pass(runtime,l) << "Expecting: " << white << a << reset
                      << " == " << white << b << reset << " within "
                      << green << x << reset << " " << m << "\n";
    }
    else
    {
      fail(runtime,l) << "Expecting: "  << white << a << reset
                      << " == " << white << b << reset << " within "
                      << red << x << reset << " " << m << " but found:\n{\n";

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
