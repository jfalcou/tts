**TTS provides** a macros to test and compare functions over a set of data. It also provides
classes to use as a base for new dataset generations.

To use those components, include the `tts/ranges.hpp` file.

# TTS_ULP_RANGE_CHECK

## Synopsis:
**Required header:** `#include <tts/ranges.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Evaluates the histogram of ULP difference between two functions run on the same data set and that
they lie in a given [ULP distance](https://github.com/jfalcou/tts/wiki/Rationale#precision-testing).
This comparison is performed by using the proper `tts::ulp_distance` [overload](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ulp-comparison).).

**Parameters:**
  + `Producer`: [Data set generator](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-data-generator) to use.
  + `RefType` : Type to use as reference function input.
  + `NewType` : Type to use as challenger function input.
  + `RefFunc` : Reference function to compare to.
  + `NewFunc` : Challenger function to be compared to the reference one.
  + `Ulpmax`  : Maximal ULPs acceptable for passing the test.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/ranges.hpp>

float ok_x (float x) { return x; }
float bad_x(float x) { return x + x*1e-7f; }

TTS_CASE( "Test range check" )
{
  TTS_ULP_RANGE_CHECK ( [] (auto i, auto c) { return (100.f*i)/c; }
                      , float, float , ok_x, bad_x
                      , 2.
                      );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# tts::prng_generator

## Synopsis:
**Required header:** `#include <tts/ranges.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace tts
{
  template<typename T, typename Distribution>
  struct prng_generator
  {
    template<typename... Args> prng_generator(Args... args);
    void init( options const& args );
    template<typename Idx, typename Count> T operator()(Idx, Count);
  };
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
`tts::prng_generator` wraps any standard [random distribution](https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution) to be used as Data Generator by TTS_ULP_RANGE_CHECK.

**Template Parameters:**
  + `T`: Type of values to generate.
  + `Distribution`: A [random distribution](https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution) type.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/ranges.hpp>
#include <random>

float ok_x (float x) { return x; }
float bad_x(float x) { return x + x*1e-7f; }

TTS_CASE( "Test range check" )
{
  TTS_ULP_RANGE_CHECK ( tts::prng_generator<float, std::normal_distribution<float>>(10.f,2.f)
                      , float, float , ok_x, bad_x
                      , 2.
                      );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Constructor

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
template<typename... Args> prng_generator(Args... args);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Construct an instance of `tts::prng_generator` with parameters for the internal random distribution.

**Parameters:**
  + `args`: Arguments to be forwarded to the internal `Distribution`.

## Member functions

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void init( options const& args );
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Initialize the current instance of `tts::prng_generator`.

**Parameters:**
  + `args`: Command-line option to read from. `tts::prng_generator` will use the CLI to retrieve
    the random generator seed.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
template<typename Idx, typename Count> T operator()(Idx index, Count count);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Return a the `Idx` value out of `Count`.

**Parameters:**
  + `index`:
  + `count`:

# tts::uniform_prng_generator

## Synopsis:
**Required header:** `#include <tts/ranges.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace tts
{
  template<typename T, typename Distribution>
  struct prng_generator
  {
    template<typename... Args> prng_generator(Args... args);
    void init( options const& args );
    template<typename Idx, typename Count> T operator()(Idx, Count);
  };
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
`tts::uniform_prng_generator` is a convenience type alias that wraps standard uniform distribution
as a Data Generator.

**Template Parameters:**
  + `T`: Type of values to generate.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/ranges.hpp>

float ok_x (float x) { return x; }
float bad_x(float x) { return x + x*1e-7f; }

TTS_CASE( "Test range check" )
{
  TTS_ULP_RANGE_CHECK ( tts::uniform_prng<float>(-100.f,+100.f)
                      , float, float , ok_x, bad_x
                      , 2.
                      );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
