
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

  In the case a given type needs to be displayed in a specific manner, specialize
  @ref tts::display for it and **TTS** will use it when necessary. The `to_text` overload this
  replaces is gone: one left behind is reported where it would have been used.

  @snippet doc/custom-display.cpp snippet1

  If needed, one can delegate a part of this string construction to the **TTS** internal
  string conversion function @ref tts::as_text that will use all runtime options for display.
  @ref tts::text can also be constructed from a formatting specification and other similar
  setup.

  @snippet doc/custom-display.cpp snippet2

  Beware that, in this situation, command-line arguments controlling value display like `-x`
  or `-s` will not be applied to the formatted string.

  @section  customize-comparison Equality and Ordering
  All equality-based checks in **TTS** use the compared value `operator==`. If needed, specialize
  @ref tts::comparison to let **TTS** use a special comparison scheme. The trait takes both operand
  types, in the order they are written, and the second one defaults to the first, so a homogeneous
  comparison names a single type.

  @snippet doc/comparators.cpp snippet1

  Similarly, **TTS** uses `operator<` to build all its ordering-based checks. The `less` member of
  the same trait covers those. Inherit from `tts::_::builtin_comparison` to keep the member left
  alone. The `compare_equal` and `compare_less` overloads these replace are gone: one left behind
  is reported where it would have been used.

  @snippet doc/comparators.cpp snippet2

  @section customize-precision Precision Measurement

  ## ULP Distance
  When dealing with floating point values, **TTS** measures all [ULP checks](rationale.html#ulp)
  through the `ulp` member of @ref tts::precision. Specialize the trait to let **TTS** use a special
  ULP comparison scheme, inheriting from `tts::_::builtin_precision` to keep the three members left
  alone. One can also reuse the pre-existing `tts::ulp_check` to implement their own.

  @snippet doc/precision_ulp.cpp snippet

  ## IEEE Comparison
  IEEE comparison consists in checking for exact equality while considering all NaN/Invalid values
  of floating point values. It is the `ieee` member of @ref tts::precision. One can also reuse the
  pre-existing `tts::ieee_check` to implement their own.

  @snippet doc/precision_ieee.cpp snippet

  ## Relative Comparison
  Relative precision checks within **TTS** are done through the `relative` member of
  @ref tts::precision, which reports a ratio rather than a percentage. One can also reuse the
  pre-existing `tts::relative_check` to implement their own.

  @snippet doc/precision_relative.cpp snippet

  ## Absolute Comparison
  **TTS** performs all absolute precision checks through the `absolute` member of
  @ref tts::precision. One can also reuse the pre-existing `tts::absolute_check` to implement their
  own.

  The four free functions `ulp_distance`, `ieee_equal`, `relative_distance` and `absolute_distance`
  are gone. One left behind is reported where it would have been used, rather than ignored in
  silence, so a suite carrying them says so at the first check that needs it.

  @snippet doc/precision_absolute.cpp snippet

  # Data Generator

  [Test cases based on data sets](@ref TTS_CASE_WITH)  and [range checks](@ref TTS_ULP_RANGE_CHECK)
  require one or more data generators to perform. If the pre-existing data generators are not suitable,
  define your own by providing a `constexpr` callable object with the following signature:

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

  ## Producing values of a user-defined type

  A generator answers for the type it is handed, so a type **TTS** knows nothing about needs to say
  how it is built. Specialize @ref tts::generation for it, inheriting from
  `tts::_::builtin_generation` to keep the default way of building, which hands a scalar straight to
  the generator and fills a sequence element by element.

  Generator bounds go through @ref tts::conversion, which casts a plain value to the type under
  test. Specialize it when a bound is a recipe rather than a value, so that a bound written once in
  a case answers for each type the case runs on.

  `produce` and `convert_as` are the dispatchers rather than customization points, and **TTS**
  reaches them qualified, so an overload of either name in another namespace is never found. They
  carry a generic default, which is why a leftover cannot be reported the way the other traits
  report theirs.

  A specialization of @ref tts::generation keys on the type being built and cannot see the generator
  in an overload set. When one has to be treated apart, @ref tts::is_randoms names the range
  generator by its type.

**/
//==================================================================================================
