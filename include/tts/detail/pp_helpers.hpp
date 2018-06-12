//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_PP_HELPERS_HPP_INCLUDED
#define TTS_DETAIL_PP_HELPERS_HPP_INCLUDED

// Generate local unique ID for function or registration varianle using line descriptor
#define TTS_FUNCTION      TTS_UNIQUE(tts_function)
#define TTS_REGISTRATION  TTS_UNIQUE(tts_registration)
#define TTS_UNIQUE3( ID, LINE )   ID ## LINE
#define TTS_UNIQUE2( ID, LINE )   TTS_UNIQUE3( ID, LINE )
#define TTS_UNIQUE( ID )          TTS_UNIQUE2( ID, __LINE__ )

// Turn a variadic macro calls into a string containing properly placed commas
#define TTS_STRING(...)   TTS_STRING_((__VA_ARGS__))
#define TTS_STRING__(...) #__VA_ARGS__
#define TTS_STRING_(TXT)  TTS_STRING__ TXT

// Remove parens around macro token if any are present
// NON TRIVIAL SHIT
#define TTS_REMOVE_PARENS(x)              TTS_EVAL((TTS_REMOVE_PARENS_I x), x)
#define TTS_REMOVE_PARENS_I(...)          1,1
#define TTS_CAT(x, y)                     TTS_CAT_I(x, y)
#define TTS_CAT_I(x, y)                   x ## y
#define TTS_APPLY(macro, args)            TTS_APPLY_I(macro, args)
#define TTS_APPLY_I(macro, args)          macro args
#define TTS_EVAL_I(test, x)               TTS_MAYBE_STRIP_PARENS(TTS_TEST_ARITY test, x)
#define TTS_EVAL(test, x)                 TTS_EVAL_I(test, x)
#define TTS_TEST_ARITY(...)               TTS_APPLY(TTS_TEST_ARITY_I, (__VA_ARGS__, 2, 1))
#define TTS_TEST_ARITY_I(a,b,c,...)       c
#define TTS_MAYBE_STRIP_PARENS(cond, x)   TTS_MAYBE_STRIP_PARENS_I(cond, x)
#define TTS_MAYBE_STRIP_PARENS_I(cond, x) TTS_CAT(TTS_MAYBE_STRIP_PARENS_, cond)(x)
#define TTS_MAYBE_STRIP_PARENS_1(x)       x
#define TTS_MAYBE_STRIP_PARENS_2(x)       TTS_APPLY(TTS_MAYBE_STRIP_PARENS_2_I, x)
#define TTS_MAYBE_STRIP_PARENS_2_I(...)   __VA_ARGS__

#endif
