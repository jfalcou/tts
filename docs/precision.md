# Precision Tests

The following component provides macros to perform precision tests between floating-point values.

Precision comparisons are, by default, handled by the `tts::ulp_distance`, `tts::relative_distance`
or `tts::absolute_distance` overloads.

To use those macros, include the `tts/tts.hpp` file.

## TTS_ABSOLUTE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ABSOLUTE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if the absolute distance between `LHS` and `RHS` is less or equal to `X`.
This comparison is performed by using the proper `tts::absolute_distance` overload
[customizable per the following protocol](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-absolute-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `X`:  Maximal absolute distance acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE("Absolute distance")
{
  TTS_ABSOLUTE_EQUAL(-2.  , 2.f  , 4.);
  TTS_ABSOLUTE_EQUAL('A'  , 80LL , 15, REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_RELATIVE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_RELATIVE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if two values are within a given percentage. This comparison is performed by using
the proper `tts::relative_distance` overload
[customizable per the following protocol](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-relative-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `X`:  Maximal percentage acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE("Relative distance")
{
  TTS_RELATIVE_EQUAL(42.f , 42.f  , 0     );
  TTS_RELATIVE_EQUAL('A'  , 80LL  , 18.75 );
  TTS_RELATIVE_EQUAL(1.   , 2.f   , 100.  );

  TTS_RELATIVE_EQUAL(1    , 10    , 900., REQUIRED  );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_ULP_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ULP_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if two values are within a given [ULP distance](https://github.com/jfalcou/tts/wiki/Rationale#precision-testing).
This comparison is performed by using the proper `tts::ulp_distance` overload
[customizable per the following protocol](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ulp-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `X`:  Maximal ULPs acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "ULP distance")
{
  float a{1};
  float eps  = std::numeric_limits<float>::epsilon();
  float qnan = std::numeric_limits<float>::quiet_NaN();
  float inf  = std::numeric_limits<float>::infinity();
  float minf = -inf;

  TTS_ULP_EQUAL(a    , a     , 0.  );

  TTS_ULP_EQUAL(a    , qnan  , inf );
  TTS_ULP_EQUAL(qnan , qnan  , 0.  );

  TTS_ULP_EQUAL(a    , inf   , inf );
  TTS_ULP_EQUAL(inf  , inf   , inf );

  TTS_ULP_EQUAL(a, a-eps   , 1   );
  TTS_ULP_EQUAL(a, a+eps   , 0.5 );
  TTS_ULP_EQUAL(a, a+3*eps , 1.5, REQUIRED );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## TTS_IEEE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_IEEE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if two values are exactly within a 0 [ULP](https://github.com/jfalcou/tts/wiki/Rationale#precision-testing)
from each other. This also allow for infinites and NaNs to be compared equal if both values are the
same infinites or are both $NaN$. This comparison is performed by using the proper `tts::ulp_distance` overload
[customizable per the following protocol](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ulp-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "ULP distance")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  TTS_IEEE_EQUAL(x,x);

  TTS_IEEE_EQUAL(1.f, 1.f);
  TTS_IEEE_EQUAL(2. , 2. );
  TTS_IEEE_EQUAL(65 , 'A');
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
