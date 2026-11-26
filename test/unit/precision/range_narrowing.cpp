//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <array>
#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

TTS_CASE("--valmin and --valmax narrow tts::sweep and tts::sample, with a note")
{
  std::vector<int> values;

  auto             ref = [ &values ](std::int16_t x)
  {
    values.push_back(x);
    return x;
  };
  auto                       same  = [](std::int16_t x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_narrowing", "--valmin=-3", "--valmax=5"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  tts::gathering_sink gs;
  std::vector<int>    whole;
  std::vector<int>    bounded;
  std::vector<int>    sampled;
  std::vector<int>    inside;
  {
    tts::scoped_sink scope(gs);

    TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, ref, same, 0.);
    whole = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(0, 100), std::int16_t, std::int16_t, ref, same, 0.);
    bounded = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sample(0, 100, 9), std::int16_t, std::int16_t, ref, same, 0.);
    sampled = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(-2, 2), std::int16_t, std::int16_t, ref, same, 0.);
    inside = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(10, 20), std::int16_t, std::int16_t, ref, same, 0.);
  }

  ::tts::_::current_arguments = saved;

  auto has                    = [ &gs ](char const* s)
  {
    return std::strstr(gs.content().data(), s) != nullptr; // NOSONAR - contains() is C++23
  };

  TTS_EXPECT(whole == (std::vector {-3, -2, -1, 0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(bounded == (std::vector {0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(sampled == (std::vector {0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(inside == (std::vector {-2, -1, 0, 1, 2}));
  TTS_EXPECT(values.empty());

  TTS_EXPECT(has("--valmin and --valmax narrow argument 1 to [-3, 5]"));
  TTS_EXPECT(has("--valmin and --valmax narrow argument 1 to [0, 5]"));
  TTS_EXPECT(!has("to [-2, 2]"));
  TTS_EXPECT(has("--valmin and --valmax leave no value of argument 1"));
};

enum class step
{
  low,
  mid,
  high
};

namespace tts
{
  template<> struct enumeration<step>
  {
    static constexpr std::uint64_t last = 2;

    static constexpr std::uint64_t index(step s) noexcept
    {
      return static_cast<std::uint64_t>(s);
    }
    static constexpr step value(std::uint64_t i) noexcept
    {
      return static_cast<step>(i);
    }
  };
}

TTS_CASE("--valmin and --valmax leave the sweep of a user type whole")
{
  std::vector<int> values;

  auto             ref = [ &values ](step s)
  {
    values.push_back(static_cast<int>(s));
    return static_cast<int>(s);
  };
  auto                       same  = [](step s) { return static_cast<int>(s); };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_narrowing", "--valmin=1", "--valmax=1"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  TTS_ULP_RANGE_CHECK(tts::sweep, step, step, ref, same, 0.);

  ::tts::_::current_arguments = saved;

  TTS_EXPECT(values == (std::vector {0, 1, 2}));
};

struct wide_randoms : tts::randoms<float, float>
{
  wide_randoms()
      : tts::randoms<float, float>(-1000.f, 1000.f)
  {
  }
};

TTS_CASE("--valmin and --valmax narrow tts::randoms")
{
  float       lo      = 0.f;
  float       hi      = 0.f;
  std::size_t outside = 0;
  std::size_t calls   = 0;

  auto        ref     = [ &lo, &hi, &outside, &calls ](float x)
  {
    ++calls;
    outside += (x < lo || x > hi) ? 1 : 0;
    return x;
  };
  auto                       same  = [](float x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_narrowing", "--valmin=1", "--valmax=2"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  tts::gathering_sink gs;
  {
    tts::scoped_sink scope(gs);

    lo = 1.f;
    hi = 2.f;
    TTS_ULP_RANGE_CHECK(tts::randoms(-1000.f, 1000.f), float, float, ref, same, 0.);
    lo = 1.5f;
    hi = 1.75f;
    TTS_ULP_RANGE_CHECK(tts::randoms(1.5f, 1.75f), float, float, ref, same, 0.);
    lo = 1.f;
    hi = 2.f;
    TTS_ULP_RANGE_CHECK(tts::randoms(1000.f, -1000.f), float, float, ref, same, 0.);
    TTS_ULP_RANGE_CHECK(wide_randoms {}, float, float, ref, same, 0.);
  }

  ::tts::_::current_arguments = saved;

  TTS_EQUAL(outside, std::size_t {0});
  TTS_EQUAL(calls, std::size_t {4 * 4096});
};

TTS_CASE("tts::randoms takes its bounds in either order")
{
  std::size_t calls   = 0;
  std::size_t outside = 0;

  auto        ref     = [ &calls, &outside ](int x)
  {
    ++calls;
    outside += (x < -10 || x > 10) ? 1 : 0;
    return x;
  };
  auto same = [](int x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::randoms(10, -10), int, int, ref, same, 0.);

  TTS_EQUAL(calls, std::size_t {4096});
  TTS_EQUAL(outside, std::size_t {0});
};
