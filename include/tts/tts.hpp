//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
#include <iostream>
namespace tts
{
  inline int usage(const char* name)
  {
    std::cout << "TTS Unit Tests Driver\n";
    std::cout << "Usage: " << name  << " [OPTION...]\n";
    std::cout << "\nFlags:\n";
    std::cout << "  -h, --help        Display this help message\n";
    std::cout << "  -x, --hex         Print the floating results in hexfloat mode\n";
    std::cout << "  -s, --scientific  Print the floating results in scientific mode\n";
    std::cout << "\nParameters:\n";
    std::cout << "  --precision=arg   Set the precision for displaying floating pint values\n";
    std::cout << "  --seed=arg        Set the PRNG seeds (default is time-based)\n";
    std::cout << "\nRange specifics Parameters:\n";
    std::cout << "  --block=arg       Set size of range checks samples (min. 32)\n";
    std::cout << "  --loop=arg        Repeat each range checks arg times\n";
    std::cout << "  --ulpmax=arg      Set global failure ulp threshold for range tests (default is 2.0)\n";
    std::cout << "  --valmax=arg      Set maximal value for range tests (default is code)\n";
    std::cout << "  --valmin=arg      Set minimal value for range tests (default is code)\n";
    std::cout << std::endl;
    return 0;
  }
}
#include <utility>
namespace tts
{
  struct callable
  {
    public:
    using signature_t = void(*)(void*);
    signature_t invoker = {};
    signature_t cleanup = {};
    void*       payload = {};
    constexpr callable() = default;
    template<typename Function>
    constexpr callable(Function f)
            : invoker{invoke<Function>}, cleanup{destroy<Function>}
            , payload{new Function{std::move(f)}}
    {}
    constexpr callable(callable&& other) noexcept
            : invoker{std::move(other.invoker)}, cleanup{std::move(other.cleanup)}
            , payload{std::move(other.payload)}
    {
      other.payload = {};
    }
    ~callable() { cleanup(payload); }
    constexpr callable(const callable&)             = delete;
    constexpr callable& operator=(const callable&)  = delete;
    constexpr callable& operator=(callable&&)       = delete;
    constexpr void operator()()       { invoker(payload); }
    constexpr void operator()() const { invoker(payload); }
    explicit constexpr operator bool() const { return payload != nullptr; }
    private:
    template <typename T>
    static void invoke(void* data) { (*static_cast<T*>(data))(); }
    template <typename T>
    static void destroy(void* data) { delete static_cast<T*>(data); }
  };
}
#include <vector>
#include <string>
namespace tts::detail
{
  inline std::string current_test = "";
  struct test
  {
    void operator()()
    {
      current_test = name;
      behaviour();
    }
    static inline bool acknowledge(test&& f);
    std::string     name;
    tts::callable   behaviour;
  };
  inline std::vector<test> suite = {};
  bool inline test::acknowledge(test&& f)
  {
    suite.emplace_back( std::forward<test>(f));
    return true;
  }
}
#include <cstddef>
#include <iostream>
namespace tts::detail
{
  struct env
  {
    void pass()     { test_count++; success_count++; }
    void fail()     { test_count++; failure_count++; }
    void fatal()    { test_count++; failure_count++; fatal_count++; }
    void invalid()  { test_count++; invalid_count++; }
    int report(std::ptrdiff_t fails, std::ptrdiff_t invalids) const
    {
      auto test_txt = test_count    > 1 ? "tests" : "test";
      auto pass_txt = success_count > 1 ? "successes" : "success";
      auto fail_txt = failure_count > 1 ? "failures" : "failure";
      auto inv_txt  = invalid_count > 1 ? "invalids" : "invalid";
      auto passes   = (fails || invalids) ?  0 : test_count;
      std::cout << "----------------------------------------------------------------\n";
      std::cout << "Results: " << test_count << " " << test_txt << " - "
                << success_count << "/" << passes << " " << pass_txt << " - "
                << failure_count << "/" << fails << " " << fail_txt << " - "
                << invalid_count << "/" << invalids << " " << inv_txt << "\n";
      if(!fails && !invalids) return test_count == success_count ? 0 : 1;
      else                    return (failure_count == fails && invalid_count == invalids) ? 0 : 1;
    }
    int test_count    = 0,
        success_count = 0,
        failure_count = 0,
        fatal_count   = 0,
        invalid_count = 0;
    bool fail_status = false;
  };
}
namespace tts
{
  inline ::tts::detail::env global_runtime;
  inline int report(std::ptrdiff_t fails, std::ptrdiff_t invalids)
  {
    return global_runtime.report(fails,invalids);
  }
}
#include <chrono>
#include <cmath>
#include <initializer_list>
#include <random>
#include <sstream>
#include <string>
namespace tts
{
  struct option
  {
    option() = default;
    option( std::string arg ) : token(std::move(arg)), position(token.rfind( '=' )) {}
    auto flag()     const { return token.substr(0, position); }
    bool is_valid() const { return !flag().empty(); }
    template<typename T> T get(T const& def = T{}) const
    {
      T that;
      if(is_valid())
      {
        std::istringstream os(token.substr(position+1));
        if(os >> that) return that;
        else           return def;
      }
      else
      {
        return def;
      }
    }
    std::string token     = "";
    size_t      position  = std::string::npos;
  };
  struct options
  {
    using params_t = std::initializer_list<const char*>;
    option find(const char* f ) const { return find({f}); }
    option find(params_t    fs) const
    {
      for(int i=1;i<argc;++i)
      {
        option o(argv[i]);
        for(auto f : fs)
          if( o.flag() == f ) return o;
      }
      return option{};
    }
    bool operator[](params_t    fs) const { return find(fs).is_valid(); }
    bool operator[](const char* f ) const { return operator[]({f}); }
    template<typename T> T value(params_t fs, T that = {}) const
    {
      if( auto o = find(fs); o.is_valid()) that = o.template get<T>(that);
      return that;
    }
    template<typename T> T value(const char* f, T that = {}) const
    {
      return value({f},that);
    }
    bool is_valid() { return argc && argv != nullptr; }
    int argc;
    char const** argv;
  };
  namespace detail
  {
    inline ::tts::options current_arguments = {0,nullptr};
    inline std::int32_t   current_seed      = -1;
  }
  inline void initialize(int argc, const char** argv)
  {
    if(!detail::current_arguments.is_valid())
      detail::current_arguments = ::tts::options{argc,argv};
  }
  inline ::tts::options const& arguments()
  {
    return detail::current_arguments;
  }
  inline std::int32_t random_seed(int base_seed = -1)
  {
    if(detail::current_seed == -1)
    {
      auto s = ::tts::arguments().value( "--seed", base_seed );
      if(s == -1 )
      {
        auto now = std::chrono::high_resolution_clock::now();
        s        = static_cast<std::int32_t>(now.time_since_epoch().count());
      }
      detail::current_seed = s;
    }
    return detail::current_seed;
  }
}
#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#  define TTS_CUSTOM_DRIVER_FUNCTION main
namespace tts::detail { constexpr bool use_main = true; }
#else
namespace tts::detail { constexpr bool use_main = false; }
#endif
namespace tts::detail { struct fatal_signal {}; }
#if defined(TTS_MAIN)
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc,[[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc,argv);
  if( ::tts::arguments()[{"-h","--help"}] )
    return ::tts::usage(argv[0]);
  auto nb_tests = ::tts::detail::suite.size();
  std::size_t done_tests = 0;
  try
  {
    for(auto &t: ::tts::detail::suite)
    {
      auto test_count     = ::tts::global_runtime.test_count;
      auto failure_count  = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;
      t();
      done_tests++;
      if(test_count     == ::tts::global_runtime.test_count)
      {
        ::tts::global_runtime.invalid();
        std::cout << "[!] - " << ::tts::detail::current_test << " : EMPTY TEST CASE\n";
      }
      else if(failure_count  == ::tts::global_runtime.failure_count)
      {
        std::cout << "[V] - " << ::tts::detail::current_test << "\n";
      }
    }
  }
  catch( ::tts::detail::fatal_signal& )
  {
    std::cout << "@@ ABORTING DUE TO EARLY FAILURE @@ - "
              << (nb_tests - done_tests - 1) << " Tests not run\n";
  }
  if constexpr( ::tts::detail::use_main )   return ::tts::report(0,0);
  else                                      return 0;
}
#endif
#include <iomanip>
#include <sstream>
#include <type_traits>
namespace tts
{
  template<typename T>
  concept support_std_to_string = requires(T e) { std::to_string(e); };
  template<typename T>
  concept support_to_string = requires(T e) { to_string(e); };
  template<typename T>
  concept has_to_string = requires(T e) { e.to_string(); };
  template<typename T>
  concept sequence = requires(T e) {std::begin(e); std::end(e); };
  template<typename T>
  concept streamable = requires(T e, std::ostream& o) { o << e; };
}
#ifndef TTS_FUNCTION
#define TTS_FUNCTION TTS_UNIQUE(tts_function)
#endif
#ifndef TTS_REGISTRATION
#define TTS_REGISTRATION TTS_UNIQUE(tts_registration)
#endif
#define TTS_UNIQUE3(ID, LINE) ID##LINE
#define TTS_UNIQUE2(ID, LINE) TTS_UNIQUE3(ID, LINE)
#define TTS_UNIQUE(ID)        TTS_UNIQUE2(ID, __COUNTER__)
#define TTS_CAT(x, y)                     TTS_CAT_I(x, y)
#define TTS_CAT_I(x, y)                   x##y
#define TTS_STRING(...)   TTS_STRING_((__VA_ARGS__))
#define TTS_STRING__(...) #__VA_ARGS__
#define TTS_STRING_(TXT)  TTS_STRING__ TXT
#define TTS_REMOVE_PARENS(x)              TTS_EVAL((TTS_REMOVE_PARENS_I x), x)
#define TTS_REMOVE_PARENS_I(...)          1, 1
#define TTS_APPLY(macro, args)            TTS_APPLY_I(macro, args)
#define TTS_APPLY_I(macro, args)          macro args
#define TTS_EVAL_I(test, x)               TTS_MAYBE_STRIP_PARENS(TTS_TEST_ARITY test, x)
#define TTS_EVAL(test, x)                 TTS_EVAL_I(test, x)
#define TTS_TEST_ARITY(...)               TTS_APPLY(TTS_TEST_ARITY_I, (__VA_ARGS__, 2, 1))
#define TTS_TEST_ARITY_I(a, b, c, ...)    c
#define TTS_MAYBE_STRIP_PARENS(cond, x)   TTS_MAYBE_STRIP_PARENS_I(cond, x)
#define TTS_MAYBE_STRIP_PARENS_I(cond, x) TTS_CAT(TTS_MAYBE_STRIP_PARENS_, cond)(x)
#define TTS_MAYBE_STRIP_PARENS_1(x)       x
#define TTS_MAYBE_STRIP_PARENS_2(x)       TTS_APPLY(TTS_MAYBE_STRIP_PARENS_2_I, x)
#define TTS_MAYBE_STRIP_PARENS_2_I(...)   __VA_ARGS__
#include <string_view>
#include <string>
namespace tts::detail
{
  template<typename T> struct typename_impl
  {
    static auto value() noexcept
    {
  #if defined(_MSC_VER )
      std::string_view data(__FUNCSIG__);
      auto i = data.find('<') + 1,
        j = data.find(">::value");
      auto name = data.substr(i, j - i);
  #else
      std::string_view data(__PRETTY_FUNCTION__);
      auto i = data.find('=') + 2,
        j = data.find_last_of(']');
      auto name = data.substr(i, j - i);
  #endif
      return std::string(name.data(), name.size());
    }
  };
}
namespace tts
{
  template<typename T> inline auto const typename_ = detail::typename_impl<T>::value();
  template<typename T> constexpr auto name(T const&){ return typename_<T>; }
}
#include <cstdint>
namespace tts
{
  template<typename... Ts>
  struct types
  {
    template<typename... Us> constexpr types<Ts...,Us...> operator+( types<Us...> const&) const;
  };
  template<typename... Ls> struct concatenate { using type = decltype( (Ls{} + ...) ); };
  template<typename... Ls> using concatenate_t = typename concatenate<Ls...>::type;
  template<typename... T> struct type {};
  using real_types        = types < double,float>;
  using int_types         = types < std::int64_t , std::int32_t , std::int16_t , std::int8_t>;
  using signed_types      = concatenate_t<real_types,int_types>;
  using uint_types        = types < std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t>;
  using arithmetic_types  = concatenate_t<real_types,int_types,uint_types>;
}
namespace tts::detail
{
  struct test_capture
  {
    test_capture(const char* id) : name(id) {}
    auto operator+(auto body) const { return test::acknowledge( {name, body} ); }
    const char* name;
  };
  inline std::string current_type = {};
  template<typename... Types> struct test_captures
  {
    test_captures(const char* id) : name(id) {}
    auto operator+(auto body) const
    {
      return test::acknowledge( { name
                                , [=]()
                                  {
                                    ( ( (current_type = " with [T = " + typename_<Types> + "]")
                                      , body(type<Types>())
                                      )
                                    , ...
                                    );
                                    current_type.clear();
                                  }
                                }
                              );
    }
    std::string name;
  };
  template<typename... Types>
  struct test_captures<types<Types...>> : test_captures<Types...>
  {};
  template<typename Generator>
  requires requires(Generator g) { typename Generator::types_list; }
  struct test_captures<Generator> : test_captures<typename Generator::types_list>
  {};
}
namespace tts::detail
{
  template<typename Generator, typename... Types> struct test_generators
  {
    test_generators(const char* id, Generator g, Types...) : name(id), generator(g) {}
    auto operator+(auto body)
    {
      std::mt19937 gen(::tts::random_seed());
      return test::acknowledge( { name
                                , [*this,body,gen]() mutable
                                  {
                                    ( ( (current_type = " with [T = " + typename_<Types> + "]")
                                      , std::apply(body, generator(type<Types>{}, gen))
                                      ), ...
                                    );
                                    current_type.clear();
                                  }
                                }
                              );
    }
    std::string name;
    Generator   generator;
  };
  template<typename Generator, typename... Types>
  test_generators(const char*,Generator,Types...) -> test_generators<Generator,Types...>;
  template<typename Generator, typename... Types>
  struct  test_generators<Generator, types<Types...>>
        : test_generators<Generator,Types...>
  {
    using parent = test_generators<Generator,Types...>;
    test_generators(const char* id, Generator g, types<Types...>) : parent(id,g,Types{}...) {}
  };
  template<typename Generator, typename TypeGenerator>
  requires requires(TypeGenerator g) { typename TypeGenerator::types_list; }
  struct  test_generators<Generator,TypeGenerator>
        : test_generators<Generator,typename TypeGenerator::types_list>
  {
    using parent = test_generators<Generator,typename TypeGenerator::types_list>;
    test_generators ( const char* id, Generator g, TypeGenerator )
                    : parent(id,g,typename TypeGenerator::types_list{}) {}
  };
}
#define TTS_PROTOTYPE(...) []__VA_ARGS__
#define TTS_CASE(ID)                                                                                \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_capture{ID} + TTS_PROTOTYPE(()) \

