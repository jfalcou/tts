# Reference Documentation

To use those macros, include the `tts/tts.hpp` file.

## Scenario Definition
The following component provides macros to define test cases as a simple function or as a template
function. Those are the basic blocks of writing tests with TTS.

### TTS_CASE

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CASE(Description)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Introduces a new test scenario and registers it into the current test driver.

The following code block will contain user-defined code for a given test case. Test cases performing no actual tests will be reported as invalid.

**Parameters:**
  - `Description`: A literal string describing the scenario intents.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check basic arithmetic" )
{
  TTS_EQUAL(2+2, 4);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### TTS_CASE_TPL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CASE_TPL(Description, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Introduces a template test case.

The following code block will contain tests parametrized by a template type of your choice passed as lambda function parameters of the tempalte type `tts::type` and instantiated for each type in the types list.

Such types list can be provided as:
  + a variadic list of types separated by commas
  + an instance of the [`tts::types`]() template class
  + an instance of a Type Generator, ie a type exposing a public `types_list` type definition

As for [TTS_CASE](#tts_case), test cases performing no actual tests will be reported as invalid.

**Parameters:**
  + `Description`: A literal string describing the scenario intents.
  + `...`: Lists of types to generate the test case from.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

TTS_CASE_TPL( "Check types using variadic list", char, short, int, double, void* )
<typename T>( tts::type<T> )
{
  TTS_GREATER_EQUAL(sizeof(T), 1);
};

TTS_CASE_TPL( "Check types using a types list", tts::types<float,double> )
<typename T>( tts::type<T> )
{
  TTS_EQUAL(1/T{2}, T{0.5});
};

// A Types Generator is any type exposing a types_list internal type
// In this example we use such a type to generate the list of types:
//
// tts::types<std::array<std::byte,1>,...,std::array<std::byte,N>>>;

template<int N, typename Indexes = std::make_index_sequence<N>>  struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::array<std::byte,I+1>...>;
};

TTS_CASE_TPL( "Check types using a types list generator", sizes<5> )
<typename T>( tts::type<T> )
{
  T x;
  TTS_EQUAL(sizeof(x), x.size());
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Basic Tests
The following component provides macros to perform basic tests that simply evaluate as a boolean value or to force the status of a given test to a failed state.

### TTS_EXPECT

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT(Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Checks if a given expression evaluates to `true`.

**Parameters:**
  + `Expression`: Expression to evaluate and compare to `true`.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT(a <  b);
  TTS_EXPECT(b >  a);
  TTS_EXPECT(a != b);

  // This test will cause the program to stop and not run the remaining tests
  TTS_EXPECT(a == b, REQUIRED);

  TTS_EXPECT(a <= b);
  TTS_EXPECT(b >= a);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### TTS_EXPECT_NOT

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT_NOT(Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Checks if a given expression evaluates to `true`.

**Parameters:**
  + `Expression`: Expression to evaluate and compare to `true`.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(b < a);

  // This test will cause the program to stop and not run the remaining tests
  TTS_EXPECT_NOT(a != b, REQUIRED);

  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(a <= b);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### TTS_FAIL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_FAIL(Message)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Register a test that always fails and displays a custom `Message`.

**Parameters:**
  + `Message`: A literal string to display as additional informations

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that forced pass passes" )
{
  TTS_FAIL("Forced fail");
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### TTS_INVALID

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_INVALID((Message)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
Register a test that is always considered invalid and displays a custom `Message`.

**Parameters:**
  + `Message`: A literal string to display as additional informations

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that forced pass passes" )
{
  TTS_INVALUD("Forced invalidation of a test");
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Constexpr Tests

The following component provides macros to perform basic and relational tests in a `constexpr` context.
All those macros generate tests that fails **at  compile-time**. Ordering and equality comparisons are,
by default, handled by the `operator==` or `operator<` overloads. If one needs to perform a custom
comparison, see the [custom comparisons](https://github.com/jfalcou/tts/wiki/Customization-Points#custom-equality-comparison) section.


### TTS_CONSTEXPR_EXPECT

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_EXPECT(Expression)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_EXPECT_NOT

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
TTS_CONSTEXPR_EXPECT_NOT c++
#define TTS_CONSTEXPR_EXPECT(Expression)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_EQUAL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_NOT_EQUAL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_NOT_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_LESS

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_LESS(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_GREATER

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_GREATER(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_LESS_EQUAL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_LESS_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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

### TTS_CONSTEXPR_GREATER_EQUAL

#### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS,RHS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Definition
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
