Status tests
============

.. _tts-pass:

TTS_PASS
--------

**Required header:** ``#include <tts/tests/infos.hpp>``

.. code-block:: c++

   #define TTS_PASS( Message )

Register a test which always passes and streams a custom message string.

**Parameters**
  - ``Message``: A literal string to display as additional informations

**Example:**

.. literalinclude:: ../../test/infos/pass.cpp
   :language: cpp
   :lines: 10-16

.. _tts-fail:

TTS_FAIL
--------

**Required header:** ``#include <tts/tests/infos.hpp>``

.. code-block:: c++

   #define TTS_FAIL( Message )

Register a test which always fails and streams a custom message string.

**Parameters**
  - ``Message``: A literal string to display as additional informations

**Example:**

.. literalinclude:: ../../test/infos/fail.cpp
   :language: cpp
   :lines: 11-17
