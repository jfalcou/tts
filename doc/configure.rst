Test suite Management
==========================

Command Line and Environment Variable
-------------------------------------

TTS provides options that can be provided either by the command line or by setting up
environment variables to control some aspects of the test behavior.

Command Line option       | Environment Variable | Value  | Description
:------------------------ | :------------------- | :------| :-----------------------------------
`--random`                | | non-zero unsigned integer | Randomize tests order using a given seed

You can pass other arbitrary options on the command line by using the format `--name value`.
Those options are then accessible through the `args` function.

Custom Driver
-------------

TODO
