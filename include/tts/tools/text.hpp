//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/preprocessor.hpp>
#include <compare>
#include <cstdio>
#include <cstring>
#include <cstdlib>

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE

namespace tts
{
  //====================================================================================================================
  /**
    @ingroup tools-text
    @brief Lightweight string-like object

    `tts::text` is a simple string-like object that can be used to represent and manipulate text
  data in TTS. It provides basic functionalities such as construction from formatted strings,
  concatenation, and stream output support

    @groupheader{Example}
    @snippet doc/text.cpp snippet

    @see tts::as_text
  **/
  //====================================================================================================================
  struct text
  {
    /// Default constructor
    text()
        : data_(nullptr)
        , size_(0)
    {
    }

    /// Construct from C-style string
    explicit text(char const* ptr)
        : text()
    {
      if(ptr)
      {
        std::size_t len = strlen(ptr);
        if(len > 0)
        {
          data_ = reinterpret_cast<char*>(malloc(len + 1));
          if(data_)
          {
            size_ = static_cast<int>(len);
            memcpy(data_, ptr, size_);
            data_[ size_ ] = '\0';
          }
        }
      }
    }

    /// Construct from fixed-size C-style string
    template<std::size_t N>
    explicit text(char const (&data)[ N ])
        : text("%.s*", N, &data[ 0 ])
    {
    }

    /**
      @brief Construct from formatted string

      tts::text is constructible from a C-style format string and a variable number of arguments.

      @param format C-style format string
      @param args    Arguments to format according to `format`
    **/
    template<typename... Args>
    explicit text(char const* format, Args... args)
        : text()
    {
      // Store result in local int to check for errors/negatives
      int len = snprintf(nullptr, 0, format, args...);
      if(len > 0)
      {
        data_ = reinterpret_cast<char*>(malloc(len + 1));
        if(data_)
        {
          size_ = len; // Only assign size if alloc succeeds
          snprintf(data_, size_ + 1, format, args...);
        }
      }
    }

    /// Copy constructor
    text(text const& other)
        : text()
    {
      // Strictly check for positive size and valid pointer to prevent allocation errors
      if(other.size_ > 0 && other.data_)
      {
        data_ = reinterpret_cast<char*>(malloc(other.size_ + 1));
        if(data_)
        {
          size_ = other.size_;
          memcpy(data_, other.data_, size_);
          data_[ size_ ] = '\0';
        }
      }
    }

    /// Move constructor
    text(text&& other) noexcept
        : text()
    {
      swap(other);
    }

    /// Copy assignment operator
    text& operator=(text const& other)
    {
      text local(other);
      swap(local);
      return *this;
    }

    /// Move assignment operator
    text& operator=(text&& other) noexcept
    {
      text local(std::move(other));
      swap(local);
      return *this;
    }

    /// Destructor
    ~text()
    {
      if(data_) free(data_);
    }

    /// Swap contents with another text instance
    void swap(text& o) noexcept
    {
      std::swap(o.data_, data_);
      std::swap(o.size_, size_);
    }

    /// Concatenate another text instance
    text& operator+=(text const& other)
    {
      text local {"%.*s%.*s", size_, data(), other.size_, other.data()};
      swap(local);
      return *this;
    }

    /// Concatenate a C-style string
    text& operator+=(char const* other)
    {
      if(other)
      {
        text local {"%.*s%s", size_, data(), other};
        swap(local);
      }
      return *this;
    }

    /// Concatenate and return a new text instance
    text operator+(text const& other) const
    {
      text local(*this);
      return local += other;
    }

    /// Stream output operator
    template<_::stream OS> friend OS& operator<<(OS& os, text const& t)
    {
      if(t.data_)
      {
        for(int i = 0; i < t.size_; ++i) os << t.data_[ i ];
      }
      return os;
    }

    /// Check if the text is empty
    bool is_empty() const { return size_ == 0; }

    /// Get the size of the text
    int size() const { return size_; }

    /// Get a pointer to the constant underlying character data
    /// Returns an empty string literal if data_ is null to prevent %s crashes
    [[nodiscard]] char const* data() const { return data_ ? data_ : ""; }

    /// Get a pointer to the underlying character data
    [[nodiscard]] char* data() { return data_; }

    /// Get a const iterator to the beginning of the text
    decltype(auto) begin() const { return data_; }

    /// Get an iterator to the beginning of the text
    decltype(auto) begin() { return data_; }

    /// Get a const iterator to the end of the text
    decltype(auto) end() const { return data_ + size_; }

    /// Get an iterator to the end of the text
    decltype(auto)     end() { return data_ + size_; }

    friend auto const& to_text(text const& t) { return t; }

    /// Equality comparison operator
    friend bool operator==(text const& a, text const& b) noexcept
    {
      if(a.size_ != b.size_) return false;
      if(a.is_empty()) return true;
      return strcmp(a.data_, b.data_) == 0;
    }

    /// Equality comparison operator with C-style string
    template<std::size_t N> friend bool operator==(text const& a, char const (&b)[ N ]) noexcept
    {
      if(a.is_empty()) return N == 1;
      return strcmp(a.data_, &b[ 0 ]) == 0;
    }

    /// Three-way comparison operator with other text instance
    friend std::strong_ordering operator<=>(text const& a, text const& b) noexcept
    {
      if(a.is_empty() && b.is_empty()) return std::strong_ordering::equal;
      if(a.is_empty()) return std::strong_ordering::less;
      if(b.is_empty()) return std::strong_ordering::greater;

      int const size = a.size_ < b.size_ ? a.size_ : b.size_;
      int const cmp  = strncmp(a.data_, b.data_, size);

      if(cmp != 0) return cmp <=> 0;
      return a.size_ <=> b.size_;
    }

    /// Three-way comparison operator with C-style string
    template<std::size_t N> friend auto operator<=>(text const& a, char const (&b)[ N ]) noexcept
    {
      return a <=> text {b};
    }

  private:
    char* data_;
    int   size_;
  };

  /// Concatenate text with C-style string
  inline text operator+(text const& lhs, char const* rhs)
  {
    text that(lhs);
    return that += rhs;
  }

  /// Concatenate text with C-style string
  inline text operator+(char const* lhs, text const& rhs)
  {
    text that(lhs);
    return that += rhs;
  }
}

TTS_DISABLE_WARNING_POP
