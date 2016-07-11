#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/endian.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/endian.h
@ONLY)

SET(pksav_utils_headers
    ${CMAKE_CURRENT_BINARY_DIR}/endian.h
)

INSTALL(
    FILES ${pksav_utils_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/utils
    COMPONENT Headers
)
