//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/deps.hpp>
#include <tts/tools/callable.hpp>
#include <tts/tools/erased_storage.hpp>

namespace tts::_
{
  struct abort_handler : erased_storage // NOSONAR - erased_storage owns the payload and destroys it
  {
    using signature_t = void (*)(void*, int);

    abort_handler()   = default;

    abort_handler(void (*f)(int))                                       // NOSONAR
        : erased_storage {reinterpret_cast<void*>(f), &destroy_nothing} // NOSONAR Type erasure
        , invoker {invoke_ptr}
    {
    }

    template<typename Function>
    abort_handler(Function f)                                             // NOSONAR
        : erased_storage {new Function {TTS_MOVE(f)}, &destroy<Function>} // NOSONAR Type erasure
        , invoker {invoke<Function>}
    {
    }

    abort_handler(abort_handler&& other) noexcept
        : erased_storage {TTS_MOVE(other)}
        , invoker {other.invoker}
    {
    }

    abort_handler& operator=(abort_handler&& other) noexcept
    {
      erased_storage::operator=(TTS_MOVE(other));
      invoker = other.invoker;
      return *this;
    }

    void operator()(int reason) const
    {
      assert(payload);
      invoker(payload, reason);
    }

    signature_t invoker = nullptr;

  private:
    template<typename T> static void invoke(void* data, int reason) // NOSONAR Type erasure
    {
      (*static_cast<T*>(data))(reason);
    }

    static void invoke_ptr(void* data, int reason) // NOSONAR Type erasure
    {
      reinterpret_cast<void (*)(int)>(data)(reason); // NOSONAR Type erasure
    }
  };

  inline callable          abort_epilogue = {}; // NOSONAR - the driver sets it once it is ready
  inline abort_handler     abort_action   = {}; // NOSONAR - set_abort_handler replaces it

  [[noreturn]] inline void exit_now()
  {
    // No atexit handler and no static destructor may run on a half-finished case, and _Exit
    // flushes nothing.
    fflush(stdout);
    fflush(stderr);
    std::_Exit(1);
  }

  [[noreturn]] inline void perform_abort(int reason)
  {
    if(abort_epilogue) abort_epilogue();
    if(abort_action) abort_action(reason);
    exit_now();
  }
}

namespace tts
{
  //====================================================================================================================
  /**
    @brief Run termination customization point
    @ingroup customization-points

    Runs just before **TTS** exits a crashed or timed out run, with the signal that brought it down
    or `0` when nothing did. **TTS** exits with 1 once it returns.

    Useful when a test binary has to handle an abrupt stop in its own way.

    @param h  Any callable taking an `int`.

    @return The handler installed before this call, empty if there was none.

    @see TTS_CUSTOM_DRIVER_FUNCTION

    @groupheader{Example}
    @snippet doc/abort_handler.cpp snippet
  **/
  //====================================================================================================================
  template<typename Handler> inline _::abort_handler set_abort_handler(Handler h)
  {
    _::abort_handler previous = TTS_MOVE(_::abort_action);
    _::abort_action           = _::abort_handler {TTS_MOVE(h)};
    return previous;
  }
}
