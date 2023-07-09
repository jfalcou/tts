## TTS_ULP_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ULP_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition



**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `X`:  Maximal  acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## TTS_IEEE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_IEEE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if two values are exactly within a 0 [ULP](rationale.html#ulp)
from each other. This also allow for infinites and NaNs to be compared equal if both values are the
same infinites or are both $NaN$. This comparison is performed by using the proper `tts::ulp_distance` overload
[customizable per the following protocol](customisation.html#ulp).

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
