#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/text_common.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/text_common.h
@ONLY)

SET(pksav_common_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/pokedex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pokerus.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stats.c
    ${CMAKE_CURRENT_SOURCE_DIR}/text_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xds_common.c
PARENT_SCOPE)
