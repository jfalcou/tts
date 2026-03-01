//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <optional>
#include <string>
#include <vector>
#include <list>
#include <iostream>

TTS_CASE("Check display of integral types")
{
  TTS_EQUAL(tts::as_text(true), "true");
  TTS_EQUAL(tts::as_text(false), "false");

  TTS_EQUAL(tts::as_text(std::int8_t {-42}), "-42");
  TTS_EQUAL(tts::as_text(std::int16_t {-42}), "-42");
  TTS_EQUAL(tts::as_text(std::int32_t {-42}), "-42");
  TTS_EQUAL(tts::as_text(std::int64_t {-42}), "-42");

  TTS_EQUAL(tts::as_text(std::uint8_t {69}), "69");
  TTS_EQUAL(tts::as_text(std::uint16_t {69}), "69");
  TTS_EQUAL(tts::as_text(std::uint32_t {69}), "69");
  TTS_EQUAL(tts::as_text(std::uint64_t {69}), "69");
};

TTS_CASE("Check display of string types")
{
  using namespace std::literals;
  TTS_EQUAL(tts::as_text("some string"s), "'some string'");
  TTS_EQUAL(tts::as_text("some string_view"sv), "'some string_view'");
};

TTS_CASE("Check display of pointer types")
{
  TTS_EQUAL(tts::as_text(nullptr), "nullptr");

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
  TTS_EQUAL(tts::as_text((float*)(0)), "0x0 (float *)");
  TTS_EQUAL(tts::as_text((void*)(0)), "0x0 (void *)");
  TTS_EQUAL(tts::as_text((char*)(0)), "0x0 (char *)");
#elif defined(__EMSCRIPTEN__)
  TTS_EQUAL(tts::as_text((float*)(0)), "0 (float *)");
  TTS_EQUAL(tts::as_text((void*)(0)), "0 (void *)");
  TTS_EQUAL(tts::as_text((char*)(0)), "0 (char *)");
#elif defined(_MSC_VER)
  TTS_EQUAL(tts::as_text((float*)(0)), "0000000000000000 (float *)");
  TTS_EQUAL(tts::as_text((void*)(0)), "0000000000000000 (void *)");
  TTS_EQUAL(tts::as_text((char*)(0)), "0000000000000000 (char *)");
#elif defined(__clang__)
  TTS_EQUAL(tts::as_text((float*)(0)), "(nil) (float *)");
  TTS_EQUAL(tts::as_text((void*)(0)), "(nil) (void *)");
  TTS_EQUAL(tts::as_text((char*)(0)), "(nil) (char *)");
#else
  TTS_EQUAL(tts::as_text((float*)(0)), "(nil) (float*)");
  TTS_EQUAL(tts::as_text((void*)(0)), "(nil) (void*)");
  TTS_EQUAL(tts::as_text((char*)(0)), "(nil) (char*)");
#endif
};

TTS_CASE("Check display of sequence type")
{
  TTS_EQUAL(tts::as_text(std::list<int> {1, 2, 3, 4}), "{ 1 2 3 4 }");
};

TTS_CASE("Check display of std::optional")
{
  TTS_EQUAL(tts::as_text(std::optional {42}), "optional<int>{42}");
  TTS_EQUAL(tts::as_text(std::optional<int> {}), "optional<int>{}");
};

struct payload
{
  double       d;
  unsigned int i, j;
};

TTS_CASE("Check display of unknown type")
{
  payload p {1.5, 0xAABBCCDD, 0x11223344};

#if defined(_MSC_VER) && !defined(__clang__)
  TTS_EQUAL(tts::as_text(p), "struct payload: [ 00 00 00 00 00 00 F8 3F DD CC BB AA 44 33 22 11 ]");
#else
  TTS_EQUAL(tts::as_text(p), "payload: [ 00 00 00 00 00 00 F8 3F DD CC BB AA 44 33 22 11 ]");
#endif
};
