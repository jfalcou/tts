//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <utility>

namespace tts::detail
{
  struct callable
  {
    public:

    using signature_t = void(*)(void*);

    signature_t invoker = {}; // Type erased invoke call functions
    signature_t cleanup = {}; // Type erased cleanup of payload
    void*       payload = {}; // Function + function state

    constexpr callable() = default;

    // Copy/transfer the function as the unknown payload holding states
    // We could have have used std::any but you know, compile-time
    template<typename Function>
    constexpr callable(Function f)
            : invoker{invoke<Function>}, cleanup{destroy<Function>}
            , payload{new Function{std::move(f)}}
    {}

    constexpr callable(callable&& other) noexcept
            : invoker{std::move(other.invoker)}, cleanup{std::move(other.cleanup)}
            , payload{std::move(other.payload)}
    {
      // Remove payload from other
      other.payload = {};
    }

    // Destroy payload if any
    ~callable() { cleanup(payload); }

    constexpr callable(const callable&)             = delete;
    constexpr callable& operator=(const callable&)  = delete;
    constexpr callable& operator=(callable&&)       = delete;

    constexpr void operator()()       { invoker(payload); }
    constexpr void operator()() const { invoker(payload); }

    explicit constexpr operator bool() const { return payload != nullptr; }

    private:

    template <typename T>
    static void invoke(void* data) { (*static_cast<T*>(data))(); }

    template <typename T>
    static void destroy(void* data) { delete static_cast<T*>(data); }
  };
}
