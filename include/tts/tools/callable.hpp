//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/erased_storage.hpp>
#include <tts/tools/preprocessor.hpp>

namespace tts::_
{
  struct callable : erased_storage // NOSONAR - erased_storage owns the payload and destroys it
  {
    using signature_t = void (*)(void*);

    callable()        = default;

    // Optimized path for simple function pointers (used by TTS_CASE)
    // Avoids template instantiation and heap allocation for stateless tests
    callable(void (*f)())                                               // NOSONAR
        : erased_storage {reinterpret_cast<void*>(f), &destroy_nothing} // NOSONAR Type erasure
        , invoker {invoke_ptr}
    {
    }

    // Copy/transfer the function as the unknown payload holding states
    template<typename Function>
    callable(Function f)                                                  // NOSONAR
        : erased_storage {new Function {TTS_MOVE(f)}, &destroy<Function>} // NOSONAR Type erasure
        , invoker {invoke<Function>}
    {
    }

    callable(callable&& other) noexcept
        : erased_storage {TTS_MOVE(other)}
        , invoker {other.invoker}
    {
    }

    callable& operator=(callable&& other) noexcept
    {
      erased_storage::operator=(TTS_MOVE(other));
      invoker = other.invoker;
      return *this;
    }

    void operator()() const
    {
      assert(payload);
      invoker(payload);
    }

    signature_t invoker = nullptr;

  private:
    template<typename T>
    static void invoke(void* data) // NOSONAR Type erasure: no need for a more complex solution
    {
      (*static_cast<T*>(data))();
    }

    static void invoke_ptr(void* data) // NOSONAR Type erasure
    {
      reinterpret_cast<void (*)()>(data)(); // NOSONAR Type erasure
    }
  };
}
