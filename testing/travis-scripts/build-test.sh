#!/bin/bash
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
cd travis-env
[ $? -ne 0 ] && exit 1

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    mkdir build
    cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure

else
    mkdir gcc clang mingw

    cd $REPO_TOPLEVEL/gcc
    cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    cd $REPO_TOPLEVEL/clang
    CC=clang cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    cd $REPO_TOPLEVEL/mingw
    cmake -DCMAKE_TOOLCHAIN=$REPO_TOPLEVEL/cmake/Toolchains/mingw_cross.cmake \
	  -DMINGW_PREFIX=x86_64-w64-mingw32 \
	  $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
fi