#define TTS_CASE_TPL(ID,...)                                                                        \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_captures<__VA_ARGS__>{ID}       \
                                              + TTS_PROTOTYPE()                                     \

#define TTS_CASE_WITH(ID, TYPES, GENERATOR)                                                         \
static bool const TTS_CAT(case_,TTS_FUNCTION)                                                       \
                  = ::tts::detail::test_generators{ID,GENERATOR,TYPES{}} + TTS_PROTOTYPE()          \

#include <string_view>
#include <ostream>
namespace tts
{
  class source_location
  {
    public:
    [[nodiscard]] static constexpr auto current ( const char* file  = __builtin_FILE()
                                                , int line          = __builtin_LINE()
                                                ) noexcept
    {
      source_location sl{};
      sl.file_ = file;
      sl.line_ = line;
      return sl;
    }
    [[nodiscard]] constexpr auto filename() const noexcept
    {
      std::string_view f(file_);
      return f.substr(f.find_last_of('/')+1);
    }
    [[nodiscard]] constexpr auto line() const noexcept { return line_; }
    friend std::ostream& operator<<(std::ostream& os, source_location const& s)
    {
      return os << "[" << s.filename() << ":" << s.line() << "]";
    }
    private:
    const char* file_{"unknown"};
    int         line_{};
  };
}
#define TTS_FAIL(Message)                                                                           \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.fail();                                                                   \
    if(!::tts::global_runtime.fail_status)                                                          \
    {                                                                                               \
      ::tts::global_runtime.fail_status = true;                                                     \
      std::cout << "[X] - " << ::tts::detail::current_test<< "\n";                                  \
    }                                                                                               \
    if( !::tts::detail::current_type.empty())                                                       \
    {                                                                                               \
      std::cout << "  > " << ::tts::detail::current_type << "\n";                                   \
    }                                                                                               \
    std::cout << "    " << ::tts::source_location::current() << " - ** FAILURE **"                  \
              << " : " << Message << std::endl;                                                     \
  }()
