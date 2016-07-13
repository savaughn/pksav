#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_common_sources "")

IF(ENABLE_GEN1 OR ENABLE_GEN2 OR ENABLE_GBA OR ENABLE_NDS)
    LIST(APPEND pksav_common_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/text_common.c
    )
ENDIF(ENABLE_GEN1 OR ENABLE_GEN2 OR ENABLE_GBA OR ENABLE_NDS)

IF(ENABLE_NDS OR ENABLE_GEN6)
    LIST(APPEND pksav_common_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/xds_common.c
    )
ENDIF(ENABLE_NDS OR ENABLE_GEN6)

SET(pksav_common_sources ${pksav_common_sources} PARENT_SCOPE)
