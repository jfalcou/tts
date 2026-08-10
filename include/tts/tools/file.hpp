//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <cstdio>

namespace tts::_
{
  // Not std::unique_ptr: avoids <memory> - see doc/compile_time.hpp.
  TTS_DISABLE_WARNING_PUSH
  TTS_DISABLE_WARNING_CRT_SECURE
  class file_guard
  {
  public:
    file_guard() = default;

    explicit file_guard(FILE* f)
        : file_(f)
    {
    }

    file_guard(file_guard const&)            = delete;
    file_guard& operator=(file_guard const&) = delete;

    file_guard(file_guard&& other) noexcept
        : file_guard()
    {
      swap(other);
    }

    file_guard& operator=(file_guard&& other) noexcept
    {
      file_guard {TTS_MOVE(other)}.swap(*this);
      return *this;
    }

    ~file_guard()
    {
      close();
    }

    void swap(file_guard& other) noexcept
    {
      FILE* tmp   = file_;
      file_       = other.file_;
      other.file_ = tmp;
    }

    FILE* get() const
    {
      return file_;
    }
    explicit operator bool() const
    {
      return file_ != nullptr;
    }

  private:
    void close()
    {
      if(file_) fclose(file_);
      file_ = nullptr;
    }

    FILE* file_ = nullptr;
  };
  TTS_DISABLE_WARNING_POP
}
