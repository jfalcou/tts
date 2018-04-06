//==================================================================================================
/*!
  @file

  Defines the various macro for defining test cases

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_CASE_HPP_INCLUDED
#define TTS_CASE_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/ct_helpers.hpp>
#include <tts/detail/test.hpp>
#include <tts/engine/env.hpp>

/*!
  @ingroup group-unit

  @brief Starts a block defining a test case.

  The following block will contains user defined preparation, actual tests,
  expectations and clean-up for said test case.

  A simple test case is then defined as :

  @snippet example/unit/case.cpp case

  Note that test cases performing no actual tests such as:

  @snippet example/unit/case.cpp fail_case

  will be reported as an automatic failure.

  Most of the time, however, compiling with all warning options will lead to compile-time
  notification of such cases.

  @param DESCRIPTION String literal describing the purpose of the test case
**/
#define TTS_CASE(DESCRIPTION)                                                                       \
void TTS_FUNCTION( ::tts::env& );                                                                   \
namespace                                                                                           \
{                                                                                                   \
  static inline bool                                                                                \
  TTS_REGISTRATION = ::tts::detail::registration(::tts::detail::test(DESCRIPTION, TTS_FUNCTION));   \
}                                                                                                   \
void TTS_FUNCTION( ::tts::env& runtime)                                                             \
/**/

/*!
  @ingroup group-unit

  @brief Starts a block defining a template test case.

  The following block will contains user defined preparation, actual tests,
  expectations and clean-up for said test case parametrized by a template type named @c T
  and instantiated for each type in the @c TYPES sequence.

  Such a test case is then defined as :

  @snippet example/unit/case.cpp case_tpl

  Note that, as for @ref TTS_CASE , empty tests are reported as automatic failures.

  @param DESCRIPTION  String literal describing the scope of the test case
  @param TYPES        Boost.Preprocessor sequence of types
**/
#define TTS_CASE_TPL(DESCRIPTION, TYPES)                                                            \
template<typename T> void TTS_FUNCTION( tts::env& );                                                \
namespace                                                                                           \
{                                                                                                   \
  static inline bool                                                                                \
  TTS_REGISTRATION = ::tts::detail::registration                                                    \
                      ( ::tts::detail::test                                                         \
                        ( DESCRIPTION                                                               \
                        , [](::tts::env& runtime)                                                   \
                          {                                                                         \
                            ::tts::detail::for_each_type                                            \
                            ( [&](auto t)                                                           \
                              {                                                                     \
                                using T = typename decltype(t)::type;                               \
                                runtime.stream() << std::endl;                                      \
                                runtime.stream() <<  "With T = [" << ::tts::type_id<T>() << "] ";   \
                                if(!runtime.is_compact()) { runtime.stream() << std::endl; }        \
                                TTS_FUNCTION<T>(runtime);                                           \
                              }                                                                     \
                            , ::tts::detail::typelist<TTS_REMOVE_PARENS(TYPES)>{}                   \
                            );                                                                      \
                          }                                                                         \
                        )                                                                           \
                      );                                                                            \
}                                                                                                   \
template<typename T> void TTS_FUNCTION( tts::env& runtime )                                         \
/**/

#endif
