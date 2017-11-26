#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

sudo apt-get -y update
[ $? -ne 0 ] && exit 1
sudo apt-get -y install cmake cppcheck doxygen
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
