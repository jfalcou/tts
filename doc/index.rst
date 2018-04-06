Simple Test Framework
=====================

TTS is a **Simple Test Framework** designed to simplify the design of unit tests in `C++17`.
**TTS** is based on the BDD methodology and is largely inspired by `the LEST framework`_ by Martin
Moene in both implementation and general design.

**TTS** is however biased toward the need of writing tests for numerical computation code by
providing a selection of various precision oriented checks including exhaustive exploration of a
function's precision by testing each and every possible values [#f1]_ or by testing functions
over random samples of values.

**TTS** is also meant to be extended for user-defined types so testing your own types using our
methodology is as simple as overloading a function.

.. note:: Godot Engine is an open source project developed by a community of
          volunteers. It means that the documentation team can always use your
          feedback and help to improve the tu

.. toctree::
   :caption: Introduction to TTS
   :maxdepth: 2

   howto
   rationale

.. toctree::
   :caption: Reference Manual
   :maxdepth: 2

   reference
   extension

.. rubric:: Footnotes

.. [#f1] This methodology is inspired from `this Bruce Dawson's blog post`_ and can be used to completely map the behavior of functions.

.. _`the LEST framework`: https://github.com/martinmoene/lest
.. _`this Bruce Dawson's blog post`: https://randomascii.wordpress.com/2014/01/27/theres-only-four-billion-floatsso-test-them-all/
