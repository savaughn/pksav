PKSav
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/pksav/blob/master/LICENSE.txt)

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

README last updated: 2021/11/13
