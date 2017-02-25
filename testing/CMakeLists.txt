#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(VALGRIND)

# Test dependencies
ADD_SUBDIRECTORY(Unity)

# Tests
ADD_SUBDIRECTORY(unit-tests)
