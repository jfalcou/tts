//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <chrono>
#include <cmath>
#include <initializer_list>
#include <sstream>
#include <string>

namespace tts
{
  namespace detail
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
  }

  //====================================================================================================================
  /*!
      @brief Command line arguments lightweight processor

      @see arguments

      @groupheader{Example}

      @code
      #define TTS_MAIN
      #include <tts/tts.hpp>

      TTS_CASE( "CLI test - flag" )
      {
        // Will fail if the test is not called via
        //  ./mytest.exe --pass--the-test or ./mytest.exe -W
        TTS_EXPECT( (tts::arguments()[{"--pass--the-test","-W"}]) );

        // Will fail if the test is not called via
        //  ./mytest.exe --at_last
        TTS_EXPECT( tts::arguments()["--at_last"] );
      };

      TTS_CASE( "CLI test - setting" )
      {
        // Will fail if the test is not called via ./mytest.exe --xxx=13.37
        TTS_EQUAL( tts::arguments().value("--xxx", 0.), 13.37);

        // Will fail if the test is not called via ./mytest.exe --www=9 or ./mytest.exe -zzz=9
        TTS_EQUAL( (tts::arguments().value({"--www","-zzz"}, 0)), 9);
      };
      @endcode
  **/
  //====================================================================================================================
  struct options
  {
    using params_t = std::initializer_list<const char*>;

    /// Checks if any flag from with the strings `fs` is set on the command line
    bool operator[](params_t    fs) const { return find(fs).is_valid(); }

    /// Checks if the flag `f` is set on the command line
    bool operator[](const char* f ) const { return operator[]({f}); }

    /// Returns a value of type `T` if a flag matches any of the strings in `fs` or `that` otherwise
    template<typename T> T value(params_t fs, T that = {}) const
    {
      if( auto o = find(fs); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    /// Returns a value of type `T` if a flag matches the string `f` or `that` otherwise
    template<typename T> T value(const char* f, T that = {}) const
    {
      return value({f},that);
    }

    /// Checks if current options set is not empty
    bool is_valid() { return argc && argv != nullptr; }

    int argc;
    char const** argv;

    private:

    detail::option find(const char* f ) const { return find({f}); }
    detail::option find(params_t    fs) const
    {
      for(int i=1;i<argc;++i)
      {
        detail::option o(argv[i]);
        for(auto f : fs)
          if( o.flag() == f ) return o;
      }

      return detail::option{};
    }
  };

  namespace detail
  {
    inline ::tts::options current_arguments = {0,nullptr};
    inline std::int32_t   current_seed      = -1;
  }

  //====================================================================================================================
  /*!
    @brief Initializes a TTS test suite

    This function is to be called when writing a custom entry point.

    @param argc Number of command line argument fetched from `main`
    @param argv Command line arguments pointer fetched from `main`
  **/
  //====================================================================================================================
  inline void initialize(int argc, const char** argv)
  {
    if(!detail::current_arguments.is_valid())
      detail::current_arguments = ::tts::options{argc,argv};
  }

  //====================================================================================================================
  /*! @brief Retrieve the current list of command line argument

      @see options
      @return The pre-processed command line arguments as a reference to a tts::options instance.
  **/
  //====================================================================================================================
  inline ::tts::options const& arguments()
  {
    return detail::current_arguments;
  }

  //====================================================================================================================
  /*! @brief Initialize the random seed for tests

      @param base_seed  Seed to use for random number generations. If not provided, the seed provided by the `--seed`
                        command lien argument will be used. If none is provided, the seed will be computed from the
                        current elapsed time.
  **/
  //====================================================================================================================
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
