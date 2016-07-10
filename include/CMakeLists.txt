#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INSTALL(
    FILES pksav.h
    DESTINATION ${INCLUDE_DIR}
    COMPONENT Headers
)

ADD_SUBDIRECTORY(pksav)
