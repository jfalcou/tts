Runtime Error Tests
===================

.. _tts-throw:

TTS_THROW
---------

**Required header:** ``#include <tts/tests/exceptions.hpp>``

.. code-block:: c++

   #define TTS_THROW( Expression )

Checks if ``Condition`` evaluates to ``true``.

**Parameters**
  - ``Expression``: Expression to test

**Example:**

.. literalinclude:: ../../test/exception/throw.cpp
   :language: cpp
   :lines: 10-22

.. _tts-no-throw:

TTS_NO_THROW
------------

**Required header:** ``#include <tts/tests/exceptions.hpp>``

.. code-block:: c++

   #define TTS_NO_THROW( Expression )

Checks if ``Condition`` evaluates to ``true``.

**Parameters**
  - ``Expression``: Expression to test

**Example:**

.. literalinclude:: ../../test/exception/nothrow.cpp
   :language: cpp
   :lines: 10-22
