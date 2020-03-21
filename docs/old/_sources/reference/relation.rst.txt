.. _tts-relation:

Relational Tests
================
The following component provides macros to perform ordering test between values.

To use those macros, include the ``tts/tests/relation.hpp`` file.

Macros
^^^^^^

.. note::
   Ordering and equality comparisons are, by default, handled by the ``operator==`` or
   ``opetator<`` overloads. In the case one needs to perform a custom comparison, see
   the :ref:`custom-comparison` section.

.. _tts-equal:

.. doxygendefine:: TTS_EQUAL
   :project: tts

.. _tts-not-equal:

.. doxygendefine:: TTS_NOT_EQUAL
   :project: tts

.. _tts-less:

.. doxygendefine:: TTS_LESS
   :project: tts

.. _tts-greater:

.. doxygendefine:: TTS_GREATER
   :project: tts

.. _tts-less-equal:

.. doxygendefine:: TTS_LESS_EQUAL
   :project: tts

.. _tts-greater-equal:

.. doxygendefine:: TTS_GREATER_EQUAL
   :project: tts
