.. _tts-sequence:

Sequence Tests
==============

The following component provides macros to perform tests over sequence of values.
All those macros are based on the fact that the check to perform must pass for every elements
of the sequence. For convenience, one can substitute one of two of the input sequences by a scalar.
This scalar will be interpreted as a sequence of one element.

To use those macros, include the ``tts/tests/sequence.hpp`` file.

.. note::
   Precision comparisons are, by default, handled by the ``tts::ulpdist``, ``tts::reldist`` or
   ``tts::absdist`` overloads. In the case one needs to perform a custom precision comparison, see
   the :ref:`custom-precision` section.

Macros
^^^^^^

.. _tts-all-ulp-equal:

.. doxygendefine:: TTS_ALL_ULP_EQUAL
   :project: tts


.. _tts-all-ieee-equal:

.. doxygendefine:: TTS_ALL_IEEE_EQUAL
   :project: tts


.. _tts-all-rel-equal:

.. doxygendefine:: TTS_ALL_RELATIVE_EQUAL
   :project: tts


.. _tts-all-equal:

.. doxygendefine:: TTS_ALL_EQUAL
   :project: tts

.. _tts-all-abs-equal:

.. doxygendefine:: TTS_ALL_ABSOLUTE_EQUAL
   :project: tts
