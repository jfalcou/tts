//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
namespace tts
{
}
#if defined(_MSC_VER)
#if _MSVC_LANG < 202002L
#error "TTS C++ version error"
#include "TTS requires C++20 or higher. Use /std:c++20 or higher to enable C++20 features."
#endif
#else
#if __cplusplus < 202002L
#error "TTS C++ version error"
#include "TTS requires C++20 or higher. Use -std=c++20 or higher to enable C++20 features."
#endif
#endif
#include <bit>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <limits>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <type_traits>
#include <utility>
namespace tts::_
{
  inline constexpr auto usage_text =
  R"(
Flags:
  -h, --help        Display this help message
  -x, --hex         Print the floating results in hexfloat mode
  -s, --scientific  Print the floating results in scientific mode
  -v, --verbose     Display tests results regardless of their status.
  -q, --quiet       Display only test failures percentage.
Parameters:
  --precision=arg   Set the precision for displaying floating pint values
  --seed=arg        Set the PRNG seeds (default is time-based)
Range specifics Parameters:
  --block=arg       Set size of range checks samples (min. 32)
  --loop=arg        Repeat each range checks arg times
  --ulpmax=arg      Set global failure ulp threshold for range tests (default is 2.0)
  --valmax=arg      Set maximal value for range tests (default is code)
  --valmin=arg      Set minimal value for range tests (default is code)
)";
  inline int usage(char const* name)
  {
    printf("TTS Unit Tests Driver\nUsage: %s [OPTION...]", name);
    puts(usage_text);
    return 0;
  }
}
namespace tts::_
{
  struct env
  {
    void pass()
    {
      test_count++;
      success_count++;
    }
    void fail()
    {
      test_count++;
      failure_count++;
    }
    void fatal()
    {
      test_count++;
      failure_count++;
      fatal_count++;
    }
    void invalid()
    {
      test_count++;
      invalid_count++;
    }
    int report(int fails, int invalids) const
    {
      auto test_txt = test_count > 1 ? "s" : "";
      auto pass_txt = success_count > 1 ? "es" : "";
      auto fail_txt = failure_count > 1 ? "s" : "";
      auto inv_txt  = invalid_count > 1 ? "s" : "";
      auto passes   = (fails || invalids) ? 0 : test_count;
      puts("----------------------------------------------------------------");
      printf("Results: %d test%s - %d/%d (%2.2f%%) success%s - %d/%d (%2.2f%%) failure%s - %d/%d "
             "(%2.2f%%) invalid%s\n",
             test_count,
             test_txt,
             success_count,
             passes,
             100.f * static_cast<float>(success_count) / static_cast<float>(test_count),
             pass_txt,
             failure_count,
             fails,
             100.f * static_cast<float>(failure_count) / static_cast<float>(test_count),
             fail_txt,
             invalid_count,
             invalids,
             100.f * static_cast<float>(invalid_count) / static_cast<float>(test_count),
             inv_txt);
      if(!fails && !invalids) return test_count == success_count ? 0 : 1;
      else return (failure_count == fails && invalid_count == invalids) ? 0 : 1;
    }
    int  test_count = 0, success_count = 0, failure_count = 0, fatal_count = 0, invalid_count = 0;
    bool fail_status = false;
  };
}
namespace tts
{
  inline _::env global_runtime       = {};
  inline bool   fatal_error_status   = false;
  inline bool   global_logger_status = false;
  inline int report(int fails, int invalids)
  {
    return global_runtime.report(fails, invalids);
  }
}
namespace tts::_
{
  template<typename T>
  concept stream = requires(T& os) {
    { os.copyfmt(os) };
    { os.fill(0) };
  };
  template<typename T>
  concept string = requires(T const& s) {
    typename T::size_type;
    typename T::value_type;
    { s[ 0 ] } -> std::convertible_to<typename T::value_type>;
    { s.data() } -> std::convertible_to<typename T::value_type const*>;
    { s.size() } -> std::same_as<typename T::size_type>;
    { s.find_first_of(typename T::value_type {}) } -> std::same_as<typename T::size_type>;
  };
  template<typename T>
  concept optional = requires(T const& o) {
    typename T::value_type;
    { o.has_value() } -> std::convertible_to<bool>;
    { o.value() } -> std::convertible_to<typename T::value_type>;
    { o.value_or(0) };
  };
  template<typename T>
  concept sequence = requires(T const& s) {
    { s.begin() };
    { s.end() };
  };
}
namespace tts::_
{
  template<typename T> using identity_t = T;
  template<typename, typename = void> extern identity_t<void (*)() noexcept> declval;
  template<typename T> extern identity_t<T && (*)() noexcept> declval<T, std::void_t<T&&>>;
}
#define TTS_MOVE(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define TTS_FWD(...)  static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#if defined(_MSC_VER)
#define TTS_DISABLE_WARNING_PUSH           __pragma(warning(push))
#define TTS_DISABLE_WARNING_POP            __pragma(warning(pop))
#define TTS_DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))
#define TTS_DISABLE_WARNING_SHADOW
#define TTS_DISABLE_WARNING_CRT_SECURE __pragma(warning(disable : 4996))
#elif defined(__GNUC__) || defined(__clang__)
#define TTS_DO_PRAGMA(X)                 _Pragma(#X)
#define TTS_DISABLE_WARNING_PUSH         TTS_DO_PRAGMA(GCC diagnostic push)
#define TTS_DISABLE_WARNING_POP          TTS_DO_PRAGMA(GCC diagnostic pop)
#define TTS_DISABLE_WARNING(warningName) TTS_DO_PRAGMA(GCC diagnostic ignored #warningName)
#define TTS_DISABLE_WARNING_SHADOW       TTS_DISABLE_WARNING(-Wshadow)
#define TTS_DISABLE_WARNING_CRT_SECURE
#else
#define TTS_DISABLE_WARNING_PUSH
#define TTS_DISABLE_WARNING_POP
#define TTS_DISABLE_WARNING_SHADOW
#define TTS_DISABLE_WARNING_CRT_SECURE
#endif
#ifndef TTS_FUNCTION
#define TTS_FUNCTION TTS_UNIQUE(tts_function)
#endif
#ifndef TTS_REGISTRATION
#define TTS_REGISTRATION TTS_UNIQUE(tts_registration)
#endif
#define TTS_UNIQUE3(ID, LINE)                           ID##LINE
#define TTS_UNIQUE2(ID, LINE)                           TTS_UNIQUE3(ID, LINE)
#define TTS_UNIQUE(ID)                                  TTS_UNIQUE2(ID, __COUNTER__)
#define TTS_CAT(x, y)                                   TTS_CAT_I(x, y)
#define TTS_CAT_I(x, y)                                 x##y
#define TTS_STRING(...)                                 TTS_STRING_((__VA_ARGS__))
#define TTS_STRING__(...)                               #__VA_ARGS__
#define TTS_STRING_(TXT)                                TTS_STRING__ TXT
#define TTS_COUNT(...)                                  TTS_COUNT_(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#define TTS_COUNT_(A0, A1, A2, A3, A4, A5, A6, A7, ...) A7
#define TTS_ARG0()
#define TTS_ARG1(A0)                         auto&& A0
#define TTS_ARG2(A0, A1)                     auto &&A0, auto &&A1
#define TTS_ARG3(A0, A1, A2)                 TTS_ARG2(A0, A1), auto&& A2
#define TTS_ARG4(A0, A1, A2, A3)             TTS_ARG3(A0, A1, A2), auto&& A3
#define TTS_ARG5(A0, A1, A2, A3, A4)         TTS_ARG4(A0, A1, A2, A3), auto&& A4
#define TTS_ARG6(A0, A1, A2, A3, A4, A5)     TTS_ARG5(A0, A1, A2, A3, A4), auto&& A5
#define TTS_ARG7(A0, A1, A2, A3, A4, A5, A6) TTS_ARG6(A0, A1, A2, A3, A4, A5), auto&& A6
#define TTS_ARG(...)                         TTS_CAT(TTS_ARG, TTS_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define TTS_VAL(x)                           x
#define TTS_REVERSE_1(a)                     (a)
#define TTS_REVERSE_2(a, b)                  (b, a)
#define TTS_REVERSE_3(a, b, c)               (c, b, a)
#define TTS_REVERSE_4(a, b, c, d)            (d, c, b, a)
#define TTS_REVERSE_5(a, b, c, d, e)         (e, d, c, b, a)
#define TTS_REVERSE_6(a, b, c, d, e, f)      (f, e, d, c, b, a)
#define TTS_REVERSE_7(a, b, c, d, e, f, g)   (g, f, e, d, c, b, a)
#define TTS_REVERSE_IMPL(N, ...)             TTS_VAL(TTS_REVERSE_##N(__VA_ARGS__))
#define TTS_REVERSE_(N, ...)                 TTS_REVERSE_IMPL(N, __VA_ARGS__)
#define TTS_REVERSE(...)                     TTS_REVERSE_(TTS_COUNT(__VA_ARGS__), __VA_ARGS__)
#define TTS_REMOVE_PARENS(x)                 TTS_EVAL((TTS_REMOVE_PARENS_I x), x)
#define TTS_REMOVE_PARENS_I(...)             1, 1
#define TTS_APPLY(macro, args)               TTS_APPLY_I(macro, args)
#define TTS_APPLY_I(macro, args)             macro args
#define TTS_EVAL_I(test, x)                  TTS_MAYBE_STRIP_PARENS(TTS_TEST_ARITY test, x)
#define TTS_EVAL(test, x)                    TTS_EVAL_I(test, x)
#define TTS_TEST_ARITY(...)                  TTS_APPLY(TTS_TEST_ARITY_I, (__VA_ARGS__, 2, 1))
#define TTS_TEST_ARITY_I(a, b, c, ...)       c
#define TTS_MAYBE_STRIP_PARENS(cond, x)      TTS_MAYBE_STRIP_PARENS_I(cond, x)
#define TTS_MAYBE_STRIP_PARENS_I(cond, x)    TTS_CAT(TTS_MAYBE_STRIP_PARENS_, cond)(x)
#define TTS_MAYBE_STRIP_PARENS_1(x)          x
#define TTS_MAYBE_STRIP_PARENS_2(x)          TTS_APPLY(TTS_MAYBE_STRIP_PARENS_2_I, x)
#define TTS_MAYBE_STRIP_PARENS_2_I(...)      __VA_ARGS__
#include <compare>
#include <cstdio>
#include <cstring>
#include <cstdlib>
TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE
namespace tts
{
  struct text
  {
    text()
        : data_(nullptr)
        , size_(0)
    {
    }
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
    template<std::size_t N>
    explicit text(char const (&data)[ N ])
        : text("%.s*", N, &data[ 0 ])
    {
    }
    template<typename... Args>
    explicit text(char const* format, Args... args)
        : text()
    {
      int len = snprintf(nullptr, 0, format, args...);
      if(len > 0)
      {
        data_ = reinterpret_cast<char*>(malloc(len + 1));
        if(data_)
        {
          size_ = len;
          snprintf(data_, size_ + 1, format, args...);
        }
      }
    }
    text(text const& other)
        : text()
    {
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
    text(text&& other) noexcept
        : text()
    {
      swap(other);
    }
    text& operator=(text const& other)
    {
      text local(other);
      swap(local);
      return *this;
    }
    text& operator=(text&& other) noexcept
    {
      text local(std::move(other));
      swap(local);
      return *this;
    }
    ~text()
    {
      if(data_) free(data_);
    }
    void swap(text& o) noexcept
    {
      std::swap(o.data_, data_);
      std::swap(o.size_, size_);
    }
    text& operator+=(text const& other)
    {
      text local {"%.*s%.*s", size_, data(), other.size_, other.data()};
      swap(local);
      return *this;
    }
    text& operator+=(char const* other)
    {
      if(other)
      {
        text local {"%.*s%s", size_, data(), other};
        swap(local);
      }
      return *this;
    }
    text operator+(text const& other) const
    {
      text local(*this);
      return local += other;
    }
    template<_::stream OS> friend OS& operator<<(OS& os, text const& t)
    {
      if(t.data_)
      {
        for(int i = 0; i < t.size_; ++i)
          os << t.data_[ i ];
      }
      return os;
    }
    bool is_empty() const
    {
      return size_ == 0;
    }
    int size() const
    {
      return size_;
    }
    [[nodiscard]] char const* data() const
    {
      return data_ ? data_ : "";
    }
    [[nodiscard]] char* data()
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
    friend auto const& to_text(text const& t)
    {
      return t;
    }
    friend bool operator==(text const& a, text const& b) noexcept
    {
      if(a.size_ != b.size_) return false;
      if(a.is_empty()) return true;
      return strcmp(a.data_, b.data_) == 0;
    }
    template<std::size_t N> friend bool operator==(text const& a, char const (&b)[ N ]) noexcept
    {
      if(a.is_empty()) return N == 1;
      return strcmp(a.data_, &b[ 0 ]) == 0;
    }
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
    template<std::size_t N> friend auto operator<=>(text const& a, char const (&b)[ N ]) noexcept
    {
      return a <=> text {b};
    }
  private:
    char* data_;
    int   size_;
  };
  inline text operator+(text const& lhs, char const* rhs)
  {
    text that(lhs);
    return that += rhs;
  }
  inline text operator+(char const* lhs, text const& rhs)
  {
    text that(lhs);
    return that += rhs;
  }
}
TTS_DISABLE_WARNING_POP
#include <cassert>
TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE
namespace tts::_
{
  struct option
  {
    option() = default;
    explicit option(char const* arg)
        : token(arg)
        , position(-1)
    {
      assert(arg && "Token cannot be null");
      auto it  = strchr(arg, '=');
      position = static_cast<int>(it ? (it - token) : strlen(token));
    }
    bool has_flag(char const* f) const
    {
      assert(f && "Flag cannot be null");
      if(position == -1) return false;
      int len = static_cast<int>(strlen(f));
      if(len > position) return false;
      return strncmp(token, f, position) == 0;
    }
    bool is_valid() const
    {
      return position > 0;
    }
    template<typename T> T get(T const& def = T {}) const
    {
      T that = {};
      if(is_valid())
      {
        int n = 0;
        if constexpr(std::integral<T>)
        {
          decltype(sizeof(void*)) v;
          n    = sscanf(token + position + 1, "%zu", &v);
          that = static_cast<T>(v);
        }
        else if constexpr(std::floating_point<T>)
        {
          double v;
          n    = sscanf(token + position + 1, "%lf", &v);
          that = static_cast<T>(v);
        }
        else
        {
          n    = 1;
          that = T {token + position + 1};
        }
        if(n != 1) that = def;
      }
      else
      {
        that = def;
      }
      return that;
    }
    char const* token    = "";
    int         position = -1;
  };
}
namespace tts
{
  struct options
  {
    bool operator[](char const* f) const
    {
      return find(f).is_valid();
    }
    template<std::same_as<char const*>... Flags> bool operator()(Flags... fs) const
    {
      return find(fs...).is_valid();
    }
    template<typename T, std::same_as<char const*>... Flags> T value(Flags... fs) const
    {
      T that = {};
      if(auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }
    template<typename T, std::same_as<char const*>... Flags> T value(T that, Flags... fs) const
    {
      if(auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }
    bool is_valid() const
    {
      return argc && argv != nullptr;
    }
    int          argc;
    char const** argv;
  private:
    template<std::same_as<char const*>... Flags> _::option find(Flags... fs) const
    {
      char const* flags[] = {fs...};
      for(int i = 1; i < argc; ++i)
      {
        _::option o(argv[ i ]);
        for(auto f: flags)
        {
          if(o.has_flag(f)) return o;
        }
      }
      return _::option {};
    }
  };
  namespace _
  {
    inline options current_arguments = {0, nullptr};
    inline int     current_seed      = -1;
    inline bool    is_verbose        = false;
    inline bool    is_quiet          = false;
  }
  inline void initialize(int argc, char const** argv)
  {
    if(!_::current_arguments.is_valid()) _::current_arguments = options {argc, argv};
  }
}
namespace tts
{
  inline options const& arguments()
  {
    return _::current_arguments;
  }
  inline int random_seed(int base_seed = -1)
  {
    if(_::current_seed == -1)
    {
      auto s = arguments().value(base_seed, "--seed");
      if(s == -1) s = static_cast<int>(time(nullptr));
      _::current_seed = s;
    }
    return _::current_seed;
  }
  inline bool is_verbose()
  {
    return _::is_verbose;
  }
}
TTS_DISABLE_WARNING_POP
namespace tts::_
{
  template<typename T> struct typename_impl
  {
  private:
    using data_t = struct
    {
      char const* data;
      int         size;
    };
    data_t data_ = {};
  public:
    constexpr typename_impl()
    {
      data_ = typename_impl_value();
    }
    constexpr auto data() const
    {
      return data_.data;
    }
    constexpr auto size() const
    {
      return data_.size;
    }
    friend text to_text(typename_impl const& t)
    {
      return text("%.*s", t.size(), t.data());
    }
    template<_::stream OS> friend OS& operator<<(OS& os, typename_impl t)
    {
      for(int i = 0; i < t.size(); ++i)
        os << t.data()[ i ];
      return os;
    }
  private:
    constexpr static int length(char const* str)
    {
      int i = 0;
      while(str[ i ] != '\0')
      {
        ++i;
      }
      return i;
    }
    constexpr static auto find(char const* str, char const* n)
    {
      auto compare = [](char const* lhs, char const* rhs, int count)
      {
        if(lhs == nullptr || rhs == nullptr)
        {
          return rhs != nullptr ? -1 : 1;
        }
        for(int i = 0; i < count; ++i)
        {
          if(lhs[ i ] != rhs[ i ])
          {
            return lhs[ i ] < rhs[ i ] ? -1 : 1;
          }
          else if(lhs[ i ] == '\0')
          {
            return 0;
          }
        }
        return 0;
      };
      auto begin = str;
      if(char c = *n++; c != '\0')
      {
        int len = length(n);
        do
        {
          char sc = {};
          do
          {
            if((sc = *str++) == '\0') return 0;
          } while(sc != c);
        } while(compare(str, n, len) != 0);
        --str;
      }
      return static_cast<int>(str - begin);
    }
    constexpr static auto typename_impl_value()
    {
      data_t that = {};
#if defined(_MSC_VER)
      that.data    = __FUNCSIG__;
      auto offset  = find(that.data, "<") + 1;
      that.size    = find(that.data, ">::typename_impl_value(") - offset;
      that.data   += offset;
#else
      that.data  = __PRETTY_FUNCTION__;
      auto i     = find(that.data, "=") + 2;
      that.size  = find(that.data, "]") - i;
      that.data += i;
#endif
      return that;
    }
  };
}
namespace tts
{
  template<typename T> inline auto constexpr typename_ = _::typename_impl<T> {};
  template<typename T> constexpr auto name([[maybe_unused]] T const& t)
  {
    return typename_<T>;
  }
}
namespace tts
{
  template<typename... Ts> struct types
  {
    template<typename... Us> constexpr types<Ts..., Us...> operator+(types<Us...> const&) const
    {
      return {};
    }
  };
  template<typename... Ls> using concatenate = decltype((Ls {} + ... + types<> {}));
  template<typename... T> struct as_type_list
  {
    using type = types<T...>;
  };
  template<typename... T> struct as_type_list<types<T...>>
  {
    using type = types<T...>;
  };
  template<typename T>
    requires requires(T) { typename T::types_list; }
  struct as_type_list<T>
  {
    using type = typename T::types_list;
  };
  template<typename... T> using as_type_list_t = typename as_type_list<T...>::type;
  template<typename L1, typename L2> struct cartesian_product;
  template<typename... Ts, typename... Us> struct cartesian_product<types<Ts...>, types<Us...>>
  {
    template<typename T> using product_row = types<types<T, Us>...>;
    using types_list                       = decltype((product_row<Ts> {} + ... + types<> {}));
  };
  template<typename L> struct cartesian_square : cartesian_product<L, L>
  {
  };
  template<template<typename> typename Pred, typename Type> struct filter
  {
    template<typename T>
    static constexpr std::conditional_t<Pred<T>::value, types<T>, types<>> filter_type()
    {
      return {};
    }
    template<typename... Ls> static constexpr auto filter_impl(types<Ls...>)
    {
      return (filter_type<Ls>() + ... + types<> {});
    }
    using types_list = decltype(filter_impl(Type {}));
  };
  template<typename T> struct type
  {
    friend text to_text(type)
    {
      return as_text(typename_<T>);
    }
    template<_::stream OS> friend OS& operator<<(OS& os, type const&)
    {
      return os << typename_<T>;
    }
  };
  using real_types = types<double, float>;
  using int_types = types<std::int64_t, std::int32_t, std::int16_t, std::int8_t>;
  using uint_types = types<std::uint64_t, std::uint32_t, std::uint16_t, std::uint8_t>;
  using integral_types = types<std::int64_t,
                               std::int32_t,
                               std::int16_t,
                               std::int8_t,
                               std::uint64_t,
                               std::uint32_t,
                               std::uint16_t,
                               std::uint8_t>;
  using signed_types = types<double, float, std::int64_t, std::int32_t, std::int16_t, std::int8_t>;
  using arithmetic_types = types<double,
                                 float,
                                 std::int64_t,
                                 std::int32_t,
                                 std::int16_t,
                                 std::int8_t,
                                 std::uint64_t,
                                 std::uint32_t,
                                 std::uint16_t,
                                 std::uint8_t>;
}
namespace tts
{
  template<typename T> text as_text(T const& e)
  {
    if constexpr(requires { to_text(e); })
    {
      return to_text(e);
    }
    else if constexpr(std::floating_point<T>)
    {
      auto precision = ::tts::arguments().value(16, "--precision");
      bool hexmode   = ::tts::arguments()("-x", "--hex");
      bool scimode   = ::tts::arguments()("-s", "--scientific");
      if(scimode) return text("%.*E", precision, e);
      else if(hexmode) return text("%#.*A", precision, e);
      else return text("%.*g", precision, e);
    }
    else if constexpr(std::integral<T>)
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
    else if constexpr(_::string<T>)
    {
      return text("'%.*s'", static_cast<int>(e.size()), e.data() ? e.data() : "");
    }
    else if constexpr(_::optional<T>)
    {
      auto type_desc = as_text(typename_<typename T::value_type>);
      text base {"optional<%s>", type_desc.data() ? type_desc.data() : "unknown"};
      if(e.has_value())
      {
        auto val_desc = as_text(e.value());
        return base + text("{%s}", val_desc.data() ? val_desc.data() : "?");
      }
      else return base + "{}";
    }
    else if constexpr(std::is_pointer_v<T>)
    {
      auto type_desc = as_text(typename_<T>);
      return text("%p (%s)", (void*)(e), type_desc.data() ? type_desc.data() : "unknown");
    }
    else if constexpr(_::sequence<T>)
    {
      text that("{ ");
      for(auto const& v: e)
        that += as_text(v) + " ";
      that += "}";
      return that;
    }
    else
    {
      unsigned char bytes[ sizeof(e) ];
      std::memcpy(bytes, &e, sizeof(e));
      text txt_bytes("[ ");
      for(auto const& b: bytes)
        txt_bytes += text("%2.2X", b) + " ";
      txt_bytes      += "]";
      auto type_desc  = as_text(typename_<T>);
      return text("%s: %s",
                  type_desc.data() ? type_desc.data() : "unknown",
                  txt_bytes.data() ? txt_bytes.data() : "[]");
    }
  }
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
namespace tts::_
{
  struct fatal_signal
  {
  };
  struct logger
  {
    explicit logger(bool status = true)
        : display(status)
        , done(false)
    {
    }
    template<typename Data> logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          printf("     >> Additional information: \n     ");
          done = true;
        }
        printf("%s", as_text(d).data());
      }
      return *this;
    }
    ~logger() noexcept(false)
    {
      if(display && done) puts("");
      if(::tts::fatal_error_status) throw ::tts::_::fatal_signal();
    }
    bool display, done;
  };
}
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
        data_ = static_cast<T*>(malloc(sizeof(T) * n));
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
        data_ = static_cast<T*>(malloc(sizeof(T) * n));
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
            (data_ + i)->~T();
        }
        free(data_);
      }
    }
    buffer(buffer const& other)
        : buffer()
    {
      if(other.size_ > 0)
      {
        data_ = static_cast<T*>(malloc(sizeof(T) * other.size_));
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
        auto new_data = static_cast<T*>(malloc(sizeof(T) * new_cap));
        assert(new_data && "tts::buffer out of memory");
        for(std::size_t i = 0; i < size_; ++i)
        {
          new(new_data + i) T(TTS_MOVE(data_[ i ]));
          if constexpr(!std::is_trivially_destructible_v<T>)
            (data_ + i)->~T();
        }
        free(data_);
        data_     = new_data;
        capacity_ = new_cap;
      }
    }
  };
}
namespace tts::_
{
  struct callable
  {
  public:
    using signature_t   = void (*)(void*);
    signature_t invoker = {};
    signature_t cleanup = {};
    void*       payload = {};
    callable()
        : invoker {nullptr}
        , cleanup {nullptr}
        , payload {nullptr}
    {
    }
    callable(void (*f)())
        : invoker {invoke_ptr}
        , cleanup {cleanup_ptr}
        , payload {reinterpret_cast<void*>(f)}
    {
    }
    template<typename Function>
    callable(Function f)
        : invoker {invoke<Function>}
        , cleanup {destroy<Function>}
        , payload {new Function {TTS_MOVE(f)}}
    {
    }
    constexpr callable(callable&& other) noexcept
        : invoker {TTS_MOVE(other.invoker)}
        , cleanup {TTS_MOVE(other.cleanup)}
        , payload {TTS_MOVE(other.payload)}
    {
      other.payload = {};
    }
    ~callable()
    {
      if(payload) cleanup(payload);
    }
    callable(callable const&)            = delete;
    callable& operator=(callable const&) = delete;
    callable& operator=(callable&& other) noexcept
    {
      payload       = TTS_MOVE(other.payload);
      other.payload = {};
      invoker       = TTS_MOVE(other.invoker);
      cleanup       = TTS_MOVE(other.cleanup);
      return *this;
    }
    void operator()()
    {
      assert(payload);
      invoker(payload);
    }
    void operator()() const
    {
      assert(payload);
      invoker(payload);
    }
    explicit operator bool() const
    {
      return payload != nullptr;
    }
  private:
    template<typename T>
    static void invoke(void* data)
    {
      (*static_cast<T*>(data))();
    }
    template<typename T>
    static void destroy(void* data)
    {
      delete static_cast<T*>(data);
    }
    static void invoke_ptr(void* data)
    {
      reinterpret_cast<void (*)()>(data)();
    }
    static void cleanup_ptr(void*)
    {
    }
  };
}
namespace tts::_
{
  inline char const* current_test = "";
  struct test
  {
    void operator()()
    {
      current_test = name;
      behaviour();
    }
    static inline bool acknowledge(test&& f);
    char const*        name;
    tts::_::callable   behaviour;
  };
  inline buffer<test>& suite()
  {
    static buffer<test> that = {};
    return that;
  }
  bool inline test::acknowledge(test&& f)
  {
    suite().emplace_back(TTS_MOVE(f));
    return true;
  }
}
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CUSTOM_DRIVER_FUNCTION
#define TTS_MAIN
#endif
#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#define TTS_CUSTOM_DRIVER_FUNCTION main
namespace tts::_
{
  inline constexpr bool use_main = true;
}
#else
namespace tts::_
{
  inline constexpr bool use_main = false;
}
#endif
#if defined(TTS_MAIN)
namespace tts::_
{
  void report_pass(char const* location, char const* message)
  {
    if(::tts::_::is_verbose && !::tts::_::is_quiet)
    {
      printf("  [+] %s : %s\n", location, message);
    }
  }
  void report_fail(char const* location, char const* message, ::tts::text const& type)
  {
    if(!::tts::_::is_verbose)
    {
      if(!type.is_empty()) printf(">  With <T = %s>\n", type.data());
    }
    if(!::tts::_::is_quiet)
    {
      printf("  [X] %s : ** FAILURE ** : %s\n", location, message);
    }
  }
  void report_fatal(char const* location, char const* message, ::tts::text const& type)
  {
    if(!::tts::_::is_verbose)
    {
      if(!type.is_empty()) printf(">  With <T = %s>\n", type.data());
    }
    printf("  [@] %s : @@ FATAL @@ : %s\n", location, message);
  }
}
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc, argv);
  if(::tts::arguments()("-h", "--help")) return ::tts::_::usage(argv[ 0 ]);
  ::tts::_::is_verbose   = ::tts::arguments()("-v", "--verbose");
  ::tts::_::is_quiet     = ::tts::arguments()("-q", "--quiet");
  auto        nb_tests   = ::tts::_::suite().size();
  std::size_t done_tests = 0;
  srand(tts::random_seed());
  try
  {
    for(auto& t: ::tts::_::suite())
    {
      auto test_count                   = ::tts::global_runtime.test_count;
      auto failure_count                = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;
      if(!::tts::_::is_quiet) printf("TEST: '%s'\n", t.name);
      fflush(stdout);
      t();
      done_tests++;
      if(test_count == ::tts::global_runtime.test_count)
      {
        ::tts::global_runtime.invalid();
        if(!::tts::_::is_quiet) printf("  [!!]: EMPTY TEST CASE\n");
        fflush(stdout);
      }
      else if(failure_count == ::tts::global_runtime.failure_count)
      {
        if(!::tts::_::is_quiet) printf("TEST: '%s' - [PASSED]\n", t.name);
        fflush(stdout);
      }
    }
  }
  catch(::tts::_::fatal_signal&)
  {
    if(!::tts::_::is_quiet)
      printf("@@ ABORTING DUE TO EARLY FAILURE @@ - %d Tests not run\n",
             static_cast<int>(nb_tests - done_tests - 1));
  }
  if constexpr(::tts::_::use_main) return ::tts::report(0, 0);
  else return 0;
}
#endif
namespace tts::_
{
  class source_location
  {
  public:
    [[nodiscard]] static auto current(char const* file = __builtin_FILE(),
                                      int         line = __builtin_LINE()) noexcept
    {
      int  offset = 0;
      auto end    = strrchr(file, '/');
      if(end) offset = static_cast<int>(end - file + 1);
      source_location that {};
      that.desc_ = text {"[%s:%d]", file + offset, line};
      return that;
    }
    friend text to_text(source_location const& s)
    {
      return s.desc_;
    }
    decltype(auto) data() const
    {
      return desc_.data();
    }
    template<_::stream OS> friend OS& operator<<(OS& os, source_location const& s)
    {
      return os << s.desc_;
    }
  private:
    text desc_ {"[unknown:?]"};
  };
}
namespace tts::_
{
  void report_pass(char const* location, char const* message);
  void report_fail(char const* location, char const* message, ::tts::text const& type);
  void report_fatal(char const* location, char const* message, ::tts::text const& type);
}
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_PASS(...)
#else
#define TTS_PASS(...)                                                                              \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.pass();                                                                  \
    ::tts::_::report_pass(::tts::_::source_location::current().data(),                             \
                          ::tts::text {__VA_ARGS__}.data());                                       \
  } while(0) 
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_FAIL(...)
#else
#define TTS_FAIL(...)                                                                              \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.fail();                                                                  \
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;               \
    ::tts::_::report_fail(::tts::_::source_location::current().data(),                             \
                          ::tts::text {__VA_ARGS__}.data(),                                        \
                          ::tts::_::current_type);                                                 \
  } while(0) 
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_FATAL(...)
#else
#define TTS_FATAL(...)                                                                             \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.fail();                                                                  \
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;               \
    ::tts::_::report_fatal(::tts::_::source_location::current().data(),                            \
                           ::tts::text {__VA_ARGS__}.data(),                                       \
                           ::tts::_::current_type);                                                \
    ::tts::fatal_error_status = true;                                                              \
    [[maybe_unused]] ::tts::_::logger _local_tts_fail_hard {};                                     \
  } while(0) 
