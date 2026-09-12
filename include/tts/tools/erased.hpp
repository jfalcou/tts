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
  template<typename Signature> struct erased;

  //====================================================================================================================
  // A callable of any signature, stored without knowing its type. std::function is not an option
  // here, see doc/compile_time.hpp.
  //====================================================================================================================
  template<typename R, typename... Args> struct erased<R(Args...)> // NOSONAR - erasure needs void*
  {
    using signature_t = R (*)(void*, Args...);
    using cleanup_t   = void (*)(void*);

    erased()          = default;

    // Optimized path for simple function pointers (used by TTS_CASE)
    // Avoids template instantiation and heap allocation for stateless tests
    erased(R (*f)(Args...))                    // NOSONAR
        : payload {reinterpret_cast<void*>(f)} // NOSONAR Type erasure
        , cleanup {destroy_nothing}
        , invoker {invoke_ptr}
    {
    }

    // Copy/transfer the function as the unknown payload holding states
    template<typename Function>
    erased(Function f)                         // NOSONAR
        : payload {new Function {TTS_MOVE(f)}} // NOSONAR Type erasure
        , cleanup {destroy<Function>}
        , invoker {invoke<Function>}
    {
    }

    erased(erased&& other) noexcept
        : payload {other.payload}
        , cleanup {other.cleanup}
        , invoker {other.invoker}
    {
      other.payload = nullptr;
    }

    erased& operator=(erased&& other) noexcept
    {
      if(payload) cleanup(payload);

      payload       = other.payload;
      cleanup       = other.cleanup;
      invoker       = other.invoker;
      other.payload = nullptr;

      return *this;
    }

    erased(erased const&)            = delete;
    erased& operator=(erased const&) = delete;

    ~erased()
    {
      if(payload) cleanup(payload);
    }

    R operator()(Args... args) const
    {
      assert(payload);
      return invoker(payload, args...);
    }

    explicit operator bool() const
    {
      return payload != nullptr;
    }

    void*       payload = nullptr;
    cleanup_t   cleanup = nullptr;
    signature_t invoker = nullptr;

  private:
    template<typename T> static R invoke(void* data, Args... args) // NOSONAR Type erasure
    {
      return (*static_cast<T*>(data))(args...);
    }

    static R invoke_ptr(void* data, Args... args) // NOSONAR Type erasure
    {
      return reinterpret_cast<R (*)(Args...)>(data)(args...); // NOSONAR Type erasure
    }

    template<typename T> static void destroy(void* data) // NOSONAR
    {
      delete static_cast<T*>(data); // NOSONAR Type erasure: delete is safe
    }

    static void destroy_nothing(void*) // NOSONAR
    {
      // Nothing to free.
    }
  };

  using callable = erased<void()>;
}
