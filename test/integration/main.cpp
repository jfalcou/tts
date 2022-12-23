#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check expectations" )
{
  TTS_EXPECT(true == true);
  TTS_EXPECT_NOT(1+1 == 3);
};

TTS_CASE( "Check relationship between values" )
{
  double x = 12.5;
  TTS_EQUAL( 12.5f, x );
  TTS_NOT_EQUAL( 17.65, x );
  TTS_LESS(1.95f, x);
  TTS_GREATER(2*x, x);
  TTS_LESS_EQUAL(x,13);
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
