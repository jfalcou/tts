
#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  customize Customizing TTS Behaviour

  @tableofcontents

  @section customize-driver Tests Driver
  By default, **TTS** provides an entry point function for the listed tests. However, it may be
  required to handle such an entry point. In this case, one can define the
  @ref TTS_CUSTOM_DRIVER_FUNCTION preprocessor symbol to a name of their own entry-point
  function as shown below.

  After defining the @ref TTS_CUSTOM_DRIVER_FUNCTION symbol, tests can be added as usual.
  Then, a regular `main` function is to be defined. This function will then perform any
  special operations required, then call the specified entry point function. Finally,
  the `main` function will call @ref tts::report which will aggregate test results and
  validate the whole tests with respect to expected numbers of failures and invalid tests.

  @snippet doc/entry_point.cpp snippet

  @section  customize-display Data display
  By default, whenever **TTS** needs to display a value in a report, it uses `std::to_string`
  or, in the case of sequence-like types, a sequence of calls to `std::to_string`. In case no
  overload for `std::to_string` exists for a given type, a string will be built from the type
  name and its byte sequence.

  @snippet doc/display-unknown.cpp snippet

  In the case a given type needs to be displayed in a specific manner, **TTS** allows to
  overload `to_text` in the type's namespace or as a friend function and will use it when
  necessary.

  @snippet doc/custom-display.cpp snippet1

  If needed, one can delegate a part of this string construction to the **TTS** internal
  string conversion function @ref tts::as_text that will use all runtime options for display.
  @ref tts::text can also be constructed from a formatting specification and other similar
  setup.

  @snippet doc/custom-display.cpp snippet2

  Beware that, in this situation, command-line arguments controlling value display like `-x`
  or `-s` will not be applied to the formatted string.

  @section  customize-comparison Equality and Ordering
  All equality-based checks in **TTS** use the compared value `operator==`. If needed, one can
  specialize the `compare_equal` function in the type's namespace or as a friend function to
  let **TTS** use a special comparison scheme.

  @snippet doc/comparators.cpp snippet1

  Similarly, **TTS** uses `operator<` to build all its ordering-based checks. If needed, one
  can specialize the `compare_less` function in the type's namespace or as a friend function
  to let **TTS** use a special ordering scheme.

  @snippet doc/comparators.cpp snippet2

  # Precision Measurement

  ## ULP Distance
  When dealing with floating point values, **TTS** uses its `ulp_distance` function to perform
  all [ULP checks](rationale.html#ulp). If needed, one can specialize this function in the
  type's namespace or as a friend function to let **TTS** use a special ULP comparison
  scheme. As usual, one can also reuse the pre-existing `tts::ulp_check` to implement their
  own.

  @snippet doc/precision_ulp.cpp snippet

  ## Relative Comparison
  Relative precision checks within **TTS** are done through the `relative_distance` function.
  If needed, one can specialize this function in the type's namespace or as a friend function
  to let **TTS** use a special relative precision scheme. As usual, one can also reuse the
  pre-existing `tts::relative_check` to implement their own.

  @snippet doc/precision_relative.cpp snippet

  ## Absolute Comparison
  **TTS** uses its `absolute_distance` function to perform all absolute precision checks. If
  needed, one can specialize this function in the type's namespace or as a friend function to
  let **TTS** use a special absolute precision scheme. As usual, one can also reuse the
  pre-existing `tts::absolute_check` to implement their own.

  @snippet doc/precision_absolute.cpp snippet

  # Data Generator

  [Test cases based on data sets](@ref TTS_CASE_WITH)  and [range checks](@ref TTS_ULP_RANGE_CHECK) require one or more data generators to perform.
  If the pre-existing data generators do not fit one's needs, one can provide a `constexpr` compatible callable object
  with the following signature:

  @code
  template<typename T>
  T operator()(tts::type<T> target, auto index, auto count);
  @endcode

  where:
    + `target` is an instance of `tts::type<T>` representing the type of value to be generated.
    + `index`  is an integral value representing the index of the generated value.
    + `count`  is an integral value representing the total number of values to be generated.

  For example, the following code defines a generator that will generate values alternating between `-1` and `1`
  every `n` iterations.

  @snippet doc/generator.cpp snippet

**/
//==================================================================================================
