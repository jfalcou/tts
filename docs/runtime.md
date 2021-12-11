# Exception Tests

The following component provides macros to perform basic tests on the throwing or
absence of runtime exceptions.

To use those macros, include the `tts/tts.hpp` file.

## TTS_THROW

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_THROW(Expression, Exception, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if a given `Expression` throws an exception of type `Exception`.

**Parameters:**
  + `Expression`: Expression to evaluate.
  + `Exception`: Expected exception type to be thrown.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

struct some_exception_type {};

void foo() { throw some_exception_type(); }

TTS_CASE( "Check that we can capture thrown exceptions" )
{
  TTS_THROW( foo(), some_exception_type );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## TTS_NO_THROW

### Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_NO_THROW(Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Definition
Checks if a given `Expression` throws no exception.

**Parameters:**
  + `Expression`: Expression to evaluate.
  + `...`: Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

**Example:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

void bar() {}

TTS_CASE( "Check that nothrow function are detected as such" )
{
  TTS_NO_THROW( bar() );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
