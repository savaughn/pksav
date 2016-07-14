#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PATH(PKSAV_INCLUDE_DIR pksav.h)
FIND_LIBRARY(PKSAV_LIBRARY NAMES pksav)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PKSav DEFAULT_MSG PKSAV_LIBRARY PKSAV_INCLUDE_DIR)
MARK_AS_ADVANCED(PKSAV_LIBRARY PKSAV_INCLUDE_DIR)
