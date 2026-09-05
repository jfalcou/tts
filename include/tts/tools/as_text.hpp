//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/source_location.hpp>
#include <tts/tools/types.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-text Text Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /*!
    @ingroup tools-text
    @public
    @brief Value-to-string conversion

    When displaying the data required to understand a test's failures, TTS may need to print out
  value of various types. @ref as_text provides a centralized way to perform such a task by handling
  most common types including:
      + pointers
      + floating points
      + integers
      + sequences of such types

    @ref as_text takes care of applying any command-line options related to formatting to the value
  printed. For user defined types, an overload of a `to_text` function must be found via ADL.

    @groupheader{Examples}
    @snippet doc/as_text.cpp snippet

    @param e Value to convert to @ref tts::text
    @return An instance of @ref tts::text containing a representation of the value of e

    @see tts::text
  **/
  //====================================================================================================================
  // Declared before the base because the base recurses through it: the elements of a sequence
  // are rendered by as_text, and for a scalar element ADL alone would find nothing.
  template<typename T> text as_text(T const& e);

  namespace _
  {
    //==================================================================================================================
    // How TTS renders a value when nothing more specific is known. Pulled into a base so that a
    // specialization of tts::display only has to say what it changes.
    //==================================================================================================================
    template<typename T> struct builtin_display
    {
      static text render(T const& e)
      {
        static_assert(
        !requires { to_text(e); },
        "[TTS] tts::to_text is no longer a customization point. "
        "Specialize tts::display<T>::render instead.");

        if constexpr(std::floating_point<T>) return floating(e);
        else if constexpr(std::integral<T>) return integral(e);
        else if constexpr(_::string<T>)
          return text("'%.*s'", static_cast<int>(e.size()), e.data() ? e.data() : "");
        else if constexpr(_::optional<T>) return optional(e);
        else if constexpr(std::is_pointer_v<T>) return pointer(e);
        else if constexpr(_::sequence<T>) return sequence(e);
        else return bytes(e);
      }

    private:
      static text floating(T const& e)
      {
        auto precision = ::tts::arguments().value(16, "--precision");
        bool hexmode   = ::tts::arguments()("-x", "--hex");
        bool scimode   = ::tts::arguments()("-s", "--scientific");

        if(scimode) return text("%.*E", precision, e);
        else if(hexmode) return text("%#.*A", precision, e);
        else return text("%.*g", precision, e);
      }

      static text integral(T const& e)
      {
        if constexpr(sizeof(T) > 4)
        {
          auto fmt = ::tts::arguments()("-x", "--hex") ? "%lX" : "%ld";
          return text(fmt, e);
        }
        else
        {
          auto fmt = ::tts::arguments()("-x", "--hex") ? "%X" : "%d";
          return text(fmt, e);
        }
      }

      static text optional(T const& e)
      {
        // Safe access to type name
        auto type_desc = as_text(typename_<typename T::value_type>);
        text base {"optional<%s>", type_desc.data() ? type_desc.data() : "unknown"};

        if(e.has_value())
        {
          auto val_desc = as_text(e.value());
          return base + text("{%s}", val_desc.data() ? val_desc.data() : "?");
        }
        else return base + "{}";
      }

      static text pointer(T const& e)
      {
        auto type_desc = as_text(typename_<T>);
        return text("%p (%s)", (void*)(e), type_desc.data() ? type_desc.data() : "unknown");
      }

      static text sequence(T const& e)
      {
        text that("{ ");
        for(auto const& v: e)
          that += as_text(v) + " ";
        that += "}";
        return that;
      }

      static text bytes(T const& e)
      {
        // Display accessible bytes
        auto const* raw = reinterpret_cast<std::byte const*>(&e);
        text        txt_bytes("[ ");
        for(std::size_t i = 0; i < sizeof(e); ++i)
          txt_bytes += text("%2.2X", std::to_integer<unsigned>(raw[ i ])) + " ";
        txt_bytes      += "]";

        auto type_desc  = as_text(typename_<T>);
        return text("%s: %s",
                    type_desc.data() ? type_desc.data() : "unknown",
                    txt_bytes.data() ? txt_bytes.data() : "[]");
      }
    };
  }

  //====================================================================================================================
  /*!
    @ingroup tools-text
    @brief How a value of type T is rendered in a report

    Specialize this rather than overloading to_text: a specialization that does not match is a
    compilation error, where a misnamed overload fell back on the byte dump in silence. Inherit
    from tts::_::builtin_display<T> to keep the rendering for anything left alone.

    The `to_text` free function this replaces is gone. An overload left behind is reported where it
    would have been used, rather than ignored in silence.
  **/
  //====================================================================================================================
  template<typename T> struct display : _::builtin_display<T>
  {
  };

  //====================================================================================================================
  // TTS renders its own types through its own trait. They used to carry a hidden friend to_text,
  // which was the customization point being removed, so they answer here like anyone else would.
  //====================================================================================================================
  template<> struct display<text>
  {
    static text render(text const& t)
    {
      return t;
    }
  };

  template<typename T> struct display<type<T>>
  {
    static text render(type<T> const&)
    {
      return as_text(typename_<T>);
    }
  };

  template<typename T> struct display<_::typename_impl<T>>
  {
    static text render(_::typename_impl<T> const& t)
    {
      return text("%.*s", t.size(), t.data());
    }
  };

  template<> struct display<_::source_location>
  {
    static text render(_::source_location const& s)
    {
      return text(s.data());
    }
  };
}

namespace tts::_
{
  //====================================================================================================================
  // Whether a type says how it is rendered. The primary of tts::display inherits the built-in
  // rendering, so a specialization that answers for a type is exactly one that does not.
  //====================================================================================================================
  template<typename T>
  concept described = !std::is_base_of_v<builtin_display<T>, display<T>>;
}

namespace tts
{

  template<typename T> text as_text(T const& e)
  {
    return display<T>::render(e);
  }

  //====================================================================================================================
  //! @}
  //====================================================================================================================

  template<std::size_t N> auto as_text(char const (&t)[ N ])
  {
    return text(t);
  }
  inline auto as_text(std::nullptr_t)
  {
    return text("nullptr");
  }
  inline auto as_text(bool b)
  {
    return text(b ? "true" : "false");
  }
}
