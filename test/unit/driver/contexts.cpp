//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>
#include <cstring>
#include <type_traits>

namespace
{
  // A result that depends on the context, as a function with an if consteval branch has.
  constexpr int by_context(int v)
  {
    return std::is_constant_evaluated() ? v + 1 : v;
  }

  int runtime_only(int v)
  {
    return v;
  }

  ::tts::_::test const* registered(char const* name)
  {
    for(auto const& t: ::tts::_::suite())
      if(::tts::text {t.name} == ::tts::text {name}) return &t;
    return nullptr;
  }

  ::tts::counter successes_before = 0; // NOSONAR - one case writes it, the next reads it

  // Runs a body in the constant context alone, under a gathering sink, and gives its report
  // back, the counters the failures moved restored.
  template<bool Probe>::tts::text report_of(auto body)
  {
    auto const            failures = ::tts::global_runtime.failure_count;
    auto const            tests    = ::tts::global_runtime.test_count;
    ::tts::gathering_sink seen;
    {
      ::tts::scoped_sink quiet {seen};
      ::tts::_::constant_t::run<::tts::_::direct, Probe>(body);
    }
    ::tts::global_runtime.failure_count = failures;
    ::tts::global_runtime.test_count    = tests;
    return seen.content();
  }

  bool says(::tts::text const& report, char const* what)
  {
    return std::strstr(report.data(), what) != nullptr; // NOSONAR - contains is C++23
  }
}

TTS_CASE("Reads the success count before the constexpr case")
{
  successes_before = ::tts::global_runtime.success_count;
  TTS_PASS("marker");
};

TTS_CASE(TTS_CONSTEXPR("A constexpr case runs its body in both contexts"))
{
  int                sum    = 0;
  std::array<int, 4> values = {1, 2, 3, 4};
  for(auto v: values)
    sum += v;

  TTS_EQUAL(sum, 10);
  TTS_NOT_EQUAL(sum, 0);
  TTS_EXPECT(sum > 0);
  TTS_TYPED_EQUAL(sum - 10, 0);
  TTS_CONSTEXPR_EQUAL(1 + 1, 2);
  TTS_TYPE_IS(decltype(sum), int);
  TTS_EXPR_IS(sum + 1, int);
  TTS_EXPECT_COMPILES(sum, { sum + 1; });
  TTS_EQUAL(sum, 10) << "a message is accepted in both contexts";
};

TTS_CASE("Both contexts counted the nine checks of the constexpr case")
{
  TTS_EQUAL(::tts::global_runtime.success_count - successes_before, ::tts::counter {1 + 2 * 9});
};

TTS_CASE(TTS_XFAIL(TTS_CONSTEXPR("A failure in constant evaluation alone is reported")))
{
  TTS_EQUAL(by_context(1), 1);
};

TTS_CASE(TTS_CONSTEVAL("A consteval case runs its body in constant evaluation alone"))
{
  TTS_EXPECT(std::is_constant_evaluated());
  TTS_EQUAL(by_context(1), 2);
};

TTS_CASE(TTS_CONSTEXPR(TTS_XFAIL("Wrappers nest in either order")))
{
  TTS_EQUAL(by_context(1), 1);
};

TTS_CASE(TTS_MAYFAIL(TTS_CONSTEVAL("TTS_MAYFAIL wraps a consteval title")))
{
  TTS_EQUAL(by_context(1), 1);
};

TTS_CASE(TTS_CONSTEVAL(TTS_XINVALID("An empty consteval body is an empty case")))
{
// Empty on purpose: the case is tagged TTS_XINVALID
};

TTS_CASE(TTS_TIMEOUT(2000, TTS_CONSTEXPR("TTS_TIMEOUT wraps a constexpr title")))
{
  TTS_EQUAL(1, 1);
};

TTS_CASE(TTS_CONSTEXPR(TTS_TIMEOUT(3000, "A constexpr title wraps TTS_TIMEOUT")))
{
  TTS_EQUAL(1, 1);
};

TTS_CASE_IN("TTS_CASE_IN names the contexts in the order given", tts::constant, tts::runtime)
{
  TTS_EQUAL(1 + 1, 2);
};

TTS_CASE_TPL(TTS_CONSTEXPR("A template case runs each type in both contexts"), int, float)
<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(T(1) + T(1), T(2));
  TTS_TYPED_EQUAL(T(2) * T(2), T(4));
};

TTS_CASE_TPL(TTS_XFAIL(TTS_CONSTEVAL("A template case fails in constant evaluation alone")), int)
<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(by_context(T(1)), T(1));
};

TTS_CASE_TPL_IN("TTS_CASE_TPL_IN names the contexts of each type",
                (tts::types<int, float>),
                tts::constant,
                tts::runtime)<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(T(1) + T(1), T(2));
};

TTS_CASE("The tags and deadlines of wrapped titles reach the registered cases")
{
  using enum ::tts::expected_outcome;

  TTS_EQUAL(registered("A failure in constant evaluation alone is reported")->tag, xfail);
  TTS_EQUAL(registered("Wrappers nest in either order")->tag, xfail);
  TTS_EQUAL(registered("TTS_MAYFAIL wraps a consteval title")->tag, may_fail);
  TTS_EQUAL(registered("An empty consteval body is an empty case")->tag, xinvalid);
  TTS_EQUAL(registered("TTS_CASE_IN names the contexts in the order given")->tag, pass);
  TTS_EQUAL(registered("A template case fails in constant evaluation alone")->tag, xfail);
  TTS_EQUAL(registered("TTS_CASE_TPL_IN names the contexts of each type")->tag, pass);

  auto outside = registered("TTS_TIMEOUT wraps a constexpr title");
  TTS_EXPECT(outside->timeout_set);
  TTS_EQUAL(outside->timeout_ns, ::tts::nanoseconds {2000} * 1'000'000);

  auto inside = registered("A constexpr title wraps TTS_TIMEOUT");
  TTS_EXPECT(inside->timeout_set);
  TTS_EQUAL(inside->timeout_ns, ::tts::nanoseconds {3000} * 1'000'000);
};

TTS_CASE("A constant failure names the values the check compares at run time")
{
  auto diverging = [](TTS_CONTEXT())
  {
    int base = 1;
    TTS_EQUAL(by_context(base), 1);
    TTS_EQUAL(by_context(base) + 1, 1);
  };

  auto const report = report_of<true>(diverging);
  TTS_EXPECT(says(report, "by_context(base) == 1 is false, at run time 1 == 1."));
  TTS_EXPECT(says(report, "by_context(base) + 1 == 1 is false, at run time 2 != 1."));
};

TTS_CASE("A consteval failure names its expression alone")
{
  auto       diverging = [](TTS_CONTEXT()) { TTS_EQUAL(by_context(1), 1); };

  auto const report    = report_of<false>(diverging);
  TTS_EXPECT(says(report, "Constant evaluation: by_context(1) == 1 is false."));
};

TTS_CASE("The constant context refuses a body with a runtime call")
{
  auto with_runtime_call  = [](TTS_CONTEXT()) { TTS_EQUAL(runtime_only(1), 1); };
  auto with_constant_code = [](TTS_CONTEXT()) { TTS_EQUAL(by_context(1), 2); };

  TTS_EXPECT_NOT(::tts::_::constant_evaluable<decltype(with_runtime_call)>);
  TTS_EXPECT(::tts::_::constant_evaluable<decltype(with_constant_code)>);
};
