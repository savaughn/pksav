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
    # Check source
    find $REPO_TOPLEVEL -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
    [ $? -ne 0 ] && exit 1

    mkdir gcc clang mingw

    # GCC compile check
    cd $REPO_TOPLEVEL/travis-env/gcc
    [ $? -ne 0 ] && exit 1
    cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    # Clang compile check
    cd $REPO_TOPLEVEL/travis-env/clang
    [ $? -ne 0 ] && exit 1
    CC=clang cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    # MinGW-GCC compile check
    cd $REPO_TOPLEVEL/travis-env/mingw
    [ $? -ne 0 ] && exit 1
    cmake -DCMAKE_TOOLCHAIN_FILE=$REPO_TOPLEVEL/cmake/Toolchains/mingw_cross.cmake \
          -DMINGW_PREFIX=x86_64-w64-mingw32 \
          -DCMAKE_BUILD_TYPE=Debug \
          $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1

    # Set up runtime testing
    cd $REPO_TOPLEVEL/travis-env
    export SAVEDIR=$REPO_TOPLEVEL/testing/pksav-test-saves
    export PATH=$PWD/gcc/apps:$PATH
    export LD_LIBRARY_PATH=$PWD/gcc/lib:$LD_LIBRARY_PATH

    # App testing
    valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/red_blue/pokemon_red.sav
    [ $? -ne 0 ] && exit 1
    valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/yellow/pokemon_yellow.sav
    [ $? -ne 0 ] && exit 1
    valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump --all --input=$SAVEDIR/gold_silver/pokemon_gold.sav
    [ $? -ne 0 ] && exit 1
    valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump --all --input=$SAVEDIR/crystal/pokemon_crystal.sav
fi
