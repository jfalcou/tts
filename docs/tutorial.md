# Getting Started

Unit Tests are usually gathered into **scenarios** in which a specific behavior of a software component is tested. Such scenarios are usually isolated into different executables. Each scenario is decomposed into **test suites**, i.e a group of related tests verifying that the current scenario is correctly serviced by the software component. Test suites are themselves composed of at least one test case.

With **TTS**, this decomposition (scenario/suite/test) is supported by various macro.

Let's dive into the detail and how **TTS** manage and report tests.

## Specify a Test Case

Test cases are functions performing repeatable, minimal operations able to unambiguously capture a function or class behavior and assess its correctness. In **TTS**, a test case can be defined either as:

  * a single function defined by the [`TTS_CASE`](https://github.com/jfalcou/tts/wiki/Scenario-Definition#tts_case) macro.
    In this case, the test function will be run and proceed to every test it contains.

  * a family of functions generated from a template function and a sequence of types
    via the [`TTS_CASE_TPL`](https://github.com/jfalcou/tts/wiki/Scenario-Definition#tts_case_tpl) macro. Inside the test case itself, the template
    type is accessible through the template parameter `T`.

In both cases, the test case is defined with a unique string description. Additionally, good
testing practices recommend having fine grain test cases to simplify test management.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Make a simple test suite" )
{
  // This test case is empty and thus will be reported as invalid
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Notice the `#define TTS_MAIN` line. This is used to notify **TTS** that the current translation unit will contain the **TTS** main entry point. You can use **TTS** in multiple translation units but only one of them must have the `TTS_MAIN` macro enabled.

After compiling and launching the test, you should end up with the following output:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
$ ./my_test
[!] - Make a simple test suite : EMPTY TEST CASE
----------------------------------------------------------------
Results: 1 test - 0/1 success - 0/0 failure - 1/0 invalid
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The unit test reports the total number of tests performed, the number of passing tests, the number of failing tests and the number of invalid tests.

In this case, unsurprisingly, our empty test is reported as invalid as we consider an empty test case as erroneous in itself.
**TTS** enforces a **"Empty Suite Is An Error"** rule that applies at the test case level. Any test case with no actual testing will be considered as a failure and reported as an invalid test.

Every test functions then require at least one call to any Testing Macros.

## Testing Macros

**TTS** provides a set of macros to perform usual tests operations and to report their successes or failures to the test suite manager directly from within a test case. Let's amend our initial test by adding a single call to
[**`TTS_EXPECT`**](https://github.com/jfalcou/tts/wiki/Basic-Tests#tts_expect):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Make a simple test suite" )
{
  TTS_EXPECT( true );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

and run it again:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bash
$ ./my_test
[V] - Make a simple test suite
----------------------------------------------------------------
Results: 1 test - 1/1 success - 0/0 failure - 0/0 invalid
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One may notice that no other information is displayed. This is the default behavior.

You can know use the various **TTS** macro to design your own unit tests.
