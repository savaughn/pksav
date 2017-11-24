#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_common_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/datetime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pokedex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/prng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sha1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stats.c
    ${CMAKE_CURRENT_SOURCE_DIR}/text_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xds_common.c
PARENT_SCOPE)
