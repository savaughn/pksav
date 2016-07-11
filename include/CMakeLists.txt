#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pksav.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/pksav.h
@ONLY)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pksav.h
    DESTINATION ${INCLUDE_DIR}
    COMPONENT Headers
)

ADD_SUBDIRECTORY(pksav)
