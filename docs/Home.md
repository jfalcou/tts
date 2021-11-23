<img src="https://github.com/jfalcou/tts/wiki/logo.png" alt="" data-canonical-src="https://github.com/jfalcou/tts/wiki/logo.png" align="left"  width="15%" height="15%" />

# TTS

[![Licence](https://img.shields.io/github/license/jfalcou/ofw?style=plastic)](https://img.shields.io/github/license/jfalcou/ofw?style=plastic)
[![Discord](https://img.shields.io/discord/692734675726237696?style=plastic)](https://discord.com/channels/692734675726237696/692735300522344468)
[![CircleCI](https://circleci.com/gh/jfalcou/tts.svg?style=svg&circle-token=8482adcf5f6785a67d19cf73b6fd4959d53e0e25)](https://circleci.com/gh/jfalcou/tts)
[![Build status](https://ci.appveyor.com/api/projects/status/020yd3e6dwhu4dae/branch/master?svg=true)](https://ci.appveyor.com/project/jfalcou/tts/branch/master)

## Overview

Tiny Test System is a C++ open-source Unit Test library designed following the ideas of libraries like CATCH or LEST.

Its main goal is to provide, in addition to classical TDD features, a set of tests for :

- IEEE precision-related testing: ULP and relative error
- Testing over arrays of data
- Exhaustive and sampling test over data sets
- A few customization points for 3rd party types

making TTS suitable for numerical-heavy testing.
