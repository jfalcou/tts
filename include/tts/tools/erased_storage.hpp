//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts::_
{
  struct erased_storage
  {
    using cleanup_t  = void (*)(void*);

    erased_storage() = default;

    erased_storage(void* data, cleanup_t how) // NOSONAR - this is the erasure
        : payload {data}
        , cleanup {how}
    {
    }

    erased_storage(erased_storage&& other) noexcept
        : payload {other.payload}
        , cleanup {other.cleanup}
    {
      other.payload = nullptr;
    }

    erased_storage& operator=(erased_storage&& other) noexcept
    {
      if(payload) cleanup(payload);

      payload       = other.payload;
      cleanup       = other.cleanup;
      other.payload = nullptr;

      return *this;
    }

    erased_storage(erased_storage const&)            = delete;
    erased_storage& operator=(erased_storage const&) = delete;

    ~erased_storage()
    {
      if(payload) cleanup(payload);
    }

    explicit operator bool() const
    {
      return payload != nullptr;
    }

    template<typename T> static void destroy(void* data) // NOSONAR
    {
      delete static_cast<T*>(data); // NOSONAR Type erasure: delete is safe
    }

    static void destroy_nothing(void*) // NOSONAR
    {
      // Nothing to free.
    }

    void*     payload = nullptr;
    cleanup_t cleanup = nullptr;
  };
}
