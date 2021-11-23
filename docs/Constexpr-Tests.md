The following component provides macros to perform basic and relational tests in a `constexpr` context.
All those macros generate tests that fails **at  compile-time**. Ordering and equality comparisons are,
by default, handled by the `operator==` or `operator<` overloads. If one needs to perform a custom
comparison, see the [custom comparisons](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-equality-comparison) section.

To use those macros, include the `tts/tts.hpp` file.


# TTS_CONSTEXPR_EXPECT

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_EXPECT(Expression)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Checks at compile-time if a given constexpr expression evaluates to `true`.

**Parameters:**
  + `Expression`:  Expression to validate.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expectation can be met" )
{
  constexpr int a = 42, b = 69;

  TTS_CONSTEXPR_EXPECT(a != b);
  TTS_CONSTEXPR_EXPECT(a <  b);
  TTS_CONSTEXPR_EXPECT(a <= b);
  TTS_CONSTEXPR_EXPECT(b >  a);
  TTS_CONSTEXPR_EXPECT(b >= a);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_EXPECT_NOT

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
TTS_CONSTEXPR_EXPECT_NOT c++
#define TTS_CONSTEXPR_EXPECT(Expression)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Checks at compile-time if a given constexpr expression evaluates to `false`.

**Parameters:**
  + `Expression`:  Expression to validate.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that counter-expectation can be met" )
{
  constexpr int a = 42, b = 69;

  TTS_CONSTEXPR_EXPECT_NOT(a == b);
  TTS_CONSTEXPR_EXPECT_NOT(a >  b);
  TTS_CONSTEXPR_EXPECT_NOT(a >= b);
  TTS_CONSTEXPR_EXPECT_NOT(b <  a);
  TTS_CONSTEXPR_EXPECT_NOT(b <= a);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_EQUAL

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs equality comparison between two constexpr expressions.
This comparison is performed at compile-time by using the proper `operator==` overload or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-equality-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of constexpr equality tests" )
{
  constexpr float a = 45.f;
  constexpr int   b = 45;

  TTS_CONSTEXPR_EQUAL(a, b);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_NOT_EQUAL

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_NOT_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs inequality comparison between two constexpr expressions.
This comparison is performed at compile-time by using the proper `operator==` overload or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-equality-comparison).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of constexpr equality tests" )
{
  constexpr float a = 4.5f;
  constexpr int   b = 45;

  TTS_CONSTEXPR_NOT_EQUAL(a, b);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_LESS

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_LESS(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs less-than comparison between two constexpr expressions.
This comparison is performed at compile-time by usingthe proper `operator<` overload or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_CONSTEXPR_LESS(42LL, 69.f);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_GREATER

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_GREATER(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs greater-than comparison between two constexpr expressions.
This comparison is performed at compile-time by usingthe proper `operator<` overload or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of greater-than comparison tests" )
{
  TTS_CONSTEXPR_GREATER(69LL, 42.f);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_LESS_EQUAL

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_LESS_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs less-than-or-equal comparison between two constexpr expressions.
This comparison is performed at compile-time by usingthe proper `operator<` and `operator==` overloads or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of less-equal comparison tests" )
{
  TTS_CONSTEXPR_LESS_EQUAL(42LL, 69.f);
  TTS_CONSTEXPR_LESS_EQUAL('A' , 65. );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TTS_CONSTEXPR_GREATER_EQUAL

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
Performs greater-than-or-equal comparison between two constexpr expressions.
This comparison is performed at compile-time by usingthe proper `operator<` and `operator==` overloads or by a
[custom comparison](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-ordering).

**Parameters:**
  + `LHS`, `RHS`:  Expressions to compare.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of greater-equal comparison tests" )
{
  TTS_CONSTEXPR_GREATER_EQUAL(69LL, 42.f);
  TTS_CONSTEXPR_GREATER_EQUAL(69., 69);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
