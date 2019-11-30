Scenario definition
===================

.. _tts-case:

TTS_CASE
--------

**Required header:** ``#include <tts/case.hpp>``

.. code-block:: c++

   #define TTS_CASE( Description )

Introduces new test scenario and registers it into the current test driver.

The following block will contains user defined preparation, actual tests, expectations and
clean-up for said test case.

.. note::
   Test cases performing no actual tests swill be reported as an automatic failure.

**Parameters**
  - ``Description``: A literal string describing the scenario intents.

**Example:**

.. literalinclude:: simple.cpp
   :language: cpp
   :lines: 8-13

.. _tts-case-tpl:

TTS_CASE_TPL
------------

**Required header:** ``#include <tts/case.hpp>``

.. code-block:: c++

   #define TTS_CASE_TPL( Description, ... )

