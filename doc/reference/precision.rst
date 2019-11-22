Precision Tests
===============

.. _tts-ulp-equal:

TTS_ULP_EQUAL
-------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ULP_EQUAL( LHS, RHS, THRES )

Checks if the ULP distance between ``LHS`` and ``RHS`` is less or equal to  ``THRES``.

.. note::
   ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-ulp` section.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares
  - ``THRES``       : Maximum number of ULPs between the two values to compare

**Example:**

.. literalinclude:: ../../test/precision/ulp.cpp
   :language: cpp
   :lines: 10-22

.. _tts-ieee-equal:

TTS_IEEE_EQUAL
--------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_IEEE_EQUAL( LHS, RHS )

Checks if the ULP distance between ``LHS`` and ``RHS`` is exactly equal to 0. Note that, contrary to
:ref:`tts-equal`, this macro allows comparison of infinites or NaN values.

.. note::
   ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-ulp` section.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/precision/ieee.cpp
   :language: cpp
   :lines: 10-21

.. _tts-rel-equal:

TTS_RELATIVE_EQUAL
------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_RELATIVE_EQUAL( LHS, RHS, THRES )

Checks if the relative distance between ``LHS`` and ``RHS`` is less or equal to ``THRES`` percent.

.. note::
   Relative comparison is, by default, handled by the ``tts::reldist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-rel` section.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares
  - ``THRES``       : Maximum percentage between the two values to compare

**Example:**

.. literalinclude:: ../../test/precision/relative.cpp
   :language: cpp
   :lines: 10-19

.. _tts-abs-equal:

TTS_ABSOLUTE_EQUAL
------------------

**Required header:** ``#include <tts/tests/precision.hpp>``

.. code-block:: c++

   #define TTS_ABSOLUTE_EQUAL( LHS, RHS, THRES )

Checks if the absolute distance between ``LHS`` and ``RHS`` is less or equal to ``THRES``.

.. note::
   Absolute comparison is, by default, handled by the ``tts::absdist`` overload. In the case one
   needs to perform a custom ULP comparison, see the :ref:`custom-abs` section.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares
  - ``THRES``       : Maximum value between the two values to compare

**Example:**

.. literalinclude:: ../../test/precision/absolute.cpp
   :language: cpp
   :lines: 10-17
