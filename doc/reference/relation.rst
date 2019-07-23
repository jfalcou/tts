Relational Tests
================

.. _tts-equal:

TTS_EQUAL
---------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_EQUAL( LHS, RHS )

Checks if ``LHS == RHS`` evaluates to ``true``.

.. note::
   Equality comparison is, by default, handled by the proper ``operator==`` overload. In the case one
   needs to perform a custom equality comparison, see the :ref:`custom-equal` section.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/equal.cpp
   :language: cpp
   :lines: 10-19

.. _tts-not-equal:

TTS_NOT_EQUAL
-------------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_NOT_EQUAL( LHS, RHS )

Checks if ``LHS == RHS`` evaluates to ``false``.

.. note::
   Equality comparison is, by default, handled by the proper ``operator==`` overload. In case one
   needs to perform a custom equality comparison, see :ref:`custom-equal`.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/not_equal.cpp
   :language: cpp
   :lines: 10-19

.. _tts-less:

TTS_LESS
--------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_LESS( LHS, RHS )

Checks if ``LHS < RHS`` evaluates to ``true``.

.. note::
   Less-than comparison is, by default, handled by the proper ``operator<`` overload. In case one
   needs to perform a custom equality comparison, see :ref:`custom-order`.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/less.cpp
   :language: cpp
   :lines: 10-19


.. _tts-greater:

TTS_GREATER
-----------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_GREATER( LHS, RHS )

Checks if ``LHS > RHS`` evaluates to ``true``.

.. note::
   Less-than comparison is, by default, handled by the proper ``operator<`` overload. In case one
   needs to perform a custom equality comparison, see :ref:`custom-order`.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/greater.cpp
   :language: cpp
   :lines: 10-19

.. _tts-less-equal:

TTS_LESS_EQUAL
--------------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_LESS_EQUAL( LHS, RHS )

Checks if ``LHS <= RHS`` evaluates to ``true``.

.. note::
   Less-than comparison is, by default, handled by the proper operator overloads. In case one
   needs to perform a custom equality comparison, see :ref:`custom-order`.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/less_equal.cpp
   :language: cpp
   :lines: 10-22

.. _tts-greater-equal:

TTS_GREATER_EQUAL
-----------------

**Required header:** ``#include <tts/tests/relation.hpp>``

.. code-block:: c++

   #define TTS_GREATER_EQUAL( LHS, RHS )

Checks if ``LHS >= RHS`` evaluates to ``true``.

.. note::
   Less-than comparison is, by default, handled by the proper operator overloads. In case one
   needs to perform a custom equality comparison, see :ref:`custom-order`.

**Parameters**
  - ``LHS``, ``RHS``: Expression to compares

**Example:**

.. literalinclude:: ../../test/relation/greater_equal.cpp
   :language: cpp
   :lines: 10-22
