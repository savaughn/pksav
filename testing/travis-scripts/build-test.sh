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
    cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure

    # Set up runtime testing
    cd $REPO_TOPLEVEL
    SAVEDIR=$REPO_TOPLEVEL/testing/pksav-test-saves
    PATH=$PWD/$dir/apps:$OLD_PATH
    LD_LIBRARY_PATH=$PWD/$dir/lib:$OLD_LD_LIBRARY_PATH

    # App testing
    if [ `which valgrind` ]
    then
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/red_blue/pokemon_red.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/yellow/pokemon_yellow.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump $SAVEDIR/gold_silver/pokemon_gold.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump $SAVEDIR/crystal/pokemon_crystal.sav
        [ $? -ne 0 ] && exit 1
    else
        pksav-gen1-save-dump --all --input=$SAVEDIR/red_blue/pokemon_red.sav
        [ $? -ne 0 ] && exit 1
        pksav-gen1-save-dump --all --input=$SAVEDIR/yellow/pokemon_yellow.sav
        [ $? -ne 0 ] && exit 1
        pksav-gen2-save-dump $SAVEDIR/gold_silver/pokemon_gold.sav
        [ $? -ne 0 ] && exit 1
        pksav-gen2-save-dump $SAVEDIR/crystal/pokemon_crystal.sav
        [ $? -ne 0 ] && exit 1
    fi
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

    # Set up runtime testing
    cd $REPO_TOPLEVEL/test-env
    SAVEDIR=$REPO_TOPLEVEL/testing/pksav-test-saves
    OLD_PATH=$PATH
    OLD_LD_LIBRARY_PATH=$LD_LIBRARY_PATH

    for dir in gcc clang
    do
        PATH=$PWD/$dir/apps:$OLD_PATH
        LD_LIBRARY_PATH=$PWD/$dir/lib:$OLD_LD_LIBRARY_PATH

        # App testing
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/red_blue/pokemon_red.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen1-save-dump --all --input=$SAVEDIR/yellow/pokemon_yellow.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump $SAVEDIR/gold_silver/pokemon_gold.sav
        [ $? -ne 0 ] && exit 1
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 pksav-gen2-save-dump $SAVEDIR/crystal/pokemon_crystal.sav
        [ $? -ne 0 ] && exit 1
    done
fi

echo # So we can check the last Valgrind for an error code
