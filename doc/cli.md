Command Line Interface {#cli}
======================

TTS supports options that can be passed as command line arguments of the test binary. Those
options modify how the tests are run and the results displayed.

## Existing Commands

 Options        | Shortcut | Description
--------------- | -------- | ---------------------------------------------------------------
`--help`        | `-h`     | Display the list of supported options `./my_test --help`
`--hex`         | `-x`     | Display floating point values as hexadecimal. `./my_test --hex`
`--scientific`  | `-s`     | Display floating point values as hexadecimal. `./my_test --scientific`

Another set of options is provided to control the specifics of the tests parameters. Those
options require a value and have no short form.

 Options         | Description
---------------- | -----------------------------------------------------------------------
`--precision=N`  | Set the precision for displaying floating point values `./my_test --precision=12`
`--seed=N`       | Set the PRNG seeds (default is time-based) `./my_test --valmin=-100`

Another set of options is provided to control the specifics of the range check tests. Those
options require a value and have no short form.

 Options     | Description
------------ | -----------------------------------------------------------------------
`--block=N`  | Set size of range checks samples to `N`. `./my_test --block=4096`
`--loop=N`   | Repeat each range checks `N` times. `./my_test --loop=100`
`--ulpmax=N` | Set global failure ulp threshold for range tests to `N`. `./my_test --ulpmax=4`
`--valmin=N` | Set minimal value for range tests to `N`. `./my_test --valmin=-100`
`--valmax=N` | Set maximal value for range tests to `N`. `./my_test --valmax=1000`

## Exploiting Command Line Arguments
Values of command-line options can be used directly in a test via the `tts::arguments()` function.
Command line arguments are detected and parsed on-demand, which means that one can define a
test-specific command-line argument by just retrieving its value from one or multiple expected
arguments string.

### `tts::options`
`tts::arguments()` returns an instance of the `tts::options` type that provides the
following members functions:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace tts
{
  struct options
  {
    bool operator[](std::initializer_list<const char*> flags ) const;
    bool operator[](const char*  flag ) const;
    template<typename T> T value(std::initializer_list<const char*> flags, T def = {}) const
    template<typename T> T value(const char* flags, T def = {}) const
  };
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### options::operator[]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
bool operator[](std::initializer_list<const char*> flags ) const;
bool operator[](const char*  flag ) const;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Parameters**:
  - `flags` : string or list of strings containing the flag(s) to check for

**Returns:** `true` if any of the flag passed as an argument is set, `false` otherwise.

#### options::value

**Parameters**:
<div class="smallskip"></div>

  - `def`   : default value if no flag are set
  - `flags` : string containing the flag to check for

**Returns:** the value associated to the flag if any flag is set, `def` otherwise.

#### options::value

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
struct options
{
  template<typename T> T value(std::initializer_list<const char*> flags, T def = {}) const
  template<typename T> T value(const char* flags, T def = {}) const
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Parameters**:
<div class="smallskip"></div>

  - `def`   : default value if no flag are set
  - `flags` : string containing the flag to check for

**Returns:** the value associated to the flag if any flag is set, `def` otherwise.

### Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "CLI test - flag" )
{
  // Will fail if the test is not called via
  //  ./mytest.exe --pass--the-test or ./mytest.exe -W
  TTS_EXPECT( tts::arguments[{"--pass--the-test","-W"}] );

  // Will fail if the test is not called via
  //  ./mytest.exe --at_last
  TTS_EXPECT( tts::arguments["--at_last"] );
};

TTS_CASE( "CLI test - setting" )
{
  // Will fail if the test is not called via ./mytest.exe --xxx=13.37
  TTS_EQUAL( tts::arguments.value("--xxx", 0.), 13.37);

  // Will fail if the test is not called via ./mytest.exe --www=9 or ./mytest.exe -zzz=9
  TTS_EQUAL( tts::arguments.value({"--www""-zzz", 0), 9);
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
