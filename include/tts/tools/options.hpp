//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

namespace tts::_
{
  struct option
  {
    option() = default;
    option( const char* arg ) : token(arg), position(-1)
    {
      auto it = strchr(arg,'=');
      position = static_cast<int>(it ? (it - token) : strlen(token));
    }

    bool has_flag(const char* f) const
    {
      if(position == -1)      return false;

      int len = static_cast<int>(strlen(f));
      if(len > position)  return false;

      return strncmp(token,f,position) == 0;
    }

    bool is_valid() const  { return position > 0;  }

    template<typename T> T get(T const& def = T{}) const
    {
      T that = {};

      if(is_valid())
      {
        int n = 0;
        if constexpr(std::integral<T>)
        {
          decltype(sizeof(void*)) v;
          n = sscanf(token+position+1, "%zu", &v);
          that = static_cast<T>(v);
        }
        else if constexpr(std::floating_point<T>)
        {
          double v;
          n = sscanf(token+position+1, "%lf", &v);
          that = static_cast<T>(v);
        }
        else
        {
          n     = 1;
          that  = T{token+position+1};
        }

        if(n!=1) that = def;
      }
      else
      {
        that = def;
      }

      return that;
    }

    const char* token    = "";
    int         position = -1;
  };
}

namespace tts
{
  //====================================================================================================================
  /*!
      @brief Command line arguments lightweight processor

      @see option

      @groupheader{Example}

      @code
      #define TTS_MAIN
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
    bool operator[](const char* f) const
    {
      return find(f).is_valid();
    }

    /// Checks if qny flag `fs` is set on the command line
    template<std::same_as<const char*>... Flags>
    bool operator()(Flags... fs) const
    {
      return find(fs...).is_valid();
    }

    /// Returns a value of type `T` if a flag matches any of the strings in `fs` or `T{}` otherwise
    template<typename T, std::same_as<const char*>... Flags>
    T value(Flags... fs) const
    {
      T that = {};
      if( auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    /// Returns a value of type `T` if a flag matches any of the strings in `fs` or `that` otherwise
    template<typename T, std::same_as<const char*>... Flags>
    T value(T that, Flags... fs) const
    {
      if( auto o = find(fs...); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    /// Checks if current options set is not empty
    bool is_valid() { return argc && argv != nullptr; }

    int           argc;
    char const**  argv;

    private:
    template<std::same_as<const char*>... Flags> _::option find(Flags... fs) const
    {
      const char* flags[] = {fs...};

      for(int i=1;i<argc;++i)
      {
        _::option o(argv[i]);
        for(auto f : flags)
        {
          if( o.has_flag(f) ) return o;
        }
      }

      return _::option{};
    }
  };
}

namespace tts
{
  namespace _
  {
    inline options current_arguments = {0,nullptr};
    inline int     current_seed      = -1;
    inline bool    is_verbose        = false;
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
    if(!_::current_arguments.is_valid()) _::current_arguments = options{argc,argv};
  }


  //====================================================================================================================
  /*! @brief Retrieve the current list of command line argument

      @see options
      @return The pre-processed command line arguments as a reference to a tts::options instance.
  **/
  //====================================================================================================================
  inline options const& arguments() { return _::current_arguments; }

  //====================================================================================================================
  /*! @brief Initialize the random seed for tests

      @param base_seed  Seed to use for random number generations. If not provided, the seed provided by the `--seed`
                        command line argument will be used. If none is provided, the seed will be computed from the
                        current elapsed time.
  **/
  //====================================================================================================================
  inline int random_seed(int base_seed = -1)
  {
    if(_::current_seed == -1)
    {
      auto s = arguments().value( base_seed, "--seed" );
      if(s == -1 ) s = static_cast<int>(time(0));
      _::current_seed = s;
    }

    return _::current_seed;
  }
}
