# Relational Tests

The following component provides macros to perform ordering tests between values. Ordering and equality comparisons are, by default, handled by the `operator==` or `operator<` overloads. If one needs to perform a custom comparison, see the [custom comparisons](customisation.html#equality) section.

To use those macros, include the `tts/tts.hpp` file.

## TTS_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EQUAL(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs equality comparison between two values. This comparison is performed by using
the proper `operator==` overload or by a [custom comparison](customisation.html#equality).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of equality tests" )
{
  TTS_EQUAL(45,45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_EQUAL(a,b);

  // This test will cause the program to stop with a failure
  TTS_EQUAL(12, 21, REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_NOT_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_NOT_EQUAL(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs inequality comparison between two values. This comparison is performed by using
the proper `operator==` overload or by a [custom comparison](customisation.html#equality).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of inequality tests" )
{
  TTS_NOT_EQUAL(42,69.f);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_NOT_EQUAL(a,b);

  // This test will cause the program to stop with a failure
  TTS_NOT_EQUAL('A', 65, REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## TTS_LESS

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_LESS(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs less-than comparison between two values. This comparison is performed by using
the proper `operator<` overload or by a [custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_LESS(42., 69);

  std::vector<float> a{1,2,3}, b{12,3};
  TTS_LESS(a,b);

  // This test will cause the program to stop with a failure
  TTS_LESS(69, 42, REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_GREATER

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_GREATER(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs greater-than comparison between two values. This comparison is performed by using
the proper `operator<` overload or by a [custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_GREATER(69., 42ULL);

  std::vector<float> a{13,5}, b{2,4,6};
  TTS_GREATER(a,b);

  // This test will cause the program to stop with a failure
  TTS_GREATER(1., 10., REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_LESS_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_LESS_EQUAL(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs less-than-or-equal comparison between two values. This comparison is performed by using
the proper `operator<` and `operator==` overloads or by a [custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_LESS_EQUAL(42., 69);
  TTS_LESS_EQUAL(69., 69);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_LESS_EQUAL(a,b);
  TTS_LESS_EQUAL(a,a);

  // This test will cause the program to stop with a failure
  TTS_LESS_EQUAL(10., 1., REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## TTS_GREATER_EQUAL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_GREATER_EQUAL(LHS,RHS,...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Performs greater-than-or-equal comparison between two values. This comparison is performed by using
the proper `operator<` and `operator==` overloads or by a [custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_GREATER_EQUAL(69LL, 42.f);
  TTS_GREATER_EQUAL(69., 69);

  std::vector<float> a{12,3}, b{7,9,11};
  TTS_GREATER_EQUAL(a,b);
  TTS_GREATER_EQUAL(a,a);

  // This test will cause the program to stop with a failure
  TTS_GREATER_EQUAL(1., 10., REQUIRED);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