#endif
namespace tts::_
{
  inline auto as_int(float a)
  {
    return std::bit_cast<std::uint32_t>(a);
  }
  inline auto as_int(double a)
  {
    return std::bit_cast<std::uint64_t>(a);
  }
  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto ia             = as_int(a);
    using r_t           = decltype(ia);
    constexpr auto mask = r_t(1) << (sizeof(r_t) * 8 - 1);
    return ((ia & mask) == mask) ? mask - ia : ia;
  }
}
#include <bit>
#if !defined(__GNUC__) && !defined(__clang__)
#define TTS_USE_STDMATH
#include <math.h>
#endif
namespace tts::_
{
#if defined(__FAST_MATH__)
  constexpr auto is_nan(auto)
  {
    return false;
  };
  constexpr auto is_inf(auto)
  {
    return false;
  };
  constexpr auto is_unordered(auto, auto)
  {
    return false;
  };
#else
  constexpr auto is_nan(auto x)
  {
    return x != x;
  };
  constexpr auto is_inf(auto x)
  {
    return !is_nan(x) && is_nan(x - x);
  };
  constexpr auto is_unordered(auto x, auto y)
  {
    return is_nan(x) || is_nan(y);
  };
#endif
  constexpr auto min(auto x, auto y)
  {
    return x < y ? x : y;
  };
  constexpr auto max(auto x, auto y)
  {
    return x < y ? y : x;
  };
  constexpr auto abs(auto x)
  {
    return x < 0 ? -x : x;
  };
  constexpr bool signbit(auto x)
  {
    return (as_int(x) >> (sizeof(x) * 8 - 1)) != 0;
  };
#if !defined(TTS_USE_STDMATH)
  inline double pow(double x, double y)
  {
    return __builtin_pow(x, y);
  }
  inline float pow(float x, float y)
  {
    return __builtin_powf(x, y);
  }
  inline long double pow(long double x, long double y)
  {
    return __builtin_powl(x, y);
  }
  inline double sqrt(double x)
  {
    return __builtin_sqrt(x);
  }
  inline float sqrt(float x)
  {
    return __builtin_sqrtf(x);
  }
  inline long double sqrt(long double x)
  {
    return __builtin_sqrtl(x);
  }
  inline double log10(double x)
  {
    return __builtin_log10(x);
  }
  inline float log10(float x)
  {
    return __builtin_log10f(x);
  }
  inline long double log10(long double x)
  {
    return __builtin_log10l(x);
  }
  inline double ceil(double x)
  {
    return __builtin_ceil(x);
  }
  inline float ceil(float x)
  {
    return __builtin_ceilf(x);
  }
  inline long double ceil(long double x)
  {
    return __builtin_ceill(x);
  }
  inline double log2(double x)
  {
    return __builtin_log2(x);
  }
  inline float log2(float x)
  {
    return __builtin_log2f(x);
  }
  inline long double log2(long double x)
  {
    return __builtin_log2l(x);
  }
#else
  inline double pow(double x, double y)
  {
    return ::pow(x, y);
  }
  inline float pow(float x, float y)
  {
    return ::powf(x, y);
  }
  inline long double pow(long double x, long double y)
  {
    return ::powl(x, y);
  }
  inline double sqrt(double x)
  {
    return ::sqrt(x);
  }
  inline float sqrt(float x)
  {
    return ::sqrtf(x);
  }
  inline long double sqrt(long double x)
  {
    return ::sqrtl(x);
  }
  inline double log10(double x)
  {
    return ::log10(x);
  }
  inline float log10(float x)
  {
    return ::log10f(x);
  }
  inline long double log10(long double x)
  {
    return ::log10l(x);
  }
  inline double ceil(double x)
  {
    return ::ceil(x);
  }
  inline float ceil(float x)
  {
    return ::ceilf(x);
  }
  inline long double ceil(long double x)
  {
    return ::ceill(x);
  }
  inline double log2(double x)
  {
    return ::log2(x);
  }
  inline float log2(float x)
  {
    return ::log2f(x);
  }
  inline long double log2(long double x)
  {
    return ::log2l(x);
  }
#endif
  constexpr std::size_t log2(std::size_t n)
  {
    return n ? std::bit_width(n) - 1 : 0;
  }
  template<typename T> T exp10(T a)
  {
    return tts::_::pow(T(10), a);
  }
}
namespace tts
{
  namespace _
  {
    struct rand_result
    {
      unsigned int val, max;
    };
    inline rand_result rand30()
    {
      if constexpr(RAND_MAX >= 2147483647)
      {
        return {static_cast<unsigned int>(std::rand()), static_cast<unsigned int>(RAND_MAX)};
      }
      else
      {
        constexpr unsigned int SHIFT_MAX =
        (static_cast<unsigned int>(RAND_MAX) << 15) | static_cast<unsigned int>(RAND_MAX);
        unsigned int r =
        (static_cast<unsigned int>(std::rand()) << 15) | static_cast<unsigned int>(std::rand());
        return {r, SHIFT_MAX};
      }
    }
    template<std::integral T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);
      using U = std::make_unsigned_t<T>;
      U range = static_cast<U>(N - M) + 1;
      if(range <= static_cast<U>(RAND_MAX))
      {
        unsigned int r_max       = static_cast<unsigned int>(RAND_MAX);
        unsigned int bucket_size = r_max / static_cast<unsigned int>(range);
        unsigned int limit       = bucket_size * static_cast<unsigned int>(range);
        unsigned int r;
        do
        {
          r = static_cast<unsigned int>(std::rand());
        } while(r >= limit);
        return M + static_cast<T>(r / bucket_size);
      }
      auto [ r_raw, r_max ] = rand30();
      if(range > r_max)
      {
        return M + static_cast<T>(r_raw % range);
      }
      U            bucket_size = r_max / range;
      U            limit       = bucket_size * range;
      unsigned int r           = r_raw;
      while(r >= limit)
      {
        r = rand30().val;
      }
      return M + static_cast<T>(r / bucket_size);
    }
    template<std::floating_point T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);
      auto [ r_raw, r_max ] = rand30();
      double uniform_01 = static_cast<double>(r_raw) / static_cast<double>(r_max);
      return static_cast<T>(M + uniform_01 * (N - M));
    }
    template<std::integral T> T roll_random(T mini, T maxi)
    {
      return _::roll(mini, maxi);
    }
    template<std::floating_point T> T roll_random(T mini, T maxi)
    {
      constexpr T smvlp     = std::numeric_limits<T>::min();
      constexpr T valmax    = std::numeric_limits<T>::max();
      constexpr T eps       = std::numeric_limits<T>::epsilon();
      constexpr T quiet_nan = std::numeric_limits<T>::quiet_NaN();
      if(mini == 0) mini = smvlp;
      if(maxi == 0) maxi = -smvlp;
      if(_::is_inf(mini) && mini < 0) mini = -valmax;
      if(_::is_inf(maxi) && maxi > 0) maxi = valmax;
      if(mini < 0 && maxi > 0)
      {
        T abs_min = _::abs(mini);
        T abs_max = _::abs(maxi);
        T    total_mag     = abs_min + abs_max;
        bool pick_positive = _::roll(T(0.0), total_mag) < abs_max;
        if(pick_positive) return roll_random(smvlp, maxi);
        else return -roll_random(smvlp, abs_min);
      }
      T value = {};
      if(mini > 0)
      {
        if(mini < 1 && maxi > 1) mini = max(T(1) / _::sqrt(maxi), mini);
        mini      = (maxi == 1) ? eps : mini;
        T log_min = _::log10(mini);
        T log_max = _::log10(maxi);
        T log_val = _::roll(log_min, log_max);
        value     = _::exp10(log_val);
      }
      else if(maxi < 0)
      {
        if(mini < -1 && maxi > -1) maxi = min(T(1) / _::sqrt(-mini), maxi);
        maxi      = (mini == -1) ? -eps : maxi;
        T log_min = _::log10(-maxi);
        T log_max = _::log10(-mini);
        T log_val = _::roll(log_min, log_max);
        value     = -_::exp10(log_val);
      }
      else
      {
        return quiet_nan;
      }
      return value;
    }
  }
  template<typename T> T random_value(T mini, T maxi)
  {
    return _::roll_random(mini, maxi);
  }
}
#include <limits>
namespace tts
{
  namespace _
  {
    template<std::size_t N, bool Signed> struct sized_integer;
    template<> struct sized_integer<1, true>
    {
      using type = std::int8_t;
    };
    template<> struct sized_integer<2, true>
    {
      using type = std::int16_t;
    };
    template<> struct sized_integer<4, true>
    {
      using type = std::int32_t;
    };
    template<> struct sized_integer<8, true>
    {
      using type = std::int64_t;
    };
    template<> struct sized_integer<1, false>
    {
      using type = std::uint8_t;
    };
    template<> struct sized_integer<2, false>
    {
      using type = std::uint16_t;
    };
    template<> struct sized_integer<4, false>
    {
      using type = std::uint32_t;
    };
    template<> struct sized_integer<8, false>
    {
      using type = std::uint64_t;
    };
    template<typename T, bool Signed = false>
    using sized_integer_t = typename sized_integer<sizeof(T), Signed>::type;
  }
  template<typename T, typename V> auto as_value(V const& v)
  {
    return static_cast<T>(v);
  }
  template<tts::_::sequence Seq, typename U> struct rebuild;
  template<template<typename, typename...> typename Seq, typename T, typename... S, typename U>
  struct rebuild<Seq<T, S...>, U>
  {
    using type = Seq<U, S...>;
  };
  template<template<typename, std::size_t> typename Seq, typename T, std::size_t N, typename U>
  struct rebuild<Seq<T, N>, U>
  {
    using type = Seq<U, N>;
  };
  template<typename T> auto produce(type<T> const& t, auto g, auto... others)
  {
    return g(t, others...);
  }
  template<tts::_::sequence T> auto produce(type<T> const&, auto g, auto... args)
  {
    using elmt_type  = std::remove_cvref_t<decltype(*begin(tts::_::declval<T>()))>;
    using value_type = decltype(produce(tts::type<elmt_type> {}, g, 0, 0ULL, args...));
    typename rebuild<T, value_type>::type that;
    auto                                  b  = begin(that);
    auto                                  e  = end(that);
    std::ptrdiff_t                        sz = e - b;
    for(std::ptrdiff_t i = 0; i < sz; ++i)
    {
      *b++ = produce(tts::type<value_type> {}, g, i, sz, args...);
    }
    return that;
  }
  template<typename T> struct base_type
  {
    using type = T;
  };
  template<typename T> using base_type_t = typename base_type<T>::type;
  template<typename T> struct boolean_type
  {
    using type = bool;
  };
  template<typename T> using boolean_type_t = typename boolean_type<T>::type;
  namespace _
  {
    struct empty_limits
    {
    };
    template<typename T> struct common_limits
    {
      using b_t = base_type_t<T>;
      static_assert(std::numeric_limits<b_t>::is_specialized,
                    "Base type must have std::numeric_limits specialized");
      T valmax = static_cast<T>(std::numeric_limits<b_t>::max());
      T valmin = static_cast<T>(std::numeric_limits<b_t>::lowest());
      T zero   = static_cast<T>(0);
      T one    = static_cast<T>(1);
    };
    template<typename T> struct float_limits
    {
      using b_t = base_type_t<T>;
      static constexpr b_t get_maxflint()
      {
        if constexpr(sizeof(b_t) == 4) return static_cast<b_t>(0x1p+24);
        else return static_cast<b_t>(0x1p+53);
      }
      T nan            = static_cast<T>(std::numeric_limits<b_t>::quiet_NaN());
      T inf            = static_cast<T>(std::numeric_limits<b_t>::infinity());
      T minf           = static_cast<T>(-std::numeric_limits<b_t>::infinity());
      T mzero          = static_cast<T>(-0.0);
      T maxflint       = static_cast<T>(get_maxflint());
      T mindenormal    = static_cast<T>(std::numeric_limits<b_t>::denorm_min());
      T smallestposval = static_cast<T>(std::numeric_limits<b_t>::min());
      T mone           = static_cast<T>(-1.0);
    };
  }
  template<typename T>
  struct limits_set
      : _::common_limits<T>
      , std::conditional_t<std::floating_point<base_type_t<T>>, _::float_limits<T>, _::empty_limits>
  {
    using type = T;
  };
  template<typename T> inline auto limits(tts::type<T>)
  {
    return limits_set<T> {};
  }
  template<typename T> struct value
  {
    constexpr explicit value(T v)
        : seed(v)
    {
    }
    template<typename D> D operator()(tts::type<D>, auto...) const
    {
      return as_value<D>(seed);
    }
    T seed;
  };
  template<typename T, typename U = T> struct logicals
  {
    constexpr logicals(T v, U k)
        : start(v)
        , range(k)
    {
    }
    template<typename D> auto operator()(tts::type<D>) const
    {
      return as_value<tts::boolean_type_t<D>>(false);
    }
    template<typename D> auto operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<tts::boolean_type_t<D>>(((start + idx) % range) == 0);
    }
    T start;
    U range;
  };
  template<typename T, typename U = T> struct ramp
  {
    constexpr explicit ramp(T s)
        : start(s)
        , step(1)
    {
    }
    constexpr ramp(T s, U st)
        : start(s)
        , step(st)
    {
    }
    template<typename D> D operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<D>(start + idx * step);
    }
    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(start);
    }
    T start;
    U step;
  };
  template<typename T, typename U = T> struct reverse_ramp
  {
    constexpr explicit reverse_ramp(T s)
        : start(s)
        , step(1)
    {
    }
    constexpr reverse_ramp(T s, U st)
        : start(s)
        , step(st)
    {
    }
    template<typename D> D operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<D>(start - idx * step);
    }
    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(start);
    }
    T start;
    U step;
  };
  template<typename T, typename U = T> struct between
  {
    constexpr between(T first, U last)
        : first_(first)
        , last_(last)
    {
    }
    template<typename D> D operator()(tts::type<D>, auto idx, auto sz, auto...) const
    {
      auto w1   = as_value<D>(first_);
      auto w2   = as_value<D>(last_);
      auto step = (sz - 1) ? (w2 - w1) / as_value<D>(sz - 1) : 0;
      return _::min(as_value<D>(w1 + idx * step), w2);
    }
    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(first_);
    }
    T first_;
    U last_;
  };
  template<typename Mx, typename Mn> struct randoms
  {
    constexpr randoms(Mn mn, Mx mx)
        : mini(mn)
        , maxi(mx)
    {
    }
    template<typename D> D operator()(tts::type<D>, auto...)
    {
      if constexpr(std::is_unsigned_v<D>)
      {
        assert(mini >= 0 &&
               "Minimum value for unsigned type random generator must be non-negative");
        assert(maxi >= 0 &&
               "Maximum value for unsigned type random generator must be non-negative");
      }
      return random_value(as_value<D>(mini), as_value<D>(maxi));
    }
    Mn mini;
    Mx maxi;
  };
  struct random_bits
  {
    template<typename D> auto operator()(tts::type<D>, auto...)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
      return tts::random_value<i_t>(0, 8 * sizeof(i_t) - 1);
    }
  };
  template<typename G> struct as_integer
  {
    constexpr explicit as_integer(G g)
        : generator_(g)
    {
    }
    template<typename D> auto operator()(tts::type<D>, auto... args)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
      return generator_(tts::type<i_t> {}, args...);
    }
    G generator_;
  };
  template<typename G> struct as_signed_integer
  {
    constexpr explicit as_signed_integer(G g)
        : generator_(g)
    {
    }
    template<typename D> auto operator()(tts::type<D>, auto... args)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>, true>;
      tts::type<i_t> tgt {};
      return generator_(tgt, args...);
    }
    G generator_;
  };
}
namespace tts::_
{
  struct capture
  {
    capture(char const* id)
        : name(id)
    {
    }
    auto operator+(auto body) const
    {
      return test::acknowledge({name, body});
    }
    char const* name;
  };
  inline text current_type = {};
  template<typename... Types> struct captures
  {
    captures(char const* id)
        : name(id)
    {
    }
    auto operator+(auto body) const
    {
      return test::acknowledge({name,
                                [ body ]()
                                {
                                  (((current_type = as_text(typename_<Types>)),
                                    (::tts::_::is_verbose && !::tts::_::is_quiet
                                     ? printf(">  With <T = %s>\n", current_type.data())
                                     : 0),
                                    body(type<Types>())),
                                   ...);
                                  current_type = text {""};
                                }});
    }
    char const* name;
  };
  template<typename... Types> struct captures<types<Types...>> : captures<Types...>
  {
  };
  template<typename Generator>
    requires requires(Generator) { typename Generator::types_list; }
  struct captures<Generator> : captures<typename Generator::types_list>
  {
  };
  template<typename Types, auto... Generators> struct test_generators;
  template<typename... Type, auto... Generators>
  struct test_generators<types<Type...>, Generators...>
  {
    test_generators(char const* id)
        : name(id)
    {
    }
    friend auto operator<<(test_generators tg, auto body)
    {
      return test::acknowledge({tg.name,
                                [ body ]() mutable
                                {
                                  (process_type<Type>(body), ...);
                                  current_type = text {""};
                                }});
    }
    template<typename T> static void process_type(auto body)
    {
      current_type = as_text(typename_<T>);
      if(::tts::_::is_verbose && !::tts::_::is_quiet)
        printf(">  With <T = %s>\n", current_type.data());
      process_call(body, produce(type<T> {}, Generators)...);
    }
    template<typename... Args> static void process_call(auto body, Args&&... args)
    {
      body(std::forward<Args>(args)...);
    }
    char const* name;
  };
}
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE(ID)
#else
#define TTS_CASE(ID)                                                                               \
  [[maybe_unused]] static auto const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::capture {ID} + +[]() 
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_TPL(ID, ...)
#else
#define TTS_CASE_TPL(ID, ...)                                                                      \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::captures<__VA_ARGS__> {ID} + [] 
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_WITH(ID, TYPES, ...)
#else
#define TTS_CASE_WITH(ID, TYPES, ...)                                                              \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::test_generators<::tts::as_type_list_t<TTS_REMOVE_PARENS(TYPES)>, __VA_ARGS__> {ID}     \
  << [] 
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT(EXPR, ...)
#else
#define TTS_EXPECT(EXPR, ...) TTS_EXPECT_##__VA_ARGS__(EXPR)
#endif
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL((EXPR), TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR), TTS_FATAL)
#define TTS_EXPECT_IMPL(EXPR, FAILURE)                                                             \
  [ & ](auto&& local_tts_expr)                                                                     \
  {                                                                                                \
    if(local_tts_expr)                                                                             \
    {                                                                                              \
      TTS_PASS("Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));          \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));          \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(EXPR) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_NOT(EXPR, ...)
#else
#define TTS_EXPECT_NOT(EXPR, ...) TTS_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif
#define TTS_EXPECT_NOT_(EXPR)         TTS_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR) TTS_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)
#define TTS_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                         \
  [ & ](auto&& local_tts_expr)                                                                     \
  {                                                                                                \
    if(!local_tts_expr)                                                                            \
    {                                                                                              \
      TTS_PASS("Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));         \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(EXPR) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EXPECT(EXPR, ...)
#else
#define TTS_CONSTEXPR_EXPECT(EXPR, ...) TTS_CEXPR_EXPECT_##__VA_ARGS__(EXPR)
#endif
#define TTS_CEXPR_EXPECT_(EXPR)         TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FATAL)
#define TTS_CEXPR_EXPECT_IMPL(EXPR, FAILURE)                                                       \
  ::tts::global_logger_status = false;                                                             \
  do                                                                                               \
  {                                                                                                \
    constexpr auto local_tts_expr = EXPR;                                                          \
    if constexpr(local_tts_expr)                                                                   \
    {                                                                                              \
      TTS_PASS("Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR))); \
      ::tts::global_logger_status = false;                                                         \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Constant expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR))); \
      ::tts::global_logger_status = true;                                                          \
    }                                                                                              \
  } while(0);                                                                                      \
  ::tts::_::logger                                                                                 \
  {                                                                                                \
    ::tts::global_logger_status                                                                    \
  }
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...)
#else
#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...) TTS_CEXPR_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif
#define TTS_CEXPR_EXPECT_NOT_(EXPR)         TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)
#define TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                   \
  ::tts::global_logger_status = false;                                                             \
  do                                                                                               \
  {                                                                                                \
    constexpr auto local_tts_expr = EXPR;                                                          \
    if constexpr(!local_tts_expr)                                                                  \
    {                                                                                              \
      TTS_PASS("Constant expression: %s evaluates to false.",                                      \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)));                                               \
      ::tts::global_logger_status = false;                                                         \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));  \
      ::tts::global_logger_status = true;                                                          \
    }                                                                                              \
  } while(0);                                                                                      \
  ::tts::_::logger                                                                                 \
  {                                                                                                \
    ::tts::global_logger_status                                                                    \
  }                                                                                                \
  
#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                   \
  [ & ]()                                                                                          \
  {                                                                                                \
    bool tts_caught = false;                                                                       \
                                                                                                   \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(EXCEPTION&)                                                                              \
    {                                                                                              \
      tts_caught = true;                                                                           \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
    }                                                                                              \
                                                                                                   \
    if(tts_caught)                                                                                 \
    {                                                                                              \
      TTS_PASS("Expected: %s throws %s.",                                                          \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                \
               TTS_STRING(TTS_REMOVE_PARENS(EXCEPTION)));                                          \
      ::tts::global_runtime.pass();                                                                \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s failed to throw %s.",                                                  \
              TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                 \
              TTS_STRING(TTS_REMOVE_PARENS(EXCEPTION)));                                           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }()
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_THROW(EXPR, EXCEPTION, ...)
#else
#define TTS_THROW(EXPR, EXCEPTION, ...) TTS_THROW_##__VA_ARGS__(EXPR, EXCEPTION)
#endif
#define TTS_THROW_(EXPR, EXCEPTION)         TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FATAL)
#define TTS_NO_THROW_IMPL(EXPR, FAILURE)                                                           \
  [ & ]()                                                                                          \
  {                                                                                                \
    bool tts_caught = false;                                                                       \
                                                                                                   \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
      tts_caught = true;                                                                           \
    }                                                                                              \
                                                                                                   \
    if(!tts_caught)                                                                                \
    {                                                                                              \
      TTS_PASS("Expected: %s does not throw.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));               \
      ::tts::global_runtime.pass();                                                                \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s throws unexpectedly.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }()
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_NO_THROW(EXPR, ...)
#else
#define TTS_NO_THROW(EXPR, ...) TTS_NO_THROW_##__VA_ARGS__(EXPR)
#endif
#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR, TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR, TTS_FATAL)
#include <cstring>
namespace tts::_
{
  template<typename L, typename R>
  concept comparable_equal = requires(L l, R r) { compare_equal(l, r); };
  template<typename L, typename R>
  concept comparable_less = requires(L l, R r) { compare_less(l, r); };
  template<typename L, typename R> inline constexpr bool bit_eq(L const& l, R const& r)
  {
    static_assert(sizeof(L) == sizeof(R), "Types must have the same size for bitwise comparison");
    return std::memcmp(&l, &r, sizeof(L)) == 0;
  }
  template<typename L, typename R> inline constexpr bool bit_neq(L const& l, R const& r)
  {
    static_assert(sizeof(L) == sizeof(R), "Types must have the same size for bitwise comparison");
    return std::memcmp(&l, &r, sizeof(L)) != 0;
  }
  template<typename L, typename R> inline constexpr bool eq(L const& l, R const& r)
  {
    if constexpr(comparable_equal<L, R>) return compare_equal(l, r);
    else return l == r;
  }
  template<typename L, typename R> inline constexpr bool neq(L const& l, R const& r)
  {
    return !eq(l, r);
  }
  template<typename L, typename R> inline constexpr bool lt(L const& l, R const& r)
  {
    if constexpr(comparable_less<L, R>) return compare_less(l, r);
    else return l < r;
  }
  template<typename L, typename R> inline constexpr bool le(L const& l, R const& r)
  {
    return lt(l, r) || eq(l, r);
  }
  template<typename L, typename R> inline constexpr bool gt(L const& l, R const& r)
  {
    return !le(l, r);
  }
  template<typename L, typename R> inline constexpr bool ge(L const& l, R const& r)
  {
    return !lt(l, r);
  }
}
#define TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                 \
  if(::tts::_::OP(local_tts_a, local_tts_b))                                                       \
  {                                                                                                \
    TTS_PASS("Expression: %s %s %s is true.", TTS_STRING(A), T, TTS_STRING(B));                    \
    return ::tts::_::logger {false};                                                               \
  }                                                                                                \
  else                                                                                             \
  {                                                                                                \
    FAILURE("Expression: %s %s %s is false because %s %s %s.",                                     \
            TTS_STRING(A),                                                                         \
            T,                                                                                     \
            TTS_STRING(B),                                                                         \
            ::tts::as_text(local_tts_a).data(),                                                    \
            F,                                                                                     \
            ::tts::as_text(local_tts_b).data());                                                   \
    return ::tts::_::logger {};                                                                    \
  }                                                                                                \
  
#define TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                           \
  constexpr auto local_tts_expr = ::tts::_::OP(A, B);                                              \
  if constexpr(local_tts_expr)                                                                     \
  {                                                                                                \
    TTS_PASS("Constant expression: %s %s %s is true.", TTS_STRING(A), T, TTS_STRING(B));           \
    ::tts::global_logger_status = false;                                                           \
  }                                                                                                \
  else                                                                                             \
  {                                                                                                \
    FAILURE("Constant expression: %s %s %s is false because %s %s %s.",                            \
            TTS_STRING(A),                                                                         \
            T,                                                                                     \
            TTS_STRING(B),                                                                         \
            ::tts::as_text(A).data(),                                                              \
            F,                                                                                     \
            ::tts::as_text(B).data());                                                             \
    ::tts::global_logger_status = true;                                                            \
  }                                                                                                \
  
#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)
#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                 \
  [ & ](auto const& local_tts_a, auto const& local_tts_b)                                          \
  { TTS_RELATION_BASE(A, B, OP, T, F, FAILURE) }(A, B) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_BIT_EQUAL(LHS, RHS, ...)
#else
#define TTS_BIT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, bit_eq, "==", "!=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_BIT_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_BIT_NOT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, bit_neq, "!=", "==", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EQUAL(LHS, RHS, ...)
#else
#define TTS_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_NOT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_LESS(LHS, RHS, ...)
#else
#define TTS_LESS(LHS, RHS, ...) TTS_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_GREATER(LHS, RHS, ...)
#else
#define TTS_GREATER(LHS, RHS, ...) TTS_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_LESS_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_GREATER_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
#endif
#define TTS_TYPED_RELATION(A, B, OP, T, F, ...) TTS_TYPED_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_TYPED_RELATION_(A, B, OP, T, F)     TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_TYPED_RELATION_REQUIRED(A, B, OP, T, F)                                                \
  TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)
#define TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                           \
  [ & ](auto&& local_tts_a, auto&& local_tts_b)                                                    \
  {                                                                                                \
    using type_a = std::remove_cvref_t<decltype(local_tts_a)>;                                     \
    using type_b = std::remove_cvref_t<decltype(local_tts_b)>;                                     \
                                                                                                   \
    if(!std::same_as<type_a, type_b>)                                                              \
    {                                                                                              \
      FAILURE("'%s %s %s' is false because '%.*s' is not '%.*s'.",                                 \
              TTS_STRING(A),                                                                       \
              T,                                                                                   \
              TTS_STRING(B),                                                                       \
              ::tts::typename_<type_a>.size(),                                                     \
              ::tts::typename_<type_a>.data(),                                                     \
              ::tts::typename_<type_b>.size(),                                                     \
              ::tts::typename_<type_b>.data());                                                    \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                   \
    }                                                                                              \
  }(A, B) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_EQUAL(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)                                                         \
  TTS_TYPED_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_LESS(LHS, RHS, ...)
#else
#define TTS_TYPED_LESS(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_GREATER(LHS, RHS, ...)
#else
#define TTS_TYPED_GREATER(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)                                                     \
  TTS_TYPED_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
#endif
#define TTS_CEXPR_RELATION(A, B, OP, T, F, ...) TTS_CEXPR_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_CEXPR_RELATION_(A, B, OP, T, F)     TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_CEXPR_RELATION_REQUIRED(A, B, OP, T, F)                                                \
  TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)
#define TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                           \
  ::tts::global_logger_status = false;                                                             \
  do                                                                                               \
  {                                                                                                \
    TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                               \
  } while(0);                                                                                      \
  ::tts::_::logger                                                                                 \
  {                                                                                                \
    ::tts::global_logger_status                                                                    \
  }                                                                                                \

#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...) TTS_CEXPR_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)                                                     \
  TTS_CEXPR_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...) TTS_CEXPR_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)                                                       \
  TTS_CEXPR_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)                                                    \
  TTS_CEXPR_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)                                                 \
  TTS_CEXPR_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
#endif
#define TTS_TYPED_CEXPR_RELATION(A, B, OP, T, F, ...)                                              \
  TTS_TYPED_CEXPR_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_TYPED_CEXPR_RELATION_(A, B, OP, T, F)                                                  \
  TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_TYPED_CEXPR_RELATION_REQUIRED(A, B, OP, T, F)                                          \
  TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)
#define TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                     \
  ::tts::global_logger_status = false;                                                             \
  do                                                                                               \
  {                                                                                                \
    using type_a = std::remove_cvref_t<decltype(A)>;                                               \
    using type_b = std::remove_cvref_t<decltype(B)>;                                               \
                                                                                                   \
    if(!std::same_as<type_a, type_b>)                                                              \
    {                                                                                              \
      FAILURE("'%s %s %s' is false because '%.*s' is not '%.*s'.",                                 \
              TTS_STRING(A),                                                                       \
              T,                                                                                   \
              TTS_STRING(B),                                                                       \
              ::tts::typename_<type_a>.size(),                                                     \
              ::tts::typename_<type_a>.data(),                                                     \
              ::tts::typename_<type_b>.size(),                                                     \
              ::tts::typename_<type_b>.data());                                                    \
      ::tts::global_logger_status = false;                                                         \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                             \
    }                                                                                              \
  } while(0);                                                                                      \
  ::tts::_::logger                                                                                 \
  {                                                                                                \
    ::tts::global_logger_status                                                                    \
  }                                                                                                \
  
#define TTS_TYPED_CONSTEXPR_EQUAL(LHS, RHS, ...)                                                   \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)                                               \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS(LHS, RHS, ...)                                                    \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER(LHS, RHS, ...)                                                 \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)                                              \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)                                           \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPE_IS(TYPE, REF, ...)
#else
#define TTS_TYPE_IS(TYPE, REF, ...) TTS_TYPE_IS_##__VA_ARGS__(TYPE, REF)
#endif
#define TTS_TYPE_IS_(TYPE, REF)         TTS_TYPE_IS_IMPL(TYPE, REF, TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(TYPE, REF) TTS_TYPE_IS_IMPL(TYPE, REF, TTS_FATAL)
#define TTS_TYPE_IS_IMPL(TYPE, REF, FAILURE)                                                       \
  [ & ]<typename TTS_T, typename TTS_R>(::tts::type<TTS_T>, ::tts::type<TTS_R>)                    \
  {                                                                                                \
    if constexpr(std::is_same_v<TTS_T, TTS_R>)                                                     \
    {                                                                                              \
      TTS_PASS("Type: '%s' is '%s'.",                                                              \
               TTS_STRING(TTS_REMOVE_PARENS(TYPE)),                                                \
               TTS_STRING(TTS_REMOVE_PARENS(REF)));                                                \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Type: '%s' is not the same as '%s' because '%.*s' is not '%.*s'.",                  \
              TTS_STRING(TTS_REMOVE_PARENS(TYPE)),                                                 \
              TTS_STRING(TTS_REMOVE_PARENS(REF)),                                                  \
              ::tts::typename_<TTS_T>.size(),                                                      \
              ::tts::typename_<TTS_T>.data(),                                                      \
              ::tts::typename_<TTS_R>.size(),                                                      \
              ::tts::typename_<TTS_R>.data());                                                     \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(::tts::type<TTS_REMOVE_PARENS(TYPE)> {}, ::tts::type<TTS_REMOVE_PARENS(REF)> {}) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPR_IS(EXPR, TYPE, ...)
#else
#define TTS_EXPR_IS(EXPR, TYPE, ...) TTS_EXPR_IS_##__VA_ARGS__(EXPR, TYPE)
#endif
#define TTS_EXPR_IS_(EXPR, TYPE)         TTS_EXPR_IS_IMPL(EXPR, TYPE, TTS_FAIL)
#define TTS_EXPR_IS_REQUIRED(EXPR, TYPE) TTS_EXPR_IS_IMPL(EXPR, TYPE, TTS_FATAL)
#define TTS_EXPR_IS_IMPL(EXPR, TYPE, FAILURE)                                                      \
  [ & ]<typename TTS_T, typename TTS_R>(::tts::type<TTS_T>, ::tts::type<TTS_R>)                    \
  {                                                                                                \
    if constexpr(std::is_same_v<TTS_T, TTS_R>)                                                     \
    {                                                                                              \
      TTS_PASS("Expression: '%s' is of type '%s'.",                                                \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                \
               TTS_STRING(TTS_REMOVE_PARENS(TYPE)));                                               \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: '%s' is not of type '%s' because '%.*s' is not '%.*s'.",                \
              TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                 \
              TTS_STRING(TTS_REMOVE_PARENS(TYPE)),                                                 \
              ::tts::typename_<TTS_T>.size(),                                                      \
              ::tts::typename_<TTS_T>.data(),                                                      \
              ::tts::typename_<TTS_R>.size(),                                                      \
              ::tts::typename_<TTS_R>.data());                                                     \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(::tts::type<decltype(TTS_REMOVE_PARENS(EXPR))> {},                                             \
    ::tts::type<TTS_REMOVE_PARENS(TYPE)> {}) 
#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                        \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  [ & ](TTS_ARG(__VA_ARGS__))                                                                      \
  {                                                                                                \
    if constexpr(requires TTS_REMOVE_PARENS(EXPR))                                                 \
    {                                                                                              \
      TTS_PASS("Expression: '%s' compiles as expected.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));     \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expression: '%s' does not compile as expected.",                                   \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)));                                               \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_COMPILES(Symbols, Expression, ...)
#else
#define TTS_EXPECT_COMPILES(...) TTS_VAL(TTS_EXPECT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
#endif
#define TTS_EXPECT_NOT_COMPILES_IMPL(EXPR, ...)                                                    \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  [ & ](TTS_ARG(__VA_ARGS__))                                                                      \
  {                                                                                                \
    if constexpr(!(requires TTS_REMOVE_PARENS(EXPR)))                                              \
    {                                                                                              \
      TTS_PASS("Expression: '%s' does not compile as expected.",                                   \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)));                                               \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expression: '%s' compiles unexpectedly.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));    \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) 
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_NOT_COMPILES(Symbols..., Expression, ...)
#else
#define TTS_EXPECT_NOT_COMPILES(...) TTS_VAL(TTS_EXPECT_NOT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
#endif
namespace tts
{
  template<typename T, typename U> inline double absolute_check(T const& a, U const& b)
  {
    if constexpr(requires { absolute_distance(a, b); }) return absolute_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (_::is_nan(a) && _::is_nan(b))) return 0.;
        if(_::is_inf(a) || _::is_inf(b) || _::is_nan(a) || _::is_nan(b))
          return std::numeric_limits<double>::infinity();
        return _::abs(a - b);
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absolute_check(d0, d1);
      }
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_ABSOLUTE_EQUAL requires integral or floating points data to compare."
        "Did you mean to use TTS_ALL_ABSOLUTE_EQUAL or to overload tts::absolute_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absolute_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
  template<typename T, typename U> inline double relative_check(T const& a, U const& b)
  {
    if constexpr(requires { relative_distance(a, b); }) return relative_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      {
        return a == b ? 0. : 100.;
      }
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (_::is_nan(a) && _::is_nan(a))) return 0.;
        if(_::is_inf(a) || _::is_inf(b) || _::is_nan(a) || _::is_nan(b))
          return std::numeric_limits<double>::infinity();
        return 100. * (_::abs(a - b) / _::max(T(1), _::max(_::abs(a), _::abs(b))));
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return relative_check(d0, d1);
      }
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_RELATIVE_EQUAL requires integral or floating points data to compare."
        "Did you mean to use TTS_ALL_RELATIVE_EQUAL or to overload tts::relative_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return relative_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
  template<typename T, typename U> inline double ulp_check(T const& a, U const& b)
  {
    if constexpr(requires { ulp_distance(a, b); }) return ulp_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      {
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      }
      else if constexpr(std::is_floating_point_v<T>)
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;
        if((a == b) || (_::is_nan(a) && _::is_nan(b)))
        {
          return 0.;
        }
        else if(_::is_unordered(a, b))
        {
          return std::numeric_limits<double>::infinity();
        }
        else
        {
          auto aa = _::bitinteger(a);
          auto bb = _::bitinteger(b);
          if(aa > bb) std::swap(aa, bb);
          auto z = static_cast<ui_t>(bb - aa);
          if(_::signbit(a) != _::signbit(b)) ++z;
          return z / 2.;
        }
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        using u_t = typename std::make_unsigned<T>::type;
        return ((a < b) ? u_t(b - a) : u_t(a - b)) / 2.;
      }
      else
      {
        static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>,
                      "[TTS] TTS_ULP_EQUAL requires integral or floating points data to compare."
                      "Did you mean to use TTS_ALL_ULP_EQUAL or to overload tts::ulp_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulp_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
  template<typename T, typename U> inline bool ieee_check(T const& a, U const& b)
  {
    if constexpr(requires { ieee_equal(a, b); }) return ieee_equal(a, b);
    else if constexpr(std::is_floating_point_v<T>)
    {
      return (a == b) || (_::is_nan(a) && _::is_nan(b));
    }
    else
    {
      return _::eq(a, b);
    }
  }
}
#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, PREC, FAILURE)                                 \
  [ & ](auto local_tts_a, auto local_tts_b)                                                        \
  {                                                                                                \
    auto r = FUNC(local_tts_a, local_tts_b);                                                       \
                                                                                                   \
    if(r <= N)                                                                                     \
    {                                                                                              \
      TTS_PASS("Expression: %s == %s within %.*g %s (over %.*g %s).",                              \
               TTS_STRING(LHS),                                                                    \
               TTS_STRING(RHS),                                                                    \
               PREC,                                                                               \
               r,                                                                                  \
               UNIT,                                                                               \
               PREC,                                                                               \
               static_cast<double>(N),                                                             \
               UNIT);                                                                              \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s == %s but %s == %s within %.*g %s when  %.*g %s were expected.",       \
              TTS_STRING(LHS),                                                                     \
              TTS_STRING(RHS),                                                                     \
              ::tts::as_text(local_tts_a).data(),                                                  \
              ::tts::as_text(local_tts_b).data(),                                                  \
              PREC,                                                                                \
              r,                                                                                   \
              UNIT,                                                                                \
              PREC,                                                                                \
              static_cast<double>(N),                                                              \
              UNIT);                                                                               \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(LHS, RHS) 
#define TTS_PRECISION(L, R, N, U, F, P, ...)     TTS_PRECISION_##__VA_ARGS__(L, R, N, U, F, P)
#define TTS_PRECISION_(L, R, N, U, F, P)         TTS_PRECISION_IMPL(L, R, N, U, F, P, TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L, R, N, U, F, P) TTS_PRECISION_IMPL(L, R, N, U, F, P, TTS_FATAL)
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ABSOLUTE_EQUAL(L, R, N, ...)
#else
#define TTS_ABSOLUTE_EQUAL(L, R, N, ...)                                                           \
  TTS_PRECISION(L, R, N, "unit", ::tts::absolute_check, 8, __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_RELATIVE_EQUAL(L, R, N, ...)
#else
#define TTS_RELATIVE_EQUAL(L, R, N, ...)                                                           \
  TTS_PRECISION(L, R, N, "%", ::tts::relative_check, 8, __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ULP_EQUAL(L, R, N, ...)
#else
#define TTS_ULP_EQUAL(L, R, N, ...) TTS_PRECISION(L, R, N, "ULP", ::tts::ulp_check, 2, __VA_ARGS__)
#endif
#define TTS_DO_IEEE_EQUAL_IMPL(LHS, RHS, FAILURE)                                                  \
  [ & ](auto local_tts_a, auto local_tts_b)                                                        \
  {                                                                                                \
    if(::tts::ieee_check(local_tts_a, local_tts_b))                                                \
    {                                                                                              \
      TTS_PASS("Expression: %s == %s.", TTS_STRING(LHS), TTS_STRING(RHS));                         \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s == %s is false because %s != %s.",                                   \
              TTS_STRING(LHS),                                                                     \
              TTS_STRING(RHS),                                                                     \
              ::tts::as_text(local_tts_a).data(),                                                  \
              ::tts::as_text(local_tts_b).data());                                                 \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(LHS, RHS) 
#define TTS_DO_IEEE_EQUAL(L, R, ...)     TTS_DO_IEEE_EQUAL_##__VA_ARGS__(L, R)
#define TTS_DO_IEEE_EQUAL_(L, R)         TTS_DO_IEEE_EQUAL_IMPL(L, R, TTS_FAIL)
#define TTS_DO_IEEE_EQUAL_REQUIRED(L, R) TTS_DO_IEEE_EQUAL_IMPL(L, R, TTS_FATAL)
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_IEEE_EQUAL(L, R, ...)
#else
#define TTS_IEEE_EQUAL(L, R, ...) TTS_DO_IEEE_EQUAL(L, R, __VA_ARGS__)
#endif
namespace tts::_
{
  template<typename T> constexpr auto size(T const& c) noexcept
  {
    if constexpr(requires { c.size(); }) return c.size();
    else if constexpr(requires { size(c); }) return size(c);
    else
    {
      static_assert(
      requires { c.size(); } || requires { size(c); },
      "[TTS] Container type must support .size() or ADL size()");
    }
  }
  template<typename T> constexpr auto begin(T const& c) noexcept
  {
    if constexpr(requires { c.begin(); }) return c.begin();
    else if constexpr(requires { begin(c); }) return begin(c);
    else
    {
      static_assert(
      requires { c.begin(); } || requires { begin(c); },
      "[TTS] Container type must support .begin() or ADL begin()");
    }
  }
  template<typename T> constexpr auto end(T const& c) noexcept
  {
    if constexpr(requires { c.end(); }) return c.end();
    else if constexpr(requires { end(c); }) return end(c);
    else
    {
      static_assert(
      requires { c.end(); } || requires { end(c); },
      "[TTS] Container type must support .end() or ADL end()");
    }
  }
  template<typename T, typename U> struct failure
  {
    std::size_t index;
    T           original;
    U           other;
  };
}
#define TTS_ALL_IMPL(SEQ1, SEQ2, OP, N, UNIT, FAILURE)                                             \
  [](auto const& local_tts_a, auto const& local_tts_b)                                             \
  {                                                                                                \
    if(tts::_::size(local_tts_b) != tts::_::size(local_tts_a))                                     \
    {                                                                                              \
      FAILURE("Expected: %s == %s but found %s elements instead of %s.",                           \
              TTS_STRING(SEQ1),                                                                    \
              TTS_STRING(SEQ2),                                                                    \
              ::tts::as_text(::tts::_::size(local_tts_a)).data(),                                  \
              ::tts::as_text(::tts::_::size(local_tts_b)).data());                                 \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
                                                                                                   \
    auto        ba = tts::_::begin(local_tts_a);                                                   \
    auto        bb = tts::_::begin(local_tts_b);                                                   \
    auto        ea = tts::_::end(local_tts_a);                                                     \
                                                                                                   \
    std::size_t i  = 0;                                                                            \
    ::tts::text failures("");                                                                      \
                                                                                                   \
    while(ba != ea)                                                                                \
    {                                                                                              \
      auto local_tts_err = OP(*ba, *bb);                                                           \
      if(local_tts_err > N)                                                                        \
      {                                                                                            \
        failures += ::tts::text("      @[%ld] : %s and %s differs by %s %s.\n",                    \
                                i++,                                                               \
                                ::tts::as_text(*ba).data(),                                        \
                                ::tts::as_text(*bb).data(),                                        \
                                ::tts::as_text(local_tts_err).data(),                              \
                                UNIT);                                                             \
      }                                                                                            \
      ba++;                                                                                        \
      bb++;                                                                                        \
    }                                                                                              \
                                                                                                   \
    if(!failures.is_empty())                                                                       \
    {                                                                                              \
      FAILURE("Expected: %s == %s but found the following errors:\n%s",                            \
              TTS_STRING(SEQ1),                                                                    \
              TTS_STRING(SEQ2),                                                                    \
              failures.data());                                                                    \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
                                                                                                   \
    ::tts::global_runtime.pass();                                                                  \
    return ::tts::_::logger {false};                                                               \
  }(SEQ1, SEQ2) 
#define TTS_ALL(L, R, F, N, U, ...)     TTS_ALL_##__VA_ARGS__(L, R, F, N, U)
#define TTS_ALL_(L, R, F, N, U)         TTS_ALL_IMPL(L, R, F, N, U, TTS_FAIL)
#define TTS_ALL_REQUIRED(L, R, F, N, U) TTS_ALL_IMPL(L, R, F, N, U, TTS_FATAL)
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)                                                       \
  TTS_ALL(L, R, ::tts::absolute_check, N, "unit", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)                                                       \
  TTS_ALL(L, R, ::tts::relative_check, N, "%", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_ULP_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_ULP_EQUAL(L, R, N, ...) TTS_ALL(L, R, ::tts::ulp_check, N, "ULP", __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_IEEE_EQUAL(L, R, ...)
#else
#define TTS_ALL_IEEE_EQUAL(L, R, ...) TTS_ALL_ULP_EQUAL(L, R, 0, __VA_ARGS__)
#endif
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_EQUAL(L, R, ...)
#else
#define TTS_ALL_EQUAL(L, R, ...) TTS_ALL_ABSOLUTE_EQUAL(L, R, 0 __VA_ARGS__)
#endif
namespace tts::_
{
  struct section_guard
  {
    int&       id;
    int const& section;
    section_guard(int& id_, int const& section_, int& count)
        : id(id_)
        , section(section_)
    {
      if(section == 0) id = count++ - 1;
    }
    bool check(char const* desc)
    {
      if(id == section && desc && is_verbose) printf("  And then: %s\n", desc);
      return id == section;
    }
  };
  struct only_once
  {
    bool     once = true;
    explicit operator bool()
    {
      bool result = once;
      once        = false;
      return result;
    }
  };
}
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_WHEN(STORY)
#else
#define TTS_WHEN(STORY)                                                                            \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW(                                                                      \
  ::tts::_::is_verbose ? printf("When      : %s\n", ::tts::text {STORY}.data()) : 0);              \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count;                                 \
      tts_count -= 0 == tts_section++)                                                             \
    for(tts::_::only_once tts_only_once_setup {}; tts_only_once_setup;)                            \
      TTS_DISABLE_WARNING_POP                                                                      \

#endif
#define TTS_AND_THEN_IMPL(TTS_LOCAL_ID, MESSAGE)                                                   \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  static int TTS_LOCAL_ID = 0;                                                                     \
  if(::tts::_::section_guard(TTS_LOCAL_ID, tts_section, tts_count).check(MESSAGE))                 \
    for(int tts_section = 0, tts_count = 1; tts_section < tts_count;                               \
        tts_count -= 0 == tts_section++)                                                           \
      for(tts::_::only_once tts__only_once_section {}; tts__only_once_section;)                    \
        TTS_DISABLE_WARNING_POP                                                                    \

#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_AND_THEN(MESSAGE)
#else
#define TTS_AND_THEN(MESSAGE) TTS_AND_THEN_IMPL(TTS_UNIQUE(id), MESSAGE)
#endif
#include <cassert>
namespace tts
{
  template<typename Base> struct adapter
  {
    template<typename U, typename Func> static void run(Base const*& src, U*& dst, Func f) noexcept
    {
      *dst++ = f(*src++);
    }
    static auto retrieve(Base const* src) noexcept
    {
      return *src;
    }
    static void display(Base const& v) noexcept
    {
      printf("%s", as_text(v).data());
    }
  };
  namespace _
  {
    template<typename Input, typename Output> struct histogram_entry
    {
      bool   status;
      Input  input_value;
      Output found_value, ref_value;
    };
    template<typename Type, typename In, typename Out, typename Func>
    void compute(In const& inputs, Out& outputs, Func fn)
    {
      auto in  = inputs.data();
      auto end = inputs.data() + inputs.size();
      auto out = outputs.data();
      while(in != end)
        adapter<Type>::run(in, out, fn);
    }
    inline std::size_t last_bucket_less(std::size_t nb_buckets, double ulp) noexcept
    {
      auto next2 = [](double x)
      {
        auto v = static_cast<std::size_t>(_::ceil(x));
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
      };
      std::size_t bucket;
      if(ulp <= 1.5) bucket = static_cast<std::size_t>(_::ceil(ulp * 2));
      else if(_::is_inf(ulp)) bucket = nb_buckets - 1;
      else bucket = _::min(nb_buckets - 2, static_cast<std::size_t>(_::log2(next2(ulp)) + 4));
      return bucket;
    }
    template<typename... S> void header(S const&... s)
    {
      if(::tts::_::is_quiet) return;
      ((printf("%-*s", 16, s)), ...);
      puts("");
    }
    template<typename U, typename R, typename V>
    void results(U ulp, unsigned int count, R ratio, auto desc, V const& v)
    {
      assert(desc && "Description cannot be null");
      if(::tts::_::is_quiet) return;
      if(ulp != -1) printf("%-16.1f%-16u%-16g%s", ulp, count, ratio, desc);
      else printf("%*s", static_cast<int>(48 + strlen(desc)), desc);
      adapter<V>::display(v);
      printf("\n");
    }
    template<typename P> void print_producer(P const& prod, auto alt)
    {
      if(::tts::_::is_quiet) return;
      if constexpr(requires(P const& p) { to_text(p); })
        printf("%s\n", ::tts::as_text(prod).data());
      else printf("%s\n", alt);
    }
  }
  template<typename RefType, typename NewType, typename Generator, typename RefFun, typename NewFun>
  double ulp_histogram(Generator g, RefFun reference, NewFun challenger)
  {
    using out_type  = std::decay_t<std::invoke_result_t<RefFun, RefType>>;
    using nout_type = std::decay_t<std::invoke_result_t<NewFun, NewType>>;
    std::size_t count = ::tts::arguments().value(4096, "--block");
    _::buffer<out_type> ref_out(count), new_out(count);
    _::buffer<RefType>  inputs(count);
    for(std::size_t i = 0; i < inputs.size(); ++i)
      inputs[ i ] = produce(type<RefType> {}, g, i, count);
    std::size_t             repetition = ::tts::arguments().value(1, "--loop");
    double                  max_ulp    = 0.;
    std::size_t             nb_buckets = 2 + 1 + 16;
    std::size_t             nb_ulps    = 0;
    _::buffer<unsigned int> ulp_map(nb_buckets, 0);
    _::buffer<_::histogram_entry<NewType, nout_type>> samples(nb_buckets, {false, {}, {}, {}});
    for(std::size_t r = 0; r < repetition; ++r)
    {
      _::compute<RefType>(inputs, ref_out, reference);
      _::compute<NewType>(inputs, new_out, challenger);
      _::buffer<double> ulpdists(count);
      for(std::size_t i = 0; i < ulpdists.size(); ++i)
      {
        nb_ulps++;
        ulpdists[ i ] = ::tts::ulp_check(ref_out[ i ], new_out[ i ]);
        max_ulp       = _::max(max_ulp, ulpdists[ i ]);
        auto idx      = _::last_bucket_less(nb_buckets, ulpdists[ i ]);
        ulp_map[ idx ]++;
        if(!samples[ idx ].status)
        {
          samples[ idx ] = {true,
                            adapter<NewType>::retrieve(&inputs[ i ]),
                            adapter<nout_type>::retrieve(&new_out[ i ]),
                            adapter<nout_type>::retrieve(&ref_out[ i ])};
        }
      }
    }
    _::header("Max ULP", "Count (#)", "Ratio Sum (%)", "Samples");
    if(!_::is_quiet)
      printf("--------------------------------------------------------------------------------\n");
    double ratio = 0.;
    for(std::size_t i = 0; i < ulp_map.size(); ++i)
    {
      if(ulp_map[ i ] != 0)
      {
        double ulps  = 0;
        ratio       += (100. * ulp_map[ i ]) / nb_ulps;
        if(i <= 3) ulps = i / 2.0;
        else if(i == nb_buckets - 1) ulps = std::numeric_limits<double>::infinity();
        else ulps = 1 << (i - 4);
        auto [ s, in, out, ref ] = samples[ i ];
        _::results(ulps, ulp_map[ i ], ratio, "Input:      ", in);
        _::results(-1., -1, -1., "Found:      ", out);
        _::results(-1., -1, -1., "instead of: ", ref);
        if(!_::is_quiet)
          printf(
          "--------------------------------------------------------------------------------\n");
      }
    }
    return max_ulp;
  }
}
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)
#else
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)                  \
  [ & ]()                                                                                          \
  {                                                                                                \
    if(!::tts::_::is_quiet)                                                                        \
      printf("Comparing: %s<%s> with %s<%s> using ",                                               \
             TTS_STRING(RefFunc),                                                                  \
             TTS_STRING(TTS_REMOVE_PARENS(RefType)),                                               \
             TTS_STRING(NewFunc),                                                                  \
             TTS_STRING(TTS_REMOVE_PARENS(NewType)));                                              \
                                                                                                   \
    auto generator = TTS_REMOVE_PARENS(Producer);                                                  \
    ::tts::_::print_producer(generator, TTS_STRING(TTS_REMOVE_PARENS(Producer)));                  \
                                                                                                   \
    double tts_ulp_max         = Ulpmax;                                                           \
    double local_tts_threshold = ::tts::arguments().value(tts_ulp_max, "--ulpmax");                \
    double local_tts_max_ulp =                                                                     \
    ::tts::ulp_histogram<TTS_REMOVE_PARENS(RefType), TTS_REMOVE_PARENS(NewType)>(                  \
    generator, RefFunc, NewFunc);                                                                  \
                                                                                                   \
    if(local_tts_max_ulp <= local_tts_threshold)                                                   \
    {                                                                                              \
      ::tts::global_runtime.pass();                                                                \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting: %s similar to %s within %s ULP but found: %s ULP instead",              \
               TTS_STRING(NewFunc),                                                                \
               TTS_STRING(RefFunc),                                                                \
               ::tts::as_text(local_tts_threshold).data(),                                         \
               ::tts::as_text(local_tts_max_ulp).data());                                          \
    }                                                                                              \
  }()
#endif
namespace tts
{
  template<typename T> struct realistic_generator
  {
    realistic_generator(T mn, T mx)
        : mini(mn)
        , maxi(mx)
    {
    }
    T operator()(auto, auto, auto) const
    {
      return ::tts::random_value(mini, maxi);
    }
    friend tts::text to_text(realistic_generator const& s)
    {
      return tts::text {"realistic_generator<%s>(%s,%s)",
                        tts::as_text(typename_<T>).data(),
                        tts::as_text(s.mini).data(),
                        tts::as_text(s.maxi).data()};
    }
  private:
    T mini, maxi;
  };
}
