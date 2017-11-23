#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env
cd test-env
[ $? -ne 0 ] && exit 1

# Check source
find $REPO_TOPLEVEL -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
[ $? -ne 0 ] && exit 1

if [ "$NEWCC" != "" ]
then
export CC=$NEWCC
fi

# Build and test
if [ "$NEWCC" = "x86_64-w64-mingw32" ]
then
    cmake -DCMAKE_TOOLCHAIN_FILE=$REPO_TOPLEVEL/cmake/Toolchains/mingw_cross.cmake \
          -DMINGW_PREFIX=x86_64-w64-mingw32 \
          $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
else
    cmake $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1
fi

echo # So we can check the last error code
