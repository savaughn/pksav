#!/bin/bash
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then

    brew update
    brew install doxygen valgrind
    brew link valgrind

else
    sudo apt-get -y update
    sudo apt-get -y install build-essential cmake clang gcc \
                            gcc-mingw-w64-x86-64 cppcheck \
                            valgrind doxygen
fi
