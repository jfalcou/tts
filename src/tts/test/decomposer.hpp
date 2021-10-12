//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <string>
#include <tts/tools/as_string.hpp>
#include <tts/engine/comparators.hpp>

//==================================================================================================
// Expression decomposition
// This code is a direct adaptation of the similar technique used by Martin Moene in LEST
//==================================================================================================
namespace tts
{
  // Represent a test result and its displayable representation
  struct result
  {
    bool        status;
    std::string lhs,op,rhs;
    explicit operator bool() const { return status; }
  };

  // Carry value around up to display point inside test macro
  template<typename Expression> struct lhs_expr
  {
    Expression lhs;
    lhs_expr(Expression x) : lhs(x) {}
    lhs_expr(lhs_expr const &)            = delete;
    lhs_expr &operator=(lhs_expr const &) = delete;

    operator result() const { return result {bool(lhs),as_string(bool(lhs)),"",""}; }
    explicit operator bool() const { return bool(lhs); }

    template<typename R> result operator &&(R const &rhs)
    {
      return { lhs && rhs, as_string(lhs), "&&", as_string(rhs) };
    }

    template<typename R> result operator ||(R const &rhs)
    {
      return { lhs || rhs, as_string(lhs), "||", as_string(rhs) };
    }

    template<typename R> result operator ==(R const &rhs)
    {
      return { detail::eq(lhs, rhs), as_string(lhs), "==", as_string(rhs) };
    }

    template<typename R> result operator !=(R const &rhs)
    {
      return { !detail::eq(lhs, rhs), as_string(lhs), "!=", as_string(rhs) };
    }

    template<typename R> result operator <(R const &rhs)
    {
      return { detail::lt(lhs, rhs), as_string(lhs), "<", as_string(rhs) };
    }

    template<typename R> result operator <=(R const &rhs)
    {
      return { detail::lt(lhs, rhs) || detail::eq(lhs, rhs), as_string(lhs), "<=", as_string(rhs) };
    }

    template<typename R> result operator >(R const &rhs)
    {
      return { !detail::lt(lhs, rhs) && !detail::eq(lhs, rhs), as_string(lhs), ">", as_string(rhs) };
    }

    template<typename R> result operator >=(R const &rhs)
    {
      return { !detail::lt(lhs, rhs), as_string(lhs), ">=", as_string(rhs) };
    }
  };

  // Trampoline type for custom display of value injected from a macro.
  struct decomposer
  {
    template<typename Expression> lhs_expr<Expression const &> operator->*(Expression const &expr)
    {
      return {expr};
    }
  };
}

#define TTS_DECOMPOSE(XPR) (::tts::decomposer{}->*XPR)