#define TTS_FATAL(Message)                                                                          \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.fatal();                                                                  \
    if(!::tts::global_runtime.fail_status)                                                          \
    {                                                                                               \
      ::tts::global_runtime.fail_status = true;                                                     \
      std::cout << "[@] - " << ::tts::detail::current_test<< "\n";                                  \
    }                                                                                               \
    if( !::tts::detail::current_type.empty())                                                       \
    {                                                                                               \
      std::cout << "  > " << ::tts::detail::current_type << "\n";                                   \
    }                                                                                               \
    std::cout << "    " << ::tts::source_location::current() << " - @@ FATAL @@"                    \
              << " : " << Message << std::endl;                                                     \
    throw ::tts::detail::fatal_signal();                                                            \
  }()
#include <iostream>
namespace tts
{
  struct logger
  {
    logger(bool status = true) : display(status), done(false) {}
    template<typename Data> logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          std::cout << ">> Additionnal information: \n";
          done = true;
        }
        std::cout << d;
      }
      return *this;
    }
    ~logger() { if(display && done) std::cout << "\n"; }
    bool display, done;
  };
}
#define TTS_EXPECT(EXPR, ...)     TTS_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL((EXPR),TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR),TTS_FATAL)
#define TTS_EXPECT_IMPL(EXPR,FAILURE)                                                               \
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( expr )                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(TTS_REMOVE_PARENS(EXPR)) << " evaluates to false." );   \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(EXPR)                                                                                             \

