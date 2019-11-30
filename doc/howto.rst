Using TTS
=========

Basic test structure
--------------------
Unit Test are usually gathered into **scenarios** in which a specific behavior of a software
component is tested. Such scenario are usually isolated into different executables.
Then, each scenario is decomposed into **test suites**, i.e a group of related test verifying that
the current scenario is correctly serviced by the software component. Test suites are themselves
composed of at least one test case.

With TTS, this decomposition (scenario/suite/test) is supported by various macro. The smallest
viable TTS scenario is then defined by a single C++ source file composed of:

  * the inclusion of the file ``tts/tts.hpp``
  * at least one test case defined by the appropriate macro.

That's it ! This is a fully functional TTS test suite. Let's dive into the detail and how TTS
manage and report tests.

Specify a Test Case
-------------------
Test cases are function performing repeatable, minimal operations able to unambiguously
capture a function or class behavior and assess its correctness. In TTS, a test case can be
defined either as:

* a single function defined by the :ref:`TTS_CASE <tts-case>` macro. In this case, the test
  function will be run and proceed to every tests it contains.

* a family of functions generated from a template function and a sequence of types
  via the :ref:`TTS_CASE_TPL <tts-case-tpl>` macro. Inside the test case itself, the template
  type is accessible through the template parameter ``T``.

In both case, the test case is defined with an unique string description. Additionally, good
testing practices recommend to have fine grain test cases to simplify test management.

.. literalinclude:: reference/simple.cpp
   :language: cpp
   :lines: 8-13

After compiling and launching the test, you should end up with the following output:

.. literalinclude:: output
   :lines: 1-2

