//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/check.hpp>
#include <tts/engine/environment.hpp>
#include <tts/engine/logger.hpp>
#include <tts/tools/as_text.hpp>
#include <tts/tools/buffer.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/output.hpp>
#include <tts/tools/text.hpp>
#include <tts/tools/types.hpp>

// The contexts a case body runs in, and what a check records into in each of them. A check body
// receives a context through local_tts_ctx(): the runtime context forwards to the counters and the
// sinks, the constant context is a literal recorder read back once the evaluation is over.
namespace tts::_
{
  //====================================================================================================================
  // Runtime context: the counters and the sinks of the whole run. In probe mode it reports nothing
  // and keeps the operands of the checks the constant context failed on, for their message.
  //====================================================================================================================
  struct probed_operands
  {
    std::size_t ordinal;
    text        lhs;
    text        rhs;
    bool        holds = false;
  };

  struct runtime_context
  {
    runtime_context& operator()()
    {
      return *this;
    }

    template<typename Message> logger pass(check_site const& site, Message const& message)
    {
      if(probing) return probed_verdict(true);
      ++ordinal;
      if(check_passed()) check_report(site, message());
      return logger {false};
    }

    template<typename Message> logger fail(check_site const& site, Message const& message)
    {
      if(probing) return probed_verdict(false);
      ++ordinal;
      return check_failed(site, message());
    }

    template<typename A, typename B> void operands(A const& a, B const& b)
    {
      if(!probing || !wanted(ordinal)) return;
      probed.emplace_back(ordinal, as_text(a), as_text(b));
    }

    logger probed_verdict(bool holds)
    {
      if(!probed.empty() && probed[ probed.size() - 1 ].ordinal == ordinal)
        probed[ probed.size() - 1 ].holds = holds;
      ++ordinal;
      return logger {false};
    }

    bool wanted(std::size_t which) const
    {
      for(std::size_t i = 0; i < wanted_count; ++i)
        if(wanted_ordinals[ i ] == which) return true;
      return false;
    }

    probed_operands const* probed_at(std::size_t which) const
    {
      for(auto const& p: probed)
        if(p.ordinal == which) return &p;
      return nullptr;
    }

    std::size_t             ordinal         = 0;
    bool                    probing         = false;
    std::size_t const*      wanted_ordinals = nullptr;
    std::size_t             wanted_count    = 0;
    buffer<probed_operands> probed          = {};
  };

  inline runtime_context current_runtime = {}; // NOSONAR - the checks of every case move it

  //====================================================================================================================
  // Constant context: counts the passes and keeps the sites of the failures during a constant
  // evaluation. A literal type, so that a constexpr variable can hold it.
  //====================================================================================================================
  struct constant_logger
  {
    template<typename Data> constexpr constant_logger& operator<<(Data const&)
    {
      return *this;
    }
  };

  struct constant_failure
  {
    check_site  site;
    std::size_t ordinal;
  };

  struct constant_context
  {
    static constexpr std::size_t capacity = 64;

    constexpr constant_context&  operator()()
    {
      return *this;
    }

    template<typename Message> constexpr constant_logger pass(check_site const&, Message const&)
    {
      ++passes;
      return {};
    }

    template<typename Message>
    constexpr constant_logger fail(check_site const& site, Message const&)
    {
      if(failures < capacity) failed[ failures ] = {site, passes + failures};
      ++failures;
      return {};
    }

    template<typename A, typename B> constexpr void operands(A const&, B const&) const
    {
      // Nothing to keep: the values of a failed check are read by the probe, at run time.
    }

    std::size_t      passes             = 0;
    std::size_t      failures           = 0;
    constant_failure failed[ capacity ] = {}; // NOSONAR - <array> is refused for the compile cost
  };

  //====================================================================================================================
  // What a body is called with, besides its context: nothing for a TTS_CASE, the type for a
  // TTS_CASE_TPL, built inside the call so that the constant evaluation holds it.
  //====================================================================================================================
  struct direct
  {
    static constexpr void apply(auto& body, auto& ctx)
    {
      body(ctx);
    }
  };

  template<typename T> struct with_type
  {
    static constexpr void apply(auto& body, auto& ctx)
    {
      body(ctx, type<T> {});
    }
  };

  // A captureless closure is default-constructible since C++20, which is what lets a body written
  // in a test file be called inside a constant expression here.
  template<typename Body, typename Maker = direct> constexpr constant_context evaluate()
  {
    constant_context result {};
    Body             body {};
    Maker::apply(body, result);
    return result;
  }

  // Tells whether a body holds in constant evaluation without a hard error, for a test of what
  // the constant context refuses.
  template<typename Body, typename Maker = direct>
  concept constant_evaluable =
  requires { typename std::integral_constant<std::size_t, evaluate<Body, Maker>().passes>; };

  // The outcome of the constant evaluation of a body, held once per body and maker.
  template<typename Body, typename Maker>
  inline constexpr constant_context constant_outcome = evaluate<Body, Maker>();

  // The ordinals of the recorded failures, as a constexpr array the probe reads.
  template<typename Body, typename Maker, std::size_t Recorded> struct failed_ordinals
  {
    static constexpr auto make()
    {
      struct
      {
        std::size_t at[ Recorded ]; // NOSONAR - <array> is refused for the compile cost
      } that {};
      for(std::size_t i = 0; i < Recorded; ++i)
        that.at[ i ] = constant_outcome<Body, Maker>.failed[ i ].ordinal;
      return that;
    }
    static constexpr auto value = make();
  };

