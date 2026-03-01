//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <type_traits>
#include <cassert>

namespace tts::_
{
  template<typename T> class buffer
  {
  public:
    buffer()
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
    {
    }

    explicit buffer(std::size_t n)
        : buffer()
    {
      if(n > 0)
      {
        data_ = static_cast<T*>(malloc(sizeof(T) * n)); // NOSONAR
        assert(data_ && "tts::buffer out of memory");

        size_     = n;
        capacity_ = n;
        for(std::size_t i = 0; i < n; ++i)
          new(data_ + i) T();
      }
    }

    buffer(std::size_t n, T val)
        : buffer()
    {
      if(n > 0)
      {
        data_ = static_cast<T*>(malloc(sizeof(T) * n)); // NOSONAR
        assert(data_ && "tts::buffer out of memory");

        size_     = n;
        capacity_ = n;
        for(std::size_t i = 0; i < n; ++i)
          new(data_ + i) T(val);
      }
    }

    ~buffer()
    {
      if(data_)
      {
        if constexpr(!std::is_trivially_destructible_v<T>)
        {
          for(std::size_t i = 0; i < size_; ++i)
            (data_ + i)->~T(); // NOSONAR - T may be non-trivial
        }
        free(data_); // NOSONAR
      }
    }

    buffer(buffer const& other)
        : buffer()
    {
      if(other.size_ > 0)
      {
        data_ = static_cast<T*>(malloc(sizeof(T) * other.size_)); // NOSONAR
        assert(data_ && "tts::buffer out of memory");

        size_     = other.size_;
        capacity_ = other.size_;
        for(std::size_t i = 0; i < size_; ++i)
          new(data_ + i) T(other.data_[ i ]);
      }
    }

    buffer(buffer&& other) noexcept
        : buffer()
    {
      swap(other);
    }

    buffer& operator=(buffer const& other)
    {
      buffer local(other);
      swap(local);
      return *this;
    }

    buffer& operator=(buffer&& other) noexcept
    {
      buffer local(TTS_MOVE(other));
      swap(local);
      return *this;
    }

    void push_back(T const& value)
    {
      ensure_capacity(size_ + 1);
      new(data_ + size_++) T(value);
    }

    void push_back(T&& value)
    {
      ensure_capacity(size_ + 1);
      new(data_ + size_++) T(TTS_MOVE(value));
    }

    template<typename... Args> void emplace_back(Args&&... args)
    {
      ensure_capacity(size_ + 1);
      new(data_ + size_++) T(TTS_FWD(args)...);
    }

    T operator[](std::size_t i) const
    {
      return data_[ i ];
    }
    T& operator[](std::size_t i)
    {
      return data_[ i ];
    }

    T const* data() const
    {
      return data_;
    }
    T* data()
    {
      return data_;
    }

    decltype(auto) begin() const
    {
      return data_;
    }
    decltype(auto) begin()
    {
      return data_;
    }
    decltype(auto) end() const
    {
      return data_ + size_;
    }
    decltype(auto) end()
    {
      return data_ + size_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }
    std::size_t size() const noexcept
    {
      return size_;
    }
    std::size_t capacity() const noexcept
    {
      return capacity_;
    }

    void swap(buffer& other) noexcept
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
    }

  private:
    std::size_t size_;
    std::size_t capacity_;
    T*          data_;

    void        ensure_capacity(std::size_t new_capacity)
    {
      if(new_capacity > capacity_)
      {
        std::size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        while(new_cap < new_capacity)
          new_cap *= 2;

        auto new_data = static_cast<T*>(malloc(sizeof(T) * new_cap)); // NOSONAR
        assert(new_data && "tts::buffer out of memory");

        for(std::size_t i = 0; i < size_; ++i)
        {
          new(new_data + i) T(TTS_MOVE(data_[ i ]));
          if constexpr(!std::is_trivially_destructible_v<T>)
            (data_ + i)->~T(); // NOSONAR - T may be non-trivial
        }

        free(data_); // NOSONAR
        data_     = new_data;
        capacity_ = new_cap;
      }
    }
  };
}
