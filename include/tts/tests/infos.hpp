//==================================================================================================
/*!
  @file

  Defines informations display macros.

  @copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
**/
//==================================================================================================
#ifndef TTS_TESTS_INFOS_HPP_INCLUDED
#define TTS_TESTS_INFOS_HPP_INCLUDED

#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/decompose.hpp>

/*!
  @ingroup group-detail
  Display an informations message

  @param INDICATOR Type of message
  @param MESSAGE Values to display
**/
#define TTS_DISPLAY( INDICATOR, MESSAGE )                                                           \
do                                                                                                  \
{                                                                                                   \
  if(!runtime.is_compact()) runtime.stream() << INDICATOR << MESSAGE << std::endl;                  \
} while( ::tts::detail::is_false() )                                                                \
/**/

/*!
  @ingroup group-unit
  Display an informations message

  @par Usage:

  @snippet test/unit/pass.cpp info

  @param MESSAGE Values to display
**/
#define TTS_INFO( MESSAGE ) TTS_DISPLAY("[INFO] ", MESSAGE)

/*!
  @ingroup group-unit
  Display a warning message.

  @par Usage:

  @snippet test/unit/pass.cpp warn

  @param MESSAGE Values to display
**/
#define TTS_WARNING( MESSAGE ) TTS_DISPLAY("[WARNING] ", MESSAGE)

/*!
  @ingroup group-unit
  Display an error message.

  @par Usage:

  @snippet test/unit/pass.cpp error

  @param MESSAGE Values to display
**/
#define TTS_ERROR( MESSAGE ) TTS_DISPLAY("[ERROR] ", MESSAGE)

/*!
  @ingroup group-unit
  Automatically successful test

  Register a test which always passes and streams a custom @c MESSAGE.

  @par Usage:

  @snippet test/unit/pass.cpp pass

  @param MESSAGE Values to stream as additional informations
**/
#define TTS_PASS( MESSAGE )                                                                         \
runtime.pass() << MESSAGE << " in: " << ::tts::detail::at(__FILE__,__LINE__) << std::endl           \
/**/

/*!
  @ingroup group-unit
  Automatically failing test

  Register a test which always fails and streams a custom @c MESSAGE.

  @par Usage:

  @snippet test/unit/failure.cpp fail

  @param MESSAGE Values to stream as additional informations
**/
#define TTS_FAIL( MESSAGE )                                                                         \
runtime.fail() << MESSAGE << " in: " << ::tts::detail::at(__FILE__,__LINE__) << std::endl           \
/**/

#endif
