//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once
#include <tts/tools/output.hpp>
#include <tts/tools/preprocessor.hpp>

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE

namespace tts::_
{
  struct option
  {
    option() = default;

    // Parses a raw CLI argument, e.g. "--capture=report.txt" or "--verbose".
    explicit option(char const* arg)
        : token(arg)
        , position(-1)
    {
      assert(arg && "Token cannot be null");
      auto it  = strchr(arg, '=');
      position = it ? static_cast<int>(it - token) : static_cast<int>(strlen(token)); // NOSONAR
    }

    // Wraps an already-split name/value pair (e.g. a flag and the environment variable value
    // standing in for it) - no '='-parsing needed, name and value are independent pointers.
    option(char const* name, char const* value)
        : token(name)
        , position(static_cast<int>(strlen(name))) // NOSONAR
        , env_value(value)
    {
      assert(name && "Name cannot be null");
      assert(value && "Value cannot be null");
    }

    bool has_flag(char const* f) const
    {
      assert(f && "Flag cannot be null");
      if(position == -1) return false;

      int len = static_cast<int>(strlen(f)); // NOSONAR
      if(len > position) return false;

      return strncmp(token, f, static_cast<size_t>(position)) == 0;
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
        char const* raw = env_value ? env_value : token + position + 1;
        int         n   = 0;
        if constexpr(std::integral<T>)
        {
          decltype(sizeof(void*)) v;
          n    = sscanf(raw, "%zu", &v);
          that = static_cast<T>(v);
        }
        else if constexpr(std::floating_point<T>)
        {
          double v;
          n    = sscanf(raw, "%lf", &v);
          that = static_cast<T>(v);
        }
        else
        {
          n    = 1;
          that = T {raw};
        }

        if(n != 1) that = def;
      }
      else
      {
        that = def;
      }

      return that;
    }

