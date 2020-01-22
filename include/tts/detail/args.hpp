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

    TTS_API void update(int argc, char **argv) const;

    bool               has_color()  const { return !disable_colors_; }
    bool               verbose()    const { return report_pass_; }
    bool               has_ulp()    const { return has_ulp_; }
    std::size_t        repetition() const { return repetition_; }
    std::size_t        count()      const { return count_; }
    std::string const &order()      const { return order_; }
    unsigned int       seed()       const { return seed_; }
    double             ulpmax()     const { return ulpmax_;}
    bool               hex()        const { return hex_; 
    }
    
  private:
    mutable bool         disable_colors_;
    mutable bool         report_pass_;
    mutable bool         has_ulp_;
    mutable std::size_t  repetition_;
    mutable std::size_t  count_;
    mutable std::string  order_;
    mutable unsigned int seed_;
    mutable double       ulpmax_;
    mutable bool         hex_; 
  };
}

namespace tts
{
  extern TTS_API const detail::args_map args;
}

#endif
