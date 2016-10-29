PKSav
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/pksav/blob/master/LICENSE.txt)
[![Travis-CI Build](https://travis-ci.org/ncorgan/pksav.svg)](https://travis-ci.org/ncorgan/pksav)

The goal of PKSav is to be a small, fast, and portable library for interacting with
and manipulating Pokémon save files. It is written in pure C with no external dependencies,
which allows it to be deployed on a wide variety of platforms, from PC's to smartphones to
handheld consoles.

In the interest of taking as little space as possible, this library can be built with support
for only the specific categories of games needed. For example, on a platform that only has emulation
for Generation I-II games, it is unnecessary to include support for later games, so support for these
can be disabled, halving the size of the library.

It is not meant to be a plug-and-play solution to save editing but rather the
internal workings of a larger library that provides users with a more convenient interface.
Being written in C, it can easily be wrapped in a larger C/C++/Objective-C library or be called as native
functions in languages such as C# or Java.

Currently, only American save files are supported, as save files from different regions are often laid
out differently, usually to accomodate strings having different lengths between regions.

Component status:
 * Generation I: complete
 * Generation II: complete
 * Game Boy Advance: complete
 * Gamecube: in progress
 * Generation IV: in progress
 * Generation V: in progress
 * Generation VI: in progress

Platform status:
 * x86: supported
 * 3DS: TBA
 * iOS: TBA
 * Android: TBA

README last updated: 2016/10/29
