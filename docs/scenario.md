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
