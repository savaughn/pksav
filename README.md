PKSav
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/pksav/blob/master/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/savaughn/pksav)](https://github.com/savaughn/pksav/releases) [![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/savaughn/pksav/build-macos.yml?branch=master)](https://github.com/savaughn/pksav/actions) [![GitHub issues](https://img.shields.io/github/issues/savaughn/pksav)](https://github.com/savaughn/pksav/issues)[![GitHub all releases](https://img.shields.io/github/downloads/savaughn/pksav/total)](https://github.com/savaughn/pksav/releases)

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


Component Status
-------------------------------------
 * Generation I: complete
 * Generation II: complete
 * Game Boy Advance: complete
 * All other generations: not implemented

Acknowledgments
-------------------------------------
* PKSAV ([@ncorgan](https://github.com/ncorgan/pksav)), the original library this was forked from.
* PRET ([@pret](https://github.com/pret)), whose Pokémon disassemblies greatly contributed to this library's functionality.
* Vicky "Endrift" Flau ([@endrift](https://github.com/endrift)), whose mGBA emulator allowed for further reverse-engineering.
* Michael Bond ([@codemonkey85](https://github.com/codemonkey85)), for consulting on game mechanics.
