.. _tts-exceptions:

Runtime Error Tests
===================

The following component provides macros to perform basic tests on the throwing or
absence of runtime exceptions. Note that those test macros requires the exception
to be detected to inherits from ``std::exception``.

To use those macros, include the ``tts/tests/exception.hpp`` file.

Macros
^^^^^^

.. _tts-throw:

.. doxygendefine:: TTS_THROW
   :project: tts

.. _tts-no-throw:

.. doxygendefine:: TTS_NO_THROW
   :project: tts
