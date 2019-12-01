Tiny Test System
================

TTS is a **Tiny Test System** designed to simplify the design of unit tests in `C++17`.
**TTS** is based on the BDD methodology and is largely inspired by `the LEST framework`_ by Martin
Moene in both implementation and general design.

**TTS** is however biased toward testing numerical computation code by providing a selection of
various precision oriented checks including exhaustive exploration of a function's precision by
testing each and every possible values [#f1]_ or by testing functions over random samples of values.

**TTS** is also meant to be extended for user-defined types so testing your own types using our
methodology is as simple as overloading a function.

.. toctree::
   :caption: Contents
   :maxdepth: 2

   install
   howto
   reference
   extension
   rationale

.. rubric:: Footnotes

.. [#f1] This methodology is inspired from `this Bruce Dawson's blog post`_ and can be used to completely map the behavior of functions.

.. _`the LEST framework`: https://github.com/martinmoene/lest
.. _`this Bruce Dawson's blog post`: https://randomascii.wordpress.com/2014/01/27/theres-only-four-billion-floatsso-test-them-all/
