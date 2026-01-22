#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  \mainpage The Tiny Test System

  The **TTS** (Tiny Test System) library is a C++ open-source Unit Test library designed following
  the ideas of libraries like CATCH or LEST.

  In addition to classical TDD features, its main goal is to provide:

  +  Test over data sets
  +  Support for template test cases generation
  +  IEEE precision-related tests: ULP, absolute and relative error
  +  Customization points for 3rd party types
  +  Streamlined behavior for interaction with post-processing scripts

  Those features make **TTS** suitable for numerical-heavy testing.

  # Supported Compilers
  +  g++ 11.1 and superior
  +  clang 13 and superior
  +  Visual Studio 17 2022 v19.30.30709.0
  + clang-CL 15.0.1 or superior
  +  emscripten 3.1.14

  # A Short Example

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>
  #include <stdexcept>

  TTS_CASE( "Check expectations" )
  {
    TTS_EXPECT(true == true);
    TTS_EXPECT_NOT(1+1 == 3);
  };

  TTS_CASE( "Check relationship between values" )
  {
    double x = 12.34;
    TTS_EQUAL( 12.34, x );
    TTS_NOT_EQUAL( 17.65, x );
    TTS_LESS(1.95f, x);
    TTS_GREATER(2*x, x);
    TTS_LESS_EQUAL(x,12.35);
    TTS_GREATER_EQUAL(x,3.5f);
  };

  void foo(bool x)  { if(x) throw std::runtime_error{"THIS IS AN ERROR"}; }

  TTS_CASE( "Check runtime exceptions" )
  {
    TTS_THROW(foo(true), std::runtime_error);
    TTS_NO_THROW(foo(false));
  };

  TTS_CASE( "Check precision tests" )
  {
    double x = 1.;

    TTS_ABSOLUTE_EQUAL(x ,1.001, 1e-3 );
    TTS_RELATIVE_EQUAL(1,1.1,10);
    TTS_ULP_EQUAL(1. + 1e-16, x, 0.5 );
    TTS_IEEE_EQUAL(1., x );
  };

  TTS_CASE( "Check types and expressions" )
  {
    double d , e;
    TTS_EXPR_IS( d + 5          , double);
    TTS_EXPR_IS( std::swap(d,e) , void  );

    TTS_TYPE_IS( std::add_pointer<float const>::type, float const* );

    TTS_EXPECT_COMPILES( d, e, { d += e; } );
    TTS_EXPECT_NOT_COMPILES(e, { e.foo();  } );
  };
  @endcode
**/
//==================================================================================================
