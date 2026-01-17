#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  cli Command Line Interface

  @tableofcontents

  # Display Options

  The following options modify how the tests are run and the results displayed.

  Options         | Shortcut | Description
  --------------- | -------- | ---------------------------------------------------------------
  `--help`        | `-h`     | Display the list of supported options `./my_test --help`
  `--hex`         | `-x`     | Display floating point values as hexadecimal. `./my_test --hex`
  `--scientific`  | `-s`     | Display floating point values as hexadecimal. `./my_test --scientific`
  `--verbose`     | `-v`     | Display tests results regardless of their status. `./my_test --verbose`
  `--quiet`       | `-q`     | Remove all tests results regardless of their status. `./my_test --quiet`

  # Tests Parameters
  These options are provided to control the specifics of the tests parameters. Those
  options require a value and have no short form.

  Options          | Description
  ---------------- | -----------------------------------------------------------------------
  `--precision=N`  | Set the precision for displaying floating point values `./my_test --precision=12`
  `--seed=N`       | Set the PRNG seeds (default is time-based) `./my_test --valmin=-100`

  # Range Parameters
  This set of options controls the specifics of the range check tests. Those
  options require a value and have no short form.

  Options      | Description
  ------------ | -----------------------------------------------------------------------
  `--block=N`  | Set size of range checks samples to `N`. `./my_test --block=4096`
  `--loop=N`   | Repeat each range checks `N` times. `./my_test --loop=100`
  `--ulpmax=N` | Set global failure ulp threshold for range tests to `N`. `./my_test --ulpmax=4`
  `--valmin=N` | Set minimal value for range tests to `N`. `./my_test --valmin=-100`
  `--valmax=N` | Set maximal value for range tests to `N`. `./my_test --valmax=1000`

**/
//==================================================================================================
