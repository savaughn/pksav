#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKSAV_DONT_INSTALL_HEADERS)
    INSTALL(
        FILES pksav.h
        DESTINATION ${INCLUDE_DIR}
        COMPONENT Headers
    )
ENDIF(NOT PKSAV_DONT_INSTALL_HEADERS)

ADD_SUBDIRECTORY(pksav)
