# Sequence Tests

The following component provides macros to perform equality precision tests between
sequences of values.

Those comparisons are, by default, handled by the `tts::ulp_distance`, `tts::relative_distance`
or `tts::absolute_distance` overloads.

To use those macros, include the `tts/tts.hpp` file.

## TTS_ALL_ABSOLUTE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ALL_ABSOLUTE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if the absolute distance between all elements of `LHS` and `RHS` is less or equal to `X`
and that the sizes of both sequences are equal.

This comparison is performed by using the proper `tts::absolute_distance` overload
[customizable per the following protocol](customisation.html#absolute).

**Parameters:**
  + `LHS`, `RHS`:  Sequences to compare.
  + `X`:  Maximal absolute distance acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE("Absolute distance")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.f,2.f,3.f,-5.f};

  TTS_ALL_ABSOLUTE_EQUAL(v,w,0);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_ALL_RELATIVE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ALL_RELATIVE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if all elements of two sequences are within a given percentage and that the sizes of both
sequences are equal. This comparison is performed by using the proper `tts::relative_distance` overload
[customizable per the following protocol](customisation.html#relative).

**Parameters:**
  + `LHS`, `RHS`:  Sequences to compare.
  + `X`:  Maximal percentage acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE("Relative distance")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.f,2.f,3.1f,-5.f};

  TTS_ALL_RELATIVE_EQUAL(v,w,3.33);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_ALL_ULP_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ALL_ULP_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if all elements of two sequences are within a given [ULP distance](rationale.html#ulp) and
that the sizes of both sequences are equal.

This comparison is performed by using the proper `tts::ulp_distance` overload
[customizable per the following protocol](customisation.html#ulp).

**Parameters:**
  + `LHS`, `RHS`:  Sequences to compare.
  + `X`:  Maximal ULPs acceptable for passing the test
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE( "ULP distance")
{
  std::vector v{1.f       , 2.f, 3.f      , -5.f};
  std::list   w{1.f+1e-7f , 2.f, 3.f      , -5.f};

  TTS_ALL_ULP_EQUAL(v,w,0.5);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_ALL_IEEE_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_ALL_IEEE_EQUAL(LHS,RHS,X, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

Checks if all elements of two sequences are exactly within 0 [ULP](rationale.html#ulp) from
each other and that the sizes of both sequences are equal. This also allow for infinites and NaNs
to be compared equal if both values are the same infinites or are both $NaN$.

This comparison is performed by using the proper `tts::ulp_distance` overload
[customizable per the following protocol](customisation.html#ulp).

**Parameters:**
  + `LHS`, `RHS`:  Sequences to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE( "ULP distance")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  TTS_ALL_IEEE_EQUAL(x,x);

  TTS_ALL_IEEE_EQUAL(1.f, 1.f);
  TTS_ALL_IEEE_EQUAL(2. , 2. );
  TTS_ALL_IEEE_EQUAL(65 , 'A');
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
