Types Tests
===========

.. _tts-type-is:

TTS_TYPE_IS
-----------

**Required header:** ``#include <tts/tests/types.hpp>``

.. code-block:: c++

   #define TTS_TYPE_IS( LHS, RHS )

Checks if ``LHS`` and ``RHS`` satisfy ``std::is_same_v<LHS,RHS> == true``.

**Parameters**
  - ``LHS``, ``RHS``: Types to compare

**Example:**

.. literalinclude:: ../../test/types/type_is.cpp
   :language: cpp
   :lines: 10-22

.. _tts-expr-is:

TTS_EXPR_IS
-----------

**Required header:** ``#include <tts/tests/types.hpp>``

.. code-block:: c++

   #define TTS_EXPR_IS( Expression, Type )

Checks if ``Expression`` evaluates to a value of type ``Type``.

**Parameters**
  - ``Expression``: Expression to test
  - ``Type``: Expexted type

**Example:**

.. literalinclude:: ../../test/types/expr_is.cpp
   :language: cpp
   :lines: 10-22
