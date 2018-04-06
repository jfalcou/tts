//==================================================================================================
/*!
  @file

  Common types and filters ready to use

  @copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_TYPES_HPP_INCLUDED
#define TTS_TYPES_HPP_INCLUDED

#include <cstddef>
#include <cstdint>

#define TTS_SIGNED_INTEGRAL_TYPES     (std::int8_t,std::int16_t,std::int32_t,std::int64_t)
#define TTS_UNSIGNED_INTEGRAL_TYPES   (std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t)

#define TTS_INTEGRAL_TYPES            ( char                                                        \
                                      , std::int8_t,std::int16_t,std::int32_t,std::int64_t          \
                                      , std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t      \
                                      )                                                             \
/**/

#define TTS_IEEE_TYPES (float,double)

#define TTS_SIGNED_NUMERIC_TYPES    ( float, double                                                 \
                                    , std::int8_t,std::int16_t,std::int32_t,std::int64_t            \
                                    )                                                               \
/**/

#define TTS_UNSIGNED_NUMERIC_TYPES  TTS_UNSIGNED_INTEGRAL_TYPES
#define TTS_NUMERIC_TYPES           ( char, float, double                                           \
                                    , std::int8_t,std::int16_t,std::int32_t,std::int64_t            \
                                    , std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t        \
                                    )                                                               \
/**/

#define TTS_ALL_TYPES ( bool, char, float, double                                                   \
                      , std::int8_t,std::int16_t,std::int32_t,std::int64_t                          \
                      , std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t                      \
                      )                                                                             \
/**/

#endif
