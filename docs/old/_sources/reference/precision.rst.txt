.. _tts-precision:

Precision Tests
===============

The following component provides macros to perform precision tests between values.
To use those macros, include the ``tts/tests/precision.hpp`` file.

.. note::
   Precision comparisons are, by default, handled by the ``tts::ulpdist``, ``tts::reldist`` or
   ``tts::absdist`` overloads. In the case one needs to perform a custom precision comparison, see
   the :ref:`custom-precision` section.

Macros
^^^^^^

.. _tts-ulp-equal:

.. doxygendefine:: TTS_ULP_EQUAL
   :project: tts


.. _tts-ieee-equal:

.. doxygendefine:: TTS_IEEE_EQUAL
   :project: tts

.. _tts-rel-equal:

.. doxygendefine:: TTS_RELATIVE_EQUAL
   :project: tts

.. _tts-abs-equal:

.. doxygendefine:: TTS_ABSOLUTE_EQUAL
   :project: tts
