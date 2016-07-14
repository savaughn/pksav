#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_math_sources "")

IF(PKSAV_ENABLE_GEN1 OR PKSAV_ENABLE_GEN2)
    LIST(APPEND pksav_math_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/base256.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bcd.c
    )
ENDIF(PKSAV_ENABLE_GEN1 OR PKSAV_ENABLE_GEN2)

SET(pksav_math_sources ${pksav_math_sources} PARENT_SCOPE)
