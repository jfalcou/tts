# The TTS Library

Tiny Test System is a C++ open-source Unit Test library designed following the ideas of libraries like CATCH or LEST.

In addition to classical TDD features, its main goal is to provide:

- Test over data sets
- Support for template test cases generation
- IEEE precision-related tests: ULP, absolute and relative error
- Customization points for 3rd party types
- Streamlined behavior for interaction with post-processing scripts

TTS is thus suitable for numerical-heavy testing.

# Supported Compilers
  * g++ 10.3 and superior
  * clang 12 and superior
  * Visual Studio 17 2022 v19.30.30709.0
  * emscripten 3.1.14
  * 
# A Short Example

[See it live on Compiler Explorer](https://godbolt.org/z/cM5sxMxjo)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check expectations" )
{
  TTS_EXPECT(true == true);
  TTS_EXPECT_NOT(1+1 == 3);
};

TTS_CASE( "Check relationship between values" )
{
  int x = 12.34;
  TTS_EQUAL( 12.34f, x );
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Licence

This library is licensed under the [MIT License](http://opensource.org/licenses/MIT):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ none
Copyright : TTS Contributors & Maintainers

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
