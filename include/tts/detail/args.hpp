//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_COMMON_ARGS_HPP_INCLUDED
#define TTS_COMMON_ARGS_HPP_INCLUDED

#include <tts/api.hpp>
#include <cstddef>
#include <string>

namespace tts::detail
{
  struct args_map
  {
    args_map();

    TTS_API void update(int argc, char **argv);
    TTS_API bool               has_color()  { return !disable_colors_; }
    TTS_API bool               verbose()    { return report_pass_; }
    TTS_API bool               has_valmin() { return has_valmin_; }
    TTS_API bool               has_valmax() { return has_valmax_; }
    TTS_API bool               has_ulp()    { return has_ulp_; }
    TTS_API std::size_t        repetition() { return repetition_; }
    TTS_API std::size_t        count()      { return count_; }
    TTS_API std::string const &order()      { return order_; }
    TTS_API unsigned int       seed()       { return seed_; }
    TTS_API double             valmin()     { return valmin_;}
    TTS_API double             valmax()     { return valmax_;}
    TTS_API double             ulpmax()     { return ulpmax_;}
    TTS_API bool               hex()        { return hex_; }

    private:
    bool         disable_colors_;
    bool         report_pass_;
    bool         has_ulp_;
    bool         has_valmax_;
    bool         has_valmin_;
    std::size_t  repetition_;
    std::size_t  count_;
    std::string  order_;
    unsigned int seed_;
    double       ulpmax_;
    double       valmax_;
    double       valmin_;
    bool         hex_;
  };
}

namespace tts
{
  extern TTS_API detail::args_map args;
}

#endif