The unit test reports the total number of tests performed, the number of passing tests, the number
of failing tests and the number of invalid tests. Depending on your terminal's settings, this
outputs may be colored [#f1]_.

In this case, unsurprisingly, our empty test is reported as invalid as we consider an empty test
case as erroneous in itself. TTS enforces a **"no test is invalid"** rule that applies at the test
case level. Any test case with no actual testing will be considered as a failure and reported as an
invalid test. Every test functions then require at least one call to any Testing Macros.

Testing Macros
--------------
TTS provides a set of macros to perform usual tests operations and reporting their success or
failure to the test suite manager directly from within a test case.

Basic Tests
^^^^^^^^^^^
Those macros provide a way to test how an arbitrary expression evaluate or to force a test
failure or success.

+-----------------------------------------+------------------------------------------------+
| Macro                                   | Description                                    |
+=========================================+================================================+
| :ref:`TTS_PASS <tts-pass>`              | Forces a success and displays a message        |
+-----------------------------------------+------------------------------------------------+
| :ref:`TTS_FAIL <tts-fail>`              | Forces a failure and displays a message        |
+-----------------------------------------+------------------------------------------------+
| :ref:`TTS_INVALID <tts-invalid>`        | Forces a failure and displays a message        |
+-----------------------------------------+------------------------------------------------+
| :ref:`TTS_EXPECT <tts-expect>`          | Checks if an expression evaluates to ``true``  |
+-----------------------------------------+------------------------------------------------+
| :ref:`TTS_EXPECT_NOT <tts-expect-not>`  | Checks if an expression evaluates to ``false`` |
+-----------------------------------------+------------------------------------------------+

Let's amend our initial test by adding a single call to :ref:`TTS_PASS <tts-pass>`:

.. literalinclude:: reference/simple.cpp
   :language: cpp
   :lines: 16-24

and run it again:

.. literalinclude:: output
   :lines: 3-4

One may notice that no other informations are displayed. This is the default behavior. One can
activate a more verbose output by passing the ``--pass`` through the :ref:`tts-cli`. The output
now includes all the informations gathered by the testing macros, including the source file
location of each tests and the current scenario being tested.

.. literalinclude:: output
   :lines: 5-12

More complex tests may be required including relation between values or precision measurements.

Relational Tests
^^^^^^^^^^^^^^^^
Relational tests compares the equality or ordering of values. Test is considered
successful if values are properly ordered with respect to each other:

+--------------------------+------------------------------------------------------+
| Macro                    | Description                                          |
+==========================+======================================================+
| :ref:`tts-equal`         | Checks if one value is equal to another              |
+--------------------------+------------------------------------------------------+
| :ref:`tts-not-equal`     | Checks if one value is not equal to another          |
+--------------------------+------------------------------------------------------+
| :ref:`tts-less`          | Checks if one value is lesser or equal than another  |
+--------------------------+------------------------------------------------------+
| :ref:`tts-greater`       | Checks if one value is greater  than another         |
+--------------------------+------------------------------------------------------+
| :ref:`tts-less-equal`    | Checks if one value is lesser or equal than another  |
+--------------------------+------------------------------------------------------+
| :ref:`tts-greater-equal` | Checks if one value is greater or equal than another |
+--------------------------+------------------------------------------------------+

These macros are accessible in the ``tts/unit/tests/relation.hpp`` header file.

Note that all those relational tests are performed using either the default ``==`` and ``<``
operators. One can customize this behavior by providing :ref:`custom-equal` or :ref:`custom-order`.

Let's see how those tests integrates in our small test suite:

.. literalinclude:: reference/simple.cpp
   :language: cpp
   :lines: 27-38

If everything goes right, the following output is to be expected:

.. literalinclude:: output
   :lines: 13-21

Precision Tests
^^^^^^^^^^^^^^^
These tests are intended to compare floating points results, allowing a third parameter to
state the admitted possible difference between the two quantities. This difference can be expressed
in:

  * ULPs, that is units in the last place, which is an indication of the number of floating
    points values that are representable between the compared quantities (see the
    :ref:`rationale-precision` section for more details).
  * A relative tolerance, that is a percentage of the reference quantity which defines a validity
    interval around the expected value.
  * A absolute tolerance, that is a fixed quantity which defines a validity interval around the
    expected value.

+--------------------------+------------------------------------------------------+
| Macro                    | Description                                          |
+==========================+======================================================+
| :ref:`tts-ulp-equal`     | Checks if two values are equal within some ULPs      |
+--------------------------+------------------------------------------------------+
| :ref:`tts-ieee-equal`    | Checks if two values are equal as IEEE 754 values    |
+--------------------------+------------------------------------------------------+
| :ref:`tts-rel-equal`     | Checks if one value is not equal to another          |
+--------------------------+------------------------------------------------------+
| :ref:`tts-abs-equal`     | Checks if one value is lesser or equal than another  |
+--------------------------+------------------------------------------------------+

As for the relational comparison macros, the behavior of this set of test comparison can
customized through a set of specific function (see :ref:`custom-ulp`, :ref:`custom-rel`
or :ref:`custom-abs`.

Let's have a look at the expected output of such macros

.. literalinclude:: ../test/precision/ulp.cpp
   :language: cpp
   :lines: 10-22

.. literalinclude:: output
   :lines: 22-28

Sequence Tests
^^^^^^^^^^^^^^

Another common use-case often consists in comparing two series of values.
Additional macros are provided for these use-cases. They perform element-wise comparison
of values and also detects size mismatch in the series. If an error occurs, a sum up of incorrect
values with their position and source of error is reported.

+---------------------------+------------------------------------------------------+
| Macro                     | Description                                          |
+===========================+======================================================+
| :ref:`tts-all-ulp-equal`  | Checks if one value is equal to another              |
+---------------------------+------------------------------------------------------+
| :ref:`tts-all-ieee-equal` | Checks if one value is equal to another              |
+---------------------------+------------------------------------------------------+
| :ref:`tts-all-rel-equal`  | Checks if one value is not equal to another          |
+---------------------------+------------------------------------------------------+
| :ref:`tts-all-equal`      | Checks if one value is equal to another              |
+---------------------------+------------------------------------------------------+
| :ref:`tts-all-abs-equal`  | Checks if one value is lesser or equal than another  |
+---------------------------+------------------------------------------------------+

These macros are accessible in the ``tts/unit/tests/precision.hpp`` header file.

Runtime Error Tests
^^^^^^^^^^^^^^^^^^^
Those tests check if a given expression exhibit a given runtime behavior with respect
to exceptions and assertions. Expression can be tested for being able to throw or not
and to trigger an assertion failure or not. Note that those test macros requires the
exception to be detected to inherits from ``std::exception``.

+---------------------+-------------------------------------------------------------------+
| Macro               | Description                                                       |
+=====================+===================================================================+
| :ref:`tts-throw`    | Checks if a given expression throws an exception of a given type  |
+---------------------+-------------------------------------------------------------------+
| :ref:`tts-no-throw` | Checks if a given expression does not throw                       |
+---------------------+-------------------------------------------------------------------+

These macros are accessible in the ``tts/unit/tests/exceptions.hpp`` file.

Type Related Tests
^^^^^^^^^^^^^^^^^^
Those tests check if a given type or type of expression is verify some relation with another types
or some general types requirements.

+---------------------+--------------------------------------------------------------------+
| Macro               | Description                                                        |
+=====================+====================================================================+
| :ref:`tts-type-is`  | Checks if two types are the same                                   |
+---------------------+--------------------------------------------------------------------+
| :ref:`tts-expr-is`  | Checks if a given expression evaluates to a result of a given type |
+---------------------+--------------------------------------------------------------------+

These macros are accessible in the ``tts/unit/tests/types.hpp`` file.

Tests Fixtures
--------------


.. rubric:: Footnotes

.. [#f1] Passing ``--no-color`` through the :ref:`tts-cli` remove said colors.
