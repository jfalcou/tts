//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_CASE_HPP_INCLUDED
#define TTS_CASE_HPP_INCLUDED

#include <tts/detail/color.hpp>
#include <tts/detail/ct_helpers.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/test.hpp>
#include <tts/engine/env.hpp>

//==================================================================================================
#define TTS_CASE(Description)                                                                      \
  void TTS_FUNCTION(::tts::env & runtime);                                                         \
  namespace                                                                                        \
  {                                                                                                \
    static bool TTS_REGISTRATION =                                                                 \
        ::tts::detail ::registration(::tts::detail::test(Description, TTS_FUNCTION));              \
  }                                                                                                \
  void TTS_FUNCTION(::tts::env &runtime)                                                           \
/**/

//==================================================================================================
#define TTS_CASE_TPL(Description, ...)                                                             \
  template<typename T> void TTS_FUNCTION(::tts::env & runtime);                                    \
  namespace                                                                                        \
  {                                                                                                \
    static bool TTS_REGISTRATION =                                                                 \
        ::tts::detail::registration(::tts::detail::test(Description, [](::tts::env &runtime) {     \
          ::tts::detail::for_each_type(                                                            \
              [&](auto t) {                                                                        \
                using T = typename decltype(t)::type;                                              \
                runtime.output()  << ".. with T = [" << ::tts::detail::magenta                     \
                                  << ::tts::type_id<T>() << ::tts::detail::reset                   \
                                  << "] " << std::endl;                                            \
                TTS_FUNCTION<T>(runtime);                                                          \
              },                                                                                   \
              ::tts::detail::typelist<__VA_ARGS__> {});                                            \
        }));                                                                                       \
  }                                                                                                \
  template<typename T>                                                                             \
  void TTS_FUNCTION(::tts::env &runtime)                                                           \
/**/

//==================================================================================================
#define TTS_WHEN(Story)                                                                            \
runtime.output()  << "When      : " << ::tts::detail::yellow << Story                              \
                  << ::tts::detail::reset << std::endl;                                            \
for ( int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++ ) \
   for ( tts::detail::only_once tts__only_once_setup{}; tts__only_once_setup; )                    \
/**/

//==================================================================================================
#define TTS_AND_THEN(Description)                                                                  \
static int TTS_UNIQUE(id) = 0;                                                                     \
if(::tts::detail::section_guard(TTS_UNIQUE( id ), tts_section, tts_count )                         \
                              .check(std::string("  And then: ") + Description, runtime)           \
  )                                                                                                \
for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++ )   \
  for(tts::detail::only_once tts__only_once_section{}; tts__only_once_section; )                   \
/**/

#endif