#define TTS_EXPECT_NOT(EXPR, ...)       TTS_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_NOT_(EXPR)           TTS_EXPECT_NOT_IMPL(EXPR,TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR)   TTS_EXPECT_NOT_IMPL(EXPR,TTS_FATAL)
#define TTS_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                           \
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( !expr )                                                                                       \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(EXPR) << " evaluates to true." );                       \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(EXPR)                                                                                             \

#define TTS_CONSTEXPR_EXPECT(EXPR)                                                                  \
[]<typename C>(C )                                                                                  \
{                                                                                                   \
  static_assert ( C::value                                                                          \
                , "[TTS] - Error: " TTS_STRING(EXPR) " evaluates to false at compile-time."         \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}(std::bool_constant<EXPR>{})                                                                       \

#define TTS_CONSTEXPR_EXPECT_NOT(EXPR)                                                              \
[]<typename C>(C )                                                                                  \
{                                                                                                   \
  static_assert ( !C::value                                                                         \
                , "[TTS] - Error: " TTS_STRING(EXPR) " evaluates to true at compile-time."          \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}(std::bool_constant<EXPR>{})                                                                       \

#include <cmath>
#include <limits>
#include <numeric>
#include <random>
#include <type_traits>
namespace tts
{
  namespace detail
  {
    template<typename T> auto dec(T x)  { return x-T(1); }
    template<typename T> auto inc(T x)  { return x+T(1); }
    template<typename T> T    abs_s(T x)
    {
      if constexpr( std::is_unsigned_v<T>) return x;
      else
      {
        return x == std::numeric_limits<T>::min() ? std::numeric_limits<T>::max()
                                                  : (x<0 ? -x : x);
      }
    }
    template< typename T = double > struct fp_realistic_distribution
    {
      using result_type = T;
      struct param_type
      {
        T a, b;
        param_type(T aa, T bb) : a(aa),  b(bb) {};
      };
      fp_realistic_distribution() : fp_realistic_distribution(0.0, 0.1, 300) { }
      fp_realistic_distribution(T aa, T bb)
        : a(std::min(aa, bb)),
          b(std::max(aa, bb)),
          nb(300),
          sd(T(0),T(1)),
          ird(1, nb-1){
      };
      explicit fp_realistic_distribution( const param_type& params )
        : a(params.a),
          b(params.b),
          nb(300),
          sd(T(0),T(1)),
          ird(1, nb-1){};
      void reset(){
        sd.reset();
        ird.reset();
      };
      template< class Generator > result_type operator()( Generator& gen )
      {
        return (*this)(gen, a, b);
      }
      template< class Generator > result_type operator()( Generator& gen, result_type aa, result_type bb)
      {
        result_type res;
        if(aa == bb) res = aa;
        else if(bb <= aa+result_type(0.5)) res = aa+(bb-aa)*sd(gen);
        else if((aa >= 0 && bb <= 1) || (bb <= 0 && aa >= -1)) res = aa+(bb-aa)*sd(gen);
        else if(aa >= -1 && bb <= 1) res =((sd(gen)*(bb-aa) > -aa) ? aa : bb)*sd(gen);
        else
        {
          auto i = ird(gen);
          if (aa >= 1)
          {
            auto la =  std::log2(aa);
            auto f =  std::log2(bb)-la;
            auto rand = sd(gen);
            auto x = la+f*(i-1+rand)/nb;
            res = std::min(std::exp2(x), bb);
          }
          else if (bb <= -1)
          {
            res = -(*this)(gen, std::abs(bb), std::abs(aa));
          }
          else if (aa >= 0)
          {
            if(i == 1)
            {
              auto r = sd(gen);
              if (r> aa) res =r;
              else {i = 2;res = 0; }
            }
            else res = (*this)(gen, result_type(1), bb);
          }
          else if (bb <= 0)
          {
            if(i == 1)
            {
              auto r = sd(gen);
              if (r> -bb) res =-r;
              else { i = 2; res = 0;}
            }
            else res = (*this)(gen, result_type(-1), aa);
          }
          else
          {
            auto z = result_type(0);
            auto choice = sd(gen)*std::midpoint(bb, -aa) <  bb/2;
            if (choice)
            {
              res = (*this)(gen, z, bb);
            }
            else
            {
              res = (*this)(gen, aa, z);
            }
          }
        }
        return res;
      }
      template< class Generator > result_type operator()( Generator& g, const param_type& params );
      param_type  param() const noexcept { return {a, b}; }
      void        param( const param_type& params ) noexcept
      {
        a = params.a;
        b = params.b;
      }
      result_type min()  const noexcept {return a; };
      result_type max()  const noexcept {return b; };
    private:
      T a;
      T b;
      int nb;
      std::uniform_real_distribution<T> sd;
      std::uniform_int_distribution<int> ird;
    };
    template< typename T = int32_t > struct integral_realistic_distribution
    {
      using result_type = T;
      struct param_type
      {
        T a;
        T b;
        param_type(T aa, T bb) : a(aa),  b(bb){};
      };
      integral_realistic_distribution() : integral_realistic_distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) { }
      integral_realistic_distribution( T aa, T bb, int nbb = 300)
        : a(std::min(aa, bb)),
          b(std::max(aa, bb)),
          nb(nbb),
          sd(0.0, 1.0),
          ird(a, b),
          ird2(1, nb) {};
      explicit integral_realistic_distribution( const param_type& params )
        : a(params.a),
          b(params.b),
          nb(params.nb),
          sd(0.0, 1.0),
          ird(a, b),
          ird2(1, nb) {};
      void reset(){
        ird.reset();
        ird2.reset();
      };
      template< class Generator > result_type operator()( Generator& gen )
      {
        return (*this)(gen, a, b);
      }
      template< class Generator > result_type operator()( Generator& gen, result_type aa, result_type bb)
      {
        result_type res(0);
        if(detail::abs_s(aa) < 256 && detail::abs_s(bb) < 256)
        {
          res = ird(gen);
        }
        auto l2 = [](auto x){return std::log2(detail::inc(double(x)));   };
        auto e2 = [](auto x){return detail::dec(T(std::round(std::exp2(x)))); };
        if(aa == bb) res = aa;
        else
        {
          auto i = ird2(gen);
          if (aa >= 0)
          {
            auto la =  l2(aa);
            auto lb =  l2(bb);
            auto f =   lb-la;
            auto rand = sd(gen);
            auto x = la+f*(i-1+rand)/nb;
            res = e2(x);
          }
          else if (bb <= 0)
          {
            res = -(*this)(gen, detail::abs_s(bb), detail::abs_s(aa));
          }
          else
          {
            auto z = result_type(0);
            auto choice = sd(gen)*std::midpoint(detail::abs_s(bb), detail::abs_s(aa)) <  bb/2;
            if (choice)
            {
              res = (*this)(gen, z,detail::abs_s(bb));
            }
            else
            {
              res = -(*this)(gen, z, detail::abs_s(aa));
            }
          }
        }
        return res;
      }
      template< class Generator > result_type operator()( Generator& g, const param_type& params );
      param_type param() const noexcept { return {a, b}; }
      void param( const param_type& params ) noexcept
      {
        a = params.a;
        b = params.b;
      }
      result_type min()  const noexcept { return a; };
      result_type max()  const noexcept { return b; };
    private:
      T a;
      T b;
      int nb;
      std::uniform_real_distribution<float> sd;
      std::uniform_int_distribution<int> ird;
      std::uniform_int_distribution<int> ird2;
    };
  }
  template<typename T>
  struct realistic_distribution : tts::detail::integral_realistic_distribution<T>
  {
    using parent = tts::detail::integral_realistic_distribution<T>;
    using parent::parent;
  };
  template<typename T>
  requires(std::is_floating_point_v<T>)
  struct realistic_distribution<T>  : tts::detail::fp_realistic_distribution<T>
  {
    using parent = tts::detail::fp_realistic_distribution<T>;
    using parent::parent;
  };
}
#include <tuple>
namespace tts
{
  template<typename... G> inline auto generate(G... g)
  {
    return [=](auto const& t, auto& rng, auto... others)
    {
      return std::make_tuple(produce(t,g,rng,others...)...);
    };
  }
  template<tts::sequence T>
  auto produce(type<T> const& t, auto g, auto& rng, auto... args)
  {
    using value_type = std::remove_cvref_t<decltype(*std::begin(std::declval<T>()))>;
    T that;
    auto b = std::begin(that);
    auto e = std::end(that);
    std::size_t sz = e - b;
    for(std::size_t i=0;i<sz;++i)
    {
      *b++ = g(tts::type<value_type>{},rng,i,sz,args...);
    }
    return that;
  }
  template<typename T> struct value
  {
    value(T v) : seed(v) {}
    template<typename U>
    auto operator()(tts::type<U>, auto&, auto...) const
    {
      return as_value<U>(seed);
    }
    T seed;
  };
  template<typename T, typename U = T> struct ramp
  {
    ramp(T s)       : start(s), step(1)   {}
    ramp(T s, U st) : start(s), step(st)  {}
    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }
    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto...) const
    {
      return as_value<D>(start)+idx*as_value<D>(step);
    }
    T start;
    U step;
  };
  template<typename T, typename U = T> struct reverse_ramp
  {
    reverse_ramp(T s)       : start(s), step(1)   {}
    reverse_ramp(T s, U st) : start(s), step(st)  {}
    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }
    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto sz, auto...) const
    {
      return as_value<D>(start)+(sz-1-idx)*as_value<D>(step);
    }
    T start;
    U step;
  };
  template<typename T, typename U = T> struct between
  {
     between(T s, U st) : first(s), last(st)  {}
    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(first); }
    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto sz, auto...) const
    {
      auto w1   = as_value<D>(first);
      auto w2   = as_value<D>(last);
      auto step = (sz-1) ? (w2-w1)/(sz-1) : 0;
      return std::min( as_value<D>(w1 + idx*step), w2);
    }
    T first;
    U last;
  };
  template<typename Distribution> struct sample
  {
    sample(Distribution d)  : dist(std::move(d))  {}
    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...)
    {
      return as_value<D>(dist(rng));
    }
    Distribution dist;
  };
  template<typename Mx, typename Mn> struct randoms
  {
    randoms(Mn mn, Mx mx)  : mini(mn), maxi(mx)  {}
    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...)
    {
      static tts::realistic_distribution<D> dist(as_value<D>(mini), as_value<D>(maxi));
      return dist(rng);
    }
    Mn mini;
    Mx maxi;
  };
  template<typename T, typename V> auto as_value(V const& v) { return static_cast<T>(v); }
  template<typename T> auto produce(type<T> const& t, auto g, auto& rng, auto... others)
  {
    return g(t,rng, others...);
  }
}
namespace tts::detail
{
  template<typename L, typename R>
  concept comparable_equal  = requires(L l, R r) { compare_equal(l,r); };
  template<typename L, typename R>
  concept comparable_less   = requires(L l, R r) { compare_less(l,r); };
  template<typename L, typename R> inline constexpr bool eq(L const &l, R const &r)
  {
    if constexpr( comparable_equal<L,R> ) return compare_equal(l,r);
    else                                  return l == r;
  }
  template<typename L, typename R> inline constexpr bool neq(L const &l, R const &r)
  {
    return !eq(l,r);
  }
  template<typename L, typename R> inline constexpr bool lt(L const &l, R const &r)
  {
    if constexpr( comparable_less<L,R> )  return compare_less(l,r);
    else                                  return l < r;
  }
  template<typename L, typename R> inline constexpr bool le(L const &l, R const &r)
  {
    return lt(l, r) || eq(l, r);
  }
  template<typename L, typename R> inline constexpr bool gt(L const &l, R const &r)
  {
    return !le(l,r);
  }
  template<typename L, typename R> inline constexpr bool ge(L const &l, R const &r)
  {
    return !lt(l,r);
  }
}
#include <iomanip>
#include <sstream>
#include <type_traits>
namespace tts
{
  template<typename T> std::string as_string(T const& e)
  {
    if constexpr( std::is_pointer_v<T> )
    {
      std::ostringstream os;
      os << std::string(typename_<T>) << "(" << e << ")";
      return os.str();
    }
    else if constexpr( std::floating_point<T> )
    {
      auto precision = ::tts::arguments().value({"--precision"}, -1);
      bool hexmode   = ::tts::arguments()[{"-x","--hex"}];
      bool scimode   = ::tts::arguments()[{"-s","--scientific"}];
      std::ostringstream os;
      if(precision != -1 ) os << std::setprecision(precision);
            if(hexmode) os << std::hexfloat   << e << std::defaultfloat;
      else  if(scimode) os << std::scientific << e << std::defaultfloat;
      else              os << e;
      return os.str();
    }
    else if constexpr( support_std_to_string<T> )
    {
      return std::to_string(e);
    }
    else if constexpr( streamable<T> )
    {
      std::ostringstream os;
      os << e;
      return os.str();
    }
    else if constexpr( support_to_string<T> )
    {
      return to_string(e);
    }
    else if constexpr( sequence<T> )
    {
      std::string that = "{ ";
      for(auto const& v : e) that += as_string(v) + " ";
      that += "}";
      return that;
    }
    else
    {
      std::ostringstream os;
      os << "[" << std::string(typename_<T>) << "]@(" << &e << ")";
      return os.str();
    }
  }
  inline std::string as_string(bool b) { return b ? std::string("true") : std::string("false"); }
  inline std::string as_string(std::string const& e)      { return  e;                          }
  inline std::string as_string(std::string_view const& e) { return  std::string(e);             }
  inline std::string as_string(std::nullptr_t)            { return  std::string("nullptr");     }
  inline std::string as_string(const char* e)
  {
    std::ostringstream os;
    os << "char*(" << (void*)e << ")";
    return os.str();
  }
  inline std::string as_string(char* e )
  {
    std::ostringstream os;
    os << "char*(" << (void*)e << ")";
    return os.str();
  }
}
#define TTS_RELATION(A, B, OP, T, F, ...)    TTS_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)
#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                  \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  if( ::tts::detail::OP(a,b) )                                                                      \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Expression: "  << TTS_STRING(A) << " " T " " << TTS_STRING(B)                      \
            <<  " is false because: " << ::tts::as_string(a) << " " F " " << ::tts::as_string(b)    \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(A,B)                                                                                              \

