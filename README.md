PKSav
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/pksav/blob/master/LICENSE.txt)
[![Travis-CI Build](https://travis-ci.org/ncorgan/pksav.svg)](https://travis-ci.org/ncorgan/pksav)
[![AppVeyor Build](https://ci.appveyor.com/api/projects/status/github/ncorgan/pksav)](https://ci.appveyor.com/project/ncorgan/pksav)

The goal of PKSav is to be a small, fast, and portable library for interacting with
and manipulating Pokémon save files. It is written in pure C with no external dependencies,
which allows it to be deployed on a wide variety of platforms, from PC's to smartphones to
handheld consoles.

It is not meant to be a plug-and-play solution to save editing but rather the
internal workings of a larger library that provides users with a more convenient interface.
Being written in C, it can easily be wrapped in a larger C/C++/Objective-C library or be called as native
functions in languages such as C# or Java.

Currently, only American save files are supported, as save files from different regions are often laid
out differently, usually to accomodate strings having different lengths between regions.

Testing
-------------------------------------

Using [Travis](https://travis-ci.org/ncorgan/pksav) and
[AppVeyor](https://ci.appveyor.com/project/ncorgan/pksav), every push to this
repository is automatically tested under the following configurations:

 * Compile, unit test:
   * Ubuntu 14.04 (x64)
     * GCC: 4.8.4, 5.4.1, 6.2.0, 7.2.0
     * Clang: 3.9.0, 4.0.1, 5.0.0
   * Windows Server 2012 R2 (x86, x64)
     * MSVC 2015 (x86, x64)

All source files are analyzed with [CppCheck](http://cppcheck.sourceforge.net/).

Unit tests use the [Unity](https://github.com/ThrowTheSwitch/Unity) framework.

Component Status
-------------------------------------
 * Generation I: complete
 * Generation II: complete
 * Game Boy Advance: complete
 * Generation IV: in progress
 * Generation V: in progress
 * Generation VI: in progress
 * Generation VII: in progress

README last updated: 2017/11/26
