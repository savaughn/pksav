#!/bin/bash
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env
cd test-env
[ $? -ne 0 ] && exit 1

cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_WITH_VALGRIND=1 $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
ctest --output-on-failure
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