#define TTS_EQUAL(LHS, RHS, ...)          TTS_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)
#define TTS_NOT_EQUAL(LHS, RHS, ...)      TTS_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)
#define TTS_LESS(LHS, RHS, ...)           TTS_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)
#define TTS_GREATER(LHS, RHS, ...)        TTS_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)
#define TTS_LESS_EQUAL(LHS, RHS, ...)     TTS_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)
#define TTS_GREATER_EQUAL(LHS, RHS, ...)  TTS_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)
#define TTS_CONSTEXPR_RELATION(A, B, OP, T, F)                                                      \
{                                                                                                   \
  static_assert ( std::bool_constant<::tts::detail::OP(A,B)>::value                                 \
                , "[TTS] - ** FAILURE** : " TTS_STRING(A) " " T " " TTS_STRING(B) " is false."      \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}                                                                                                   \

#define TTS_CONSTEXPR_EQUAL(LHS, RHS)          TTS_CONSTEXPR_RELATION(LHS,RHS, eq , "==" , "!=")
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS)      TTS_CONSTEXPR_RELATION(LHS,RHS, neq, "!=" , "==")
#define TTS_CONSTEXPR_LESS(LHS, RHS)           TTS_CONSTEXPR_RELATION(LHS,RHS, lt , "<"  , ">=")
#define TTS_CONSTEXPR_GREATER(LHS, RHS)        TTS_CONSTEXPR_RELATION(LHS,RHS, gt , ">"  , "<=")
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS)     TTS_CONSTEXPR_RELATION(LHS,RHS, le , "<=" , ">" )
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS)  TTS_CONSTEXPR_RELATION(LHS,RHS, ge , ">=" , "<=")
#define TTS_TYPE_IS(TYPE, REF)                                                                      \
{                                                                                                   \
  static_assert ( std::is_same_v<TTS_REMOVE_PARENS(TYPE),TTS_REMOVE_PARENS(REF)>                    \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(TYPE))                     \
                  " is not the same as " TTS_STRING(TTS_REMOVE_PARENS(REF)) "."                     \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}
