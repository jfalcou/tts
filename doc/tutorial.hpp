#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  tts101  Getting Started

  @tableofcontents

  Unit Tests are usually organised into **scenarios** that test the specific behaviour of a software component.
  Such scenarios are usually implemented in separate executables. Each scenario is decomposed into **test suites**,
  i.e., a group of related tests verifying that the software component correctly services the current scenario.
  Test suites are themselves composed of at least one test case.

  With **TTS**, this decomposition (scenario/suite/test) is supported by various macros.

  Let's dive into the details of how **TTS** manages and reports tests.

  @section tuto-test-case Specify a Test Case

  Test cases are functions performing repeatable, minimal operations able to unambiguously capture a function or class
  behaviour and assess its correctness. In **TTS**, a test case can be defined either as:

  + a single function defined by the @ref TTS_CASE macro. In this case, the test function will be run and proceed
  to every test it contains.

  + a family of functions generated from a template function and a sequence of types via the @ref TTS_CASE_TPL macro.
  Inside the test case itself, the template type is accessible through the template parameter `T`.

  In both cases, the test case is defined with a unique string description. Additionally, good testing practices
  recommend using fine-grained test cases to simplify test management.

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>

  TTS_CASE( "Make a simple test suite" )
  {
    // This test case is empty and thus will be reported as invalid
  };
  @endcode

  Notice the `#define TTS_MAIN` line. This is used to notify **TTS** that the current translation unit will contain
  the **TTS** main entry point. You can use **TTS** in multiple translation units, but only one of them must have the
  @ref TTS_MAIN macro enabled.

  The unit test reports the total number of tests performed, the number of passing tests, the number of failing tests
  and the number of invalid tests. In this case, unsurprisingly, our empty test is reported as invalid, as we consider
  an empty test case as erroneous in itself. **TTS** enforces an **Empty Suite Is An Error** rule that applies at the
  test case level: **any test case with no actual testing will be considered as a failure and reported as an invalid test**.

  @section tuto-macro Testing Macros

  Most of the **TTS** components are available as macros that perform usual test operations and report their successes
  or failures to the test suite manager directly from within a test case. Let's amend our initial test by adding a single
  call to @ref TTS_EXPECT :

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>

  TTS_CASE( "Make a simple test suite" )
  {
    TTS_EXPECT( true );
  };
  @endcode

  One may notice that no other information is displayed. This is the default behaviour.
  You can now use [the various **TTS** testing macros](usergroup2.html) to design your own unit tests.
**/
//==================================================================================================
