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
  template<typename T> class buffer
  {
  public:
    buffer()
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
    {
    }

    buffer(std::size_t n)
        : buffer()
    {
      if(n > 0)
      {
        data_     = reinterpret_cast<T *>(malloc(sizeof(T) * n));
        size_     = n;
        capacity_ = n;
      }
    }

    buffer(std::size_t n, T val)
        : buffer(n)
    {
      for(std::size_t i = 0; i < n; ++i) data_[ i ] = val;
    }

    ~buffer()
    {
      if(data_)
      {
        for(std::size_t i = 0; i < size_; ++i) (&data_[ i ])->~T();
        free(data_);
      }
    }

    buffer(const buffer &other)
        : buffer(other.size_)
    {
      for(std::size_t i = 0; i < size_; ++i) new(data_ + i) T(TTS_MOVE(other.data_[ i ]));
    }

    buffer(buffer &&other) noexcept
        : buffer()
    {
      swap(other);
    }

    buffer &operator=(const buffer &other)
    {
      buffer local(other);
      swap(local);
      return *this;
    }

    buffer &operator=(buffer &&other) noexcept
    {
      buffer local(TTS_MOVE(other));
      swap(local);
      return *this;
    }

    void push_back(T const &value)
    {
      ensure_capacity(size_ + 1);
      data_[ size_++ ] = value;
    }

    void push_back(T &&value)
    {
      ensure_capacity(size_ + 1);
      data_[ size_++ ] = TTS_MOVE(value);
    }

    template<typename... Args> void emplace_back(Args &&...args) { push_back(T(TTS_FWD(args)...)); }

    T  operator[](std::size_t i) const { return data_[ i ]; }
    T &operator[](std::size_t i) { return data_[ i ]; }

    T const *data() const { return data_; }
    T       *data() { return data_; }

    decltype(auto) begin() const { return data_; }
    decltype(auto) begin() { return data_; }
    decltype(auto) end() const { return data_ + size_; }
    decltype(auto) end() { return data_ + size_; }

    bool        empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }

    void swap(buffer &other)
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
    }

  private:
    std::size_t size_;
    std::size_t capacity_;
    T          *data_;

    // Ensure enough capacity for new elements
    void ensure_capacity(std::size_t new_capacity)
    {
      if(new_capacity > capacity_)
      {
        std::size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        while(new_cap < new_capacity) { new_cap *= 2; }

        T *new_data = reinterpret_cast<T *>(malloc(sizeof(T) * new_cap));

        for(std::size_t i = 0; i < size_; ++i) new_data[ i ] = TTS_MOVE(data_[ i ]);

        free(data_);
        data_     = new_data;
        capacity_ = new_cap;
      }
    }
  };
}