#define TTS_EXPR_IS(EXPR, TYPE)                                                                     \
{                                                                                                   \
  static_assert ( std::is_same_v<decltype(TTS_REMOVE_PARENS(EXPR)),TTS_REMOVE_PARENS(TYPE)>         \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(EXPR))                     \
                  " does not evaluates to an instance of " TTS_STRING(TTS_REMOVE_PARENS(TYPE)) "."  \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}
#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                    \
[&]()                                                                                               \
{                                                                                                   \
  bool tts_caught = false;                                                                          \
                                                                                                    \
  try                 { EXPR; }                                                                     \
  catch(EXCEPTION&  ) { tts_caught = true; }                                                        \
  catch(...)          { }                                                                           \
                                                                                                    \
  if(tts_caught)                                                                                    \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(EXPR) << " failed to throw " << TTS_STRING(EXCEPTION) );   \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()
#define TTS_THROW(EXPR, EXCEPTION, ...)     TTS_THROW_ ## __VA_ARGS__ ( EXPR, EXCEPTION )
#define TTS_THROW_(EXPR, EXCEPTION)         TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FATAL)
#define TTS_NO_THROW_IMPL(EXPR,FAILURE)                                                             \
[&]()                                                                                               \
{                                                                                                   \
  bool tts_caught = false;                                                                          \
                                                                                                    \
  try        { EXPR; }                                                                              \
  catch(...) { tts_caught = true; }                                                                 \
                                                                                                    \
  if(!tts_caught)                                                                                   \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(EXPR) << " throws unexpectedly." );                       \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()
