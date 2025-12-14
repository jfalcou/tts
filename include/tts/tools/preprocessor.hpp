//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/traits.hpp>

//======================================================================================================================
// Macro for move semantic/forward semantic
//======================================================================================================================
#define TTS_MOVE(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define TTS_FWD(...)  static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

//======================================================================================================================
// Portable PRAGMA Handler
//======================================================================================================================
#if defined(_MSC_VER)
  #define TTS_DISABLE_WARNING_PUSH           __pragma(warning( push ))
  #define TTS_DISABLE_WARNING_POP            __pragma(warning( pop ))
  #define TTS_DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))
  #define TTS_DISABLE_WARNING_SHADOW
  #define TTS_DISABLE_WARNING_CRT_SECURE     __pragma(warning( disable : 4996 ))
#elif defined(__GNUC__) || defined(__clang__)
  #define TTS_DO_PRAGMA(X)                    _Pragma(#X)
  #define TTS_DISABLE_WARNING_PUSH            TTS_DO_PRAGMA(GCC diagnostic push)
  #define TTS_DISABLE_WARNING_POP             TTS_DO_PRAGMA(GCC diagnostic pop)
  #define TTS_DISABLE_WARNING(warningName)    TTS_DO_PRAGMA(GCC diagnostic ignored #warningName)
  #define TTS_DISABLE_WARNING_SHADOW          TTS_DISABLE_WARNING(-Wshadow)
  #define TTS_DISABLE_WARNING_CRT_SECURE
#else
  #define TTS_DISABLE_WARNING_PUSH
  #define TTS_DISABLE_WARNING_POP
  #define TTS_DISABLE_WARNING_SHADOW
  #define TTS_DISABLE_WARNING_CRT_SECURE
#endif

//======================================================================================================================
// Macro chains for proper unique line based ID generation
//======================================================================================================================
#ifndef TTS_FUNCTION
#define TTS_FUNCTION TTS_UNIQUE(tts_function)
#endif

#ifndef TTS_REGISTRATION
#define TTS_REGISTRATION TTS_UNIQUE(tts_registration)
#endif

#define TTS_UNIQUE3(ID, LINE) ID##LINE
#define TTS_UNIQUE2(ID, LINE) TTS_UNIQUE3(ID, LINE)
#define TTS_UNIQUE(ID)        TTS_UNIQUE2(ID, __COUNTER__)

//======================================================================================================================
// Proper Macro concatenation of tokens
//======================================================================================================================
#define TTS_CAT(x, y)   TTS_CAT_I(x, y)
#define TTS_CAT_I(x, y) x##y

//======================================================================================================================
// Proper Macro token -> string conversion
//======================================================================================================================
#define TTS_STRING(...)   TTS_STRING_((__VA_ARGS__))
#define TTS_STRING__(...) #__VA_ARGS__
#define TTS_STRING_(TXT)  TTS_STRING__ TXT

//======================================================================================================================
// Count amount of variadic tokens
//======================================================================================================================
#define TTS_COUNT(...) TTS_COUNT_(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#define TTS_COUNT_(A0, A1, A2, A3, A4, A5, A6, A7, ...) A7

//======================================================================================================================
// Turns a,b,c into auto a, auto b, auto c
//======================================================================================================================
#define TTS_ARG0()
#define TTS_ARG1(A0)                          auto&& A0
#define TTS_ARG2(A0, A1)                      auto&& A0, auto&& A1
#define TTS_ARG3(A0, A1, A2)                  TTS_ARG2(A0, A1)                , auto&& A2
#define TTS_ARG4(A0, A1, A2, A3)              TTS_ARG3(A0, A1, A2)            , auto&& A3
#define TTS_ARG5(A0, A1, A2, A3, A4)          TTS_ARG4(A0, A1, A2, A3)        , auto&& A4
#define TTS_ARG6(A0, A1, A2, A3, A4, A5)      TTS_ARG5(A0, A1, A2, A3, A4)    , auto&& A5
#define TTS_ARG7(A0, A1, A2, A3, A4, A5, A6)  TTS_ARG6(A0, A1, A2, A3, A4, A5), auto&& A6

#define TTS_ARG(...) TTS_CAT(TTS_ARG, TTS_COUNT(__VA_ARGS__))(__VA_ARGS__)

//======================================================================================================================
// Revert a VA_ARGS
//======================================================================================================================
#define TTS_VAL(x)                    x
#define TTS_REVERSE_1(a)              (a)
#define TTS_REVERSE_2(a,b)            (b, a)
#define TTS_REVERSE_3(a,b,c)          (c, b, a)
#define TTS_REVERSE_4(a,b,c,d)        (d, c, b, a)
#define TTS_REVERSE_5(a,b,c,d,e)      (e, d, c, b, a)
#define TTS_REVERSE_6(a,b,c,d,e,f)    (f, e, d, c, b, a)
#define TTS_REVERSE_7(a,b,c,d,e,f,g)  (g, f, e, d, c, b, a)

#define TTS_REVERSE_IMPL(N,...) TTS_VAL(TTS_REVERSE_ ## N(__VA_ARGS__))
#define TTS_REVERSE_(N,...)     TTS_REVERSE_IMPL( N, __VA_ARGS__)
#define TTS_REVERSE(...)        TTS_REVERSE_( TTS_COUNT(__VA_ARGS__), __VA_ARGS__)

//======================================================================================================================
// Remove parens around macro token if any are present -- NON TRIVIAL SHIT
//======================================================================================================================
#define TTS_REMOVE_PARENS(x)              TTS_EVAL((TTS_REMOVE_PARENS_I x), x)
#define TTS_REMOVE_PARENS_I(...)          1, 1
#define TTS_APPLY(macro, args)            TTS_APPLY_I(macro, args)
#define TTS_APPLY_I(macro, args)          macro args
#define TTS_EVAL_I(test, x)               TTS_MAYBE_STRIP_PARENS(TTS_TEST_ARITY test, x)
#define TTS_EVAL(test, x)                 TTS_EVAL_I(test, x)
#define TTS_TEST_ARITY(...)               TTS_APPLY(TTS_TEST_ARITY_I, (__VA_ARGS__, 2, 1))
#define TTS_TEST_ARITY_I(a, b, c, ...)    c
#define TTS_MAYBE_STRIP_PARENS(cond, x)   TTS_MAYBE_STRIP_PARENS_I(cond, x)
#define TTS_MAYBE_STRIP_PARENS_I(cond, x) TTS_CAT(TTS_MAYBE_STRIP_PARENS_, cond)(x)
#define TTS_MAYBE_STRIP_PARENS_1(x)       x
#define TTS_MAYBE_STRIP_PARENS_2(x)       TTS_APPLY(TTS_MAYBE_STRIP_PARENS_2_I, x)
#define TTS_MAYBE_STRIP_PARENS_2_I(...)   __VA_ARGS__
