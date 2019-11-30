Basic Tests
===========

.. _tts-expect:

TTS_EXPECT
----------

**Required header:** ``#include <tts/tests/basic.hpp>``

.. code-block:: c++

   #define TTS_EXPECT( Condition )

Checks if ``Condition`` evaluates to ``true``.

**Parameters**
  - ``Condition``: Expression to test

**Example:**

.. literalinclude:: ../../test/basic/expect.cpp
   :language: cpp
   :lines: 10-22

.. _tts-expect-not:

TTS_EXPECT_NOT
--------------

**Required header:** ``#include <tts/tests/basic.hpp>``

.. code-block:: c++

   #define TTS_EXPECT_NOT( Condition )

Checks if ``Condition`` evaluates to ``false``.

**Parameters**
  - ``Condition``: Expression to test

**Example:**

.. literalinclude:: ../../test/basic/expect_not.cpp
   :language: cpp
   :lines: 10-22
