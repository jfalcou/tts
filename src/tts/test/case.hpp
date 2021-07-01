//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <sstream>

//================================================================================================
// Type wrapping helpers
//================================================================================================
namespace tts
{
  template<typename     T > struct type  {};
  template<typename...  Ts> struct types {};
}

//================================================================================================
// Lambda "seed" capture
//================================================================================================
namespace tts::detail
{
  template<typename TestBed> struct lambda_test
  {
    lambda_test(TestBed f) : base_test(f) {}
    auto operator+(auto TestBody) const
    {
      base_test( TestBody );
      return true;
    }

    TestBed base_test;
  };
}

//==================================================================================================
// Test case registration macros
//==================================================================================================
#define TTS_CASE_TPL(DESCRIPTION,...)                                                               \
inline bool const TTS_CAT(register_,TTS_FUNCTION) =  ::tts::detail::lambda_test{                    \
[](auto tests)                                                                                      \
  {                                                                                                 \
    auto const single_test = [=]<typename T>( ::tts::type<T> )                                      \
    {                                                                                               \
      ::tts::detail::test::acknowledge(::tts::detail::test                                          \
      {                                                                                             \
          std::string{DESCRIPTION} + " (with T = " + std::string{::tts::typename_<T>} + ")"         \
        , [=]() {tests(::tts::type<T>{}); }                                                         \
        });                                                                                         \
    };                                                                                              \
                                                                                                    \
    [&]<template<class...> class L,typename... Ts>(L<Ts...>)                                        \
    {                                                                                               \
      (single_test( ::tts::type<Ts>() ),...);                                                       \
    }( ::tts::types<__VA_ARGS__>{} );                                                               \
                                                                                                    \
    return true;                                                                                    \
  }} + []                                                                                           \
/**/

//==================================================================================================
// Test case registration macros
//==================================================================================================
#define TTS_CASE(...)                                                                               \
inline bool const TTS_CAT(register_,TTS_FUNCTION) =  ::tts::detail::lambda_test{                    \
[](auto tests)                                                                                      \
  {                                                                                                 \
    std::ostringstream title;                                                                       \
    title << __VA_ARGS__;                                                                           \
    return ::tts::detail::test::acknowledge(::tts::detail::test{ title.str(), [=](){ tests(); } }); \
  }} + []()                                                                                         \
/**/

//==================================================================================================
// Test sub-case registrations
//==================================================================================================
namespace tts::detail
{
  // Setup/section environment guard
  struct section_guard
  {
    int &      id;
    int const &section;

    section_guard(int &id_, int const &section_, int &count) : id(id_) , section(section_)
    {
      if(section == 0) id = count++ - 1;
    }

    template<typename Desc> bool check(Desc const& desc)
    {
      if(id == section) std::cout << "  And then: " << desc << std::endl;
      return id == section;
    }
  };

  struct only_once
  {
    bool once = true;
    explicit operator bool() { bool result = once; once = false; return result; }
  };
}

#define TTS_WHEN(STORY)                                                                             \
  std::cout << "When      : " << ::tts::yellow << STORY << ::tts::reset << std::endl;               \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++)   \
    for( tts::detail::only_once tts_only_once_setup{}; tts_only_once_setup; )                       \
/**/

#define TTS_AND_THEN_IMPL(TTS_LOCAL_ID, ...)                                                        \
  static int TTS_LOCAL_ID = 0;                                                                      \
  std::ostringstream TTS_CAT(desc_,TTS_LOCAL_ID);                                                   \
  if(::tts::detail::section_guard(TTS_LOCAL_ID, tts_section, tts_count )                            \
                  .check( ((TTS_CAT(desc_,TTS_LOCAL_ID)  << __VA_ARGS__)                            \
                          , TTS_CAT(desc_,TTS_LOCAL_ID).str())                                      \
                        )                                                                           \
    )                                                                                               \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++ )  \
    for(tts::detail::only_once tts__only_once_section{}; tts__only_once_section; )                  \
/**/

#define TTS_AND_THEN(...) TTS_AND_THEN_IMPL(TTS_UNIQUE(id), __VA_ARGS__)
