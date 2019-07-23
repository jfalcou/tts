Sequence Tests
==============

.. _tts-all-ulp-equal:

TTS_ALL_ULP_EQUAL
-----------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ALL_ULP_EQUAL( LHS, RHS, THRES )

Checks if the ULP distances between all values in the sequences ``LHS`` and ``RHS`` are less
or equal to  ``THRES``.

.. note::
   For convenience, one can substitute one of two of the input sequences by a scalar. This scalar
   will be interpreted as a sequence of one element.

.. note::
   ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-ulp` section.

**Parameters**
  - ``LHS``, ``RHS``: Sequences to compares
  - ``THRES``       : Maximum number of ULPs between the values of the two sequences to compare

**Example:**

.. literalinclude:: ../../test/sequence/ulp.cpp
   :language: cpp
   :lines: 10-25

.. _tts-all-ieee-equal:

TTS_ALL_IEEE_EQUAL
------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ALL_IEEE_EQUAL( LHS, RHS )

Checks if the ULP distances between all values in the sequences ``LHS`` and ``RHS`` are exactly
equal to 0. Note that, contrary to :ref:`tts-equal`, this macro allows comparison of infinites or
NaN values.

.. note::
   For convenience, one can substitute one of two of the input sequences by a scalar. This scalar
   will be interpreted as a sequence of one element.

.. note::
   ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-ulp` section.

**Parameters**
  - ``LHS``, ``RHS``: Sequences to compares

**Example:**

.. literalinclude:: ../../test/sequence/ieee.cpp
   :language: cpp
   :lines: 10-20

.. _tts-all-rel-equal:

TTS_ALL_RELATIVE_EQUAL
----------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_RELATIVE_EQUAL( LHS, RHS, THRES )

Checks if the relative distances between all values in the sequences ``LHS`` and ``RHS`` are
less or equal to ``THRES`` percent.

.. note::
   For convenience, one can substitute one of two of the input sequences by a scalar. This scalar
   will be interpreted as a sequence of one element.

.. note::
   Relative comparison is, by default, handled by the ``tts::reldist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-rel` section.

**Parameters**
  - ``LHS``, ``RHS``: Sequences to compares
  - ``THRES``       : Maximum percentage between the values of the two sequences to compare

**Example:**

.. literalinclude:: ../../test/sequence/relative.cpp
   :language: cpp
   :lines: 10-22

.. _tts-all-equal:

TTS_ALL_EQUAL
------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ALL_EQUAL( LHS, RHS )

Checks if the relative distances between all values in the sequences ``LHS`` and ``RHS`` are
exactly equal to 0%.

.. note::
   For convenience, one can substitute one of two of the input sequences by a scalar. This scalar
   will be interpreted as a sequence of one element.

.. note::
   ULP comparison is, by default, handled by the ``tts::reldist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-rel` section.

**Parameters**
  - ``LHS``, ``RHS``: Sequences to compares

**Example:**

.. literalinclude:: ../../test/sequence/equal.cpp
   :language: cpp
   :lines: 10-21

.. _tts-all-abs-equal:

TTS_ALL_ABSOLUTE_EQUAL
----------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ABSOLUTE_EQUAL( LHS, RHS, THRES )

Checks if the absolute distances between all values in the sequences ``LHS`` and ``RHS`` are
less or equal to ``THRES``.

.. note::
   For convenience, one can substitute one of two of the input sequences by a scalar. This scalar
   will be interpreted as a sequence of one element.

.. note::
   Absolute comparison is, by default, handled by the ``tts::absdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-abs` section.

**Parameters**
  - ``LHS``, ``RHS``: Sequences to compares
  - ``THRES``       : Maximum value between the values of the two sequences compare

**Example:**

.. literalinclude:: ../../test/sequence/absolute.cpp
   :language: cpp
   :lines: 10-22

