#!/bin/bash
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env
cd test-env
[ $? -ne 0 ] && exit 1

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    # Compile test
    mkdir -p build
    cd build

    cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1
else
    # Check source
    find $REPO_TOPLEVEL -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
    [ $? -ne 0 ] && exit 1

    mkdir -p gcc clang mingw

    # GCC compile check
    cd $REPO_TOPLEVEL/test-env/gcc
    [ $? -ne 0 ] && exit 1
    cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    # Clang compile check
    cd $REPO_TOPLEVEL/test-env/clang
    [ $? -ne 0 ] && exit 1
    CC=clang cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    # MinGW-GCC compile check
    cd $REPO_TOPLEVEL/test-env/mingw
    [ $? -ne 0 ] && exit 1
    cmake -DCMAKE_TOOLCHAIN_FILE=$REPO_TOPLEVEL/cmake/Toolchains/mingw_cross.cmake \
          -DMINGW_PREFIX=x86_64-w64-mingw32 \
          -DCMAKE_BUILD_TYPE=Debug \
          $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
fi

echo # So we can check the last error code
