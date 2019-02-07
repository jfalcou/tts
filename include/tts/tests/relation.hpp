//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//===================================================================================================
#ifndef TTS_TESTS_RELATION_HPP_INCLUDED
#define TTS_TESTS_RELATION_HPP_INCLUDED

#include <tts/tests/basic.hpp>

/*!
  @ingroup group-unit
  Test for value equality.

  This macro performs an equality test between its arguments using the @c == operator.

  @par Usage:

  @snippet test/unit/relation.cpp equal

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_EQUAL(A, B) TTS_EXPECT((A) == (B))

/*!
  @ingroup group-unit
  Test for value inequality.

  This macro performs an inequality test between its arguments using either the natural
  @c != operator or an ADL-accessible function named @c compare_not_equal, whichever occurs first.

  @par Usage:

  @snippet test/unit/relation.cpp not_equal

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_NOT_EQUAL(A, B) TTS_EXPECT((A) != (B))

/*!
  @ingroup group-unit
  Test for value less-than relationship.

  This macro performs a less-than test between its arguments using either the natural @c < operator
  or an ADL-accessible function named @c compare_less , whichever occurs first.

  @par Usage:

  @snippet test/unit/relation.cpp less

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_LESS(A, B) TTS_EXPECT((A) < (B))

/*!
  @ingroup group-unit
  Test for value greater-than relationship.

  This macro performs a greater-than test between its arguments using either the natural
  @c > operator or an ADL-accessible function named @c compare_greater, whichever occurs first.

  @par Usage:

  @snippet test/unit/relation.cpp greater

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_GREATER(A, B) TTS_EXPECT((A) > (B))

/*!
  @ingroup group-unit
  Test for value lesser-or-equal-than relationship.

  This macro performs a lesser-or-equal-than test between its arguments using either the natural
  @c <= operator or an ADL-accessible function named @c compare_less_equal, whichever occurs first.

  @par Usage:

  @snippet test/unit/relation.cpp less_equal

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_LESS_EQUAL(A, B) TTS_EXPECT((A) <= (B))

/*!
  @ingroup group-unit
  Test for value greater-or-equal-than relationship.

  This macro performs a greater-or-equal-than test between its arguments using either the natural
  @c >= operator or an ADL-accessible function named @c compare_greater_equal, whichever occurs
first.

  @par Usage:

  @snippet test/unit/relation.cpp greater_equal

  where @c foo::bar is defined as follow:

  @snippet test/unit/relation.cpp bar

  @param A First argument of the test
  @param B Second argument of the test
**/
#define TTS_GREATER_EQUAL(A, B) TTS_EXPECT((A) >= (B))

#endif
