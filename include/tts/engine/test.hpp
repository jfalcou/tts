//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/buffer.hpp>
#include <tts/tools/clock.hpp>
#include <tts/tools/erased.hpp>
#include <tts/tools/text.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @brief What a given @ref TTS_CASE is expected to produce.

    Defaults to `pass` for a plain @ref TTS_CASE. Set to anything else by wrapping the case's ID in
    @ref TTS_XFAIL, @ref TTS_MAYFAIL or @ref TTS_XINVALID.
  **/
  //====================================================================================================================
  enum class expected_outcome
  {
    pass,     ///< The case is expected to pass, like a plain untagged @ref TTS_CASE.
    xfail,    ///< The case is expected to fail - if it passes instead, that's reported.
    may_fail, ///< The case may pass or fail, either is accepted - only an empty case is reported.
    xinvalid  ///< The case is expected to be empty (register no assertion at all).
  };
}

namespace tts::_
{
  // A case deadline, in milliseconds.

  inline char const* current_test = "";

  // Carries a TTS_XFAIL/TTS_MAYFAIL/TTS_XINVALID tag without changing TTS_CASE(ID)'s call shape.
  struct tagged_id
  {
    char const*             name;
    ::tts::expected_outcome tag;
    milliseconds            timeout_ms  = 0;
    bool                    timeout_set = false;
  };

  inline char const* tag_name(::tts::expected_outcome tag)
  {
    using enum ::tts::expected_outcome;
    switch(tag)
    {
    case pass: return "PASS";
    case xfail: return "XFAIL";
    case may_fail: return "MAYFAIL";
    case xinvalid: return "XINVALID";
    }
    return ""; // NOSONAR - unreachable, expected_outcome only ever holds one of the 4 cases above
  }

  struct test
  {
    void operator()()
    {
      current_test = name;
      behaviour();
    }
    static inline bool      acknowledge(test&& f);

    char const*             name;
    tts::_::callable        behaviour;
    tts::text               types       = {};
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    milliseconds            timeout_ms  = 0;
    bool                    timeout_set = false;
  };

  // Global tests suite
  inline buffer<test>& suite()
  {
    static buffer<test> that = {};
    return that;
  }

  bool inline test::acknowledge(test&& f)
  {
    suite().emplace_back(TTS_MOVE(f));
    return true;
  }
}

namespace tts
{
  /// Tags a @ref TTS_CASE's ID as expected to fail - see @ref TTS_XFAIL.
  inline _::tagged_id expect_fail(char const* id)
  {
    return {id, expected_outcome::xfail};
  }

  /// Tags an already tagged ID as expected to fail, keeping whatever deadline it carries.
  inline _::tagged_id expect_fail(_::tagged_id const& id)
  {
    return {id.name, expected_outcome::xfail, id.timeout_ms, id.timeout_set};
  }

  /// Tags a @ref TTS_CASE's ID as allowed to pass or fail - see @ref TTS_MAYFAIL.
  inline _::tagged_id may_fail(char const* id)
  {
    return {id, expected_outcome::may_fail};
  }

  /// Tags an already tagged ID as allowed to fail, keeping whatever deadline it carries.
  inline _::tagged_id may_fail(_::tagged_id const& id)
  {
    return {id.name, expected_outcome::may_fail, id.timeout_ms, id.timeout_set};
  }

  /// Tags a @ref TTS_CASE's ID as expected to be empty - see @ref TTS_XINVALID.
  inline _::tagged_id expect_invalid(char const* id)
  {
    return {id, expected_outcome::xinvalid};
  }

  /// Tags an already tagged ID as expected to be empty, keeping whatever deadline it carries.
  inline _::tagged_id expect_invalid(_::tagged_id const& id)
  {
    return {id.name, expected_outcome::xinvalid, id.timeout_ms, id.timeout_set};
  }

  /// Gives a @ref TTS_CASE's ID a deadline in milliseconds - see @ref TTS_TIMEOUT.
  inline _::tagged_id with_timeout(milliseconds ms, char const* id)
  {
    return {id, expected_outcome::pass, ms, true};
  }

  /// Gives an already tagged ID a deadline, keeping its tag - see @ref TTS_TIMEOUT.
  inline _::tagged_id with_timeout(milliseconds ms, _::tagged_id const& id)
  {
    return {id.name, id.tag, ms, true};
  }
}
