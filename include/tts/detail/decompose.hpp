//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_DECOMPOSE_HPP_INCLUDED
#define TTS_DETAIL_DECOMPOSE_HPP_INCLUDED

#include <tts/detail/comparator.hpp>
#include <tts/detail/rt_helpers.hpp>

namespace tts
{
  namespace detail
  {
    // Carry value around up to display point inside test macro
    template<typename Expression> struct lhs_expr
    {
      Expression lhs;

      lhs_expr(Expression x)
          : lhs(x)
      {
      }

      lhs_expr(lhs_expr const &) = delete;
      lhs_expr &operator=(lhs_expr const &) = delete;

      operator result()
      {
        return result {bool(lhs),
                       ::tts::detail::to_string(bool(lhs)),
                       ::tts::detail::to_string(""),
                       ::tts::detail::to_string("")};
      }

#define TTS_BINARY_DECOMPOSE(OP, SB, FN)                                                           \
  template<typename R> result operator OP(R const &rhs)                                            \
  {                                                                                                \
    return {tts::detail::FN(lhs, rhs),                                                             \
            ::tts::detail::to_string(lhs),                                                         \
            ::tts::detail::split_line(lhs, rhs, SB),                                               \
            ::tts::detail::to_string(rhs)};                                                        \
  }                                                                                                \
  /**/

      TTS_BINARY_DECOMPOSE(==, "==", eq)
      TTS_BINARY_DECOMPOSE(!=, "!=", neq)
      TTS_BINARY_DECOMPOSE(<, "<", lt)
      TTS_BINARY_DECOMPOSE(>, ">", gt)
      TTS_BINARY_DECOMPOSE(>=, ">=", ge)
      TTS_BINARY_DECOMPOSE(<=, "<=", le)

#undef TTS_BINARY_DECOMPOSE
    };

    /*!
      @brief Trampoline type for custom display of value injected from a macro.
      @ingroup group-details

      When a macro like TTS_EXPECT( a == b ) is called, it's often useful to be able to
      display the value of a and b without having to butcher the macro API. The decomposer
      type is used to provide a member pointer access overload able to catch arbitrary binary
      expression and forward them to the display system.

      @note This code is a direct adaptation of the similar technique used by Martin Moene in LEST
    **/
    struct decomposer
    {
      template<typename Expression> lhs_expr<Expression const &> operator->*(Expression const &expr)
      {
        return {expr};
      }
    };
  }
}

/*!
  @brief Macro decomposing a test expression for display
  @ingroup group-details

  @param XPR Expression to display
**/
#define TTS_DECOMPOSE(XPR) (tts::detail::decomposer()->*XPR)

#endif