#define TTS_NO_THROW(EXPR, ...)     TTS_NO_THROW_ ## __VA_ARGS__ ( EXPR )
#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR,TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR,TTS_FATAL)
#include <bit>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>
#if !defined(__cpp_lib_bit_cast)
# include <cstring>
#endif
namespace tts::detail
{
#if !defined(__cpp_lib_bit_cast)
  template <class To, class From>
  To bit_cast(const From& src) noexcept requires(sizeof(To) == sizeof(From))
  {
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
  }
#else
  using std::bit_cast;
#endif
  inline auto as_int(float a)   noexcept  { return bit_cast<std::int32_t>(a); }
  inline auto as_int(double a)  noexcept  { return bit_cast<std::int64_t>(a); }
  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto ia = as_int(a);
    using r_t = std::remove_cvref_t<decltype(ia)>;
    constexpr auto Signmask = r_t(1) << (sizeof(r_t)*8-1);
    return std::signbit(a) ?  Signmask-ia : ia;
  }
}
#include <type_traits>
#include <limits>
#include <cmath>
namespace tts
{
  template<typename T, typename U> inline double absolute_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (std::isnan(a) && std::isnan(b))) return 0.;
        if(std::isinf(a) || std::isinf(b) || std::isnan(a) || std::isnan(b))
          return std::numeric_limits<double>::infinity();
        return std::abs(a - b);
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absolute_distance(d0, d1);
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absolute_distance(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
  template<typename T, typename U> inline double relative_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      { return a == b ? 0. : 100.; }
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (std::isnan(a) && std::isnan(b))) return 0.;
        if(std::isinf(a) || std::isinf(b) || std::isnan(a) || std::isnan(b))
          return std::numeric_limits<double>::infinity();
        return 100. * (std::abs(a - b) / std::max(T(1), std::max(std::abs(a), std::abs(b))));
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return relative_distance(d0, d1);
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return relative_distance(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
  template<typename T, typename U> inline double ulp_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>)
      {
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      }
      else if constexpr(std::is_floating_point_v<T>)
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;
        if((a == b) || (std::isnan(a) && std::isnan(b)))
        {
          return 0.;
        }
        else if (std::isunordered(a, b))
        {
          return std::numeric_limits<double>::infinity();
        }
        else
        {
          auto aa = detail::bitinteger(a);
          auto bb = detail::bitinteger(b);
          if(aa > bb) std::swap(aa, bb);
          auto z = static_cast<ui_t>(bb-aa);
          if( std::signbit(a) ^ std::signbit(b) ) ++z;
          return z/2.;
        }
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        using u_t = typename std::make_unsigned<T>::type;
        return ((a < b) ? u_t(b - a) : u_t(a - b))/2.;
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulp_distance(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
}
#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, FAILURE)                                        \
[&](auto&& lhs, auto&& rhs)                                                                         \
{                                                                                                   \
  auto eval_a = (lhs);                                                                              \
  auto eval_b = (rhs);                                                                              \
  auto r      = FUNC (eval_a,eval_b);                                                               \
  auto& fmt_n = N<1000 ? std::defaultfloat : std::scientific;                                       \
  auto& fmt_r = r<1000 ? std::defaultfloat : std::scientific;                                       \
                                                                                                    \
  if(r <= N)                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)                          \
                            << " but "                                                              \
                            << ::tts::as_string(eval_a) << " == " << ::tts::as_string(eval_b)       \
                            << " within " << std::setprecision(2) << fmt_r                          \
                            << r << std::defaultfloat                                               \
                            << " " << UNIT << " when "                                              \
                            << std::setprecision(2) << fmt_n                                        \
                            << N << std::defaultfloat                                               \
                            << " " << UNIT << " was expected."                                      \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(LHS,RHS)                                                                                          \