  // The message of a failed check: with the values the same check compares at run time when the
  // body ran there as well, and the relation those values satisfy.
  inline text constant_failure_message(check_site const& site, probed_operands const* probed)
  {
    if(site.op[ 0 ] == '\0') return text {"Constant evaluation: %s evaluates to false.", site.lhs};
    if(probed)
      return text {"Constant evaluation: %s %s %s is false, at run time %s %s %s.",
                   site.lhs,
                   site.op,
                   site.rhs,
                   probed->lhs.data(),
                   probed->holds ? site.op : site.negated,
                   probed->rhs.data()};
    return text {"Constant evaluation: %s %s %s is false.", site.lhs, site.op, site.rhs};
  }

  //====================================================================================================================
  // The two contexts a case names: each one runs a body its own way.
  //====================================================================================================================
  struct runtime_t
  {
    static constexpr char const*                                               name = "runtime";

    template<typename Maker = direct, bool = false, typename Body> static void run(Body body)
    {
      current_runtime.ordinal = 0;
      Maker::apply(body, current_runtime);
    }
  };

  struct constant_t
  {
    static constexpr char const* name = "constant";

    // Probe tells whether the body also runs at run time, where the operands of a failed check
    // are formatted for its message: a body of a constant context alone is never run that way.
    template<typename Maker = direct, bool Probe = false, typename Body> static void run(Body body)
    {
      constexpr constant_context const& outcome = constant_outcome<Body, Maker>;

      if(::tts::is_detailed())
        ::tts::output().writeln(">  In constant evaluation, %zu check%s passed",
                                outcome.passes,
                                outcome.passes > 1 ? "s" : "");

      for(std::size_t i = 0; i < outcome.passes; ++i)
        ::tts::global_runtime.pass();

      constexpr auto recorded =
      outcome.failures < constant_context::capacity ? outcome.failures : constant_context::capacity;
      if constexpr(recorded == 0) return;
      else
      {
        if constexpr(Probe)
        {
          constexpr auto const& ordinals  = failed_ordinals<Body, Maker, recorded>::value;

          current_runtime.probing         = true;
          current_runtime.wanted_ordinals = ordinals.at;
          current_runtime.wanted_count    = recorded;
          current_runtime.ordinal         = 0;
          Maker::apply(body, current_runtime);
          current_runtime.probing = false;
        }

        for(std::size_t i = 0; i < recorded; ++i)
        {
          auto const& failure = outcome.failed[ i ];
          check_failed(
          failure.site,
          constant_failure_message(failure.site, current_runtime.probed_at(failure.ordinal)));
        }
        current_runtime.probed = {};

        if(outcome.failures > recorded)
          check_failed(outcome.failed[ recorded - 1 ].site,
                       text {"Constant evaluation: %zu more failure%s past the %zu recorded.",
                             outcome.failures - recorded,
                             outcome.failures - recorded > 1 ? "s" : "",
                             recorded});
      }
    }
  };

  template<typename T>
  concept context = std::same_as<T, runtime_t> || std::same_as<T, constant_t>;

  template<typename... Contexts>
  inline constexpr bool has_runtime = (std::same_as<Contexts, runtime_t> || ...);
}

namespace tts
{
  //====================================================================================================================
  /**
    @ingroup test-scenario
    @brief Context running a case body as ordinary code.

    A plain @ref TTS_CASE runs in this context. @ref TTS_CASE_IN names the context, alone or
    with @ref tts::constant. The body is called like a function. The checks of the
    body count and report like those of a plain case.
  **/
  //====================================================================================================================
  inline constexpr _::runtime_t runtime = {};

  //====================================================================================================================
  /**
    @ingroup test-scenario
    @brief Context running a case body in a constant evaluation.

    @ref TTS_CASE_IN names the context, alone or with @ref tts::runtime, and @ref TTS_CONSTEXPR
    or @ref TTS_CONSTEVAL on the title of a case does the same. The body is evaluated in one
    constant expression. The checks are reported afterward: each pass
    counts, each failure names its file, its line, and its expression. If the case runs in
    @ref tts::runtime too, the body runs once more, silently, so that the message adds the
    values the failed check compares at run time. A body that calls a function the language
    refuses in a constant expression does not compile. The diagnostic names the call.

    The checks accepted in this context:
    - the basic checks, @ref TTS_EXPECT and @ref TTS_EXPECT_NOT.
    - the relation checks, @ref TTS_EQUAL and the others, typed or constexpr included.
    - the type checks, @ref TTS_TYPE_IS, @ref TTS_EXPR_IS, and @ref TTS_EXPECT_COMPILES.

    The precision, sequence, range, and exception checks run at run time alone, and so does a
    @ref TTS_CASE_WITH, whose values the generators produce at run time.
  **/
  //====================================================================================================================
  inline constexpr _::constant_t constant = {};
}

// Every check reads its context through this name. A body run in a named context shadows it with
// its parameter, which a function allows under -Wshadow where a variable does not.
inline ::tts::_::runtime_context& local_tts_ctx()
{
  return ::tts::_::current_runtime;
}
