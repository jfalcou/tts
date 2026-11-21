//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION threads_main
#include <tts/tts.hpp>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <utility>

// Pure functions a few ULP apart, so that each histogram fills several buckets.
float root(float x)
{
  return static_cast<float>(std::sqrt(static_cast<double>(x)));
}

float rough_root(float x)
{
  float r    = std::sqrt(x);
  auto  bits = std::bit_cast<std::uint32_t>(x);
  if(bits % 7 == 0) r = std::nextafter(r, 1e30f);
  if(bits % 11 == 0) r = std::nextafter(std::nextafter(r, 0.f), 0.f);
  return r;
}

TTS_CASE("Range check of every float from 1 to 2")
{
  TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 2.f), float, float, root, rough_root, 4.);
};

TTS_CASE("Range check of a sample of floats")
{
  TTS_ULP_RANGE_CHECK(tts::sweep(0.5f, 64.f, 1 << 20), float, float, root, rough_root, 4.);
};

TTS_CASE("Range check of a sample of 16-bit pairs")
{
  auto ratio = [](std::int16_t a, std::int16_t b)
  { return static_cast<float>(a) / static_cast<float>(b | 1); };
  auto rough_ratio = [ ratio ](std::int16_t a, std::int16_t b)
  { return a % 5 == 0 ? std::nextafter(ratio(a, b), 1e30f) : ratio(a, b); };

  TTS_ULP_RANGE_CHECK(tts::sweep(1 << 20),
                      (tts::types<std::int16_t, std::int16_t>),
                      (tts::types<std::int16_t, std::int16_t>),
                      ratio,
                      rough_ratio,
                      4.);
};

TTS_CASE("Range check of a result with two members")
{
  auto both       = [](float x) { return std::pair {root(x), x * x}; };
  auto rough_both = [](float x) { return std::pair {rough_root(x), x * x}; };

  TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 4.f, 1 << 20), float, float, both, rough_both, 4.);
};

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE
namespace
{
  // The output of a verbose run, without the times, which change from one run to the next.
  bool run(char const* cores, char* out, std::size_t size)
  {
    std::array<char const*, 5> argv {
    "threads", "--verbose", "--seed=1", "--capture=tts_threads_test.txt", cores};
    ::tts::_::current_arguments = ::tts::options {5, argv.data()};
    if(threads_main(1, argv.data()) != 0) return false;

    FILE* f = fopen("tts_threads_test.txt", "r");
    if(!f) return false;

    char        line[ 1024 ] = {}; // NOSONAR - avoids std::string
    std::size_t n            = 0;
    bool        fits         = true;
    while(fits && fgets(line, sizeof(line), f))
    {
      if(strncmp(line, "Total Time", 10) == 0) continue;
      if(char* t = strstr(line, " - [PASSED] ("); t)
      {
        t[ 0 ] = '\n';
        t[ 1 ] = '\0';
      }
      std::size_t l = strnlen(line, sizeof(line));
      fits          = n + l < size;
      if(fits)
      {
        memcpy(out + n, line, l);
        n += l;
      }
    }
    out[ n ] = '\0';
    fclose(f);
    remove("tts_threads_test.txt");
    return fits;
  }
}

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  static char one[ 1 << 16 ];  // NOSONAR - avoids std::string
  static char four[ 1 << 16 ]; // NOSONAR - avoids std::string

  bool        ok     = run("--cores=1", one, sizeof(one)) && run("--cores=4", four, sizeof(four));
  bool        report = strstr(one, "Max ULP") != nullptr; // NOSONAR - contains() is C++23
  ok                 = ok && report && strcmp(one, four) == 0;

  if(!ok)
    printf("One thread:\n%s\nFour threads:\n%s\n", one, four); // NOSONAR - std::print is C++23
  return ok ? 0 : 1;
}
TTS_DISABLE_WARNING_POP
