# Customization Point

## Tests Driver
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

## Custom Display
By default, whenever **TTS** needs to display a value in a report, it uses `std::to_string` or, in
the case of sequence-like types, a sequence of calls to `std::to_string`. In case no overload
for `std::to_string` exists for a given type, a string will be built from the type name and its
instance's address.

In the case a given type needs to be displayed in a specific manner, **TTS** allows to overload the
`to_string` in the type's namespace and will use it if it is present.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  struct ratio { int n,d; };

  std::string to_string(ratio const& b) { return std::to_string(n) + "/" + std::to_string(d); }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If needed, one can delegates a part of this string construction to the **TTS** internal string

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  template<typename T> struct box { T value; };

  template<typename T>
  std::string to_string(box<T> const& b) { return "box<" + tts::as_string(b.value) + ">"; }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom Equality Comparison
<a name="equality">

**TTS** uses `operator==` to build all its equality-based checks. If needed, one can specialize a
`compare_equal` function in a type's namespace to let **TTS** use special comparison scheme.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  template<typename T> struct box { T value; };

  template<typename T>
  bool compare_equal(box<T> const& l, box<T> const& r)
  {
    return l.value == r.value;
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom Ordering
**TTS** uses `operator<` to build all its ordering-based checks. If needed, one can specialize a
`compare_less` function in a type's namespace to let **TTS** use special comparison scheme.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  template<typename T> struct box { T value; };

  template<typename T>
  bool compare_less(box<T> const& l, box<T> const& r)
  {
    return l.value < r.value;
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom ULP Comparison
**TTS** uses its `ulp_distance` function to perform all ULP checks. If needed, one can specialize
this function in the `tts` namespace to let **TTS** use special ULP comparison scheme. As usual,
one can also reuse the pre-existing `tts::ulp_distance` to implement their own.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  struct ratio { int n,d; };
}

namespace tts
{
  double ulp_distance(sample::ratio const &a, sample::ratio const &b)
  {
    auto ra = static_cast<double>(a.n) / a.d;
    auto rb = static_cast<double>(b.n) / b.d;

    return tts::ulp_distance(ra,rb);
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom Relative Comparison
**TTS** uses its `relative_distance` function to perform all relative precision checks. If needed,
one can specialize this function in the `tts` namespace to let **TTS** use special relative precision
 scheme. As usual, one can also reuse the pre-existing `tts::relative_distance` to implement their own.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  struct ratio { int n,d; };
}

namespace tts
{
  double relative_distance(sample::ratio const &a, sample::ratio const &b)
  {
    return tts::relative_distance( a.n*b.d , b.n*a.d );
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom Absolute Comparison
**TTS** uses its `absolute_distance` function to perform all absolute precision checks. If needed,
one can specialize this function in the `tts` namespace to let **TTS** use special absolute precision
 scheme. As usual, one can also reuse the pre-existing `tts::absolute_distance` to implement their own.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
namespace sample
{
  struct ratio { int n,d; };
}

namespace tts
{
  double relative_distance(sample::ratio const &a, sample::ratio const &b)
  {
    return std::abs( a.n*b.d - b.n*a.d );
  }
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Custom Data Generator

Range checks require a data generator to fill their tests. Outside the provided PRNG generators, one
can build their own generator.

A range generator is a Callable object providing the following interface.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
auto operator()(auto index, auto count);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Parameters**:
  + `index` : an integral value representing the index of the generated value.
  + `count` : an integral value representing the total number of values to be generated.

**Returns:** A value of any type.

By this definition, any lambda function with the proper interface is a suitable data generator.
This makes defining local generator trivial as it doesn't requires an actual callable object
to be defined.

A data generator can optionally provide a `init` member function that will allow the generator to
prepare some internal states and, potentially use the command line arguments to do so. This
member function has the following interface.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void init( tts::options const& args );
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Parameters**:
  + `args` : an instance of [**`tts::options`**](#options) containing current command line arguments.

**Examples:**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
struct cli_generator
{
  void init( options const& args )
  {
    value_ = args.value_or(1., "--gen-value");
  }

  template<typename Idx, typename Count> double operator()(Idx i, Count c)
  {
    return (value_ * i)/c;
  }

  private:
  double value_;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This generator will generate `double` between `0` and `value`, each generation returning the `i`th
portion of the full value. It can then be used this way:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
TTS_CASE( "Test stateless range check" )
{
  TTS_ULP_RANGE_CHECK ( cli_generator{}, double, double, f, g , 1. );
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
