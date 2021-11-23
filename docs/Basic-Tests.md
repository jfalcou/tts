The following component provides macros to perform basic tests that simply evaluate as a boolean value or to force the status of a given test to a failed state. 

To use those macros, include the `tts/tts.hpp` file.

# TTS_EXPECT

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT(Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
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

# TTS_EXPECT_NOT

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_EXPECT_NOT(Expression, ...)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
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

# TTS_FAIL

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_FAIL(Message)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
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

# TTS_INVALID

## Synopsis:
**Required header:** `#include <tts/tts.hpp>`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_INVALID((Message)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Definition
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
