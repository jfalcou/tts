//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/preprocessor.hpp>

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE

namespace tts
{
  struct text
  {
    text() : data_(nullptr), size_(0) {}

    explicit text(const char* ptr) : text()
    {
      size_ = static_cast<int>(strlen(ptr));
      if(size_)
      {
        data_ = reinterpret_cast<char*>(malloc(size_+1));
        strncpy(data_, ptr, size_);
        data_[size_] = '\0';
      }
    }

    template<std::size_t N>
    explicit text(const char (&data)[N]) : text("%.s*",N,&data[0])
    {}

    template<typename ... Args>
    explicit text( const char* format, Args ... args ) : text()
    {
      size_ = snprintf( nullptr, 0, format, args ... );
      if( size_ > 0 )
      {
        data_ = reinterpret_cast<char*>(malloc(size_+ 1));
        snprintf( data_, size_ + 1, format, args ... );
      }
    }

    text(text const& other) : text()
    {
      size_ = other.size_;
      if(size_)
      {
        data_ = reinterpret_cast<char*>(malloc(size_+1));
        strncpy(data_, other.data_, size_);
        data_[size_] = '\0';
      }
    }

    text(text&& other) : text() { swap(other); }

    text& operator=(text const& other)
    {
      text local(other);
      swap(local);
      return *this;
    }

    text& operator=(text&& other)
    {
      text local(std::move(other));
      swap(local);
      return *this;
    }

    ~text() { if(data_) free(data_); }

    void swap(text& o)
    {
      std::swap(o.data_,data_);
      std::swap(o.size_,size_);
    }

    text& operator+=(text const& other)
    {
      text local{"%.*s%.*s",size_,data_, other.size_,other.data_};
      swap(local);
      return *this;
    }

    text& operator+=(const char* other)
    {
      if(other)
      {
        text local{"%.*s%s",size_,data_, other};
        swap(local);
      }
      return *this;
    }

    text operator+(text const& other) const
    {
      text local(*this);
      return local += other;
    }

    template<_::stream OS>
    friend OS& operator<<(OS& os, text const& t)
    {
      for(int i=0;i<t.size_;++i) os << t.data_[i];
      return os;
    }

    bool is_empty()       const   { return size_ == 0;  }
    int  size()           const   { return size_;       }
    decltype(auto) data() const   { return data_;       }
    decltype(auto) data()         { return data_;       }
    decltype(auto) begin() const  { return data_;       }
    decltype(auto) begin()        { return data_;       }
    decltype(auto) end() const    { return data_+size_; }
    decltype(auto) end()          { return data_+size_; }

    friend auto const& to_text(text const& t) { return t; }

    friend auto operator==(text const& a, text const& b) noexcept
    {
      return strcmp(a.data_, b.data_) == 0;
    }

    template<std::size_t N>
    friend auto operator==(text const& a, const char (&b)[N]) noexcept
    {
      return strcmp(a.data_, &b[0]) == 0;
    }

    friend auto operator<=>(text const& a, text const& b) noexcept
    {
      return strncmp(a.data_, b.data_, a.size_<b.size_?a.size_:b.size_) <=> 0;
    }

    template<std::size_t N>
    friend auto operator<=>(text const& a, const char (&b)[N]) noexcept
    {
      return a <=> text{b};
    }

    private:
    char* data_;
    int   size_;
  };

  inline text operator+(text const& lhs, const char* rhs)
  {
    text that(lhs);
    return that += rhs;
  }

  inline text operator+(const char* lhs, text const& rhs)
  {
    text that(lhs);
    return that += rhs;
  }
}

TTS_DISABLE_WARNING_POP