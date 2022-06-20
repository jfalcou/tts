# Type Tests

The following component provides macros to perform tests over expression type equivalence or properties.
All those macros generate tests that fails **at  compile-time**.

To use those macros, include the `tts/tts.hpp` file.

## TTS_TYPE_IS

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_TYPE_IS(Type, Target, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if two types satisfy `std::is_same_v<Type,Target> == true`.

**Parameters:**
  + `Type`: Type to compare.
  + `Target`: Expected type.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>
#include <type_traits>

TTS_CASE( "Check that types can be tested for equality" )
{
  TTS_TYPE_IS( std::add_pointer<float const>::type, float const* );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_EXPR_IS

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPR_IS(Expression, Type, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if an `Expression` evaluates to a value of a given `Type`.

**Parameters:**
  + `Expression`: Expression to evaluate.
  + `Type`: Expected type.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expression types can be tested for equality" )
{
  double d;

  TTS_EXPR_IS( &d + 5        , double*   );
  TTS_EXPR_IS( std::move(d)  , double&&  );
  TTS_EXPR_IS( std::swap(d,d), void      );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_EXPECT_COMPILES

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT_COMPILES(Symbols..., Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if an `Expression` based on a list of `Symbols` will compile properly in a SFINAE
context.

**Parameters:**
  + `Expression`: Brace-enclosed Expression to evaluate.
  + `Symbols`: Variadic lists of symbols used in the expression.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expression can compile properly" )
{
  double d, e;

  TTS_EXPECT_COMPILES(d, e, { d += 4. * e; } );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_EXPECT_NOT_COMPILES

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT_NOT_COMPILES(Symbols..., Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if an `Expression` based on a list of `Symbols` will not compile properly in a SFINAE
context.

**Parameters:**
  + `Expression`: Brace-enclosed Expression to evaluate.
  + `Symbols`: Variadic lists of symbols used in the expression.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that expression can compile properly" )
{
  double d, e;

  TTS_EXPECT_NOT_COMPILES(d, e, { d.foo(e); } );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
