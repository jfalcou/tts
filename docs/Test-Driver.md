By default, **TTS** provides an entry point function for the listed tests. However, it may be
required to handle such an entry point. In this case, one can define the `TTS_CUSTOM_DRIVER_FUNCTION`
preprocessor symbol to a name of their own entry-point function as shown below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION custom_entry_point
#include <tts/tts.hpp>

TTS_CASE( "Tautological test" )
{
  TTS_EXPECT_NOT(false == true);
};

int main(int argc, char const** argv)
{
  custom_entry_point(argc, argv);
  return tts::report(0,0);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As usual, at most one translation unit mush use `#define TTS_MAIN`.

After defining the `TTS_CUSTOM_DRIVER_FUNCTION` symbol, tests can be added as usual.
Then, a regular `main` function is to be defined. This function will then performs any
special operations required then calls the aforementioned entry point function.

Finally, the `main` function will call `tts::report`.

`tts::report` is in charge of aggregating test results and validate the whole tests
with respect to expect number of failures and invalid tests.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace tts
{
  int report(std::ptrdiff_t fails, std::ptrdiff_t invalids);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
</script>

**Parameters**:
  + `fails` : Number of expected failures.
  + `invalids` : Number of expected invalid tests.

**Returns:** `0` is the tests all passed as expecte, `1` otherwise.
