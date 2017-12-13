#!/bin/bash
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env/pksav-analysis-build
cd test-env/pksav-analysis-build
[ $? -ne 0 ] && exit 1

echo Running CppCheck.
find $REPO_TOPLEVEL/lib -name '*.[ch]' | xargs cppcheck --enable=performance,portability,warning --std=c99 -I $REPO_TOPLEVEL/include --error-exitcode=1 --force --quiet
#[ $? -ne 0 ] && exit 1

echo Building PKSav with Clang-tidy analysis enabled.
cmake -DCMAKE_BUILD_TYPE=Release \
  -DPKSAV_USE_CLANGTIDY=ON \
  -DCLANG_TIDY_EXECUTABLE=$CLANG_TIDY_EXE \
  $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
