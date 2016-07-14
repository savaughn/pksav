#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_headers
    checksum.h
    config.h
    error.h
    items.h
    prng.h
    stats.h
)

INSTALL(
    FILES ${pksav_headers}
    DESTINATION ${INCLUDE_DIR}/pksav
    COMPONENT Headers
)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(math)

IF(PKSAV_ENABLE_GEN1)
    ADD_SUBDIRECTORY(gen1)
ENDIF(PKSAV_ENABLE_GEN1)
IF(PKSAV_ENABLE_GEN2)
    ADD_SUBDIRECTORY(gen2)
ENDIF(PKSAV_ENABLE_GEN2)
IF(PKSAV_ENABLE_GBA)
    ADD_SUBDIRECTORY(gba)
ENDIF(PKSAV_ENABLE_GBA)
IF(PKSAV_ENABLE_GCN)
    ADD_SUBDIRECTORY(gcn)
ENDIF(PKSAV_ENABLE_GCN)
IF(PKSAV_ENABLE_NDS)
    ADD_SUBDIRECTORY(nds)
ENDIF(PKSAV_ENABLE_NDS)