    char const* token     = "";
    int         position  = -1;
    char const* env_value = nullptr;
  };

  // Derives "TTS_<NAME>" from a long-form flag like "--shard" (-> "TTS_SHARD") or
  // "--allow-empty" (-> "TTS_ALLOW_EMPTY"). Returns an empty text for short flags (single
  // dash, e.g. "-v") since those don't map to a sensible environment variable name.
  inline text env_var_name(char const* flag)
  {
    if(!flag || flag[ 0 ] != '-' || flag[ 1 ] != '-' || flag[ 2 ] == '\0') return text {};

    char        buffer[ 64 ] = "TTS_";
    std::size_t pos          = 4;
    for(char const* p = flag + 2; *p && pos < sizeof(buffer) - 1; ++p)
    {
      char c = *p;
      if(c == '-') c = '_';
      else if(c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
      buffer[ pos++ ] = c;
    }
    buffer[ pos ] = '\0';

    return text {buffer};
  }
}

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-config Configuration Utilities
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @ingroup tools-config
    @public
    @brief Command line arguments lightweight processor

    @see option

    @groupheader{Example}

    @code
    #define TTS_MAIN  // No need for main()
    #include <tts/tts.hpp>

    TTS_CASE( "CLI test - flag" )
    {
      // Will fail if the test is not called via
      //  ./mytest.exe --pass--the-test or ./mytest.exe -W
      TTS_EXPECT( (tts::arguments()("--pass--the-test","-W")) );

      // Will fail if the test is not called via
      //  ./mytest.exe --at_last
      TTS_EXPECT( tts::arguments()["--at_last"] );
    };

    TTS_CASE( "CLI test - setting" )
    {
      // Will fail if the test is not called via ./mytest.exe --xxx=13.37
      TTS_EQUAL( tts::arguments().value<double>("--xxx"), 13.37);

      // Will fail if the test is not called via ./mytest.exe --www=9 or ./mytest.exe -zzz=9
      TTS_EQUAL( (tts::arguments().value(0,"--www","-zzz")), 9);
    };
    @endcode
  **/
  //====================================================================================================================
  struct options
  {
    /// Checks if the flag `f` is set on the command line
    bool operator[](char const* f) const
    {
      return find(f).is_valid();
    }

    /// Checks if qny flag `fs` is set on the command line
    template<std::same_as<char const*>... Flags> bool operator()(Flags... fs) const
    {
      return find(fs...).is_valid();
    }

    /// Returns a value of type `T` if a flag matches any of the strings in `fs` or `T{}` otherwise
    template<typename T, std::same_as<char const*>... Flags> T value(Flags... fs) const
    {
      T that = {};
      if(auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    /// Returns a value of type `T` if a flag matches any of the strings in `fs` or `that` otherwise
    template<typename T, std::same_as<char const*>... Flags> T value(T that, Flags... fs) const
    {
      if(auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    /// Checks if current options set is not empty
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

      // No matching CLI flag: fall back to a TTS_<FLAG_NAME> environment variable, if set.
      // CLI always takes precedence over the environment.
      for(auto f: flags)
      {
        auto name = _::env_var_name(f);
        if(name.is_empty()) continue;

        if(char const* value = getenv(name.data())) return _::option {f, value};
      }

      return _::option {};
    }
  };

  namespace _
  {
    inline options current_arguments = {0, nullptr};
    inline int     current_seed      = -1;
  }

  //====================================================================================================================
  /**
    @brief Initializes a TTS test suite
    @ingroup customization-points

    This function is to be called when writing a custom entry point.

    @param argc Number of command line argument fetched from `main`
    @param argv Command line arguments pointer fetched from `main`

    @see TTS_CUSTOM_DRIVER_FUNCTION
  **/
  //====================================================================================================================
  inline void initialize(int argc, char const** argv)
  {
    if(!_::current_arguments.is_valid()) _::current_arguments = options {argc, argv};
  }
}

namespace tts
{
  //====================================================================================================================
  /**
      @ingroup tools-config
      @public
      @brief Retrieve the current list of command line argument

      @see options
      @return The pre-processed command line arguments as a reference to a tts::options instance.
  **/
  //====================================================================================================================
  inline options const& arguments()
  {
    return _::current_arguments;
  }

  //====================================================================================================================
  /**
      @ingroup tools-random
      @public
      @brief Initialize the random seed for tests

      Initializes and retrieves the random seed used by TTS for random number generation. If the
      seed has not been initialized yet, it uses the value provided as argument. If that value is
  -1, it uses the current time as seed. Once initialized, calling this function will return the same
  seed each time.

      @groupheader{Example}
      @snippet doc/random_seed.cpp snippet

      @param base_seed  Seed to use for random number generations.
      @return The current random seed used by TTS.
  **/
  //====================================================================================================================
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

  //====================================================================================================================
  /**
    @ingroup tools-config
    @public
    @brief Check if verbose mode is enabled

    @return `true` if verbose mode is enabled, `false` otherwise.
            Verbose mode is enabled when the `--verbose` or `-v` command line argument is provided.
  **/
  //====================================================================================================================
  inline bool is_verbose()
  {
    return _::current_verbosity.verbose;
  }

  //====================================================================================================================
  /**
    @ingroup tools-config
    @public
    @brief Check if quiet mode is enabled

    @return `true` if quiet mode is enabled, `false` otherwise.
            Quiet mode is enabled when the `--quiet` or `-q` command line argument is provided.
  **/
  //====================================================================================================================
  inline bool is_quiet()
  {
    return _::current_verbosity.quiet;
  }

  //====================================================================================================================
  /**
    @ingroup tools-config
    @public
    @brief Check if verbose-only details should be reported

    @return `true` if verbose mode is enabled and quiet mode is not, `false` otherwise. This is the
            condition under which extra detail lines - like passing test confirmations or type
            hints - get reported: quiet mode always takes precedence over verbose mode.
  **/
  //====================================================================================================================
  inline bool is_detailed()
  {
    return is_verbose() && !is_quiet();
  }
}

TTS_DISABLE_WARNING_POP
