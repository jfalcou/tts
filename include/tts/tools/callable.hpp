//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>

namespace tts::_
{
  struct callable
  {
    public:

    using signature_t = void(*)(void*);

    signature_t invoker = {}; // Type erased invoke call functions
    signature_t cleanup = {}; // Type erased cleanup of payload
    void*       payload = {}; // Function + function state

    callable(): invoker{nullptr}, cleanup{nullptr}, payload{nullptr} {}

    // Optimized path for simple function pointers (used by TTS_CASE)
    // Avoids template instantiation and heap allocation for stateless tests
    callable(void(*f)())
            : invoker{invoke_ptr}, cleanup{cleanup_ptr}
            , payload{reinterpret_cast<void*>(f)}
    {}

    // Copy/transfer the function as the unknown payload holding states
    // We could have have used std::any but you know, compile-time
    template<typename Function>
    callable(Function f)
            : invoker{invoke<Function>}, cleanup{destroy<Function>}
            , payload{new Function{TTS_MOVE(f)}}
    {}

    constexpr callable(callable&& other) noexcept
            : invoker{TTS_MOVE(other.invoker)}, cleanup{TTS_MOVE(other.cleanup)}
            , payload{TTS_MOVE(other.payload)}
    {
      other.payload = {};
    }

    ~callable() { if(payload) cleanup(payload); }

    callable(const callable&)             = delete;
    callable& operator=(const callable&)  = delete;

    callable& operator=(callable&& other)
    {
      payload = TTS_MOVE(other.payload);
      other.payload = {};
      invoker = TTS_MOVE(other.invoker);
      cleanup = TTS_MOVE(other.cleanup);

      return *this;
    }

    void operator()()       { assert(payload); invoker(payload); }
    void operator()() const { assert(payload); invoker(payload); }

    explicit operator bool() const { return payload != nullptr; }

    private:
    template<typename T> static void invoke(void* data)   { (*static_cast<T*>(data))();   }
    template<typename T> static void destroy(void* data)  { delete static_cast<T*>(data); }

    // Static helpers for the function pointer path
    static void invoke_ptr(void* data)  { reinterpret_cast<void(*)()>(data)(); }
    static void cleanup_ptr(void*)      {}
  };
}
