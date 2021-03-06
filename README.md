<img src="https://github.com/jfalcou/tts/raw/develop/docs/logo.png" alt="" data-canonical-src="https://github.com/jfalcou/tts/raw/develop/docs/logo.png" align="left"  width="15%" height="15%" />

# TTS

[![Licence](https://img.shields.io/github/license/jfalcou/ofw?style=plastic)](https://img.shields.io/github/license/jfalcou/ofw?style=plastic) 
[![Discord](https://img.shields.io/discord/692734675726237696?style=plastic)](https://discord.com/channels/692734675726237696/692735300522344468) 

## Overview

Tiny Test System is a C++ open-source Unit Test library designed following
the ideas of libraries like CATCH or LEST.

Its main goal is to provide, in addition to classical TDD features, a set of
tests for :

- IEEE precision-related testing: ULP and relative error
- Testing over arrays of data
- Exhaustive and sampling test over data sets
- A few customization points for 3rd party types

making TTS suitable for numerical-heavy testing.

## Status

CircleCI :
[![CircleCI](https://circleci.com/gh/jfalcou/tts.svg?style=svg&circle-token=8482adcf5f6785a67d19cf73b6fd4959d53e0e25)](https://circleci.com/gh/jfalcou/tts)

AppVeyo :
[![Build status](https://ci.appveyor.com/api/projects/status/020yd3e6dwhu4dae/branch/master?svg=true)](https://ci.appveyor.com/project/jfalcou/tts/branch/master)

## Acknowledgment

Thanks to Ihor Kalnytskyi for [Termcolor](https://termcolor.readthedocs.io/).

## License

This library is licensed under the [MIT License](http://opensource.org/licenses/MIT):

```Copyright (c) 2018 Joel FALCOU

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.```

