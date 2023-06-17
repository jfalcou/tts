//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <cstring>
#include <array>

namespace tts::detail
{
  template<typename T, std::size_t N>
  struct block
  {
    block() : nbelems{0} {}
    block(std::size_t sz) : storage{} ,nbelems{sz} {}

    std::size_t size()      const { return nbelems; }
    std::size_t capacity()  const { return N; }
    std::size_t empty()     const { return nbelems == 0; }

    void resize(std::size_t n)  { nbelems = n; }
    void push_back(T v)         { storage[nbelems] = v; nbelems++; }
    void push_front(T v)        { insert(begin(),v); }

    void insert(auto it, T v)
    {
      std::memmove(it+1, it, size()*sizeof(T));
      *it = v;
      nbelems++;
    }

    void clear() { nbelems = 0; }

    T&  back()       { return storage[size()-1]; }
    T   back() const { return storage[size()-1]; }

    T&  front()       { return storage[0]; }
    T   front() const { return storage[0]; }

    T&  operator[](std::size_t i)       { return storage[i]; }
    T   operator[](std::size_t i) const { return storage[i]; }

    auto begin()  { return  &storage[0]; }
    auto end()    { return begin() + nbelems; }

    auto begin()  const { return &storage[0]; }
    auto end()    const { return begin() + nbelems; }

    private:
    std::array<T,N> storage;
    std::size_t     nbelems;
  };
}
