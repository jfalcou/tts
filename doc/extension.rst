Customizing TTS
===============

.. _tts-cli:

Command Line Interface
----------------------

TTS supports options that can be passed as command line arguments of the test binary.

+----------------+----------+-----------------------------------------------------------------------+
| Option         | Shortcut | Description                                                           |
+================+==========+=======================================================================+
| ``--no-color`` | ``-n``   | Disable the colored output of test results.                           |
|                |          |   ``./my_test --no-color``                                            |
+----------------+----------+-----------------------------------------------------------------------+
| ``--pass``     | ``-p``   | Display both failing and passing tests.                               |
|                |          |   ``./my_test --pass``                                                |
+----------------+----------+-----------------------------------------------------------------------+
| ``--repeat N`` | ``-r N`` | Repeat each tests ``N`` times                                         |
|                |          |   ``./my_test --repeat 42``                                           |
+----------------+----------+-----------------------------------------------------------------------+
| ``--order X``  | ``-o X`` | Change the order in which tests are run:                              |
|                |          |  ``./my_test --order sorted``: tests are run in alphabetical order    |
|                |          |                                                                       |
|                |          |  ``./my_test --order random``: tests are run in random order          |
+----------------+----------+-----------------------------------------------------------------------+
| ``--seed N``   | ``-s N`` | Provide the pseudo-random number generator seed                       |
|                |          |   ``./my_test --seed 13374269``                                       |
+----------------+----------+-----------------------------------------------------------------------+
| ``--help``     | ``-h``   | Display the list of supported options                                 |
|                |          |   ``./my_test --help``                                                |
+----------------+----------+-----------------------------------------------------------------------+

.. _custom-equal:

Custom Equality Comparison
--------------------------

TODO

.. _custom-order:

Custom Ordering
---------------

TODO

.. _custom-ulp:

Custom ULP Comparison
---------------------

TODO

.. _custom-rel:

Custom Relative Comparison
--------------------------

TODO

.. _custom-abs:

Custom Absolute Comparison
--------------------------

TODO

.. _custom-driver:

Custom Driver
-------------

TODO
