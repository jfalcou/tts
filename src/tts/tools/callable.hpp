//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <utility>

namespace tts
{
  template <typename> struct callable;

  template <typename Return, typename... Params>
  struct callable<Return(Params...)>
  {
  public:

    using signature_t = Return(*)(void*, Params...);
    using deleter_t   = void(*)(void*);

    signature_t invoker = {}; // Type erased invoke call functions
    deleter_t   cleanup = {}; // Type erased cleanup of payload
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

    constexpr Return operator()(Params... args)       { return invoker(payload, args...); }
    constexpr Return operator()(Params... args) const { return invoker(payload, args...); }

    private:

    template <typename T>
    static Return invoke(void* data, Params... args) { return (*static_cast<T*>(data))(args...); }

    template <typename T>
    static void destroy(void* data) { delete static_cast<T*>(data); }
  };
}
