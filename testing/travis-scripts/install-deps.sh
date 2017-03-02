#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

sudo apt-get -y update
sudo apt-get -y install build-essential cmake clang gcc \
		    gcc-mingw-w64-x86-64 cppcheck \
		    valgrind doxygen
