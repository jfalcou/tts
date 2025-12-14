#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  customize Customizing TTS Behaviour

  @tableofcontents

  @section customize-driver Tests Driver
  By default, **TTS** provides an entry point function for the listed tests. However, it may be
  required to handle such an entry point. In this case, one can define the @ref TTS_CUSTOM_DRIVER_FUNCTION
  preprocessor symbol to a name of their own entry-point function as shown below.

  After defining the @ref TTS_CUSTOM_DRIVER_FUNCTION symbol, tests can be added as usual.
  Then, a regular `main` function is to be defined. This function will then performs any
  special operations required then calls the afforementioned entry point function. Finally,
  the `main` function will call @ref tts::report which will aggregate test results and validate the whole tests
  with respect to expect number of failures and invalid tests.

  @snippet doc/entry_point.cpp snippet

  @section  customize-display Data display
  By default, whenever **TTS** needs to display a value in a report, it uses `std::to_string` or, in
  the case of sequence-like types, a sequence of calls to `std::to_string`. In case no overload
  for `std::to_string` exists for a given type, a string will be built from the type name and its byte sequence.

  @snippet doc/display-unknown.cpp snippet

  In the case a given type needs to be displayed in a specific manner, **TTS** allows to overload the
  `to_text` in the type's namespace or as a friend function and will use it when necessary.

  @snippet doc/custom-display.cpp snippet1

  If needed, one can delegates a part of this string construction to the **TTS** internal string
  conversion function @ref tts::as_text that will use all runtime options for display. @ref tts::text
  can also be constructed from a formatting specification and other similar setup.

  @snippet doc/custom-display.cpp snippet2

  Beware that, in this situation, command-line argument controlling value display like `-x` or `-s` will not be applied
  to the formatted string.

  @section  customize-comparison Equality and Ordering
  All equality-based checks in **TTS** uses the compared value `operator==`. If needed, one can
  specialize the `compare_equal` function in a type's namespace to let **TTS** use a special comparison
  scheme.

  @code
  namespace sample
  {
    template<typename T> struct box { T value; };

    template<typename T>
    bool compare_equal(box<T> const& l, box<T> const& r)
    {
      return l.value == r.value;
    }
  };
  @endcode

  Similarly, **TTS** uses `operator<` to build all its ordering-based checks. If needed, one can
  specialize the `compare_less` function in a type's namespace to let **TTS** use special ordering
  scheme.

  @code
  namespace sample
  {
    template<typename T> struct box { T value; };

    template<typename T>
    bool compare_less(box<T> const& l, box<T> const& r)
    {
      return l.value < r.value;
    }
  };
  @endcode

  # Precision Measurement
  When dealing with floating point values, **TTS** uses its `ulp_distance` function to perform all ULP checks.
  If needed, one can specialize this function in the `tts` namespace to let **TTS** use special ULP comparison scheme.
  As usual, one can also reuse the pre-existing `tts::ulp_distance` to implement their own.

  @code
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
  @endcode

  ## Relative Comparison
  Relative precision checks within **TTS** are doen through the `relative_distance` function. If needed,
  one can specialize this function in the `tts` namespace to let **TTS** use special relative precision
  scheme. As usual, one can also reuse the pre-existing `tts::relative_distance` to implement their own.

  @code
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
  @endcode

  ## Absolute Comparison
  **TTS** uses its `absolute_distance` function to perform all absolute precision checks. If needed,
  one can specialize this function in the `tts` namespace to let **TTS** use special absolute precision
  scheme. As usual, one can also reuse the pre-existing `tts::absolute_distance` to implement their own.

  @code
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
  @endcode

  # Data Generator

  Range checks require a data generator to fill their tests. Outside the provided PRNG generators, one
  can build their own generator.

  A range generator is a Callable object providing the following interface.

  @code
  auto operator()(auto index, auto count);
  @endcode

  **Parameters**:
    + `index` : an integral value representing the index of the generated value.
    + `count` : an integral value representing the total number of values to be generated.

  **Returns:** A value of any type.

  By this definition, any lambda function with the proper interface is a suitable data generator.
  This makes defining local generator trivial as it doesn't requires an actual callable object
  to be defined.

  **Examples:**

  This code defines a generator that will generate `double` between `0` and `value`, each generation returning the
  `i`th portion of the full value.

  @snippet doc/cli_generator.cpp snippet

  Run with `./my_test.exe --gen-value=99`, it produces the following output:

  @code
  TEST: 'Test stateless range check'
  Comparing: f<float> with g<float> using cli_generator(99)
  Max ULP         Count (#)       Ratio Sum (%)   Samples
  --------------------------------------------------------------------------------
  0.0             1               0.0244141       Input:      0
                                                  Found:      0
                                                  instead of: 0
  --------------------------------------------------------------------------------
  0.5             3534            86.3037         Input:      0.024169921875
                                                  Found:      0.02416992373764515
                                                  instead of: 0.024169921875
  --------------------------------------------------------------------------------
  1.0             561             100             Input:      0.120849609375
                                                  Found:      0.1208496242761612
                                                  instead of: 0.120849609375
  --------------------------------------------------------------------------------
  TEST: 'Test stateless range check' - [PASSED]
  ----------------------------------------------------------------
  Results: 1 test - 1/1 (100.00%) success - 0/0 (0.00%) failure - 0/0 (0.00%) invalid
  @endcode
**/
//==================================================================================================
