# Scenario Definition
The following component provides macros to define test cases as a simple function or as a template
function. Those are the basic blocks of writing tests with TTS. To use those macros, include the `tts/tts.hpp` file.

## TTS_CASE

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CASE(Description)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
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

## TTS_CASE_TPL

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CASE_TPL(Description, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
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

## TTS_CASE_WITH

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_CASE_WITH(Description, Types, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Introduces a template test case providing dynamically generated data to the test code.

The following code block will contain tests parametrized by a template type of your choice passed
as lambda function parameters and generated for each type in the types list.

Such types list can be provided as:
  + an instance of the [`tts::types`]() template class
  + an instance of a Type Generator, ie a type exposing a public `types_list` type definition

Data are passed using the `tts::generate` function to aggregate data generator.

As for [TTS_CASE](#tts_case), test cases performing no actual tests will be reported as invalid.

**Parameters:**
  + `Description`: A literal string describing the scenario intents.
  + `Types`: Lists of types to generate the test case from.
  + `...`: Generator description to produce the data used by the test.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_WHEN, TTS_AND_THEN

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_WHEN(Description)
#define TTS_AND_THEN(Description)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition

`TTS_WHEN` starts a block containing a sub-test. Code in a sub-test can contain:
  + Normal expressions
  + Sub-tests block introduced by `TTS_AND_THEN`

Compared to regular local scope, whenever a sub-test is run, the data defined in the enclosing
`TTS_WHEN` are re-initialized, thus serving as a setup/tear-down system.

**Parameters:**
  + `Description`: A literal string describing the scenario intents.

### Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check test with sub-test" )
{
  TTS_WHEN("We start some sub-test")
  {
    int i = 99;

    TTS_AND_THEN("We increment a variable")
    {
      TTS_EQUAL(i,99);
      i++;
      TTS_EQUAL(i,100);
    }

    TTS_AND_THEN("We decrement a variable")
    {
      // At the start of this sub-test, i is equal to 99 again
      TTS_EQUAL(i,99);
      i--;
      TTS_EQUAL(i,98);
    }
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