#define TTS_PRECISION(L,R,N,U,F, ...)     TTS_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F)
#define TTS_PRECISION_(L,R,N,U,F)         TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L,R,N,U,F) TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FATAL)
#define TTS_ABSOLUTE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"unit", ::tts::absolute_distance, __VA_ARGS__ )
#define TTS_RELATIVE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"%"   , ::tts::relative_distance, __VA_ARGS__ )
#define TTS_ULP_EQUAL(L,R,N,...)      TTS_PRECISION(L,R,N,"ULP" , ::tts::ulp_distance     , __VA_ARGS__ )
#define TTS_IEEE_EQUAL(L,R,...)       TTS_ULP_EQUAL(L, R, 0, __VA_ARGS__ )
#include <type_traits>
namespace tts::detail
{
  template<typename T, typename U> struct failure
  {
    std::size_t index;
    T original;
    U other;
  };
}
#define TTS_ALL_IMPL(SEQ1,SEQ2,OP,N,UNIT,FAILURE)                                                   \
[](auto const& a, auto const& b)                                                                    \
{                                                                                                   \
  if( std::size(b) != std::size(a) )                                                                \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but sizes does not match: "                                        \
                            << "size(" TTS_STRING(SEQ1) ") = " << std::size(a)                      \
                            << " while size(" TTS_STRING(SEQ2) ") = " << std::size(b)               \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
                                                                                                    \
  auto ba = std::begin(a);                                                                          \
  auto bb = std::begin(b);                                                                          \
  auto ea = std::end(a);                                                                            \
                                                                                                    \
  std::vector < ::tts::detail::failure< std::remove_cvref_t<decltype(*ba)>                          \
                                      , std::remove_cvref_t<decltype(*bb)>                          \
                                      >                                                             \
              > failures;                                                                           \
  std::size_t i = 0;                                                                                \
                                                                                                    \
  while(ba != ea)                                                                                   \
  {                                                                                                 \
    if( OP(*ba,*bb) > N )  failures.push_back({i++,*ba,*bb});                                       \
    ba++;                                                                                           \
    bb++;                                                                                           \
  }                                                                                                 \
                                                                                                    \
  if( !failures.empty( ) )                                                                          \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but values differ from more than " << N << " "<< UNIT              \
            );                                                                                      \
                                                                                                    \
    for(auto f : failures)                                                                          \
      std::cout << "    @[" << f.index << "] : " << f.original << " and " << f.other                \
                << " differ by " << OP(f.original,f.other) << " " << UNIT << "\n";                  \
                                                                                                    \
    std::cout << "\n";                                                                              \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
                                                                                                    \
  ::tts::global_runtime.pass();                                                                     \
  return ::tts::logger{false};                                                                      \
}(SEQ1, SEQ2)                                                                                       \

#define TTS_ALL(L,R,F,N,U, ...)     TTS_ALL_ ## __VA_ARGS__ (L,R,F,N,U)
#define TTS_ALL_(L,R,F,N,U)         TTS_ALL_IMPL(L,R,F,N,U,TTS_FAIL)
#define TTS_ALL_REQUIRED(L,R,F,N,U) TTS_ALL_IMPL(L,R,F,N,U,TTS_FATAL)
#define TTS_ALL_ABSOLUTE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::absolute_distance,N,"unit", __VA_ARGS__ )
#define TTS_ALL_RELATIVE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::relative_distance,N,"%"   , __VA_ARGS__ )
#define TTS_ALL_ULP_EQUAL(L,R,N,...)      TTS_ALL(L,R, ::tts::ulp_distance     ,N,"ULP" , __VA_ARGS__ )
#define TTS_ALL_IEEE_EQUAL(S1,S2,...)     TTS_ALL_ULP_EQUAL(S1,S2,0, __VA_ARGS__)
#define TTS_ALL_EQUAL(L,R,...)            TTS_ALL_ABSOLUTE_EQUAL(L,R, 0 __VA_ARGS__ )
